//////////////////////////////////////////////////////////////////////////
///
///  PROJECT AZENOR
/// COPYRIGHT 2018-2019
/// ALL RIGHTS RESERVED.
///
//////////////////////////////////////////////////////////////////////////

#pragma once

/// Common
#include "Common.h"

/// Shared
#include "ByteBuffer.h"

/// Game
#include "SharedDefines.h"

/// Available SmartScriptTypes
enum SmartScriptType : uint32
{
    SMART_SCRIPT_TYPE_CREATURE          =  0,
    SMART_SCRIPT_TYPE_GAMEOBJECT        =  1,
    SMART_SCRIPT_TYPE_AREATRIGGER       =  2,
    SMART_SCRIPT_TYPE_EVENT             =  3,
    SMART_SCRIPT_TYPE_GOSSIP            =  4, ///< NYI
    SMART_SCRIPT_TYPE_QUEST             =  5,
    SMART_SCRIPT_TYPE_SPELL             =  6,
    SMART_SCRIPT_TYPE_TRANSPORT         =  7, ///< NYI
    SMART_SCRIPT_TYPE_INSTANCE          =  8, ///< NYI
    SMART_SCRIPT_TYPE_TIMED_ACTIONLIST  =  9,
    SMART_SCRIPT_TYPE_WAYPOINT          = 10,

    /// Add more types you want here

    SMART_SCRIPT_TYPE_MAX,
};

struct WayPoint
{
    WayPoint(uint32 _id, float _x, float _y, float _z) : 
        id(_id), x(_x), y(_y), z(_z) { }

    uint32 id;
    float x;
    float y;
    float z;
};

enum eSmartAI
{
    SMART_EVENT_PARAM_COUNT     = 4,
    SMART_ACTION_PARAM_COUNT    = 6,
    SMART_SUMMON_COUNTER        = 0xFFFFFF,
    SMART_ESCORT_LAST_OOC_POINT = 0xFFFFFF,
    SMART_RANDOM_POINT          = 0xFFFFFE,
    SMART_ESCORT_TARGETS        = 0xFFFFFF
};

enum SMART_EVENT_PHASE : uint32
{
    SMART_EVENT_PHASE_ALWAYS  = 0,
    SMART_EVENT_PHASE_1,
    SMART_EVENT_PHASE_2,
    SMART_EVENT_PHASE_3,
    SMART_EVENT_PHASE_4,
    SMART_EVENT_PHASE_5,
    SMART_EVENT_PHASE_6,
    SMART_EVENT_PHASE_7,
    SMART_EVENT_PHASE_8,
    SMART_EVENT_PHASE_9,
    SMART_EVENT_PHASE_10,
    SMART_EVENT_PHASE_11,
    SMART_EVENT_PHASE_12,
    SMART_EVENT_PHASE_13,
    SMART_EVENT_PHASE_14,
    SMART_EVENT_PHASE_15,
    SMART_EVENT_PHASE_16,
    SMART_EVENT_PHASE_17,
    SMART_EVENT_PHASE_18,
    SMART_EVENT_PHASE_19,
    SMART_EVENT_PHASE_20,
    SMART_EVENT_PHASE_21,
    SMART_EVENT_PHASE_22,
    SMART_EVENT_PHASE_23,
    SMART_EVENT_PHASE_24,
    SMART_EVENT_PHASE_MAX,

    SMART_EVENT_PHASE_COUNT = SMART_EVENT_PHASE_MAX - 1
};

enum SMART_EVENT_PHASE_BITS
{
    SMART_EVENT_PHASE_ALWAYS_BIT   = 0,
    SMART_EVENT_PHASE_1_BIT        = 1,
    SMART_EVENT_PHASE_2_BIT        = 2,
    SMART_EVENT_PHASE_3_BIT        = 4,
    SMART_EVENT_PHASE_4_BIT        = 8,
    SMART_EVENT_PHASE_5_BIT        = 16,
    SMART_EVENT_PHASE_6_BIT        = 32,
    SMART_EVENT_PHASE_7_BIT        = 64,
    SMART_EVENT_PHASE_8_BIT        = 128,
    SMART_EVENT_PHASE_9_BIT        = 256,
    SMART_EVENT_PHASE_ALL = SMART_EVENT_PHASE_1_BIT + SMART_EVENT_PHASE_2_BIT + SMART_EVENT_PHASE_3_BIT + SMART_EVENT_PHASE_4_BIT + SMART_EVENT_PHASE_5_BIT + SMART_EVENT_PHASE_6_BIT + SMART_EVENT_PHASE_7_BIT + SMART_EVENT_PHASE_8_BIT + SMART_EVENT_PHASE_9_BIT
};

const uint32 SmartPhaseMask[SMART_EVENT_PHASE_COUNT][2] =
{
    {SMART_EVENT_PHASE_1, SMART_EVENT_PHASE_1_BIT },
    {SMART_EVENT_PHASE_2, SMART_EVENT_PHASE_2_BIT },
    {SMART_EVENT_PHASE_3, SMART_EVENT_PHASE_3_BIT },
    {SMART_EVENT_PHASE_4, SMART_EVENT_PHASE_4_BIT },
    {SMART_EVENT_PHASE_5, SMART_EVENT_PHASE_5_BIT },
    {SMART_EVENT_PHASE_6, SMART_EVENT_PHASE_6_BIT },
    {SMART_EVENT_PHASE_7, SMART_EVENT_PHASE_7_BIT },
    {SMART_EVENT_PHASE_8, SMART_EVENT_PHASE_8_BIT },
    {SMART_EVENT_PHASE_9, SMART_EVENT_PHASE_9_BIT }
};

enum SMART_EVENT : uint32
{
    SMART_EVENT_UPDATE_IC                =   0,     // InitialMin, InitialMax, RepeatMin, RepeatMax
    SMART_EVENT_UPDATE_OOC               =   1,     // InitialMin, InitialMax, RepeatMin, RepeatMax
    SMART_EVENT_HEALT_PCT                =   2,     // HPMin%, HPMax%,  RepeatMin, RepeatMax
    SMART_EVENT_MANA_PCT                 =   3,     // ManaMin%, ManaMax%, RepeatMin, RepeatMax
    SMART_EVENT_AGGRO                    =   4,     // NONE
    SMART_EVENT_KILL                     =   5,     // CooldownMin0, CooldownMax1, playerOnly2, else creature entry3
    SMART_EVENT_DEATH                    =   6,     // NONE
    SMART_EVENT_EVADE                    =   7,     // NONE
    SMART_EVENT_SPELLHIT                 =   8,     // SpellID, School, CooldownMin, CooldownMax
    SMART_EVENT_RANGE                    =   9,     // MinDist, MaxDist, RepeatMin, RepeatMax
    SMART_EVENT_OOC_LOS                  =  10,      // NoHostile, MaxRange, CooldownMin, CooldownMax, playerOnly
    SMART_EVENT_RESPAWN                  =  11,      // type, MapId, ZoneId
    SMART_EVENT_TARGET_HEALTH_PCT        =  12,      // HPMin%, HPMax%, RepeatMin, RepeatMax
    SMART_EVENT_VICTIM_CASTING           =  13,      // RepeatMin, RepeatMax, spellid
    SMART_EVENT_FRIENDLY_HEALTH          =  14,      // HPDeficit, Radius, RepeatMin, RepeatMax
    SMART_EVENT_FRIENDLY_IS_CC           =  15,      // Radius, RepeatMin, RepeatMax
    SMART_EVENT_FRIENDLY_MISSING_BUFF    =  16,      // SpellId, Radius, RepeatMin, RepeatMax
    SMART_EVENT_SUMMONED_UNIT            =  17,      // CreatureId(0 all), CooldownMin, CooldownMax
    SMART_EVENT_TARGET_MANA_PCT          =  18,      // ManaMin%, ManaMax%, RepeatMin, RepeatMax
    SMART_EVENT_ACCEPTED_QUEST           =  19,      // QuestID(0any)
    SMART_EVENT_REWARD_QUEST             =  20,      // QuestID(0any)
    SMART_EVENT_REACHED_HOME             =  21,      // NONE
    SMART_EVENT_RECEIVE_EMOTE            =  22,      // EmoteId, CooldownMin, CooldownMax, condition, val1, val2, val3
    SMART_EVENT_HAS_AURA                 =  23,      // Param1 = SpellID, Param2 = Number of Time STacked, Param3/4 RepeatMin, RepeatMax
    SMART_EVENT_TARGET_BUFFED            =  24,      // Param1 = SpellID, Param2 = Number of Time STacked, Param3/4 RepeatMin, RepeatMax
    SMART_EVENT_RESET                    =  25,      // Called after combat, when the creature respawn and spawn.
    SMART_EVENT_IC_LOS                   =  26,      // NoHostile, MaxRange, CooldownMin, CooldownMax, playerOnly
    SMART_EVENT_PASSENGER_BOARDED        =  27,      // CooldownMin, CooldownMax
    SMART_EVENT_PASSENGER_REMOVED        =  28,      // CooldownMin, CooldownMax
    SMART_EVENT_CHARMED                  =  29,      // onRemove (0 - on apply, 1 - on remove)
    SMART_EVENT_CHARMED_TARGET           =  30,      // NONE
    SMART_EVENT_SPELLHIT_TARGET          =  31,      // SpellID, School, CooldownMin, CooldownMax
    SMART_EVENT_DAMAGED                  =  32,      // MinDmg, MaxDmg, CooldownMin, CooldownMax
    SMART_EVENT_DAMAGED_TARGET           =  33,      // MinDmg, MaxDmg, CooldownMin, CooldownMax
    SMART_EVENT_MOVEMENTINFORM           =  34,      // MovementType(any), PointID
    SMART_EVENT_SUMMON_DESPAWNED         =  35,      // Entry, CooldownMin, CooldownMax
    SMART_EVENT_CORPSE_REMOVED           =  36,      // NONE
    SMART_EVENT_AI_INIT                  =  37,      // NONE
    SMART_EVENT_DATA_SET                 =  38,      // Id, Value, CooldownMin, CooldownMax
    SMART_EVENT_WAYPOINT_START           =  39,      // PointId(0any), pathID(0any)
    SMART_EVENT_WAYPOINT_REACHED         =  40,      // PointId(0any), pathID(0any)
    SMART_EVENT_AURA_APPLIED             =  41,      ///< SpellId (0 any)
    SMART_EVENT_AURA_REMOVED             =  42,      ///< SpellId (0 any)
    SMART_EVENT_AFTER_PASSENGER_REMOVED  =  43,
    SMART_EVENT_VEHICLE_EXIT_REQUSTED    =  44,
    SMART_EVENT_BEFORE_DEATH             =  45,
    SMART_EVENT_AREATRIGGER_ONTRIGGER    =  46,      // TriggerId(0 any)
    SMART_EVENT_VEHICLE_EXIT             =  47,      // none
    SMART_EVENT_UNUSED_48                =  48,      // none
    SMART_EVENT_UNUSED_49                =  49,      // none
    SMART_EVENT_UNUSED_50                =  50,      // none
    SMART_EVENT_UNUSED_51                =  51,      // none
    SMART_EVENT_TEXT_OVER                =  52,      // GroupId from creature_text,  creature entry who talks (0 any)
    SMART_EVENT_RECEIVE_HEAL             =  53,      // MinHeal, MaxHeal, CooldownMin, CooldownMax
    SMART_EVENT_JUST_SUMMONED            =  54,      // none
    SMART_EVENT_WAYPOINT_PAUSED          =  55,      // PointId(0any), pathID(0any)
    SMART_EVENT_WAYPOINT_RESUMED         =  56,      // PointId(0any), pathID(0any)
    SMART_EVENT_WAYPOINT_STOPPED         =  57,      // PointId(0any), pathID(0any)
    SMART_EVENT_WAYPOINT_ENDED           =  58,      // PointId(0any), pathID(0any)
    SMART_EVENT_TIMED_EVENT_TRIGGERED    =  59,      // id
    SMART_EVENT_UPDATE                   =  60,      // InitialMin, InitialMax, RepeatMin, RepeatMax
    SMART_EVENT_LINK                     =  61,      // INTERNAL USAGE, no params, used to link together multiple events, does not use any extra resources to iterate event lists needlessly
    SMART_EVENT_GOSSIP_SELECT            =  62,      // menuID, actionID
    SMART_EVENT_JUST_CREATED             =  63,      // none
    SMART_EVENT_GOSSIP_HELLO             =  64,      // none
    SMART_EVENT_FOLLOW_COMPLETED         =  65,      // none
    SMART_EVENT_DUMMY_EFFECT             =  66,      // spellId, effectIndex
    SMART_EVENT_IS_BEHIND_TARGET         =  67,      // cooldownMin, CooldownMax
    SMART_EVENT_GAME_EVENT_START         =  68,      // game_event.Entry
    SMART_EVENT_GAME_EVENT_END           =  69,      // game_event.Entry
    SMART_EVENT_GO_STATE_CHANGED         =  70,      // go state
    SMART_EVENT_GO_EVENT_INFORM          =  71,      // eventId
    SMART_EVENT_ACTION_DONE              =  72,      // eventId (SharedDefines.EventId)
    SMART_EVENT_ON_SPELLCLICK            =  73,      // clicker (unit)
    SMART_EVENT_FRIENDLY_HEALTH_PCT      =  74,      // minHpPct, maxHpPct, repeatMin, repeatMax
    SMART_EVENT_DISTANCE_CREATURE        =  75,      // guid, entry, distance, repeat
    SMART_EVENT_DISTANCE_GAMEOBJECT      =  76,      // guid, entry, distance, repeat
    SMART_EVENT_COUNTER_SET              =  77,      // id, value, cooldownMin, cooldownMax
    SMART_EVENT_DISTANCE_PLAYER          =  78,      // range, aura, ispresent, repeat
    SMART_EVENT_QUEST_ACCEPT             = 100,
    SMART_EVENT_QUEST_COMPLETE           = 101,
    SMART_EVENT_QUEST_REWARD             = 102,
    SMART_EVENT_QUEST_FAIL               = 103,
    SMART_EVENT_QUEST_ABANDON            = 104,

    SMART_EVENT_END,
};

struct SmartEvent
{
    SMART_EVENT type;
    uint32 event_phase_mask;
    uint32 event_chance;
    uint32 event_flags;
    union
    {
        /// SMART_EVENT_UPDATE_IC
        /// SMART_EVENT_UPDATE_OOC
        /// SMART_EVENT_HEALTH_PCT
        /// SMART_EVENT_MANA_PCT
        struct
        {
            uint32 min;
            uint32 max;
            uint32 repeatMin;
            uint32 repeatMax;
        } minMaxRepeat;

        /// SMART_EVENT_KILL
        struct
        {
            uint32 cooldownMin;
            uint32 cooldownMax;
            uint32 playerOnly;
            uint32 creature;
        } kill;

        /// SMART_EVENT_SPELLHIT
        struct
        {
            uint32 spell;
            uint32 school;
            uint32 cooldownMin;
            uint32 cooldownMax;
        } spellHit;

        /// SMART_EVENT_OOC_LOS
        /// SMART_EVENT_IC_LOS
        struct
        {
            uint32 noHostile;
            uint32 maxDist;
            uint32 cooldownMin;
            uint32 cooldownMax;
            uint32 playerOnly;
        } los;

        struct
        {
            uint32 type;
            uint32 map;
            uint32 area;
        } respawn;

        struct
        {
            uint32 repeatMin;
            uint32 repeatMax;
        } minMax;

        struct
        {
            uint32 repeatMin;
            uint32 repeatMax;
            uint32 spellId;
        } targetCasting;

        struct
        {
            uint32 hpDeficit;
            uint32 radius;
            uint32 repeatMin;
            uint32 repeatMax;
        } friendlyHealth;

        struct
        {
            uint32 radius;
            uint32 repeatMin;
            uint32 repeatMax;
        } friendlyCC;

        struct
        {
            uint32 spell;
            uint32 radius;
            uint32 repeatMin;
            uint32 repeatMax;
        } missingBuff;

        struct
        {
            uint32 creature;
            uint32 cooldownMin;
            uint32 cooldownMax;
        } summoned;

        struct
        {
            uint32 quest;
        } quest;

        /// Verified 5-27-19
        /// SMART_ACTION_ADD_QUEST
        struct
        {
            uint32 QuestID;
            uint32 CheckRequirements;
            uint32 ShowPopup;
        } AddQuest;

        struct
        {
            uint32 emote;
            uint32 cooldownMin;
            uint32 cooldownMax;
        } emote;

        struct
        {
            uint32 spell;
            uint32 count;
            uint32 repeatMin;
            uint32 repeatMax;
        } aura;

        struct
        {
            uint32 onRemove;
        } charm;

        struct
        {
            uint32 spell;
            uint32 count;
            uint32 repeatMin;
            uint32 repeatMax;
        } targetAura;

        struct
        {
            uint32 type;
            uint32 id;
        } movementInform;

        struct
        {
            uint32 id;
            uint32 value;
            uint32 cooldownMin;
            uint32 cooldownMax;
        } dataSet;

        struct
        {
            uint32 pointID;
            uint32 pathID;
        } waypoint;

        struct
        {
            uint32 team;
            uint32 cooldownMin;
            uint32 cooldownMax;
        } instancePlayerEnter;

        struct
        {
            uint32 id;
        } areatrigger;

        struct
        {
            uint32 textGroupID;
            uint32 creatureEntry;
        } textOver;

        struct
        {
            uint32 id;
        } timedEvent;

        struct
        {
            uint32 sender;
            uint32 action;
        } gossip;

        struct
        {
            uint32 spell;
            uint32 effIndex;
        } dummy;

        struct
        {
            uint32 cooldownMin;
            uint32 cooldownMax;
        } behindTarget;

        struct
        {
            uint32 gameEventId;
        } gameEvent;

        struct
        {
            uint32 state;
        } goStateChanged;

        struct
        {
            uint32 eventId;
        } eventInform;

        struct
        {
            uint32 eventId;
        } doAction;

        struct
        {
            uint32 minHpPct;
            uint32 maxHpPct;
            uint32 repeatMin;
            uint32 repeatMax;
        } friendlyHealthPct;

        struct
        {
            uint32 guid;
            uint32 entry;
            uint32 dist;
            uint32 repeat;
        } distance;

        struct
        {
            uint32 range;
            uint32 aura;
            uint32 isPresent;
            uint32 repeat;
        } playerDistance;

        struct
        {
            uint32 id;
            uint32 value;
            uint32 cooldownMin;
            uint32 cooldownMax;
        } counter;

        /// SMART_EVENT_AURA_APPLIED 41
        /// SMART_EVENT_AURA_REMOVED 42
        struct
        {
            uint32 SpellID;
        } AuraAppliedOrRemoved;

        struct
        {

            uint32 param1;
            uint32 param2;
            uint32 param3;
            uint32 param4;
            uint32 param5;
        } raw;
    };
};

enum SMART_SCRIPT_RESPAWN_CONDITION : uint32
{
    SMART_SCRIPT_RESPAWN_CONDITION_NONE     = 0,
    SMART_SCRIPT_RESPAWN_CONDITION_MAP      = 1,
    SMART_SCRIPT_RESPAWN_CONDITION_AREA     = 2,
    SMART_SCRIPT_RESPAWN_CONDITION_END      = 3
};

enum SMART_ACTION : uint32
{
    SMART_ACTION_NONE                               =   0,    // No action
    SMART_ACTION_TALK                               =   1,    // groupID from creature_text, duration to wait before TEXT_OVER event is triggered
    SMART_ACTION_SET_FACTION                        =   2,    // FactionId (or 0 for default)
    SMART_ACTION_MORPH_TO_ENTRY_OR_MODEL            =   3,    // Creature_template entry(param1) OR ModelId (param2) (or 0 for both to demorph)
    SMART_ACTION_SOUND                              =   4,    // SoundId, TextRange
    SMART_ACTION_PLAY_EMOTE                         =   5,    // EmoteId
    SMART_ACTION_FAIL_QUEST                         =   6,    // QuestID
    SMART_ACTION_ADD_QUEST                          =   7,    // QuestID
    SMART_ACTION_SET_REACT_STATE                    =   8,    // state
    SMART_ACTION_ACTIVATE_GOBJECT                   =   9,    //
    SMART_ACTION_RANDOM_EMOTE                       =  10,    // EmoteId1, EmoteId2, EmoteId3...
    SMART_ACTION_CAST                               =  11,    // SpellId, CastFlags
    SMART_ACTION_SUMMON_CREATURE                    =  12,    // CreatureID, summonType, duration in ms, storageID, attackInvoker,
    SMART_ACTION_THREAT_SINGLE_PCT                  =  13,    // Threat%
    SMART_ACTION_THREAT_ALL_PCT                     =  14,    // Threat%
    SMART_ACTION_CALL_AREAEXPLOREDOREVENTHAPPENS    =  15,    // QuestID
    SMART_ACTION_SET_INGAME_PHASE_GROUP             =  16,    ///< Creature.PhaseGroup, 1=apply, 0=remove
    SMART_ACTION_SET_EMOTE_STATE                    =  17,    // emoteID
    SMART_ACTION_SET_UNIT_FLAG                      =  18,    // Flags (may be more than one field OR'd together), Target
    SMART_ACTION_REMOVE_UNIT_FLAG                   =  19,    // Flags (may be more than one field OR'd together), Target
    SMART_ACTION_AUTO_ATTACK                        =  20,    // AllowAttackState (0 = stop attack, anything else means continue attacking)
    SMART_ACTION_ALLOW_COMBAT_MOVEMENT              =  21,    // AllowCombatMovement (0 = stop combat based movement, anything else continue attacking)
    SMART_ACTION_SET_EVENT_PHASE                    =  22,    // PhaseID
    SMART_ACTION_INC_EVENT_PHASE                    =  23,    // Increment, Decrement
    SMART_ACTION_EVADE                              =  24,    // No Params
    SMART_ACTION_FLEE_FOR_ASSIST                    =  25,    // With Emote
    SMART_ACTION_CALL_GROUPEVENTHAPPENS             =  26,    // QuestID
    SMART_ACTION_COMBAT_STOP                        =  27,    // CombatStop
    SMART_ACTION_REMOVE_AURAS                       =  28,    // Spellid, 0 removes all auras, charges (0 removes aura)
    SMART_ACTION_FOLLOW                             =  29,    // Distance (0 = default), Angle (0 = default), EndCreatureEntry, credit, creditType (0monsterkill, 1event)
    SMART_ACTION_RANDOM_PHASE                       =  30,    // PhaseId1, PhaseId2, PhaseId3...
    SMART_ACTION_RANDOM_PHASE_RANGE                 =  31,    // PhaseMin, PhaseMax
    SMART_ACTION_RESET_GOBJECT                      =  32,    //
    SMART_ACTION_CALL_KILLEDMONSTER                 =  33,    // CreatureId,
    SMART_ACTION_SET_INST_DATA                      =  34,    // Field, Data
    SMART_ACTION_SET_INST_DATA64                    =  35,    // Field,
    SMART_ACTION_UPDATE_TEMPLATE                    =  36,    // Entry, Team, Level
    SMART_ACTION_DIE                                =  37,    // No Params
    SMART_ACTION_SET_IN_COMBAT_WITH_ZONE            =  38,    // No Params
    SMART_ACTION_CALL_FOR_HELP                      =  39,    // Radius, With Emote
    SMART_ACTION_SET_SHEATH                         =  40,    // Sheath (0-unarmed, 1-melee, 2-ranged)
    SMART_ACTION_FORCE_DESPAWN                      =  41,    // timer
    SMART_ACTION_SET_INVINCIBILITY_HP_LEVEL         =  42,    // MinHpValue(+pct, -flat)
    SMART_ACTION_MOUNT_TO_ENTRY_OR_MODEL            =  43,    // Creature_template entry(param1) OR ModelId (param2) (or 0 for both to dismount)
    SMART_ACTION_SET_INGAME_PHASE_ID                =  44,    // PhaseId, Apply Remove
    SMART_ACTION_SET_DATA                           =  45,    // Field, Data
    SMART_ACTION_MOVE_FORWARD                       =  46,    // distance
    SMART_ACTION_SET_VISIBILITY                     =  47,    // on/off
    SMART_ACTION_SET_ACTIVE                         =  48,    // on/off
    SMART_ACTION_ATTACK_START                       =  49,    //
    SMART_ACTION_SUMMON_GO                          =  50,    // GameObjectID, DespawnTime in ms,
    SMART_ACTION_KILL_UNIT                          =  51,    //
    SMART_ACTION_ACTIVATE_TAXI                      =  52,    // TaxiID
    SMART_ACTION_WP_START                           =  53,    // run/walk, pathID, canRepeat, quest, despawntime, reactState
    SMART_ACTION_WP_PAUSE                           =  54,    // time
    SMART_ACTION_WP_STOP                            =  55,    // despawnTime, quest, fail?
    SMART_ACTION_ADD_ITEM                           =  56,    // itemID, count
    SMART_ACTION_REMOVE_ITEM                        =  57,    // itemID, count
    SMART_ACTION_INSTALL_AI_TEMPLATE                =  58,    // AITemplateID
    SMART_ACTION_SET_RUN                            =  59,    // 0/1
    SMART_ACTION_SET_FLY                            =  60,    // 0/1
    SMART_ACTION_SET_SWIM                           =  61,    // 0/1
    SMART_ACTION_TELEPORT                           =  62,    // mapID,
    SMART_ACTION_SET_COUNTER                        =  63,    // id, value, reset (0/1)
    SMART_ACTION_STORE_TARGET_LIST                  =  64,    // varID,
    SMART_ACTION_WP_RESUME                          =  65,    // none
    SMART_ACTION_SET_ORIENTATION                    =  66,    //
    SMART_ACTION_CREATE_TIMED_EVENT                 =  67,    // id, InitialMin, InitialMax, RepeatMin(only if it repeats), RepeatMax(only if it repeats), chance
    SMART_ACTION_PLAYMOVIE                          =  68,    // entry
    SMART_ACTION_MOVE_TO_POS                        =  69,    // PointId, xyz, disablePathfinding
    SMART_ACTION_RESPAWN_TARGET                     =  70,    //
    SMART_ACTION_EQUIP                              =  71,    // entry, slotmask slot1, slot2, slot3   , only slots with mask set will be sent to client, bits are 1, 2, 4, leaving mask 0 is defaulted to mask 7 (send all), slots1-3 are only used if no entry is set
    SMART_ACTION_CLOSE_GOSSIP                       =  72,    // none
    SMART_ACTION_TRIGGER_TIMED_EVENT                =  73,    // id(>1)
    SMART_ACTION_REMOVE_TIMED_EVENT                 =  74,    // id(>1)
    SMART_ACTION_ADD_AURA                           =  75,    // spellid,  targets
    SMART_ACTION_OVERRIDE_SCRIPT_BASE_OBJECT        =  76,    // WARNING: CAN CRASH CORE, do not use if you dont know what you are doing
    SMART_ACTION_RESET_SCRIPT_BASE_OBJECT           =  77,    // none
    SMART_ACTION_CALL_SCRIPT_RESET                  =  78,    // none
    SMART_ACTION_SET_RANGED_MOVEMENT                =  79,    // Distance, angle
    SMART_ACTION_CALL_TIMED_ACTIONLIST              =  80,    // ID (overwrites already running actionlist), stop after combat?(0/1), timer update type(0-OOC, 1-IC, 2-ALWAYS)
    SMART_ACTION_SET_NPC_FLAG                       =  81,    // Flags
    SMART_ACTION_ADD_NPC_FLAG                       =  82,    // Flags
    SMART_ACTION_REMOVE_NPC_FLAG                    =  83,    // Flags
    SMART_ACTION_SIMPLE_TALK                        =  84,    // groupID, can be used to make players say groupID, Text_over event is not triggered, whisper can not be used (Target units will say the text)
    SMART_ACTION_INVOKER_CAST                       =  85,    // spellID, castFlags,   if avaliable, last used invoker will cast spellId with castFlags on targets
    SMART_ACTION_CROSS_CAST                         =  86,    // spellID, castFlags, CasterTargetType, CasterTarget param1, CasterTarget param2, CasterTarget param3, ( + the origonal target fields as Destination target),   CasterTargets will cast spellID on all Targets (use with caution if targeting multiple * multiple units)
    SMART_ACTION_CALL_RANDOM_TIMED_ACTIONLIST       =  87,    // script9 ids 1-9
    SMART_ACTION_CALL_RANDOM_RANGE_TIMED_ACTIONLIST =  88,    // script9 id min, max
    SMART_ACTION_RANDOM_MOVE                        =  89,    // maxDist
    SMART_ACTION_SET_UNIT_FIELD_ANIM_TIER           =  90,    // bytes, target
    SMART_ACTION_REMOVE_UNIT_FIELD_ANIM_TIER        =  91,    // bytes, target
    SMART_ACTION_INTERRUPT_SPELL                    =  92,
    SMART_ACTION_SEND_GO_CUSTOM_ANIM                =  93,    // anim id
    SMART_ACTION_SET_STAND_STATE                    =  94,
    SMART_ACTION_APPLY_STAND_FLAGS                  =  95,
    SMART_ACTION_APPLY_STAND_MISC_FLAGS             =  96,
    SMART_ACTION_JUMP_TO_POS                        =  97,    // speedXY, speedZ, targetX, targetY, targetZ
    SMART_ACTION_SEND_GOSSIP_MENU                   =  98,    // menuId, optionId
    SMART_ACTION_GO_SET_LOOT_STATE                  =  99,    // state
    SMART_ACTION_SEND_TARGET_TO_TARGET              = 100,    // id
    SMART_ACTION_SET_HOME_POS                       = 101,    // none
    SMART_ACTION_SET_HEALTH_REGEN                   = 102,    // 0/1
	SMART_ACTION_SET_ROOT                           = 103,
    SMART_ACTION_SET_GO_FLAG                        = 104,    // Flags
    SMART_ACTION_ADD_GO_FLAG                        = 105,    // Flags
    SMART_ACTION_REMOVE_GO_FLAG                     = 106,    // Flags
    SMART_ACTION_SUMMON_CREATURE_GROUP              = 107,    // Group, attackInvoker
    SMART_ACTION_SET_POWER                          = 108,    // PowerType, newPower
    SMART_ACTION_ADD_POWER                          = 109,    // PowerType, newPower
    SMART_ACTION_REMOVE_POWER                       = 110,    // PowerType, newPower
	SMART_ACTION_START_CLOSEST_WAYPOINT             = 113,    // wp1, wp2, wp3, wp4, wp5, wp6, wp7
    SMART_ACTION_PLAY_RANDOM_SOUND                  = 115,
    SMART_ACTION_DISABLE_EVADE                      = 117,    // 0/1 (1 = disabled, 0 = enabled)

    // Not used in DB currently. 
    SMART_ACTION_MOVE_TO_POS_FIXED_ORIENTATION      = 141,
    SMART_ACTION_STORE_VARIABLE_DECIMAL             = 142,    // varID, number

	// Custom, not TC. Some are not working fully (yet)
	SMART_ACTION_ATTACK_STOP                        = 150,
	SMART_ACTION_IGNORE_PATHFINDING					= 151,
	SMART_ACTION_MOVE_VERTICALLY                    = 152,

	SMART_ACTION_MOVE_BY_PATH                       = 153,
	SMART_ACTION_SET_SPEED                          = 154,
	SMART_ACTION_SET_CORPSE_DELAY                   = 155,
	SMART_ACTION_SET_ANIM_KIT                       = 156,
	SMART_ACTION_MOVE_FALL                          = 157,
	SMART_ACTION_DISABLE_TURN                       = 158,
	SMART_ACTION_HANDLE_DOOR                        = 159,
	SMART_ACTION_PLAY_MUSIC                         = 160,
	SMART_ACTION_MOVE_CIRCLE                        = 161,
	SMART_ACTION_THREAT_SINGLE                      = 162,
	SMART_ACTION_IGNORE_EVADE                       = 163,
	SMART_ACTION_CAST_RANDOM                        = 164,
	SMART_ACTION_PLAY_ONE_SHOT_ANIM_KIT             = 165,
	SMART_ACTION_PLAY_SPELL_VISUAL_KIT              = 166,
	SMART_ACTION_PLAY_SPELL_VISUAL                  = 167,
    SMART_ACTION_PREVENT_EVENT_PHASE_RESET          = 168,
	SMART_ACTION_SET_VEHICLE_ID                     = 169,
    SMART_ACTION_PLAYER_TALK                        = 170,
    SMART_ACTION_STOP_MOVEMENT                      = 171,
    SMART_ACTION_CUSTOM_MOVE_BY_PATH                = 172,
    SMART_ACTION_OVERWRITE_INVOKER                  = 173,
	SMART_ACTION_SEND_QUEST_EVENT                   = 200,
    SMART_ACTION_CUSTOM_MOVE_BY_PATHTWO             = 201,
    SMART_ACTION_REMOVE_AURAS_BY_TYPE               = 202,
        
    SMART_ACTION_SET_DYNAMIC_FLAG                   = 400,     // Flags
    SMART_ACTION_ADD_DYNAMIC_FLAG                   = 401,     // Flags
    SMART_ACTION_REMOVE_DYNAMIC_FLAG                = 402,     // Flags
    SMART_ACTION_REWARD_LFG_DUNGEON                 = 403,
    SMART_ACTION_DO_DIRECT_AI_TALK                  = 404,


    /// AZENOR CUSTOM ACTIONS

    SMART_ACTION_PAUSE_WAYPOINT_FOR_MS              = 700,
    SMART_ACTION_END,
};

struct SmartAction
{
    SMART_ACTION type;

    union
    {
        struct
        {
            uint32 textGroupID;
            uint32 duration;
            uint32 useTalkTarget;
        } talk;

        struct
        {
            uint32 factionID;
        } faction;

        struct
        {
            uint32 creature;
            uint32 model;
        } morphOrMount;

        struct
        {
            uint32 sound;
            uint32 onlySelf;
            uint32 distance;
        } sound;

        struct
        {
            uint32 emote;
        } emote;

        struct
        {
            uint32 quest;
        } quest;

        /// Verified 5-27-19
        /// SMART_ACTION_ADD_QUEST
        struct
        {
            uint32 QuestID;
            uint32 CheckRequirements;
            uint32 ShowPopup;
        } AddQuest;

        struct
        {
            uint32 state;
        } react;

        struct
        {
            uint32 emotes[SMART_ACTION_PARAM_COUNT];
        } randomEmote;

        struct
        {
            uint32 spell;
            uint32 castFlags;
            uint32 triggerFlags;
            uint32 targetsLimit;
        } cast;

        struct
        {
            uint32 spell;
            uint32 castFlags;
            uint32 targetType;
            uint32 targetParam1;
            uint32 targetParam2;
            uint32 targetParam3;
        } crossCast;

        /// Verified - 5-26-19
        /// SMART_ACTION_SUMMON_CREATURE
        struct
        {
            uint32 Entry;
            uint32 SummonType;
            uint32 Duration;
            uint32 AttackInvoker;
            uint32 SummonField;
        } summonCreature;

        struct
        {
            uint32 threatINC;
            uint32 threatDEC;
        } threatPCT;

        struct
        {
            uint32 quest;
            uint32 spell;
        } castCreatureOrGO;

        struct
        {
            uint32 flag1;
            uint32 flag2;
            uint32 flag3;
            uint32 flag4;
            uint32 flag5;
            uint32 flag6;
        } addUnitFlag;

        struct
        {
            uint32 flag1;
            uint32 flag2;
            uint32 flag3;
            uint32 flag4;
            uint32 flag5;
            uint32 flag6;
        } removeUnitFlag;

        struct
        {
            uint32 attack;
        } autoAttack;

        struct
        {
            uint32 move;
        } combatMove;

        struct
        {
            uint32 phase;
        } setEventPhase;

        struct
        {
            uint32 inc;
            uint32 dec;
        } incEventPhase;

        struct
        {
            uint32 creature;
            uint32 spell;
        } castedCreatureOrGO;

        /// Verified 5-27-19
        /// SMART_ACTION_REMOVE_AURAS
        struct
        {
            uint32 SpellId;
            uint32 Stacks;
        } RemoveAuras;

        struct
        {
            uint32 dist;
            uint32 angle;
            uint32 entry;
            uint32 credit;
            uint32 creditType;
        } follow;

        struct
        {
            uint32 phases[SMART_ACTION_PARAM_COUNT];
        } randomPhase;

        struct
        {
            uint32 phaseMin;
            uint32 phaseMax;
        } randomPhaseRange;

        struct
        {
            uint32 creature;
        } killedMonster;

        struct
        {
            uint32 field;
            uint32 data;
        } setInstanceData;

        struct
        {
            uint32 field;
        } setInstanceData64;

        struct
        {
            uint32 creature;
            uint32 team;
            uint32 updateLevel;
        } updateTemplate;

        struct
        {
            uint32 range;
            uint32 withEmote;
        } callHelp;

        struct
        {
            uint32 sheath;
        } setSheath;

        struct
        {
            uint32 delay;
            uint32 forceRespawnTimer;
        } forceDespawn;

        struct
        {
            uint32 minHP;
            uint32 percent;
        } invincHP;

        struct
        {
            uint32 id;
            uint32 apply;
        } ingamePhaseId;

        struct
        {
            uint32 groupId;
            uint32 apply;
        } ingamePhaseGroup;

        struct
        {
            uint32 field;
            uint32 data;
        } setData;

        struct
        {
            uint32 distance;       ///<  (0 - disable random movement)
            uint32 walkChance;     ///< Hades: (0-100; 0-always run, 100-always walk)
            uint32 skipWalkChance; ///< Hades: (100 - always skip wait chance)
        } moveRandom;

        struct
        {
            uint32 state;
        } visibility;

        struct
        {
            uint32 entry;
            uint32 despawnTime;
        } summonGO;

        struct
        {
            uint32 state;
        } active;

        struct
        {
            uint32 id;
        } taxi;

        struct
        {
			uint32 run;
           // uint32 movementTemplateId;
            uint32 pathID;
            uint32 canRepeat;
            uint32 pathIndex;
            uint32 despawnTime;
            uint32 reactState;
        } wpStart;

        struct
        {
            uint32 delay;
        } wpPause;

        struct
        {
            uint32 despawnTime;
            uint32 quest;
            uint32 fail;
        } wpStop;

        struct
        {
            uint32 entry;
            uint32 count;
        } item;

        struct
        {
            uint32 id;
            uint32 param1;
            uint32 param2;
            uint32 param3;
            uint32 param4;
            uint32 param5;
        } installTtemplate;

        struct
        {
            uint32 run;
        } setRun;

        struct
        {
            uint32 fly;
            uint32 Type;
        } setFly;

        struct
        {
            uint32 swim;
        } setSwim;

        struct
        {
            uint32 mapID;
        } teleport;

        struct
        {
            uint32 counterId;
            uint32 value;
            uint32 reset;
        } setCounter;

        struct
        {

            uint32 id;
            uint32 number;
        } storeVar;

        struct
        {
            uint32 id;
        } storeTargets;

        struct
        {
            uint32 id;
            uint32 min;
            uint32 max;
            uint32 repeatMin;
            uint32 repeatMax;
            uint32 chance;
        } timeEvent;

        struct
        {
            uint32 entry;
        } movie;

        struct
        {
            uint32 entry;
            uint32 mask;
            uint32 slot1;
            uint32 slot2;
            uint32 slot3;
        } equip;

        struct
        {
            uint32 flag;
            uint32 type;
        } unitFlag;

        struct
        {
            uint32 byte1;
            uint32 type;
        } setunitByte;

        struct
        {
            uint32 byte1;
            uint32 type;
        } delunitByte;

        struct
        {
            uint32 seat;
        } enterVehicle;

        struct
        {
            uint32 id;
            uint32 timerType;
        } timedActionList;

        struct
        {
            uint32 actionLists[SMART_ACTION_PARAM_COUNT];
        } randTimedActionList;

        struct
        {
            uint32 withDelayed;
            uint32 spell_id;
            uint32 withInstant;
        } interruptSpellCasting;

        struct
        {
            uint32 anim;
        } sendGoCustomAnim;

        struct
        {
            uint32 speedxy;
            uint32 speedz;
            uint32 Id;
        } jump;

        struct
        {
            uint32 withEmote;
        } flee;

        struct
        {
            uint32 goRespawnTime;
        } RespawnTarget;

        /// SMART_ACTION_MOVE_TO_POS
        struct
        {
            uint32 pointId;
            uint32 movementTemplateId;
            uint32 disablePathfinding;
            uint32 MovementType;
            uint32 useWalk;
            uint32 ContactDistance;
        } MoveToPos;

        struct
        {
            uint32 gossipMenuId;
            uint32 gossipNpcTextId;
        } sendGossipMenu;

        struct
        {
            uint32 state;
        } setGoLootState;

        struct
        {
            uint32 id;
        } sendTargetToTarget;

        struct
        {
            uint32 ResetTAL;
        } CallScriptReset;

        struct
        {
            uint32 distance;
            uint32 angle;
        } setRangedMovement;

        struct
        {
            uint32 regenHealth;
        } setHealthRegen;

        struct
        {
            uint32 questId;
        } getQuestEventId;


		struct
		{
			uint32 onOff;
		} setIgnorePathfinding;

		struct
		{
			uint32 distance;
			uint32 pointId;
			uint32 upDown;
			uint32 zSpeed;
		} moveVertically;

		struct
		{
			uint32 pointId;
			uint32 startWp;
			uint32 endWp;
			uint32 isCatmull;
			uint32 waypointEntry;
			uint32 isCyclic;
		} moveByPath;

        struct
        {
            uint32 Flags;
        } RewardLFGDungeon;

        struct
        {
            uint32 GroupId;
            uint32 UseSourceGUID;
        } DirectAITalk;

		struct
		{
			uint32 customWaypointEntry;
			uint32 isCatmull;
			uint32 isCyclic;
			uint32 run;
			uint32 repeat;
		} moveByPathCustom;

        struct
        {
            uint32 flag;
        } goFlag;

        struct
        {
            uint32 group;
            uint32 attackInvoker;
        } creatureGroup;
		
		struct
        {
            uint32 wps[SMART_ACTION_PARAM_COUNT];
        } closestWaypointFromList;

        struct
        {
            uint32 powerType;
            uint32 newPower;
        } power;

        struct
        {
            uint32 disable;
        } disableEvade;

		//! Note for any new future actions
		//! All parameters must have type uint32

		struct
		{
			uint32 speedType;
			uint32 speed;
		} setSpeed;

		struct
		{
			uint32 corpseDelay;
		} setCorpseDelay;

		struct
		{
			uint32 type;
			uint32 animId;
		} setAnimKit;

		struct
		{
			uint32 id;
		} moveFall;

		struct
		{
			uint32 state;
			uint32 seconds;
		} handleDoor;

		struct
		{
			uint32 soundId;
			uint32 onlySelf;
		} playMusic;

		struct
		{
			uint32 minDistance;
			uint32 maxDistance;
			uint32 clockwise;
			uint32 stepAmount;
            uint32 UseTargetAsCenter;
		} moveCircle;

		struct
		{
			uint32 increase;
			uint32 decrease;
		} threatSingle;

		struct
		{
			uint32 onOff;
		} ignoreEvade;

		struct
		{
            uint32 spells[SMART_ACTION_PARAM_COUNT];
		} castRandom;

		struct
		{
			uint32 id;
		} playAnimKit;

		struct
		{
			uint32 id;
			uint32 type;
			uint32 duration;
		} playSpellVisualKit;

		struct
		{
			uint32 id;
			uint32 type;
			uint32 duration;
		} playSpellVisual;

		struct
		{
			uint32 id;
		} setVehicleKit;

		struct
		{
			uint32 onOff;
		} setRoot;

        struct
        {
            uint32 Entry;
            uint32 GroupId;
        } playerTalk;

        struct
        {
            uint32 MovementType;
            uint32 SetIdle;
        } stopMovement;

        struct
        {
            uint32 AuraTypeId;
        } removeAurasByType;

        struct
        {
            uint32 sounds[SMART_ACTION_PARAM_COUNT - 2];
            uint32 onlySelf;
            uint32 distance;
        } randomSound;

        struct
        {
            uint32 MSTime;
        } PauseWaypintForMs;

        struct
        {
            uint32 param1;
            uint32 param2;
            uint32 param3;
            uint32 param4;
            uint32 param5;
            uint32 param6;
        } raw;
    };
};

enum SMARTAI_TEMPLATE : uint32
{
    SMARTAI_TEMPLATE_BASIC          = 0, //nothing is preset
    SMARTAI_TEMPLATE_CASTER         = 1, //spellid, repeatMin, repeatMax, range, manaPCT +JOIN: target_param1 as castFlag
    SMARTAI_TEMPLATE_TURRET         = 2, //spellid, repeatMin, repeatMax +JOIN: target_param1 as castFlag
    SMARTAI_TEMPLATE_PASSIVE        = 3,
    SMARTAI_TEMPLATE_CAGED_GO_PART  = 4, //creatureID, give credit at point end?,
    SMARTAI_TEMPLATE_CAGED_NPC_PART = 5, //gameObjectID, despawntime, run?, dist, TextGroupID
    SMARTAI_TEMPLATE_END            = 6
};

enum SMARTAI_TARGETS : uint32
{
    SMART_TARGET_NONE                           = 0,    // NONE, defaulting to invoket
    SMART_TARGET_SELF                           = 1,    // Self cast
    SMART_TARGET_VICTIM                         = 2,    // Our current target (ie: highest aggro)
    SMART_TARGET_HOSTILE_SECOND_AGGRO           = 3,    // Second highest aggro
    SMART_TARGET_HOSTILE_LAST_AGGRO             = 4,    // Dead last on aggro
    SMART_TARGET_HOSTILE_RANDOM                 = 5,    // Just any random target on our threat list
    SMART_TARGET_HOSTILE_RANDOM_NOT_TOP         = 6,    // Any random target except top threat
    SMART_TARGET_ACTION_INVOKER                 = 7,    // Unit who caused this Event to occur
    SMART_TARGET_POSITION                       = 8,    // use xyz from event params
    SMART_TARGET_CREATURE_RANGE                 = 9,    // CreatureEntry(0any), minDist, maxDist
    SMART_TARGET_CREATURE_GUID                  = 10,   // guid, entry
    SMART_TARGET_CREATURE_DISTANCE              = 11,   // CreatureEntry(0any), maxDist
    SMART_TARGET_STORED                         = 12,   // id, uses pre-stored target(list)
    SMART_TARGET_GAMEOBJECT_RANGE               = 13,   // entry(0any), min, max
    SMART_TARGET_GAMEOBJECT_GUID                = 14,   // guid, entry
    SMART_TARGET_GAMEOBJECT_DISTANCE            = 15,   // entry(0any), maxDist
    SMART_TARGET_INVOKER_PARTY                  = 16,   // invoker's party members
    SMART_TARGET_PLAYER_RANGE                   = 17,   // min, max
    SMART_TARGET_PLAYER_DISTANCE                = 18,   // maxDist
    SMART_TARGET_CLOSEST_CREATURE               = 19,   // CreatureEntry(0any), maxDist, dead?
    SMART_TARGET_CLOSEST_GAMEOBJECT             = 20,   // entry(0any), maxDist
    SMART_TARGET_CLOSEST_PLAYER                 = 21,   // maxDist, aura (-aura == not present)
    SMART_TARGET_ACTION_INVOKER_VEHICLE         = 22,   // Unit's vehicle who caused this Event to occur
    SMART_TARGET_OWNER_OR_SUMMONER              = 23,   // Unit's owner or summoner
    SMART_TARGET_THREAT_LIST                    = 24,   // All units on creature's threat list
    SMART_TARGET_CLOSEST_ENEMY                  = 25,   // maxDist
    SMART_TARGET_CLOSEST_FRIENDLY               = 26,   // maxDist
    SMART_TARGET_LOOT_RECIPIENTS                = 27,   // all players that have tagged this creature (for kill credit)
    SMART_TARGET_FARTHEST_PLAYER                = 28,   // maxDist
    SMART_TARGET_VEHICLE_ACCESSORY              = 29,   // seat number (vehicle can target it's own accessory)

    
	SMART_TARGET_INVOKER_SUMMON                 = 50,
	SMART_TARGET_SPELL_CASTER                   = 51,
	SMART_TARGET_SPELL_TARGET                   = 52,
	SMART_TARGET_ANY_IN_RANGE_WITH_CONDITIONS   = 53,
	SMART_TARGET_VEHICLE_PASSENGER              = 54,
	SMART_TARGET_SUMMONER_SUMMON_LIST           = 55,
    SMART_TARGET_END                            = 56
};

struct SmartTarget
{
    SmartTarget (SMARTAI_TARGETS p_Type = SMART_TARGET_NONE, uint32 p_TargetParam1 = 0, uint32 p_TargetParam2 = 0, uint32 p_TargetParam3 = 0) :
        type(p_Type)
    {
        raw.param1 = p_TargetParam1;
        raw.param2 = p_TargetParam2;
        raw.param3 = p_TargetParam3;

        x          = 0.0f;
        y          = 0.0f;
        z          = 0.0f;
        o          = 0.0f;
    }

    SMARTAI_TARGETS type;
    float x, y, z, o;

    union
    {
        struct
        {
            uint32 creature;
            uint32 minDist;
            uint32 maxDist;
            uint32 maxSize;
        } unitRange;

        struct
        {
            uint32 dbGuid;
            uint32 entry;
        } unitGUID;

        struct
        {
            uint32 creature;
            uint32 dist;
            uint32 maxSize;
        } unitDistance;

        struct
        {
            uint32 dist;
            int32 aura;
        } playerDistance;

        struct
        {
            uint32 minDist;
            uint32 maxDist;
        } playerRange;

        struct
        {
            uint32 id;
        } stored;

        struct
        {
            uint32 entry;
            uint32 minDist;
            uint32 maxDist;
            uint32 maxSize;
        } goRange;

        struct
        {
            uint32 dbGuid;
            uint32 entry;
        } goGUID;

        struct
        {
            uint32 entry;
            uint32 dist;
            uint32 maxSize;
        } goDistance;

        struct
        {
            uint32 Position;
            uint32 Distance; ///< Only for Param1 4, 5, 6
            uint32 Speed; ///< Only For param1, 6, 7
        } position;

        struct
        {
            uint32 UseCharmerOrOwner;
        } owner;

        struct
        {
            uint32 entry;
            uint32 dist;
            uint32 dead;
        } closest;

        struct
        {
            uint32 maxDist;
            uint32 playerOnly;
        } closestAttackable;

        struct
        {
            uint32 maxDist;
            uint32 playerOnly;
        } closestFriendly;

		struct
		{
			uint32 SummonEntry;
		} invokerSummon;

		struct
		{
			uint32 MinRange;
			uint32 MaxRange;
			uint32 TargetsCount; ///< (0 = all) >0 = number of targets (randomly chosen)
        } AnyInRangeWithConditions;

		struct
		{
			uint32 seatId;
		} vehiclePassenger;

		struct
		{
			uint32 entry;
			uint32 maxDist;
			uint32 minDist;
			uint32 count;
		} summonList;

        struct
        {
            uint32 param1;
            uint32 param2;
            uint32 param3;
        } raw;

        struct
        {
            uint32 seat;
        } vehicle;
    };
};

enum SmartEventFlags : uint32
{
    SMART_EVENT_FLAG_NOT_REPEATABLE        = 0x001,  //Event can not repeat
    SMART_EVENT_FLAG_DIFFICULTY_0          = 0x002,  //Event only occurs in instance difficulty 0
    SMART_EVENT_FLAG_DIFFICULTY_1          = 0x004,  //Event only occurs in instance difficulty 1
    SMART_EVENT_FLAG_DIFFICULTY_2          = 0x008,  //Event only occurs in instance difficulty 2
    SMART_EVENT_FLAG_DIFFICULTY_3          = 0x010,  //Event only occurs in instance difficulty 3
    SMART_EVENT_FLAG_RESERVED_5            = 0x020,  
    SMART_EVENT_FLAG_RESERVED_6            = 0x040,  
    SMART_EVENT_FLAG_DEBUG_ONLY            = 0x080,  //Event only occurs in debug build
    SMART_EVENT_FLAG_DONT_RESET            = 0x100,  //Event will not reset in SmartScript::OnReset()
    SMART_EVENT_FLAG_WHILE_CHARMED         = 0x200,  //Event occurs even if AI owner is charmed
    SMART_EVENT_FLAG_SPELL_EFFECT_0        = 0x1000, ///< Event will only work with effectIndex 0
    SMART_EVENT_FLAG_SPELL_EFFECT_1        = 0x2000, ///< Event will only work with effectIndex 1
    SMART_EVENT_FLAG_SPELL_EFFECT_2        = 0x4000, ///< Event will only work with effectIndex 2

    SMART_EVENT_FLAG_DIFFICULTY_ALL = (SMART_EVENT_FLAG_DIFFICULTY_0 | SMART_EVENT_FLAG_DIFFICULTY_1 | SMART_EVENT_FLAG_DIFFICULTY_2 | SMART_EVENT_FLAG_DIFFICULTY_3),
    SMART_EVENT_FLAGS_ALL = (SMART_EVENT_FLAG_NOT_REPEATABLE | SMART_EVENT_FLAG_DIFFICULTY_ALL | SMART_EVENT_FLAG_RESERVED_5 | SMART_EVENT_FLAG_RESERVED_6 | SMART_EVENT_FLAG_DEBUG_ONLY | SMART_EVENT_FLAG_DONT_RESET | SMART_EVENT_FLAG_WHILE_CHARMED | SMART_EVENT_FLAG_SPELL_EFFECT_0 | SMART_EVENT_FLAG_SPELL_EFFECT_1 | SMART_EVENT_FLAG_SPELL_EFFECT_2)
};

enum SmartCastFlags : uint32
{
    SMARTCAST_INTERRUPT_PREVIOUS     = 0x001,  ///< Interrupt any spell casting
    SMARTCAST_TRIGGERED              = 0x002,  ///< Triggered (this makes spell cost zero mana and have no cast time)
    CAST_FORCE_CAST                  = 0x004,  ///< Forces cast even if creature is out of mana or out of range
    CAST_NO_MELEE_IF_OOM             = 0x008,  ///< Prevents creature from entering melee if out of mana or out of range
    CAST_FORCE_TARGET_SELF           = 0x010,  ///< Forces the target to cast this spell on itself
    SMARTCAST_AURA_NOT_PRESENT       = 0x020,  ///< Only casts the spell if the target does not have an aura from the spell
    SMARTCAST_COMBAT_MOVE            = 0x040,  ///< Prevents combat movement if cast successful. Allows movement on range, OOM, LOS
    SMARTCAST_IGNORE_PREVIOUS        = 0x080,  ///< Allow cast spell if another spell cast is in progress
    SMARTCAST_TARGET_CAST_SPELL      = 0x100,  ///< Targets will cast the spell
};

///SMART_TARGET_POSITION
namespace SmartTargetPositionTypes
{
    enum Type
    {
        Position,
        TransportPosition,
        HomePosition,
        GroundPosition,
        Foward,
        Backward,
        Up,
        Down,
        SpawnPosition,
    };
};

struct SmartScriptHolder
{
    public:
        /// Constructor
        SmartScriptHolder() : entryOrGuid(0), source_type(SMART_SCRIPT_TYPE_CREATURE)
            , event_id(0), link(0), timer(0), active(false), runOnce(false)
            , enableTimed(false), m_Event(SmartEvent()), action(SmartAction()), target(SmartTarget())
        { }

        /////////////////////////////////////////////////////////////////////
        /// Getters
        uint32 GetScriptType() const { return source_type; }
        uint32 GetEventType() const  { return m_Event.type; }
        uint32 GetActionType() const { return action.type; }
        uint32 GetTargetType() const { return target.type; }

        /////////////////////////////////////////////////////////////////////
        /// Keys
        int32                   entryOrGuid;
        SmartScriptType         source_type;
        uint32                  event_id;
        uint32                  link;

        /////////////////////////////////////////////////////////////////////
        /// Internal
        uint32                  timer;
        bool                    active;
        bool                    runOnce;
        bool                    enableTimed;

        /////////////////////////////////////////////////////////////////////
        /// Data
        SmartEvent              m_Event;
        SmartAction             action;
        SmartTarget             target;
        /////////////////////////////////////////////////////////////////////

};

/// We need WPPath to be ordered
                         /// pointid   path
typedef std::map<uint32, WayPoint*> WPPath;

typedef std::vector<WorldObject*> ObjectVector;

typedef std::vector<uint64> GuidVector;

class ObjectGuidVector
{
    public:
        explicit ObjectGuidVector(ObjectVector const& objectVector);

        ObjectVector const* GetObjectVector(WorldObject const& ref) const
        {
            UpdateObjects(ref);
            return &_objectVector;
        }

        ~ObjectGuidVector() { }

    private:
        GuidVector _guidVector;
        mutable ObjectVector _objectVector;

        //sanitize vector using _guidVector
        void UpdateObjects(WorldObject const& ref) const;
};
typedef std::unordered_map<uint32, ObjectGuidVector> ObjectVectorMap;

class SmartWaypointMgr
{
    public:
        SmartWaypointMgr() {}
        ~SmartWaypointMgr();

        static SmartWaypointMgr* instance()
        {
            static SmartWaypointMgr instance;
            return &instance;
        }

        void LoadFromDB();

        WPPath* GetPath(uint32 id)
        {
            if (waypoint_map.find(id) != waypoint_map.end())
                return waypoint_map[id];

            return nullptr;
        }

    private:
        std::unordered_map<uint32, WPPath*> waypoint_map;
};

// all events for a single entry
typedef std::vector<SmartScriptHolder> SmartAIEventList;

// all events for all entries / guids
typedef std::unordered_map<int32, SmartAIEventList> SmartAIEventMap;

// Helper Stores
typedef std::map<uint32 /*entry*/, std::pair<uint32 /*spellId*/, SpellEffIndex /*effIndex*/> > CacheSpellContainer;
typedef std::pair<CacheSpellContainer::const_iterator, CacheSpellContainer::const_iterator> CacheSpellContainerBounds;

class SmartAIMgr
{
    public:
        SmartAIMgr() { }
        ~SmartAIMgr() { }

        /// Returns static object\ of SmartAIMgr
        static SmartAIMgr* instance()
        {
            static SmartAIMgr l_Instance;
            return &l_Instance;
        }

        /// Loads SmartAIFromDB
        ///@p_Entry. Entry to load, if it's 0 it will load all SAI
        void LoadSmartAIFromDB(int32 p_Entry = 0);

        /// Returns script from mEventMap with identidifer, type and entry
        /// @p_Entry - Entry/GUID
        /// @p_Type - SmartScriptType
        SmartAIEventList GetScript(int32 p_Entry, uint32 p_Type)
        {
            if (mEventMap[p_Type].find(p_Entry) != mEventMap[p_Type].end())
                return mEventMap[uint32(p_Type)][p_Entry];

            return SmartAIEventList();
        }

        /// Returns whether an entry and type combination of a smart script exists
        /// @p_Entry - Entry/GUID
        /// @p_Type - SmartScriptType
        bool ScriptExist(int32 p_Entry, uint32 p_Type) const;

    private:
        //event stores
        SmartAIEventMap mEventMap[SMART_SCRIPT_TYPE_MAX];

        /// DB error functions
        /// They search their respective store/check the param data to make sure it's exist
        /// Otherwise they throw error.

        bool IsEventValid(SmartScriptHolder& e);
        bool IsTargetValid(SmartScriptHolder const& e);
        bool IsMinMaxValid(SmartScriptHolder const& e, uint32& min, uint32& max);
        bool NotNULL(SmartScriptHolder const& e, uint32 data);
        bool IsCreatureValid(SmartScriptHolder const& e, uint32 entry);
        bool IsQuestValid(SmartScriptHolder const& e, uint32 entry);
        bool IsGameObjectValid(SmartScriptHolder const& e, uint32 entry);
        bool IsSpellValid(SmartScriptHolder const& e, uint32 entry);
        bool IsItemValid(SmartScriptHolder const& e, uint32 entry);
        bool IsTextEmoteValid(SmartScriptHolder const& e, uint32 entry);
        bool IsEmoteValid(SmartScriptHolder const& e, uint32 entry);
        bool IsAreaTriggerValid(SmartScriptHolder const& e, uint32 entry);
        bool IsSoundValid(SmartScriptHolder const& e, uint32 entry);

        // Helpers
        void LoadHelperStores();
        void UnLoadHelperStores();

        CacheSpellContainerBounds GetSummonCreatureSpellContainerBounds(uint32 creatureEntry) const;
        CacheSpellContainerBounds GetSummonGameObjectSpellContainerBounds(uint32 gameObjectEntry) const;
        CacheSpellContainerBounds GetKillCreditSpellContainerBounds(uint32 killCredit) const;

        CacheSpellContainer SummonCreatureSpellStore;
        CacheSpellContainer SummonGameObjectSpellStore;
        CacheSpellContainer KillCreditSpellStore;
};

#define sSmartScriptMgr SmartAIMgr::instance()
#define sSmartWaypointMgr SmartWaypointMgr::instance()