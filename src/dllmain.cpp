#include "dllmain.h"
#include <minecraft/src/common/world/item/registry/ItemRegistryManager.hpp>

AmethystContext* amethyst;

ModFunction void Initialize(AmethystContext* _amethyst)
{
	amethyst = _amethyst;

	Log::Info("hello!");
	amethyst->mEventManager.registerItems.AddListener(&RegisterItems);
}

SharedPtr<Item> testItem;

void RegisterItems(ItemRegistry* registry) 
{
	if (registry == nullptr) {
		Log::Info("ItemRegistry* was nullptr");
		return;
	}

	Log::Info("Register Items");

	testItem = SharedPtr(new Item("test_item", ++registry->mMaxItemID));

	registry->registerItem(testItem);
}