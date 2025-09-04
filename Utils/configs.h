#pragma once

#include "../nlohmann/json.hpp"
#include "../options.h"
#include "../globals.h"
#include <fstream>  

using json = nlohmann::json;

inline json CreateConfig(std::string configName)
{
    json j;

    j["ESP"] = 
    {
        { "Team Check", Options::ESP::TeamCheck },
        { "Box", Options::ESP::Box },
        { "Tracers", Options::ESP::Tracers },
        { "TracersStart", Options::ESP::TracersStart },
        { "Skeleton", Options::ESP::Skeleton },
        { "Name", Options::ESP::Name },
        { "Distance", Options::ESP::Distance },
        { "Health", Options::ESP::Health },
        { "3D ESP", Options::ESP::ESP3D },
        { "Tracer Thickness", Options::ESP::TracerThickness },
        { "Head Circles", Options::ESP::HeadCircle },
        { "Remove Borders", Options::ESP::RemoveBorders },

        { "Name Color", Options::ESP::Color },
        { "Box Color", Options::ESP::BoxColor },
        { "Box Fill Color", Options::ESP::BoxFillColor },
        { "Skeleton Color", Options::ESP::SkeletonColor },
        { "Distance Color", Options::ESP::DistanceColor },
        { "Tracers Color", Options::ESP::TracerColor },
        { "3D ESP Color", Options::ESP::ESP3DColor },
        { "Head Circles Color", Options::ESP::HeadCircleColor }
    };

    j["Aimbot"] =
    {
        {"Aimbot Key", Options::Aimbot::AimbotKey},
        {"Aiming Type", Options::Aimbot::AimingType},
        {"Aimbot", Options::Aimbot::Aimbot},
        {"Team Check", Options::Aimbot::TeamCheck},
        {"Downed Check", Options::Aimbot::DownedCheck},
        {"Sticky Aim", Options::Aimbot::StickyAim},
        {"Target Bone", Options::Aimbot::TargetBone},
        {"FOV", Options::Aimbot::FOV},
        {"Show FOV", Options::Aimbot::ShowFOV},
        {"FOV Color", Options::Aimbot::FOVColor},
        {"FOV Fill Color", Options::Aimbot::FOVFillColor},
        {"Smoothness", Options::Aimbot::Smoothness},
        {"Range", Options::Aimbot::Range}
    };

    j["Misc"] =
    {
        {"Walkspeed", Options::Misc::Walkspeed},
        {"JumpPower", Options::Misc::JumpPower},
        {"FOV", Options::Misc::FOV}
    };

    std::ofstream out(Globals::configsPath + "\\" + configName);
    out << j.dump(4);
    out.close();

    return j;
}

inline void LoadConfig(std::string configName)
{
    std::ifstream f(Globals::configsPath + "\\" + configName);
    json data = json::parse(f);

    //ESP Loading

    Options::ESP::TeamCheck = data["ESP"]["Team Check"];
    Options::ESP::Box = data["ESP"]["Box"];
    Options::ESP::Tracers = data["ESP"]["Tracers"];
    Options::ESP::TracersStart = data["ESP"]["TracersStart"];
    Options::ESP::Skeleton = data["ESP"]["Skeleton"];
    Options::ESP::Name = data["ESP"]["Name"];
    Options::ESP::Distance = data["ESP"]["Distance"];
    Options::ESP::Health = data["ESP"]["Health"];
    Options::ESP::ESP3D = data["ESP"]["3D ESP"];
    Options::ESP::TracerThickness = data["ESP"]["Tracer Thickness"];
    Options::ESP::HeadCircle = data["ESP"]["Head Circles"];
    Options::ESP::RemoveBorders = data["ESP"]["Remove Borders"];

    Options::ESP::Color[0] = data["ESP"]["Name Color"][0];
    Options::ESP::Color[1] = data["ESP"]["Name Color"][1];
    Options::ESP::Color[2] = data["ESP"]["Name Color"][2];

    Options::ESP::BoxColor[0] = data["ESP"]["Box Color"][0];
    Options::ESP::BoxColor[1] = data["ESP"]["Box Color"][1];
    Options::ESP::BoxColor[2] = data["ESP"]["Box Color"][2];

    Options::ESP::BoxFillColor[0] = data["ESP"]["Box Fill Color"][0];
    Options::ESP::BoxFillColor[1] = data["ESP"]["Box Fill Color"][1];
    Options::ESP::BoxFillColor[2] = data["ESP"]["Box Fill Color"][2];
    Options::ESP::BoxFillColor[3] = data["ESP"]["Box Fill Color"][3];

    Options::ESP::DistanceColor[0] = data["ESP"]["Distance Color"][0];
    Options::ESP::DistanceColor[1] = data["ESP"]["Distance Color"][1];
    Options::ESP::DistanceColor[2] = data["ESP"]["Distance Color"][2];

    Options::ESP::TracerColor[0] = data["ESP"]["Tracers Color"][0];
    Options::ESP::TracerColor[1] = data["ESP"]["Tracers Color"][1];
    Options::ESP::TracerColor[2] = data["ESP"]["Tracers Color"][2];

    Options::ESP::ESP3DColor[0] = data["ESP"]["3D ESP Color"][0];
    Options::ESP::ESP3DColor[1] = data["ESP"]["3D ESP Color"][1];
    Options::ESP::ESP3DColor[2] = data["ESP"]["3D ESP Color"][2];

    Options::ESP::HeadCircleColor[0] = data["ESP"]["Head Circles Color"][0];
    Options::ESP::HeadCircleColor[1] = data["ESP"]["Head Circles Color"][1];
    Options::ESP::HeadCircleColor[2] = data["ESP"]["Head Circles Color"][2];

    // Aimbot Loading

    Options::Aimbot::AimbotKey = data["Aimbot"]["Aimbot Key"];
    Options::Aimbot::AimingType = data["Aimbot"]["Aiming Type"];
    Options::Aimbot::Aimbot = data["Aimbot"]["Aimbot"];
    Options::Aimbot::TeamCheck = data["Aimbot"]["Team Check"];
    Options::Aimbot::DownedCheck = data["Aimbot"]["Downed Check"];
    Options::Aimbot::StickyAim = data["Aimbot"]["Sticky Aim"];
    Options::Aimbot::TargetBone = data["Aimbot"]["Target Bone"];
    Options::Aimbot::FOV = data["Aimbot"]["FOV"];
    Options::Aimbot::ShowFOV = data["Aimbot"]["Show FOV"];

    Options::Aimbot::FOVColor[0] = data["Aimbot"]["FOV Color"][0];
    Options::Aimbot::FOVColor[1] = data["Aimbot"]["FOV Color"][1];
    Options::Aimbot::FOVColor[2] = data["Aimbot"]["FOV Color"][2];

    Options::Aimbot::FOVFillColor[0] = data["Aimbot"]["FOV Fill Color"][0];
    Options::Aimbot::FOVFillColor[1] = data["Aimbot"]["FOV Fill Color"][1];
    Options::Aimbot::FOVFillColor[2] = data["Aimbot"]["FOV Fill Color"][2];

    Options::Aimbot::Smoothness = data["Aimbot"]["Smoothness"];
    Options::Aimbot::Range = data["Aimbot"]["Range"];

    // Misc Loading

    Options::Misc::Walkspeed = data["Misc"]["Walkspeed"];
    Options::Misc::JumpPower = data["Misc"]["JumpPower"];
    Options::Misc::FOV = data["Misc"]["FOV"];
}