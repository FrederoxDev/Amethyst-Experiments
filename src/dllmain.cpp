#include "dllmain.h"
#include <chrono>
#include <cmath>
#include <math.h>
#include <vector>
#include "minecraft/src-vanilla/vanilla_shared/common/resources/VanillaInPackagePacks.hpp"

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

ModFunction void Initialize(AmethystContext* _amethyst)
{
    amethyst = _amethyst;
    amethyst->mHookManager.RegisterFunction<&VanillaInPackagePacks::getPacks>("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA 48 89 55 ? 33 F6");
    amethyst->mHookManager.CreateHook<&VanillaInPackagePacks::getPacks>(_getPacks, &getPacks);
}