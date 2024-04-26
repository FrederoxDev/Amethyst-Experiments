#include "dllmain.h"
#include <chrono>
#include <cmath>
#include <math.h>
#include <vector>
#include "minecraft/src-vanilla/vanilla_shared/common/resources/VanillaInPackagePacks.hpp"
#include <filesystem>
#include "minecraft/src-vanilla/vanilla_client/common/module/VanillaGameModuleClient.hpp"
#include "minecraft/src-deps/core/resource/ResourceHelper.hpp"

namespace fs = std::filesystem;

AmethystContext* amethyst;

SafetyHookInline _getPacks;

std::vector<IInPackagePacks::MetaData>* getPacks(VanillaInPackagePacks* self, std::vector<IInPackagePacks::MetaData>* result, PackType packType) {
    _getPacks.call(self, result, packType);

    if (packType == PackType::Resources) {
        Core::Path path("amethyst/mods/Amethyst-Experiments@1.0.0/resources");
        IInPackagePacks::MetaData meta(path, false, PackCategory::Standard);
        result->push_back(meta);
    }

    return result;
}

SafetyHookInline _initializeResourceStack;

void initializeResourceStack(VanillaGameModuleClient* self, const Experiments* a2, IResourcePackRepository* a3, ResourcePackStack* a4, const BaseGameVersion* a5, GameModuleClient::ResourceLoadingPhase a6) {
    mce::UUID uuid = mce::UUID::fromString("6f01ac0c-8774-49c1-33c4-678c962b85ac");
    SemVersion semVersion {0, 0, 1, "", "", "", true, true};
    PackIdVersion packVersion { uuid, semVersion, PackType::Resources };

    _initializeResourceStack.call(self, a2, a3, a4, a5, a6);
}

ModFunction void Initialize(AmethystContext* _amethyst)
{
    amethyst = _amethyst;

    amethyst->mHookManager.RegisterFunction<&VanillaInPackagePacks::getPacks>("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA 48 89 55 ? 33 F6");
    amethyst->mHookManager.CreateHook<&VanillaInPackagePacks::getPacks>(_getPacks, &getPacks);

    amethyst->mHookManager.RegisterFunction<&VanillaGameModuleClient::initializeResourceStack>("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E9 4C 89 8D ? ? ? ? 49 8B F0");
    amethyst->mHookManager.CreateHook<&VanillaGameModuleClient::initializeResourceStack>(_initializeResourceStack, &initializeResourceStack);
}