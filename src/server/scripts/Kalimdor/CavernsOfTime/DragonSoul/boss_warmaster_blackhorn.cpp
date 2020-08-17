#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "PassiveAI.h"
#include "SpellScript.h"
#include "MoveSplineInit.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "dragonsoul.h"

enum Talks
{
    // Sky captain swayze
    TALK_INTRO                  = -55430, // 17:01:06
    TALK_INTRO_2                = -56079, // 17:01:19
    TALK_INTRO_3                = -55431, // 17:01:33
    TALK_TWILIGHT_SAPPER        = -55434,
    TALK_SKYIRE_LOW_HEALTH      = -55435,
    TALK_SKYFIRE_DESTROYED      = -55436,
    TALK_PHASE_2                = -55433,
    EMOTE_DECK_FIRE             = -57030,
    EMOTE_SKYFIRE_DESTROYED     = -57107,

    // Warmaster Blackhorn
    TALK_INTRO_BLACKHORN        = -55442, // 17:01:13
    TALK_INTRO_2_BLACKHORN      = -55443, // 17:01:26
    TALK_PHASE_2_BLACKHORN      = -55456,
    EMOTE_DISRUPTING_ROAR       = -48966,
    TALK_SHOCKWAVE              = -55454, // grouping
    TALK_TWILIGHT_ONSLAUGHT     = -55453,
    TALK_BERSERK                = -55457,
    TALK_KILLING_A_PLAYER       = -55446, // grouping
    TALK_DEATH                  = -55444,
    EMOTE_SIPHON_VITALITY       = -57062,

    // Goriona
    EMOTE_BROADSIDE             = -57050,
    EMOTE_TWILIGHT_ONSLAUGHT    = -56596,
    EMOTE_GORIONA_ESCAPE        = -56156
};

enum Spells
{
    // The Skyfire
    SPELL_DECK_FIRE_INITIAL_SPAWN   = 109470,
    SPELL_DECK_FIRE                 = 109445,
    SPELL_DECK_FIRE_PERIODIC        = 110092,
    SPELL_DECK_FIRE_DAMAGE          = 110095,
    SPELL_MASSIVE_EXPLOSION         = 108132,

    // Skyfire Fire Brigade
    SPELL_ROCKET_PACK               = 110054,
    SPELL_WATER_JET                 = 110061,

    // Fire Stalker
    SPELL_SHIP_FIRE                 = 109245,

    // Sky captain swayze
    SPELL_TELEPORT_ALL_TO_GUNSHIP   = 106051, // X: 13456.6 Y: -12133.8 Z: 151.168
    SPELL_SPINE_OF_DEATHWING_MOVIE  = 106085,
    SPELL_PARATROOPER_KIT           = 104953,

    // All drakes
    SPELL_RIDE_VEHICLE_HARDCODED    = 46598,

    // Goriona
    SPELL_TWILIGHT_BLAST            = 107583, // hit 57190
    SPELL_LOSING_SPEED              = 107513,
    SPELL_TWILIGHT_ONSLAUGHT        = 107588,
    SPELL_TWILIGHT_ONSLAUGHT_VISUAL = 107927,
    SPELL_BROADSIDE_VISUAL          = 110153,
    SPELL_BROADSIDE_DAMAGE          = 110157,

    // Goriona Phase two
    SPELL_TWILIGHT_FLAMES           = 108050,
    SPELL_TWILIGHT_FLAMES_PERIODIC  = 108053,
    SPELL_TWILIGHT_BREATH           = 110212,
    SPELL_CONSUMING_SHROUD          = 110214,
    SPELL_CONSUMING_SHROUD_DAMAGE   = 110215,

    // Twilight Assault Drake
    SPELL_TWILIGHT_BARRAGE          = 107286,
    SPELL_TWILIGHT_BARRAGE_DAMAGE   = 107439,
    SPELL_EJECT_PASSENGER_1         = 60603,

    // Twilight Elite Dreadblade
    SPELL_BLADE_RUSH                = 107594,
    SPELL_DEGENERATION              = 107558,

    // Twilight Elite Slayer
    SPELL_BRUTAL_STRIKE             = 107567,

    // Twilight Sapper
    SPELL_DETONATE                  = 107518,
    SPELL_SMOKE_BOMB                = 107752,
    SPELL_CLEAR_ALL_DEBUFFS         = 34098,

    // Skyfire Harpoon Gun
    SPELL_HARPOON                   = 108038,   // hit 56587

    // Skyfire Cannon
    SPELL_HEAVY_SLUG                = 108010,   // hit 56587
    SPELL_ARTILLERY_BARRAGE         = 108040,

    // Engine Stalker
    SPELL_ENGINE_FIRE               = 107799,

    // Warmaster Blackhorn
    SPELL_DEVASTATE                 = 108042,
    SPELL_DISRUPTING_ROAR           = 108044,
    SPELL_SHOCKWAVE                 = 108046,
    SPELL_VENGEANCE                 = 108045,
    SPELL_SIPHON_VITALITY           = 110312,
    SPELL_SIPHON_VITALITY_DND       = 110315,
    SPELL_BERSERK                   = 110055
};

enum Events
{
    // Sky captain swayze
    EVENT_SUMMON_ASSAULTER          = 1,
    EVENT_SUMMON_SAPPER             = 2,
    EVENT_PLAYER_ALIVE_CHECK        = 3,
    EVENT_INTRO_TALK                = 4,
    EVENT_INTRO_TALK_2              = 5,
    EVENT_MASSIVE_EXPLOSION         = 6,
    EVENT_DECK_FIRE_CHECK           = 7,
    EVENT_CHECK_BRIGADE_TARGET      = 8,

    // Warmaster Blackhorn
    EVENT_TALK_INTRO_BLACKHORN      = 1,
    EVENT_TALK_INTRO_BLACKHORN_2    = 2,
    EVENT_DEVASTATE                 = 3,
    EVENT_DISRUPTING_ROAR           = 4,
    EVENT_SHOCKWAVE                 = 5,
    EVENT_SIPHON_VITALITY           = 6,
    EVENT_BERSERK                   = 7,

    // Twilight assaulter
    EVENT_MOVE_TO_END_POSITION      = 1,
    EVENT_TWILIGHT_BARRAGE          = 2,
    EVENT_CORRECT_ORIENTATION       = 3,
    EVENT_ACTIVATE_HARPOON          = 4,
    EVENT_DEACTIVATE_HARPOON        = 5,

    // Goriona
    EVENT_TWILIGHT_ONSLAUGHT        = 1,
    EVENT_MOVE_ON_THE_OTHER_SIDE    = 2,
    EVENT_TWILIGHT_FLAMES           = 3,
    EVENT_CORRECT_ORIENTATION_2     = 4,
    EVENT_BROADSIDE                 = 5,
    EVENT_CONSUMING_SHROUD          = 6,
    EVENT_TWILIGHT_BREATH           = 7,
    EVENT_LAND                      = 8,
    EVENT_COMBAT_START              = 9,

    // Skyfire cannon
    EVENT_HEAVY_SLUG                = 1,
    EVENT_SET_FACING_TO             = 2,

    // Twilight Elite Dreadblade & Slayer
    EVENT_BLADE_RUSH                = 1,
    EVENT_DEGENERATION              = 2,
    EVENT_BRUTAL_STRIKE             = 3,

    // Twilight sapper
    EVENT_SMOKE_BOMB                = 1,
    EVENT_MOVE_TO_MACHINERY         = 2,

    // Skyfire Fire Brigade
    EVENT_MOVE_TO_SHIP              = 1,
    EVENT_SEARCH_FIRE               = 2,
    EVENT_FIRE_EXTINGUISHED         = 3,
};

enum Points
{

    POINT_END_POSITION          = 1,
    POINT_MACHINERY             = 2,
    POINT_SKYFIRE_DECK          = 3,
    POINT_DESPAWN               = 4,
    POINT_LANDING               = 5,

    // Skyfire Fire Brigade
    POINT_ASC                   = 6,
    POINT_SHIP                  = 7,
    POINT_FIRE                  = 8,
};

enum Actions
{
    ACTION_START_COMBAT         = 1,
    ACTION_START_PHASE_2        = 2,
    ACTION_ACTIVATE_CANNONS     = 3,
    ACTION_DEACTIVATE_CANNONS   = 4,
    ACTION_FIGHT_FINISHED       = 5,
    ACTION_FIND_DYNOBJ_FIRE     = 6,
    ACTION_DRAGON_DESPAWN       = 7,
    ACTION_DELAY_DECK_FIRE      = 8,
};

enum Phases
{
    PHASE_NONE  = 0,
    PHASE_INTRO = 1,
    PHASE_TWO   = 2,
    PHASE_SHIP  = 3
};

enum Data32
{
    DATA_FIRE_ATTEMPS   = 1,
};

Position const infiltratorPosition      = { 13622.690f, -12114.150f, 168.946f, 3.3286f };
Position const gorionaSpawnPos          = { 13618.170f, -12102.950f, 168.420f, 3.3286f };
Position const deckFireSpawnPos         = { 13461.200f, -12134.100f, 151.111f, 0.00f   };
Position const skyfireBrigadeSpawnPos   = { 13436.180f, -12089.330f, 115.225f, 0.00f   };

static const DragonSoulEventSpawns twilightAssaulterPos[2] =
{
    { NPC_TWILIGHT_ASSAULT_DRAKE, 13583.830f, -12082.740f, 174.507f, 3.3286f },
    { NPC_TWILIGHT_ASSAULT_DRAKE_2, 13605.120f, -12167.980f, 158.413f, 3.1244f }
};

Position const twilightBarragePos[57] =
{
    { 13416.0f, -12142.5f, 150.839f, 0.00f },
    { 13419.8f, -12137.2f, 151.134f, 0.00f },
    { 13436.0f, -12136.6f, 150.913f, 0.00f },
    { 13442.8f, -12131.2f, 150.838f, 0.00f },
    { 13449.5f, -12121.4f, 150.828f, 0.00f },
    { 13453.0f, -12114.9f, 150.886f, 0.00f },
    { 13462.4f, -12127.3f, 150.928f, 0.00f },
    { 13465.0f, -12136.8f, 151.176f, 0.00f },
    { 13466.9f, -12145.0f, 150.830f, 0.00f },
    { 13465.0f, -12146.4f, 151.181f, 0.00f },
    { 13455.1f, -12148.1f, 151.189f, 0.00f },
    { 13427.4f, -12120.7f, 150.834f, 0.00f },
    { 13431.8f, -12146.7f, 150.891f, 0.00f },
    { 13443.1f, -12145.3f, 150.807f, 0.00f },
    { 13410.7f, -12146.0f, 150.834f, 0.00f },
    { 13419.1f, -12149.5f, 151.189f, 0.00f },
    { 13418.4f, -12125.7f, 151.110f, 0.00f },
    { 13414.4f, -12115.6f, 150.860f, 0.00f },
    { 13427.7f, -12124.8f, 150.863f, 0.00f },
    { 13462.1f, -12153.4f, 151.195f, 0.00f },
    { 13445.8f, -12152.3f, 151.163f, 0.00f },
    { 13432.8f, -12141.2f, 151.197f, 0.00f },
    { 13442.8f, -12135.4f, 150.838f, 0.00f },
    { 13441.9f, -12125.0f, 150.815f, 0.00f },
    { 13457.1f, -12129.6f, 151.174f, 0.00f },
    { 13451.8f, -12134.2f, 150.928f, 0.00f },
    { 13456.5f, -12140.7f, 151.176f, 0.00f },
    { 13437.1f, -12115.6f, 150.759f, 0.00f },
    { 13447.0f, -12124.6f, 150.826f, 0.00f },
    { 13460.3f, -12121.0f, 150.850f, 0.00f },
    { 13429.5f, -12116.9f, 150.794f, 0.00f },
    { 13421.0f, -12127.9f, 150.889f, 0.00f },
    { 13426.0f, -12130.4f, 150.909f, 0.00f },
    { 13422.3f, -12123.0f, 150.866f, 0.00f },
    { 13414.1f, -12120.7f, 150.824f, 0.00f },
    { 13418.5f, -12117.4f, 150.987f, 0.00f },
    { 13423.2f, -12142.6f, 150.853f, 0.00f },
    { 13429.9f, -12140.8f, 150.854f, 0.00f },
    { 13430.2f, -12151.3f, 151.190f, 0.00f },
    { 13439.1f, -12149.4f, 150.776f, 0.00f },
    { 13424.5f, -12121.6f, 150.853f, 0.00f },
    { 13424.7f, -12117.0f, 150.822f, 0.00f },
    { 13431.7f, -12116.9f, 150.783f, 0.00f },
    { 13431.0f, -12123.4f, 150.845f, 0.00f },
    { 13424.1f, -12127.4f, 150.885f, 0.00f },
    { 13439.1f, -12149.4f, 150.776f, 0.00f },
    { 13451.9f, -12125.1f, 150.842f, 0.00f },
    { 13430.2f, -12131.5f, 150.918f, 0.00f },
    { 13423.6f, -12135.6f, 150.893f, 0.00f },
    { 13449.7f, -12139.0f, 150.840f, 0.00f },
    { 13442.7f, -12116.3f, 150.786f, 0.00f },
    { 13439.8f, -12141.3f, 150.814f, 0.00f },
    { 13438.6f, -12121.0f, 150.788f, 0.00f },
    { 13422.9f, -12147.2f, 150.825f, 0.00f },
    { 13451.8f, -12130.0f, 150.844f, 0.00f },
    { 13436.9f, -12127.1f, 150.815f, 0.00f },
    { 13423.7f, -12146.2f, 150.831f, 0.00f }
};

Position const massiveExplosionPos[6] =
{
    { 13427.83f, -12142.54f, 150.9250f, 0.00f },
    { 13422.49f, -12123.28f, 150.9463f, 0.00f },
    { 13490.29f, -12107.85f, 155.4958f, 0.00f },
    { 13491.31f, -12137.50f, 170.5503f, 0.00f },
    { 13454.64f, -12143.74f, 150.9223f, 0.00f },
    { 13438.56f, -12128.25f, 150.9052f, 0.00f }

};

Position const shipFireStalkerPos[12] =
{
    { 13488.26f, -12130.09f, 170.5464f, 0.00f },
    { 13366.94f, -12129.88f, 163.1623f, 0.00f },
    { 13493.64f, -12160.52f, 162.1842f, 0.00f },
    { 13494.39f, -12142.64f, 170.5455f, 0.00f },
    { 13459.93f, -12105.18f, 155.1799f, 0.00f },
    { 13487.15f, -12174.49f, 155.4944f, 0.00f },
    { 13395.83f, -12144.88f, 161.4096f, 0.00f },
    { 13496.22f, -12111.84f, 163.4314f, 0.00f },
    { 13492.84f, -12096.21f, 155.4947f, 0.00f },
    { 13471.40f, -12136.87f, 163.3683f, 0.00f },
    { 13503.00f, -12132.30f, 170.5578f, 0.00f },
    { 13494.36f, -12156.25f, 155.4958f, 0.00f }
};

uint32 const GorionaPathSize = 13;
const G3D::Vector3 GorionaPath[GorionaPathSize] =
{
    { 13622.46f, -12100.59f, 170.5641f },
    { 13621.46f, -12100.59f, 170.5641f },
    { 13614.30f, -12102.46f, 167.7289f },
    { 13584.09f, -12089.64f, 167.4736f },
    { 13527.58f, -12066.98f, 167.4736f },
    { 13485.61f, -12057.89f, 169.8847f },
    { 13454.18f, -12052.43f, 172.5224f },
    { 13422.94f, -12053.89f, 176.6686f },
    { 13371.22f, -12099.46f, 187.0653f },
    { 13356.59f, -12135.83f, 180.2729f },
    { 13360.49f, -12173.27f, 180.2729f },
    { 13391.76f, -12203.25f, 188.6195f },
    { 13391.76f, -12203.25f, 188.6195f }
};

uint32 const brigadePathSize = 7;
const G3D::Vector3 brigadePath[brigadePathSize] =
{
    { 13434.97f, -12088.83f, 111.5319f },
    { 13435.97f, -12088.83f, 111.5319f },
    { 13437.29f, -12093.58f, 145.0945f },
    { 13436.68f, -12097.96f, 160.1569f },
    { 13435.83f, -12113.01f, 163.7356f },
    { 13435.75f, -12117.79f, 162.3688f },
    { 13435.75f, -12117.79f, 162.3688f },
};

uint32 const infiltratorPathSize = 9;
const G3D::Vector3 infiltratorPath[infiltratorPathSize] =
{
    { 13630.61f, -12132.06f, 166.6226f },
    { 13629.61f, -12132.02f, 166.6226f },
    { 13616.54f, -12104.06f, 169.9852f },
    { 13587.36f, -12084.23f, 170.0140f },
    { 13538.41f, -11989.17f, 166.7399f },
    { 13396.21f, -12030.91f, 182.2028f },
    { 13334.26f, -12095.19f, 196.0419f },
    { 13406.59f, -12133.06f, 169.6312f },
    { 13406.59f, -12133.06f, 169.6312f },
};

uint32 const assaultDrakePathSize = 9;
const G3D::Vector3 assaultDrakePath[assaultDrakePathSize] =
{
    { 13624.19f, -12066.98f, 157.8710f },
    { 13623.19f, -12066.95f, 157.8710f },
    { 13616.54f, -12104.06f, 169.9852f },
    { 13587.36f, -12084.23f, 170.0140f },
    { 13541.30f, -12070.96f, 166.7399f },
    { 13462.05f, -12056.04f, 170.3707f },
    { 13441.70f, -12065.96f, 172.2309f },
    { 13434.73f, -12123.99f, 171.8846f },
    { 13434.73f, -12123.99f, 171.8846f },
};

uint32 const assaultDrake2PathSize = 10;
const G3D::Vector3 assaultDrake2Path[assaultDrake2PathSize] =
{
    { 13607.50f, -12171.00f, 155.4982f },
    { 13606.53f, -12170.76f, 155.4982f },
    { 13595.93f, -12161.82f, 166.2525f },
    { 13548.54f, -12167.88f, 170.3928f },
    { 13522.30f, -12171.48f, 168.2348f },
    { 13478.16f, -12183.98f, 173.9983f },
    { 13445.84f, -12183.30f, 172.7414f },
    { 13438.69f, -12166.11f, 169.0544f },
    { 13435.55f, -12138.29f, 171.0571f },
    { 13435.55f, -12138.29f, 171.0571f },
};

class boss_warmaster_blackhorn : public CreatureScript
{
public:
    boss_warmaster_blackhorn() : CreatureScript("boss_warmaster_blackhorn") {}

    struct boss_warmaster_blackhornAI : public BossAI
    {
        boss_warmaster_blackhornAI(Creature* creature) : BossAI(creature, DATA_WARMASTER_BLACKHORN) {}

        void IsSummonedBy(Unit* summoner)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
            events.SetPhase(PHASE_INTRO);
            events.ScheduleEvent(EVENT_TALK_INTRO_BLACKHORN, 7000);
            DragonDespawned = false;
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                me->SetLootMode(LOOT_MODE_LFR);
        }

        void DoAction(int32 const actionId) override
        {
            switch (actionId)
            {
                case ACTION_START_PHASE_2:
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    Talk(TALK_PHASE_2_BLACKHORN);
                    DoCast(me, SPELL_VENGEANCE, true);
                    events.ScheduleEvent(EVENT_DEVASTATE, 8500);
                    events.ScheduleEvent(EVENT_DISRUPTING_ROAR, 10000);
                    events.ScheduleEvent(EVENT_SHOCKWAVE, 13000);

                    if (instance->GetData(DATA_RAID_MODE) != RAID_MODE_LFR)
                        events.ScheduleEvent(EVENT_BERSERK, 240000);
                    break;
                case ACTION_DRAGON_DESPAWN:
                    DragonDespawned = true;
                    break;
                default:
                    break;
            }
        }

        void EnterCombat(Unit* victim) override
        {
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                me->SetLootMode(LOOT_MODE_LFR);
            _EnterCombat();
            eventActive = false;
        }

        void JustDied(Unit* killer) override
        {
            if (Creature* captain = me->FindNearestCreature(NPC_SKY_CAPTAIN_SWAYZE, 200.0f))
                captain->AI()->DoAction(ACTION_FIGHT_FINISHED);
            _JustDied();
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(TALK_KILLING_A_PLAYER);
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (!DragonDespawned && me->HealthBelowPctDamaged(1, damage))
            {
                if (!IsHeroic())
                {
                    DragonDespawned = true;
                    if (Creature* goriona = me->FindNearestCreature(NPC_GORIONA, 500.0f))
                        goriona->DespawnOrUnsummon();
                }
                else
                    damage = 0;
            }

            if (IsHeroic() && me->HealthBelowPctDamaged(20, damage) && !eventActive)
            {
                if (Creature* goriona = me->FindNearestCreature(NPC_GORIONA, 500.0f))
                {
                    if (goriona->GetHealthPct() > 21.00f)
                    {
                        eventActive = true;
                        events.ScheduleEvent(EVENT_SIPHON_VITALITY, 3000);
                    }
                }
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!events.IsInPhase(PHASE_INTRO) && !UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_TALK_INTRO_BLACKHORN:
                        Talk(TALK_INTRO_BLACKHORN);
                        events.ScheduleEvent(EVENT_TALK_INTRO_BLACKHORN_2, 13000);
                        break;
                    case EVENT_TALK_INTRO_BLACKHORN_2:
                        Talk(TALK_INTRO_2_BLACKHORN);
                        break;
                    case EVENT_DEVASTATE:
                        DoCastVictim(SPELL_DEVASTATE);
                        events.ScheduleEvent(EVENT_DEVASTATE, 8500);
                        break;
                    case EVENT_DISRUPTING_ROAR:
                        Talk(EMOTE_DISRUPTING_ROAR, NULL, CHAT_MSG_EMOTE);
                        DoCast(me, SPELL_DISRUPTING_ROAR, true);
                        events.ScheduleEvent(EVENT_DISRUPTING_ROAR, 18500);
                        break;
                    case EVENT_SHOCKWAVE:
                        Talk(TALK_SHOCKWAVE);
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 45.00f, true))
                        {
                            me->StopMoving();
                            me->SetFacingToObject(target);
                            me->SendMeleeAttackStop();
                            DoCast(target, SPELL_SHOCKWAVE);
                        }
                        events.ScheduleEvent(EVENT_SHOCKWAVE, 23000);
                        break;
                    case EVENT_SIPHON_VITALITY:
                        if (Creature* goriona = me->FindNearestCreature(NPC_GORIONA, 200.0f))
                        {
                            DoCast(goriona, SPELL_SIPHON_VITALITY, true);
                            eventActive = false;
                        }
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

        private:
            bool DragonDespawned;
            bool eventActive;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_warmaster_blackhornAI(creature);
    }
};

class npc_ds_sky_captain_swayze : public CreatureScript
{
public:
    npc_ds_sky_captain_swayze() : CreatureScript("npc_ds_sky_captain_swayze") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                {
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->DoAction(ACTION_START_COMBAT);
                }
                player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                creature->CastSpell((Unit*)NULL, SPELL_SPINE_OF_DEATHWING_MOVIE, true);
                player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                creature->CastSpell((Unit*)NULL, SPELL_TELEPORT_ALL_TO_GUNSHIP, true);
                player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF + 4:
                player->CLOSE_GOSSIP_MENU();
                break;
            default:
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (InstanceScript* instance = creature->GetInstanceScript())
        {
            switch (creature->GetAreaId())
            {
                case 5922:
                    if (instance->IsDone(DATA_ULTRAXION) && !instance->IsDone(DATA_WARMASTER_BLACKHORN))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bring us in closer!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    if (instance->IsDone(DATA_ULTRAXION) && instance->IsDone(DATA_WARMASTER_BLACKHORN) && !instance->IsDone(DATA_SPINE_OF_DEATHWING))
                        if (!creature->GetMap()->IsHeroic()
                            || (creature->GetMap()->IsHeroic() && instance->IsDoneInHeroic(DATA_MORCHOK) &&
                            instance->IsDoneInHeroic(DATA_YORSAHJ) && instance->IsDoneInHeroic(DATA_WARLORD_ZONOZZ) &&
                            instance->IsDoneInHeroic(DATA_HAGARA) && instance->IsDoneInHeroic(DATA_ULTRAXION) &&
                            instance->IsDoneInHeroic(DATA_WARMASTER_BLACKHORN)))
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, (player->GetTeam() == TEAM_HORDE ? "FOR THE HORDE!" : "FOR THE ALLIANCE!"), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    break;
                case 5923:
                    if (instance->IsDone(DATA_ULTRAXION))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We are the Horde. We are always ready.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    break;
                default:
                    break;
            }
        }
        player->SEND_GOSSIP_MENU(13373, creature->GetGUID());
        return true;
    }

    struct npc_ds_sky_captain_swayzeAI : public Scripted_NoMovementAI
    {
        npc_ds_sky_captain_swayzeAI(Creature* creature) : Scripted_NoMovementAI(creature), summons(creature)
        {
            instance = me->GetInstanceScript();
        }

        void setCannonsCombatState(bool active, bool phaseTwo = false)
        {
            std::list<WorldObject*> targetList;
            Trinity::AllWorldObjectsInRange objects(me, 70.0f);
            Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects);
            me->VisitNearbyObject(70.0f, searcher);
            for (std::list<WorldObject*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                if (Creature* cannon = (*i)->ToCreature())
                    if (cannon->GetEntry() == NPC_SKYFIRE_CANNON)
                    {
                        if (phaseTwo)
                        {
                            if ((cannon->GetPositionX() < 13422.00 && cannon->GetPositionX() > 13420.00f) || (cannon->GetPositionX() < 13445.00 && cannon->GetPositionX() > 13443.00f))
                                cannon->AI()->DoAction(ACTION_START_PHASE_2);
                        }
                        else
                            cannon->AI()->DoAction(active ? ACTION_ACTIVATE_CANNONS : ACTION_DEACTIVATE_CANNONS);
                    }
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            switch (summon->GetEntry())
            {
                case NPC_THE_SKYFIRE:
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, summon);
                    break;
                case NPC_GORIONA:
                    if (Creature* warmaster = me->SummonCreature(NPC_WARMASTER_BLACKHORN, gorionaSpawnPos))
                        warmaster->CastCustomSpell(summon, SPELL_RIDE_VEHICLE_HARDCODED, &seatId, NULL, NULL, true);
                    break;
                case NPC_TWILIGHT_ASSAULT_DRAKE:
                    if (Creature* dreadblade = me->SummonCreature(NPC_TWILIGHT_ELITE_DREADBLADE, twilightAssaulterPos[0].x, twilightAssaulterPos[0].y, twilightAssaulterPos[0].z, twilightAssaulterPos[0].orientation))
                        dreadblade->CastCustomSpell(summon, SPELL_RIDE_VEHICLE_HARDCODED, &seatId, NULL, NULL, true);
                    summon->GetMotionMaster()->MoveSmoothPath(assaultDrakePath, assaultDrakePathSize);
                    break;
                case NPC_TWILIGHT_ASSAULT_DRAKE_2:
                    if (Creature* slayer = me->SummonCreature(NPC_TWILIGHT_ELITE_SLAYER, twilightAssaulterPos[1].x, twilightAssaulterPos[1].y, twilightAssaulterPos[1].z, twilightAssaulterPos[1].orientation))
                        slayer->CastCustomSpell(summon, SPELL_RIDE_VEHICLE_HARDCODED, &seatId, NULL, NULL, true);
                    summon->GetMotionMaster()->MoveSmoothPath(assaultDrake2Path, assaultDrake2PathSize);
                    break;
                case NPC_TWILIGHT_INFILTRATOR:
                    if (Creature* sapper = me->SummonCreature(NPC_TWILIGHT_SAPPER, infiltratorPosition, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 4000))
                        sapper->CastCustomSpell(summon, SPELL_RIDE_VEHICLE_HARDCODED, &seatId, NULL, NULL, true);
                    summon->GetMotionMaster()->MoveSmoothPath(infiltratorPath, infiltratorPathSize);
                    break;
                case NPC_FIRE_STALKER:
                    summon->CastSpell(summon, SPELL_SHIP_FIRE, true);
                    break;
                default:
                    break;
            }

            summon->SetInCombatWithZone();
        }

        void SummonedCreatureDies(Creature* summon, Unit* killer)
        {
            switch (summon->GetEntry())
            {
                case NPC_TWILIGHT_ASSAULT_DRAKE:
                case NPC_TWILIGHT_ASSAULT_DRAKE_2:
                    if (++summonKillCount >= 6)
                    {
                        Talk(TALK_PHASE_2);
                        setCannonsCombatState(true, true);
                        events.CancelEvent(EVENT_SUMMON_SAPPER);
                        events.CancelEvent(EVENT_SUMMON_ASSAULTER);
                        for (uint64 summonGuid : summons)
                            if (Creature* summoned = ObjectAccessor::GetCreature(*me, summonGuid))
                                if (summoned->GetEntry() == NPC_GORIONA)
                                    summoned->AI()->DoAction(ACTION_START_PHASE_2);
                    }
                    break;
                case NPC_THE_SKYFIRE:
                    Talk(TALK_SKYFIRE_DESTROYED);
                    events.ScheduleEvent(EVENT_MASSIVE_EXPLOSION, 1000);
                    for (uint8 i = 0; i < 6; i++)
                        if (Creature* massive = me->SummonCreature(NPC_MASSIVE_EXPLOSION, massiveExplosionPos[i]))
                            massiveExplosion.push_back(massive);
                    break;
                default:
                    break;
            }
        }

        void SummonedCreatureDamageTaken(Unit* /*attacker*/, Creature* summon, uint32& damage, SpellInfo const* /*spellProto*/)
        {
            switch (summon->GetEntry())
            {
                case NPC_THE_SKYFIRE:
                    if (!IsHeroic())
                        break;

                    if (summon->HealthBelowPctDamaged(75, damage) && phaseCount == 0)
                    {
                        for (uint8 i = 0; i < 12; i++)
                            me->SummonCreature(NPC_FIRE_STALKER, shipFireStalkerPos[i]);

                        SpawnDeckFire(true);
                        events.RescheduleEvent(EVENT_DECK_FIRE_CHECK, 3000);
                        events.ScheduleEvent(EVENT_CHECK_BRIGADE_TARGET, 6000);
                        summon->AI()->Talk(EMOTE_DECK_FIRE, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        phaseCount++;
                    }
                    else if (summon->HealthBelowPctDamaged(50, damage) && phaseCount == 1)
                    {
                        SpawnDeckFire(true);
                        summon->AI()->Talk(EMOTE_DECK_FIRE, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        events.RescheduleEvent(EVENT_DECK_FIRE_CHECK, 3000);
                        phaseCount++;
                    }
                    else if (summon->HealthBelowPctDamaged(25, damage) && phaseCount == 2)
                    {
                        SpawnDeckFire(true);
                        summon->AI()->Talk(EMOTE_DECK_FIRE, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        events.RescheduleEvent(EVENT_DECK_FIRE_CHECK, 3000);
                        phaseCount++;
                    }
                    break;
                default:
                    break;
            }
        }

        void SpawnDeckFire(bool initial)
        {
            if (initial)
            {
                if (Creature* deckFireController = me->SummonCreature(NPC_DECK_FIRE_CONTROLLER, deckFireSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                {
                    std::list<Creature* > triggers;
                    me->GetCreatureListWithEntryInGrid(triggers, NPC_SUMMON_ENABLER_STALKER, 200.0f);

                    if (initialFireController)
                        deckFireController->CastSpell(deckFireController, SPELL_DECK_FIRE_PERIODIC, true);
                    initialFireController = false;

                    if (!triggers.empty())
                    {
                        auto trigger = Trinity::Containers::SelectRandomContainerElement(triggers);
                        Position pos;
                        Position triggerPos(*trigger);
                        deckFireController->GetRandomPoint(triggerPos, 6.0f, pos);
                        deckFireController->CastSpell(pos.m_positionX, pos.m_positionY, pos.m_positionZ, SPELL_DECK_FIRE_INITIAL_SPAWN, true);
                    }
                }
            }
            else
            {
                for (uint64 summonGuid : summons)
                {
                    if (Creature* summon = ObjectAccessor::GetCreature(*me, summonGuid))
                    {
                        if (summon->GetEntry() == NPC_DECK_FIRE_CONTROLLER)
                        {
                            for (int8 i = 0; i < 2; i++)
                            {
                                if (DynamicObject* lastDynObj = summon->GetLastDynObject())
                                {
                                    float angle = frand(0.0f, 2.0f * M_PI);
                                    float step = float(M_PI) / 8.0f;
                                    float spawnDist = 6.0f;
                                    for (uint8 i = 0; i < 16; ++i)
                                    {
                                        float tempAngle = Position::NormalizeOrientation(angle);
                                        Position pos;
                                        lastDynObj->GetFirstCollisionPosition(pos, spawnDist, tempAngle);
                                        std::list<WorldObject*> targetList;
                                        Trinity::AllWorldObjectsInRange objects(lastDynObj, 8.0f);
                                        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(lastDynObj, targetList, objects, GRID_MAP_TYPE_MASK_DYNAMICOBJECT);
                                        me->VisitNearbyObject(10.0f, searcher);

                                        bool validPos = true;
                                        for (auto nearDynObj : targetList)
                                        {
                                            if (nearDynObj->GetExactDist2d(pos.GetPositionX(), pos.GetPositionY()) < spawnDist / 2.0f)
                                            {
                                                validPos = false;
                                                break;
                                            }
                                        }

                                        if (validPos)
                                        {
                                            summon->CastSpell(pos.m_positionX, pos.m_positionY, pos.m_positionZ, SPELL_DECK_FIRE, true);
                                            if (++dynObjCount >= 8)
                                            {
                                                me->SummonCreature(NPC_SKYFIRE_FIRE_BRIGADE, skyfireBrigadeSpawnPos);
                                                dynObjCount = 0;
                                            }
                                            break;
                                        }
                                        angle += step;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            switch (summon->GetEntry())
            {
                case NPC_THE_SKYFIRE:
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, summon);
                    break;
                case NPC_MASSIVE_EXPLOSION:
                    massiveExplosion.remove(summon);
                    break;
                case NPC_DECK_FIRE_CONTROLLER:
                    summon->RemoveAllDynObjects();
                    break;
            }
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_START_COMBAT:
                    Talk(TALK_INTRO);
                    initialFireController = true;
                    summonCount = 0;
                    summonKillCount = 0;
                    phaseCount = 0;
                    dynObjCount = 0;
                    seatId = 1;
                    me->SummonCreature(NPC_GORIONA, gorionaSpawnPos);
                    me->SummonCreature(NPC_THE_SKYFIRE, skyfirePosition);
                    instance->SetBossState(DATA_WARMASTER_BLACKHORN, IN_PROGRESS);
                    events.ScheduleEvent(EVENT_SUMMON_ASSAULTER, 22800);
                    events.ScheduleEvent(EVENT_PLAYER_ALIVE_CHECK, 2000);
                    events.ScheduleEvent(EVENT_INTRO_TALK, 13000);
                    if (instance->GetData(DATA_RAID_MODE) != RAID_MODE_LFR)
                        events.ScheduleEvent(EVENT_SUMMON_SAPPER, 69000);
                    break;
                case ACTION_FIGHT_FINISHED:
                    summons.DespawnEntry(NPC_THE_SKYFIRE);
                    events.Reset();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    setCannonsCombatState(false);
                    break;
                case ACTION_DELAY_DECK_FIRE:
                    // Only restart event timer if the event is active
                    if (events.GetNextEventTime(EVENT_DECK_FIRE_CHECK) != 0)
                        events.RescheduleEvent(EVENT_DECK_FIRE_CHECK, 6000);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_ASSAULTER:
                    {
                        for (int8 i = 0; i < 2; i++)
                            me->SummonCreature(twilightAssaulterPos[i].npcId, twilightAssaulterPos[i].x, twilightAssaulterPos[i].y, twilightAssaulterPos[i].z, twilightAssaulterPos[i].orientation, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 4000);
                        if (++summonCount < 3)
                            events.ScheduleEvent(EVENT_SUMMON_ASSAULTER, 61000);
                        break;
                    }
                    case EVENT_SUMMON_SAPPER:
                        me->SummonCreature(NPC_TWILIGHT_INFILTRATOR, infiltratorPosition, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 4000);
                        events.ScheduleEvent(EVENT_SUMMON_SAPPER, 40000);
                        break;
                    case EVENT_PLAYER_ALIVE_CHECK:
                    {
                        bool playerAlive = false;
                        auto& players = me->GetMap()->GetPlayers();
                        for (auto itr = players.begin(); itr != players.end(); ++itr)
                        {
                            if (Player* player = itr->getSource())
                                if (player->GetAreaId() == 5922 && player->isAlive() && !player->isGameMaster())
                                {
                                    playerAlive = true;
                                    break;
                                }
                        }

                        if (!playerAlive)
                        {
                            events.Reset();
                            summons.DespawnAll();
                            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            if (!instance->IsDone(DATA_WARMASTER_BLACKHORN))
                                instance->SetBossState(DATA_WARMASTER_BLACKHORN, NOT_STARTED);
                            setCannonsCombatState(false);
                            break;
                        }
                        events.ScheduleEvent(EVENT_PLAYER_ALIVE_CHECK, 2000);
                        break;
                    }
                    case EVENT_INTRO_TALK:
                        Talk(TALK_INTRO_2);
                        events.ScheduleEvent(EVENT_INTRO_TALK_2, 14000);
                        break;
                    case EVENT_INTRO_TALK_2:
                        Talk(TALK_INTRO_3);
                        setCannonsCombatState(true);
                        break;
                    case EVENT_MASSIVE_EXPLOSION:
                        if (Creature* explosion = Trinity::Containers::SelectRandomContainerElement(massiveExplosion))
                            explosion->CastSpell(explosion, SPELL_MASSIVE_EXPLOSION, true);
                        events.ScheduleEvent(EVENT_MASSIVE_EXPLOSION, 1000);
                        break;
                    case EVENT_DECK_FIRE_CHECK:
                        SpawnDeckFire(false);
                        events.RescheduleEvent(EVENT_DECK_FIRE_CHECK, 3000);
                        break;
                    case EVENT_CHECK_BRIGADE_TARGET:
                    {
                        events.ScheduleEvent(EVENT_CHECK_BRIGADE_TARGET, 250);

                        std::list<Creature*> targetList;
                        for (uint64 summonGuid : summons)
                            if (Creature* summoned = ObjectAccessor::GetCreature(*me, summonGuid))
                                if (summoned->GetEntry() == NPC_SKYFIRE_FIRE_BRIGADE)
                                    targetList.push_back(summoned);

                        std::list<WorldObject*> dynObjList;
                        Trinity::AllWorldObjectsInRange objects(me, 200.0f);
                        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, dynObjList, objects, GRID_MAP_TYPE_MASK_DYNAMICOBJECT);
                        me->VisitNearbyObject(200.0f, searcher);

                        if (targetList.empty())
                            break;

                        // remove wrong dyn objects (other visuals)
                        dynObjList.remove_if([](WorldObject* target)
                        {
                            return !target->ToDynObject() || (target->ToDynObject()->GetSpellId() != SPELL_DECK_FIRE_INITIAL_SPAWN && target->ToDynObject()->GetSpellId() != SPELL_DECK_FIRE);
                        });

                        if (dynObjList.empty() && !targetList.empty())
                        {
                            for (Creature* brigade : targetList)
                                brigade->DespawnOrUnsummon();
                            break;
                        }

                        for (auto itr = targetList.begin(); itr != targetList.end(); /*do nothing*/)
                        {
                            Creature* brigade = *itr;
                            // remove dyn objects that are already in use
                            dynObjList.remove_if([brigade] (WorldObject* target) -> bool
                            {
                                return target->GetGUID() == brigade->AI()->GetGUID();
                            });

                            // remove busy brigades
                            if (brigade->AI()->GetGUID())
                                itr = targetList.erase(itr);
                            else
                                ++itr;
                        }
                        // now set target
                        for (Creature* brigade : targetList)
                        {
                            if (dynObjList.empty())
                            {
                                if (brigade->AI()->GetData(DATA_FIRE_ATTEMPS) > 10)
                                    brigade->DespawnOrUnsummon();
                                else
                                    brigade->AI()->SetData(DATA_FIRE_ATTEMPS, brigade->AI()->GetData(DATA_FIRE_ATTEMPS) + 1);
                            }
                            else
                            {
                                auto itr = dynObjList.begin();
                                std::advance(itr, urand(0, dynObjList.size() - 1));
                                brigade->AI()->SetGUID(( *itr )->GetGUID(), 0);
                                dynObjList.erase(itr);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        SummonList summons;
        std::list<Creature*> massiveExplosion;
        uint8 summonCount;
        uint8 summonKillCount;
        uint8 phaseCount;
        uint8 dynObjCount;
        int32 seatId;
        bool initialFireController;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_sky_captain_swayzeAI(creature);
    }
};

class npc_ds_skyfire_fire_brigade : public CreatureScript
{
public:
    npc_ds_skyfire_fire_brigade() : CreatureScript("npc_ds_skyfire_fire_brigade")
    {}

    struct npc_ds_skyfire_fire_brigadeAI : public ScriptedAI
    {
        npc_ds_skyfire_fire_brigadeAI(Creature* creature) : ScriptedAI(creature)
        {}

        void IsSummonedBy(Unit* summoner)
        {
            me->AddAura(SPELL_ROCKET_PACK, me);
            me->GetMotionMaster()->MoveSmoothPath(brigadePath, brigadePathSize);
            DynObjectGuid = 0;
            fireAttemps = 0;
            ready = false;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE)
            {
                switch (id)
                {
                    case POINT_FIRE:
                        DoCast(SPELL_WATER_JET);
                        events.ScheduleEvent(EVENT_FIRE_EXTINGUISHED, 2500);
                        break;
                    default:
                        break;
                }
            }
            if (type == SPLINE_MOTION_TYPE)
            {
                switch (id)
                {
                    case POINT_ASC:
                        ready = true;
                        break;
                    default:
                        break;
                }
            }
        }

        void SetGUID(uint64 guid, int32 /*id*/) override
        {
            if (ready)
            {
                DynObjectGuid = guid;
                DoAction(ACTION_FIND_DYNOBJ_FIRE);
            }
        }

        void DoAction(int32 const action)
        {
            if (action == ACTION_FIND_DYNOBJ_FIRE)
                events.ScheduleEvent(EVENT_SEARCH_FIRE, 100);
        }

        uint32 GetData(uint32 data) const override
        {
            if (data == DATA_FIRE_ATTEMPS)
                return fireAttemps;
            return 0;
        }

        void SetData(uint32 data, uint32 value) override
        {
            if (data == DATA_FIRE_ATTEMPS)
                fireAttemps = value;
        }

        uint64 GetGUID(int32 /*id*/) const override
        {
            return DynObjectGuid;
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SEARCH_FIRE:
                    {
                        if (DynamicObject* dyn = ObjectAccessor::GetDynamicObject(*me, DynObjectGuid))
                        {
                            Position pos(*dyn);
                            pos.m_positionZ = me->GetPositionZ();
                            me->GetMotionMaster()->MovePoint(POINT_FIRE, pos, false);
                        }
                        else
                            DynObjectGuid = 0;
                        break;
                    }
                    case EVENT_FIRE_EXTINGUISHED:
                    {
                        if (DynamicObject* dynObj = ObjectAccessor::GetDynamicObject(*me, DynObjectGuid))
                            dynObj->Remove();
                        DynObjectGuid = 0;
                        break;
                    }
                }
            }
        }
    private:
        EventMap events;
        uint64 DynObjectGuid;
        uint32 fireAttemps;
        bool ready;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_skyfire_fire_brigadeAI(creature);
    }
};

class npc_ds_goriona : public CreatureScript
{
public:
    npc_ds_goriona() : CreatureScript("npc_ds_goriona") { }

    struct npc_ds_gorionaAI : public ScriptedAI
    {
        npc_ds_gorionaAI(Creature* creature) : ScriptedAI(creature) {}

        void IsSummonedBy(Unit* summoner) override
        {
            land = false;
            despawn = false;
            me->SetReactState(REACT_PASSIVE);
            events.ScheduleEvent(EVENT_TWILIGHT_ONSLAUGHT, 48000);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_BROADSIDE, 57000);

            me->GetMotionMaster()->MoveSmoothPath(GorionaPath, GorionaPathSize);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE)
            {
                switch (id)
                {
                    case POINT_SKYFIRE_DECK:
                        DoCast(me, SPELL_EJECT_PASSENGER_1, true);
                        events.ScheduleEvent(EVENT_CORRECT_ORIENTATION_2, 100);
                        events.ScheduleEvent(EVENT_MOVE_ON_THE_OTHER_SIDE, 7000);
                        break;
                    case POINT_END_POSITION:
                        events.ScheduleEvent(EVENT_TWILIGHT_FLAMES, 1000);
                        events.ScheduleEvent(EVENT_CORRECT_ORIENTATION_2, 100);
                        break;
                    case POINT_LANDING:
                        events.ScheduleEvent(EVENT_CORRECT_ORIENTATION_2, 100);
                        events.ScheduleEvent(EVENT_LAND, 200);
                        break;
                    case POINT_DESPAWN:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }
            if (type == SPLINE_MOTION_TYPE)
            {
                switch (id)
                {
                    case GorionaPathSize-1:
                        events.ScheduleEvent(EVENT_CORRECT_ORIENTATION_2, 200);
                        break;
                    default:
                        break;
                }
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (!land)
            {
                if (!IsHeroic() && me->HealthBelowPctDamaged(21, damage))
                {
                    land = true;
                    events.Reset();
                    me->GetMotionMaster()->MovePoint(POINT_DESPAWN, 13448.653f, -11990.629f, 167.487f);
                }

                if (IsHeroic() && events.IsInPhase(PHASE_TWO) && me->HealthBelowPctDamaged(91, damage))
                {
                    land = true;
                    events.CancelEvent(EVENT_TWILIGHT_FLAMES);
                    Position pos = { 13421.300f, -12131.900f, 167.952f, 5.191356f };
                    me->GetMotionMaster()->MovePoint(POINT_LANDING, pos);
                }
            }
            else if (land && !despawn)
            {
                if (IsHeroic() && events.IsInPhase(PHASE_SHIP) && me->HealthBelowPctDamaged(21, damage))
                {
                    despawn = true;
                    events.Reset();
                    me->SetWalk(false);
                    me->SetDisableGravity(true);
                    me->SetCanFly(true);
                    me->SendMeleeAttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->GetMotionMaster()->MovePoint(POINT_DESPAWN, 13448.653f, -11990.629f, 167.487f);

                    despawn = true;

                    if (Creature* blackhorn = me->FindNearestCreature(NPC_WARMASTER_BLACKHORN, 200.0f))
                        blackhorn->AI()->DoAction(ACTION_DRAGON_DESPAWN);

                    Talk(EMOTE_GORIONA_ESCAPE, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                }
            }
        }

        void PassengerBoarded(Unit* passenger, int8 seatId, bool apply) override
        {
            if (!apply)
                passenger->ToCreature()->AI()->DoAction(ACTION_START_PHASE_2);
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_START_PHASE_2)
            {
                events.Reset();
                events.SetPhase(PHASE_TWO);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_CONSUMING_SHROUD, 45000);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->GetMotionMaster()->MovePoint(POINT_SKYFIRE_DECK, 13422.190f, -12133.19f, 171.3658f);
            }
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_SIPHON_VITALITY)
            {
                DoCast(caster, SPELL_SIPHON_VITALITY_DND, true);
                caster->ToCreature()->AI()->Talk(EMOTE_SIPHON_VITALITY, 0, CHAT_MSG_RAID_BOSS_EMOTE);
            }
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_BROADSIDE_VISUAL && targetCount++ == 1)
                target->CastSpell(target, SPELL_BROADSIDE_DAMAGE, true);
        }

        void JustSummoned(Creature* summon) override
        {
            if (summon->GetEntry() == NPC_TWILIGHT_FLAME_GORIONA)
                summon->CastSpell(summon, SPELL_TWILIGHT_FLAMES_PERIODIC, true);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (events.IsInPhase(PHASE_SHIP) && !UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TWILIGHT_ONSLAUGHT:
                    {
                        if (Creature* blackhorn = me->FindNearestCreature(NPC_WARMASTER_BLACKHORN, 100.0f))
                            blackhorn->AI()->Talk(TALK_TWILIGHT_ONSLAUGHT);

                        Talk(EMOTE_TWILIGHT_ONSLAUGHT, 0, CHAT_MSG_RAID_BOSS_EMOTE);

                        if (IsHeroic())
                        {
                            std::list<WorldObject*> dynObjList;
                            Trinity::AllWorldObjectsInRange objects(me, 200.0f);
                            Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, dynObjList, objects, GRID_MAP_TYPE_MASK_DYNAMICOBJECT);
                            me->VisitNearbyObject(200.0f, searcher);
                            for (uint8 i = 0; i < 56; i++)
                            {
                                uint8 pos = urand(0, 56);
                                bool validPos = true;
                                for (WorldObject* dynObj : dynObjList)
                                    if (dynObj->GetDistance2d(twilightBarragePos[pos].GetPositionX(), twilightBarragePos[pos].GetPositionY()) <= 8.00f)
                                        validPos = false;

                                if (validPos)
                                {
                                    if (Creature* target = me->SummonCreature(NPC_ONSLAUGHT_TARGET, twilightBarragePos[pos].GetPositionX(), twilightBarragePos[pos].GetPositionY(), twilightBarragePos[pos].GetPositionZ(), 0.00f, TEMPSUMMON_TIMED_DESPAWN, 10500))
                                    {
                                        target->CastSpell(target, SPELL_TWILIGHT_ONSLAUGHT_VISUAL, true);
                                        me->CastSpell(target, SPELL_TWILIGHT_ONSLAUGHT, false);

                                        if (Creature* swayze = me->FindNearestCreature(NPC_SKY_CAPTAIN_SWAYZE, 100.0f))
                                            swayze->AI()->DoAction(ACTION_DELAY_DECK_FIRE);
                                    }
                                    break;
                                }
                            }
                        }
                        else
                        {
                            uint8 pos = urand(0, 56);
                            if (Creature* target = me->SummonCreature(NPC_ONSLAUGHT_TARGET, twilightBarragePos[pos].GetPositionX(), twilightBarragePos[pos].GetPositionY(), twilightBarragePos[pos].GetPositionZ(), 0.00f, TEMPSUMMON_TIMED_DESPAWN, 10500))
                            {
                                target->CastSpell(target, SPELL_TWILIGHT_ONSLAUGHT_VISUAL, true);
                                me->CastSpell(target, SPELL_TWILIGHT_ONSLAUGHT, false);
                            }
                        }
                        events.ScheduleEvent(EVENT_TWILIGHT_ONSLAUGHT, 35000);
                        break;
                    }
                    case EVENT_CORRECT_ORIENTATION_2:
                        if (Creature* skyfire = me->FindNearestCreature(NPC_THE_SKYFIRE, 200.0f))
                            me->SetFacingToObject(skyfire);
                        break;
                    case EVENT_MOVE_ON_THE_OTHER_SIDE:
                        me->GetMotionMaster()->MovePoint(POINT_END_POSITION, 13408.500f, -12090.550f, 168.464f);
                        break;
                    case EVENT_TWILIGHT_FLAMES:
                        DoCastRandom(SPELL_TWILIGHT_FLAMES, 200.0f);
                         events.ScheduleEvent(EVENT_TWILIGHT_FLAMES, urand(8500, 9500));
                        break;
                    case EVENT_BROADSIDE:
                        Talk(EMOTE_BROADSIDE, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        DoCast(SPELL_BROADSIDE_VISUAL);
                        targetCount = 0;
                        events.ScheduleEvent(EVENT_BROADSIDE, 90000);
                        break;
                    case EVENT_CONSUMING_SHROUD:
                        DoCast(me, SPELL_CONSUMING_SHROUD);
                        events.ScheduleEvent(EVENT_CONSUMING_SHROUD, 30000);
                        break;
                    case EVENT_TWILIGHT_BREATH:
                        DoCast(SPELL_TWILIGHT_BREATH);
                        events.ScheduleEvent(EVENT_TWILIGHT_BREATH, 20500);
                        break;
                    case EVENT_LAND:
                    {
                        Movement::MoveSplineInit init(me);
                        G3D::Vector3 ground(13423.400f, -12132.400f, 153.000f);
                        init.MoveTo(ground);
                        init.SetFly();
                        init.SetSmooth();
                        init.SetParabolic(14.00f, 0);
                        init.Launch();
                        events.ScheduleEvent(EVENT_COMBAT_START, 1000);
                        break;
                    }
                    case EVENT_COMBAT_START:
                        events.SetPhase(PHASE_SHIP);
                        me->SetWalk(true);
                        me->SetDisableGravity(false);
                        me->SetCanFly(false);
                        me->SetReactState(REACT_AGGRESSIVE);
                        events.ScheduleEvent(EVENT_TWILIGHT_BREATH, 10000);
                        break;
                    default:
                        break;
                }
            }
            if (events.IsInPhase(PHASE_SHIP))
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        bool land;
        bool despawn;
        int8 targetCount;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_gorionaAI(creature);
    }
};

class npc_ds_twilight_assaulter_blackhorn : public CreatureScript
{
public:
    npc_ds_twilight_assaulter_blackhorn() : CreatureScript("npc_ds_twilight_assaulter_blackhorn") { }

    struct npc_ds_twilight_assaulter_blackhornAI : public ScriptedAI
    {
        npc_ds_twilight_assaulter_blackhornAI(Creature* creature) : ScriptedAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            firstHarpoon = true;
            me->SetReactState(REACT_PASSIVE);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE)
            {
                switch (id)
                {
                    case POINT_END_POSITION:
                        events.ScheduleEvent(EVENT_CORRECT_ORIENTATION, 100);
                        break;
                }
            }
            if (type == SPLINE_MOTION_TYPE)
            {
                switch (id)
                {
                    case assaultDrakePathSize-1:
                    case assaultDrake2PathSize-1:
                        if (me->GetEntry() == NPC_TWILIGHT_ASSAULT_DRAKE && id == assaultDrakePathSize-1
                            || me->GetEntry() == NPC_TWILIGHT_ASSAULT_DRAKE_2 && id == assaultDrake2PathSize-1)
                        {
                            DoCast(me, SPELL_EJECT_PASSENGER_1, true);
                            events.ScheduleEvent(EVENT_TWILIGHT_BARRAGE, 5000);
                            events.ScheduleEvent(EVENT_MOVE_TO_END_POSITION, 200);
                        }
                }
            }
        }

        void SpellHit(Unit* attacker, SpellInfo const* spellInfo)
        {
            if (spellInfo->Id == SPELL_HARPOON)
            {
                Position pos(*attacker);
                attacker->GetNearPosition(pos, 5.0f, 0.0f);
                pos.m_positionZ += 5.0f;
                me->GetMotionMaster()->MovePoint(0, pos);
            }
        }

        void PassengerBoarded(Unit* passenger, int8 seatId, bool apply)
        {
            if (!apply)
                passenger->ToCreature()->AI()->DoAction(ACTION_START_COMBAT);
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_TO_END_POSITION:
                        if (me->GetEntry() == NPC_TWILIGHT_ASSAULT_DRAKE)
                            me->GetMotionMaster()->MovePoint(POINT_END_POSITION, 13433.3f, -12205.2f, 176.183f);
                        else
                            me->GetMotionMaster()->MovePoint(POINT_END_POSITION, 13433.3f, -12060.7f, 176.183f);
                        break;
                    case EVENT_TWILIGHT_BARRAGE:
                    {
                        if (IsHeroic())
                        {
                            std::list<WorldObject*> dynObjList;
                            Trinity::AllWorldObjectsInRange objects(me, 200.0f);
                            Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, dynObjList, objects, GRID_MAP_TYPE_MASK_DYNAMICOBJECT);
                            me->VisitNearbyObject(200.0f, searcher);
                            for (uint8 i = 0; i < 56; i++)
                            {
                                uint8 pos = urand(0, 56);
                                bool validPos = true;
                                for (WorldObject* dynObj : dynObjList)
                                    if (dynObj->GetDistance2d(twilightBarragePos[pos].GetPositionX(), twilightBarragePos[pos].GetPositionY()) <= 8.00f)
                                        validPos = false;

                                if (validPos)
                                {
                                    me->CastSpell(twilightBarragePos[pos].GetPositionX(), twilightBarragePos[pos].GetPositionY(), twilightBarragePos[pos].GetPositionZ(), SPELL_TWILIGHT_BARRAGE, true);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            uint8 pos = urand(0, 56);
                            me->CastSpell(twilightBarragePos[pos].GetPositionX(), twilightBarragePos[pos].GetPositionY(), twilightBarragePos[pos].GetPositionZ(), SPELL_TWILIGHT_BARRAGE, true);
                        }
                        events.ScheduleEvent(EVENT_TWILIGHT_BARRAGE, me->GetMap()->Is25ManRaid() ? 5000 : 10000);
                        break;
                    }
                    case EVENT_CORRECT_ORIENTATION:
                    {
                        if (Creature* cannon = me->FindNearestCreature(NPC_SKYFIRE_HARPOON_GUN, 80.0f))
                            me->SetFacingToObject(cannon);
                        Position pos(*me);
                        me->SetHomePosition(pos);
                        events.ScheduleEvent(EVENT_ACTIVATE_HARPOON, 10000);
                        firstHarpoon = false;
                        break;
                    }
                    case EVENT_ACTIVATE_HARPOON:
                        if (Creature* harpoon = me->FindNearestCreature(NPC_SKYFIRE_HARPOON_GUN, 80.0f))
                        {
                            if (!harpoon->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            {
                                harpoon->CastSpell(me, SPELL_HARPOON, false);
                                events.ScheduleEvent(EVENT_DEACTIVATE_HARPOON, urand(20000, 23000));
                            }
                            else
                                events.ScheduleEvent(EVENT_ACTIVATE_HARPOON, 1000);
                        }
                        break;
                    case EVENT_DEACTIVATE_HARPOON:
                        if (Creature* harpoon = me->FindNearestCreature(NPC_SKYFIRE_HARPOON_GUN, 80.0f))
                            harpoon->InterruptNonMeleeSpells(false);
                        me->GetMotionMaster()->MovePoint(POINT_END_POSITION, me->GetHomePosition());
                        break;
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
        uint8 lastWaypoint;
        bool firstHarpoon;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_twilight_assaulter_blackhornAI(creature);
    }
};

class npc_ds_skyfire_cannon : public CreatureScript
{
public:
    npc_ds_skyfire_cannon() : CreatureScript("npc_ds_skyfire_cannon") { }

    struct npc_ds_skyfire_cannonAI : public Scripted_NoMovementAI
    {
        npc_ds_skyfire_cannonAI(Creature* creature) : Scripted_NoMovementAI(creature) {}

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_ACTIVATE_CANNONS:
                    events.ScheduleEvent(EVENT_HEAVY_SLUG, urand(1000, 3000));
                    break;
                case ACTION_DEACTIVATE_CANNONS:
                    me->RemoveAurasDueToSpell(SPELL_ARTILLERY_BARRAGE);
                    events.Reset();
                    break;
                case ACTION_START_PHASE_2:
                    if (!IsHeroic())
                        break;
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    events.CancelEvent(EVENT_HEAVY_SLUG);
                    events.ScheduleEvent(EVENT_SET_FACING_TO, 1000);
                    if (Creature* goriona = me->FindNearestCreature(NPC_GORIONA, 200.00f))
                        DoCast(goriona, SPELL_ARTILLERY_BARRAGE);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_HEAVY_SLUG:
                    {
                        Creature* assaulter = me->FindNearestCreature(NPC_TWILIGHT_ASSAULT_DRAKE, 100.0f);
                        Creature* twilightAssaulter = me->FindNearestCreature(NPC_TWILIGHT_ASSAULT_DRAKE_2, 100.0f);

                        if ((assaulter && me->isInFront(assaulter)) || (twilightAssaulter && me->isInFront(twilightAssaulter)))
                            DoCast(me, SPELL_HEAVY_SLUG, false);

                        events.ScheduleEvent(EVENT_HEAVY_SLUG, 5000);
                        break;
                    }
                    case EVENT_SET_FACING_TO:
                        if (Creature* goriona = me->FindNearestCreature(NPC_GORIONA, 200.00f))
                        {
                            if (!goriona->IsFlying())
                            {
                                me->RemoveAurasDueToSpell(SPELL_ARTILLERY_BARRAGE);
                                break;
                            }
                            else
                                me->SetFacingToObject(goriona);
                        }

                        events.ScheduleEvent(EVENT_SET_FACING_TO, 1000);
                        break;
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_skyfire_cannonAI(creature);
    }
};

class npc_ds_twilight_elite : public CreatureScript
{
public:
    npc_ds_twilight_elite() : CreatureScript("npc_ds_twilight_elite") { }

    struct npc_ds_twilight_eliteAI : public ScriptedAI
    {
        npc_ds_twilight_eliteAI(Creature* creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_START_COMBAT:
                    events.ScheduleEvent(EVENT_BLADE_RUSH, 15500);
                    events.ScheduleEvent(me->GetEntry() == NPC_TWILIGHT_ELITE_DREADBLADE ? EVENT_DEGENERATION : EVENT_BRUTAL_STRIKE, urand(8500, 9500));
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BLADE_RUSH:

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 70.0f, true))
                        {
                            float targetDist = me->GetExactDist2d(target);
                            uint32 points = std::max<uint32>(1, targetDist / 3);
                            float dist = 3.0f;
                            float angle = me->GetAngle(target) - me->GetOrientation();
                            angle = me->NormalizeOrientation(angle);

                            for (uint32 i = 0; i < points; i++)
                            {
                                Position pos(*me);
                                me->GetNearPosition(pos, dist, angle);
                                dist += 3.0f;
                                me->SendPlaySpellVisual(22474, NULL, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
                            }
                            me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_BLADE_RUSH, false);
                            Position pos(*me);
                            me->SummonCreature(NPC_LIGHTNING_COSMETIC_TARGET, pos, TEMPSUMMON_TIMED_DESPAWN, 4000); // this trigger do NOTHING we just need the trigger for the path calculation of blade rush
                        }
                        events.ScheduleEvent(EVENT_BLADE_RUSH, 15500);
                        break;
                    case EVENT_BRUTAL_STRIKE:
                        DoCastVictim(SPELL_BRUTAL_STRIKE);
                        events.ScheduleEvent(EVENT_BRUTAL_STRIKE, urand(8500, 9500));
                        break;
                    case EVENT_DEGENERATION:
                        DoCastVictim(SPELL_DEGENERATION);
                        events.ScheduleEvent(EVENT_DEGENERATION, urand(8500, 9500));
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_twilight_eliteAI(creature);
    }
};

class npc_ds_twilight_infiltrator : public CreatureScript
{
public:
    npc_ds_twilight_infiltrator() : CreatureScript("npc_ds_twilight_infiltrator") { }

    struct npc_ds_twilight_infiltratorAI : public ScriptedAI
    {
        npc_ds_twilight_infiltratorAI(Creature* creature) : ScriptedAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            me->SetReactState(REACT_PASSIVE);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == SPLINE_MOTION_TYPE && id == infiltratorPathSize-1)
            {
                DoCast(me, SPELL_EJECT_PASSENGER_1, true);
                me->DespawnOrUnsummon(2000);
            }
        }

        void PassengerBoarded(Unit* passenger, int8 seatId, bool apply)
        {
            if (!apply)
                passenger->ToCreature()->AI()->DoAction(ACTION_START_COMBAT);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_twilight_infiltratorAI(creature);
    }
};

class npc_ds_sapper : public CreatureScript
{
public:
    npc_ds_sapper() : CreatureScript("npc_ds_sapper") { }

    struct npc_ds_sapperAI : public ScriptedAI
    {
        npc_ds_sapperAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
            delayedDeath = false;
        }

        void JustSummoned(Creature* /*summoner*/) override
        {
            me->SetReactState(REACT_PASSIVE);
            me->ApplySpellImmune(0, IMMUNITY_ID, 82691, true); // ring of frost
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        }

        void DoAction(int32 const action)
        {
            if (action == ACTION_START_COMBAT)
            {
                events.ScheduleEvent(EVENT_SMOKE_BOMB, 2100);
                events.ScheduleEvent(EVENT_MOVE_TO_MACHINERY, 2200);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (!delayedDeath && type == POINT_MOTION_TYPE && id == POINT_MACHINERY)
            {
                if (me->GetPositionX() >= 13469.00f)
                {
                    delayedDeath = true;
                    if (Creature* swayze = me->FindNearestCreature(NPC_SKY_CAPTAIN_SWAYZE, 100.0f))
                        swayze->AI()->Talk(TALK_TWILIGHT_SAPPER);

                    DoCast(me, SPELL_DETONATE, true);
                    me->DespawnOrUnsummon(5000);
                }
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!me->isAlive())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SMOKE_BOMB:
                        DoCast(me, SPELL_CLEAR_ALL_DEBUFFS, true);
                        DoCast(me, SPELL_SMOKE_BOMB, true);
                        break;
                    case EVENT_MOVE_TO_MACHINERY:
                        if (!me->HasAuraWithMechanic(1 << MECHANIC_STUN))
                            me->GetMotionMaster()->MovePoint(POINT_MACHINERY, 13474.548f, -12134.195f, 150.808f);
                        events.ScheduleEvent(EVENT_MOVE_TO_MACHINERY, 500);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        bool delayedDeath;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ds_sapperAI(creature);
    }
};

class DelayedKillEvent : public BasicEvent
{
public:
    DelayedKillEvent(Unit* owner) : _owner(owner) {}

    bool Execute(uint64 /*execTime*/, uint32 /*diff*/)
    {
        if (!_owner->ToCreature())
            return true;

        _owner->Kill(_owner);
        _owner->ToCreature()->DespawnOrUnsummon(5000);
        return true;
    }

private:
    Unit* _owner;
};

class spell_ds_detonate : public SpellScriptLoader
{
public:
    spell_ds_detonate() : SpellScriptLoader("spell_ds_detonate") {}

    class spell_ds_detonate_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_detonate_SpellScript);

        void HandleInstaKill(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex); // we need a instant kill delay because the spell visual is not shown on instant kill
            GetCaster()->m_Events.AddEvent(new DelayedKillEvent(GetCaster()), GetCaster()->m_Events.CalculateTime(1000));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_detonate_SpellScript::HandleInstaKill, EFFECT_2, SPELL_EFFECT_INSTAKILL);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_detonate_SpellScript();
    }
};

class spell_ds_skyfire_damage : public SpellScriptLoader
{
public:
    spell_ds_skyfire_damage() : SpellScriptLoader("spell_ds_skyfire_damage") { }

    class spell_ds_skyfire_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_skyfire_damage_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint32 barrageId = sSpellMgr->GetSpellIdForDifficulty(SPELL_TWILIGHT_BARRAGE_DAMAGE, GetCaster());
            if ((barrageId == GetSpellInfo()->Id) && !targets.empty())
                return;

            if (Creature* skyfire = GetCaster()->FindNearestCreature(NPC_THE_SKYFIRE, 500.0f))
                targets.push_back(skyfire);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_skyfire_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_skyfire_damage_SpellScript();
    }
};

class spell_ds_vengeance : public SpellScriptLoader
{
public:
    spell_ds_vengeance() : SpellScriptLoader("spell_ds_vengeance") { }

    class spell_ds_vengeance_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_vengeance_AuraScript);

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            int32 healthPct = std::max<int32>(1, int32(100 - round(GetTarget()->GetHealthPct())));
            if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_0))
                aurEff->SetAmount(healthPct);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_vengeance_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_vengeance_AuraScript();
    }
};

class spell_ds_blade_rush : public SpellScriptLoader
{
public:
    spell_ds_blade_rush() : SpellScriptLoader("spell_ds_blade_rush") {}

    class spell_ds_blade_rush_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_blade_rush_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Creature* startPosTrigger = NULL;
            Unit* caster = GetCaster();
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, GetCaster(), NPC_LIGHTNING_COSMETIC_TARGET, 80.0f);
            for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
            {
                if ((*itr)->ToTempSummon() && (*itr)->ToTempSummon()->GetSummonerGUID() == GetCaster()->GetGUID())
                {
                    startPosTrigger = (*itr);
                    break;
                }
            }

            if (startPosTrigger && caster)
            {
                targets.remove_if([startPosTrigger, caster](WorldObject* target)
                {
                    return !target->IsInBetween(startPosTrigger, caster, 3.0f);
                });
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_blade_rush_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_blade_rush_SpellScript();
    }
};

class spell_ds_broadside : public SpellScriptLoader
{
public:
    spell_ds_broadside() : SpellScriptLoader("spell_ds_broadside") {}

    class spell_ds_broadside_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_broadside_SpellScript);

        void HandleInstaKill(SpellEffIndex effIndex)
        {
            uint32 damage = GetHitUnit()->CountPctFromCurHealth(GetEffectValue());
            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_broadside_SpellScript::HandleInstaKill, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_broadside_SpellScript();
    }
};

class spell_ds_consuming_shroud : public SpellScriptLoader
{
public:
    spell_ds_consuming_shroud() : SpellScriptLoader("spell_ds_consuming_shroud") { }

    class spell_ds_consuming_shroud_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_consuming_shroud_AuraScript);

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
                return;

            uint32 remainingAmount = aurEff->GetAmount();

            GetTarget()->CastCustomSpell(SPELL_CONSUMING_SHROUD_DAMAGE, SPELLVALUE_BASE_POINT0, remainingAmount, GetTarget(), true);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectApplyFn(spell_ds_consuming_shroud_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_consuming_shroud_AuraScript();
    }

    class spell_ds_consuming_shroud_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_consuming_shroud_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(true, sSpellMgr->GetSpellIdForDifficulty(SPELL_CONSUMING_SHROUD, GetCaster())));
            targets.remove_if([](WorldObject* target){
                return target->ToPlayer() && target->ToPlayer()->HasTankSpec();
            });

            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_consuming_shroud_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_consuming_shroud_SpellScript();
    }
};

struct DeckFireTargetCheck : public Trinity::WorldObjectSpellAreaTargetCheck
{
    DeckFireTargetCheck(float range, Position const* position, Unit* caster, Unit* referer, SpellInfo const* spellInfo)
        : Trinity::WorldObjectSpellAreaTargetCheck(range, position, caster, referer, spellInfo, TARGET_CHECK_ALLY, nullptr)
    {}

    bool operator()(Unit* target)
    {
        if (!Trinity::WorldObjectSpellAreaTargetCheck::operator()(target))
            return false;

        return target->ToPlayer() && (target->HasAura(SPELL_DECK_FIRE_INITIAL_SPAWN) || target->HasAura(SPELL_DECK_FIRE));
    }
};

class spell_ds_deck_fire_periodic : public SpellScriptLoader
{
public:
    spell_ds_deck_fire_periodic() : SpellScriptLoader("spell_ds_deck_fire_periodic")
    {}

    class spell_ds_deck_fire_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_deck_fire_periodic_AuraScript);

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();
            std::list<Unit*> targets;
            Unit* me = GetTarget();
            DeckFireTargetCheck check(200.0f, me, me, me, GetSpellInfo());
            Trinity::UnitListSearcher<DeckFireTargetCheck> searcher(me, targets, check);
            me->VisitNearbyWorldObject(200.0f, searcher);

            for (Unit* target : targets)
                me->CastSpell(target, SPELL_DECK_FIRE_DAMAGE, true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_deck_fire_periodic_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_deck_fire_periodic_AuraScript();
    }
};


void AddSC_boss_warmaster_blackhorn()
{
    new boss_warmaster_blackhorn();
    new npc_ds_sky_captain_swayze();
    new npc_ds_skyfire_fire_brigade();
    new npc_ds_goriona();
    new npc_ds_twilight_assaulter_blackhorn();
    new npc_ds_skyfire_cannon();
    new npc_ds_twilight_elite();
    new npc_ds_twilight_infiltrator();
    new npc_ds_sapper();
    new spell_ds_detonate();
    new spell_ds_skyfire_damage();
    new spell_ds_vengeance();
    new spell_ds_blade_rush();
    new spell_ds_broadside();
    new spell_ds_consuming_shroud();
    new spell_ds_deck_fire_periodic();
};
