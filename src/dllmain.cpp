#include "dllmain.h"

SafetyHookInline _VanillaItems_registerItems;
SafetyHookInline _handleAssert;

void* VanillaItems_registerItems(
    const ItemRegistryRef itemRegistry, 
    const BaseGameVersion* baseGameVersion, 
    const Experiments* experiments, 
    void* enableExperimentalGameplay
) {
    //_VanillaItems_registerItems.call<void, const ItemRegistryRef, const BaseGameVersion*, const Experiments*, bool>(itemRegistry, baseGameVersion, experiments, enableExperimentalGameplay);
    Log::Info("VanillaItems::registerItems");

    Item* item = new Item("minecraft:test_item", 999);
    SharedPtr<Item> sharedItem(item);

    void* res = _VanillaItems_registerItems.fastcall<void*>(&itemRegistry, baseGameVersion, experiments, enableExperimentalGameplay);

    std::shared_ptr<ItemRegistry> registry = itemRegistry._lockRegistry();
    
    registry->registerItem(sharedItem);

    return res;
}

ModFunction void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) 
{
    InitializeVtablePtrs();

    hookManager->RegisterFunction(&VanillaItems::registerItems, "40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 0F 29 B4 24");
    hookManager->CreateHook(&VanillaItems::registerItems, _VanillaItems_registerItems, &VanillaItems_registerItems);

    //hookManager->CreateHookAbsolute(_handleAssert, SigScan("4C 8B DC 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4C 8B D1"), &handleAssert);
}