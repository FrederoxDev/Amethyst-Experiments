#pragma once
#include <Windows.h>
#include <amethyst/events/EventManager.h>
#include <amethyst/HookManager.h>
#include <amethyst/InputManager.h>
#include <minecraft/src/common/world/item/Item.h>
#include <minecraft/src/common/world/item/BlockItem.h>
#include <amethyst/MinecraftVtables.h>
#include <minecraft/src/common/world/item/registry/ItemRegistryRef.h>
#include <minecraft/src/common/world/item/registry/ItemRegistryManager.h>
#include <minecraft/src/common/world/item/VanillaItems.h>
#include <minecraft/src-deps/core/string/StringHash.h>
#include <cstdarg>
#include <minecraft/src/common/resources/BaseGameVersion.h>
#include <minecraft/src/common/world/level/block/BlockLegacy.h>
#include <minecraft/src/common/world/level/block/Block.h>
#include <minecraft/src/common/world/level/block/registry/VanillaBlockTypeRegistry.h>
#include <minecraft/src/common/world/level/block/registry/BlockTypeRegistry.h>
#include <minecraft/src/common/world/level/block/definition/BlockDefinitionGroup.h>

#define ModFunction extern "C" __declspec(dllexport)

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}