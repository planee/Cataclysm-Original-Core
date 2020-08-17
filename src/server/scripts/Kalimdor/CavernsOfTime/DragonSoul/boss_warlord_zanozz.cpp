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
    TALK_INTRO                          = -56754,
    TALK_INTRO_WHISPER                  = -56755,
    TALK_AGGRO                          = -56756,
    TALK_AGGRO_WHISPER                  = -56757,
    TALK_DISRUPTING_SHADOWS             = -56799,
    TALK_DISRUPTING_SHADOWS_2           = -56789,
    TALK_DISRUPTING_SHADOWS_3           = -56795,
    TALK_DISRUPTING_SHADOWS_WHISPER     = -56800, // we cant use text grouping here because all players should get the same yell + whisper text
    TALK_DISRUPTING_SHADOWS_WHISPER_2   = -56790,
    TALK_DISRUPTING_SHADOWS_WHISPER_3   = -56798,
    TALK_BLACK_BLOOD_OF_GORATH          = -56787,
    TALK_BLACK_BLOOD_OF_GORATH_WHISPER  = -56788,
    TALK_VOID_OF_THE_UNMAKING           = -56785,
    TALK_VOID_OF_THE_UNMAKING_WHISPER   = -56786,
    TALK_KILLING_A_PLAYER               = -56778,
    TALK_KILLING_A_PLAYER_2             = -56783,
    TALK_KILLING_A_PLAYER_3             = -56780,
    TALK_KILLING_A_PLAYER_WHISPER       = -56779,
    TALK_KILLING_A_PLAYER_WHISPER_2     = -56784,
    TALK_KILLING_A_PLAYER_WHISPER_3     = -56782,
    TALK_DEATH                          = -56764,
    TALK_DEATH_WHISPER                  = -56765
};

enum Spells
{
    // Zonozz
    SPELL_VOID_OF_THE_UNMAKING_SUMMON   = 103571,
    SPELL_VOID_OF_THE_UNMAKING_CHANNEL  = 103946,
    SPELL_FOCUSED_ANGER                 = 109409,
    SPELL_PSYCHIC_DRAIN                 = 104606,
    SPELL_DISRUPTING_SHADOWS            = 103434,
    SPELL_DISRUPTING_SHADOWS_DISPEL     = 103948,
    SPELL_BLACK_BLOOD_OF_GORATH         = 104378,
    SPELL_BLACK_BLOOD_OF_GORATH_HEROIC  = 104377,
    SPELL_TANTRUM                       = 103953,
    SPELL_DARKNESS                      = 109413,
    SPELL_BERSERK                       = 64238,

    // Void of the unmaking
    SPELL_VOID_OF_THE_UNMAKING_REMOVE   = 105336,
    SPELL_VOID_OF_THE_UNMAKING          = 103521,
    SPELL_VOID_OF_THE_UNMAKING_ACTIVE   = 109187,
    SPELL_VOID_DIFFUSION                = 104605,
    SPELL_VOID_DIFFUSION_DMG_INCREASE   = 106836,
    SPELL_VOID_DIFFUSION_ZONOZZ         = 104031,
    SPELL_BLACK_BLOOD_ERUPTION          = 108799,

    // Eye of Go'rath
    SPELL_EYE_OF_GORATH_SUMMON          = 109190, // unused on normal mode sniff... hc only ?
    SPELL_SHADOW_GAZE                   = 104347,

    // Claw of Go'rath
    SPELL_CLAW_OF_GORATH_SUMMON         = 109191, // unused on normal mode sniff... hc only ?
    SPELL_OOZE_SPIT                     = 109396,

    // Flail of Go'rath
    SPELL_FLAIL_OF_GORATH_SUMMON        = 109193, // unused on normal mode sniff... hc only ?
    SPELL_WILD_FLAIL                    = 109199,
    SPELL_SLUDGE_SPEW                   = 110102,
    SPELL_BLOOD_OF_GORATH_VISUAL        = 109361,
    SPELL_BLOOD_OF_GORATH_VISUAL_2      = 109352,

    // Dream Warden
    SPELL_EJECT_ALL_PASSENGERS          = 50630,
    SPELL_RIDE_DRAKE                    = 108582,
    SPELL_HALO_JUMP_SUMMON              = 108774,
    SPELL_HALO_JUMP_SUMMON_YORSAHJ      = 108764,
    SPELL_INCREASE_SPEED                = 105887,
    SPELL_FINAL_JUMP                    = 108689,
};

enum Events
{
    // Zonozz
    EVENT_VOID_OF_THE_UNMAKING_SUMMON   = 1,
    EVENT_PSYCHIC_DRAIN                 = 2,
    EVENT_DISRUPTING_SHADOWS            = 3,
    EVENT_FOCUSED_ANGER                 = 4,
    EVENT_BERSERK                       = 5,
    EVENT_BLACK_BLOOD_OF_GORATH         = 6,
    EVENT_BLACK_BLOOD_OF_GORATH_ENDS    = 7,
    EVENT_RESTART_MOVEMENT              = 8, // temp fix for some reason zonozz cant move anymore after getmotionmaster->movepoint call

    // Void of the unmaking
    EVENT_CHECK_NEAR_TARGET             = 9,
    EVENT_ENABLE_EXPLOSON               = 10,
    EVENT_CHANGE_MODEL                  = 11,
    EVENT_START_MOVEMENT                = 12,
    EVENT_CHANGE_DIRECTION              = 13,
    EVENT_LFR_CHANGE_DIRECTION          = 14,
    EVENT_LFR_MOVE_TO_ZONOZZ            = 15,
    EVENT_LFR_CHECK_ZONOZZ_DISTANCE     = 16,

    // Eye of Go'rath
    EVENT_SHADOW_GAZE                   = 17,

    // Flail of Go'rath
    EVENT_WILD_FLAIL                    = 18,
    EVENT_SLUDGE_SPEW                   = 19,

    // Dream Warden
    EVENT_START_NEXT_WAYPOINT           = 20
};

enum Misc
{
    POINT_BLOOD_OF_GORATH               = 1,
    POINT_WALL                          = 2,
    ACTION_START_BLOOD_PHASE            = 3,
    EVENT_GROUP_PHASE_NORMAL            = 4,
    ACTION_SUMMON_NEW_VOID              = 5,
    POINT_PATH_ONE                      = 6,
    POINT_PATH_TWO                      = 7,
    POINT_EJECT_PLAYER                  = 8
};

const Position EyeOfGorathPositions[] =
{
    { -1745.312f, -1846.073f, -221.357f, 4.405f },
    { -1701.801f, -1883.280f, -221.397f, 3.651f },
    { -1697.114f, -1940.587f, -221.384f, 2.855f },
    { -1734.128f, -1984.644f, -221.333f, 2.116f },
    { -1791.751f, -1989.054f, -221.355f, 1.511f },
    { -1835.634f, -1951.853f, -221.308f, 0.709f },
    { -1840.202f, -1894.214f, -221.307f, 0.022f },
    { -1802.477f, -1850.386f, -221.339f, 5.345f }
};

const Position FlailPositions[] =
{
    { -1765.915f, -1973.526f, -225.1207f, 1.658f },
    { -1804.931f, -1889.417f, -225.3907f, 5.745f },
    { -1731.457f, -1875.717f, -225.2117f, 4.118f },
    { -1802.752f, -1957.847f, -225.2931f, 0.879f }
};

const Position ClawPositions[] =
{
    { -1725.000f, -1914.870f, -226.2588f, 3.295f },
    { -1798.677f, -1926.582f, -226.1772f, 0.310f }
};

const Position WardenPositionsZonozz[3] =
{
    { -1790.790f, -2347.113f, 54.806f, 0.000f},
    { -1787.997f, -2265.523f, 65.543f, 0.000f},
    { -1758.500f, -1915.516f, 122.721f, 0.00f}
};

const Position WardenPositionsYorsahj[3] =
{
    { -1781.611f, -2443.896f, 64.4222f, 0.00f },
    { -1775.915f, -2590.288f, 70.5573f, 0.00f },
    { -1771.205f, -3040.021f, 152.960f, 0.00f }
};

const Position HaloJumpZonozz[4] =
{
    { -1759.109f, -1920.111f, 130.489f, 0.00f },
    { -1758.800f, -1919.160f, 130.489f, 0.00f },
    { -1769.748f, -1922.333f, -152.790f, 0.00f },
    { -1769.748f, -1922.333f, -152.790f, 0.00f }
};

const Position HaloJumpYorsahj[4] =
{
    { -1770.293f, -3038.962f, 155.693f, 0.00f },
    { -1771.210f, -3039.360f, 155.693f, 0.00f },
    { -1764.342f, -3033.661f, -123.726f, 0.00f },
    { -1764.342f, -3033.661f, -123.726f, 0.00f }
};

class boss_ds_warlord_zonozz : public CreatureScript
{
public:
    boss_ds_warlord_zonozz() : CreatureScript("boss_ds_warlord_zonozz") {}

    struct boss_ds_warlord_zonozzAI : public BossAI
    {
        boss_ds_warlord_zonozzAI(Creature* creature) : BossAI(creature, DATA_WARLORD_ZONOZZ) 
        {
            introDone = false;
        }

        void Reset() override
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_WYRMREST_TEMPLE, 200.00f))
                portal->SetVisible(true);
            _Reset();
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
            {
                if (!me->HasAura(SPELL_PRESENCE_OF_THE_DRAGON_SOUL))
                    me->CastSpell(me, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, true);
                me->SetLootMode(LOOT_MODE_LFR);
            }
            else if (instance->GetData(DATA_RAID_NERF) == RAID_NERF_ACTIVE && !me->HasAura(SPELL_PRESENCE_OF_THE_DRAGON_SOUL))
            {
                int32 currentNormalAndHcNerfAmount = -5;
                me->CastCustomSpell(me, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, &currentNormalAndHcNerfAmount, &currentNormalAndHcNerfAmount, NULL, true);
                DoCast(me, SPELL_POWER_OF_THE_ASPECTS, true);
            }
        }

        void JustSummoned(Creature* summon) override
        {
            switch (summon->GetEntry())
            {
                case NPC_VOID_OF_THE_UNMAKING:
                    DoCast(summon, SPELL_VOID_OF_THE_UNMAKING_CHANNEL);
                    break;
                default:
                    break;
            }
            BossAI::JustSummoned(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit* killer)
        {
            if (!IsHeroic())
                return;

            switch (summon->GetEntry())
            {
                case NPC_EYE_OF_GORATH:
                case NPC_FLAIL_OF_GORATH:
                case NPC_CLAW_OF_GORATH:
                    instance->DoRemoveAuraStackOnPlayers(SPELL_BLACK_BLOOD_OF_GORATH_HEROIC, me->GetGUID(), AURA_REMOVE_BY_DEFAULT);
                    break;
                default:
                    break;
            }
            BossAI::SummonedCreatureDies(summon, killer);
        }

        void Whisper(int32 id)
        {
            auto& players = me->GetMap()->GetPlayers();
            for (auto itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* target = itr->getSource())
                {
                    Talk(id, target->GetGUID(), CHAT_MSG_MONSTER_WHISPER, TEXT_RANGE_AREA);
                    break; // remove this after whisper is not longer raid wide
                }
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (!introDone && me->isAlive() && who->GetTypeId() == TYPEID_PLAYER)
            {
                if (me->GetDistance(who) <= 100.0f)
                {
                    introDone = true;
                    Talk(TALK_INTRO);
                    Whisper(TALK_INTRO_WHISPER);
                }
            }
        }

        void EnterCombat(Unit* victim) override
        {
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                me->SetLootMode(LOOT_MODE_LFR);

            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_WYRMREST_TEMPLE, 200.00f))
                portal->SetVisible(false);

            me->CallForHelp(100.0f);
            Talk(TALK_AGGRO);
            Whisper(TALK_AGGRO_WHISPER);
            events.ScheduleEvent(EVENT_VOID_OF_THE_UNMAKING_SUMMON, 5500, EVENT_GROUP_PHASE_NORMAL);
            events.ScheduleEvent(EVENT_FOCUSED_ANGER, 10500, EVENT_GROUP_PHASE_NORMAL);
            events.ScheduleEvent(EVENT_PSYCHIC_DRAIN, 13000, EVENT_GROUP_PHASE_NORMAL);
            events.ScheduleEvent(EVENT_DISRUPTING_SHADOWS, 25000, EVENT_GROUP_PHASE_NORMAL);
            events.ScheduleEvent(EVENT_BERSERK, 6 * IN_MILLISECONDS * MINUTE);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            _EnterCombat();
        }

        void JustDied(Unit* killer) override
        {
            Talk(TALK_DEATH);
            Whisper(TALK_DEATH_WHISPER);
            if (IsHeroic())
            {
                uint32 blackBlood = sSpellMgr->GetSpellIdForDifficulty(SPELL_BLACK_BLOOD_OF_GORATH_HEROIC, me);
                instance->DoRemoveAurasDueToSpellOnPlayers(blackBlood);
            }

            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_WYRMREST_TEMPLE, 200.00f))
                portal->SetVisible(true);
            _JustDied();
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
            {
                switch (urand(0, 2))
                {
                    case 0:
                        Talk(TALK_KILLING_A_PLAYER);
                        Whisper(TALK_KILLING_A_PLAYER);
                        break;
                    case 1:
                        Talk(TALK_KILLING_A_PLAYER_2);
                        Whisper(TALK_KILLING_A_PLAYER_2);
                        break;
                    case 2:
                        Talk(TALK_KILLING_A_PLAYER_3);
                        Whisper(TALK_KILLING_A_PLAYER_3);
                        break;
                }
            }
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_START_BLOOD_PHASE:
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

                    Talk(TALK_BLACK_BLOOD_OF_GORATH);
                    Whisper(TALK_BLACK_BLOOD_OF_GORATH_WHISPER);
                    events.CancelEventGroup(EVENT_GROUP_PHASE_NORMAL);
                    me->GetMotionMaster()->MovePoint(POINT_BLOOD_OF_GORATH, me->GetHomePosition());
                    break;
                case ACTION_SUMMON_NEW_VOID:
                    events.ScheduleEvent(EVENT_VOID_OF_THE_UNMAKING_SUMMON, 40000, EVENT_GROUP_PHASE_NORMAL);
                    break;
                default:
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == POINT_BLOOD_OF_GORATH)
                events.ScheduleEvent(EVENT_BLACK_BLOOD_OF_GORATH, 500);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_VOID_OF_THE_UNMAKING_SUMMON:
                        Talk(TALK_VOID_OF_THE_UNMAKING);
                        Whisper(TALK_VOID_OF_THE_UNMAKING_WHISPER);
                        DoCast(SPELL_VOID_OF_THE_UNMAKING_SUMMON);
                        break;
                    case EVENT_FOCUSED_ANGER:
                        DoCast(me, SPELL_FOCUSED_ANGER, true);
                        events.ScheduleEvent(EVENT_FOCUSED_ANGER, 6000, EVENT_GROUP_PHASE_NORMAL);
                        break;
                    case EVENT_PSYCHIC_DRAIN:
                        DoCast(SPELL_PSYCHIC_DRAIN);
                        events.ScheduleEvent(EVENT_PSYCHIC_DRAIN, 20000, EVENT_GROUP_PHASE_NORMAL);
                        break;
                    case EVENT_DISRUPTING_SHADOWS:
                        switch (urand(0, 2))
                        {
                            case 0:
                                Talk(TALK_DISRUPTING_SHADOWS);
                                Whisper(TALK_DISRUPTING_SHADOWS_WHISPER);
                                break;
                            case 1:
                                Talk(TALK_DISRUPTING_SHADOWS_2);
                                Whisper(TALK_DISRUPTING_SHADOWS_WHISPER_2);
                                break;
                            case 2:
                                Talk(TALK_DISRUPTING_SHADOWS_3);
                                Whisper(TALK_DISRUPTING_SHADOWS_WHISPER_3);
                                break;
                        }
                        DoCastRandom(SPELL_DISRUPTING_SHADOWS, 100.0f);
                        events.ScheduleEvent(EVENT_DISRUPTING_SHADOWS, 25000, EVENT_GROUP_PHASE_NORMAL);
                        break;
                    case EVENT_BLACK_BLOOD_OF_GORATH:
                    {
                        uint8 eyeCount = Is25ManRaid() ? 8 : 5;
                        uint8 ClawCount = Is25ManRaid() ? 2 : 1;
                        uint8 FlailCount = Is25ManRaid() ? 4 : 2;
                        uint32 blackBloodStacks = eyeCount + ClawCount + FlailCount;

                        for (uint8 i = 0; i < eyeCount; i++)
                            me->SummonCreature(NPC_EYE_OF_GORATH, EyeOfGorathPositions[i], TEMPSUMMON_MANUAL_DESPAWN);

                        for (uint8 i = 0; i < ClawCount; i++)
                            me->SummonCreature(NPC_CLAW_OF_GORATH, ClawPositions[i], TEMPSUMMON_MANUAL_DESPAWN);

                        for (uint8 i = 0; i < FlailCount; i++)
                            me->SummonCreature(NPC_FLAIL_OF_GORATH, FlailPositions[i], TEMPSUMMON_MANUAL_DESPAWN);

                        if (IsHeroic())
                            me->CastCustomSpell(SPELL_BLACK_BLOOD_OF_GORATH_HEROIC, SPELLVALUE_AURA_STACK, blackBloodStacks, (Unit*)NULL, TRIGGERED_FULL_MASK);
                        else
                            DoCastAOE(SPELL_BLACK_BLOOD_OF_GORATH, true);
                            
                        DoCast(me, SPELL_DARKNESS, true);
                        DoCast(me, SPELL_TANTRUM);

                        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);

                        events.ScheduleEvent(EVENT_RESTART_MOVEMENT, 10000); // temp fix - for some reason zonozz cant move anymore after getmotionmaster->movepoint call
                        events.ScheduleEvent(EVENT_BLACK_BLOOD_OF_GORATH_ENDS, 30000);
                        break;
                    }
                    case EVENT_RESTART_MOVEMENT:
                        if (me->getVictim())
                            DoStartMovement(me->getVictim());
                        break;
                    case EVENT_BLACK_BLOOD_OF_GORATH_ENDS:
                    {
                        uint32 voidOfTheUnmakingTimer = urand((IsHeroic() ? 15300 : 5300), 30000);
                        events.ScheduleEvent(EVENT_DISRUPTING_SHADOWS, 5500, EVENT_GROUP_PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_FOCUSED_ANGER, 6000, EVENT_GROUP_PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_VOID_OF_THE_UNMAKING_SUMMON, voidOfTheUnmakingTimer, EVENT_GROUP_PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_PSYCHIC_DRAIN, (voidOfTheUnmakingTimer + urand(3000, 10000)), EVENT_GROUP_PHASE_NORMAL);
                        if (!IsHeroic())
                        {
                            summons.DespawnEntry(NPC_EYE_OF_GORATH);
                            summons.DespawnEntry(NPC_CLAW_OF_GORATH);
                            summons.DespawnEntry(NPC_FLAIL_OF_GORATH);
                        }
                        break;
                    }
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
            bool introDone;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ds_warlord_zonozzAI(creature);
    }
};

class npc_ds_void_of_the_unmaking : public CreatureScript
{
public:
    npc_ds_void_of_the_unmaking() : CreatureScript("npc_ds_void_of_the_unmaking") {}

    struct npc_ds_void_of_the_unmakingAI : public ScriptedAI
    {
        npc_ds_void_of_the_unmakingAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, false);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* summoner)
        {
            me->SetControlled(true, UNIT_STATE_ROOT);
            canExplode = true;
            me->SetFacingToObject(summoner);

            if (instance->GetData(DATA_RAID_MODE) != RAID_MODE_LFR)
            {
                events.ScheduleEvent(EVENT_CHECK_NEAR_TARGET, 5000);
                events.ScheduleEvent(EVENT_START_MOVEMENT, 5000);
                events.ScheduleEvent(EVENT_CHANGE_MODEL, 2500);
            }
            else
            {
                events.ScheduleEvent(EVENT_LFR_CHANGE_DIRECTION, 10000);
                events.ScheduleEvent(EVENT_LFR_MOVE_TO_ZONOZZ, 55000);
                events.ScheduleEvent(EVENT_START_MOVEMENT, 5000);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == POINT_WALL)
            {
                DoCast(me, SPELL_BLACK_BLOOD_ERUPTION, true);
                if (Creature* zonozz = instance->GetCreature(DATA_WARLORD_ZONOZZ))
                    zonozz->AI()->DoAction(ACTION_SUMMON_NEW_VOID);
                me->DespawnOrUnsummon(3100);
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_CHECK_NEAR_TARGET:
                    {
                        std::list<WorldObject*> targetList;
                        Trinity::AllWorldObjectsInRange objects(me, 0.5f);
                        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects);
                        me->VisitNearbyObject(1.0f, searcher);
                        targetList.remove(me);
                        if (!targetList.empty())
                        {
                            targetList.sort(Trinity::ObjectDistanceOrderPred(me));
                            if (Unit* target = targetList.front()->ToUnit())
                            {
                                if (me->HasAura(SPELL_VOID_OF_THE_UNMAKING_ACTIVE))
                                {
                                    if (target->ToPlayer() && target->isAlive())
                                    {
                                        me->RemoveAurasDueToSpell(SPELL_VOID_OF_THE_UNMAKING_ACTIVE);
                                        DoCast(SPELL_VOID_DIFFUSION);
                                        me->GetMotionMaster()->Clear();
                                        //me->SetFacingToObject(target);
                                        float orientation = me->GetOrientation();
                                        me->SetFacingTo(frand(orientation - bounceDirection, orientation + bounceDirection));
                                        events.ScheduleEvent(EVENT_CHANGE_DIRECTION, 1000);
                                        events.ScheduleEvent(EVENT_ENABLE_EXPLOSON, 5000);
                                    }
                                    else if (target->GetEntry() == NPC_WARLORD_ZONOZZ)
                                    {
                                        me->RemoveAurasDueToSpell(SPELL_VOID_OF_THE_UNMAKING_ACTIVE);
                                        me->GetMotionMaster()->Clear();
                                        if (Creature* zonozz = instance->GetCreature(DATA_WARLORD_ZONOZZ))
                                        {
                                            if (Aura* aura = me->GetAura(SPELL_VOID_DIFFUSION_DMG_INCREASE))
                                            {
                                                uint32 stacks = aura->GetStackAmount();
                                                me->CastCustomSpell(SPELL_VOID_DIFFUSION_ZONOZZ, SPELLVALUE_AURA_STACK, stacks, zonozz, true);
                                            }
                                            uint32 focusedAnger = sSpellMgr->GetSpellIdForDifficulty(SPELL_FOCUSED_ANGER, me);
                                            zonozz->RemoveAurasDueToSpell(focusedAnger);
                                            zonozz->AI()->DoAction(ACTION_START_BLOOD_PHASE);
                                        }
                                        me->DespawnOrUnsummon(1000);
                                    }
                                }
                            }
                        }
                        events.ScheduleEvent(EVENT_CHECK_NEAR_TARGET, 300);
                        break;
                    }
                    case EVENT_ENABLE_EXPLOSON:
                        DoCast(me, SPELL_VOID_OF_THE_UNMAKING_ACTIVE, true);
                        break;
                    case EVENT_CHANGE_MODEL:
                        me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
                        break;
                    case EVENT_START_MOVEMENT:
                    {
                        me->SetControlled(false, UNIT_STATE_ROOT);
                        DoCast(me, SPELL_VOID_OF_THE_UNMAKING_ACTIVE, true);
                        Position pos(*me);
                        float angle = Position::NormalizeOrientation(M_PI);
                        me->MoveBlink(pos, 240.0f, angle);
                        me->GetMotionMaster()->MovePoint(POINT_WALL, pos);
                        break;
                    }
                    case EVENT_CHANGE_DIRECTION:
                    {
                        me->GetMotionMaster()->Clear();
                        Position pos(*me);
                        float angle = Position::NormalizeOrientation(M_PI);
                        me->MoveBlink(pos, 240.0f, angle);
                        me->GetMotionMaster()->MovePoint(POINT_WALL, pos);
                        break;
                    }
                    case EVENT_LFR_CHANGE_DIRECTION:
                    {
                        DoCast(SPELL_VOID_DIFFUSION);
                        me->GetMotionMaster()->Clear();
                        Position pos(*me);
                        float angle = Position::NormalizeOrientation(M_PI);
                        me->MoveBlink(pos, 240.0f, angle);
                        me->GetMotionMaster()->MovePoint(0, pos);
                        events.ScheduleEvent(EVENT_LFR_CHANGE_DIRECTION, 5000);
                        break;
                    }
                    case EVENT_LFR_MOVE_TO_ZONOZZ:
                        me->GetMotionMaster()->Clear();
                        if (Creature* zonozz = instance->GetCreature(DATA_WARLORD_ZONOZZ))
                            me->GetMotionMaster()->MoveChase(zonozz);
                        events.CancelEvent(EVENT_LFR_CHANGE_DIRECTION);
                        events.ScheduleEvent(EVENT_LFR_CHECK_ZONOZZ_DISTANCE, 500);
                        break;
                    case EVENT_LFR_CHECK_ZONOZZ_DISTANCE:
                    {
                        std::list<Creature*> creatures;
                        GetCreatureListWithEntryInGrid(creatures, me, NPC_WARLORD_ZONOZZ, 1.00f);
                        if (creatures.empty())
                        {
                            events.ScheduleEvent(EVENT_LFR_CHECK_ZONOZZ_DISTANCE, 500);
                            break;
                        }

                        if (Creature* zonozz = creatures.front())
                        {
                            me->RemoveAurasDueToSpell(SPELL_VOID_OF_THE_UNMAKING_ACTIVE);
                            me->GetMotionMaster()->Clear();
                            if (Aura* aura = me->GetAura(SPELL_VOID_DIFFUSION_DMG_INCREASE))
                            {
                                uint32 stacks = aura->GetStackAmount();
                                me->CastCustomSpell(SPELL_VOID_DIFFUSION_ZONOZZ, SPELLVALUE_AURA_STACK, stacks, zonozz, true);
                            }
                            uint32 focusedAnger = sSpellMgr->GetSpellIdForDifficulty(SPELL_FOCUSED_ANGER, me);
                            zonozz->RemoveAurasDueToSpell(focusedAnger);
                            zonozz->AI()->DoAction(ACTION_START_BLOOD_PHASE);
                            me->DespawnOrUnsummon(1000);
                        } 
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
        InstanceScript* instance;
        bool canExplode;
        float bounceDirection = 5 * M_PI / 180;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_void_of_the_unmakingAI(creature);
    }
};

class npc_ds_eye_of_gorath : public CreatureScript
{
public:
    npc_ds_eye_of_gorath() : CreatureScript("npc_ds_eye_of_gorath") {}

    struct npc_ds_eye_of_gorathAI : public Scripted_NoMovementAI
    {
        npc_ds_eye_of_gorathAI(Creature* creature) : Scripted_NoMovementAI(creature) 
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* summoner)
        {
            me->AddAura(SPELL_BLOOD_OF_GORATH_VISUAL, me);
            me->AddAura(SPELL_BLOOD_OF_GORATH_VISUAL_2, me);
            events.ScheduleEvent(EVENT_SHADOW_GAZE, 1000);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_SHADOW_GAZE:
                        DoCastRandom(SPELL_SHADOW_GAZE, 200.0f);
                        events.ScheduleEvent(EVENT_SHADOW_GAZE, 3000);
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
        return new npc_ds_eye_of_gorathAI(creature);
    }
};

class npc_ds_claw_of_gorath : public CreatureScript
{
public:
    npc_ds_claw_of_gorath() : CreatureScript("npc_ds_claw_of_gorath") {}

    struct npc_ds_claw_of_gorathAI : public Scripted_NoMovementAI
    {
        npc_ds_claw_of_gorathAI(Creature* creature) : Scripted_NoMovementAI(creature) 
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* /*summoner*/)
        {
            me->AddAura(SPELL_BLOOD_OF_GORATH_VISUAL, me);
            me->AddAura(SPELL_BLOOD_OF_GORATH_VISUAL_2, me);
        }

        void DoMeleeAttackIfReady()
        {
            if (me->getVictim())
            {
                if (me->isAttackReady() && !me->IsNonMeleeSpellCasted(false))
                {
                    if (me->IsWithinMeleeRange(me->getVictim()))
                    {
                        me->AttackerStateUpdate(me->getVictim());
                        me->resetAttackTimer();
                    }
                    else if (!me->HasUnitState(UNIT_STATE_CASTING))
                    {
                        me->resetAttackTimer();
                        DoCastVictim(SPELL_OOZE_SPIT);
                    }
                }
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim() || !IsHeroic())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_claw_of_gorathAI(creature);
    }
};

class npc_ds_flail_of_gorath : public CreatureScript
{
public:
    npc_ds_flail_of_gorath() : CreatureScript("npc_ds_flail_of_gorath") {}

    struct npc_ds_flail_of_gorathAI : public Scripted_NoMovementAI
    {
        npc_ds_flail_of_gorathAI(Creature* creature) : Scripted_NoMovementAI(creature) 
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* summoner)
        {
            if (IsHeroic())
            {
                events.ScheduleEvent(EVENT_WILD_FLAIL, 5000);
                events.ScheduleEvent(EVENT_SLUDGE_SPEW, 1500);
            }
            me->AddAura(SPELL_BLOOD_OF_GORATH_VISUAL, me);
            me->AddAura(SPELL_BLOOD_OF_GORATH_VISUAL_2, me);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_WILD_FLAIL:
                        DoCast(SPELL_WILD_FLAIL);
                        events.ScheduleEvent(EVENT_WILD_FLAIL, 5000);
                        break;
                    case EVENT_SLUDGE_SPEW:
                        DoCastRandom(SPELL_SLUDGE_SPEW, 200.0f);
                        events.ScheduleEvent(EVENT_SLUDGE_SPEW, 1500);
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
        return new npc_ds_flail_of_gorathAI(creature);
    }
};

class DelayedHaloJumpSummon : public BasicEvent
{
public:
    DelayedHaloJumpSummon(Unit* caster, bool yorsahjPath) : BasicEvent(), _caster(caster), _yorsahjPath(yorsahjPath) { }

    bool Execute(uint64 /*eventTime*/, uint32 /*diff*/)
    {
        if (_yorsahjPath)
            _caster->CastSpell(-1771.210f, -3039.360f, 155.610f, SPELL_HALO_JUMP_SUMMON_YORSAHJ, true);
        else
            _caster->CastSpell(-1758.800f, -1919.160f, 130.406f, SPELL_HALO_JUMP_SUMMON, true);
        return true;
    }

private:
    Unit* _caster;
    bool _yorsahjPath;
};

class npc_ds_warden_transport : public CreatureScript
{
public:
    npc_ds_warden_transport() : CreatureScript("npc_ds_warden_transport") { }

    struct npc_ds_warden_transportAI : public PassiveAI
    {
        npc_ds_warden_transportAI(Creature* creature) : PassiveAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            summoner->CastSpell(me, SPELL_RIDE_DRAKE, true);
        }

        void PassengerBoarded(Unit* player, int8 seatId, bool apply)
        {
            if (!apply)
            {
                me->DespawnOrUnsummon(500);
                player->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY);
                player->SendMovementCanFlyChange();
                player->m_Events.AddEvent(new DelayedHaloJumpSummon(player, me->GetEntry() == NPC_TIME_WARDEN), player->m_Events.CalculateTime(600));
            }
            else
            {
                me->GetMotionMaster()->MovePoint(POINT_PATH_ONE, me->GetEntry() == NPC_TIME_WARDEN ? WardenPositionsYorsahj[0] : WardenPositionsZonozz[0]);
            }
        }

        void MovementInform(uint32 type, uint32 point)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (point)
            {
                case POINT_PATH_ONE:
                case POINT_PATH_TWO:
                    events.ScheduleEvent(EVENT_START_NEXT_WAYPOINT, 50);
                    break;
                case POINT_EJECT_PLAYER:
                    DoCast(me, SPELL_EJECT_ALL_PASSENGERS);
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
                    case EVENT_START_NEXT_WAYPOINT:
                        if (firstWaypoint)
                        {
                            firstWaypoint = false;
                            me->GetMotionMaster()->MovePoint(POINT_PATH_TWO, me->GetEntry() == NPC_TIME_WARDEN ? WardenPositionsYorsahj[1] : WardenPositionsZonozz[1]);
                        }
                        else
                            me->GetMotionMaster()->MovePoint(POINT_EJECT_PLAYER, me->GetEntry() == NPC_TIME_WARDEN ? WardenPositionsYorsahj[2] : WardenPositionsZonozz[2]);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        EventMap events;
        bool firstWaypoint;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_warden_transportAI(creature);
    }
};

class DelayedHaloJump : public BasicEvent
{
public:
    DelayedHaloJump(Unit* caster, bool yorsahjPath) : BasicEvent(), _caster(caster), _yorsahjPath(yorsahjPath) { }

    bool Execute(uint64 /*eventTime*/, uint32 /*diff*/)
    {
        _caster->RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY);
        _caster->SendMovementCanFlyChange();
        _caster->AddAura(SPELL_FINAL_JUMP, _caster);
        if (_yorsahjPath)
            _caster->GetMotionMaster()->MoveJump(-1866.614f, -3075.333f, -177.170f, 30.0f, 15.0f);
        else
            _caster->GetMotionMaster()->MoveJump(-1735.804f, -1817.244f, -219.354f, 30.0f, 15.0f);
        return true;
    }

private:
    Unit* _caster;
    bool _yorsahjPath;
};

class npc_ds_halo_jump : public CreatureScript
{
public:
    npc_ds_halo_jump() : CreatureScript("npc_ds_halo_jump") { }

    struct npc_ds_halo_jumpAI : public PassiveAI
    {
        npc_ds_halo_jumpAI(Creature* creature) : PassiveAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            point = 0;
            summoner->CastSpell(me, SPELL_RIDE_DRAKE, true);
            me->SetDisableGravity(true);
            me->SetCanFly(true);
        }

        void PassengerBoarded(Unit* player, int8 seatId, bool apply)
        {
            if (!apply)
            {
                me->DespawnOrUnsummon(500);
                player->m_Events.AddEvent(new DelayedHaloJump(player, me->GetEntry() == NPC_HALO_JUMP_YORSAHJ), player->m_Events.CalculateTime(500));
            }
            else
            {
                me->GetMotionMaster()->MovePoint(point, me->GetEntry() == NPC_HALO_JUMP_YORSAHJ ? HaloJumpYorsahj[point] : HaloJumpZonozz[point]);
                point++;
            }
        }

        void MovementInform(uint32 type, uint32 point)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (point)
            {
                case 0:
                case 1:
                case 2:
                    events.ScheduleEvent(EVENT_START_NEXT_WAYPOINT, 50);
                    break;
                case 3:
                    DoCast(me, SPELL_EJECT_ALL_PASSENGERS);
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
                    case EVENT_START_NEXT_WAYPOINT:
                        me->GetMotionMaster()->MovePoint(point, me->GetEntry() == NPC_HALO_JUMP_YORSAHJ ? HaloJumpYorsahj[point] : HaloJumpZonozz[point]);
                        point++;
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        EventMap events;
        uint8 point;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_halo_jumpAI(creature);
    }
};

class spell_ds_disrupting_shadows : public SpellScriptLoader
{
public:
    spell_ds_disrupting_shadows() : SpellScriptLoader("spell_ds_disrupting_shadows") { }

    class spell_ds_disrupting_shadows_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_disrupting_shadows_AuraScript);

        void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL)
                if (InstanceScript* instance = GetTarget()->GetInstanceScript())
                    if (instance->GetData(DATA_RAID_MODE) != RAID_MODE_LFR)
                        GetTarget()->CastSpell(GetTarget(), SPELL_DISRUPTING_SHADOWS_DISPEL, true);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_ds_disrupting_shadows_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_disrupting_shadows_AuraScript();
    }
};

void AddSC_boss_warlord_zonozz()
{
    new boss_ds_warlord_zonozz();
    new npc_ds_void_of_the_unmaking();
    new npc_ds_eye_of_gorath();
    new npc_ds_claw_of_gorath();
    new npc_ds_flail_of_gorath();
    new npc_ds_warden_transport();
    new npc_ds_halo_jump();
    new spell_ds_disrupting_shadows();
};