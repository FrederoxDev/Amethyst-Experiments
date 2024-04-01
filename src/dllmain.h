#pragma once
#include <Windows.h>
#include <amethyst/runtime/AmethystContext.h>
#include <minecraft/src-client/common/client/input/ClientInputHandler.h>
#include <amethyst/runtime/input/InputManager.h>
#include <minecraft/src-client/common/client/gui/controls/ToggleComponent.h>
#include <minecraft/src-client/common/client/gui/controls/SliderComponent.h>
#include <minecraft/src-client/common/client/gui/controls/TextComponent.h>

#define ModFunction extern "C" __declspec(dllexport)

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}