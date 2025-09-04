

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
    if (!IsGameRunning(L"Roblox"))
    {
        log("Roblox not found!", 2);
        log("Waiting for Roblox...", 0);
        while (!IsGameRunning(L"Roblox"))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    log("Roblox found!", 1);

    log("Attaching to Roblox...", 0);
    if (!Memory->attachToProcess("RobloxPlayerBeta.exe"))
    {
        log("Failed to attach dm rupo!", 2);
        log("Press any key to exit...", 0);
        std::cin.get();
        return -1;
    }

    log("Succesfully attached FrostWare!", 1);

    if (Memory->getProcessId("RobloxPlayerBeta.exe") == 0)
    {
        log("Failed to get Roblox's PID!", 2);
        log("Press any key to exit...", 0);
        std::cin.get();
        return -1;
    }

    log(std::string("Roblox PID -> " + std::to_string(Memory->getProcessId())), 1);
    log(std::string("Roblox Base Address -> 0x" + toHexString(std::to_string(Memory->getBaseAddress()), false, true)), 1);

    Globals::executablePath = GetExecutableDir();

    std::string fontsFolderPath = Globals::executablePath + "\\fonts";

    struct stat buffer;
    if (stat(fontsFolderPath.c_str(), &buffer) != 0)
    {
        log("Failed to find fonts folder!", 2);
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

    log(std::string("DataModel -> 0x" + toHexString(std::to_string(Globals::Roblox::DataModel.address), false, true)), 1);
    log(std::string("VisualEngine -> 0x" + toHexString(std::to_string(Globals::Roblox::VisualEngine), false, true)), 1);

    log(std::string("Workspace -> 0x" + toHexString(std::to_string(Globals::Roblox::Workspace.address), false, true)), 1);
    log(std::string("Players -> 0x" + toHexString(std::to_string(Globals::Roblox::Players.address), false, true)), 1);
    log(std::string("Camera -> 0x" + toHexString(std::to_string(Globals::Roblox::Camera.address), false, true)), 1);

    log(std::string("Logged in as " + Globals::Roblox::LocalPlayer.Name()), 1);

    std::thread(ShowImgui).detach();
    std::thread(CachePlayers).detach();
    std::thread(CachePlayerObjects).detach();
    std::thread(TPHandler).detach();
    std::thread(MiscLoop).detach();

    std::cin.get();

    return 1;
}