#include "dllmain.h"
#include <chrono>
#include <cmath>
#include <math.h>

//class TestItem : public Item {
//public:
//    TestItem(const std::string& identifier, short mId) : Item(identifier, mId)
//    {
//        setMaxDamage(10);
//        setIconInfo("diamond", 0);
//    }
//
//    virtual void appendFormattedHovertext(ItemStackBase const& isb, Level& level, std::string& text, bool b) const override {
//        Item::appendFormattedHovertext(isb, level, text, b);
//        text += "\nHello from custom item";
//    }
//
//    virtual ItemStack& use(ItemStack& itemStack, Player& player) const override {
//        Log::Info("Use");
//        return Item::use(itemStack, player);
//    };
//};
//
//WeakPtr<TestItem> testItem;
//WeakPtr<BlockLegacy> testBlock;
//WeakPtr<BlockItem> testBlockItem;
//
//void RegisterItems(ItemRegistry* registry) {
//    ItemRegistryRef regRef = ItemRegistryManager::getItemRegistry();
//        
//    testItem = registry->registerItemShared<TestItem>("minecraft:test_item", ++registry->mMaxItemID);
//
//    testBlockItem = registry->registerItemShared<BlockItem>("minecraft:test_block", testBlock->getBlockItemId());
//    HashedString hashedBlockName("minecraft:test_block");
//
//    auto* ret = new BlockTypeRegistry::LookupByNameImplReturnType();
//    BlockTypeRegistry::_lookupByNameImpl(ret, hashedBlockName, 0, BlockTypeRegistry::DefaultBlockState);
//
//    const Block* defaultBlockState = BlockTypeRegistry::getDefaultBlockState(hashedBlockName);
//    if (defaultBlockState->mLegacyBlock == nullptr) {
//        Log::Info("mLegacyBlock was nullptr!");
//    }
//    else {
//        Log::Info("mLegacyBlock was found!");
//    }
//
//    Item::addCreativeItem(&regRef, defaultBlockState);
//
//    //Log::Info("{}", defaultBlockState->mLegacyBlock->mID);
//}
//
//void RegisterBlocks(BlockDefinitionGroup* blockDef) {
//    Material testMaterial;
//    testMaterial.mType = Dirt;
//    testMaterial.mNeverBuildable = false;
//    testMaterial.mAlwaysDestroyable = true;
//    testMaterial.mReplaceable = false;
//    testMaterial.mLiquid = false;
//    testMaterial.mTranslucency = 0.0f;
//    testMaterial.mBlocksMotion = true;
//    testMaterial.mBlocksPrecipitation = true;
//    testMaterial.mSolid = true;
//    testMaterial.mSuperHot = false;
//
//    testBlock = BlockTypeRegistry::registerBlock<BlockLegacy>("minecraft:test_block", ++blockDef->mLastBlockId, testMaterial);
//}

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

void* renderLevel(LevelRenderer* levelRenderer, ScreenContext* screenContext, FrameRenderObject* frameRenderObject) {
    auto current_time = std::chrono::steady_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();
    double phase = fmod(time_elapsed / 5000.0f, 1.0);
    double value = (1 + sin(2 * PI * phase - PI / 2)) / 2;

    Tessellator* tes = &screenContext->tessellator;
    mce::MaterialPtr* mat = *reinterpret_cast<mce::MaterialPtr**>(SlideAddress(0x572A440));
    
    // Camera pos in order Z, X, Y
    Vec3* cameraPos = &levelRenderer->mLevelRendererPlayer->cameraPos;
    float x = cameraPos->y;
    float y = cameraPos->z;
    float z = cameraPos->x;

    tes->begin(mce::PrimitiveMode::TriangleList, 36);

    Vec3 pos(0.0f, 100.0f + (float)value, 0.0f);

    for (auto& tri : tris) {
        Vec3 vertex0 = vertexes[tri[0]];
        Vec3 vertex1 = vertexes[tri[1]];
        Vec3 vertex2 = vertexes[tri[2]];

        tes->vertex(pos.x + vertex0.x - x, pos.y + vertex0.y - y, pos.z + vertex0.z - z);
        tes->vertex(pos.x + vertex1.x - x, pos.y + vertex1.y - y, pos.z + vertex1.z - z);
        tes->vertex(pos.x + vertex2.x - x, pos.y + vertex2.y - y, pos.z + vertex2.z - z);
    }

    MeshHelpers::renderMeshImmediately(screenContext, tes, mat);

    void* res = _renderLevel.call<void*>(levelRenderer, screenContext, frameRenderObject);
    return res;
}

ModFunction void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) 
{
    InitializeVtablePtrs();

    hookManager->CreateHookAbsolute(
        _renderLevel,
        SigScan("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F0 48 8B DA 4C 8B F1"),
        &renderLevel
    );

    /*eventManager->registerItems.AddListener(&RegisterItems);
    eventManager->registerBlocks.AddListener(&RegisterBlocks);*/
}