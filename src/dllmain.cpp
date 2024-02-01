#include "dllmain.h"

SafetyHookInline _Item_appendFormattedHoverText;

//void Item_appendFormattedHovertext(Item* self, const ItemStackBase& itemStack, Level& level, std::string& text, uint8_t a5) {
//    _Item_appendFormattedHoverText.call<void, Item*, const ItemStackBase&, Level&, std::string&, uint8_t>(self, itemStack, level, text, a5);
//    Log::Info("{}", self->mId);
//}

ModFunction void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) 
{
    InitializeVtablePtrs();

    // Not really the correct time to construct an item yet, just a proof of concept for virtuals right now
    Item testItem("minecraft:test_item", 999);
    Log::Info("isBucket: {}", testItem.isBucket());
    Log::Info("mId: {}", testItem.mId); 

    /*hookManager->CreateHookAbsolute(
        _Item_appendFormattedHoverText,
        SigScan("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F1 4C 89 44 24 ? 4C 8B F2 48 8B D9"),
        &Item_appendFormattedHovertext
    );*/
}