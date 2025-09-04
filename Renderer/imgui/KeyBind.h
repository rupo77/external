#pragma once
#include "imgui.h"
#include <Windows.h>
#include <string>

namespace KeyBind
{
    inline bool Listening = false;
    inline int CurrentBind = 0;

    inline const char* GetKeyName(int vk)
    {
        static char name[128] = {};
        if (vk == 0) return "None";

        UINT scanCode = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);
        if (scanCode == 0) return "Unknown";

        GetKeyNameTextA(scanCode << 16, name, sizeof(name));
        return name;
    }

    inline bool KeyBindButton(const char* label, int* key)
    {
        ImGui::PushID(label);
        ImGui::PushStyleColor(ImGuiCol_Button, Listening ? ImVec4(0.9f, 0.2f, 0.2f, 1.f) : ImGui::GetStyle().Colors[ImGuiCol_Button]);

        std::string btnText = Listening ? "Press a key..." : GetKeyName(*key);
        if (ImGui::Button(btnText.c_str(), ImVec2(100, 0)))
        {
            Listening = !Listening;
            CurrentBind = *key;
        }

        ImGui::PopStyleColor();
        ImGui::PopID();

        if (Listening)
        {
            for (int i = 1; i < 256; ++i)
            {
                if (GetAsyncKeyState(i) & 0x8000)
                {
                    *key = i;
                    Listening = false;
                    return true;
                }
            }
        }

        return false;
    }

    inline bool IsPressed(int key)
    {
        if (key <= 0 || key >= 256)
            return false;

        return (GetAsyncKeyState(key) & 0x8000);
    }
}
