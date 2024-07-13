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
#include <minecraft/src/common/world/level/ILevel.hpp>
#include <memory>
#include <minecraft/src/common/world/phys/Vec2.hpp>
#include <minecraft/src/common/dataloadhelper/DefaultDataLoadHelper.hpp>
#include <minecraft/src-deps/core/utility/NonOwnerPointer.hpp>
#include <minecraft/src/common/network/ServerNetworkHandler.hpp>
#include <minecraft/src/common/world/level/dimension/VanillaDimensions.hpp>
#include <minecraft/src/common/network/packet/ChangeDimensionPacket.hpp>
#include <minecraft/src/common/world/level/dimension/DimensionBrightnessRamp.hpp>
#include "F3Screen.hpp"

AmethystContext* amethyst;
static DimensionType testDimensionID = DimensionType(0);
static std::string testDimensionName = "overworld";

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
        //Log::Info("[loadChunk] mPosition: {}, loadState: {}, min: {}, max: {}", lc.mPosition, (int)lc.mLoadState, lc.mMin, lc.mMax);

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
	TestOverworldDimension(ILevel& level, DimensionType dimId, DimensionHeightRange heightRange, Scheduler& callbackContext, std::string dimensionName)
        : OverworldDimension(level, dimId, heightRange, callbackContext, testDimensionName)
    {
        mHasWeather = true;
        mDefaultBrightness.sky = Brightness::MAX;
        mSeaLevel = 63;
        mDimensionBrightnessRamp = std::make_unique<DimensionBrightnessRamp>();
        mDimensionBrightnessRamp->buildBrightnessRamp();
        Log::Info("custom ctor");
	};

    //TestOverworldDimension(ILevel& level, Scheduler& callbackContext) : OverworldDimension(level, callbackContext) {}

    virtual std::unique_ptr<class WorldGenerator> createGenerator(const br::worldgen::StructureSetRegistry&) override {
        return std::make_unique<TestGenerator>(*this);
    }
};

OwnerPtr<Dimension> makeTestDimension(ILevel& level, Scheduler& scheduler) {
    DimensionHeightRange heightRange;
    heightRange.mMin = 0;
    heightRange.mMax = 255;

    //return OwnerPtr<Dimension>(std::make_shared<TestOverworldDimension>(level, scheduler));
    return OwnerPtr<Dimension>(std::make_shared<TestOverworldDimension>(level, testDimensionID, heightRange, scheduler, testDimensionName));
}

SafetyHookInline __loadNewPlayer;

struct LambdaFields {
    ServerNetworkHandler* serverNetworkHandler;
    ServerPlayer* player;
    CompoundTag** compound;
};

void _loadNewPlayer(LambdaFields* a1) { 
    ILevel& level = *a1->serverNetworkHandler->mLevel;
    CompoundTag* actorData = *a1->compound;
    ServerPlayer* player = a1->player;
    DefaultDataLoadHelper dataLoadHelper = DefaultDataLoadHelper();

    if (actorData) {
        player->mInitMethod = ActorInitializationMethod::LOADED;
        player->load(*actorData, dataLoadHelper);
    }
    else {
        player->mInitMethod = ActorInitializationMethod::SPAWNED;
    }

    // Try and load the dimension from NBT
    if (!player->hasDimension()) {
        DimensionType dimId = VanillaDimensions::Undefined;

        // load from compoundTag
        if (actorData && actorData->contains("DimensionId")) {
            dimId = DimensionType(actorData->getInt("DimensionId"));
        }

        // Validate the dimension is not undefined
        dimId = level.getLastOrDefaultSpawnDimensionId(dimId);
        dimId = testDimensionID; // Force an ID for debugging, not apart of func.
        WeakRef<Dimension> dimension = level.getOrCreateDimension(dimId);

        if (!dimension) {
            Assert("Failed to get or create dimension for loading player which had a serialzed dimension id {}", dimId.runtimeID);
        }

        player->setDimension(dimension);
    }
    
    if (!player->hasDimension()) {
        Assert("Could not get or create dimension when loading the player");
    }

    const Dimension& dimension = player->getDimensionConst();
    const BlockPos& defaultSpawn = level.getDefaultSpawn();

    // the defaultSpawn BlockPos seems to get validated before doing this, emitting for now..
    // seems to be just a check for: bug 475722 (which is hidden to public)
    player->moveTo(Vec3(defaultSpawn), Vec2(0.0, 0.0));
    player->prepareRegion(*dimension.mChunkSource.get());

    if (actorData) {
        player->reload();
        player->load(*actorData, dataLoadHelper);
    }
}

void registerDimensionTypes(OwnerPtrFactory<Dimension, ILevel&, Scheduler&>* factory, void* a, void* b, void* c) {
    VanillaDimensions::DimensionMap->clear();
    VanillaDimensions::DimensionMap->emplace(testDimensionName, testDimensionID);

	// register a dimension with the overworld name because custom names don't seem to get created
	// I suspect they are registered on demand when loading into the dimension.
	//_registerDimensionTypes.call(factory, a, b, c);
    factory->registerFactory(testDimensionName, makeTestDimension);

    Log::Info("0x{:x}", (uintptr_t)&factory->mFactoryMap);

	Log::Info("registerDimensionTypes 0x{:x} 0x{:x} 0x{:x} 0x{:x}", (uintptr_t)factory, (uintptr_t)a, (uintptr_t)b, (uintptr_t)c);
}

SafetyHookInline _getOrCreateDimension;

WeakRef<Dimension>* getOrCreateDimension(DimensionManager* self, WeakRef<Dimension>* result, DimensionType dimType) {
    Log::Info("getOrCreateDimension, creating dimType {:d}", dimType.runtimeID);

    result = _getOrCreateDimension.call<WeakRef<Dimension>*>(self, result, dimType);

    Log::Info("getOrCreateDimension result: 0x{:x}", (uint64_t)result->get());
    return result;
}

SafetyHookInline _VanillaDimensions_toString;

std::string VanillaDimensions_toString(const DimensionType& dimId) {
    
    if (dimId.runtimeID == testDimensionID.runtimeID) return testDimensionName;

    if (dimId.runtimeID == 0) return "overworld";
    if (dimId.runtimeID == 1) return "nether";
    if (dimId.runtimeID == 2) return "the end";
    if (dimId.runtimeID == 3) return "undefined";

    Assert("Unknown DimensionType: {}", dimId.runtimeID);
}

SafetyHookInline _VanillaDimensions_toSerializedInt;

struct UnknownType {
    short someShort;
    std::string biome;
    DimensionType dimId;
};

void serializeDimID(UnknownType& ukn, BinaryStream& stream) {
    Log::Info("{} {} {}", ukn.someShort, ukn.biome, ukn.dimId.runtimeID);

    stream.write<short>(0);
    stream.writeString("plains");
    stream.writeUnsignedVarInt32(ukn.dimId.runtimeID * 2);
}

SafetyHookInline _VanillaDimensions_fromSerializedInt;

Bedrock::Result<DimensionType> VanillaDimensions_fromSerializedInt(Bedrock::Result<int>&& serializedValue) {
    DimensionType dimType = DimensionType(serializedValue.value());
    return Bedrock::Result<DimensionType>(dimType);
}

SafetyHookInline _ChangeDimensionPacket_write;

void ChangeDimensionPacket_write(ChangeDimensionPacket* self, BinaryStream& stream) {
    Log::Info("ChangeDimensionPacket::write: {}", self->mDimensionId.runtimeID);

    stream.writeUnsignedVarInt32(self->mDimensionId.runtimeID);
    stream.write(self->mPos);
    stream.write(self->mRespawn);
}

SafetyHookInline _MinecraftPackets_createPacket;

std::shared_ptr<Packet> MinecraftPackets_createPacket(MinecraftPacketIds packetID) {
    auto packet = _MinecraftPackets_createPacket.call<std::shared_ptr<Packet>>(packetID);
    Log::Info("[MinecraftPackets::createPacket] {}", packet->getName());
    return packet;
}

ModFunction void Initialize(AmethystContext* _amethyst)
{
	InitializeVtablePtrs();
	amethyst = _amethyst;

	HookManager& hooks = *amethyst->mHookManager;

	hooks.CreateHookAbsolute(_registerDimensionTypes, SlideAddress(0x40818E0), &registerDimensionTypes);
    hooks.CreateHookAbsolute(__loadNewPlayer, SlideAddress(0x174AFE0), &_loadNewPlayer);

    hooks.RegisterFunction<&DimensionManager::getOrCreateDimension>("48 89 5C 24 ? 44 89 44 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B FA 4C 8B F9");
    hooks.CreateHook<&DimensionManager::getOrCreateDimension>(_getOrCreateDimension, &getOrCreateDimension);

    // todo: hook fromString..

    hooks.RegisterFunction<&VanillaDimensions::toString>("40 53 48 83 EC ? 4C 63 02");
    hooks.CreateHook<&VanillaDimensions::toString>(_VanillaDimensions_toString, &VanillaDimensions_toString);

    hooks.RegisterFunction<&VanillaDimensions::toSerializedInt>("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B DA 48 8B F9 0F B7 11");
    hooks.CreateHook<&VanillaDimensions::toSerializedInt>(_VanillaDimensions_toSerializedInt, &serializeDimID);

    hooks.RegisterFunction<&VanillaDimensions::fromSerializedInt>("48 89 5C 24 ? 48 89 7C 24 ? 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B D9 48 8D 4D");
    hooks.CreateHook<&VanillaDimensions::fromSerializedInt>(_VanillaDimensions_fromSerializedInt, &VanillaDimensions_fromSerializedInt);

    // Unused:

    //hooks.RegisterFunction<&ChangeDimensionPacket::write>("48 89 5C 24 ? 57 48 83 EC ? 8B 41 ? 48 8B FA 39 05");
    //hooks.CreateHook<&ChangeDimensionPacket::write>(_ChangeDimensionPacket_write, &ChangeDimensionPacket_write);

    //hooks.RegisterFunction<&MinecraftPackets::createPacket>("40 53 48 83 EC ? 45 33 C0 48 8B D9 FF CA 81 FA");
    //hooks.CreateHook<&MinecraftPackets::createPacket>(_MinecraftPackets_createPacket, &MinecraftPackets_createPacket);

    auto& events = *amethyst->mEventManager;
    events.afterRenderUI.AddListener(&RenderF3);
}