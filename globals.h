#pragma once
#include <Windows.h>
#include <cstdint>
#include <memory>

#include "Memory/MemoryManager.h"
#include "SDK/sdk.h"

struct RobloxPlayer
{
    uintptr_t address = 0;

    int RigType = 0;

    std::string Name = "";
    float Health = 0.0f;
    float MaxHealth = 0.0f;
    RobloxInstance Team = RobloxInstance(0);

    RobloxInstance Character = RobloxInstance(0);
    RobloxInstance Humanoid = RobloxInstance(0);

    // R6
    RobloxInstance Head = RobloxInstance(0);
    RobloxInstance HumanoidRootPart = RobloxInstance(0);

    RobloxInstance Left_Arm = RobloxInstance(0);
    RobloxInstance Left_Leg = RobloxInstance(0);

    RobloxInstance Right_Arm = RobloxInstance(0);
    RobloxInstance Right_Leg = RobloxInstance(0);

    RobloxInstance Torso = RobloxInstance(0);

    // R15
    RobloxInstance Upper_Torso = RobloxInstance(0);
    RobloxInstance Lower_Torso = RobloxInstance(0);

    RobloxInstance Right_Upper_Arm = RobloxInstance(0);
    RobloxInstance Right_Lower_Arm = RobloxInstance(0);
    RobloxInstance Right_Hand = RobloxInstance(0);

    RobloxInstance Left_Upper_Arm = RobloxInstance(0);
    RobloxInstance Left_Lower_Arm = RobloxInstance(0);
    RobloxInstance Left_Hand = RobloxInstance(0);

    RobloxInstance Right_Upper_Leg = RobloxInstance(0);
    RobloxInstance Right_Lower_Leg = RobloxInstance(0);
    RobloxInstance Right_Foot = RobloxInstance(0);

    RobloxInstance Left_Upper_Leg = RobloxInstance(0);
    RobloxInstance Left_Lower_Leg = RobloxInstance(0);
    RobloxInstance Left_Foot = RobloxInstance(0);
}; // i know this is long but it's for optimization ok? we're gonna cache this every now and then

namespace Globals
{
	namespace Roblox
	{
		inline RobloxInstance DataModel(0);
		inline uintptr_t VisualEngine; // doesn't make sense to store it as a RobloxInstance

		inline RobloxInstance Workspace(0);
		inline RobloxInstance Players(0);
		inline RobloxInstance Camera(0);

		inline RobloxInstance LocalPlayer(0);

		inline int lastPlaceID;
	}
	namespace Caches
	{
		inline std::vector<RobloxInstance> CachedPlayers;
		inline std::vector<RobloxPlayer> CachedPlayerObjects;
	}
    inline std::string executablePath;
    inline std::string configsPath;
    inline std::vector<std::string> configsArray;
}