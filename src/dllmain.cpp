#include "dllmain.h"

//AmethystContext& amethyst;

ModFunction void Initialize(AmethystContext& amethyst)
{
	auto& featureFlags = amethyst.mFeatures;
	featureFlags->enableInputSystem = true;

	Log::Info("hello!");
}