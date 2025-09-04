

#include <iostream>
#include <thread>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include "Memory/MemoryManager.h" 

#include "Utils/colors.h"
#include "Utils/console.h"

#include "Renderer/renderer.h"

#include "Hacks/misc.h"

#include "Caches/playercache.h"
#include "Caches/playerobjectscache.h"
#include "Caches/TPHandler.h"

#include "globals.h"

bool IsGameRunning(const wchar_t* windowTitle)
{
	HWND hwnd = FindWindowW(NULL, windowTitle);
	return hwnd != NULL;
}

std::string GetExecutableDir()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::filesystem::path exePath(path);
    return exePath.parent_path().string();
}

int main()
{
    print_frostware_banner();
    if (!IsGameRunning(L"Roblox"))
    {
        log("Roblox is not running!", 2);
        log("Please start Roblox and wait...", 0);
        while (!IsGameRunning(L"Roblox"))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    log("Roblox detected successfully!", 1);

    log("Connecting to Roblox process...", 3);
    if (!Memory->attachToProcess("RobloxPlayerBeta.exe"))
    {
        log("Failed to connect to Roblox process!", 2);
        log("Press any key to exit...", 0);
        std::cin.get();
        return -1;
    }

    log("FrostWare successfully injected!", 3);

    if (Memory->getProcessId("RobloxPlayerBeta.exe") == 0)
    {
        log("Unable to retrieve Roblox process ID!", 2);
        log("Press any key to exit...", 0);
        std::cin.get();
        return -1;
    }

    log(std::string("Process ID: " + std::to_string(Memory->getProcessId())), 3);
    log(std::string("Base Address: 0x" + toHexString(std::to_string(Memory->getBaseAddress()), false, true)), 3);

    Globals::executablePath = GetExecutableDir();

    std::string fontsFolderPath = Globals::executablePath + "\\fonts";

    struct stat buffer;
    if (stat(fontsFolderPath.c_str(), &buffer) != 0)
    {
        log("Fonts directory not found! Please ensure fonts folder exists.", 2);
        log("Press any key to exit...", 0);
        std::cin.get();
        return -1;
    }

    Globals::configsPath = Globals::executablePath + "\\configs";

    if (stat(Globals::configsPath.c_str(), &buffer) != 0)
    {
        std::filesystem::create_directory(Globals::configsPath);
    }

    auto fakeDataModel = Memory->read<uintptr_t>(Memory->getBaseAddress() + offsets::FakeDataModelPointer);
    auto dataModel = RobloxInstance(Memory->read<uintptr_t>(fakeDataModel + offsets::FakeDataModelToDataModel));

    while (dataModel.Name() != "Ugc")
    {
        fakeDataModel = Memory->read<uintptr_t>(Memory->getBaseAddress() + offsets::FakeDataModelPointer);
        dataModel = RobloxInstance(Memory->read<uintptr_t>(fakeDataModel + offsets::FakeDataModelToDataModel));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    Globals::Roblox::DataModel = dataModel;

    auto visualEngine = Memory->read<uintptr_t>(Memory->getBaseAddress() + offsets::VisualEnginePointer);

    while (visualEngine == 0)
    {
        visualEngine = Memory->read<uintptr_t>(Memory->getBaseAddress() + offsets::VisualEnginePointer);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    Globals::Roblox::VisualEngine = visualEngine;

    Globals::Roblox::Workspace = Globals::Roblox::DataModel.FindFirstChildWhichIsA("Workspace");
    Globals::Roblox::Players = Globals::Roblox::DataModel.FindFirstChildWhichIsA("Players");
    Globals::Roblox::Camera = Globals::Roblox::Workspace.FindFirstChildWhichIsA("Camera");

    Globals::Roblox::LocalPlayer = RobloxInstance(Memory->read<uintptr_t>(Globals::Roblox::Players.address + offsets::LocalPlayer));

    Globals::Roblox::lastPlaceID = Memory->read<int>(Globals::Roblox::DataModel.address + offsets::PlaceId);;

    log(std::string("Game DataModel: 0x" + toHexString(std::to_string(Globals::Roblox::DataModel.address), false, true)), 3);
    log(std::string("Rendering Engine: 0x" + toHexString(std::to_string(Globals::Roblox::VisualEngine), false, true)), 3);

    log(std::string("Game Workspace: 0x" + toHexString(std::to_string(Globals::Roblox::Workspace.address), false, true)), 3);
    log(std::string("Players Service: 0x" + toHexString(std::to_string(Globals::Roblox::Players.address), false, true)), 3);
    log(std::string("Game Camera: 0x" + toHexString(std::to_string(Globals::Roblox::Camera.address), false, true)), 3);

    log(std::string("Welcome, " + Globals::Roblox::LocalPlayer.Name() + "! FrostWare is ready."), 3);

    std::thread(ShowImgui).detach();
    std::thread(CachePlayers).detach();
    std::thread(CachePlayerObjects).detach();
    std::thread(TPHandler).detach();
    std::thread(MiscLoop).detach();

    std::cin.get();

    return 1;
}