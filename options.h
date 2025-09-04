#pragma once
#include "globals.h"

namespace Options
{
	namespace Misc
	{
		inline bool Bypass;
		inline float Walkspeed = 16.f;
		inline float JumpPower = 50.f;
		inline float FOV = 70.f;
	}
	namespace ESP
	{
		inline bool TeamCheck;
		inline bool TransparencyCheck;
		inline bool Box;
		inline bool Tracers;
		inline int TracersStart;
		inline bool Skeleton;
		inline bool Name;
		inline bool Distance;
		inline bool Health;
		inline bool ESP3D;
		inline bool HeadCircle;
		inline bool CornerESP;

		inline float Color[3];
		inline float BoxColor[3];
		inline float CornerColor[3];
		inline float BoxFillColor[4] = {1.0f, 1.0f, 1.0f, 0.2f};
		inline float SkeletonColor[3];
		inline float DistanceColor[3];
		inline float TracerColor[3];
		inline float TracerThickness = 1.0f;
		inline bool RemoveBorders = false;
		inline float ESP3DColor[3];
		inline float HeadCircleColor[3];
		inline float HeadCircleMaxScale = 2.5f;
	}
	namespace Aimbot
	{
		inline int AimbotKey = VK_RBUTTON;
		inline int AimingType;

		inline int ToggleType;

		inline bool Aimbot;
		inline bool TeamCheck;
		inline bool DownedCheck;
		inline bool StickyAim;
		inline bool TransparencyCheck;
		inline float FOV;
		inline float Smoothness;
		inline bool ShowFOV;
		inline float Range = 100.f;

		inline float FOVColor[3];
		inline float FOVFillColor[4];
		
		inline int TargetBone = 0;
		
		inline RobloxPlayer CurrentTarget;
		inline bool Toggled;
	}
}