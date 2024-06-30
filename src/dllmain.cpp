#include "dllmain.h"
#include <minecraft/src-vanilla/vanilla_shared/common/world/level/dimension/VanillaDimensionFactory.hpp>

AmethystContext* amethyst;

class ILevel;
class Scheduler;

SafetyHookInline _registerDimensionTypes;

OwnerPtrT<SharePtrRefTraits<Dimension>> makeTestDimension(ILevel& level, Scheduler& scheduler) {
	Assert("makeTestDimension notImplemented");
}

void registerDimensionTypes(OwnerPtrFactory<Dimension, ILevel&, Scheduler&>* factory, void* a, void* b, void* c) {
	_registerDimensionTypes.call(factory, a, b, c);

	// register a dimension with the overworld name because custom names don't seem to get created
	// I suspect they are registered on demand when loading into the dimension.
	factory->registerFactory("overworld", makeTestDimension);

	Log::Info("registerDimensionTypes 0x{:x} 0x{:x} 0x{:x} 0x{:x}", (uintptr_t)factory, (uintptr_t)a, (uintptr_t)b, (uintptr_t)c);
}

ModFunction void Initialize(AmethystContext* _amethyst)
{
	amethyst = _amethyst;

	HookManager& hooks = amethyst->mHookManager;

	hooks.CreateHookAbsolute(_registerDimensionTypes, SlideAddress(0x40818E0), &registerDimensionTypes);
}