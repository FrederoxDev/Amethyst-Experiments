#include "dllmain.h"
#include <minecraft/src/common/world/item/registry/ItemRegistryManager.hpp>
#include <minecraft/src/common/world/level/block/registry/BlockTypeRegistry.hpp>
#include <amethyst/MinecraftVtables.hpp>
#include <minecraft/src/common/world/item/BlockItem.hpp>
#include <minecraft/src-client/common/client/renderer/block/BlockGraphics.hpp>
#include <minecraft/src/common/world/events/gameEvents/GameEvent.hpp>
#include <minecraft/src-client/common/client/renderer/blockActor/BlockActorRenderer.hpp>
#include <minecraft/src-client/common/client/renderer/blockActor/BlockActorRendererDispatcher.hpp>
#include <minecraft/src-client/common/client/renderer/helpers/MeshHelpers.hpp>
#include <minecraft/src-deps/minecraftrenderer/renderer/MaterialPtr.hpp>
#include <minecraft/src-client/common/client/world/item/ItemIconManager.hpp>

static AmethystContext* amethyst;

class TestBlockActor : public BlockActor {
public:
	ItemStackBase mHeldItem;

	TestBlockActor(BlockActorType type, const BlockPos& pos, const std::string& id)
		: BlockActor(type, pos, id) {
		mRendererId = (BlockActorRendererId)26;
	};

	void SetStoredItem(const ItemStackBase& itemStack) {
		mHeldItem = itemStack;
	}
};

class TestBlockActorRenderer : public BlockActorRenderer {
public:
	virtual void render(BaseActorRenderContext& ctx, BlockActorRenderData& data) {

	}
	 
	TestBlockActorRenderer() : BlockActorRenderer() {}
};

class TestBlockClass : public BlockLegacy {
public:
	TestBlockClass(const std::string& nameId, short id, const Material& material) 
		: BlockLegacy(nameId, id, material) 
	{
		mBlockEntityType = (BlockActorType)58;
	}

	virtual std::shared_ptr<BlockActor> newBlockEntity(const BlockPos& pos, const Block& block) const override {
		return std::make_shared<TestBlockActor>((BlockActorType)58, pos, "bosh");
	}

	virtual void addAABBs(const Block&, const IConstBlockSource&, const BlockPos&, const AABB*, std::vector<AABB>&) const override {
		Log::Info("addAABBs");
	}

protected:
	virtual void onPlace(BlockSource& region, const BlockPos& pos) const override
	{
		const Block& block = region.getBlock(pos.below());
	}

	virtual bool use(Player& player, const BlockPos& pos, unsigned char face, std::optional<Vec3> s) const override {
		const Dimension& dimension = player.getDimensionConst();
		BlockSource& region = dimension.getBlockSourceFromMainChunkSource();

		TestBlockActor* blockActor = (TestBlockActor*)region.getBlockEntity(pos);
		if (blockActor == nullptr) {
			Log::Info("Failed to get block actor!");
			return false;
		}

		Inventory* inventory = player.playerInventory->mInventory.get();
		const ItemStack* itemStack = inventory->getItem(player.playerInventory->mSelected);

		if (itemStack == nullptr || itemStack->mItem == nullptr) {
			Log::Info("itemStack or itemStack->mItem was nullptr");
			return false;
		}

		blockActor->SetStoredItem(*itemStack);

		return true;
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

		const Dimension& dimension = entity.getDimensionConst();
		BlockSource& region = dimension.getBlockSourceFromMainChunkSource();

		const Block& block = region.getBlock(pos);

		for (auto& state : block.mLegacyBlock->mStates)
		{
			int value = block.getState<int>(*state.second.mState);
			Log::Info("state: {}, value: {:d}, max_value: {:d}", state.second.mState->mName.c_str(), value, state.second.mVariationCount);

			if ((uint64_t)value + 1 >= (uint64_t)state.second.mVariationCount) value = 0;
			else value++;

			auto result = block.setState<int>(*state.second.mState, value);
			region.setBlock(pos, *result, 3, nullptr, nullptr);

			Log::Info("changed to: {}", value);
		}

		GameEvent* blockChangeEvent = (GameEvent*)SlideAddress(0x5665208);
		region.postGameEvent(&entity, *blockChangeEvent, pos, &block);

		return result;
	}
};
 
SafetyHookInline _initBlocks;
std::string blockIdentifier = "minecraft:test_block";
std::string blockItemIdentifier = "test_block";
WeakPtr<BlockItem> testBlockItem;
WeakPtr<DebugStickItem> debugStickItem;
WeakPtr<TestBlockClass> testBlock;

SafetyHookInline _initializeBlockEntityRenderers;

void initializeBlockEntityRenderers(
	BlockActorRenderDispatcher* self, void* a2, void* a3, void* a4, void* a5, 
	void* a6, void* a7, void* a8, void* a9
){
	_initializeBlockEntityRenderers.call<void>(self, a2, a3, a4, a5, a6, a7, a8, a9);
	self->mRenderers[(BlockActorRendererId)26] = std::make_unique<TestBlockActorRenderer>();
}

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

	new TestBlockActorRenderer();

	amethyst->mEventManager.registerItems.AddListener(&RegisterItems);
	amethyst->mEventManager.registerBlocks.AddListener(&RegisterBlocks);
	amethyst->mHookManager.RegisterFunction<&BlockGraphics::initBlocks>("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B D9 45 33 E4 E8");
	amethyst->mHookManager.CreateHook<&BlockGraphics::initBlocks>(_initBlocks, &initBlocks);

	amethyst->mHookManager.RegisterFunction<&BlockActorRenderDispatcher::initializeBlockEntityRenderers>("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B F9 49 8B F8 48 89 55");
	amethyst->mHookManager.CreateHook<&BlockActorRenderDispatcher::initializeBlockEntityRenderers>(_initializeBlockEntityRenderers, &initializeBlockEntityRenderers);
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