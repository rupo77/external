#pragma once
#include "../Utils/structures.h"
#include "../Offsets/offsets.h"
#include "../globals.h"
#include "imgui/imgui.h"	

inline Vectors::Vector2 WorldToScreen(Vectors::Vector3 world)
{
    Matrixes::Matrix4 viewmatrix = Memory->read<Matrixes::Matrix4>(Globals::Roblox::VisualEngine + offsets::viewmatrix);

    Vectors::Vector4 quaternion;

    quaternion.x = (world.x * viewmatrix.data[0]) + (world.y * viewmatrix.data[1]) + (world.z * viewmatrix.data[2]) + viewmatrix.data[3];
    quaternion.y = (world.x * viewmatrix.data[4]) + (world.y * viewmatrix.data[5]) + (world.z * viewmatrix.data[6]) + viewmatrix.data[7];
    quaternion.z = (world.x * viewmatrix.data[8]) + (world.y * viewmatrix.data[9]) + (world.z * viewmatrix.data[10]) + viewmatrix.data[11];
    quaternion.w = (world.x * viewmatrix.data[12]) + (world.y * viewmatrix.data[13]) + (world.z * viewmatrix.data[14]) + viewmatrix.data[15];

    if (quaternion.w < 0.1f)
        return{ -1, -1 };

    float inv_w = 1.0f / quaternion.w;
    Vectors::Vector3 ndc;
    ndc.x = quaternion.x * inv_w;
    ndc.y = quaternion.y * inv_w;
    ndc.z = quaternion.z * inv_w;

    HWND hwnd = FindWindowA(NULL, "Roblox");

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int width = clientRect.right;
    int height = clientRect.bottom;

    POINT point = { 0, 0 };
    ClientToScreen(hwnd, &point);

    Vectors::Vector2 Dimensions = { (float)width, (float)height };

    return
    {
        ((Dimensions.x / 2.0f) * ndc.x + (Dimensions.x / 2.0f)) + point.x,
        (-(Dimensions.y / 2.0f) * ndc.y + (Dimensions.y / 2.0f)) + point.y
    };
}