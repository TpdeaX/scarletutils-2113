#include "menu.hpp"
#include "../includes.hpp"
#include "../config.hpp"
#include <imgui.h>
#include <algorithm>

namespace GUI {

void setupStyle() {
    ImGuiStyle &style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.4f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 4.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.ChildRounding = 4.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 4.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(5.0f, 2.0f);
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;
    style.ItemSpacing = ImVec2(6.0f, 6.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
    style.CellPadding = ImVec2(6.0f, 6.0f);
    style.IndentSpacing = 25.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 4.0f;
    style.TabBorderSize = 1.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(0.8583691f, 0.8583605f, 0.8583605f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.1882353f, 0.1882353f, 0.1882353f, 0.5f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.047058824f, 0.047058824f, 0.047058824f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1882353f, 0.1882353f, 0.1882353f, 0.54f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2f, 0.21960784f, 0.22745098f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05882353f, 0.05882353f, 0.05882353f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.13725491f, 0.13725491f, 0.13725491f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.047058824f, 0.047058824f, 0.047058824f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3372549f, 0.3372549f, 0.3372549f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5568628f, 0.5568628f, 0.5568628f, 0.54f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7424893f, 0.14658587f, 0.2540021f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.3372549f, 0.3372549f, 0.3372549f, 0.54f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5568628f, 0.5568628f, 0.5568628f, 0.54f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.047058824f, 0.047058824f, 0.047058824f, 0.54f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1882353f, 0.1882353f, 0.1882353f, 0.54f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.21960784f, 0.22745098f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 0.52f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.36f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2f, 0.21960784f, 0.22745098f, 0.33f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.2784314f, 0.2784314f, 0.2784314f, 0.29f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4392157f, 0.4392157f, 0.4392157f, 0.29f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.4f, 0.4392157f, 0.46666667f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2784314f, 0.2784314f, 0.2784314f, 0.29f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4392157f, 0.4392157f, 0.4392157f, 0.29f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4f, 0.4392157f, 0.46666667f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.0f, 0.0f, 0.52f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.13725491f, 0.13725491f, 0.13725491f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.2f, 0.2f, 0.2f, 0.36f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.0f, 0.0f, 0.52f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13725491f, 0.13725491f, 0.13725491f, 1.0f);
}

void render() {
    if (!menuVisible) return;

    ImGui::SetNextWindowSize(ImVec2(360, 480), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Scarlet Utils (GD 2.113)", &menuVisible,
                    ImGuiWindowFlags_NoCollapse)) {

        if (ImGui::BeginTabBar("main_tabbar")) {

            // TAB 1: GAMEPLAY
            if (ImGui::BeginTabItem("Gameplay")) {

                ImGui::Checkbox("Flip Input On Death", &flipOnDeath);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Flips player input automatically after respawning.");
                    ImGui::EndTooltip();
                }

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_flip_opts", ImGuiDir_Right))
                    ImGui::OpenPopup("flipOnDeath options");

                if (ImGui::BeginPopup("flipOnDeath options")) {
                    ImGui::Checkbox("Player 1##flipOnDeath", &flipOnDeathP1);
                    ImGui::Checkbox("Player 2##flipOnDeath", &flipOnDeathP2);
                    ImGui::Checkbox("Click Both##flipOnDeath", &flipOnDeathBoth);
                    ImGui::Checkbox("Swift##flipOnDeath", &flipOnDeathSwift);
                    ImGui::Checkbox("Unfreeze##flipOnDeath", &flipOnDeathUnfreeze);
                    if (ImGui::IsItemHovered()) {
                        ImGui::BeginTooltip();
                        ImGui::Text("Requires frame advance toggle keybind to be set to V.");
                        ImGui::EndTooltip();
                    }
                    ImGui::EndPopup();
                }

                ImGui::Checkbox("Click Green Dash Orbs", &clickGreenDash);
                ImGui::Checkbox("Click Black Orbs", &clickBlackOrbs);

                ImGui::Checkbox("Easy Black Orb UFO", &blackOrbUfo);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Won't click black orbs if going up (or down depending on gravity).");
                    ImGui::EndTooltip();
                }

                ImGui::Checkbox("Click Jump Pads", &clickJumpPads);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("When hitting pink, yellow or red pads, automatically swift click.");
                    ImGui::EndTooltip();
                }

                ImGui::Checkbox("Click Gravity Pads", &clickGravityPads);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("When hitting blue pads, automatically swift click.");
                    ImGui::EndTooltip();
                }

                ImGui::Checkbox("Maintain Gravity", &maintainGravity);

                ImGui::Checkbox("Auto Straight Fly", &straightFly);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Attempts to stabilize your ship.");
                    ImGui::EndTooltip();
                }

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_ship_opts", ImGuiDir_Right))
                    ImGui::OpenPopup("straight fly options");

                if (ImGui::BeginPopup("straight fly options")) {
                    if (ImGui::Checkbox("Player 1##ship", &straightFlyP1)) Config::save();

                    ImGui::PushItemWidth(100.f);
                    if (ImGui::InputDouble("Threshold##p1ship", &straightFlyThresholdP1, 0.0, 0.0, "%.2f")) {
                        straightFlyThresholdP1 = std::max(straightFlyThresholdP1, 0.0);
                        Config::save();
                    }

                    if (ImGui::Checkbox("Player 2##ship", &straightFlyP2)) Config::save();

                    if (ImGui::InputDouble("Threshold##p2ship", &straightFlyThresholdP2, 0.0, 0.0, "%.2f")) {
                        straightFlyThresholdP2 = std::max(straightFlyThresholdP2, 0.0);
                        Config::save();
                    }
                    ImGui::PopItemWidth();
                    ImGui::EndPopup();
                }

                ImGui::Checkbox("Auto Straight Ufo", &straightUfo);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Stabilizes your UFO on a Y position.");
                    ImGui::EndTooltip();
                }

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_ufo_opts", ImGuiDir_Right))
                    ImGui::OpenPopup("straight ufo options");

                if (ImGui::BeginPopup("straight ufo options")) {
                    if (ImGui::Checkbox("Player 1##ufo", &straightUfoP1)) Config::save();

                    ImGui::PushItemWidth(100.f);
                    if (ImGui::InputDouble("Target Y##p1ufo", &straightUfoTargetP1, 0.0, 0.0, "%.2f")) {
                        straightUfoTargetP1 = std::max(straightUfoTargetP1, 0.0);
                        Config::save();
                    }
                    if (ImGui::InputDouble("Threshold##p1ufo", &straightUfoThresholdP1, 0.0, 0.0, "%.2f")) {
                        straightUfoThresholdP1 = std::max(straightUfoThresholdP1, 0.0);
                        Config::save();
                    }

                    if (ImGui::Checkbox("Player 2##ufo", &straightUfoP2)) Config::save();

                    if (ImGui::InputDouble("Target Y##p2ufo", &straightUfoTargetP2, 0.0, 0.0, "%.2f")) {
                        straightUfoTargetP2 = std::max(straightUfoTargetP2, 0.0);
                        Config::save();
                    }
                    if (ImGui::InputDouble("Threshold##p2ufo", &straightUfoThresholdP2, 0.0, 0.0, "%.2f")) {
                        straightUfoThresholdP2 = std::max(straightUfoThresholdP2, 0.0);
                        Config::save();
                    }
                    ImGui::PopItemWidth();
                    ImGui::EndPopup();
                }

                if (ImGui::Checkbox("Autoclicker P1", &autoclickerP1)) {
                    autoclickerTimerP1 = INT_MAX;
                }

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_autoclick_p1", ImGuiDir_Right))
                    ImGui::OpenPopup("autoclicker p1 options");

                if (ImGui::BeginPopup("autoclicker p1 options")) {
                    ImGui::PushItemWidth(100.f);
                    if (ImGui::InputInt("Click every frames##p1", &autoclickerEveryP1, 0, 0)) {
                        autoclickerEveryP1 = std::max(autoclickerEveryP1, 0);
                    }
                    if (ImGui::InputInt("Hold for frames##p1", &autoclickerHoldP1, 0, 0)) {
                        autoclickerHoldP1 = std::max(autoclickerHoldP1, 1);
                    }
                    ImGui::Checkbox("Swift##autoclickP1", &autoclickerSwiftP1);
                    ImGui::PopItemWidth();
                    ImGui::EndPopup();
                }

                if (ImGui::Checkbox("Autoclicker P2", &autoclickerP2)) {
                    autoclickerTimerP2 = INT_MAX;
                }

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_autoclick_p2", ImGuiDir_Right))
                    ImGui::OpenPopup("autoclicker p2 options");

                if (ImGui::BeginPopup("autoclicker p2 options")) {
                    ImGui::PushItemWidth(100.f);
                    if (ImGui::InputInt("Click every frames##p2", &autoclickerEveryP2, 0, 0)) {
                        autoclickerEveryP2 = std::max(autoclickerEveryP2, 0);
                    }
                    if (ImGui::InputInt("Hold for frames##p2", &autoclickerHoldP2, 0, 0)) {
                        autoclickerHoldP2 = std::max(autoclickerHoldP2, 1);
                    }
                    ImGui::Checkbox("Swift##autoclickP2", &autoclickerSwiftP2);
                    ImGui::PopItemWidth();
                    ImGui::EndPopup();
                }

                ImGui::Checkbox("Noclip", &noclip);

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_noclip_opts", ImGuiDir_Right))
                    ImGui::OpenPopup("noclip options");

                if (ImGui::BeginPopup("noclip options")) {
                    ImGui::Checkbox("Player 1##noclip", &noclipP1);
                    ImGui::Checkbox("Player 2##noclip", &noclipP2);
                    ImGui::EndPopup();
                }

                ImGui::Checkbox("Auto Swift", &autoSwift);
                ImGui::Checkbox("Extra Clicks", &extraClick);

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_extraclick_opts", ImGuiDir_Right))
                    ImGui::OpenPopup("extra click options");

                if (ImGui::BeginPopup("extra click options")) {
                    ImGui::PushItemWidth(100.f);
                    ImGui::InputInt("Amount##extraclicks", &extraClickAmount, 0, 0);
                    ImGui::PopItemWidth();
                    ImGui::EndPopup();
                }

                ImGui::EndTabItem();
            }

            // TAB 2: VISUAL
            if (ImGui::BeginTabItem("Visual")) {

                if (ImGui::Checkbox("Level Fade In/Out", &fadeLevel)) Config::save();
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Levels will fade in and out on level start/end.");
                    ImGui::EndTooltip();
                }

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_levelfade_opts", ImGuiDir_Right))
                    ImGui::OpenPopup("level fade options");

                if (ImGui::BeginPopup("level fade options")) {
                    ImGui::PushItemWidth(100.f);
                    if (ImGui::InputDouble("Fade In##level", &fadeLevelInDuration, 0.0, 0.0, "%.2f")) {
                        fadeLevelInDuration = std::max(fadeLevelInDuration, 0.0);
                        Config::save();
                    }
                    if (ImGui::InputDouble("Fade Out##level", &fadeLevelOutDuration, 0.0, 0.0, "%.2f")) {
                        fadeLevelOutDuration = std::max(fadeLevelOutDuration, 0.0);
                        Config::save();
                    }
                    ImGui::PopItemWidth();
                    ImGui::EndPopup();
                }

                if (ImGui::Checkbox("Audio Fade In/Out", &fadeAudio)) Config::save();
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Music will fade in and out on level start/end.");
                    ImGui::EndTooltip();
                }

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_audiofade_opts", ImGuiDir_Right))
                    ImGui::OpenPopup("audio fade options");

                if (ImGui::BeginPopup("audio fade options")) {
                    ImGui::PushItemWidth(100.f);
                    if (ImGui::InputDouble("Fade In##audio", &fadeAudioInDuration, 0.0, 0.0, "%.2f")) {
                        fadeAudioInDuration = std::max(fadeAudioInDuration, 0.0);
                        Config::save();
                    }
                    if (ImGui::InputDouble("Fade Out##audio", &fadeAudioOutDuration, 0.0, 0.0, "%.2f")) {
                        fadeAudioOutDuration = std::max(fadeAudioOutDuration, 0.0);
                        Config::save();
                    }
                    ImGui::PopItemWidth();
                    ImGui::EndPopup();
                }

                if (ImGui::Checkbox("Hide Endscreen", &hideEndscreen)) Config::save();
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Disables the end layer statistics dropdown.");
                    ImGui::EndTooltip();
                }

                if (ImGui::Checkbox("Hide New Best", &hideNewBest)) Config::save();

                if (ImGui::Checkbox("No Death Effect", &noDeathEffect)) Config::save();

                if (ImGui::Checkbox("No Effects", &noEffect)) Config::save();
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Stops most visual particle and circle effects from rendering.");
                    ImGui::EndTooltip();
                }

                ImGui::Checkbox("Layout Mode", &layoutMode);

                ImGui::SameLine();
                if (ImGui::ArrowButton("btn_layout_opts", ImGuiDir_Right))
                    ImGui::OpenPopup("layout mode options");

                if (ImGui::BeginPopup("layout mode options")) {
                    static std::array<float, 3> bg = colorToFloat(layoutModeColorBackground);
                    if (ImGui::ColorEdit3("Background", bg.data())) {
                        layoutModeColorBackground = floatToColor(bg.data());
                    }

                    static std::array<float, 3> ground = colorToFloat(layoutModeColorGround);
                    if (ImGui::ColorEdit3("Ground", ground.data())) {
                        layoutModeColorGround = floatToColor(ground.data());
                    }
                    ImGui::EndPopup();
                }

                ImGui::Checkbox("Restart First Frame", &restartFirstFrame);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Useful for rendering levels that break on the first attempt.");
                    ImGui::EndTooltip();
                }

                if (ImGui::Checkbox("Stacked Orb Optimization", &optimizeStackedOrbs)) Config::save();
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Clicking multiple orbs in a frame will only trigger one effect.");
                    ImGui::EndTooltip();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

} // namespace GUI
