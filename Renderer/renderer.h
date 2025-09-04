#pragma once

#pragma comment (lib, "d3d11.lib")

#pragma comment(lib, "dwmapi.lib")
#include <dwmapi.h>

#include <Windows.h>

#include <d3d11.h>
#include <tchar.h>
#include <algorithm>
#include <string>
#include <thread>
#include <iostream>
#include <filesystem>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/KeyBind.h"
#include "imgui/imgui_settings.h"

#include "font/IconsFontAwesome6.h"

#include "../globals.h"
#include "../options.h"

#include "../Utils/configs.h"

#include "../Hacks/esp.h"
#include "../Hacks/aimbot.h"
#include "W2S.h"

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool IsGameOnTop(const std::string& expectedTitle) {
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) return false;

    char windowTitle[256];
    int length = GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

    if (length == 0) return false;

    return expectedTitle == std::string(windowTitle);
}

void SetTransparency(HWND hwnd, bool boolean)
{
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (boolean)
    {
        exStyle |= WS_EX_TRANSPARENT;
        SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
    }
    else
    {
        exStyle &= ~WS_EX_TRANSPARENT;
        SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
    }
}

void DrawNode(RobloxInstance& node)
{
    const auto& children = node.GetChildren();
    if (children.empty())
    {
        ImGui::BulletText(node.Name().c_str());
    }
    else
    {
        if (ImGui::TreeNode(node.Name().c_str()))
        {
            for (auto child : children)
            {
                DrawNode(child);
            }
            ImGui::TreePop();
        }
    }
}

void ShowImgui()
{
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    size_t width = (size_t)GetSystemMetrics(SM_CXSCREEN);
    size_t height = (size_t)GetSystemMetrics(SM_CYSCREEN);

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);

    HWND hwnd = ::CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        wc.lpszClassName,
        L"FrostWareisUD",
        WS_POPUP,
        0, 0, width + 1, height + 1,
        nullptr, nullptr, wc.hInstance, nullptr);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS Margin = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &Margin);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    // FrostWare Theme Colors (Frost Blue Accents)
    style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.32f, 0.31f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.12f, 0.18f, 0.86f);  // Frost blue tint
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.14f, 0.20f, 0.00f);   // Subtle frost blue
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.12f, 0.18f, 0.86f);   // Frost blue tint
    style.Colors[ImGuiCol_Border] = ImVec4(0.39f, 0.58f, 0.93f, 0.78f);    // Frost blue border
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.16f, 0.22f, 1.00f);   // Frost blue frame
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.20f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.24f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.12f, 0.18f, 1.00f);   // Frost blue title
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.18f, 0.26f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.12f, 0.18f, 0.75f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.12f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.12f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.58f, 0.93f, 1.00f);  // Frost blue
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.66f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.74f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.68f, 0.85f, 0.90f, 1.00f);  // Ice blue checkmark
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.39f, 0.58f, 0.93f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.68f, 0.85f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.16f, 0.22f, 1.00f);        // Frost blue button
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.22f, 0.30f, 1.00f);  // Lighter frost blue
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.28f, 0.38f, 1.00f);   // Active frost blue
    style.Colors[ImGuiCol_Header] = ImVec4(0.12f, 0.16f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.22f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.28f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.58f, 0.93f, 0.78f);      // Frost blue separator
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.47f, 0.66f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.55f, 0.74f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.39f, 0.58f, 0.93f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.47f, 0.66f, 1.00f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.74f, 1.00f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.16f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.39f, 0.58f, 0.93f, 0.80f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.39f, 0.58f, 0.93f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.16f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.22f, 0.30f, 1.00f);

    // Pandora styling
    style.WindowPadding = ImVec2(15.00f, 15.00f);
    style.FramePadding = ImVec2(8.00f, 4.00f);
    style.ItemSpacing = ImVec2(8.00f, 8.00f);
    style.ItemInnerSpacing = ImVec2(8.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.ScrollbarSize = 8.0f;
    style.GrabMinSize = 20;
    style.WindowBorderSize = 0;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 0;
    style.WindowRounding = 6.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    ImFontConfig config;
    config.MergeMode = false;
    config.PixelSnapH = true;

    ImFont* baseFont = io.Fonts->AddFontDefault(&config);

    config.MergeMode = true;

    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

    io.Fonts->AddFontFromFileTTF("fonts/fa-regular-400.ttf", 10.0f, &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF("fonts/fa-solid-900.ttf", 10.0f, &config, icon_ranges);
    io.Fonts->AddFontFromFileTTF("fonts/fa-brands-400.ttf", 10.0f, &config, icon_ranges);

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    ImGui_ImplDX11_CreateDeviceObjects();

    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_SwapChainOccluded && g_pSwapChain->Present(0, 0) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        static bool showMenu = true;
        static bool playerList = false;
        static bool explorer = false;

        static std::string spectatingSubject;

        if (GetAsyncKeyState(VK_INSERT) & 1) // change this if u don't have an insert key
        {
            SetTransparency(hwnd, showMenu);
            showMenu = !showMenu;
        }

        static int category = 0;

        ImGui::SetNextWindowSize(ImVec2(660, 740));

        auto character = Globals::Roblox::LocalPlayer.Character();

        if (showMenu)
        {
            ImGui::Begin("FROSTWARE", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
            
            // Header with icons
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 window_pos = ImGui::GetWindowPos();
            ImVec2 window_size = ImGui::GetWindowSize();
            
            // Draw header background
            draw_list->AddRectFilled(
                ImVec2(window_pos.x, window_pos.y),
                ImVec2(window_pos.x + window_size.x, window_pos.y + 60),
                IM_COL32(28, 29, 32, 220)
            );
            
            // Title
            ImGui::SetCursorPos(ImVec2(window_size.x - 80, 20));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::Text("FROSTWARE");
            ImGui::PopStyleColor();
            
            // Header icons
            ImGui::SetCursorPos(ImVec2(20, 20));
            if (ImGui::Button(ICON_FA_EYE "##general", ImVec2(30, 30))) category = 0;
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_CROSSHAIRS "##aimbot", ImVec2(30, 30))) category = 1;
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_WRENCH "##misc", ImVec2(30, 30))) category = 2;
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_USERS "##players", ImVec2(30, 30))) playerList = !playerList;
            
            ImGui::SetCursorPosY(80);
            
            // Left sidebar for categories
            ImGui::BeginChild("##sidebar", ImVec2(200, window_size.y - 100), true);
            
            // Category buttons with icons
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 10));
            
            if (ImGui::Button(ICON_FA_EYE " GENERAL", ImVec2(-1, 40))) category = 0;
            if (ImGui::Button(ICON_FA_CROSSHAIRS " AIMBOT", ImVec2(-1, 40))) category = 1;
            if (ImGui::Button(ICON_FA_WRENCH " MISCELLANEOUS", ImVec2(-1, 40))) category = 2;
            
            ImGui::Separator();
            
            if (ImGui::Button(ICON_FA_PALETTE " OTHERS", ImVec2(-1, 40))) category = 3;
            if (ImGui::Button(ICON_FA_KEYBOARD " Keybind", ImVec2(-1, 40))) { /* Keybind functionality */ }
            
            ImGui::PopStyleVar(2);
            ImGui::EndChild();
            
            ImGui::SameLine();
            
            // Main content area
            ImGui::BeginChild("##content", ImVec2(0, 0), true);

            switch (category)
            {
                case 0: // General/Visuals
                {
                    ImGui::Text("Checkbox active");
                    ImGui::Checkbox("Team Check", &Options::ESP::TeamCheck);
                    
                    ImGui::Spacing();
                    ImGui::Text("Box ESP");
                    ImGui::Checkbox("Box ESP", &Options::ESP::Box);
                    ImGui::SameLine();
                    ImGui::ColorEdit3("##BoxColor", Options::ESP::BoxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    ImGui::Text("Skeleton ESP");
                    ImGui::Checkbox("Skeleton ESP", &Options::ESP::Skeleton);
                    ImGui::SameLine();
                    ImGui::ColorEdit3("##SkeletonColor", Options::ESP::SkeletonColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    ImGui::Text("Name ESP");
                    ImGui::Checkbox("Name ESP", &Options::ESP::Name);
                    ImGui::SameLine();
                    ImGui::ColorEdit3("##NameColor", Options::ESP::Color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    ImGui::Text("Distance ESP");
                    ImGui::Checkbox("Distance ESP", &Options::ESP::Distance);
                    ImGui::SameLine();
                    ImGui::ColorEdit3("##DistanceColor", Options::ESP::DistanceColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    ImGui::Text("Health Bar");
                    ImGui::Checkbox("Health Bar", &Options::ESP::Health);
                    
                    ImGui::Text("3D ESP");
                    ImGui::Checkbox("3D ESP", &Options::ESP::ESP3D);
                    ImGui::SameLine();
                    ImGui::ColorEdit3("##3DColor", Options::ESP::ESP3DColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    ImGui::Text("Head Circle");
                    ImGui::Checkbox("Head Circle", &Options::ESP::HeadCircle);
                    ImGui::SameLine();
                    ImGui::ColorEdit3("##HeadColor", Options::ESP::HeadCircleColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    ImGui::Text("Tracers");
                    ImGui::Checkbox("Tracers", &Options::ESP::Tracers);
                    ImGui::SameLine();
                    ImGui::ColorEdit3("##TracerColor", Options::ESP::TracerColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    ImGui::Checkbox("Transparency Check", &Options::ESP::TransparencyCheck);

                    ImGui::SliderFloat("Tracer Thickness", &Options::ESP::TracerThickness, 1.0f, 10.0f, "%.1f");

                    static const char* tracersStarts[]{ "Bottom","Top","Mouse","Torso" };

                    ImGui::Combo("Tracers Start", &Options::ESP::TracersStart, tracersStarts, IM_ARRAYSIZE(tracersStarts));
                    break;
                }

                case 1: // Aimbot
                {
                    ImGui::Text("Aimbot");
                    ImGui::Checkbox("Aimbot", &Options::Aimbot::Aimbot);
                    ImGui::SameLine();
                    KeyBind::KeyBindButton("##AimbotKey", &Options::Aimbot::AimbotKey);

                    ImGui::Text("Team Check");
                    ImGui::Checkbox("Team Check", &Options::Aimbot::TeamCheck);

                    ImGui::Text("Toggle Type");
                    static const char* toggleType[]{ "Hold", "Toggle" };
                    if(ImGui::Combo("##ToggleType", &Options::Aimbot::ToggleType, toggleType, IM_ARRAYSIZE(toggleType)))
                    {
                        Options::Aimbot::Toggled = false;
                        Options::Aimbot::CurrentTarget = RobloxPlayer(0);
                    }

                    ImGui::Text("Dead Check");
                    ImGui::Checkbox("Dead Check", &Options::Aimbot::DownedCheck);

                    ImGui::Text("Aiming Method");
                    static const char* aimingMethods[]{ "Camera", "Mouse" };
                    ImGui::Combo("##AimingMethod", &Options::Aimbot::AimingType, aimingMethods, IM_ARRAYSIZE(aimingMethods));

                    ImGui::Text("Target Bone");
                    static const char* targetBones[]{ "Head", "HumanoidRootPart", "Left Arm", "Right Arm", "Left Leg", "Right Leg" };
                    ImGui::Combo("##TargetBone", &Options::Aimbot::TargetBone, targetBones, IM_ARRAYSIZE(targetBones));

                    ImGui::Checkbox("Sticky Aim", &Options::Aimbot::StickyAim);
                    ImGui::Checkbox("Transparency Check", &Options::Aimbot::TransparencyCheck);

                    ImGui::Spacing();
                    ImGui::SliderFloat("Smoothness", &Options::Aimbot::Smoothness, 0.f, 1.f, "%.1f");
                    ImGui::SliderFloat("Range (Studs)", &Options::Aimbot::Range, 1.f, 1000.f, "%.0f");
                    ImGui::SliderFloat("FOV", &Options::Aimbot::FOV, 10.f, 360.f, "%.0f");
                    
                    ImGui::Text("FOV Visual");
                    ImGui::Checkbox("Show FOV", &Options::Aimbot::ShowFOV);
                    ImGui::SameLine();
                    ImGui::ColorEdit3("##FOVColor", Options::Aimbot::FOVColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    break;
                }
                case 2: // Miscellaneous
                {
                    ImGui::SliderFloat("Walkspeed", &Options::Misc::Walkspeed, 0.f, 500.f, "%.0f");
                    ImGui::SliderFloat("JumpPower", &Options::Misc::JumpPower, 0.f, 500.f, "%.0f");
                    ImGui::SliderFloat("FOV", &Options::Misc::FOV, 1.f, 120, "%.0f");
                    break;
                }
                case 3: // Others
                {
                    ImGui::Text("Color Picker");
                    static ImVec4 color = ImVec4(0.77f, 0.73f, 1.00f, 1.00f);
                    ImGui::ColorEdit3("##ColorPicker", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    
                    ImGui::Text("Keybind");
                    ImGui::Button("None", ImVec2(100, 25));
                    
                    ImGui::Spacing();
                    ImGui::Text("Configurations");
                    
                    Globals::configsArray.clear();
                    for (const auto& entry : std::filesystem::directory_iterator(Globals::configsPath))
                    {
                        if (entry.path().extension() == ".json")
                        {
                            Globals::configsArray.push_back(entry.path().filename().string());
                        }
                    }

                    for (auto& config : Globals::configsArray)
                    {
                        ImVec2 cursorPos = ImGui::GetCursorScreenPos();
                        ImVec2 contentRegion = ImGui::GetContentRegionAvail();

                        float height = 30.0f;
                        float paddingX = 8.0f;

                        float buttonWidth = 30.0f;
                        float spacing = 5.0f;
                        float totalButtonWidth = (buttonWidth * 2) + spacing;
                        float labelWidth = contentRegion.x - totalButtonWidth - spacing;

                        ImVec2 rectMin = cursorPos;
                        ImVec2 rectMax = ImVec2(cursorPos.x + labelWidth - 6.f, cursorPos.y + height);

                        ImU32 rectColor = IM_COL32(46, 44, 53, 255);
                        ImU32 textColor = IM_COL32(255, 255, 255, 255);

                        ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, rectColor, 4.0f);

                        ImVec2 textSize = ImGui::CalcTextSize(config.c_str());
                        ImVec2 textPos = ImVec2(
                            rectMin.x + (labelWidth - textSize.x) / 2.0f,
                            rectMin.y + (height - textSize.y) / 2.0f
                        );
                        ImGui::GetWindowDrawList()->AddText(textPos, textColor, config.c_str());

                        ImGui::SetCursorScreenPos(ImVec2(rectMax.x + spacing, rectMin.y));

                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.f, 6.f));

                        std::string loadId = std::string(ICON_FA_RECYCLE) + "##load" + config;
                        std::string deleteId = std::string(ICON_FA_XMARK) + "##delete" + config;

                        if (ImGui::Button(loadId.c_str(), ImVec2(buttonWidth, height)))
                        {
                            LoadConfig(config);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button(deleteId.c_str(), ImVec2(buttonWidth, height)))
                        {
                            std::filesystem::remove(Globals::configsPath + "\\" + config);
                        }

                        ImGui::PopStyleVar();

                        ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, rectMax.y + 5.0f));
                        ImGui::Dummy(ImVec2(1, 1));
                    }

                    static char buf[256] = "";

                    ImGui::InputText("##SaveIconInput", buf, sizeof(buf));
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_FA_FLOPPY_DISK))
                        CreateConfig(std::string(static_cast<std::string>(buf) + ".json"));

                    ImGui::SetNextItemWidth(120);
                    break;
                }
                default:
                    break;
            }
            
            ImGui::EndChild();
            ImGui::End();
        }
        if (IsGameOnTop("Roblox"))
        {
            auto displaySize = io.DisplaySize;

            // Watermark

            std::string str = "FrostWare | " + std::to_string(static_cast<int>(io.Framerate)) + " FPS";

            ImVec2 textSize = ImGui::CalcTextSize(str.c_str());

            ImVec2 pos = ImVec2(displaySize.x - textSize.x - 10.0f, 10.0f);

            ImDrawList* drawList = ImGui::GetBackgroundDrawList();
            ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
            ImU32 textColor = IM_COL32(0, 150, 255, 255);

            drawList->AddText(ImVec2(pos.x - 1, pos.y - 1), outlineColor, str.c_str());
            drawList->AddText(ImVec2(pos.x + 1, pos.y - 1), outlineColor, str.c_str());
            drawList->AddText(ImVec2(pos.x - 1, pos.y + 1), outlineColor, str.c_str());
            drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), outlineColor, str.c_str());

            drawList->AddText(pos, textColor, str.c_str());
        }

        ImGui::SetNextWindowSize(ImVec2(500, 440));

        if (showMenu && explorer)
        {
            ImGui::Begin("Explorer", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
            for (auto& child : Globals::Roblox::DataModel.GetChildren())
            {
                DrawNode(child);
            }
            ImGui::End();
        }

        ImGui::SetNextWindowSize(ImVec2(500, 440));

        if (playerList && showMenu)
        {
            ImGui::Begin("Player List", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

            for (auto& player : Globals::Caches::CachedPlayerObjects)
            {
                ImGui::BulletText(player.Name.c_str());
                ImGui::SameLine();

                std::string teleportID = "Teleport##" + player.Name;
                if (ImGui::Button(teleportID.c_str()))
                {
                    auto localCFrame = character.FindFirstChild("HumanoidRootPart").CFrame();
                    auto playerCFrame = player.Character.FindFirstChild("HumanoidRootPart").CFrame();

                    localCFrame.x = playerCFrame.x;
                    localCFrame.y = playerCFrame.y;
                    localCFrame.z = playerCFrame.z;

                    auto primitive = Memory->read<uintptr_t>(character.FindFirstChild("HumanoidRootPart").address + offsets::Primitive);

                    std::thread([=]() {
                        for (auto i = 0; i != 100; i++)
                            Memory->write<Vectors::Vector3>(primitive + offsets::Position, {localCFrame.x, localCFrame.y, localCFrame.z});
                        }).detach(); // you gotta run it in a thread or it crashes cuz it times out ImGui
                }

                ImGui::SameLine();

                std::string spectateID = "Spectate##" + player.Name;
                if (ImGui::Button(spectateID.c_str()))
                {
                    if (spectatingSubject == player.Name)
                    {
                        spectatingSubject = "";
                        Memory->write<uintptr_t>(Globals::Roblox::Camera.address + offsets::CameraSubject, character.FindFirstChildWhichIsA("Humanoid").address);
                    }
                    else
                    {
                        Memory->write<uintptr_t>(Globals::Roblox::Camera.address + offsets::CameraSubject, player.Humanoid.address);
                        spectatingSubject = player.Name;
                    }
                }
            }
            ImGui::End();
        }

        if (Options::ESP::Box || Options::ESP::Skeleton || Options::ESP::Tracers || Options::ESP::Name ||
            Options::ESP::Distance || Options::ESP::Health || Options::ESP::ESP3D)
        {
            if (!showMenu && IsGameOnTop("Roblox"))
                RenderESP(ImGui::GetBackgroundDrawList());
        }

        if (Options::Aimbot::Aimbot)
        {
            if (!showMenu && IsGameOnTop("Roblox"))
                RunAimbot(ImGui::GetBackgroundDrawList());
        }

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        HRESULT hr = g_pSwapChain->Present(0, 0);
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}


bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 4;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
