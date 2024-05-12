#include "dllmain.h"
#include <minecraft/src/common/world/item/registry/ItemRegistryManager.hpp>
#include <minecraft/src/common/world/level/block/registry/BlockTypeRegistry.hpp>
#include <amethyst/MinecraftVtables.hpp>
#include <minecraft/src/common/world/item/BlockItem.hpp>
#include <minecraft/src-client/common/client/renderer/block/BlockGraphics.hpp>

static AmethystContext* amethyst;
static bool isClient = false;

class TestBlockClass : public BlockLegacy {
public:
	TestBlockClass(const std::string& nameId, short id, const Material& material) 
		: BlockLegacy(nameId, id, material) {}

protected:
	virtual void onPlace(BlockSource& region, const BlockPos& pos) const override
	{
		const Block& block = region.getBlock(pos.below());

		Log::Info("TestBlockClass::onPlace");
	}
};

class DebugStickItem : public Item {
public:
	DebugStickItem(const std::string& identifier, short numericalId) 
		: Item(identifier, numericalId) {}

public:
	virtual bool isGlint(const ItemStackBase& stack) const override {
		return true;
	}

private:
	virtual InteractionResult _useOn(ItemStack& instance, Actor& entity, BlockPos pos, unsigned char face, const Vec3& clickPos) const override {
		InteractionResult result;

		result.mResult = 3;

		BlockSource* region = amethyst->mClientInstance->getRegion();
		const Block& block = region->getBlock(pos);

		isClient = !isClient;
		if (isClient) return result;

		for (auto& state : block.mLegacyBlock->mStates)
		{
			int value = block.getState<int>(*state.second.mState);
			Log::Info("state: {}, value: {:d}, max_value: {:d}", state.second.mState->mName.c_str(), value, state.second.mVariationCount);

			if ((uint64_t)value + 1 >= (uint64_t)state.second.mVariationCount) value = 0;
			else value++;

			auto result = block.setState<int>(*state.second.mState, value);
			region->setBlock(pos, *result, 2, nullptr, &entity);

			Log::Info("changed to: {}", value);
		}

		return result;
	}
};

WeakPtr<BlockItem> testBlockItem;
WeakPtr<DebugStickItem> debugStickItem;
WeakPtr<TestBlockClass> testBlock;
 
SafetyHookInline _initBlocks;
std::string blockIdentifier = "minecraft:test_block";
std::string blockItemIdentifier = "test_block";

void initBlocks(BlockGraphics* a1) {
	Log::Info("initBlocks");
	_initBlocks.call(a1);

	HashedString hashedIdentifier(blockIdentifier);
	BlockGraphics* graphics = BlockGraphics::createBlockGraphics(hashedIdentifier, BlockShape::STAIRS);
	if (!graphics) {
		Log::Info("BlockGraphics::createBlockGraphics returned nullptr");
		return;
	}

	if (graphics->mTextureItems.size() == 0) {
		graphics->setTextureItem("diamond_block", "diamond_block", "diamond_block", "diamond_block", "diamond_block", "diamond_block");
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
	testBlockItem = registry->registerItemShared<BlockItem>(blockItemIdentifier, testBlock->getBlockItemId());
	debugStickItem = registry->registerItemShared<DebugStickItem>("debug_stick", ++registry->mMaxItemID);
	debugStickItem->setIconInfo("stick", 0);
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

	BlockState* weirdoDirectionBlockState = (BlockState*)SlideAddress(0x5875420);

	testBlock = BlockTypeRegistry::registerBlock<TestBlockClass>(blockIdentifier, ++blockDefinitions->mLastBlockId, testMaterial);
	testBlock->setDestroyTime(1.5, 6.0);
	testBlock->addState(*weirdoDirectionBlockState);
}