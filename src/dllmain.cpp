#include "dllmain.h"
#include <minecraft/src/common/ActorUniqueID.hpp>
#include <minecraft/src-vanilla/vanilla_shared/common/world/level/dimension/VanillaDimensionFactory.hpp>
#include <minecraft/src/common/world/level/dimension/Dimension.hpp>
#include <minecraft/src/common/world/level/dimension/DimensionHeightRange.hpp>
#include <minecraft/src-vanilla/vanilla_shared/common/world/level/dimension/OverworldDimension.hpp>
#include <minecraft/src/common/world/level/chunk/ChunkSource.hpp>
#include <minecraft/src/common/world/actor/Actor.hpp>
#include <minecraft/src/common/nbt/CompoundTag.hpp>
#include <minecraft/src/common/server/ServerPlayer.hpp>
#include <minecraft/src/common/world/level/DimensionManager.hpp>
#include <memory>

AmethystContext* amethyst;

class ILevel;
class Scheduler;


SafetyHookInline _registerDimensionTypes;

class TestOverworldDimension : public OverworldDimension {
public:
	TestOverworldDimension(ILevel& level, Scheduler& callbackContext) 
	: OverworldDimension(level, callbackContext) {
		Log::Info("TestOverworldDimension");
	};

    /**@vIndex {1} */
    virtual bool isNaturalDimension() const override {
        Log::Info("isNaturalDimension");
        return OverworldDimension::isNaturalDimension();
    }

    /**@vIndex {2} */
    virtual DimensionType getDimensionId() const override {
        DimensionType baseType = OverworldDimension::getDimensionId();
        Log::Info("base {}", baseType.runtimeID);

        //Log::Info("getDimensionId mBlockSource: 0x{:x} 0x{:x}", (uintptr_t)mBlockSource.get(), (uintptr_t)this->weak_from_this().lock().get());
        return DimensionType::AutomaticID(0);
    }

    /**@vIndex {3} */
    virtual void sendPacketForPosition(const BlockPos& pos, const Packet& packet, const Player* player) override {
        Log::Info("sendPacketForPosition");
        return OverworldDimension::sendPacketForPosition(pos, packet, player);
    }

    /**@vIndex {4} */
    virtual void flushLevelChunkGarbageCollector() override {
        Log::Info("flushLevelChunkGarbageCollector");
        return OverworldDimension::flushLevelChunkGarbageCollector();
    }

    /**@vIndex {5} */
    virtual void initializeWithLevelStorageManager(class LevelStorageManager& manager) override {
        Log::Info("initializeWithLevelStorageManager");
        return OverworldDimension::initializeWithLevelStorageManager(manager);
    }

    /**@vIndex {6} */
    virtual BiomeRegistry& getBiomeRegistry() override {
        Log::Info("getBiomeRegistry");
        return OverworldDimension::getBiomeRegistry();
    }

    /**@vIndex {7} */
    virtual const BiomeRegistry& getBiomeRegistry() const override {
        Log::Info("const getBiomeRegistry");
        return OverworldDimension::getBiomeRegistry();
    }

    /**@vIndex {8} */
    virtual Vec3 translatePosAcrossDimension(const Vec3& vec, DimensionType dimId) const override {
        Log::Info("translatePosAcrossDimension");
        return OverworldDimension::translatePosAcrossDimension(vec, dimId);
	}

    /**@vIndex {9} */
    virtual void forEachPlayer(std::function<bool(class Player&)> callback) const override {
        Log::Info("forEachPlayer");
        return OverworldDimension::forEachPlayer(callback);
	}

    /**@vIndex {10} */
    virtual Actor* fetchEntity(ActorUniqueID actorID, bool getRemoved) const override {
        Log::Info("fetchEntity");
        return OverworldDimension::fetchEntity(actorID, getRemoved);
	}

    /**@vIndex {11} */
    virtual void init(const br::worldgen::StructureSetRegistry& reg) override {
        Log::Info("init");
        return OverworldDimension::init(reg);
	}

    /**@vIndex {12} */
    virtual void tick() override {
        Log::Info("tick");
        return OverworldDimension::tick();
	}

    /**@vIndex {13} */
    virtual void tickRedstone() override {
        Log::Info("tickRedstone");
        return OverworldDimension::tickRedstone();
	}

    /**@vIndex {14} */
    /*virtual std::unique_ptr<class WorldGenerator> createGenerator(const br::worldgen::StructureSetRegistry& reg) override {
        Log::Info("createGenerator");
        return OverworldDimension::createGenerator(reg);
	}*/

    /**@vIndex {15} */
    virtual void upgradeLevelChunk(class ChunkSource& source, class LevelChunk& lc, class LevelChunk& generatedChunk) override {
        Log::Info("upgradeLevelChunk");
        return OverworldDimension::upgradeLevelChunk(source, lc, generatedChunk);
	}

    /**@vIndex {16} */
    virtual void fixWallChunk(class ChunkSource& source, class LevelChunk& chunk) override {
        Log::Info("fixWallChunk");
        return OverworldDimension::fixWallChunk(source, chunk);
	}

    /**@vIndex {17} */
    virtual bool levelChunkNeedsUpgrade(const LevelChunk& chunk) const override {
        Log::Info("levelChunkNeedsUpgrade");
        return OverworldDimension::levelChunkNeedsUpgrade(chunk);
	}

    /**@vIndex {18} */
    virtual bool isValidSpawn(int x, int z) const override {
        Log::Info("isValidSpawn");
        return OverworldDimension::isValidSpawn(x, z);
	}

    /**@vIndex {19} */
    virtual class mce::Color getBrightnessDependentFogColor(const mce::Color& baseColor, float brightness) const override {
        Log::Info("getBrightnessDependentFogColor");
        return OverworldDimension::getBrightnessDependentFogColor(baseColor, brightness);
	}

    /**@vIndex {20} */
    virtual bool hasPrecipitationFog() const override {
        Log::Info("hasPrecipitationFog");
        return OverworldDimension::hasPrecipitationFog();
	}

    /**@vIndex {21} */
    virtual short getCloudHeight() const override {
        Log::Info("getCloudHeight");
        return OverworldDimension::getCloudHeight();
	}

    /**@vIndex {22} */
    virtual class HashedString getDefaultBiome() const override {
        Log::Info("getDefaultBiome");
        return OverworldDimension::getDefaultBiome();
	}

    /**@vIndex {23} */
    virtual bool hasGround() const override {
        Log::Info("hasGround");
        return OverworldDimension::hasGround();
	}

    /**@vIndex {24} */
    virtual bool showSky() const override {
        Log::Info("showSky");
        return OverworldDimension::showSky();
	}

    /**@vIndex {25} */
    virtual class BlockPos getSpawnPos() const override {
        Log::Info("getSpawnPos");
        return OverworldDimension::getSpawnPos();
	}

    /**@vIndex {26} */
    virtual int getSpawnYPosition() const override {
        Log::Info("getSpawnYPosition");
        return OverworldDimension::getSpawnYPosition();
	}

    /**@vIndex {27} */
    virtual bool mayRespawnViaBed() const override {
        Log::Info("mayRespawnViaBed");
        return OverworldDimension::mayRespawnViaBed();
	}

    /**@vIndex {28} */
    virtual bool isDay() const override {
        Log::Info("isDay");
        return OverworldDimension::isDay();
	}

    /**@vIndex {29} */
    virtual float getTimeOfDay(int time, float a) const override {
        Log::Info("getTimeOfDay");
        return OverworldDimension::getTimeOfDay(time, a);
	}

    /**@vIndex {30} */
    virtual float getSunIntensity(float a, const Vec3& viewVector, float minInfluenceAngle) const override {
        Log::Info("getSunIntensity");
        return OverworldDimension::getSunIntensity(a, viewVector, minInfluenceAngle);
	}

    /**@vIndex {31} */
    virtual bool forceCheckAllNeighChunkSavedStat() const override {
        Log::Info("forceCheckAllNeighChunkSavedStat");
        return OverworldDimension::forceCheckAllNeighChunkSavedStat();
	}

    /**@vIndex {32} */
    virtual void sendBroadcast(const Packet& packet, class Player* except) override {
        Log::Info("sendBroadcast");
        return OverworldDimension::sendBroadcast(packet, except);
	}

    /**@vIndex {33} */
    virtual bool is2DPositionRelevantForPlayer(const BlockPos& position, class Player& player) const override {
        Log::Info("is2DPositionRelevantForPlayer");
        return OverworldDimension::is2DPositionRelevantForPlayer(position, player);
	}

    /**@vIndex {34} */
    virtual bool isActorRelevantForPlayer(class Player& player, const Actor& actor) const override {
        Log::Info("isActorRelevantForPlayer");
        return OverworldDimension::isActorRelevantForPlayer(player, actor);
	}

    /**@vIndex {35} */
    virtual class BaseLightTextureImageBuilder* getLightTextureImageBuilder() const override {
        Log::Info("getLightTextureImageBuilder");
        return OverworldDimension::getLightTextureImageBuilder();
	}

    /**@vIndex {36} */
    virtual const DimensionBrightnessRamp& getBrightnessRamp() const override {
        Log::Info("getBrightnessRamp");
        return OverworldDimension::getBrightnessRamp();
	}

    /**@vIndex {37} */
    virtual void startLeaveGame() override {
        Log::Info("startLeaveGame");
        return OverworldDimension::startLeaveGame();
	}

    /**@vIndex {38} */
    /*virtual std::unique_ptr<class ChunkBuildOrderPolicyBase> _createChunkBuildOrderPolicy() override {
        Log::Info("_createChunkBuildOrderPolicy");
        return OverworldDimension::_createChunkBuildOrderPolicy();
	}*/

    /**@vIndex {39} */
    virtual void _upgradeOldLimboEntity(class CompoundTag& tag, LimboEntitiesVersion vers) override {
        Log::Info("_upgradeOldLimboEntity");
        return OverworldDimension::_upgradeOldLimboEntity(tag, vers);
	}

    /**@vIndex {40} */
    virtual std::unique_ptr<ChunkSource> _wrapStorageForVersionCompatibility(std::unique_ptr<ChunkSource> storageSource, StorageVersion levelVersion) override {
        Log::Info("_wrapStorageForVersionCompatibility");
        return OverworldDimension::_wrapStorageForVersionCompatibility(std::move(storageSource), levelVersion);
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