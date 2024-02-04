#include "dllmain.h"

SafetyHookInline _VanillaItems_registerItems;
SafetyHookInline _handleAssert;

class TestItem : public Item {
public:
    TestItem(const std::string& identifier, short mId) : Item(identifier, mId)
    {
        setMaxDamage(10);
        setIconInfo("diamond", 0);
    }

    virtual void fixupCommon(ItemStackBase&) const override {
        Log::Info("fixupCommon(ItemStackBase&)");
    }

    /*virtual void fixupCommon(ItemStackBase&, Level&) const override {
        Log::Info("fixupCommon(ItemStackBase&, Level&)");
    }*/

    virtual void appendFormattedHovertext(ItemStackBase const& isb, Level& level, std::string& text, bool b) const override {
        Item::appendFormattedHovertext(isb, level, text, b);
        text += "Hello from custom item";
    }
};

void* VanillaItems_registerItems(
    const ItemRegistryRef itemRegistry, 
    const BaseGameVersion* baseGameVersion, 
    const Experiments* experiments, 
    void* enableExperimentalGameplay
) {
    void* res = _VanillaItems_registerItems.fastcall<void*>(&itemRegistry, baseGameVersion, experiments, enableExperimentalGameplay);

    std::shared_ptr<ItemRegistry>* sharedRegistryPtr = new std::shared_ptr<ItemRegistry>();
    itemRegistry._lockRegistry(&sharedRegistryPtr);

    ItemRegistry* registry = sharedRegistryPtr->get();

    Log::Info("minecraft:test_item {}", ++registry->mMaxItemID);
    Item* item = new TestItem("minecraft:test_item", registry->mMaxItemID);

    // Replace with regular item vtable
   /* uintptr_t* vtablePtr = *reinterpret_cast<uintptr_t**>(&item);
    *vtablePtr = SlideAddress(0x53C9B70);*/

    SharedPtr<Item> sharedItem(item);
    registry->registerItem(sharedItem);

    Log::Info("Registered Item");

    return res;
}

ModFunction void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) 
{
    InitializeVtablePtrs();

    hookManager->RegisterFunction(&VanillaItems::registerItems, "40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 0F 29 B4 24");
    hookManager->CreateHook(&VanillaItems::registerItems, _VanillaItems_registerItems, &VanillaItems_registerItems);

    //hookManager->CreateHookAbsolute(_handleAssert, SigScan("4C 8B DC 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4C 8B D1"), &handleAssert);
}