#include "dllmain.h"
#include <minecraft/src/common/ActorUniqueID.hpp>
#include <minecraft/src-vanilla/vanilla_shared/common/world/level/dimension/VanillaDimensionFactory.hpp>
#include <minecraft/src/common/world/level/dimension/Dimension.hpp>
#include <minecraft/src/common/world/level/dimension/DimensionHeightRange.hpp>
#include <minecraft/src-vanilla/vanilla_shared/common/world/level/dimension/OverworldDimension.hpp>
#include <minecraft/src/common/world/level/chunk/ChunkSource.hpp>
#include <minecraft/src/common/world/level/chunk/LevelChunk.hpp>
#include <minecraft/src/common/world/actor/Actor.hpp>
#include <minecraft/src/common/nbt/CompoundTag.hpp>
#include <minecraft/src/common/server/ServerPlayer.hpp>
#include <minecraft/src/common/world/level/DimensionManager.hpp>
#include <minecraft/src/common/world/level/levelgen/WorldGenerator.hpp>
#include <minecraft/src/common/world/level/levelgen/structure/StructureFeatureRegistry.hpp>
#include <minecraft/src/common/world/level/ChunkPos.hpp>
#include <memory>

AmethystContext* amethyst;

class ILevel;
class Scheduler;

SafetyHookInline _registerDimensionTypes;

class BiomeArea {};

class TestGenerator : public WorldGenerator {
public:
    TestGenerator(Dimension& dimension) : WorldGenerator(dimension, std::make_unique<StructureFeatureRegistry>()) {
        Log::Info("TestGenerator");
    }

    /**@vIndex {11} */
    virtual void loadChunk(LevelChunk& lc, bool forceImmediateReplacementDataLoad) {
        Log::Info("[loadChunk] mPosition: {}, loadState: {}, min: {}, max: {}", lc.mPosition, (int)lc.mLoadState, lc.mMin, lc.mMax);

        lc.mLoadState = ChunkState::Generated;

        if (lc.mGenerator == nullptr) {
            lc.mGenerator = this;
        }
    }

    /**@vIndex {38} */
    virtual void prepareHeights(class BlockVolume& box, class ChunkPos const& chunkPos, bool factorInBeardsAndShavers) {Assert("Unimplemented");}

    /**@vIndex {39} */
    virtual void prepareAndComputeHeights(class BlockVolume& box, class ChunkPos const& chunkPos, std::vector<short>& ZXheights, bool factorInBeardsAndShavers, int skipTopN) {Assert("Unimplemented");}

    /**@vIndex {40} */
    virtual class BiomeArea getBiomeArea(class BoundingBox const& area, uint32_t scale) const {Assert("Unimplemented");}

    /**@vIndex {41} */
    virtual class BiomeSource const& getBiomeSource() const {Assert("Unimplemented");}

    /**@vIndex {42} */
    virtual struct BlockVolumeDimensions getBlockVolumeDimensions() const {Assert("Unimplemented");}

    /**@vIndex {43} */
    virtual class BlockPos findSpawnPosition() const {
        return BlockPos(0, 100, 0);
    }

    /**@vIndex {46} */
    virtual void decorateWorldGenLoadChunk(
        class Biome const& biome,
        class LevelChunk& lc,
        class BlockVolumeTarget& target,
        class Random& random,
        class ChunkPos const& pos) const 
    {
        Assert("Unimplemented");
    }

    /**@vIndex {47} */
    virtual void decorateWorldGenPostProcess(
        class Biome const& biome,
        class LevelChunk& lc,
        class BlockSource& source,
        class Random& random) const 
    {
        Assert("Unimplemented");
    }
};

class TestOverworldDimension : public OverworldDimension {
public:
	TestOverworldDimension(ILevel& level, Scheduler& callbackContext) 
	: OverworldDimension(level, callbackContext) {
		Log::Info("TestOverworldDimension");
	};

    virtual std::unique_ptr<class WorldGenerator> createGenerator(const br::worldgen::StructureSetRegistry&) override {
        return std::make_unique<TestGenerator>(*this);
    }
};

OwnerPtr<Dimension> makeTestDimension(ILevel& level, Scheduler& scheduler) {
	return OwnerPtr<Dimension>(std::make_shared<TestOverworldDimension>(level, scheduler));
}

SafetyHookInline __loadNewPlayer;

struct LambdaFields {
    Level* level;
    ServerPlayer* actor;
    CompoundTag** compound;
};

void _loadNewPlayer(LambdaFields* a1) { 
    CompoundTag* compound = *a1->compound;
    Actor* actor = a1->actor;

    Log::Info("before {}", actor->hasDimension() ? "has dimension" : "no dimension?");

    __loadNewPlayer.call(a1);

    Log::Info("after {}", actor->hasDimension() ? "has dimension" : "no dimension?");

    return;

    // begin reimplementation

    /*if (compound) {
        Assert("loading branch not implemented");
    }
    else {
        actor->mInitMethod = ActorInitializationMethod::SPAWNED;
    }


    if (!actor->hasDimension()) {
        std::string_view dimensionKey = "DimensionId";
        DimensionType dimensionId = DimensionType::Undefined;

        if (compound && compound->contains(dimensionKey)) {
            dimensionId = (DimensionType)compound->getInt(dimensionKey);
            Log::Info("Compound contained DimensionId {:d}", (uint32_t)dimensionId);
        }
    }  */  
}

void registerDimensionTypes(OwnerPtrFactory<Dimension, ILevel&, Scheduler&>* factory, void* a, void* b, void* c) {
	//_registerDimensionTypes.call(factory, a, b, c);

	// register a dimension with the overworld name because custom names don't seem to get created
	// I suspect they are registered on demand when loading into the dimension.
	factory->registerFactory("overworld", makeTestDimension);

	Log::Info("registerDimensionTypes 0x{:x} 0x{:x} 0x{:x} 0x{:x}", (uintptr_t)factory, (uintptr_t)a, (uintptr_t)b, (uintptr_t)c);
}

SafetyHookInline _getOrCreateDimension;

WeakRef<Dimension>* getOrCreateDimension(DimensionManager* self, WeakRef<Dimension>* result, DimensionType dimType) {
    Log::Info("getOrCreateDimension, creating dimType {:d}", dimType.runtimeID);
    result = _getOrCreateDimension.call<WeakRef<Dimension>*>(self, result, dimType);

    Log::Info("getOrCreateDimension result: 0x{:x}", (uint64_t)result->get());
    return result;
}

ModFunction void Initialize(AmethystContext* _amethyst)
{
	InitializeVtablePtrs();
	amethyst = _amethyst;

	HookManager& hooks = amethyst->mHookManager;

	hooks.CreateHookAbsolute(_registerDimensionTypes, SlideAddress(0x40818E0), &registerDimensionTypes);
    hooks.CreateHookAbsolute(__loadNewPlayer, SlideAddress(0x174AFE0), &_loadNewPlayer);

    hooks.RegisterFunction<&DimensionManager::getOrCreateDimension>("48 89 5C 24 ? 44 89 44 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B FA 4C 8B F9");
    hooks.CreateHook<&DimensionManager::getOrCreateDimension>(_getOrCreateDimension, &getOrCreateDimension);
}