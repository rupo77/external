#pragma once

#include "../options.h"
#include "../globals.h"

#include <thread>

inline void MiscLoop()
{
	static auto character = Globals::Roblox::LocalPlayer.Character();
	static auto humanoid = character.FindFirstChildWhichIsA("Humanoid");

	while (true)
	{
		character = Globals::Roblox::LocalPlayer.Character();
		humanoid = character.FindFirstChildWhichIsA("Humanoid");

		humanoid.SetWalkspeed(Options::Misc::Walkspeed);
		humanoid.SetJumpPower(Options::Misc::JumpPower);
		Globals::Roblox::Camera.SetFOV(Options::Misc::FOV);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}