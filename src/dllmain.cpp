#include "dllmain.h"

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

WeakPtr<TestItem> testItem;
WeakPtr<BlockLegacy> testBlock;
WeakPtr<BlockItem> testBlockItem;

void RegisterItems(ItemRegistry* registry) {
    Log::Info("RegisterItems");

    testItem = registry->registerItemShared<TestItem>("minecraft:test_item", ++registry->mMaxItemID);

    testBlockItem = registry->registerItemShared<BlockItem>("minecraft:test_block", testBlock->getBlockItemId());
}

SafetyHookInline _BlockDefinitionGroup_registerBlocks;

void BlockDefinitionGroup_registerBlocks(BlockDefinitionGroup* self) {
    Material testMaterial;
    testMaterial.mType = Dirt;
    testMaterial.mNeverBuildable = false;
    testMaterial.mAlwaysDestroyable = true;
    testMaterial.mReplaceable = false;
    testMaterial.mLiquid = false;
    testMaterial.mTranslucency = 0.0f;
    testMaterial.mBlocksMotion = true;
    testMaterial.mBlocksPrecipitation = true;
    testMaterial.mSolid = true;
    testMaterial.mSuperHot = false;

    testBlock = BlockTypeRegistry::registerBlock<BlockLegacy>("minecraft:test_block", ++self->mLastBlockId, testMaterial);

    Log::Info("BlockDefinitionGroup::registerBlocks");
    _BlockDefinitionGroup_registerBlocks.thiscall<void>(self);
}

ModFunction void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) 
{
    InitializeVtablePtrs();

    eventManager->registerItems.AddListener(&RegisterItems);

    hookManager->RegisterFunction(&BlockDefinitionGroup::registerBlocks, "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 4C 8B F9");
    hookManager->CreateHook(&BlockDefinitionGroup::registerBlocks, _BlockDefinitionGroup_registerBlocks, &BlockDefinitionGroup_registerBlocks);

    //hookManager->CreateHookAbsolute(_handleAssert, SigScan("4C 8B DC 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4C 8B D1"), &handleAssert);
}