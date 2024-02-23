#include "dllmain.h"
#include <chrono>
#include <cmath>
#include <math.h>

ClientInstance* ci;

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

    auto* ret = new BlockTypeRegistry::LookupByNameImplReturnType();
    BlockTypeRegistry::_lookupByNameImpl(ret, hashedBlockName, 0, BlockTypeRegistry::DefaultBlockState);
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

class FrameRenderObject;
SafetyHookInline _renderLevel;

Vec3 vertexes[8] = {
    Vec3(0.0f, 0.0f, 0.0f),
    Vec3(1.0f, 0.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    Vec3(1.0f, 1.0f, 0.0f),
    Vec3(0.0f, 0.0f, 1.0f),
    Vec3(1.0f, 0.0f, 1.0f),
    Vec3(0.0f, 1.0f, 1.0f),
    Vec3(1.0f, 1.0f, 1.0f),
};

int tris[12][3] = {
    { 2, 6, 3 },
    { 3, 6, 7 },
    { 0, 2, 1 },
    { 2, 3, 1 },
    { 1, 3, 7 },
    { 1, 7, 5 },
    { 0, 1, 5 },
    { 4, 0, 5 },
    { 4, 6, 2 },
    { 2, 0, 4 },
    { 5, 7, 4 },
    { 7, 6, 4 }
};

#define PI       3.14159265358979323846

//SafetyHookInline _getMeshForBlock;
//
//mce::Mesh* getMeshForBlock(BlockTessellator* self, Tessellator* tessellator, const Block* block) {
//    blockTess = self;
//    Log::Info("0x{:x}", FindOffsetOfPointer(ci, self, sizeof(ClientInstance)));
//    return _getMeshForBlock.thiscall<mce::Mesh*>(self, tessellator, block);
//}

void* renderLevel(LevelRenderer* levelRenderer, ScreenContext* screenContext, FrameRenderObject* frameRenderObject) {
    Tessellator* tes = &screenContext->tessellator;

    HashedString hashedMaterialName("entity_static");
    mce::MaterialPtr material(*mce::RenderMaterialGroup::switchable, hashedMaterialName);

    BlockSource* region = ci->getRegion();
    const Block block = region->getBlock(0, -57, 0);
    BlockPos blockPos(0, -57, 0);

    Vec3* cameraPos = &levelRenderer->mLevelRendererPlayer->cameraPos;

    /*ci->mBlockTessellator->appendTessellatedBlock(tes, &block);

    Vec3* cameraPos = &levelRenderer->mLevelRendererPlayer->cameraPos;
    BlockPos blockPos(0, -57, 0);

    Vec3 offset(blockPos.x - cameraPos->y - 0.5f, blockPos.y - cameraPos->z + 0.5f, blockPos.z - cameraPos->x - 0.5f);

    mce::MeshData* meshData = &tes->mMeshData;

    for (auto& pos : meshData->mPositions) {
        pos = pos + offset;
    }*/

    tes->begin(mce::TriangleList, 1);

    //ci->mBlockTessellator->tessellateInWorld(*tes, block, blockPos, 0, 0);

    Vec3 inverseCamera(-cameraPos->y, -cameraPos->z, -cameraPos->x);
    tes->addPostTransformOffset(inverseCamera);

    tes->vertex(0.0f, -56.0f, 0.0f);
    tes->vertex(1.0f, -56.0f, 0.0f);
    tes->vertex(0.0f, -56.0f, 1.0f);
    
    tes->setPosTransformOffset(Vec3(0.0f, 0.0f, 0.0f));

    Log::Info("mCount: {}", tes->mCount);

    mce::Mesh* mesh = new mce::Mesh();
    tes->end(mesh, 0, "Test Mesh", 0);
    tes->clear();

    mesh->renderMesh(screenContext, &material);
    delete mesh;

    //MeshHelpers::renderMeshImmediately(screenContext, tes, &material);
    //tes->clear();

    void* res = _renderLevel.call<void*>(levelRenderer, screenContext, frameRenderObject);
    return res;
}

void OnStartJoinGame(ClientInstance* _ci) {
    ci = _ci;
}
 
ModFunction void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) 
{
    InitializeVtablePtrs();

    eventManager->onStartJoinGame.AddListener(&OnStartJoinGame);

    hookManager->CreateHookAbsolute(
        _renderLevel,
        SigScan("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F0 48 8B DA 4C 8B F1"),
        &renderLevel
    );

    /*hookManager->RegisterFunction(&BlockTessellator::getMeshForBlock, "48 89 5C 24 ? 55 56 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B D8");
    hookManager->CreateHook(&BlockTessellator::getMeshForBlock, _getMeshForBlock, &getMeshForBlock);*/

    /*eventManager->registerItems.AddListener(&RegisterItems);
    eventManager->registerBlocks.AddListener(&RegisterBlocks);*/
}