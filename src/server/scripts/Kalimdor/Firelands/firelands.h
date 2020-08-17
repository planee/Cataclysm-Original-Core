/*
* Copyright (C) 2012-2013 Reign of Darkness Network <http://reignofdarkness.net/>
*
* Este codigo es de uso exclusivo y privado para la red de gaming
* Reign of Darkness Network esta totalmente prohibida su distribucion y
* uso en proyectos ajenos, si has obtenido o encontrado este codigo
* publicamente envianos un email a administracion@reignofdarkness.net
* indicandonos como y donde has obtenido este codigo
*
* Recuerda, no distribuyas, no compartas y no subas este codigo a ningun
* lugar publico, usa siempre nuestros repositorios Git!
*/

#ifndef FIRELANDS_H_
#define FIRELANDS_H_

#define MAX_ENCOUNTERS 7

#include "Map.h"
#include "Creature.h"

#define FirelandsScriptName "instance_firelands"
#define DataHeader "FL"

#define MOVIE_ID_BRIDGE_INTRO   197
#define ACTION_ENABLE_TELEPORT 1337

uint32 const EncounterCount         = 7;
Position const shannoxspawn = { -36.625f, -62.700f, 57.697f, 2.968f };

enum Datas
{
    DATA_BETHTILAC                  = 0,
    DATA_LORD_RHYOLITH              = 1,
    DATA_ALYSRAZOR                  = 2,
    DATA_SHANNOX                    = 3,
    DATA_BALOROC                    = 4,
    DATA_MAJORDOMO_STAGHELM         = 5,
    DATA_RAGNAROS                   = 6,

    // Adds
    DATA_CRYSTAL_SHARD              = 9,
    DATA_CURRENT_ENCOUNTER_PHASE    = 10,

    DATA_BETHTILAC_DOOR,
    DATA_BALOROC_DOOR,
    DATA_ALYSRAZOR_VOLCANO,
    DATA_SULFURON_DOOR,
    DATA_RAGNAROS_PLATFORM,

    // misc
    DATA_GROUND_STATE = 42,
    DATA_METEOR_SUMMON_COUNT,

    // Achievements
    DATA_DEATH_FROM_ABOVE,
    DATA_KNEEL_TO_FLAME
};

enum GameObjectIds
{
    GO_DOOR_BETHILAC                = 208877,
    GO_DOOR_BALOROC_AFTER           = 209277,
    GO_DOOR_BALOROC                 = 209066,
    GO_GROUND_ALYS                  = 209252, // destructable
    GO_VOLCANO_ALYS                 = 209253, // destructable
    GO_WALL_SULFURON                = 208873,
    GO_MAJ_DOOR                     = 208906,
    GO_MAJA_DOOR                    = 208873,
    GO_SULFURON_BRIDGE              = 209251,
    GO_RAGNAROS_DOOR                = 209073,
    GO_RAGNAROS_PLATFORM            = 208835,
    GO_CACHE_OF_THE_FIRELORD_10     = 209261,
    GO_CACHE_OF_THE_FIRELORD_25     = 208967,
    GO_ICE_BLOCK_COLLISION          = 201722,
    GO_RHYOLITH_BRIDGE              = 209255,
};

enum CreatureIds
{
    //Ragnaros
    BOSS_RAGNAROS                   = 52409,
    NPC_SULFURAS_SMASH              = 53268,
    NPC_SULFURAS_HAMMER             = 53420,
    NPC_MAGMA_TRAP                  = 53086,
    NPC_SPLITTING_BLOW              = 53393,
    NPC_ENGULFING_FLAMES            = 53485,
    NPC_MOLTEN_SEED_CASTER          = 53186,
    NPC_LAVA_WAVE                   = 53363,
    NPC_LAVA_SCION                  = 53231,
    NPC_SON_OF_FLAME                = 53140,
    NPC_LIVING_METEOR               = 53500,
    NPC_MOLTEN_ELEMENTAL            = 53189,
    NPC_SULFURAS_HAND_OF_RAGNAROS   = 53420,
    NPC_ENTRAPPING_ROOTS            = 54074,
    NPC_DREADFLAME                  = 54127,
    NPC_CLOUDBURST                  = 54147,
    NPC_DREADFLAME_SPAWN            = 54203,
    NPC_BREADTH_OF_FROST            = 53953,
    NPC_HAMUUL                      = 53876,
    NPC_MALFURION                   = 53875,
    NPC_CENARIUS                    = 53872,
    NPC_HEART_OF_RAGNAROS           = 54293,
    NPC_SULFURAS_SMASH_TARGET       = 53268,
    NPC_SULFURAS_SMASH_TRIGGER      = 53266,
    NPC_ENGULFING_FLAMES_TRIGGER    = 53485,
    NPC_SPLITTING_BLOW_TRIGGER      = 53393,
    NPC_BLAZING_HEAT                = 53473,
    NPC_MAGMA_POOL_TRIGGER          = 53729,
    NPC_PLATFORM_TRIGGER            = 53952,
    NPC_MOLTEN_ERUPTER              = 53617,
    NPC_MOLTEN_SPEWER               = 53545,
    NPC_MAGMA_GEYSER                = 54184,

    // Baleroc
    BOSS_BALEROC                    = 53494,
    NPC_CRYSTAL_SHARD               = 53495,

    // Alyrazor
    BOSS_ALYSRAZOR                  = 52530,
    NPC_BLAZING_MONSTROSITY_LEFT    = 53786,
    NPC_BLAZING_MONSTROSITY_RIGHT   = 53791,
    NPC_EGG_PILE                    = 53795,
    NPC_HARBINGER_OF_FLAME          = 53793,
    NPC_MOLTEN_EGG_TRASH            = 53914,
    NPC_SMOULDERING_HATCHLING       = 53794,
    NPC_MAJORDOMO_EVENT_INTRO       = 54015,

    // Shannox
    BOSS_SHANNOX                    = 53691,
    NPC_RIPLIMB                     = 53694, // Shannox Dogs
    NPC_RAGEFACE                    = 53695,
    NPC_SHANNOX_SPEAR               = 53752, // Shannox Spear
    NPC_SHANNOX_GRIP_SPEAR          = 54112,
    NPC_CRYSTAL_TRAP                = 53713,
    NPC_CRYSTAL_PRISON              = 53819,
    NPC_VISUAL_CIRCLE               = 54030,
    NPC_IMMOLATION_TRAP             = 53724,
    NPC_SHANNOX_CONTROLLER          = 53910,

    // Bethilac
    BOSS_BETHTILAC                  = 52498,
    NPC_CINDERWEB_SPINNER           = 52524,
    NPC_CINDERWEB_DRONE             = 52581,
    NPC_CINDERWEB_SPIDERLING        = 52447,
    NPC_ENGORGED_BROODLING          = 53745,
    NPC_SPIDERWEB_FILAMENT          = 53082, // This is the Elevator
    NPC_WEB_RIP                     = 53450,
    NPC_LIFT_CONTROLLER             = 52528,
    NPC_DESTINATION_STALKER         = 53237,
    NPC_SPIDERLING_STALKER          = 53178,
    NPC_BETHTILAC_VEHICLE           = 53529,

    //Majordomo Staghelm
    BOSS_MAJORDOMO_STAGHELM         = 52571,
    NPC_SPIRIT_OF_THE_FLAME         = 52593,
    NPC_DRUID_OF_THE_FLAME          = 53619,
    NPC_INVISIBLE_STALKER           = 42098,
    NPC_MAGMA_ORB                   = 54299,

    // Lord Rhyolith
    BOSS_RHYOLITH                   = 52558,
    NPC_LEFT_FOOT                   = 52577,
    NPC_RIGHT_FOOT                  = 53087,
    NPC_VOLCANO                     = 52582,
    NPC_CRATER                      = 52866,
    NPC_MOVEMENT_CONTROLLER         = 52659,
    NPC_PILLAR                      = 53122,
    NPC_LIQUID_OBSIDIAN             = 52619,
    NPC_FRAGMENT_OF_RHYOLITH        = 52620,
    NPC_SPARK_OF_RHYOLITH           = 53211,
    NPC_MAGMA_CHECK_TRIGGER         = 52110,
    NPC_ARMOR_FRAGMENT_HEROIC       = 52619,

    // Misc
    NPC_FIRELANDS_TELEPORT_MAJOR    = 54367,
    NPC_FIRELANDS_TELEPORT_SHANNOX  = 54348,

    NPC_FANDRALS_FLAME              = 53696,
};

enum Phase
{
    //Shannox
    PHASE_NON                       = 0,
    PHASE_SHANNOX_HAS_SPEAR         = 1,
    PHASE_SPEAR_ON_THE_GROUND       = 2,
    PHASE_RIPLIMB_GOES_TO_SPEAR     = 3,
    PHASE_RIPLIMB_BRINGS_SPEAR      = 4,
};

enum PathIDs
{
    PATH_SHANNOX                    = 154280940,
};

enum Actions
{
    // Ragnaros
    ACTION_START_DANCE,
    ACTION_SON_KILLED,
    ACTION_ACTIVATE_SON,
    ACTION_INSTANT_EMERGE,
    ACTION_ACTIVATE_HEROIC,
    ACTION_SUBMERGE,
    ACTION_HEART_OF_FLAME_FAILED,

    // Archdruids
    ACTION_SCHEDULE_CLOUDBURST,
    ACTION_SCHEDULE_ROOTS,
    ACTION_SCHEDULE_BREADTH,
    ACTION_SCHEDULE_OUTRO,

    // Molten Worms
    ACTION_EMERGE_WORM,

    // Majordomos
    ACTION_START_INTRO,
    ACTION_START_INTRO2,
};

enum Miscs
{
    BUILDING_STATE_DESTROYED = 42,
    BUILDING_STATE_INTACT = 43,
    FANDRALL_ACTIVATION_COUNT = 6,
    SPELL_FANDRALL_ACHIEVEMENT_CHECK = 99763,
    SPELL_FANDRALL_PLAYER_POSITIONS = 99795,
    SPELL_FANDRALL_VISUEL_ACTIVATION_AURA = 99796 
};

enum LegendaryLine
{
    ITEM_HEART_OF_FLAME     = 69848,
    SPELL_SMOULDERING       = 101093
};

enum ProcessedEvents
{
    EVENT_FANDRALL_LEFT_ACTIVATION = 16624,
    EVENT_FANDRALL_RIGHT_ACTIVATION = 28303,
};

enum TimedEvents
{
    EVENT_DESACTIVATE_FANDRALL = 1,
};

class DelayedAttackStartEvent : public BasicEvent
{
    public:
        DelayedAttackStartEvent(Creature* owner) : _owner(owner) { }

        bool Execute(uint64 /*e_time*/, uint32 /*p_time*/)
        {
            _owner->AI()->DoZoneInCombat(_owner, 200.0f);
            return true;
        }

    private:
        Creature* _owner;
};

template<class AI>
inline AI* GetFirelandsAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(FirelandsScriptName))
                return new AI(creature);
    return NULL;
}

#endif // FIRELANDS_H_
