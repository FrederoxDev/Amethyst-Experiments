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
    ItemRegistryRef regRef = ItemRegistryManager::getItemRegistry();
        
    testItem = registry->registerItemShared<TestItem>("minecraft:test_item", ++registry->mMaxItemID);

    testBlockItem = registry->registerItemShared<BlockItem>("minecraft:test_block", testBlock->getBlockItemId());
    HashedString hashedBlockName("minecraft:test_block");

    Block* defaultBlockState = BlockTypeRegistry::getDefaultBlockState(&hashedBlockName);
    if (defaultBlockState->mLegacyBlock == nullptr) {
        Log::Info("mLegacyBlock was nullptr!");
    }
    else {
        Log::Info("mLegacyBlock was found!");
    }

    //Log::Info("{}", defaultBlockState->mLegacyBlock->mID);
}

void RegisterBlocks(BlockDefinitionGroup* blockDef) {
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

    testBlock = BlockTypeRegistry::registerBlock<BlockLegacy>("minecraft:test_block", ++blockDef->mLastBlockId, testMaterial);
}

ModFunction void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) 
{
    InitializeVtablePtrs();

    eventManager->registerItems.AddListener(&RegisterItems);
    eventManager->registerBlocks.AddListener(&RegisterBlocks);
}