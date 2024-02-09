#include "dllmain.h"

SafetyHookInline _handleAssert;

class TestItem : public Item {
public:
    TestItem(const std::string& identifier, short mId) : Item(identifier, mId)
    {
        setMaxDamage(10);
        setIconInfo("diamond", 0);
    }

    virtual void appendFormattedHovertext(ItemStackBase const& isb, Level& level, std::string& text, bool b) const override {
        Item::appendFormattedHovertext(isb, level, text, b);
        text += "\nHello from custom item";
    }

    virtual ItemStack& use(ItemStack& itemStack, Player& player) const override {
        Log::Info("Use");
        return Item::use(itemStack, player);
    };
};

void RegisterItems(ItemRegistry* registry) {
    SharedPtr<Item> sharedItem(
        new TestItem("minecraft:test_item", ++registry->mMaxItemID)
    );

    registry->registerItem(sharedItem);
}

ModFunction void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) 
{
    InitializeVtablePtrs();

    eventManager->registerItems.AddListener(&RegisterItems);

    Material material;

    BlockLegacy block("minecraft:test_block", 0, material);
    Log::Info("{}", block.isButtonBlock());

    //hookManager->CreateHookAbsolute(_handleAssert, SigScan("4C 8B DC 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4C 8B D1"), &handleAssert);
}