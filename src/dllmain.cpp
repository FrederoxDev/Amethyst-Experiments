#include "dllmain.h"
#include <minecraft/src/common/world/item/registry/ItemRegistryManager.hpp>
#include <minecraft/src/common/world/level/block/registry/BlockTypeRegistry.hpp>
#include <amethyst/MinecraftVtables.hpp>
#include <minecraft/src/common/world/item/BlockItem.hpp>
#include <minecraft/src-client/common/client/renderer/block/BlockGraphics.hpp>

class TestBlockClass : public BlockLegacy {
public:
	TestBlockClass(const std::string& nameId, short id, const Material& material) 
		: BlockLegacy(nameId, id, material) {
	
	}
};

AmethystContext* amethyst;
WeakPtr<BlockItem> testBlockItem;
WeakPtr<TestBlockClass> testBlock;
 
SafetyHookInline _initBlocks;
std::string blockIdentifier = "minecraft:test_block";

void initBlocks(BlockGraphics* a1) {
	Log::Info("initBlocks");
	_initBlocks.call(a1);

	HashedString hashedIdentifier(blockIdentifier);
	BlockGraphics* graphics = BlockGraphics::createBlockGraphics(hashedIdentifier, BlockShape::STAIRS);
	if (!graphics) {
		Log::Info("BlockGraphics::createBlockGraphics returned nullptr");
		return;
	}

	// if there are no things in the textureItems vector add missing_tile
	if (graphics->mTextureItems.size() == 0) {
		Log::Info("Textures were not found, defaulting to missing_tile texture!");
		graphics->setTextureItem("missing_tile", "missing_tile", "missing_tile", "missing_tile", "missing_tile", "missing_tile");
	}

	graphics->setDefaultCarriedTextures();
}

ModFunction void Initialize(AmethystContext* _amethyst)
{
	amethyst = _amethyst;
	InitializeVtablePtrs();

	amethyst->mEventManager.registerItems.AddListener(&RegisterItems);
	amethyst->mEventManager.registerBlocks.AddListener(&RegisterBlocks);
	amethyst->mHookManager.RegisterFunction<&BlockGraphics::initBlocks>("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B D9 45 33 E4 E8");
	amethyst->mHookManager.CreateHook<&BlockGraphics::initBlocks>(_initBlocks, &initBlocks);
}

void RegisterItems(ItemRegistry* registry) 
{
	testBlockItem = registry->registerItemShared<BlockItem>(blockIdentifier, testBlock->getBlockItemId());
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

	testBlock = BlockTypeRegistry::registerBlock<TestBlockClass>(blockIdentifier, ++blockDefinitions->mLastBlockId, testMaterial);
	testBlock->setDestroyTime(1.5, 6.0);
}