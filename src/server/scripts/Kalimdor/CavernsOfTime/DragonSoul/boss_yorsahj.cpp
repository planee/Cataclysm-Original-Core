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

/*
@todo:
- visuals for globule spawn
*/

enum Spells
{
    SPELL_VOID_BOLT                 = 104849,
    SPELL_VOID_BOLT_AOE             = 105416,
    SPELL_DEEP_CORRUPTION           = 105171,
    SPELL_DEEP_CORRUPTION_TR        = 103628,
    SPELL_DEEP_CORRUPTION_EXPLOSION = 105173,
    SPELL_DEEP_CORRUPTION_TRASH     = 109390,
    SPELL_DEEP_CORRUPTION_TRASH_TR  = 109389,
    SPELL_DIGESTIVE_ACID            = 105031,
    SPELL_DIGESTIVE_ACID_VISUAL     = 105562,
    SPELL_SEARING_BLOOD             = 105033,
    SPELL_SUMMON_MANAVOID           = 105034,
    SPELL_CORRUPTED_MINIONS         = 105636, // summons Forgotten One
    SPELL_CORRUPTED_MINIONS_VIS     = 105634, // ressurects them
    SPELL_PSYCHIC_SLICE             = 105671,
    SPELL_MANA_VOID                 = 105530,
    SPELL_MANA_VOID_VIS             = 105527,
    SPELL_MANA_DIFFUSION            = 105539,

    SPELL_FUSING_VAPORS             = 103968,
    SPELL_FUSING_VAPORS_IMMUNITY    = 105904,

    // these spells are applied on yorashj
    SPELL_ACID_BLOOD                = 104898,
    SPELL_BLACK_BLOOD               = 104894,
    SPELL_COBALT_BLOOD              = 105027,
    SPELL_CRIMSON_BLOOD             = 104897,
    SPELL_GLOWING_BLOOD             = 104901,
    SPELL_SHADOWED_BLOOD            = 104896,

    SPELL_SUMMON_GLOBULE_1          = 105420,
    SPELL_SUMMON_GLOBULE_2          = 105435,
    SPELL_SUMMON_GLOBULE_3          = 105436,
    SPELL_SUMMON_GLOBULE_4          = 105437,
    SPELL_SUMMON_GLOBULE_5          = 105439,
    SPELL_SUMMON_GLOBULE_6          = 105440,
    SPELL_SUMMON_GLOBULE            = 105441,

    SPELL_VIS_BLUE                  = 105473,
    SPELL_VIS_RED                   = 105474,
    SPELL_VIS_GREEN                 = 105475,
    SPELL_VIS_YELLOW                = 105476,
    SPELL_VIS_PURPLE                = 105477,
    SPELL_VIS_BLACK                 = 105478,

    SPELL_BESTOW_BLOOD              = 103969, // Cast by Globule when reached Yorsahj
    SPELL_CRIT_REDUCE               = 64481,
    SPELL_FIXATE                    = 105695,

    SPELL_TRACK_ACHIEVEMENT         = 109326, // cast on yorsahj
    SPELL_GROUND_VISUAL_1           = 105600,
    SPELL_GROUND_VISUAL_2           = 105601,
    SPELL_GROUND_VISUAL_3           = 105603,
    SPELL_BERSERK                   = 26662,

    SPELL_SAY_INTRO                 = 109894,
    SPELL_SAY_AGGRO                 = 109895,
    SPELL_SAY_DEATH                 = 109896,
    SPELL_SAY_PLAYER_KILLED         = 109897,
    SPELL_SAY_ADD_SUMMONED            = 109898
};

enum Phases
{
    PHASE_NORMAL    = 0,
    PHASE_ADD       = 1
};

enum Actions
{
    ACTION_CHOISE_TARGET = 1,
};

enum Events
{
    EVENT_VOID_BOLT         = 1,
    EVENT_SUMMON_ADDS       = 2,
    EVENT_BERSERK           = 3,
    EVENT_SEARING_BLOOD     = 4,
    EVENT_DIGESTIVE_ACID    = 5,
    EVENT_DEEP_CORRUPTION   = 6,
    EVENT_MANA_VOID         = 7,
    EVENT_SUMMON_CORRUPTED  = 8,

    // the forgotten ones
    EVENT_PSYCHIC_SLICE     = 1,
    EVENT_ACTIVATE_MINION   = 2
};

struct TextGroups
{
    uint32 spell;
    std::vector<int32> whisper;
    std::vector<int32> yell;
};

// this cannot be done properly by groups since each yell has a certain whisper
static const TextGroups texts[5]
{
    { SPELL_SAY_INTRO,          { -56803 },                 { -56802 }},
    { SPELL_SAY_AGGRO,          { -56805 },                 { -56804 } },
    { SPELL_SAY_ADD_SUMMONED,   { -56819, -56817 },         { -56818, -56816 } },
    { SPELL_SAY_PLAYER_KILLED,  { -56815, -56812, -56810 }, { -56814, -56811, -56809 } },
    { SPELL_SAY_DEATH,          { -56808 },                 { -56806 } }
};

enum AchievementCombs
{
    AC_BLACK_YELLOW     = 0x1,
    AC_RED_GREEN        = 0x2,
    AC_BLACK_BLUE       = 0x4,
    AC_PURPLE_YELLOW    = 0x8
};

enum Points
{
    POINT_HOME_POS  = 1
};

Position const MiddlePos[1]
{
    { -1764.286f, -3036.903f, -182.4475f }
};

struct Globule
{
    uint32 entry;
    Position spawnPos;
    uint32 visualSpell;
};

static const Globule globule[6] =
{
    { BLUE_GLOBULE,     { -1722.604f, -3137.158f,-173.39f   },  SPELL_VIS_BLUE  },
    { RED_GLOBULE,      { -1662.964f, -2992.283f,-173.5141f },  SPELL_VIS_RED   },
    { GREEN_GLOBULE,    { -1723.757f, -2935.328f,-174.029f  },  SPELL_VIS_GREEN },
    { YELLOW_GLOBULE,   { -1863.986f, -2993.09f, -174.1103f },  SPELL_VIS_YELLOW},
    { VIOLETT_GLOBULE,  { -1663.894f, -3077.13f, -174.4792f },  SPELL_VIS_PURPLE},
    { BLACK_GLOBULE,    { -1808.226f, -3136.74f, -173.4804f },  SPELL_VIS_BLACK },
};

struct globuleColors
{
    uint32 entry1;
    uint32 entry2;
    uint32 entry3;
    uint32 entry4;
};

std::map<uint32, globuleColors> globuleActivation =
{
    { SPELL_SUMMON_GLOBULE_1, { VIOLETT_GLOBULE, GREEN_GLOBULE, BLUE_GLOBULE, BLACK_GLOBULE } },
    { SPELL_SUMMON_GLOBULE_2, { GREEN_GLOBULE, RED_GLOBULE, BLACK_GLOBULE, BLUE_GLOBULE } },
    { SPELL_SUMMON_GLOBULE_3, { GREEN_GLOBULE, RED_GLOBULE, YELLOW_GLOBULE, BLACK_GLOBULE } },
    { SPELL_SUMMON_GLOBULE_4, { VIOLETT_GLOBULE, BLUE_GLOBULE, YELLOW_GLOBULE, GREEN_GLOBULE } },
    { SPELL_SUMMON_GLOBULE_5, { BLUE_GLOBULE, BLACK_GLOBULE, YELLOW_GLOBULE, VIOLETT_GLOBULE } },
    { SPELL_SUMMON_GLOBULE_6, { VIOLETT_GLOBULE, BLACK_GLOBULE, RED_GLOBULE, YELLOW_GLOBULE } },
};

class boss_ds_yorsahj : public CreatureScript
{
public:
    boss_ds_yorsahj() : CreatureScript("boss_ds_yorsahj") {}

    struct boss_ds_yorsahjAI : public BossAI
    {
        boss_ds_yorsahjAI(Creature* creature) : BossAI(creature, DATA_YORSAHJ) {}

        void SendInitWorldStates()
        {
            instance->DoUpdateWorldState(WS_BLACK_YELLOW, 0);
            instance->DoUpdateWorldState(WS_RED_GREEN, 0);
            instance->DoUpdateWorldState(WS_BLACK_BLUE, 0);
            instance->DoUpdateWorldState(WS_PURPLE_YELLOW, 0);
        }

        void summonGlobules()
        {
            DoCast(SPELL_SAY_ADD_SUMMONED);

            for (Globule const& i : globule)
            {
                DoSummon(i.entry, i.spawnPos, 0, TEMPSUMMON_MANUAL_DESPAWN);
            }
        }

        void Reset() override
        {
            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_WYRMREST_TEMPLE, 200.00f))
                portal->SetVisible(true);
            me->RemoveAllAurasOnDeath();
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            RemoveEncounterFrame();
            SendInitWorldStates();
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
            summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            if (summon->GetEntry() == NPC_FORGOTTEN_ONE)
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FIXATE, summon->GetGUID());
        }

        void DoAction(int32 const actionId) override
        {
            if (events.IsInPhase(PHASE_ADD))
            {
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->GetMotionMaster()->MoveChase(me->getVictim());
                events.ScheduleEvent(EVENT_VOID_BOLT, 3000);
                events.SetPhase(PHASE_NORMAL);
            }

            switch (actionId)
            {
                case BLUE_GLOBULE: // done
                    DoCast(me, SPELL_COBALT_BLOOD, true);
                    events.ScheduleEvent(EVENT_MANA_VOID, 3000);
                    break;
                case RED_GLOBULE: // done
                    DoCast(me, SPELL_CRIMSON_BLOOD, true);
                    events.ScheduleEvent(EVENT_SEARING_BLOOD, 4000);
                    break;
                case GREEN_GLOBULE: // done
                    DoCast(me, SPELL_ACID_BLOOD, true);
                    DoCast(SPELL_DIGESTIVE_ACID_VISUAL);
                    events.ScheduleEvent(EVENT_DIGESTIVE_ACID, 8300);
                    break;
                case BLACK_GLOBULE: // done
                    DoCast(me, SPELL_BLACK_BLOOD, true);
                    DoCast(SPELL_GROUND_VISUAL_1);
                    DoCast(SPELL_GROUND_VISUAL_2);
                    DoCast(SPELL_GROUND_VISUAL_3);
                    events.ScheduleEvent(EVENT_SUMMON_CORRUPTED, 5000);
                    break;
                case YELLOW_GLOBULE: // done
                    DoCast(me, SPELL_GLOWING_BLOOD, true);
                    break;
                case VIOLETT_GLOBULE: // done
                    DoCast(me, SPELL_SHADOWED_BLOOD, true);
                    events.ScheduleEvent(EVENT_DEEP_CORRUPTION, 4000);
                    break;
                default:
                    break;
            }
            if (AuraEffect* aura = me->GetAuraEffect(SPELL_TRACK_ACHIEVEMENT, EFFECT_0))
            {
                aura->SetCanBeRecalculated(true);
                if (me->HasAura(SPELL_GLOWING_BLOOD) && me->HasAura(SPELL_BLACK_BLOOD))
                {
                    instance->DoUpdateWorldState(WS_BLACK_YELLOW, 1);
                    aura->SetAmount(aura->GetAmount() | AC_BLACK_YELLOW);
                }
                if (me->HasAura(SPELL_CRIMSON_BLOOD) && me->HasAura(SPELL_ACID_BLOOD))
                {
                    instance->DoUpdateWorldState(WS_RED_GREEN, 1);
                    aura->SetAmount(aura->GetAmount() | AC_RED_GREEN);
                }
                if (me->HasAura(SPELL_BLACK_BLOOD) && me->HasAura(SPELL_COBALT_BLOOD))
                {
                    instance->DoUpdateWorldState(WS_BLACK_BLUE, 1);
                    aura->SetAmount(aura->GetAmount() | AC_BLACK_BLUE);
                }
                if (me->HasAura(SPELL_SHADOWED_BLOOD) && me->HasAura(SPELL_GLOWING_BLOOD))
                {
                    instance->DoUpdateWorldState(WS_PURPLE_YELLOW, 1);
                    aura->SetAmount(aura->GetAmount() | AC_PURPLE_YELLOW);
                }
            }
        }

        void MovementInform(uint32 movementType, uint32 pointId) override
        {
            if (movementType != POINT_MOTION_TYPE)
                return;

            if (pointId == POINT_HOME_POS)
            {
                events.CancelEvent(EVENT_VOID_BOLT);
                events.SetPhase(PHASE_ADD);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                DoCast(RAND(SPELL_SUMMON_GLOBULE_1, SPELL_SUMMON_GLOBULE_2, SPELL_SUMMON_GLOBULE_3, SPELL_SUMMON_GLOBULE_4, SPELL_SUMMON_GLOBULE_5, SPELL_SUMMON_GLOBULE_6));
                events.ScheduleEvent(EVENT_SUMMON_ADDS, 90000);
            }
        }

        void EnterCombat(Unit* victim) override
        {
            if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR)
                me->SetLootMode(LOOT_MODE_LFR);

            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_WYRMREST_TEMPLE, 200.00f))
                portal->SetVisible(false);

            me->CallForHelp(100.0f);
            DoCast(SPELL_SAY_AGGRO);
            DoCast(SPELL_TRACK_ACHIEVEMENT);
            events.ScheduleEvent(EVENT_VOID_BOLT, 6000);
            events.ScheduleEvent(EVENT_SUMMON_ADDS, 22000);
            events.ScheduleEvent(EVENT_BERSERK, 600000);
            AddEncounterFrame();
            _EnterCombat();
        }

        void JustDied(Unit* killer) override
        {
            if (Creature* portal = me->FindNearestCreature(NPC_TRAVEL_TO_WYRMREST_TEMPLE, 200.00f))
                portal->SetVisible(true);

            RemoveEncounterFrame();
            DoCast(SPELL_SAY_DEATH);
            _JustDied();
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
            {
                DoCast(SPELL_SAY_PLAYER_KILLED);
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_VOID_BOLT:
                    {
                        if (me->HasAura(SPELL_GLOWING_BLOOD))
                        {
                            DoCast(SPELL_VOID_BOLT_AOE);
                            events.ScheduleEvent(EVENT_VOID_BOLT, 3000);
                        }
                        else
                        {
                            DoCastVictim(SPELL_VOID_BOLT);
                            events.ScheduleEvent(EVENT_VOID_BOLT, 6000);
                        }
                        break;
                    }
                    case EVENT_SUMMON_ADDS:
                    {
                        for (Globule const& i : globule)
                        {
                            summons.DespawnEntry(i.entry);
                        }
                        summonGlobules();
                        me->GetMotionMaster()->MovePoint(POINT_HOME_POS, me->GetHomePosition());
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        break;
                    }
                    case EVENT_SEARING_BLOOD:
                    {
                        DoCast(SPELL_SEARING_BLOOD);
                        if (Aura* aura = me->GetAura(SPELL_CRIMSON_BLOOD))
                            if (aura->GetDuration() > 6000)
                                events.ScheduleEvent(EVENT_SEARING_BLOOD, 6000);
                        break;
                    }
                    case EVENT_DIGESTIVE_ACID:
                    {
                        DoCast(SPELL_DIGESTIVE_ACID);
                        if (Aura* aura = me->GetAura(SPELL_ACID_BLOOD))
                            if (aura->GetDuration() > 12000)
                                events.ScheduleEvent(EVENT_DIGESTIVE_ACID, 8300);
                        break;
                    }
                    case EVENT_DEEP_CORRUPTION:
                    {
                        DoCast(SPELL_DEEP_CORRUPTION);
                        if (Aura* aura = me->GetAura(SPELL_SHADOWED_BLOOD))
                            if (aura->GetDuration() > 25000)
                                events.ScheduleEvent(EVENT_DEEP_CORRUPTION, 30000);
                        break;
                    }
                    case EVENT_MANA_VOID:
                    {
                        DoCast(SPELL_SUMMON_MANAVOID);
                        break;
                    }
                    case EVENT_SUMMON_CORRUPTED:
                    {
                        DoCast(SPELL_CORRUPTED_MINIONS);
                        if (Aura* aura = me->GetAura(SPELL_BLACK_BLOOD))
                            if (aura->GetDuration() > 30000)
                                events.ScheduleEvent(EVENT_SUMMON_CORRUPTED, 30000);
                        break;
                    }
                    case EVENT_BERSERK:
                    {
                        DoCast(SPELL_BERSERK);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ds_yorsahjAI(creature);
    }
};

class npc_ds_globule : public CreatureScript
{
public:
    npc_ds_globule() : CreatureScript("npc_ds_globule") { }

    struct npc_ds_globuleAI : public ScriptedAI
    {
        npc_ds_globuleAI(Creature* creature) : ScriptedAI(creature)
        {
            _instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            DoCast(me, SPELL_FUSING_VAPORS);
            me->SetDisplayId(15435); // invisible
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void SpellHit(Unit* caster, SpellInfo const* spellInfo)
        {
            if (spellInfo->Id == SPELL_FUSING_VAPORS_IMMUNITY)
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void MovementInform(uint32 movementType, uint32 pointId) override
        {
            if (movementType != POINT_MOTION_TYPE)
                return;

            if (pointId == POINT_HOME_POS)
            {
                if (Creature* yorsahj = _instance->GetCreature(DATA_YORSAHJ))
                    yorsahj->AI()->DoAction(me->GetEntry());
                DoCast(SPELL_BESTOW_BLOOD);
                RemoveEncounterFrame();
                me->DespawnOrUnsummon(2000);
            }
        }

        void JustDied(Unit* killer)
        {
            DoCast(me, SPELL_FUSING_VAPORS_IMMUNITY, true);
            RemoveEncounterFrame();
            me->DespawnOrUnsummon(2000);
        }

        void DoAction(int32 const /*type*/)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            AddEncounterFrame();
            events.ScheduleEvent(EVENT_ACTIVATE_MINION, 4000);
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                if (eventid == EVENT_ACTIVATE_MINION)
                    me->GetMotionMaster()->MovePoint(POINT_HOME_POS, MiddlePos[0]);
            }
        }

    private:
        InstanceScript* _instance;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ds_globuleAI(creature);
    }
};

class ForgottenOneActivation : public BasicEvent
{
public:
    ForgottenOneActivation(Creature* owner) : _summon(owner) {}

    bool Execute(uint64 /*execTime*/, uint32 /*diff*/)
    {
        _summon->SetReactState(REACT_AGGRESSIVE);
        _summon->SetInCombatWithZone();
        /// @hack: SPELL_FIXATE should do all this magic
        if (Unit* target = _summon->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true, -SPELL_FIXATE))
        {
            _summon->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            _summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
            _summon->getThreatManager().tauntApply(target);
            _summon->GetMotionMaster()->MoveChase(target);
            _summon->AddAura(SPELL_FIXATE, target); // for dbm
            _summon->AI()->SetGUID(target->GetGUID(), ACTION_CHOISE_TARGET);
            _summon->GetMotionMaster()->MoveChase(target);
            _summon->AI()->AttackStart(target);
            _summon->AddThreat(target, 50000000.0f);
        }
        return false;
    }

private:
    Creature* _summon;
};

class npc_ds_chosen_one : public CreatureScript
{
public:
    npc_ds_chosen_one() : CreatureScript("npc_ds_chosen_one") { }

    struct npc_ds_chosen_oneAI : public ScriptedAI
    {
        npc_ds_chosen_oneAI(Creature* creature) : ScriptedAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            _targetGUID = 0;
            me->SetReactState(REACT_PASSIVE);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
            me->CastSpell(me, SPELL_CORRUPTED_MINIONS_VIS);
            me->CastSpell(me, SPELL_CRIT_REDUCE);
            me->m_Events.AddEvent(new ForgottenOneActivation(me), me->m_Events.CalculateTime(3000));
            _events.ScheduleEvent(EVENT_PSYCHIC_SLICE, 9000);
        }

        void JustDied(Unit* killer) override
        {
            if (Unit *player = Unit::GetUnit(*me, _targetGUID))
                player->RemoveAurasDueToSpell(SPELL_FIXATE, me->GetGUID());
            me->DespawnOrUnsummon();
        }

        void SetGUID(uint64 guid, int32 id)
        {
            if (id == ACTION_CHOISE_TARGET)
                _targetGUID = guid;
        }

        void UpdateAI(uint32 const diff) override
        {
            _events.Update(diff);

            if (!UpdateVictim())
                return;

            while (uint32 eventid = _events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_PSYCHIC_SLICE:
                    {
                        DoCastVictim(SPELL_PSYCHIC_SLICE);
                        _events.ScheduleEvent(EVENT_PSYCHIC_SLICE, 10000);
                        break;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        uint64 _targetGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ds_chosen_oneAI(creature);
    }
};

class npc_ds_mana_void : public CreatureScript
{
public:
    npc_ds_mana_void() : CreatureScript("npc_ds_mana_void") { }

    struct npc_ds_mana_voidAI : public ScriptedAI
    {
        npc_ds_mana_voidAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetDisplayId(me->GetCreatureTemplate()->Modelid1);
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        }

        bool CanAIAttack(Unit const* victim) const override
        {
            return false;
        }

        void JustDied(Unit* killer) override
        {
            DoCast(SPELL_MANA_DIFFUSION);
            me->DespawnOrUnsummon(1000);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (Creature* yorsahj = instance->GetCreature(DATA_YORSAHJ))
                yorsahj->AI()->JustSummoned(me);

            manaLeeched = 0;
            me->CastSpell(me, SPELL_MANA_VOID, true);
            me->CastSpell(me, SPELL_MANA_VOID_VIS, true);
            me->SetReactState(REACT_PASSIVE);
            events.ScheduleEvent(EVENT_ACTIVATE_MINION, 4000);
        }

        void SetData(uint32 type, uint32 value)
        {
            if (type == DATA_RESTORED_MANA)
                manaLeeched += value;
        }

        uint32 GetData(uint32 data) const
        {
            if (data == DATA_RESTORED_MANA)
                return manaLeeched;
            return 0;
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type != EFFECT_MOTION_TYPE)
                return;

            switch (pointId)
            {
                case 1:
                    me->SetHomePosition(*me);
                    me->GetMotionMaster()->MoveRandom(20.0f);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                if (eventid == EVENT_ACTIVATE_MINION)
                {
                    me->GetMotionMaster()->MoveFall(1);
                    me->SetCanFly(false);
                    me->SetDisableGravity(false);
                }
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        uint32 manaLeeched;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ds_mana_voidAI(creature);
    }
};

// 105435, 105436, 105437, 105439, 105440, 105441
class spell_ds_summon_globule : public SpellScriptLoader
{
public:
    spell_ds_summon_globule() : SpellScriptLoader("spell_ds_summon_globule") { }

    class spell_ds_summon_globule_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_summon_globule_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint32 spell = GetSpellInfo()->Id;
            targets.remove_if([spell](WorldObject* target) -> bool
            {
                return target->GetEntry() != globuleActivation[spell].entry1 && target->GetEntry() != globuleActivation[spell].entry2 && target->GetEntry() != globuleActivation[spell].entry3 && target->GetEntry() != globuleActivation[spell].entry4;
            });
            targets.remove_if([spell](WorldObject* target) -> bool
            {
                return !target->GetMap()->IsHeroic() && target->GetEntry() == globuleActivation[spell].entry4;
            });
        }

        void HandleVisual(SpellEffIndex effIndex)
        {
            uint32 spell = 0;
            for (Globule const& i : globule)
            {
                if (i.entry == GetHitUnit()->GetEntry())
                {
                    GetHitUnit()->CastSpell(GetHitUnit(), i.visualSpell, true);
                    break;
                }
            }
        }

        void HandleActivation(SpellEffIndex /*effIndex*/)
        {
            if (Creature* target = GetHitCreature())
            {
                target->SetDisplayId(target->GetCreatureTemplate()->Modelid1);
                target->AI()->DoAction(EVENT_ACTIVATE_MINION);
            }
        }

        void HandleText(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster()->GetMap()->IsHeroic())
                GetCaster()->ToCreature()->AI()->Talk(-(GetSpellInfo()->Effects[EFFECT_0].MiscValue), 0, CHAT_MSG_RAID_BOSS_EMOTE);
            else
                GetCaster()->ToCreature()->AI()->Talk(-(GetSpellInfo()->Effects[EFFECT_1].BasePoints), 0, CHAT_MSG_RAID_BOSS_EMOTE);
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(spell_ds_summon_globule_SpellScript::HandleText, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            OnEffectHitTarget += SpellEffectFn(spell_ds_summon_globule_SpellScript::HandleActivation, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            OnEffectLaunchTarget += SpellEffectFn(spell_ds_summon_globule_SpellScript::HandleVisual, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_summon_globule_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_summon_globule_SpellScript();
    }
};

// 103628 109389
class spell_ds_deep_corruption : public SpellScriptLoader
{
public:
    spell_ds_deep_corruption() : SpellScriptLoader("spell_ds_deep_corruption") { }

    class spell_ds_deep_corruption_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_deep_corruption_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DEEP_CORRUPTION_EXPLOSION) ||
                !sSpellMgr->GetSpellInfo(SPELL_DEEP_CORRUPTION_TRASH))
                return false;

            return true;
        }

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (GetAura()->GetStackAmount() == GetSpellInfo()->Effects[EFFECT_1].BasePoints)
            {
                if (Unit* owner = GetOwner()->ToUnit())
                {
                    owner->CastSpell(owner, GetSpellInfo()->Id == SPELL_DEEP_CORRUPTION_TRASH_TR ? SPELL_DEEP_CORRUPTION_TRASH : SPELL_DEEP_CORRUPTION_EXPLOSION, true);
                    Remove(AURA_REMOVE_BY_DEFAULT);
                }
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ds_deep_corruption_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_deep_corruption_AuraScript();
    }
};

// 105539
class spell_ds_mana_diffusion : public SpellScriptLoader
{
public:
    spell_ds_mana_diffusion() : SpellScriptLoader("spell_ds_mana_diffusion") { }

    class spell_ds_mana_diffusion_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_mana_diffusion_SpellScript);

        bool Load()
        {
            _targets = 0;
            return GetCaster()->ToCreature();
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            _targets = targets.size();
        }

        void HandleScript(SpellEffIndex effIndex)
        {
            uint32 mana = GetCaster()->ToCreature()->AI()->GetData(DATA_RESTORED_MANA);
            SetEffectValue(mana / _targets);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_mana_diffusion_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_ENERGIZE);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_mana_diffusion_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }

    private:
        uint8 _targets;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_mana_diffusion_SpellScript();
    }
};

// 105033
class spell_ds_searing_blood : public SpellScriptLoader
{
public:
    spell_ds_searing_blood() : SpellScriptLoader("spell_ds_searing_blood") { }

    class spell_ds_searing_blood_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_searing_blood_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            targets.remove_if([caster](WorldObject* target)
            {
                return target->GetDistance2d(caster) > 60.0f;
            });
            targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster(), false));
            uint8 targetcnt = GetCaster()->GetMap()->Is25ManRaid() ? 8 : 3;
            if (targets.size() > targetcnt)
                targets.resize(targetcnt);
        }

        void HandleScript(SpellEffIndex effIndex)
        {
            uint32 damage = GetHitDamage();
            // guessed value
            SetHitDamage(AddPct(damage, GetHitUnit()->GetDistance2d(GetCaster()) * 5));
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_searing_blood_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_searing_blood_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_searing_blood_SpellScript();
    }
};

// 103968
class spell_ds_fusing_vapors : public SpellScriptLoader
{
public:
    spell_ds_fusing_vapors() : SpellScriptLoader("spell_ds_fusing_vapors") { }

    class spell_ds_fusing_vapors_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_fusing_vapors_AuraScript);

        bool Load()
        {
            wasTriggered = false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetActor()->HealthBelowPct(50) && !wasTriggered)
                return true;
            return false;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            wasTriggered = true;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_ds_fusing_vapors_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_ds_fusing_vapors_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    private:
        bool wasTriggered;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_fusing_vapors_AuraScript();
    }
};

// 103635
class spell_ds_fusing_vapors_heal : public SpellScriptLoader
{
public:
    spell_ds_fusing_vapors_heal() : SpellScriptLoader("spell_ds_fusing_vapors_heal") { }

    class spell_ds_fusing_vapors_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_fusing_vapors_heal_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove(GetCaster());
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_fusing_vapors_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_fusing_vapors_heal_SpellScript();
    }
};

// 105571
class spell_ds_digestive_acid : public SpellScriptLoader
{
public:
    spell_ds_digestive_acid() : SpellScriptLoader("spell_ds_digestive_acid") { }

    class spell_ds_digestive_acid_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_digestive_acid_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, GetCaster()->GetMap()->Is25ManRaid() ? 15 : 5);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->CastSpell(GetHitUnit(), GetSpellInfo()->Effects[EFFECT_0].BasePoints, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_digestive_acid_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_digestive_acid_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_digestive_acid_SpellScript();
    }
};

// 105034
class spell_ds_summon_manavoid : public SpellScriptLoader
{
public:
    spell_ds_summon_manavoid() : SpellScriptLoader("spell_ds_summon_manavoid") { }

    class spell_ds_summon_manavoid_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_summon_manavoid_SpellScript);

        void ChangeSummonPos(SpellEffIndex /*effIndex*/)
        {
            // Adjust effect summon position to lower Z
            WorldLocation summonPos = *GetExplTargetDest();
            Position offset = { 0.0f, 0.0f, 25.0f, 0.0f };
            summonPos.RelocateOffset(offset);
            SetExplTargetDest(summonPos);
            GetHitDest()->RelocateOffset(offset);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_ds_summon_manavoid_SpellScript::ChangeSummonPos, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_summon_manavoid_SpellScript();
    }
};

// 109894,109895,109896,109897,109898
class spell_ds_yorsahj_text : public SpellScriptLoader
{
public:
    spell_ds_yorsahj_text() : SpellScriptLoader("spell_ds_yorsahj_text") { }

    class spell_ds_yorsahj_text_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_yorsahj_text_SpellScript);

        void HandleYell(SpellEffIndex effIndex)
        {
            for (TextGroups const& i : texts)
            {
                if (i.spell == GetSpellInfo()->Id)
                {
                    selection = urand(0, i.yell.size() - 1);
                    GetCaster()->ToCreature()->AI()->Talk(i.yell[selection]);
                    break;
                }
            }
        }

        void HandleWhisper(SpellEffIndex effIndex)
        {
            for (TextGroups const& i : texts)
            {
                if (i.spell == GetSpellInfo()->Id)
                {
                    GetCaster()->ToCreature()->AI()->Talk(i.whisper[selection], GetHitUnit()->GetGUID(), CHAT_MSG_MONSTER_WHISPER);
                    break;
                }
            }
        }

        void Register() override
        {
            OnEffectLaunch += SpellEffectFn(spell_ds_yorsahj_text_SpellScript::HandleYell, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            OnEffectHitTarget += SpellEffectFn(spell_ds_yorsahj_text_SpellScript::HandleWhisper, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    private:
        uint8 selection;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_yorsahj_text_SpellScript();
    }
};

class achievement_taste_the_rainbow : public AchievementCriteriaScript
{
public:
    achievement_taste_the_rainbow(char const* scriptName, uint8 combination) : AchievementCriteriaScript(scriptName), _combination(combination) {}

    bool OnCheck(Player* source, Unit* target)
    {
        if (!target)
            return false;

        uint32 amount = 0;
        if (AuraEffect* aura = target->GetAuraEffect(SPELL_TRACK_ACHIEVEMENT, EFFECT_0))
            return (uint8(aura->GetAmount()) & _combination);

        return false;
    }
private:
    uint8 _combination;
};

void AddSC_boss_yorsahj()
{
    new boss_ds_yorsahj();
    new npc_ds_globule();
    new npc_ds_chosen_one();
    new npc_ds_mana_void();
    new spell_ds_summon_globule();
    new spell_ds_deep_corruption();
    new spell_ds_mana_diffusion();
    new spell_ds_searing_blood();
    new spell_ds_fusing_vapors();
    new spell_ds_fusing_vapors_heal();
    new spell_ds_digestive_acid();
    new spell_ds_summon_manavoid();
    new spell_ds_yorsahj_text();
    new achievement_taste_the_rainbow("achievement_taste_the_rainbow_1", AC_BLACK_YELLOW);
    new achievement_taste_the_rainbow("achievement_taste_the_rainbow_2", AC_RED_GREEN);
    new achievement_taste_the_rainbow("achievement_taste_the_rainbow_3", AC_BLACK_BLUE);
    new achievement_taste_the_rainbow("achievement_taste_the_rainbow_4", AC_PURPLE_YELLOW);
};
