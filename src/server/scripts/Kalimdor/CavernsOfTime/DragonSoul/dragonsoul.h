#ifndef DRAGONSOUL_H_
#define DRAGONSOUL_H_

#define MAX_ENCOUNTERS 8

#include "Map.h"
#include "Creature.h"

#define DragonsoulScriptName "instance_dragon_soul"
#define DataHeader "DS"

enum Datas
{
    DATA_MORCHOK                    = 0,
    DATA_WARLORD_ZONOZZ             = 1,
    DATA_YORSAHJ                    = 2,
    DATA_HAGARA                     = 3,
    DATA_ULTRAXION                  = 4,
    DATA_WARMASTER_BLACKHORN        = 5,
    DATA_SPINE_OF_DEATHWING         = 6,
    DATA_MADNESS_OF_DEATHWING       = 7,
    // Misc
    DATA_RAID_MODE                  = 8,
    // Ultraxion
    DATA_ALEXSTRASZA_ULTRAXION      = 9,
    DATA_KALECGOS_ULTRAXION         = 10,
    DATA_YSERA_ULTRAXION            = 11,
    DATA_NOZDORMU_ULTRAXION         = 12,
    DATA_THRALL_ULTRAXION           = 13,
    DATA_DEATHWING_ULTRAXION        = 14,
    DATA_GAUNTLET_TRIGGER           = 15,
    DATA_THE_DRAGON_SOUL_ULTRAXION  = 16,

    DATA_RESTORED_MANA              = 17,
    DATA_ALLIANCE_SHIP_CINEMATIC    = 18,
    DATA_HORDE_SHIP_CINEMATIC       = 19,
    DATA_ALLIANCE_BATTLE_SHIP       = 20,
    DATA_DW_SPINE_BACKPLATE_1       = 21,
    DATA_DW_SPINE_BACKPLATE_2       = 22,
    DATA_DW_SPINE_BACKPLATE_3       = 23,
    DATA_DW_SPINE_BACKPLATE_CNT     = 24,
    DATA_DW_SPINE_ROLL_INFO         = 25,

    DATA_ULTRAXION_CHEST            = 26,
    DATA_ULTRAXION_CHEST_LFR        = 27,
    DATA_SPINE_CHEST                = 28,
    DATA_SPINE_CHEST_LFR            = 29,
    DATA_MADNESS_CHEST              = 30,
    DATA_MADNESS_CHEST_LFR          = 31,
    DATA_ACTIVE_PLATFORM            = 32,
    DATA_MAELSTROM_PORTAL           = 33,
    DATA_LFR_PART                   = 34,
    DATA_MADNESS_PLATFORM_COUNT     = 35,
    DATA_RAID_NERF                  = 36
};

enum GameObjectIds
{
    // Morchok
    GO_FALLING_FRAGMENT                     = 209596,
    // Hagara
    GO_FOCUSING_IRIS                        = 210132,
    GO_ICE_BLOCK_COLLISION                  = 201722,
    // Ultraxion
    GO_ALLIANCE_SHIP_CINEMATIC              = 210211,
    GO_HORDE_SHIP_CINEMATIC                 = 210061,
    GO_ALLIANCE_SHIP_BATTLE                 = 210210,
    GO_GIFT_OF_LIFE                         = 209873,
    GO_ESSENCE_OF_DREAMS                    = 209874,
    GO_SOURCE_OF_MAGIC                      = 209875,
    GO_LESSER_CACHE_OF_THE_ASPECTS_10       = 210160,
    GO_LESSER_CACHE_OF_THE_ASPECTS_25       = 210161,
    GO_LESSER_CACHE_OF_THE_ASPECTS_10_HC    = 210162,
    GO_LESSER_CACHE_OF_THE_ASPECTS_25_HC    = 210163,
    GO_LESSER_CACHE_OF_THE_ASPECTS_LFR      = 210221,

    // Deathwing Spine
    GO_DW_BACKPLATE_1                       = 209623,
    GO_DW_BACKPLATE_2                       = 209631,
    GO_DW_BACKPLATE_3                       = 209632,
    GO_GREATER_CACHE_OF_THE_ASPECTS_10      = 209894,
    GO_GREATER_CACHE_OF_THE_ASPECTS_25      = 209895,
    GO_GREATER_CACHE_OF_THE_ASPECTS_10_HC   = 209896,
    GO_GREATER_CACHE_OF_THE_ASPECTS_25_HC   = 209897,
    GO_GREATER_CACHE_OF_THE_ASPECTS_LFR     = 210222,

    // Madness of Deathwing
    GO_ELEMENTIUM_FRAGMENT_LFR              = 210079,
    GO_ELEMENTIUM_FRAGMENT_10               = 210217,
    GO_ELEMENTIUM_FRAGMENT_25               = 210218,
    GO_ELEMENTIUM_FRAGMENT_10_HC            = 210219,
    GO_ELEMENTIUM_FRAGMENT_25_HC            = 210220
};

enum GlobalMisc
{
    SPELL_PRESENCE_OF_THE_DRAGON_SOUL   = 109247,
    SPELL_POWER_OF_THE_ASPECTS          = 109251, // 5 PCT
    SPELL_GUNSHIP_ENGINE_SOUND          = 109654, // casted by 22517 at -1699.474 Y: -2388.026 Z: 340.1926
	SPELL_WARDEN_RING_GREEN             = 95514,
    SPELL_WARDEN_RING_ORANGE            = 110468,
    SPELL_WARDEN_RING_YELLOW            = 110469,
    LOOT_MODE_LFR                       = 2,
    RAID_MODE_UNKNOWN                   = 0,
    RAID_MODE_NORMAL                    = 1,
    RAID_MODE_LFR                       = 2,
    RAID_NERF_ACTIVE                    = 0,
    RAID_NERF_INACTIVE                  = 1
};

enum CreatureIds
{
    NPC_MORCHOK                 = 55265,
    NPC_WARLORD_ZONOZZ          = 55308,
    NPC_YORSAHJ                 = 55312,
    NPC_HAGARA                  = 55689,
    NPC_ULTRAXION               = 55294,
    NPC_WARMASTER_BLACKHORN     = 56427,
    NPC_DW_SPINE                = 53879,
    NPC_DW_MADNESS              = 56173,

    // Morchok Trash
    NPC_ANCIENT_WATER_LORD      = 57160,
    NPC_EARTHEN_DESTROYER       = 57158,
    NPC_TWILIGHT_SIEGE_CAPTAIN  = 57280,
    NPC_TWILIGHT_PORTAL         = 57231,
    NPC_EARTHEN_SOLDIER         = 57159,

    // Morchok
    NPC_RESONATING_CRYSTAL      = 55346,
    NPC_EARTHEN_VORTEX_VEHICLE  = 109615,
    NPC_WORLD_TRIGGER_VISUAL    = 361710,
    NPC_KOHCROM                 = 57773,

    // Warlord & Yorsahj Zon'ozz trash
    NPC_RUIN_TENTACLE           = 57751,
    NPC_FACELESS_DESTROYER      = 57746,
    NPC_FACELESS_CORRUPTOR      = 57749,
    NPC_WYRMREST_DEFENDER       = 57348,
    NPC_WYRMREST_DEFENDER_2     = 57334,
    NPC_WYRMREST_CAPTAIN        = 57380,
    NPC_WYRMREST_PROTECTOR      = 57398,
    NPC_WYRMREST_PROTECTOR_2    = 27953,
    NPC_FLAIL_OF_GORATH_TRASH   = 57877,
    NPC_EYE_OF_GORATH_TRASH     = 57875,
    NPC_GLOWING_GLOBULE         = 57387,
    NPC_ACIDIC_GLOBULE          = 57333,
    NPC_CRIMSON_GLOBULE         = 57386,
    NPC_DARK_GLOBULE            = 57382,
    NPC_SHADOWED_GLOBULE        = 57388,
    NPC_COBALT_GLOBULE          = 57384,

    // Wyrmrest Temple
    NPC_VALEERA                 = 57289,
    NPC_LETHECUS                = 27940,
    NPC_DALORMI                 = 27943,
    NPC_DASNURIMI               = 58153,
    NPC_CRIMSON_LIFEBINDER      = 57632,
    NPC_TRIZORMU                = 27938,
    NPC_MR_STOMP                = 57460,
    NPC_GORIN                   = 57459,
    NPC_EIENDORMI               = 57288,
    NPC_FERITHOS                = 27935,
    NPC_LORD_AFRASASTRASZ       = 55476,
    NPC_TYRYGOSA                = 57682,
    NPC_DEMESTRASZ              = 58036,

    // Warlord Zon'ozz
    NPC_CLAW_OF_GORATH          = 55418,
    NPC_EYE_OF_GORATH           = 55416,
    NPC_FLAIL_OF_GORATH         = 55417,
    NPC_VOID_OF_THE_UNMAKING    = 55334,
    NPC_TRAVEL_TO_WYRMREST_TEMPLE = 57328,

    // Yorsahj
    BLUE_GLOBULE                = 55866, // Cobalt Globule      - SPELL_SUMMON_MANAVOID
    RED_GLOBULE                 = 55865, // Crimson Globule     - SPELL_SEARING_BLOOD
    GREEN_GLOBULE               = 55862, // Acidic Globule      - SPELL_DIGESTIVE_ACID
    YELLOW_GLOBULE              = 55864, // Glowing Globule     - SPELL_VOID_BOLT_AOE
    VIOLETT_GLOBULE             = 55863, // Shadowed Globule    - SPELL_DEEP_CORRUPTION
    BLACK_GLOBULE               = 55867, // Dark Globule        - SPELL_CORRUPTED_MINIONS
    NPC_MANA_VOID               = 56231,
    NPC_FORGOTTEN_ONE           = 56265, // Corrupted Minion
    NPC_TIME_WARDEN             = 57474,
    NPC_HALO_JUMP_YORSAHJ       = 57629,

    // Hagara
    NPC_ICE_TOMB                = 55695,
    NPC_BINDING_CRYSTAL         = 56136,
    NPC_ICE_WAVE                = 56104,
    NPC_ICICLE_TRIGGER          = 32780,
    NPC_CRYSTAL_CONDUCTOR       = 56165,
    NPC_LIGHTNING_ELEMENTAL     = 56700,

    // Hagara Trash pre event
    NPC_TRASH_TWILIGHT_PORTAL   = 57809,
    NPC_HOVER_DISK              = 57924,
    NPC_TWILIGHT_FROST_EVOKER   = 57807,
    NPC_STORMBORN_MYRMIDON      = 57817,
    NPC_CORRUPTED_FRAGMENT      = 57819,
    NPC_STORMBINDER_ADEPT       = 57823,
    NPC_LIEUTENANT_SHARA        = 57821,
    NPC_TRAVEL_TO_SUMMIT        = 57379,
    NPC_NETHESTRASZ             = 57287,

    // Ultraxion
    NPC_ALEXSTRASZA_ULTRAXION   = 56630,
    NPC_KALECGOS_ULTRAXION      = 56664,
    NPC_YSERA_ULTRAXION         = 56665,
    NPC_NOZDORMU_ULTRAXION      = 56666,
    NPC_THRALL_ULTRAXION        = 56667,
    NPC_DEATHWING_ULTRAXION     = 55971,
    NPC_TWILIGHT_ASSAULTER      = 56249,
    NPC_TWILIGHT_ASSAULTER_2    = 56250,
    NPC_TWILIGHT_ASSAULTER_3    = 56251,
    NPC_ULTRAXION_GAUNTLET      = 56305,
    NPC_THE_DRAGON_SOUL_ULTRAXION = 56668,
    NPC_TWILIGHT_FLAME_TARGET   = 57281,
    NPC_ULTRAXION_VEHICLE       = 56259,
    NPC_LIGHTNING_COSMETIC_TARGET = 91000,
    NPC_YSERA_TARGET            = 51114, // @TODO: replace this entry with the correct entry (+ update condition entry)
    NPC_ALEXSTRASZA_TARGET      = 51132, // @TODO: replace this entry with the correct entry (+ update condition entry)
    NPC_KALECGOS_TARGET         = 51135, // @TODO: replace this entry with the correct entry (+ update condition entry)
    NPC_SKY_CAPTAIN_SWAYZE      = 55870,
    NPC_OUTRO_THRALL            = 57915,
    NPC_OUTRO_YSERA             = 57917,
    NPC_OUTRO_ALEXSTRAZA        = 57919,
    NPC_OUTRO_KALECGOS          = 57916,
    NPC_OUTRO_NOZDORMU          = 57918,
    NPC_OUTRO_THE_DRAGON_SOUL   = 56694,

    // Warmaster Blackhorn
    NPC_GORIONA                 = 56781,
    NPC_TWILIGHT_ASSAULT_DRAKE  = 56587,
    NPC_TWILIGHT_ASSAULT_DRAKE_2 = 56855,
    NPC_TWILIGHT_INFILTRATOR    = 56922,
    NPC_TWILIGHT_ELITE_DREADBLADE = 56854,
    NPC_TWILIGHT_ELITE_SLAYER   = 56848,
    NPC_TWILIGHT_SAPPER         = 56923,
    NPC_THE_SKYFIRE             = 56598,
    NPC_SKYFIRE_HARPOON_GUN     = 56681,
    NPC_SKYFIRE_CANNON          = 57260,
    NPC_ONSLAUGHT_TARGET        = 57238,
    NPC_TWILIGHT_FLAME_GORIONA  = 57268,
    NPC_MASSIVE_EXPLOSION       = 57297,
    NPC_SKYFIRE_FIRE_BRIGADE    = 58174,
    NPC_FIRE_STALKER            = 57852,
    NPC_DECK_FIRE_CONTROLLER    = 57920,
    NPC_SUMMON_ENABLER_STALKER  = 53488,

    // Maelstrom
    NPC_TRAVEL_TO_WYRMREST_BASE = 57882,
    NPC_TRAVEL_TO_SKYFIRE_DECK  = 57378,
    NPC_TRAVEL_TO_THE_MAELSTROM = 57443,

    // Deathwing Spine
    NPC_CORRUPTION_VEH_L        = 56161,
    NPC_CORRUPTION_VEH_R        = 56162,
    NPC_CORRUPTION              = 53891,
    NPC_DW_SPINE_SPAWNER        = 53888,
    NPC_CORRUPTED_BLOOD         = 53889,
    NPC_HIDEOUS_AMALGAMATION    = 53890,
    NPC_BURNING_TENDONS_L       = 56575,
    NPC_BURNING_TENDONS_R       = 56341,

    // Deathwing Madness
    NPC_KALECGOS                = 56101,
    NPC_ALEXSTRASZA             = 56099,
    NPC_NOZDORMU                = 56102,
    NPC_YSERA                   = 56100,
    NPC_DEATHWING_TAIL          = 56844,
    NPC_DEATHWING_ARM_LEFT      = 57694, // seat 1
    NPC_DEATHWING_ARM_RIGHT     = 57686, // seat 0 
    NPC_DEATHWING_WING_LEFT     = 57696, // seat 4
    NPC_DEATHWING_WING_RIGHT    = 57695, // seat 3
    NPC_DEATHWING_HEAD          = 57962, // seat 5
    NPC_ARM_TENTACLE            = 56167,
    NPC_ARM_TENTACLE_2          = 56846,
    NPC_WING_TENTACLE           = 56168,
    NPC_BLISTERING_TENTACLE     = 56188,
    NPC_ELEMENTIUM_FRAGMENT     = 56724,
    NPC_PLATFORM_TRIGGER        = 56307,
    NPC_JUMP_PAD                = 56699,
    NPC_CORRUPTION_TRIGGER      = 56519,
    NPC_DW_MADNESS_THRALL       = 56103,
    NPC_ELEMENTIUM_TERROR       = 56710,
    NPC_THRALL_OUTRO            = 58232,
    NPC_AGGRA_OUTRO             = 58211,
    NPC_ASPECT_1_OUTRO          = 58210,
    NPC_ASPECT_2_OUTRO          = 58209,
    NPC_ASPECT_3_OUTRO          = 58207,
    NPC_ASPECT_4_OUTRO          = 58208,
    NPC_MUTETED_CORRUPTION      = 56471,
    NPC_CRUSH_TARGET            = 56581,
    NPC_REGENERATIVE_BLOOD      = 56263,
    NPC_SUMMON_ENABLER          = 53488,
    NPC_ELEMENTIUM_BOLT_TARGET  = 56545,
    NPC_TIME_ZONE               = 56311,
    NPC_TIME_ZONE_TARGET        = 56332,
    NPC_CORRUPTING_PARASITE     = 57479,
    NPC_CORRUPTING_PARASITE_TENTACLE = 57480,
    NPC_CONGAELING_BLOOD_TARGET = 57788
};

enum DSWorldStates
{
    WS_BLACK_YELLOW     = 6221,
    WS_RED_GREEN        = 6222,
    WS_BLACK_BLUE       = 6223,
    WS_PURPLE_YELLOW    = 6224
};

enum LfrParts
{
    LFR_PART_ONE    = 416,
    LFR_PART_TWO    = 417
};

Position const skyCaptainPos = { -1695.909f, -2353.453f, 339.729f, 4.750f };
Position const skyfirePosition = { 13444.930f, -12133.320f, 151.209f, 0.00f };

struct DragonSoulEventSpawns
{
    uint32 npcId;
    float x;
    float y;
    float z;
    float orientation;
};

template<class AI>
inline AI* GetDragonsoulAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(DragonsoulScriptName))
                return new AI(creature);
    return NULL;
}

#endif // DRAGONSOUL_H_
