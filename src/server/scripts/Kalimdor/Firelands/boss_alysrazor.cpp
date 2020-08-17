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

#include "ObjectMgr.h"
#include "PassiveAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "SpellScript.h"
#include "MoveSplineInit.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "firelands.h"

enum Texts
{
    // Alysrazor
    YELL_AGGRO                  = 0,
    YELL_VORTEX_PHASE           = 1,
    YELL_BURNOUT                = 3,
    YELL_REIGNITE               = 4,
    YELL_KILL_PLAYER            = 5,
    YELL_DEAD                   = 6,
    EMOTE_FIRESTORM             = 7,
    EMOTE_BURNOUT               = 8,
    EMOTE_IGNITED               = 9,
    EMOTE_FULL_POWER            = 10,
    EMOTE_SOFT_ENRAGE           = 11,
    EMOTE_MOLTEN_EGGS           = 12,
    EMOTE_LAVA_WORMS            = 13,
    EMOTE_VORTEX_TURN           = 14,
    // Blazing Talon Initiate
    YELL_INITIATE               = 0,
    // Herald of the Burning End
    YELL_HERALD_BURNING         = 0,
    EMOTE_HERALD_BURNING        = 1,
    // Fiery Vortex
    EMOTE_FIERY_VORTEX          = 0,
    // Blazing Talon Clawshaper
    YELL_CLAWSHAPER             = 0,
    EMOTE_CRACKING_EGGS         = 0,
};

Position const SpawnPos[2] =
{
    // Position on summon
    {110.404f, -376.861f, 30.465743f, 2.517f}, // startPos
    {49.467f, -344.118f, 90.471171f, 2.517f},  // endPos
};

Position const WormsPos[4] =
{
    {-36.8941f, -246.4028f, 54.77835f, 1.0f},
    {-71.6441f, -272.5156f, 55.48354f, 4.0f},
    {-45.65451f, -306.559f, 54.85931f, 0.2f},
    {-11.63889f, -278.0087f, 53.10715f, 2.0f}
};

Position const FallPos[2] =
{
    {-41.534f, -282.622f, 47.500f, 0.188f}, // fall pos ground
    {-41.534f, -282.622f, 115.000f, 0.188f} // fall pos fly
};

Position const BirdsPos[2] =
{
    {46.930f, -207.560f, 105.000f, 3.951f},
    {-28.813f, -378.088f, 107.371f, 1.697f}
};

Position const Talon[8] =
{
    { -6.561f, -376.156f, 75.623f, 2.017f },
    { -27.041f, -329.757f, 57.953f, 2.402f }, //North East

    { -93.998f, -338.753f, 67.583f, 1.239f },
    { -92.066f, -296.570f, 59.674f, 0.459f }, //South East

    { -85.828f, -218.517f, 71.189f, 5.014f },
    { -82.602f, -254.543f, 61.894f, 5.922f }, //South West

    { 35.679f, -260.996f, 67.902f, 3.880f },
    { 8.355f, -286.534f, 56.162f, 2.984f }    // North West
};

Position const ClawTalon[2] =
{
    {-27.773f, -191.712f, 83.280f, 4.769f},
    {-65.413f, -351.496f, 72.721f, 1.356f}
};

Position const Feather[1] =
{
    {0.760f, -305.456f, 53.5113f, 5.597f}
};

Position const SpawnVolcanos[11] =
{
    {5.375f, -336.376f, 54.629f, 2.304f},
    {9.478f, -331.562f, 54.387f, 2.304f},
    {16.252f, -323.355f, 54.448f, 2.304f},
    {19.871f, -320.852f, 54.960f, 2.304f},
    {22.058f, -315.533f, 54.959f, 2.304f},
    {28.059f, -307.899f, 54.958f, 2.304f},
    {32.530f, -300.595f, 54.965f, 2.304f},

    {-103.034f, -294.838f, 56.158f, 2.304f},
    {-103.872f, -286.840f, 56.507f, 2.304f},
    {-103.593f, -280.863f, 56.531f, 2.304f},
    {-104.593f, -274.393f, 56.534f, 2.304f}
};

enum actions
{
    ACTION_MOVE_FROM_LEFT  = 0,
    ACTION_MOVE_FROM_RIGHT = 1,
    ACTION_LAUNCH_EGG      = 3,
    ACTION_BREAK_GROUND    = 4,
};

enum Spells
{
    SPELL_FIRESTORM                          = 99605,
    SPELL_HEROIC_FIRESTORM                   = 100744,
    SPELL_HEROIC_FIRESTORM_DAMAGE            = 100745,
    SPELL_ROOT_EVENT                         = 100555,
    SPELL_REMOVE_ROOT_EVENT                  = 100561,
    // Volcano
    SPELL_VOLCANO_DUMMY                      = 98402,
    SPELL_VOLCANO_AURA                       = 98462,
    // Tornados
    SPELL_SUMMON_DMG                         = 99793,
    // Feather
    SPELL_COSMETIC                           = 99507,
    SPELL_CAST_ON_MOVE_VISUAL                = 97128,
    // Firehawk
    SPELL_FIREHAWK_COSMETIC                  = 100712,
    SPELL_FIREHAWK_TRANSFORM_M               = 99550,
    SPELL_FIREHAWK_TRANSFORM_F               = 100350,
    SPELL_FIREHAWK_CLAWSHAPPER               = 99923,
    SPELL_FIREHAWK_REMOVE_FORM               = 99924,
    SPELL_IGNITION                           = 99919,
    SPELL_BRUSHFIRE                          = 98868,
    SPELL_FIEROBLAST                         = 101223,
    SPELL_FIREITUP                           = 100093,
    SPELL_BRUSHFIRE_VIS                      = 98884,
    SPELL_BLAZING_SHIELD                     = 101484,
    // Hatchling
    SPELL_SATIATED                           = 99359,
    SPELL_HUNGRY                             = 99361,
    SPELL_TANTRUM                            = 99362,
    SPELL_IMPRINTED_TARGETING                = 99388,
    SPELL_IMPRINTED                          = 99389,
    SPELL_IMPRINTED_TAUNT                    = 99390,
    SPELL_IMPRINTED_CANCEL                   = 100023, // unused
    SPELL_IMPRINTED_TARGETING_2              = 100358,
    SPELL_IMPRINTED_2                        = 100359,
    SPELL_IMPRINTED_TAUNT_2                  = 100360,
    SPELL_IMPRINTED_CANCEL_2                 = 100362, // unused
    SPELL_GUSHING_WOUND                      = 100024, // 14:08:06.458 14:08:17.332 14:08:25.740 14:08:52.338 14:08:52.370 14:09:06.81 14:09:06.831
    SPELL_GUSHING_WOUND_HEROIC               = 99308,  // not 100% sure if this is the heroic entry but this have 4m more effect radius...
    // Worm
    SPELL_LAVA_SPEW                          = 99335,
    // Blazing power
    SPELL_BLAZING_POWER                      = 99462,
    SPELL_BLAZING_POWER_PREVENTION_AURA      = 99565,
    SPELL_ALYSRAS_RAZOR                      = 100029,
    // Cloud
    SPELL_INCENDIARY_CLOUD                   = 99426,
    // Alysrazor
    SPELL_INDICATOR                          = 101410,
    SPELL_BLAZING_CLAW                       = 99843,
    SPELL_PASSIVE_BUF_FROM_FEATHER           = 99461,
    SPELL_BURNOUT                            = 99432,
    SPELL_IGNITED                            = 99922,
    SPELL_SPARK                              = 99921,
    SPELL_ZERO_POWER                         = 72242,
    SPELL_FULL_POWER                         = 99504,
    SPELL_MOLTING                            = 99464,
    SPELL_PLAYER_FLY                         = 98630,
    SPELL_FLY                                = 98619,
    SPELL_HARSH_WINDS                        = 100640,
    SPELL_ESSENCE_OF_THE_GREEN               = 99433,
    SPELL_BERSERK                            = 110055,
    //molten egg
    SPELL_EGG_EXPLOSION                      = 99943,
    SPELL_SUMM_HATHING                       = 99723,
    SPELL_SUMM_HATHING_2                     = 100363,
    SPELL_CREATE_FRAG                        = 100519,
    //broodmother
    SPELL_BROODMOTHER_VIS                    = 99734,
    SPELL_BROODMOTHER_VIS1                   = 100712,
    SPELL_EJECT_ALL_PASSENGERS               = 50630,
    // Harbinger of Flame
    SPELL_FIRE_IT_UP                         = 100093,
    SPELL_FIEROBLAST_TRASH                   = 100094,
    SPELL_FIEROCLAST_BARRAGE                 = 100095,
    SPELL_FIRE_CHANNELING                    = 100109,
    // Blazing Monstrosity
    SPELL_RIDE_MONSTROSITY                   = 93970,
    SPELL_SHARE_HEALTH_LEFT                  = 101502,
    SPELL_SHARE_HEALTH_RIGHT                 = 101503,
    SPELL_SLEEP_ULTRA_HIGH_PRIORITY          = 99480,
    SPELL_GENERIC_DUMMY_CAST                 = 100088,
    SPELL_LEFT_SIDE_SMACK_L                  = 100076,
    SPELL_RIGHT_SIDE_SMACK_L                 = 100078,
    SPELL_HEAD_BONK_L                        = 100080,
    SPELL_TICKLE_L                           = 100082,
    SPELL_KNOCKBACK_RIGHT                    = 100084,
    SPELL_KNOCKBACK_LEFT                     = 100085,
    SPELL_KNOCKBACK_FORWARD                  = 100086,
    SPELL_KNOCKBACK_BACK                     = 100087,
    SPELL_HEAD_BONK_R                        = 100089,
    SPELL_LEFT_SIDE_SMACK_R                  = 100090,
    SPELL_RIGHT_SIDE_SMACK_R                 = 100091,
    SPELL_TICKLE_R                           = 100092,
    SPELL_MOLTEN_BARRAGE_EFFECT_L            = 100071,
    SPELL_MOLTEN_BARRAGE_LEFT                = 100072,
    SPELL_MOLTEN_BARRAGE_RIGHT               = 100073,
    SPELL_MOLTEN_BARRAGE_EFFECT_R            = 100074,
    SPELL_MOLTEN_BARRAGE_VISUAL              = 100075,
    SPELL_AGGRO_CLOSEST                      = 100462,
    SPELL_INVISIBILITY_AND_STEALTH_DETECTION = 18950,
    // Egg Pile
    SPELL_SUMMON_SMOULDERING_HATCHLING       = 100096,
    SPELL_MOLTEN_EGG_TRASH_CALL_L            = 100097,
    SPELL_MOLTEN_EGG_TRASH_CALL_R            = 100098,
    SPELL_ALYSRAZOR_COSMETIC_EGG_XPLOSION    = 100099,
    SPELL_BREAK_GROUND                       = 99944,
    SPELL_FIERY_TORNADO_AURA                 = 99817,
    SPELL_FIRE_FEATHER_MAGE                  = 98761,
    SPELL_FIRE_FEATHER_WARRIOR               = 98762,
    SPELL_FIRE_FEATHER_WARLOCK               = 98764,
    SPELL_FIRE_FEATHER_PRIEST                = 98765,
    SPELL_FIRE_FEATHER_DRUID                 = 98766,
    SPELL_FIRE_FEATHER_ROGUE                 = 98767,
    SPELL_FIRE_FEATHER_HUNTER                = 98768,
    SPELL_FIRE_FEATHER_PALADIN               = 98769,
    SPELL_FIRE_FEATHER_SHAMAN                = 98770,
    SPELL_FIRE_FEATHER_DEATHNIGHT            = 98771,
    SPELL_SUMMON_INCIENDARY_CLOUD            = 99529,
    SPELL_SUMMON_BLAZING_POWER               = 99528,
    // NPC_HERALD_OF_THE_BURNING_END
    SPELL_RITUEL_OF_FLAMME                   = 99199,
    SPELL_CATACLYSME                         = 100761, // send event 28784
    SPELL_CATACLYSME_2                       = 102111, // send event 29096
    SPELL_FUSION_METEOR_MISSILE              = 99553, // trigger SPELL_FUSION_METEOR_SUMMON
    SPELL_FUSION_METEOR_SUMMON               = 99554, // summon NPC_FUSION_METEOR
    SPELL_FUSION_METEOR_DEATH                = 100055, // summon NPC_DEAD_METEOR
    // NPC_DEAD_METEOR
    SPELL_DEATH_FUSION_METEOR_AURA           = 100059,
    // hit wall ???
    SPELL_FUSION_ROCK_MISSILE                = 99602, // trigger 99603
    // NPC_FUSION_METEOR
    SPELL_METEOR_IMPACT                      = 99558,
    SPELL_METEOR_PERIODIC_AURA               = 99215, // !aura 99274 trigger SPELL_METEOR_DAMAGE
    SPELL_METEOR_DAMAGE                      = 99274,
    // fusion rock
    SPELL_FUSION_ROCK_PERIODIC_AURA          = 99265, // !aura 99275 trigger SPELL_FUSION_ROCK_DAMAGE
    SPELL_FUSION_ROCK_DAMAGE                 = 99275,
    // misc
    SPELL_ALYS_CHANNEl_TRACK_TARGET          = 74508,
};

#define SPELL_SHARE_HEALTH          (me->GetEntry() == NPC_BLAZING_MONSTROSITY_LEFT ? SPELL_SHARE_HEALTH_LEFT : SPELL_SHARE_HEALTH_RIGHT)
#define SPELL_MOLTEN_BARRAGE        (me->GetEntry() == NPC_BLAZING_MONSTROSITY_LEFT ? SPELL_MOLTEN_BARRAGE_LEFT : SPELL_MOLTEN_BARRAGE_RIGHT)
#define SPELL_MOLTEN_BARRAGE_EFFECT (me->GetEntry() == NPC_BLAZING_MONSTROSITY_LEFT ? SPELL_MOLTEN_BARRAGE_EFFECT_L : SPELL_MOLTEN_BARRAGE_EFFECT_R)

enum Phases
{
    PHASE_NONE         = 0,

    PHASE_JUMP         = 1,
    PHASE_START_MOVING = 2,
    PHASE_AIR          = 3,
    PHASE_GROUND       = 4,
    PHASE_TORNADO      = 5,
};

enum Events
{
    EVENT_START_ENCOUNTER        = 1,
    // Moving
    EVENT_MOVE_TO_START_OF_TOP,
    EVENT_MOVE_TORNADOS,
    EVENT_MOVE_AROUND_PLATFORM,
    EVENT_FEATHER,
    EVENT_FEATHER_MOVE,
    EVENT_FEATHER_END,
    // Phase
    EVENT_PHASE_2_TORNADOS_START,
    EVENT_PHASE_2_TORNADOS_ENDS,
    EVENT_INCENDIARY_CLOUD,
    EVENT_HARSH_WINDS,
    EVENT_MOVE_AROUND_POINT,
    EVENT_SUMMON_TORNADO,
    EVENT_SUMMON_HERALD_OF_THE_BURNING_END,
    // Hatchling
    EVENT_EAT_WORM,
    EVENT_HUNGRY,
    EVENT_DEBUFF,
    // Lava worm
    EVENT_CALC_ORIENT,
    EVENT_SUMMON_WORMS,
    EVENT_SUMMON_BROODMOTHER,
    // Egg
    EVENT_EGG_EXPLOSION,
    EVENT_SUMMON_HATCHING,
    EVENT_DESPAWN,
    // Clawshaper
    EVENT_SUMMON_DRUIDS,
    EVENT_START_IGNITION,
    EVENT_START_COMBAT,
    EVENT_CHECK_ENERGY,
    EVENT_SPELL_CAST,
    EVENT_SUMMON_TALON,
    EVENT_BLAZING_POWER,
    EVENT_BLAZING_POWER_GROUND,
    EVENT_HEROIC_FIRESTORM,
    // Blazing Monstrosity
    EVENT_START_SPITTING,
    EVENT_CONTINUE_SPITTING,
    // Harbinger of Flame
    EVENT_FIEROBLAST,
    EVENT_FIEROCLAST_BARRAGE,
    // Egg Pile
    EVENT_SUMMON_SMOULDERING_HATCHLING,
    // miscs
    EVENT_VOLCANO_AURA,
    EVENT_MOVE,
    EVENT_CHECK_DESPAWN,
    // lava worms
    EVENT_LAVA_SPEW,
    EVENT_TURN,
};

enum MiscData
{
    MODEL_INVISIBLE_STALKER     = 11686,
    ANIM_KIT_BIRD_WAKE          = 1469,
    ANIM_KIT_BIRD_TURN          = 1473,
    POINT_TORNADO               = 40,
    // alysrazor points
    POINT_START_TOP             = 1,
    POINT_TURN_TOP              = 2,
    POINT_FEATHER               = 3,
    POINT_FEATHER_MOVE          = 4,
    GROUP_GROUND                = 1,
    ACTION_FIRESTORM            = 1
};

enum NpcsOrGobs
{
    NPC_MOLTEN_EGG                = 53681,
    NPC_MOLTEN_EGG_2              = 53899,
    NPC_ALYS_VOLCANO              = 53158,
    NPC_MOLTEN_FEATHER            = 53089,
    NPC_BLAZING_TALON_CLAWSHAPER  = 53734,
    NPC_PLUMB_LAVA_WORMS          = 53520,
    NPC_FIERY_TORNADO             = 53698,
    NPC_FIERY_VORTEX              = 53693,
    NPC_BLAZING_POWER             = 53554,
    NPC_INCENDIARY_CLOUD          = 53541,
    NPC_BLAZING_TALON_INITIATE    = 53896,
    NPC_GENERAL_BUNNY             = 45979,
    NPC_BROODMOTHER               = 53680,
    NPC_BROODMOTHER_2             = 53900,
    NPC_VORACIOUS_HATCHLING       = 53509,
    NPC_VORACIOUS_HATCHLING_2     = 53898,
    NPC_FUSION_METEOR             = 53489,
    NPC_HERALD_OF_THE_BURNING_END = 53375,
    NPC_FUSION_ROCK               = 53497, // 53498 53496
    NPC_DEAD_METEOR               = 53784,
    NPC_LAVA_WORM_TARGET          = 53521,

    GO_METEOR_COLLISION           = 208966,
};

#define SPELL_MOLTING_AURA RAID_MODE(99464, 100698, 99464, 100698)

static const Position flyTriggerPos[8]
{
    { 0.0f, -12.0f, 8.0f,   0.0f },
    { 0.0f, 0.0f,   8.0f,   0.0f },
    { 0.0f, 12.0f,  8.0f,   0.0f },
    { 0.0f, -10.0f, -5.0f, 0.0f },
    { 0.0f, 10.0f,  -5.0f, 0.0f },
    { 0.0f, -12.0f, -11.0f, 0.0f },
    { 0.0f, 0.0f,   -11.0f, 0.0f },
    { 0.0f, 12.0f,  -11.0f, 0.0f }
};

static const Position wallPos[8]
{
    { 11.99f,  -280.86f, 52.87f, 6.19f },
    { 0.40f,   -310.95f, 52.0f,  5.60f },
    { -36.27f, -338.64f, 52.70f, 4.79f },
    { -90.11f, -307.11f, 58.1f,  3.72f },
    { -98.12f, -273.32f, 55.39f, 3.11f },
    { -89.21f, -249.63f, 56.83f, 2.63f },
    { -28.04f, -217.62f, 52.2f,  1.35f },
    { 5.48f,   -229.14f, 58.19f, 0.76f },
};

class RespawnEggEvent : public BasicEvent
{
public:
    explicit RespawnEggEvent(Creature* egg) : _egg(egg) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        _egg->RestoreDisplayId();
        return true;
    }

private:
    Creature* _egg;
};

class MoltenEggCheck
{
public:
    explicit MoltenEggCheck(Creature* pile) : _eggPile(pile) { }

    bool operator()(Unit* object) const
    {
        if (object->GetEntry() != NPC_MOLTEN_EGG_TRASH)
            return false;

        if (object->GetDisplayId() != object->GetNativeDisplayId())
            return false;

        if (_eggPile->GetDistance2d(object) > 20.0f)
            return false;
        return true;
    }

private:
    Creature* _eggPile;
};

class TrashRespawnWorker
{
public:
    void operator()(Creature* creature) const
    {
        switch (creature->GetEntry())
        {
            case NPC_BLAZING_MONSTROSITY_LEFT:
            case NPC_BLAZING_MONSTROSITY_RIGHT:
            case NPC_EGG_PILE:
            case NPC_HARBINGER_OF_FLAME:
            case NPC_MOLTEN_EGG_TRASH:
                if (!creature->isAlive())
                    creature->Respawn(true);
                break;
            case NPC_SMOULDERING_HATCHLING:
                creature->DespawnOrUnsummon();
                break;
        }
    }
};

static void AlysrazorTrashEvaded(Creature* creature)
{
    TrashRespawnWorker check;
    Trinity::CreatureWorker<TrashRespawnWorker> worker(creature, check);
    creature->VisitNearbyGridObject(SIZE_OF_GRIDS, worker);
}

class boss_alysrazor : public CreatureScript
{
public:
    boss_alysrazor() : CreatureScript("boss_alysrazor") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_alysrazorAI (creature);
    }

    struct boss_alysrazorAI: public BossAI
    {
        boss_alysrazorAI(Creature* creature) : BossAI(creature, DATA_ALYSRAZOR)
        {
            ASSERT(creature->GetVehicleKit()); // Power.
            me->SetMaxPower(POWER_ENERGY,100);
            me->SetPower(POWER_ENERGY,100);
        }

        void Reset() override
        {
            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
            {
                uint32 preNerfHealth = RAID_MODE(44868533, 154605600, 78479270, 231907228);
                me->SetMaxHealth(preNerfHealth);
                me->SetHealth(preNerfHealth);
            }
            if (instance)
                instance->SetBossState(DATA_ALYSRAZOR, NOT_STARTED);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_POWER_BURN, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            events.Reset();
            me->SetReactState(REACT_AGGRESSIVE);
            featherCount = 0;
            cataCast = 0;
            softEnrage = 0;
            mui_hardEnrageTimer = 900000;
            _Reset();
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(YELL_DEAD);
            me->RemoveAllAuras();
            RemoveEncounterAuras();
            if (instance)
            {
                instance->SetBossState(DATA_ALYSRAZOR, DONE);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove
                instance->SetData(DATA_GROUND_STATE, BUILDING_STATE_INTACT);
            }

            _JustDied();

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->HasQuestForItem(ITEM_HEART_OF_FLAME))
                {
                    DoCast(me, SPELL_SMOULDERING, true);
                    break;
                }
            }
        }

        void EnterEvadeMode() override
        {
            Reset();

            if (instance)
            {
                instance->SetBossState(DATA_ALYSRAZOR, FAIL);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove

                if (GameObject* volcano =  me->FindNearestGameObject(GO_VOLCANO_ALYS, 1000.0f))
                    volcano->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING);
                if (GameObject* ground =  me->FindNearestGameObject(GO_GROUND_ALYS, 1000.0f))
                {
                    instance->SetData(DATA_GROUND_STATE, BUILDING_STATE_INTACT);
                    ground->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING);
                }
            }

            DespawnMinions();
            RemoveEncounterAuras();

            me->DespawnOrUnsummon();

            me->SummonCreature(NPC_MOLTEN_FEATHER, Feather[0], TEMPSUMMON_MANUAL_DESPAWN);

            _EnterEvadeMode();
        }

        inline void DespawnMinions()
        {
            DespawnCreatures(NPC_ALYS_VOLCANO);
            DespawnCreatures(NPC_MOLTEN_EGG);
            DespawnCreatures(NPC_MOLTEN_EGG_2);
            DespawnCreatures(NPC_BROODMOTHER);
            DespawnCreatures(NPC_BROODMOTHER_2);
            DespawnCreatures(NPC_VORACIOUS_HATCHLING);
            DespawnCreatures(NPC_VORACIOUS_HATCHLING_2);
            DespawnCreatures(NPC_FIERY_VORTEX);
            DespawnCreatures(NPC_FIERY_TORNADO);
        }

        void DespawnCreatures(uint32 entry)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, me, entry, 1000.0f);

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                 (*iter)->DespawnOrUnsummon();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            EnterAir();
            if (GameObject* ground = me->FindNearestGameObject(GO_GROUND_ALYS, 1000.0f))
                ground->RemoveFromWorld();

            if (instance)
            {
                instance->SetBossState(DATA_ALYSRAZOR, IN_PROGRESS);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me); // Add
            }

            me->SetSpeed(MOVE_RUN, 2.0f);
            me->SetSpeed(MOVE_FLIGHT, 1.4f);
            Talk(YELL_AGGRO);
            DoCast(me, SPELL_INDICATOR);

            _EnterCombat();
        }

        void EnterGround()
        {
            me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
        }

        void EnterAir()
        {
            me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
            me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            events.SetPhase(PHASE_JUMP);
            me->SetReactState(REACT_PASSIVE);
            DoZoneInCombat();
            me->GetMotionMaster()->MoveJump(SpawnPos[1].GetPositionX(),SpawnPos[1].GetPositionY(),SpawnPos[1].GetPositionZ(), 200, 8);
            EnterAir();
            events.ScheduleEvent(EVENT_START_ENCOUNTER, 500);
        }

        void KilledUnit(Unit* victim) override
        {
            Talk(YELL_KILL_PLAYER);
        }

        void JustSummoned(Creature* summon) override
        {
            switch (summon->GetEntry())
            {
                case NPC_INCENDIARY_CLOUD:
                    summon->CastSpell(summon, SPELL_INCENDIARY_CLOUD, true);
                    break;
                case NPC_BLAZING_POWER:
                    summon->SetCanFly(true);
                    summon->CastSpell(me, SPELL_BLAZING_POWER);
                    break;
                case NPC_HERALD_OF_THE_BURNING_END:
                    summon->SetReactState(REACT_PASSIVE);
                    summon->CastSpell(summon, SPELL_RITUEL_OF_FLAMME, true);
                    summon->CastWithDelay(5000, summon, SPELL_FUSION_METEOR_MISSILE, true);
                    summon->CastWithDelay(5100, summon, SPELL_METEOR_IMPACT, true);
                    summon->CastSpell(summon, SPELL_CATACLYSME, false);
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                    break;
                case NPC_FUSION_METEOR:
                    summon->CastSpell(summon, SPELL_METEOR_PERIODIC_AURA, true);
                    break;
                case NPC_FUSION_ROCK:
                    summon->SetReactState(REACT_PASSIVE);
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                    summon->CastSpell(summon, SPELL_FUSION_ROCK_PERIODIC_AURA, true);
                    break;
                case NPC_DEAD_METEOR:
                    summon->CastSpell(summon, SPELL_DEATH_FUSION_METEOR_AURA, true);
                    break;
            }
            BossAI::JustSummoned(summon);
        }

        void MovementInform(uint32 movementType, uint32 pointId) override
        {
            if (movementType != POINT_MOTION_TYPE)
                return;
            switch (pointId)
            {
                case POINT_START_TOP:
                    events.ScheduleEvent(EVENT_FEATHER, 0, GROUP_GROUND);
                    break;
                case POINT_FEATHER:
                    events.ScheduleEvent(EVENT_FEATHER_MOVE, 0, GROUP_GROUND);
                    break;
                case POINT_FEATHER_MOVE:
                    events.ScheduleEvent(EVENT_FEATHER_END, 0, GROUP_GROUND);
                    break;
                case POINT_TURN_TOP:
                    events.ScheduleEvent(EVENT_MOVE_AROUND_PLATFORM, 0, GROUP_GROUND);
                    break;
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            if (mui_hardEnrageTimer <= diff)
            {
                me->CastSpell(me, SPELL_BERSERK, true);
                mui_hardEnrageTimer = 900000;
            }
            else mui_hardEnrageTimer -= diff;

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (events.IsInPhase(PHASE_GROUND) && me->GetPower(POWER_ENERGY) >= 50 && !me->HasAura(SPELL_IGNITED))
            {
                EnterGround();
                TalkToMap(EMOTE_IGNITED);
                Talk(YELL_REIGNITE);
                me->RemoveAura(SPELL_BURNOUT);
                me->RemoveAura(SPELL_SPARK);
                me->CastSpell(me, SPELL_IGNITED, true);
                me->CastWithDelay(6000, me, SPELL_BLAZING_CLAW, true);
                RemoveAllFeathers();
                EnterAir();
                me->SetReactState(REACT_AGGRESSIVE);
                me->AI()->AttackStart(me->SelectVictim());
            }

            if (events.IsInPhase(PHASE_GROUND) && softEnrage < 3 && me->GetPower(POWER_ENERGY) >= 100 && me->HasAura(SPELL_IGNITED))
            {
                softEnrage++;
                if (softEnrage < 3)
                {
                    events.SetPhase(PHASE_AIR);
                    TalkToMap(EMOTE_FULL_POWER);
                    featherCount++; // feather spawn with SPELL_FULL_POWER
                    NextTalon = 0;
                    cataCast = 0;
                    me->RemoveAura(SPELL_IGNITED);
                    if (me->HasAura(SPELL_BLAZING_CLAW))
                        me->RemoveAura(SPELL_BLAZING_CLAW);
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->InterruptNonMeleeSpells(false);
                    me->CastSpell(me, SPELL_FULL_POWER, true);
                    events.ScheduleEvent(EVENT_PHASE_2_TORNADOS_START, IsHeroic() ? 205000: 175000);
                    me->GetMotionMaster()->MovePoint(POINT_TURN_TOP, -9.44f, -310.12f, 135.322f);
                    events.ScheduleEvent(EVENT_SUMMON_WORMS, IsHeroic() ? 22000 : 32000, GROUP_GROUND);
                    events.ScheduleEvent(EVENT_SUMMON_BROODMOTHER, IsHeroic() ? 2000 : 12000, GROUP_GROUND);
                    events.ScheduleEvent(EVENT_SUMMON_TALON, 13500, GROUP_GROUND);
                    if (IsHeroic())
                    {
                        events.ScheduleEvent(EVENT_HEROIC_FIRESTORM, 70000, GROUP_GROUND);
                        events.ScheduleEvent(EVENT_SUMMON_HERALD_OF_THE_BURNING_END, 18000, GROUP_GROUND);
                    }
                    else
                        events.ScheduleEvent(EVENT_FEATHER, 60000, GROUP_GROUND);
                }
                else
                    TalkToMap(EMOTE_SOFT_ENRAGE);
            }

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_ENCOUNTER:
                        for (uint8 i = 0; i < 11; ++i)
                            me->SummonCreature(NPC_ALYS_VOLCANO, SpawnVolcanos[i].GetPositionX(), SpawnVolcanos[i].GetPositionY(), SpawnVolcanos[i].GetPositionZ(), TEMPSUMMON_MANUAL_DESPAWN);
                        me->CastSpell(me, SPELL_FIRESTORM, true); // 22:54:03.737
                        events.ScheduleEvent(EVENT_PHASE_2_TORNADOS_START, IsHeroic() ? 243000 : 196000);
                        events.ScheduleEvent(EVENT_MOVE_TO_START_OF_TOP, 500);
                        events.ScheduleEvent(EVENT_INCENDIARY_CLOUD, 34000);
                        events.ScheduleEvent(EVENT_BLAZING_POWER, 30000);
                        events.ScheduleEvent(EVENT_SUMMON_BROODMOTHER, IsHeroic() ? 20000 : 25000, GROUP_GROUND);
                        events.ScheduleEvent(EVENT_SUMMON_WORMS, IsHeroic() ? 45000 : 50000, GROUP_GROUND);
                        events.ScheduleEvent(EVENT_SUMMON_TALON, 27000, GROUP_GROUND);
                        if (IsHeroic())
                        {
                            events.ScheduleEvent(EVENT_HEROIC_FIRESTORM, 93000, GROUP_GROUND);
                            events.ScheduleEvent(EVENT_SUMMON_HERALD_OF_THE_BURNING_END, 31000, GROUP_GROUND);
                        }
                        NextTalon = 0;
                        cataCast = 0;
                        break;
                    case EVENT_INCENDIARY_CLOUD:
                    {
                        if (events.IsInPhase(PHASE_AIR))
                        {
                            if (!me->HasAura(SPELL_MOLTING_AURA))
                            {
                                if (!IsHeroic())
                                {
                                    for (uint8 cnt = 0; cnt < featherCount; cnt++)
                                    {
                                        uint32 curIndex = rand() % 4;
                                        Position pos;
                                        pos.Relocate(me->GetPositionX() + flyTriggerPos[curIndex].m_positionX, me->GetPositionY() + flyTriggerPos[curIndex].m_positionY,
                                                     me->GetPositionZ() + flyTriggerPos[curIndex].m_positionZ, me->GetOrientation());
                                        me->SummonCreature(NPC_INCENDIARY_CLOUD, pos, TEMPSUMMON_TIMED_DESPAWN, 3000);
                                    }
                                }
                                else
                                {
                                    uint32 curIndex = rand() % 2;
                                    Position pos;
                                    pos.Relocate(me->GetPositionX() + flyTriggerPos[2].m_positionX, me->GetPositionY() + flyTriggerPos[2].m_positionY,
                                                 me->GetPositionZ() + flyTriggerPos[2].m_positionZ, me->GetOrientation());
                                    me->SummonCreature(NPC_INCENDIARY_CLOUD, pos, TEMPSUMMON_TIMED_DESPAWN, 3000);
                                    pos.Relocate(me->GetPositionX() + flyTriggerPos[3].m_positionX, me->GetPositionY() + flyTriggerPos[3].m_positionY,
                                                 me->GetPositionZ() + flyTriggerPos[3].m_positionZ, me->GetOrientation());
                                    me->SummonCreature(NPC_INCENDIARY_CLOUD, pos, TEMPSUMMON_TIMED_DESPAWN, 3000);
                                    pos.Relocate(me->GetPositionX() + flyTriggerPos[curIndex].m_positionX, me->GetPositionY() + flyTriggerPos[curIndex].m_positionY,
                                                 me->GetPositionZ() + flyTriggerPos[curIndex].m_positionZ, me->GetOrientation());
                                    me->SummonCreature(NPC_INCENDIARY_CLOUD, pos, TEMPSUMMON_TIMED_DESPAWN, 3000);
                                }
                            }
                            else
                            {
                                Position destPos;
                                me->GetPosition(&destPos);
                                destPos.m_positionZ = me->GetPositionZ() + 5.0f;
                                destPos.m_positionX -= 8.0f;
                                me->SummonCreature(NPC_INCENDIARY_CLOUD, destPos, TEMPSUMMON_TIMED_DESPAWN, 3000);
                                destPos.m_positionX += 8.0f;
                                me->SummonCreature(NPC_INCENDIARY_CLOUD, destPos, TEMPSUMMON_TIMED_DESPAWN, 3000);
                                destPos.m_positionX += 8.0f;
                                destPos.m_positionZ += 5.0f;
                                me->SummonCreature(NPC_BLAZING_POWER, destPos, TEMPSUMMON_TIMED_DESPAWN, 3000);
                            }
                        }
                        events.ScheduleEvent(EVENT_INCENDIARY_CLOUD, urand(1500, 2000));
                        break;
                    }
                    case EVENT_BLAZING_POWER:
                        if (events.IsInPhase(PHASE_AIR) && !me->HasAura(SPELL_MOLTING_AURA) && me->GetPositionZ() >= 90.0f)
                        {
                            uint32 curIndex = rand() % 4 + 4;
                            Position pos;
                            pos.Relocate(me->GetPositionX() + flyTriggerPos[curIndex].m_positionX, me->GetPositionY() + flyTriggerPos[curIndex].m_positionY,
                                         me->GetPositionZ() + flyTriggerPos[curIndex].m_positionZ, me->GetOrientation());
                            me->SummonCreature(NPC_BLAZING_POWER, pos, TEMPSUMMON_TIMED_DESPAWN, 3000);
                        }
                        events.ScheduleEvent(EVENT_BLAZING_POWER, urand(3000, 5000));
                        break;
                    case EVENT_MOVE_TO_START_OF_TOP:
                        events.SetPhase(PHASE_AIR);
                        me->GetMotionMaster()->MovePoint(POINT_START_TOP, -9.44f, -310.12f, 100.0f);
                        break;
                    case EVENT_MOVE_AROUND_PLATFORM:
                        me->GetMotionMaster()->MoveAroundPoint(FallPos[0], 50.0f, 0, 135.322f);
                        break;
                    case EVENT_SUMMON_WORMS:
                    {
                        TalkToMap(EMOTE_LAVA_WORMS);
                        for(uint8 i = 0; i < 4; ++i)
                        {
                            float z = instance->GetData(DATA_GROUND_STATE) == BUILDING_STATE_INTACT ? WormsPos[i].GetPositionZ() : 50.0f;
                            me->SummonCreature(NPC_PLUMB_LAVA_WORMS, WormsPos[i].GetPositionX(), WormsPos[i].GetPositionY(), z, WormsPos[i].GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                            if (Creature* wormTarget = me->SummonCreature(NPC_LAVA_WORM_TARGET, WormsPos[i].GetPositionX(), WormsPos[i].GetPositionY(), z, WormsPos[i].GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                Position circle(WormsPos[i]);
                                circle.m_positionZ = z + 1.00f;
                                wormTarget->GetMotionMaster()->MoveAroundPoint(circle, 13.0f, RAND(true, false), 100, circle.GetPositionZ());
                            }
                        }
                        break;
                    }
                    case EVENT_SUMMON_TALON:
                        switch (NextTalon)
                        {
                            case 0: // SW + NE
                                me->SummonCreature(NPC_BLAZING_TALON_INITIATE, Talon[0].GetPositionX(), Talon[0].GetPositionY(), Talon[0].GetPositionZ(), Talon[0].GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                                me->SummonCreature(NPC_BLAZING_TALON_INITIATE, Talon[4].GetPositionX(), Talon[4].GetPositionY(), Talon[4].GetPositionZ(), Talon[4].GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                                events.ScheduleEvent(EVENT_SUMMON_TALON, IsHeroic() ? 22000 : 31000, GROUP_GROUND);
                                break;
                            case 1: // NW + SE
                                me->SummonCreature(NPC_BLAZING_TALON_INITIATE, Talon[6].GetPositionX(), Talon[6].GetPositionY(), Talon[6].GetPositionZ(), Talon[6].GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                                me->SummonCreature(NPC_BLAZING_TALON_INITIATE, Talon[2].GetPositionX(), Talon[2].GetPositionY(), Talon[2].GetPositionZ(), Talon[2].GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                                events.ScheduleEvent(EVENT_SUMMON_TALON, IsHeroic() ? 63000 : 31000, GROUP_GROUND);
                                break;
                            case 2: // NE
                                me->SummonCreature(NPC_BLAZING_TALON_INITIATE, Talon[0].GetPositionX(), Talon[0].GetPositionY(), Talon[0].GetPositionZ(), Talon[0].GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                                events.ScheduleEvent(EVENT_SUMMON_TALON, 21000, GROUP_GROUND);
                                break;
                            case 3: // SW
                                me->SummonCreature(NPC_BLAZING_TALON_INITIATE, Talon[4].GetPositionX(), Talon[4].GetPositionY(), Talon[4].GetPositionZ(), Talon[4].GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                                events.ScheduleEvent(EVENT_SUMMON_TALON, 21000, GROUP_GROUND);
                                break;
                            case 4: // SE
                                me->SummonCreature(NPC_BLAZING_TALON_INITIATE, Talon[2].GetPositionX(), Talon[2].GetPositionY(), Talon[2].GetPositionZ(), Talon[2].GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                                events.ScheduleEvent(EVENT_SUMMON_TALON, IsHeroic() ? 40000 : 21000, GROUP_GROUND);
                                break;
                            case 5: // NW
                                me->SummonCreature(NPC_BLAZING_TALON_INITIATE, Talon[6].GetPositionX(), Talon[6].GetPositionY(), Talon[6].GetPositionZ(), Talon[6].GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
                                break;
                            default:
                                break;
                        }
                        ++NextTalon;
                        break;
                    case EVENT_SUMMON_BROODMOTHER:
                        me->SummonCreature(NPC_BROODMOTHER, BirdsPos[0]);
                        me->SummonCreature(NPC_BROODMOTHER_2, BirdsPos[1]);
                        break;
                    case EVENT_HEROIC_FIRESTORM:
                    {
                        me->GetMotionMaster()->Clear();
                        me->DisableSpline();
                        TalkToMap(EMOTE_FIRESTORM);
                        if (Creature *centerTrigger = me->FindNearestCreature(53723, 200.0f))
                            me->CastSpell(centerTrigger, SPELL_ALYS_CHANNEl_TRACK_TARGET, true);
                        DoCast(SPELL_HEROIC_FIRESTORM);
                        summons.DespawnEntry(NPC_DEAD_METEOR, 10000);
                        std::list<GameObject *> meteorCol;
                        GetGameObjectListWithEntryInGrid(meteorCol, me, GO_METEOR_COLLISION, 500.0f);
                        for (std::list<GameObject*>::const_iterator itr =meteorCol.begin(); itr != meteorCol.end(); ++itr)
                            if (GameObject *meteor = *itr)
                                meteor->Delete(10500);
                        events.ScheduleEvent(EVENT_HEROIC_FIRESTORM, 72000, GROUP_GROUND);
                        me->SummonCreature(NPC_BROODMOTHER, BirdsPos[0]);
                        me->SummonCreature(NPC_BROODMOTHER_2, BirdsPos[1]);
                        //                        events.ScheduleEvent(EVENT_SUMMON_BROODMOTHER, 1000, GROUP_GROUND);
                        events.ScheduleEvent(EVENT_SUMMON_WORMS, 21000, GROUP_GROUND);
                        featherCount++;
                        me->CastSpell(me, SPELL_MOLTING, true);
                        events.ScheduleEvent(EVENT_FEATHER_END, 1000, GROUP_GROUND);
                        if (cataCast < 3)
                            events.ScheduleEvent(EVENT_SUMMON_HERALD_OF_THE_BURNING_END, 10000, GROUP_GROUND);
                        else if (cataCast < 5)
                            events.ScheduleEvent(EVENT_SUMMON_HERALD_OF_THE_BURNING_END, 20000, GROUP_GROUND);

                        for (auto itr : summons)
                            if (Creature* summon = ObjectAccessor::GetCreature(*me, itr))
                                if (summon->GetEntry() == NPC_BLAZING_TALON_INITIATE)
                                    summon->AI()->DoAction(ACTION_FIRESTORM);
                        break;
                    }
                    case EVENT_FEATHER:
                    {
                        featherCount++;
                        me->GetMotionMaster()->Clear();
                        me->DisableSpline();
                        float z = me->GetMap()->GetHeight(me->GetPhaseMask(), me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->GetMotionMaster()->MovePoint(POINT_FEATHER, me->GetPositionX(), me->GetPositionY(), z + 20.0f);
                        if (!IsHeroic() && featherCount < 3)
                            events.ScheduleEvent(EVENT_FEATHER, 60000, GROUP_GROUND);
                        break;
                    }
                    case EVENT_FEATHER_MOVE:
                    {
                        if (Creature *centerTrigger = me->FindNearestCreature(53723, 100.0f))
                            me->SetFacingToObject(centerTrigger);
                        Position pos;
                        me->GetNearPosition(pos, 100.0f, 0.0f);
                        me->GetMotionMaster()->MovePoint(POINT_FEATHER_MOVE, pos);
                        me->CastSpell(me, SPELL_BLAZING_CLAW, true);
                        me->CastWithDelay(1000, me, SPELL_MOLTING, true);
                        break;
                    }
                    case EVENT_FEATHER_END:
                    {
                        if (Creature *centerTrigger = me->FindNearestCreature(53723, 100.0f))
                            centerTrigger->RemoveAura(SPELL_ALYS_CHANNEl_TRACK_TARGET);
                        if (me->HasAura(SPELL_BLAZING_CLAW))
                            me->RemoveAura(SPELL_BLAZING_CLAW);
                        me->GetMotionMaster()->MovePoint(POINT_TURN_TOP, me->GetPositionX(), me->GetPositionY(), 135.322f);
                        break;
                    }
                    case EVENT_PHASE_2_TORNADOS_START:
                    {
                        Talk(YELL_VORTEX_PHASE);
                        events.CancelEventGroup(GROUP_GROUND);
                        if (me->HasAura(SPELL_BLAZING_CLAW))
                            me->RemoveAura(SPELL_BLAZING_CLAW);
                        if (Creature *fieryv = me->SummonCreature(NPC_FIERY_VORTEX, FallPos[0].GetPositionX(), FallPos[0].GetPositionY(), FallPos[0].GetPositionZ(), TEMPSUMMON_MANUAL_DESPAWN))
                            if (instance->GetData(DATA_GROUND_STATE) == BUILDING_STATE_INTACT)
                                fieryv->NearTeleportTo(fieryv->GetPositionX(), fieryv->GetPositionY(), 60.00f, me->GetOrientation());
                        events.ScheduleEvent(EVENT_BLAZING_POWER_GROUND, 0);
                        me->SetSpeed(MOVE_FLIGHT, 16.0f);
                        me->GetMotionMaster()->MovePoint(0, -74.0f, -235.0f, 115.0f);

                        summons.DespawnEntry(NPC_DEAD_METEOR, 3000);
                        std::list<GameObject *> meteorCol;
                        GetGameObjectListWithEntryInGrid(meteorCol, me, GO_METEOR_COLLISION, 500.0f);
                        for (std::list<GameObject*>::const_iterator itr = meteorCol.begin(); itr != meteorCol.end(); ++itr)
                            if (GameObject *meteor = *itr)
                                meteor->Delete(3000);

                        events.SetPhase(PHASE_TORNADO);
                        events.ScheduleEvent(EVENT_MOVE_TORNADOS, 5000);
                        events.ScheduleEvent(EVENT_PHASE_2_TORNADOS_ENDS, 35000);
                        break;
                    }
                    case EVENT_HARSH_WINDS:
                        CastHarshOnPlayers();
                        events.ScheduleEvent(EVENT_HARSH_WINDS, 2000);
                        break;
                    case EVENT_MOVE_TORNADOS:
                    {
                        DespawnCreatures(NPC_MOLTEN_FEATHER);
                        RemoveFly();
                        TalkToMap(EMOTE_VORTEX_TURN);
                        me->GetMotionMaster()->MoveAroundPoint(FallPos[0], 50.0f, 0, 125.322f);
                        events.ScheduleEvent(EVENT_HARSH_WINDS, 2000);
                        break;
                    }
                    case EVENT_SUMMON_DRUIDS:
                    {
                        for (uint8 i = 0; i < 10; i++)
                            instance->DoRemoveAurasDueToSpellOnPlayers(98761 + i);
                        RemoveFly();
                        RemoveAllFeathers();

                        DoCast(SPELL_BREAK_GROUND);
                        instance->SetData(DATA_GROUND_STATE, BUILDING_STATE_DESTROYED);
                        for (uint8 i = 0; i < 2; ++i)
                            me->SummonCreature(NPC_BLAZING_TALON_CLAWSHAPER, ClawTalon[i], TEMPSUMMON_MANUAL_DESPAWN);
                        EntryCheckPredicate pred(NPC_PLUMB_LAVA_WORMS);
                        summons.DoAction(ACTION_BREAK_GROUND, pred);
                        break;
                    }
                    case EVENT_PHASE_2_TORNADOS_ENDS:
                        featherCount = 0;
                        events.CancelEvent(EVENT_BLAZING_POWER_GROUND);
                        me->SetSpeed(MOVE_FLIGHT, 1.4f);
                        events.CancelEvent(EVENT_HARSH_WINDS);
                        DespawnCreatures(NPC_FIERY_VORTEX);
                        DespawnCreatures(NPC_FIERY_TORNADO);
                        events.SetPhase(PHASE_GROUND);
                        Talk(YELL_BURNOUT);
                        TalkToMap(EMOTE_BURNOUT);
                        me->SetPower(POWER_ENERGY, 0);
                        me->CastSpell(me, SPELL_ZERO_POWER, true);
                        me->CastSpell(me, SPELL_BURNOUT, true);
                        me->CastSpell(me, SPELL_SPARK, true);
                        me->GetMotionMaster()->MoveJump(FallPos[0].GetPositionX(),FallPos[0].GetPositionY(),FallPos[0].GetPositionZ(),100,4);
                        events.ScheduleEvent(EVENT_SUMMON_DRUIDS, 500);
                        break;
                    case EVENT_BLAZING_POWER_GROUND:
                    {
                        Position pos;
                        me->GetRandomPoint(FallPos[0], 30, pos);
                        pos.m_positionZ = instance->GetData(DATA_GROUND_STATE) == BUILDING_STATE_INTACT ? 56.5f : 52.0f;
                        if (Creature* c = me->SummonCreature(NPC_BLAZING_POWER, pos, TEMPSUMMON_TIMED_DESPAWN, 3000))
                            c->SetFacingTo(c->GetAngle(FallPos[0].m_positionX, FallPos[0].m_positionY) + M_PI / 2);
                        events.ScheduleEvent(EVENT_BLAZING_POWER_GROUND, 4000);
                        break;
                    }
                    case EVENT_SUMMON_HERALD_OF_THE_BURNING_END:
                    {
                        cataCast++;
                        float z = instance->GetData(DATA_GROUND_STATE) == BUILDING_STATE_INTACT ? 54.4f : 50.0f;
                        me->SummonCreature(NPC_HERALD_OF_THE_BURNING_END, -42.39f, -275.42f, z, 5.68f, TEMPSUMMON_TIMED_DESPAWN, 6000);
                        if (cataCast == 1 || cataCast == 3)
                            events.ScheduleEvent(EVENT_SUMMON_HERALD_OF_THE_BURNING_END, 31000, GROUP_GROUND);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        void CastHarshOnPlayers()
        {
            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if (i->getSource()->GetDistance(me) > 60.0f)
                        me->AddAura(SPELL_HARSH_WINDS,i->getSource());
        }

        void SetPlayersInCombat()
        {
            uint8 players = 0;
            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if (Player *player = i->getSource())
                        if (player->isAlive() && !player->isGameMaster())
                        {
                            ++players;
                            player->SetInCombatWith(me->ToUnit());
                        }
            if (players == 0)
                EnterEvadeMode();
        }

        void RemoveFly()
        {
            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    i->getSource()->RemoveAura(SPELL_FLY);
        }

        void RemoveAllFeathers()
        {
            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if (i->getSource()->HasAura(SPELL_CAST_ON_MOVE_VISUAL))
                    {
                        i->getSource()->RemoveAura(SPELL_CAST_ON_MOVE_VISUAL);
                        i->getSource()->SetPower(POWER_ALTERNATE_POWER, 0);
                    }
        }

        void RemoveEncounterAuras()
        {
            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->HasAura(SPELL_FLY))
                        i->getSource()->RemoveAura(SPELL_FLY);
                    if (i->getSource()->HasAura(SPELL_INDICATOR))
                        i->getSource()->RemoveAura(SPELL_INDICATOR);
                    if (i->getSource()->HasAura(SPELL_CAST_ON_MOVE_VISUAL))
                        i->getSource()->RemoveAura(SPELL_CAST_ON_MOVE_VISUAL);
                    if (i->getSource()->HasAura(SPELL_IMPRINTED))
                        i->getSource()->RemoveAura(SPELL_IMPRINTED);
                    if (i->getSource()->HasAura(SPELL_IMPRINTED_2))
                        i->getSource()->RemoveAura(SPELL_IMPRINTED_2);
                }
        }

    private:
        uint8 NextTalon;
        uint8 featherCount;
        uint8 cataCast;
        uint8 softEnrage;
        uint32 mui_hardEnrageTimer;
    };
};

class mob_molten_feather : public CreatureScript // NPC_MOLTEN_FEATHER
{
public:
    mob_molten_feather() : CreatureScript("mob_molten_feather") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_molten_featherAI(creature);
    }

    struct mob_molten_featherAI : public ScriptedAI
    {
        mob_molten_featherAI(Creature* creature) : ScriptedAI(creature), instance(me->GetInstanceScript()) { }

        void InitializeAI()
        {
            me->SetReactState(REACT_PASSIVE);
            ScriptedAI::InitializeAI();
        }

        bool CanAIAttack(Unit const* /*target*/) const override
        {
            return false;
        }

        void Reset() override
        {
            me->CastWithDelay(1000, me, SPELL_COSMETIC, true);
        }

        void IsSummonedBy(Unit* owner) override
        {
            Position pos;
            owner->GetRandomNearPosition(pos, 2.0f);
            pos.m_positionZ = 60.0f;
            me->NearTeleportTo(pos, false);
        }

        void castFeatherCastWhileWalkingOnPlayerClass(Player *player)
        {
            if (!player)
                return;
            uint32 spellId = SPELL_FIRE_FEATHER_MAGE;
            switch (player->getClass())
            {
                case CLASS_MAGE:
                    spellId = SPELL_FIRE_FEATHER_MAGE;
                    break;
                case CLASS_WARRIOR:
                    spellId = SPELL_FIRE_FEATHER_WARRIOR;
                    break;
                case CLASS_WARLOCK:
                    spellId = SPELL_FIRE_FEATHER_WARLOCK;
                    break;
                case CLASS_PRIEST:
                    spellId = SPELL_FIRE_FEATHER_PRIEST;
                    break;
                case CLASS_DRUID:
                    spellId = SPELL_FIRE_FEATHER_DRUID;
                    break;
                case CLASS_ROGUE:
                    spellId = SPELL_FIRE_FEATHER_ROGUE;
                    break;
                case CLASS_HUNTER:
                    spellId = SPELL_FIRE_FEATHER_HUNTER;
                    break;
                case CLASS_PALADIN:
                    spellId = SPELL_FIRE_FEATHER_PALADIN;
                    break;
                case CLASS_SHAMAN:
                    spellId = SPELL_FIRE_FEATHER_SHAMAN;
                    break;
                case CLASS_DEATH_KNIGHT:
                    spellId = SPELL_FIRE_FEATHER_DEATHNIGHT;
                    break;
            }
            player->CastSpell(player, spellId, true);
        }

        void OnSpellClick(Unit* player, bool& result) override
        {
            if (Creature* alysrazor = Unit::GetCreature(*me, instance->GetData64(DATA_ALYSRAZOR)))
            {
                player->CastSpell(player, SPELL_CAST_ON_MOVE_VISUAL, true);
                castFeatherCastWhileWalkingOnPlayerClass(player->ToPlayer());
                if (Aura* aura = player->GetAura(SPELL_CAST_ON_MOVE_VISUAL))
                    if (aura->GetStackAmount() == 3 && !player->HasAura(SPELL_FLY))
                        player->CastSpell(player, SPELL_PLAYER_FLY, true);
            }
            else
                me->SummonCreature(BOSS_ALYSRAZOR, SpawnPos[0], TEMPSUMMON_MANUAL_DESPAWN);

            me->DespawnOrUnsummon();
        }

    private:
        InstanceScript *instance;
    };
};

class npc_alysrazor_volcano : public CreatureScript // NPC_ALYS_VOLCANO
{
public:
    npc_alysrazor_volcano() : CreatureScript("npc_alysrazor_volcano") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_alysrazor_volcanoAI(creature);
    }

    struct npc_alysrazor_volcanoAI : public ScriptedAI
    {
        npc_alysrazor_volcanoAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override { }

        void IsSummonedBy(Unit* summoner) override
        {
            summoner->CastSpell(me, SPELL_VOLCANO_DUMMY, false);
            events.ScheduleEvent(EVENT_VOLCANO_AURA, 2000);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_VOLCANO_AURA:
                        if (!me->HasAura(SPELL_VOLCANO_AURA))
                            me->CastSpell(me, SPELL_VOLCANO_AURA, true);
                        break;
                }
            }
        }

    private:
        EventMap events;
    };
};

class npc_voracious_hatchling : public CreatureScript // NPC_VORACIOUS_HATCHLING
{
public:
    npc_voracious_hatchling() : CreatureScript("npc_voracious_hatchling") { }

    struct npc_voracious_hatchlingAI : public ScriptedAI
    {
        npc_voracious_hatchlingAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            isStarted = false;
            targetGUID = 0;
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
            me->CastSpell(me, SPELL_FIREHAWK_COSMETIC, true);
            me->SetReactState(REACT_PASSIVE);
            events.ScheduleEvent(EVENT_START_COMBAT, 3000);
        }

        void EnterCombat(Unit* target) override {}

        bool CanAIAttack(Unit const* target) const override
        {
            return !target->HasAura(SPELL_FLY);
        }

        void JustDied(Unit* killer) override
        {
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_IMPRINTED, me->GetGUID());
        }

        void SetGUID(uint64 guid, int32) override
        {
            targetGUID = guid;
        }

        void UpdateAI(const uint32 diff) override
        {
            if (Unit* victim = ObjectAccessor::GetUnit(*me, targetGUID))
            {
                if (victim != me->getVictim() && victim->isAlive())
                    victim->CastSpell(me, SPELL_IMPRINTED_TAUNT, true);

                if (!victim->isAlive() || !victim->IsInMap(me) || !victim->IsInWorld() || !victim->HasAura(SPELL_IMPRINTED) || victim->HasAura(SPELL_FLY))
                {
                    victim->RemoveAurasDueToSpell(SPELL_IMPRINTED);
                    DoCastAOE(SPELL_IMPRINTED_TARGETING);
                }
            }
            else if (isStarted)
                DoCastAOE(SPELL_IMPRINTED_TARGETING);

            if (!UpdateVictim() && isStarted)
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_COMBAT:
                        me->SetInCombatWithZone();
                        me->SetReactState(REACT_AGGRESSIVE);
                        isStarted = true;
                        DoCastAOE(SPELL_IMPRINTED_TARGETING);
                        me->ModifyAuraState(AURA_STATE_UNKNOWN22, true);
                        events.ScheduleEvent(EVENT_EAT_WORM, 1000);
                        events.ScheduleEvent(EVENT_HUNGRY, 9000);
                        events.ScheduleEvent(EVENT_DEBUFF, 15000);
                        break;
                    case EVENT_DEBUFF:
                    {
                        DoCastVictim(SPELL_GUSHING_WOUND);
                        events.ScheduleEvent(EVENT_DEBUFF, urand(8000, 30000));
                        break;
                    }
                    case EVENT_EAT_WORM:
                    {
                        if (Creature* worm = me->FindNearestCreature(NPC_PLUMB_LAVA_WORMS, 10.0f, true))
                        {
                            me->SetFacingToObject(worm);
                            me->Kill(worm);
                            DoCast(me, SPELL_SATIATED);
                            me->RemoveAurasDueToSpell(SPELL_HUNGRY);
                            me->RemoveAurasDueToSpell(SPELL_TANTRUM);
                        }
                        events.ScheduleEvent(EVENT_EAT_WORM, 4000);
                        break;
                    }
                    case EVENT_HUNGRY:
                    {
                        if (!me->HasAura(SPELL_SATIATED) && !me->HasAura(SPELL_HUNGRY))
                        {
                            DoCast(me, SPELL_HUNGRY);
                            events.ScheduleEvent(EVENT_HUNGRY, 9000);
                        }
                        else
                            events.ScheduleEvent(EVENT_HUNGRY, 2000);
                        break;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        uint64 targetGUID;
        InstanceScript *instance;
        bool isStarted;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_voracious_hatchlingAI(creature);
    }
};

class npc_voracious_hatchling_two : public CreatureScript // NPC_VORACIOUS_HATCHLING_2
{
public:
    npc_voracious_hatchling_two() : CreatureScript("npc_voracious_hatchling_two") { }

    struct npc_voracious_hatchling_twoAI : public ScriptedAI
    {
        npc_voracious_hatchling_twoAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            targetGUID = 0;
            isStarted = false;
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
            me->CastSpell(me, SPELL_FIREHAWK_COSMETIC, true);
            me->SetReactState(REACT_PASSIVE);
            events.ScheduleEvent(EVENT_START_COMBAT, 3000);
        }

        void EnterCombat(Unit* target) override {}


        bool CanAIAttack(Unit const* target) const override
        {
            return !target->HasAura(SPELL_FLY);
        }

        void JustDied(Unit* killer) override
        {
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_IMPRINTED_2, me->GetGUID());
        }

        void SetGUID(uint64 guid, int32) override
        {
            targetGUID = guid;
        }

        void UpdateAI(const uint32 diff) override
        {
            if (Unit* victim = ObjectAccessor::GetUnit(*me, targetGUID))
            {
                if (victim != me->getVictim() && victim->isAlive())
                    victim->CastSpell(me, SPELL_IMPRINTED_TAUNT_2, true);

                if (!victim->isAlive() || !victim->IsInMap(me) || !victim->IsInWorld() || !victim->HasAura(SPELL_IMPRINTED_2) || victim->HasAura(SPELL_FLY))
                {
                    victim->RemoveAurasDueToSpell(SPELL_IMPRINTED_2);
                    DoCastAOE(SPELL_IMPRINTED_TARGETING_2);
                }
            }
            else if (isStarted)
                DoCastAOE(SPELL_IMPRINTED_TARGETING_2);

            if (!UpdateVictim() && isStarted)
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_COMBAT:
                        me->SetInCombatWithZone();
                        me->SetReactState(REACT_AGGRESSIVE);
                        isStarted = true;
                        DoCastAOE(SPELL_IMPRINTED_TARGETING_2);
                        me->ModifyAuraState(AURA_STATE_UNKNOWN20, true);
                        events.ScheduleEvent(EVENT_EAT_WORM, 1000);
                        events.ScheduleEvent(EVENT_HUNGRY, 9000);
                        events.ScheduleEvent(EVENT_DEBUFF, 15000);
                        break;
                    case EVENT_DEBUFF:
                    {
                        DoCastVictim(SPELL_GUSHING_WOUND);
                        events.ScheduleEvent(EVENT_DEBUFF, urand(8000, 30000));
                        break;
                    }
                    case EVENT_EAT_WORM:
                    {
                        if (Creature* worm = me->FindNearestCreature(NPC_PLUMB_LAVA_WORMS, 10.0f, true))
                        {
                            me->SetFacingToObject(worm);
                            me->Kill(worm);
                            DoCast(me, SPELL_SATIATED);
                            me->RemoveAurasDueToSpell(SPELL_HUNGRY);
                            me->RemoveAurasDueToSpell(SPELL_TANTRUM);
                        }
                        events.ScheduleEvent(EVENT_EAT_WORM, 4000);
                        break;
                    }
                    case EVENT_HUNGRY:
                    {
                        if (!me->HasAura(SPELL_SATIATED) && !me->HasAura(SPELL_HUNGRY))
                        {
                            DoCast(me, SPELL_HUNGRY);
                            events.ScheduleEvent(EVENT_HUNGRY, 9000);
                        }
                        else
                            events.ScheduleEvent(EVENT_HUNGRY, 2000);
                        break;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        uint64 targetGUID;
        InstanceScript* instance;
        bool isStarted;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_voracious_hatchling_twoAI(creature);
    }
};

class npc_plumb_lava_worm : public CreatureScript // NPC_PLUMB_LAVA_WORMS
{
public:
    npc_plumb_lava_worm() : CreatureScript("npc_plumb_lava_worm") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_plumb_lava_wormAI (creature);
    }

    struct npc_plumb_lava_wormAI : public ScriptedAI
    {
        npc_plumb_lava_wormAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner) override
        {
            me->SetReactState(REACT_PASSIVE);
            events.ScheduleEvent(EVENT_LAVA_SPEW, 5000, GROUP_GROUND);
        }

        void DoAction(const int32 action) override
        {
            if (action == ACTION_BREAK_GROUND)
            {
                me->InterruptNonMeleeSpells(false);
                events.CancelEventGroup(GROUP_GROUND);
                events.ScheduleEvent(EVENT_LAVA_SPEW, 2000, GROUP_GROUND);
                me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 50.0f, me->GetOrientation());
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_LAVA_SPEW:
                        if (Creature* spellTarget = me->FindNearestCreature(NPC_LAVA_WORM_TARGET, 30.00f))
                            DoCast(spellTarget, SPELL_LAVA_SPEW);
                        break;
                }
            }
        }

    private:
        EventMap events;
    };
};

class npc_blazing_broodmother : public CreatureScript // NPC_BROODMOTHER
{
public:
    npc_blazing_broodmother() : CreatureScript("npc_blazing_broodmother") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_blazing_broodmotherAI (creature);
    }

    struct npc_blazing_broodmotherAI : public npc_escortAI
    {
        npc_blazing_broodmotherAI(Creature* c) : npc_escortAI(c)
        {
            instance = me->GetInstanceScript();
        }

        void IsSummonedBy(Unit* summoner) override
        {
            eggGUID = 0;
            me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
            me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
            me->InterruptNonMeleeSpells(false);

            me->SetCanFly(true);
            SetDespawnAtFar(false);
            me->CastSpell(me, SPELL_BROODMOTHER_VIS, true);
            me->CastSpell(me, SPELL_BROODMOTHER_VIS1, true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            if (Creature * egg = me->SummonCreature(me->GetEntry() == NPC_BROODMOTHER ? NPC_MOLTEN_EGG : NPC_MOLTEN_EGG_2, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation()))
            {
                egg->CastCustomSpell(VEHICLE_SPELL_RIDE_HARDCODED, SPELLVALUE_BASE_POINT0, 1, me, TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE);
                eggGUID = egg->GetGUID();
            }
            if (me->GetPositionX() > 0)
                InitLeftBirdWaypoints();
            else
                InitRightBirdWaypoints();
            Start(false, false, me->GetGUID());
        }

        void InitLeftBirdWaypoints()
        {
            AddWaypoint(1, -32.901f, -272.019f, 75.0f, 1000); //left egg pos z+5
            AddWaypoint(2, 46.930f, -207.560f, 90.000f); //left bird point to despawn
        }

        void InitRightBirdWaypoints()
        {
            AddWaypoint(1, -44.463f, -285.802f, 75.0f, 1000); //right egg pos z+5
            AddWaypoint(2, -28.813f, -378.088f, 90.371f); //right bird point to despawn
        }

        void WaypointReached(uint32 point) override
        {
            switch (point)
            {
                case 1:
                {
                    if (Creature *egg = Unit::GetCreature(*me, eggGUID))
                        egg->AI()->DoAction(ACTION_LAUNCH_EGG);

                    Position pos(*me);
                    if (instance->GetData(DATA_GROUND_STATE) == BUILDING_STATE_INTACT)
                        pos.m_positionZ = 55.0f;
                    else
                        pos.m_positionZ = 48.0f;

                    if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                        passenger->ExitVehicle(&pos);
                    break;
                }
                case 2:
                    me->DisappearAndDie();
                    break;
            }
        }

    private:
        uint64 eggGUID;
        InstanceScript* instance;
    };
};

class npc_blazing_talon_clawshaper : public CreatureScript // NPC_BLAZING_TALON_CLAWSHAPER
{
public:
    npc_blazing_talon_clawshaper() : CreatureScript("npc_blazing_talon_clawshaper") { }

    struct npc_blazing_talon_clawshaperAI : public ScriptedAI
    {
        npc_blazing_talon_clawshaperAI(Creature* c) : ScriptedAI(c), instance(c->GetInstanceScript()) {}

        void IsSummonedBy(Unit* summoner) override
        {
            castIgnition = true;
            me->SetReactState(REACT_PASSIVE);
            me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
            me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->InterruptNonMeleeSpells(false);
            me->SetSpeed(MOVE_FLIGHT, 3.5f);
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            if (me->GetPositionX() > -35.00f)
                me->GetMotionMaster()->MovePoint(1, -34.850f, -240.583f, 77.014f, false, 3.5f);
            else
                me->GetMotionMaster()->MovePoint(2, -55.794f, -325.271f, 77.766f, false, 3.5f);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (pointId)
            {
                case 1:
                    me->GetMotionMaster()->MovePoint(3, -48.5921f, -264.4634f, 55.00f, false, 3.5f);
                    break;
                case 2:
                    me->GetMotionMaster()->MovePoint(3, -62.971f, -295.9055f, 55.00f, false, 3.5f);
                    break;
                case 3:
                    DoCast(me, SPELL_FIREHAWK_CLAWSHAPPER);
                    me->GetMotionMaster()->MoveFall();
                    me->SetCanFly(false);
                    me->SetDisableGravity(false);
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
                    TalkToMap(YELL_CLAWSHAPER);
                    events.ScheduleEvent(EVENT_START_IGNITION, 2000);
                    events.ScheduleEvent(EVENT_CHECK_ENERGY, 12000);
                    break;
                case 4:
                    me->DespawnOrUnsummon(1000);
                    break;
            }
        }

        void SpellHit(Unit* attacker, SpellInfo const* spellInfo)
        {
            bool channelInterrupt = false;
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                if (spellInfo->Effects[i].Mechanic == MECHANIC_DISORIENTED && spellInfo->Mechanic == MECHANIC_NONE || spellInfo->Effects[i].Mechanic == MECHANIC_SILENCE && spellInfo->Mechanic == MECHANIC_NONE
                    || spellInfo->Effects[i].Mechanic == MECHANIC_STUN && spellInfo->Mechanic == MECHANIC_NONE || spellInfo->Effects[i].Mechanic == MECHANIC_KNOCKOUT && spellInfo->Mechanic == MECHANIC_NONE
                    || spellInfo->Effects[i].Mechanic == MECHANIC_GRIP && spellInfo->Mechanic == MECHANIC_NONE || spellInfo->Effects[i].Mechanic == MECHANIC_FEAR && spellInfo->Mechanic == MECHANIC_NONE)
                    channelInterrupt = true;

            // special check some spells only have a mechanic but not effect mechanic
            if (spellInfo->Mechanic == MECHANIC_DISORIENTED || spellInfo->Mechanic == MECHANIC_SILENCE
                || spellInfo->Mechanic == MECHANIC_STUN || spellInfo->Mechanic == MECHANIC_KNOCKOUT
                || spellInfo->Mechanic == MECHANIC_GRIP || spellInfo->Mechanic == MECHANIC_FEAR)
                channelInterrupt = true;

            if (channelInterrupt && events.GetNextEventTime(EVENT_START_IGNITION) == 0) // 5 sec after interrupt the channel restarts
                events.ScheduleEvent(EVENT_START_IGNITION, 5000);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_IGNITION:
                    {
                        if (Creature* alysrazor = Creature::GetCreature(*me, instance->GetData64(DATA_ALYSRAZOR)))
                        {
                            if (alysrazor->GetPower(POWER_ENERGY) < 50)
                            {
                                me->StopMoving(); // make sure they dont kite interrupt the cast (old cata exploit)
                                DoCast(alysrazor, SPELL_IGNITION);
                            }
                        }
                        break;
                    }
                    case EVENT_CHECK_ENERGY:
                    {
                        if (Creature* alysrazor = Creature::GetCreature(*me, instance->GetData64(DATA_ALYSRAZOR)))
                        {
                            if (alysrazor->GetPower(POWER_ENERGY) >= 50)
                            {
                                DoCast(me, SPELL_FIREHAWK_REMOVE_FORM);
                                me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
                                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                                me->SetCanFly(true);
                                me->SetDisableGravity(true);
                                me->AttackStop();
                                me->InterruptNonMeleeSpells(false);
                                me->GetMotionMaster()->MovePoint(4, ClawTalon[me->GetPositionX() > -35 ? 0 : 1].GetPositionX(), ClawTalon[me->GetPositionX() > -35 ? 0 : 1].GetPositionY(), ClawTalon[me->GetPositionX() > -35 ? 0 : 1].GetPositionZ());
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                        }
                        break;
                    }
                }
            }
        }

    private:
        EventMap events;
        InstanceScript* instance;
        bool castIgnition;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_blazing_talon_clawshaperAI(creature);
    }
};

class npc_blazing_talon : public CreatureScript // NPC_BLAZING_TALON_INITIATE
{
public:
    npc_blazing_talon() : CreatureScript("npc_blazing_talon") { }

    struct npc_blazing_talonAI : public ScriptedAI
    {
        npc_blazing_talonAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner) override
        {
            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
            {
                uint32 preNerfHealth = me->GetHealth() / 0.75f;
                me->SetMaxHealth(preNerfHealth);
                me->SetHealth(preNerfHealth);
            }
            canUseShield = false;
            me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
            me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
            me->AttackStop();
            me->SetReactState(REACT_PASSIVE);
            me->InterruptNonMeleeSpells(false);
            me->SetCanFly(true);
            for (uint8 i = 0; i < 4; ++i)
            {
                uint8 tal = 0;

                     if (i == 0) tal = 0;
                else if (i == 1) tal = 2;
                else if (i == 2) tal = 4;
                else if (i == 3) tal = 6;

                if (Talon[tal].GetPositionX() == me->GetPositionX())
                    waypoint = ++tal;
            }
            me->GetMotionMaster()->MovePoint(1, Talon[waypoint].GetPositionX(), Talon[waypoint].GetPositionY(), Talon[waypoint].GetPositionZ());
        }

        void MovementInform(uint32 type, uint32 point)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (point)
            {
                case 1:
                    canUseShield = true;
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoCast(me,urand(0,1) == 0 ? SPELL_FIREHAWK_TRANSFORM_M : SPELL_FIREHAWK_TRANSFORM_F);
                    me->GetMotionMaster()->MoveFall();
                    events.ScheduleEvent(EVENT_START_COMBAT, 500);
                    me->SetCanFly(false);
                    me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    Talk(YELL_INITIATE);
                    break;
            }
        }

        void DoAction(int32 const action)
        {
            if (action == ACTION_FIRESTORM && canUseShield)
                DoCast(me, SPELL_BLAZING_SHIELD, true);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (me->HasAura(SPELL_BLAZING_SHIELD)) // dont update event timers if blazing shield is active
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_COMBAT:
                        if (Unit* nearTarget = me->SelectNearestTarget(5000.0f))
                            me->AI()->AttackStart(nearTarget);
                        events.ScheduleEvent(EVENT_SPELL_CAST, 500);
                        break;
                    case EVENT_SPELL_CAST:
                        // 3 - 1 - 3 - 1 - 3 - 1 cast squence on blizz
                        if (++bushfireCasts > 3)
                        {
                            // always targets nearest player
                            if (Player* plr = me->FindNearestPlayer(300.00f))
                                DoCast(plr, SPELL_FIEROBLAST, false);
                            bushfireCasts = 0;
                        }
                        else
                            DoCastRandom(SPELL_BRUSHFIRE, 500.00f);

                        events.ScheduleEvent(EVENT_SPELL_CAST, 1500);
                        break;
                }
            }
        }

    private:
        EventMap events;
        uint8 waypoint;
        int8 bushfireCasts = 0;
        bool canUseShield;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_blazing_talonAI(creature);
    }
};

class npc_brushfire : public CreatureScript // 53372
{
public:
    npc_brushfire() : CreatureScript("npc_brushfire") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_brushfireAI (creature);
    }

    struct npc_brushfireAI : public ScriptedAI
    {
        npc_brushfireAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner) override
        {
            me->CastSpell(me, SPELL_BRUSHFIRE_VIS, true);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            events.ScheduleEvent(EVENT_MOVE, 1000);
            events.ScheduleEvent(EVENT_CHECK_DESPAWN, 1000);
            me->DespawnOrUnsummon(20000);
        }

        void MovementInform(uint32 movementType, uint32 pointId) override
        {
            if (movementType != POINT_MOTION_TYPE)
                return;

            if (pointId == POINT_START_TOP)
                me->DespawnOrUnsummon(200);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHECK_DESPAWN:
                        if (me->GetPositionZ() >= 58.00f)
                            me->DespawnOrUnsummon(200);
                        events.ScheduleEvent(EVENT_CHECK_DESPAWN, 1000);
                        break;
                    case EVENT_MOVE:
                    {
                        Position pos;
                        me->GetFirstCollisionPosition(pos, 100.0f, 0.0f);
                        me->GetMotionMaster()->MovePoint(POINT_START_TOP, pos);
                        break;
                    }
                }
            }
        }

    private:
        EventMap events;
    };
};

enum fieryEvent
{
    EVENT_TORNADO_POSITION = 0,
};

class npc_fiery_tornado : public CreatureScript // NPC_FIERY_TORNADO
{
public:
    npc_fiery_tornado() : CreatureScript("npc_fiery_tornado") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fiery_tornadoAI (creature);
    }

    struct npc_fiery_tornadoAI : public ScriptedAI
    {
        npc_fiery_tornadoAI(Creature* c) : ScriptedAI(c) { }

        void IsSummonedBy(Unit* summoner) override
        {
            numb = 0;
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetCanFly(true);
            me->GetPosition(&center);
            DoCast(SPELL_FIERY_TORNADO_AURA);
        }

        void SetData(uint32 /*DataId*/, uint32 value) override
        {
            numb = value;
        }

        void MovementInform(uint32 movementType, uint32 pointId) override
        {
            if (movementType != POINT_MOTION_TYPE)
                return;
            switch (pointId)
            {
                case POINT_TORNADO:
                    events.ScheduleEvent(EVENT_MOVE_AROUND_POINT, 1000);
                    break;
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_AROUND_POINT:
                    {
                        float ray = me->GetDistance2d(center.m_positionX, center.m_positionY);
                        me->GetMotionMaster()->MoveAroundPoint(center, ray, (bool)numb, 254);
                        break;
                    }
                }
            }
        }

    private:
        uint8 numb;
        Position center;
        EventMap events;
    };
};

class npc_molten_egg : public CreatureScript // NPC_MOLTEN_EGG
{
public:
    npc_molten_egg() : CreatureScript("npc_molten_egg") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_molten_eggAI (creature);
    }

    struct npc_molten_eggAI : public ScriptedAI
    {
        npc_molten_eggAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

        void IsSummonedBy(Unit* summoner) override
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        }

        void DoAction(const int32 action) override
        {
            if (action == ACTION_LAUNCH_EGG)
                events.ScheduleEvent(EVENT_EGG_EXPLOSION, 9000);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_EGG_EXPLOSION:
                        if (Creature* alysrazor = Unit::GetCreature(*me, instance->GetData64(DATA_ALYSRAZOR)))
                            alysrazor->AI()->TalkToMap(EMOTE_MOLTEN_EGGS);
                        DoCast(me, SPELL_EGG_EXPLOSION);
                        events.ScheduleEvent(EVENT_SUMMON_HATCHING, 5000);
                        break;
                    case EVENT_SUMMON_HATCHING:
                        me->CastSpell(me, me->GetEntry() == NPC_MOLTEN_EGG ? SPELL_SUMM_HATHING : SPELL_SUMM_HATHING_2, true);
                        me->CastSpell(me, SPELL_CREATE_FRAG, true);
                        events.ScheduleEvent(EVENT_DESPAWN, 300);
                        break;
                    case EVENT_DESPAWN:
                        me->DespawnOrUnsummon();
                        break;
                }
            }
        }

    private:
        EventMap events;
        InstanceScript* instance;
    };
};

class npc_fiery_vortex : public CreatureScript // NPC_FIERY_VORTEX
{
public:
    npc_fiery_vortex() : CreatureScript("npc_fiery_vortex") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fiery_vortexAI(creature);
    }

    struct npc_fiery_vortexAI : public ScriptedAI
    {
        npc_fiery_vortexAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner) override
        {
            DoCast(me, SPELL_SUMMON_DMG, false);
            events.ScheduleEvent(EVENT_SUMMON_TORNADO, 5100);
            TalkToMap(EMOTE_FIERY_VORTEX);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_TORNADO:
                    {
                        int32 maxTornado = 2;
                        float count = 0.0f;
                        bool dir = false;
                        for (int tc = 0; tc < 4; tc++)
                        {
                            count = count + 15.0f;
                            float ori = 0.0f;
                            for (int i = 0; i < maxTornado; i++)
                                if (Creature* tornado = me->SummonCreature(NPC_FIERY_TORNADO, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN))
                                {
                                    float x, y, z;
                                    me->GetNearPoint(me, x, y, z, 0.0f, count, ori);
                                    tornado->GetMotionMaster()->MovePoint(POINT_TORNADO, x, y, z);
                                    tornado->AI()->SetData(0, dir);
                                    ori += 2 * M_PI / maxTornado;
                                }
                            dir = !dir;
                            maxTornado += 1;
                        }
                        break;
                    }
                }
            }
        }

        private:
            EventMap events;
    };
};

enum MajordomoEvent
{
    EVENT_START             = 1,
    EVENT_TALK_2            = 2,
    EVENT_START_KILLING     = 3,
    EVENT_TALK_3            = 4,
    EVENT_FLYING            = 5,
    EVENT_ALYS              = 6,
};

enum MajordomoText
{
    YELL_MAJORDOMO_1 = 0,
    YELL_MAJORDOMO_2 = 1,
    YELL_MAJORDOMO_3 = 2,
};

class npc_majordomo_alys : public CreatureScript // 54015
{
public:
    npc_majordomo_alys() : CreatureScript("npc_majordomo_alys") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_majordomo_alysAI (creature);
    }

    struct npc_majordomo_alysAI : public ScriptedAI
    {
        npc_majordomo_alysAI(Creature* c) : ScriptedAI(c)
        {
            if (GameObject* volcano = c->FindNearestGameObject(GO_VOLCANO_ALYS, 1000.0f))
                volcano->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING);
        }

        void Reset() override
        {
            if (GameObject* volcano =  me->FindNearestGameObject(GO_VOLCANO_ALYS, 1000.0f))
                volcano->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING);

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
        }

        void EnterCombat(Unit* /*unit*/) override
        {
            SetCombatMovement(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
            events.ScheduleEvent(EVENT_START, 100);
            if (Player* player = me->FindNearestPlayer(50.0f))
                me->SetFacingToObject(player);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START:
                        TalkToMap(YELL_MAJORDOMO_1);
                        SetCombatMovement(false);
                        me->AttackStop();
                        me->SetReactState(REACT_PASSIVE);
                        me->InterruptNonMeleeSpells(false);
                        events.ScheduleEvent(EVENT_TALK_2, 8000);
                        break;
                    case EVENT_TALK_2:
                    {
                        TalkToMap(YELL_MAJORDOMO_2);
                        Map::PlayerList const& PlayerList = me->GetMap()->GetPlayers();
                        if (!PlayerList.isEmpty())
                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                                if (Player *player = i->getSource())
                                    player->CastSpell(player, SPELL_ROOT_EVENT, true);
                        events.ScheduleEvent(EVENT_START_KILLING, 7000);
                        break;
                    }
                    case EVENT_START_KILLING:
                        if (Creature* druid = me->FindNearestCreature(54019, 20.0f, true))
                            me->CastSpell(druid, 100557, true);

                        if (!me->FindNearestCreature(54019, 20.0f, true))
                            events.ScheduleEvent(EVENT_TALK_3, 4000);
                        else
                            events.ScheduleEvent(EVENT_START_KILLING, 1500);
                        break;

                    case EVENT_TALK_3:
                        TalkToMap(YELL_MAJORDOMO_3);
                        me->CastSpell(me, SPELL_REMOVE_ROOT_EVENT, true);
                        me->CastSpell(me, 100582, true);

                        events.ScheduleEvent(EVENT_FLYING, 2000);
                        break;

                    case EVENT_FLYING:
                        me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                        me->SetCanFly(true);
                        me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, 0x02);
                        me->CastSpell(me, 100565, true);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->GetMotionMaster()->MovePoint(0, 59.63651f, -361.6911f, 137.7812f, 0);

                        events.ScheduleEvent(EVENT_ALYS, 7000);
                        break;

                    case EVENT_ALYS:
                        if (GameObject* volcano =  me->FindNearestGameObject(GO_VOLCANO_ALYS, 1000.0f))
                            volcano->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);

                        me->SummonCreature(BOSS_ALYSRAZOR, SpawnPos[0], TEMPSUMMON_MANUAL_DESPAWN);
                        me->DespawnOrUnsummon(5000);
                        break;
                }
            }
        }

    private:
        EventMap events;
    };
};

class npc_alysrazor_fusion_meteor : public CreatureScript
{
public:
    npc_alysrazor_fusion_meteor() : CreatureScript("npc_alysrazor_fusion_meteor") { }

    struct npc_alysrazor_fusion_meteorAI : public NullCreatureAI
    {
        npc_alysrazor_fusion_meteorAI(Creature* creature) : NullCreatureAI(creature), instance(me->GetInstanceScript()) { }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            if (Creature* alysrazor = Unit::GetCreature(*me, instance->GetData64(DATA_ALYSRAZOR)))
                alysrazor->AI()->JustSummoned(me);
            me->GetMotionMaster()->MovePoint(POINT_START_TOP, wallPos[rand() % 8]);
            rockCount = 0;
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->CastSpell(me, SPELL_FUSION_METEOR_DEATH, true);
        }

        void JustSummoned(Creature* summon) override
        {
            if (Creature* alysrazor = Unit::GetCreature(*me, instance->GetData64(DATA_ALYSRAZOR)))
                alysrazor->AI()->JustSummoned(summon);
            if (summon->GetEntry() == NPC_FUSION_ROCK)
            {
                float orientation = 3 * M_PI / 8 + rockCount * M_PI / 8;
                Position pos;
                if (Creature *centerTrigger = me->FindNearestCreature(53723, 100.0f))
                    me->SetFacingToObject(centerTrigger);
                me->GetNearPosition(pos, 100.0f, M_PI / 2.0f + orientation);
                summon->GetMotionMaster()->MovePoint(0, pos);
                rockCount++;
            }
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type != POINT_MOTION_TYPE || pointId != POINT_START_TOP)
                return;

            Position pos;
            me->GetPosition(&pos);
            for (uint8 cnt = 0; cnt < 3; cnt++)
                me->SummonCreature(NPC_FUSION_ROCK, pos, TEMPSUMMON_TIMED_DESPAWN, 10000);
            me->DespawnOrUnsummon(1);
        }

    private:
        uint8 rockCount;
        InstanceScript *instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_alysrazor_fusion_meteorAI(creature);
    }
};

class spell_alysrazor_aggro_closest : public SpellScriptLoader
{
public:
    spell_alysrazor_aggro_closest() : SpellScriptLoader("spell_alysrazor_aggro_closest") { }

    class spell_alysrazor_aggro_closest_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_aggro_closest_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_UNIT;
        }

        void HandleEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            float curThreat = GetCaster()->getThreatManager().getThreat(GetHitUnit(), true);
            GetCaster()->getThreatManager().addThreat(GetHitUnit(), -curThreat + 50000.0f / std::min(1.0f, GetCaster()->GetDistance(GetHitUnit())));
        }

        void UpdateThreat()
        {
            GetCaster()->ClearUnitState(UNIT_STATE_CASTING);
            GetCaster()->GetAI()->AttackStart(GetCaster()->ToCreature()->SelectVictim());
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_aggro_closest_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
            AfterCast += SpellCastFn(spell_alysrazor_aggro_closest_SpellScript::UpdateThreat);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_aggro_closest_SpellScript();
    }
};

class spell_alysrazor_fieroblast : public SpellScriptLoader
{
public:
    spell_alysrazor_fieroblast() : SpellScriptLoader("spell_alysrazor_fieroblast") { }

    class spell_alysrazor_fieroblast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_fieroblast_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_FIRE_IT_UP))
                return false;
            return true;
        }

        void FireItUp()
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_FIRE_IT_UP, TRIGGERED_FULL_MASK);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_alysrazor_fieroblast_SpellScript::FireItUp);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_fieroblast_SpellScript();
    }
};


class spell_molthen_feater : public SpellScriptLoader
{
    public:
        spell_molthen_feater() : SpellScriptLoader("spell_molthen_feater") { }

        class spell_molthen_feater_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_molthen_feater_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                target->SetPower(POWER_ALTERNATE_POWER, 0);

                for (uint8 i = 0; i < 10; i++)
                    target->RemoveAura(98761 + i);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_molthen_feater_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_molthen_feater_AuraScript();
        }
};

class spell_fieroblast : public SpellScriptLoader
{
    public:
        spell_fieroblast() : SpellScriptLoader("spell_fieroblast") { }

        class spell_fieroblast_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_fieroblast_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_FIREITUP, caster);
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_fieroblast_AuraScript::OnApply, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_fieroblast_AuraScript();
        }
};

class spell_hatchling_debuff : public SpellScriptLoader
{
    public:
        spell_hatchling_debuff() : SpellScriptLoader("spell_hatchling_debuff") { }

        class spell_hatchling_debuff_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hatchling_debuff_AuraScript);

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* target = GetUnitOwner())
                    if (target->HealthBelowPct(50))
                        Remove(AURA_REMOVE_BY_DEFAULT);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hatchling_debuff_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_hatchling_debuff_AuraScript();
        }
};

class spell_alysrazor_wings_of_flames : public SpellScriptLoader
{
public:
    spell_alysrazor_wings_of_flames() : SpellScriptLoader("spell_alysrazor_wings_of_flames") {}

    class spell_alysrazor_wings_of_flames_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_wings_of_flames_SpellScript);

        void HandleForceCast(SpellEffIndex eff)
        {
            PreventHitDefaultEffect(eff);
            GetCaster()->CastSpell(GetCaster(), GetSpellInfo()->Effects[eff].TriggerSpell, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_wings_of_flames_SpellScript::HandleForceCast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_wings_of_flames_SpellScript();
    }
};

// 99432
class spell_alyzrasor_ardeur : public SpellScriptLoader
{
public:
    spell_alyzrasor_ardeur() : SpellScriptLoader("spell_alyzrasor_ardeur") { }

    class spell_alyzrasor_ardeur_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_alyzrasor_ardeur_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetSpellInfo())
                if (Unit* target = eventInfo.GetDamageInfo()->GetAttacker())
                    if (Unit* alysrazor = GetCaster())
                        alysrazor->CastSpell(target, SPELL_ESSENCE_OF_THE_GREEN, true, NULL, aurEff);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_alyzrasor_ardeur_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_alyzrasor_ardeur_AuraScript();
    }
};

class DistanceFilter
{
public:
    explicit DistanceFilter(Unit* caster) : _caster(caster) { }

    bool operator()(WorldObject* unit) const
    {
        if (Unit* target = unit->ToUnit())
            if (target->GetTypeId() == TYPEID_PLAYER)
                if (target->GetExactDist2d(_caster) <= 8.0f)
                    return false;
        return true;
    }

private:
    Unit* _caster;
};

class spell_alysrazor_blazing_power : public SpellScriptLoader
{
public:
    spell_alysrazor_blazing_power() : SpellScriptLoader("spell_alysrazor_blazing_power") { }

    class spell_alysrazor_blazing_power_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_blazing_power_SpellScript);

        std::list<WorldObject*> sharedTargets;

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(DistanceFilter(GetCaster()));
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_BLAZING_POWER_PREVENTION_AURA));
            sharedTargets = targets;
        }

        void FilterTargetsSubsequent(std::list<WorldObject*>& targets)
        {
            targets = sharedTargets;
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alysrazor_blazing_power_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alysrazor_blazing_power_SpellScript::FilterTargetsSubsequent, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_blazing_power_SpellScript();
    }
};

class spell_alysrazor_blazing_power_prevention : public SpellScriptLoader
{
public:
    spell_alysrazor_blazing_power_prevention() : SpellScriptLoader("spell_alysrazor_blazing_power_prevention") { }

    class spell_alysrazor_blazing_power_prevention_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_blazing_power_prevention_SpellScript);

        std::list<WorldObject*> sharedTargets;

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove(GetCaster());
            targets.remove_if(DistanceFilter(GetCaster()));
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_BLAZING_POWER_PREVENTION_AURA));
            sharedTargets = targets;
        }

        void FilterTargetsSubsequent(std::list<WorldObject*>& targets)
        {
            targets = sharedTargets;
        }

        void HandleForceCast(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* target = GetHitUnit())
                target->CastSpell(target, GetSpellInfo()->Effects[effIndex].TriggerSpell, true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alysrazor_blazing_power_prevention_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alysrazor_blazing_power_prevention_SpellScript::FilterTargetsSubsequent, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alysrazor_blazing_power_prevention_SpellScript::FilterTargetsSubsequent, EFFECT_2, TARGET_UNIT_SRC_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_blazing_power_prevention_SpellScript::HandleForceCast, EFFECT_1, SPELL_EFFECT_FORCE_CAST);
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_blazing_power_prevention_SpellScript::HandleForceCast, EFFECT_2, SPELL_EFFECT_FORCE_CAST);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_blazing_power_prevention_SpellScript();
    }
};

class spell_alysrazor_alysras_razor : public SpellScriptLoader
{
public:
    spell_alysrazor_alysras_razor() : SpellScriptLoader("spell_alysrazor_alysras_razor") { }

    class spell_alysrazor_alysras_razor_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_alysras_razor_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (Unit* caster = GetCaster())
            {
                if (Aura* flying = caster->GetAura(SPELL_FLY))
                    flying->RefreshDuration();

                if (Aura* feather = caster->GetAura(SPELL_PASSIVE_BUF_FROM_FEATHER))
                    if (feather->GetStackAmount() >= 25)
                        caster->CastSpell(caster, SPELL_ALYSRAS_RAZOR, true);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_alysras_razor_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_alysras_razor_SpellScript();
    }
};

class spell_alysrazor_imprinted : public SpellScriptLoader
{
public:
    spell_alysrazor_imprinted() : SpellScriptLoader("spell_alysrazor_imprinted") { }

    class spell_alysrazor_imprinted_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_imprinted_SpellScript);

        void HandleExtraEffect(SpellEffIndex effIndex)
        {
            if (Creature* caster = GetCaster()->ToCreature())
            {
                if (Unit* target = GetHitUnit())
                {
                    caster->AI()->AttackStart(target);
                    caster->AI()->SetGUID(target->GetGUID());
                    target->CastSpell(caster, caster->GetEntry() == NPC_VORACIOUS_HATCHLING ? SPELL_IMPRINTED_TAUNT : SPELL_IMPRINTED_TAUNT_2, true);
                    caster->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                    caster->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_imprinted_SpellScript::HandleExtraEffect, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_imprinted_SpellScript();
    }
};

class spell_alysrazor_imprinted_targeting : public SpellScriptLoader
{
public:
    spell_alysrazor_imprinted_targeting() : SpellScriptLoader("spell_alysrazor_imprinted_targeting") { }

    class spell_alysrazor_imprinted_targeting_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_imprinted_targeting_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_IMPRINTED));
            targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_IMPRINTED_2));
            targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
            targets.resize(1);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alysrazor_imprinted_targeting_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_NEARBY_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_imprinted_targeting_SpellScript();
    }
};

class ExtraLosFilter
{
public:
    explicit ExtraLosFilter(Unit* caster, std::list<GameObject*> const& collisionList) : _caster(caster), _collisionList(collisionList) { }

    bool operator()(WorldObject* unit) const
    {
        for (std::list<GameObject*>::const_iterator itr = _collisionList.begin(); itr != _collisionList.end(); ++itr)
            if ((*itr)->IsInBetween(_caster, unit, 5.0f))
                return true;
        return false;
    }

private:
    Unit* _caster;
    std::list<GameObject* > _collisionList;
};

class spell_alysrazor_firestorm_damage : public SpellScriptLoader
{
public:
    spell_alysrazor_firestorm_damage() : SpellScriptLoader("spell_alysrazor_firestorm_damage") { }

    class spell_alysrazor_firestorm_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_firestorm_damage_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<GameObject*> meteorCollision;
            GetGameObjectListWithEntryInGrid(meteorCollision, GetCaster(), GO_METEOR_COLLISION, 200.0f);
            targets.remove_if(ExtraLosFilter(GetCaster(), meteorCollision));
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alysrazor_firestorm_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_firestorm_damage_SpellScript();
    }
};


// =================     blazing monstruosity event =====================

class npc_harbinger_of_flame : public CreatureScript
{
public:
    npc_harbinger_of_flame() : CreatureScript("npc_harbinger_of_flame") { }

    struct npc_harbinger_of_flameAI : public ScriptedAI
    {
        npc_harbinger_of_flameAI(Creature* creature) : ScriptedAI(creature) { }

        void EnterCombat(Unit* /*target*/) override
        {
            if (Creature* bird = ObjectAccessor::GetCreature(*me, me->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT)))
                DoZoneInCombat(bird, 200.0f);
            me->InterruptSpell(CURRENT_CHANNELED_SPELL);
            _events.Reset();
            _events.ScheduleEvent(EVENT_FIEROBLAST, 1);
            _events.ScheduleEvent(EVENT_FIEROCLAST_BARRAGE, 6000);
        }

        void JustReachedHome() override
        {
            AlysrazorTrashEvaded(me);
        }

        void MoveInLineOfSight(Unit* unit) override
        {
            if (me->isInCombat())
                return;

            if (!unit->isCharmedOwnedByPlayerOrPlayer())
                return;

            ScriptedAI::MoveInLineOfSight(unit);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!me->isInCombat())
                if (!me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                    if (Creature* fireBird = me->FindNearestCreature((me->GetHomePosition().GetPositionY() > -275.0f ? NPC_BLAZING_MONSTROSITY_LEFT : NPC_BLAZING_MONSTROSITY_RIGHT), 100.0f))
                        DoCast(fireBird, SPELL_FIRE_CHANNELING);

            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FIEROBLAST:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, false, -SPELL_RIDE_MONSTROSITY))
                            DoCast(target, SPELL_FIEROBLAST_TRASH);
                        _events.RescheduleEvent(EVENT_FIEROBLAST, 500);
                        break;
                    case EVENT_FIEROCLAST_BARRAGE:
                        DoCastAOE(SPELL_FIEROCLAST_BARRAGE);
                        _events.ScheduleEvent(EVENT_FIEROCLAST_BARRAGE, urand(9000, 12000));
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_harbinger_of_flameAI(creature);
    }
};

class npc_blazing_monstrosity : public CreatureScript
{
public:
    npc_blazing_monstrosity() : CreatureScript("npc_blazing_monstrosity") { }

    struct npc_blazing_monstrosityAI : public PassiveAI
    {
        npc_blazing_monstrosityAI(Creature* creature) : PassiveAI(creature), _summons(creature) { }

        void EnterEvadeMode() override
        {
            _summons.DespawnAll();
            _events.Reset();
            PassiveAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/) override
        {
            _summons.DespawnAll();
            _events.Reset();
            me->DespawnOrUnsummon(10000);
        }

        void JustReachedHome() override
        {
            AlysrazorTrashEvaded(me);
        }

        void EnterCombat(Unit* /*target*/) override
        {
            DoZoneInCombat();
            me->RemoveAurasDueToSpell(SPELL_SLEEP_ULTRA_HIGH_PRIORITY);
            me->PlayOneShotAnimKit(ANIM_KIT_BIRD_WAKE);
            _events.Reset();
            _events.ScheduleEvent(EVENT_START_SPITTING, 6000);
            _events.ScheduleEvent(EVENT_CONTINUE_SPITTING, 9000);
        }

        void PassengerBoarded(Unit* passenger, int8 /*seat*/, bool apply) override
        {
            if (!apply)
                return;

            // Our passenger is another vehicle (boardable by players)
            DoCast(passenger, SPELL_SHARE_HEALTH, true);
            passenger->setFaction(35);
            passenger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            Movement::MoveSplineInit init(passenger);
            init.DisableTransportPathTransformations();
            init.MoveTo(0.6654003f, 0.0f, 1.9815f);
            init.SetFacing(0.0f);
            init.Launch();
        }

        void JustSummoned(Creature* summon) override
        {
            _summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon) override
        {
            _summons.Despawn(summon);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_SPITTING:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, false, -SPELL_RIDE_MONSTROSITY))
                            DoCast(target, SPELL_MOLTEN_BARRAGE);
                        break;
                    case EVENT_CONTINUE_SPITTING:
                        DoCastAOE(SPELL_MOLTEN_BARRAGE_EFFECT);
                        if (Creature* egg = me->FindNearestCreature(NPC_EGG_PILE, 100.0f))
                            egg->AI()->DoAction(me->GetEntry());
                        break;
                }
            }
        }

    private:
        SummonList _summons;
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_blazing_monstrosityAI(creature);
    }
};

class npc_molten_barrage : public CreatureScript
{
public:
    npc_molten_barrage() : CreatureScript("npc_molten_barrage") { }

    struct npc_molten_barrageAI : public NullCreatureAI
    {
        npc_molten_barrageAI(Creature* creature) : NullCreatureAI(creature) { }

        void AttackStart(Unit* target) override
        {
            if (target)
                me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f, MOTION_SLOT_IDLE);
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            DoCastAOE(SPELL_AGGRO_CLOSEST, true);
            DoCast(me, SPELL_MOLTEN_BARRAGE_VISUAL, true);
            DoCast(me, SPELL_INVISIBILITY_AND_STEALTH_DETECTION, true);
        }

        void MovementInform(uint32 movementType, uint32 /*pointId*/) override
        {
            if (movementType != EFFECT_MOTION_TYPE)
                return;

            DoCastAOE(SPELL_AGGRO_CLOSEST);
            me->ClearUnitState(UNIT_STATE_CANNOT_TURN);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_molten_barrageAI(creature);
    }
};

class npc_egg_pile : public CreatureScript
{
public:
    npc_egg_pile() : CreatureScript("npc_egg_pile") { }

    struct npc_egg_pileAI : public CreatureAI
    {
        npc_egg_pileAI(Creature* creature) : CreatureAI(creature) { }

        void AttackStart(Unit* /*target*/) override { }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            _events.Reset();
            _callHatchlingSpell = 0;
        }

        void JustDied(Unit* /*killer*/) override
        {
            _events.Reset();
            std::list<Creature*> eggs;
            GetCreatureListWithEntryInGrid(eggs, me, NPC_MOLTEN_EGG_TRASH, 20.0f);
            for (std::list<Creature*>::const_iterator itr = eggs.begin(); itr != eggs.end(); ++itr)
                (*itr)->CastSpell(*itr, SPELL_ALYSRAZOR_COSMETIC_EGG_XPLOSION, TRIGGERED_FULL_MASK);

            DoCast(me, SPELL_ALYSRAZOR_COSMETIC_EGG_XPLOSION, true);
        }

        void JustReachedHome() override
        {
            AlysrazorTrashEvaded(me);
        }

        void JustSummoned(Creature* summon) override
        {
            summon->SetInCombatWithZone();
        }

        void DoAction(const int32 action) override
        {
            if (action != NPC_BLAZING_MONSTROSITY_LEFT && action != NPC_BLAZING_MONSTROSITY_RIGHT)
                return;

            if (action == NPC_BLAZING_MONSTROSITY_LEFT)
                TalkToMap(EMOTE_CRACKING_EGGS);

            _callHatchlingSpell = (action == NPC_BLAZING_MONSTROSITY_LEFT) ? SPELL_MOLTEN_EGG_TRASH_CALL_L : SPELL_MOLTEN_EGG_TRASH_CALL_R;
            DoZoneInCombat();
            _events.Reset();
            _events.ScheduleEvent(EVENT_SUMMON_SMOULDERING_HATCHLING, 1);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_SMOULDERING_HATCHLING:
                    {
                        std::list<Creature*> eggs;
                        MoltenEggCheck check(me);
                        Trinity::CreatureListSearcher<MoltenEggCheck> searcher(me, eggs, check);
                        me->VisitNearbyGridObject(20.0f, searcher);
                        if (!eggs.empty())
                        {
                            Creature* egg = Trinity::Containers::SelectRandomContainerElement(eggs);
                            egg->CastSpell(egg, SPELL_SUMMON_SMOULDERING_HATCHLING, TRIGGERED_FULL_MASK);
                            egg->SetDisplayId(MODEL_INVISIBLE_STALKER);
                            egg->m_Events.AddEvent(new RespawnEggEvent(egg), egg->m_Events.CalculateTime(5000));
                        }

                        if (_callHatchlingSpell)
                            DoCastAOE(_callHatchlingSpell, true);
                        _events.ScheduleEvent(EVENT_SUMMON_SMOULDERING_HATCHLING, urand(6000, 10000));
                        break;
                    }
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        uint32 _callHatchlingSpell;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_egg_pileAI(creature);
    }
};

class spell_alysrazor_cosmetic_egg_xplosion : public SpellScriptLoader
{
public:
    spell_alysrazor_cosmetic_egg_xplosion() : SpellScriptLoader("spell_alysrazor_cosmetic_egg_xplosion") { }

    class spell_alysrazor_cosmetic_egg_xplosion_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_cosmetic_egg_xplosion_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sCreatureDisplayInfoStore.LookupEntry(MODEL_INVISIBLE_STALKER))
                return false;
            return true;
        }

        void HandleExplosion(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            GetHitUnit()->SetDisplayId(MODEL_INVISIBLE_STALKER);
            if (Creature* creature = GetHitCreature())
                creature->DespawnOrUnsummon(4000);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_cosmetic_egg_xplosion_SpellScript::HandleExplosion, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_cosmetic_egg_xplosion_SpellScript();
    }
};

class spell_alysrazor_turn_monstrosity : public SpellScriptLoader
{
public:
    spell_alysrazor_turn_monstrosity() : SpellScriptLoader("spell_alysrazor_turn_monstrosity") { }

    class spell_alysrazor_turn_monstrosity_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alysrazor_turn_monstrosity_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_GENERIC_DUMMY_CAST))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_KNOCKBACK_RIGHT))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_KNOCKBACK_LEFT))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_KNOCKBACK_FORWARD))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_KNOCKBACK_BACK))
                return false;
            return true;
        }

        void KnockBarrage(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            GetHitUnit()->GetMotionMaster()->MoveIdle();
            if (TempSummon* summ = GetHitUnit()->ToTempSummon())
                if (Unit* summoner = summ->GetSummoner())
                    GetHitUnit()->CastSpell(summoner, SPELL_GENERIC_DUMMY_CAST, TRIGGERED_FULL_MASK);

            float angle = 0.0f;

            if (Unit* bird = GetCaster()->GetVehicleBase())
            {
                bird->SetInFront(GetHitUnit());
                angle = bird->GetOrientation();
            }

            uint32 spellId = 0;
            switch (GetSpellInfo()->Id)
            {
                case SPELL_RIGHT_SIDE_SMACK_R:
                case SPELL_RIGHT_SIDE_SMACK_L:
                    spellId = SPELL_KNOCKBACK_RIGHT;
                    angle -= M_PI * 0.5f;
                    break;
                case SPELL_LEFT_SIDE_SMACK_R:
                case SPELL_LEFT_SIDE_SMACK_L:
                    spellId = SPELL_KNOCKBACK_LEFT;
                    angle += M_PI * 0.5f;
                    break;
                case SPELL_HEAD_BONK_R:
                case SPELL_HEAD_BONK_L:
                    spellId = SPELL_KNOCKBACK_FORWARD;
                    break;
                case SPELL_TICKLE_R:
                case SPELL_TICKLE_L:
                    spellId = SPELL_KNOCKBACK_BACK;
                    angle -= M_PI;
                    break;
            }

            // Cannot wait for object update to process facing spline, it's needed in next spell cast
            GetHitUnit()->SetOrientation(angle);
            GetHitUnit()->SetFacingTo(angle);
            GetHitUnit()->AddUnitState(UNIT_STATE_CANNOT_TURN);
            GetHitUnit()->CastSpell(GetHitUnit(), spellId, TRIGGERED_FULL_MASK);
        }

        void TurnBird(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            GetHitUnit()->PlayOneShotAnimKit(ANIM_KIT_BIRD_TURN);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_turn_monstrosity_SpellScript::KnockBarrage, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            OnEffectHitTarget += SpellEffectFn(spell_alysrazor_turn_monstrosity_SpellScript::TurnBird, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_alysrazor_turn_monstrosity_SpellScript();
    }
};


void AddSC_boss_alysrazor()
{
    new boss_alysrazor();
    new mob_molten_feather();
    new npc_alysrazor_volcano();
    new npc_fiery_vortex();
    new npc_plumb_lava_worm();
    new npc_blazing_broodmother();
    new npc_molten_egg();
    new npc_voracious_hatchling();
    new npc_voracious_hatchling_two();
    new npc_blazing_talon_clawshaper();
    new npc_blazing_talon();
    new npc_brushfire();
    new npc_fiery_tornado();
    new npc_majordomo_alys();
    new npc_alysrazor_fusion_meteor();
    new spell_alysrazor_aggro_closest();
    new spell_alysrazor_fieroblast();
    new spell_molthen_feater();
    new spell_hatchling_debuff();
    new spell_fieroblast();
    new spell_alysrazor_wings_of_flames();
    new spell_alyzrasor_ardeur();
    new spell_alysrazor_blazing_power();
    new spell_alysrazor_blazing_power_prevention();
    new spell_alysrazor_alysras_razor();
    new spell_alysrazor_imprinted();
    new spell_alysrazor_imprinted_targeting();
    new spell_alysrazor_firestorm_damage();

    // blazing monstruosity event
    new npc_harbinger_of_flame();
    new npc_blazing_monstrosity();
    new npc_molten_barrage();
    new npc_egg_pile();
    new spell_alysrazor_cosmetic_egg_xplosion();
    new spell_alysrazor_turn_monstrosity();
}
