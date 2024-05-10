#include "dllmain.h"
#include <minecraft/src/common/world/item/registry/ItemRegistryManager.hpp>
#include <minecraft/src/common/world/level/block/registry/BlockTypeRegistry.hpp>
#include <amethyst/MinecraftVtables.hpp>
#include <minecraft/src/common/world/item/BlockItem.hpp>

class TestBlockClass : public BlockLegacy {
public:
	virtual const AABB& getVisualShape(const Block& block, AABB& aabb) const override {
		return AABB(0.0, 0.0, 0.0, 10.0, 10.0, 1.0); 
	}

public:
	TestBlockClass(const std::string& nameId, short id, const Material& material) 
		: BlockLegacy(nameId, id, material) {
	
	}
};

AmethystContext* amethyst;
WeakPtr<BlockItem> testBlockItem;
WeakPtr<TestBlockClass> testBlock;

ModFunction void Initialize(AmethystContext* _amethyst)
{
	amethyst = _amethyst;
	InitializeVtablePtrs();

	amethyst->mEventManager.registerItems.AddListener(&RegisterItems);
	amethyst->mEventManager.registerBlocks.AddListener(&RegisterBlocks);
}

void RegisterItems(ItemRegistry* registry) 
{
	testBlockItem = registry->registerItemShared<BlockItem>("minecraft:test_block", testBlock->getBlockItemId());
}

void RegisterBlocks(BlockDefinitionGroup* blockDefinitions) {
	Material testMaterial;
	testMaterial.mType = Dirt;
	testMaterial.mNeverBuildable = false;
	testMaterial.mAlwaysDestroyable = true;
	testMaterial.mLiquid = false;
	testMaterial.mTranslucency = 0.0f;
	testMaterial.mBlocksMotion = true;
	testMaterial.mBlocksPrecipitation = true;
	testMaterial.mSolid = true;
	testMaterial.mSuperHot = false;

	testBlock = BlockTypeRegistry::registerBlock<TestBlockClass>("minecraft:test_block", ++blockDefinitions->mLastBlockId, testMaterial);
	testBlock->setDestroyTime(1.5, 6.0);
}