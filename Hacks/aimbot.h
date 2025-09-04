#pragma once

#include <algorithm>
#include <cmath>

#include "../renderer/W2S.h"
#include "../renderer/imgui/imgui.h"

#include "../options.h"
#include "../Renderer/imgui/KeyBind.h"

inline Vectors::Vector3 GetTargetPosition(const RobloxPlayer& player)
{
    switch (Options::Aimbot::TargetBone)
    {
        case 0: // Head
            return player.Head.Position();
        case 1: // HumanoidRootPart
            return player.HumanoidRootPart.Position();
        case 2: // Left Arm
            if (player.RigType == 0) // R6
                return player.Left_Arm.Position();
            else // R15
                return player.Left_Hand.Position();
        case 3: // Right Arm
            if (player.RigType == 0) // R6
                return player.Right_Arm.Position();
            else // R15
                return player.Right_Hand.Position();
        case 4: // Left Leg
            if (player.RigType == 0) // R6
                return player.Left_Leg.Position();
            else // R15
                return player.Left_Foot.Position();
        case 5: // Right Leg
            if (player.RigType == 0) // R6
                return player.Right_Leg.Position();
            else // R15
                return player.Right_Foot.Position();
        default:
            return player.Head.Position(); // Default to head
    }
}

inline RobloxPlayer GetClosestPlayer()
{
    RobloxPlayer target;
    auto maxDistance = FLT_MAX;
    auto localTeam = Globals::Roblox::LocalPlayer.Team();
    auto localCharacter = Globals::Roblox::LocalPlayer.Character();
    auto localHRP = localCharacter.FindFirstChild("HumanoidRootPart");

    POINT p;
    GetCursorPos(&p);

    for (auto& player : Globals::Caches::CachedPlayerObjects)
    {
        auto HRP = player.HumanoidRootPart;
        if (!HRP.address)
            continue;

        if (player.address == Globals::Roblox::LocalPlayer.address)
            continue;

        if (player.Team.address == localTeam.address && Options::Aimbot::TeamCheck)
            continue;

        if (player.Health == 0)
            continue;

        if (player.Health <= 4 && Options::Aimbot::DownedCheck)
            continue;

        if (player.Head.Transparency() >= 1.f && Options::Aimbot::TransparencyCheck)
            continue;

        auto targetPos = GetTargetPosition(player);
        auto targetPos2D = WorldToScreen(targetPos);

        if (targetPos2D.x == -1 && targetPos2D.y == -1)
            continue;

        // Check 3D distance in studs
        Vectors::Vector3 diff = localHRP.Position() - targetPos;
        float distance3D = diff.Magnitude();
        
        if (distance3D > Options::Aimbot::Range)
            continue;

        auto distance = targetPos2D.Distance({ static_cast<float>(p.x), static_cast<float>(p.y) });

        if (distance < maxDistance && distance <= Options::Aimbot::FOV)
        {
            maxDistance = distance;
            target = player;
        }
    }
    return target;
}

inline void CameraRotation(const RobloxPlayer& target)
{
    Matrixes::Matrix3x3 currentRotation = Memory->read<Matrixes::Matrix3x3>(Globals::Roblox::Camera.address + offsets::CameraRotation);

    sCFrame cameraCFrame = Globals::Roblox::Camera.CFrame();
    Vectors::Vector3 camPos = Memory->read<Vectors::Vector3>(Globals::Roblox::Camera.address + offsets::CameraPos);

    sCFrame lookAtCFrame = LookAt(camPos, GetTargetPosition(target));

    Vectors::Vector3 rightVec = lookAtCFrame.GetRightVector();
    Vectors::Vector3 upVec = lookAtCFrame.GetUpVector();
    Vectors::Vector3 lookVec = lookAtCFrame.GetLookVector();

    Matrixes::Matrix3x3 rotationMatrix
    {
        rightVec.x, upVec.x, lookVec.x,
        rightVec.y, upVec.y, lookVec.y,
        rightVec.z, upVec.z, lookVec.z
    };

    Vectors::Vector4 currentQuat = Vectors::Vector4::FromMatrix(currentRotation);
    Vectors::Vector4 targetQuat = Vectors::Vector4::FromMatrix(rotationMatrix);

    float smooth = Options::Aimbot::Smoothness;
    float t = 1.0f - smooth;
    t = t * t * t * t;

    if (smooth > 0 && t < 0.01f)
    {
        t = 0.01f;
    }

    Vectors::Vector4 smoothedQuat = Vectors::Vector4::Slerp(currentQuat, targetQuat, t);
    Matrixes::Matrix3x3 smoothedMatrix = smoothedQuat.ToMatrix();

    Memory->write<Matrixes::Matrix3x3>(Globals::Roblox::Camera.address + offsets::CameraRotation, smoothedMatrix);
}

inline void Mouse(const Vectors::Vector2& targetPos, const POINT& p)
{
    static float accumulatedX = 0.0f;
    static float accumulatedY = 0.0f;

    float dx = static_cast<float>(targetPos.x - p.x);
    float dy = static_cast<float>(targetPos.y - p.y);

    float smooth = Options::Aimbot::Smoothness;
    float t = std::clamp<float>(1.0f - smooth, 0.01f, 1.0f);

    float moveX = dx * t;
    float moveY = dy * t;

    accumulatedX += moveX;
    accumulatedY += moveY;

    int intMoveX = static_cast<int>(accumulatedX);
    int intMoveY = static_cast<int>(accumulatedY);

    accumulatedX -= intMoveX;
    accumulatedY -= intMoveY;

    if (intMoveX != 0 || intMoveY != 0)
    {
        SetCursorPos(p.x + intMoveX, p.y + intMoveY);
    }
}

inline void MouseSendInput(const Vectors::Vector2& targetPos, const POINT& currentPos, float sensitivity)
{
    if (currentPos.x == targetPos.x && currentPos.y == targetPos.y)
        return;

    static float accumulatedX = 0.0f;
    static float accumulatedY = 0.0f;

    float dx = static_cast<float>(targetPos.x - currentPos.x);
    float dy = static_cast<float>(targetPos.y - currentPos.y);

    auto smoothness = std::clamp<float>(Options::Aimbot::Smoothness, 0, 0.8);
    // the mouse barely move at smoothness 0.9 and 1 and i'm too lazy to fix so we just manually make it 0.8 :D

    float smooth = std::clamp(smoothness, 0.0f, 0.99f);
    float t = 1.0f - smooth;

    float sensitivityScale = 1.0f / (sensitivity + 0.2f);
    float speedScale = 0.01f;

    float moveX = dx * t * sensitivityScale * speedScale;
    float moveY = dy * t * sensitivityScale * speedScale;

    accumulatedX += moveX;
    accumulatedY += moveY;

    int intMoveX = static_cast<int>(accumulatedX);
    int intMoveY = static_cast<int>(accumulatedY);

    if (std::abs(dx) < 1.0f && std::abs(dy) < 1.0f)
    {
        accumulatedX = 0.0f;
        accumulatedY = 0.0f;
        return;
    }

    accumulatedX -= intMoveX;
    accumulatedY -= intMoveY;

    if (intMoveX != 0 || intMoveY != 0)
    {
        INPUT input = {};
        input.type = INPUT_MOUSE;
        input.mi.dx = intMoveX;
        input.mi.dy = intMoveY;
        input.mi.dwFlags = MOUSEEVENTF_MOVE;

        SendInput(1, &input, sizeof(INPUT));
    }
}

inline void RunAimbot(ImDrawList* drawList)
{
    auto localTeam = Globals::Roblox::LocalPlayer.Team();
    auto localCharacter = Globals::Roblox::LocalPlayer.Character();
    auto localHRP = localCharacter.FindFirstChild("HumanoidRootPart");
    auto Dimensions = Memory->read<Vectors::Vector2>(Globals::Roblox::VisualEngine + offsets::Dimensions);

    if (Globals::Caches::CachedPlayerObjects.empty())
        return;

    POINT p;
    GetCursorPos(&p);

    HWND robloxWindow = FindWindowA("Roblox", nullptr);
    if (robloxWindow)
    {
        ScreenToClient(robloxWindow, &p);
    }

    int CombatType; // FPS: 0 TPS: 1
    
    bool yAxisCheck;

    if (Dimensions.x < GetSystemMetrics(SM_CXSCREEN) || Dimensions.y < GetSystemMetrics(SM_CYSCREEN))
    {
        yAxisCheck = (p.y - Dimensions.y / 2) <= 25; // windowed mode
    }
    else
    {
        yAxisCheck = p.y == Dimensions.y / 2;
    }

    if (p.x == Dimensions.x / 2 && yAxisCheck) //if cursor is in the very middle of the screen then they're most
    {                                          //likely in first person
        CombatType = 0; // FPS
    }
    else
    {
        CombatType = 1; // TPS
    }

    ImColor FOVColor = IM_COL32(
        static_cast<int>(Options::Aimbot::FOVColor[0] * 255.f),
        static_cast<int>(Options::Aimbot::FOVColor[1] * 255.f),
        static_cast<int>(Options::Aimbot::FOVColor[2] * 255.f),
        255);

    ImColor FOVFillColor = IM_COL32(
        static_cast<int>(Options::Aimbot::FOVFillColor[0] * 255.f),
        static_cast<int>(Options::Aimbot::FOVFillColor[1] * 255.f),
        static_cast<int>(Options::Aimbot::FOVFillColor[2] * 255.f),
        static_cast<int>(Options::Aimbot::FOVFillColor[3] * 255.f));

    if (Options::Aimbot::FOV && Options::Aimbot::ShowFOV)
    {
        drawList->AddCircle(ImVec2(p.x, p.y), Options::Aimbot::FOV, FOVColor, 0, 2);
        drawList->AddCircleFilled(ImVec2(p.x, p.y), Options::Aimbot::FOV, FOVFillColor, 0);
    }

    if (KeyBind::IsPressed(Options::Aimbot::AimbotKey) && Options::Aimbot::ToggleType == 1)
    {
        Options::Aimbot::Toggled = !Options::Aimbot::Toggled;
    }

    if (!KeyBind::IsPressed(Options::Aimbot::AimbotKey) && !Options::Aimbot::Toggled)
    {
        Options::Aimbot::CurrentTarget = RobloxPlayer(0);
        return;
    }

    RobloxPlayer target;
    if (Options::Aimbot::StickyAim)
    {
        if (Options::Aimbot::CurrentTarget.address == 0 ||
            Options::Aimbot::CurrentTarget.Health == 0 ||
            (Options::Aimbot::CurrentTarget.Health <= 4 && Options::Aimbot::DownedCheck) ||
            (Options::Aimbot::CurrentTarget.Team.address == localTeam.address && Options::Aimbot::TeamCheck) ||
            Options::Aimbot::CurrentTarget.Head.Transparency() >= 1.f && Options::Aimbot::TransparencyCheck)
        {
            Options::Aimbot::CurrentTarget = GetClosestPlayer();
        }
        else
        {
            // Check if current target is still within range
            auto targetPos = GetTargetPosition(Options::Aimbot::CurrentTarget);
            Vectors::Vector3 diff = targetPos - localHRP.Position();
            float distance3D = diff.Magnitude();
            
            if (distance3D > Options::Aimbot::Range)
            {
                Options::Aimbot::CurrentTarget = GetClosestPlayer();
            }
        }

        target = Options::Aimbot::CurrentTarget;
    }
    else
    {
        target = GetClosestPlayer();
    }

    auto sensitivity = Memory->read<float>(Memory->getBaseAddress() + offsets::MouseSensitivity);

    if (target.address != 0)
    {
        auto targetPos = WorldToScreen(GetTargetPosition(target));

        if (targetPos.x != -1 && targetPos.y != -1)
        {
            switch (CombatType)
            {
            case 0:
            {
                switch(Options::Aimbot::AimingType)
                {
                    case 0: // Camera
                    {
                        CameraRotation(target);
                        break;
                    }
                    case 1: // Mouse
                    {
                        MouseSendInput(targetPos, p, sensitivity);
                        break;
                    }
                }
                break;
            }

            case 1:
            {
                Mouse(targetPos, p);
                break;
            }

            default:
                break;
            }

        }
    }
}



