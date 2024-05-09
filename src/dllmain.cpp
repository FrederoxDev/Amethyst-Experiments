#include "dllmain.h"
#include <minecraft/src/common/world/item/registry/ItemRegistryManager.hpp>
#include <minecraft/src/common/world/level/block/registry/BlockTypeRegistry.hpp>
#include <amethyst/MinecraftVtables.hpp>
#include <minecraft/src/common/world/item/BlockItem.hpp>

AmethystContext* amethyst;

WeakPtr<BlockItem> testBlockItem;
WeakPtr<BlockLegacy> testBlock;

ModFunction void Initialize(AmethystContext* _amethyst)
{
	amethyst = _amethyst;
	InitializeVtablePtrs();

	amethyst->mEventManager.registerItems.AddListener(&RegisterItems);
	amethyst->mEventManager.registerBlocks.AddListener(&RegisterBlocks);
}

void RegisterItems(ItemRegistry* registry) 
{
	Log::Info("Register Items");

	testBlockItem = registry->registerItemShared<BlockItem>("minecraft:test_block", testBlock->getBlockItemId());

	HashedString hashedName("minecraft:test_block");
	auto lookupResult = BlockTypeRegistry::_lookupByNameImpl(hashedName, 0, BlockTypeRegistry::BlockLegacyResolve);

	Log::Info("looked up id: {}", lookupResult.blockLegacy->mID);
}

void RegisterBlocks(BlockDefinitionGroup* blockDefinitions) {
	Log::Info("RegisterBlocks");

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

	testBlock = BlockTypeRegistry::registerBlock<BlockLegacy>("minecraft:test_block", ++blockDefinitions->mLastBlockId, testMaterial);
	Log::Info("testBlock id: {}", testBlock->mID);
}