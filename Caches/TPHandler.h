#pragma once
#include "../globals.h"

#include <thread>
#include <vector>


inline void TPHandler()
{
	while (true)
	{
		auto fakeDataModel = Memory->read<uintptr_t>(Memory->getBaseAddress() + offsets::FakeDataModelPointer);
		auto dataModel = RobloxInstance(Memory->read<uintptr_t>(fakeDataModel + offsets::FakeDataModelToDataModel));
		auto placeId = Memory->read<int>(dataModel.address + offsets::PlaceId);
		uintptr_t visualEngine;

		if (!dataModel || dataModel.address == 0 || dataModel.Name() == "LuaApp" || placeId != Globals::Roblox::lastPlaceID) // player left the game
		{

			while (dataModel.Name() != "Ugc")
			{
				fakeDataModel = Memory->read<uintptr_t>(Memory->getBaseAddress() + offsets::FakeDataModelPointer);
				dataModel = RobloxInstance(Memory->read<uintptr_t>(fakeDataModel + offsets::FakeDataModelToDataModel));
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}

			Globals::Roblox::DataModel = dataModel;

			visualEngine = Memory->read<uintptr_t>(Memory->getBaseAddress() + offsets::VisualEnginePointer);

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

			Globals::Roblox::lastPlaceID = placeId;

			Globals::Caches::CachedPlayers.clear();
			Globals::Caches::CachedPlayerObjects.clear();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

