#include <windows.h>
#include <GL/gl.h>
#include "includes.hpp"
#include "config.hpp"
#include "hooks/hooks.hpp"
#include "gui/menu.hpp"
#include "minhook/MinHook.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef BOOL (WINAPI* t_wglSwapBuffers)(HDC hdc);
static t_wglSwapBuffers orig_wglSwapBuffers = nullptr;

static WNDPROC orig_wndProc = nullptr;
static bool imgui_initialized = false;

static LRESULT CALLBACK hook_wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_KEYDOWN) {
        // Toggle menu on 'F' key (0x46) or TAB (VK_TAB)
        if (wParam == 'F' || wParam == VK_TAB) {
            menuVisible = !menuVisible;
            return 0;
        }
    }

    if (menuVisible) {
        ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse && (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)) {
            return 0;
        }
        if (io.WantCaptureKeyboard && (msg >= WM_KEYFIRST && msg <= WM_KEYLAST)) {
            return 0;
        }
    }

    return CallWindowProcA(orig_wndProc, hWnd, msg, wParam, lParam);
}

static BOOL WINAPI hook_wglSwapBuffers(HDC hdc) {
    if (!wglGetCurrentContext()) {
        return orig_wglSwapBuffers(hdc);
    }

    if (!imgui_initialized) {
        HWND currentHwnd = WindowFromDC(hdc);
        if (currentHwnd) {
            hwnd = currentHwnd;
            orig_wndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hook_wndProc)));

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            GUI::setupStyle();

            ImGui_ImplWin32_Init(hwnd);
            ImGui_ImplOpenGL3_Init("#version 120");

            imgui_initialized = true;
        }
    }

    if (imgui_initialized && menuVisible) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        GUI::render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    return orig_wglSwapBuffers(hdc);
}

static DWORD WINAPI MainThread(LPVOID lpParam) {
    // 1. Resolve base address of Geometry Dash
    gd_base = reinterpret_cast<uintptr_t>(GetModuleHandleA(NULL));

    // 2. Wait until Geometry Dash window is initialized
    while (!hwnd) {
        hwnd = FindWindowA("GLFW30", NULL);
        if (!hwnd) hwnd = FindWindowA(NULL, "Geometry Dash");
        Sleep(50);
    }

    // 3. Load saved settings
    Config::load();

    // 4. Hook wglSwapBuffers for ImGui rendering
    HMODULE opengl32 = GetModuleHandleA("opengl32.dll");
    if (!opengl32) {
        opengl32 = LoadLibraryA("opengl32.dll");
    }

    if (opengl32) {
        void* pSwap = reinterpret_cast<void*>(GetProcAddress(opengl32, "wglSwapBuffers"));
        if (pSwap) {
            MH_Initialize();
            MH_CreateHook(pSwap, reinterpret_cast<LPVOID>(&hook_wglSwapBuffers), reinterpret_cast<LPVOID*>(&orig_wglSwapBuffers));
            MH_EnableHook(pSwap);
        }
    }

    // 5. Install game hooks from 2.113 addresses
    Hooks::install();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, MainThread, hModule, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        if (orig_wndProc && hwnd) {
            SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(orig_wndProc));
        }
        if (imgui_initialized) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
        Hooks::remove();
        break;
    }
    return TRUE;
}
