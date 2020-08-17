/*
 * Copyright (C) 2010-2017 OMFG.GG <http://www.omfg.gg/>
 * Copyright (C) 2017-2017 WoWZealot <http://www.wowzealot.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "deadmines.h"

enum Spells
{
    SPELL_ACTIVATE                  = 89198,
    SPELL_ACTIVATE_2                = 89121,
    SPELL_RED_EYES                  = 24263,
    SPELL_ENERGIZE_2                = 91846, // hit 45979 or boss
    // All Modes
    SPELL_OFF_LINE                  = 88348,
    SPELL_OVERDRIVE                 = 88481,
    SPELL_HARVEST                   = 88495,
    SPELL_HARVEST_READY             = 88508,
    SPELL_HARVEST_2                 = 88497,
    SPELL_HARVEST_SWEEP             = 88521,
    SPELL_REAPER_STRIKE             = 88490,
    SPELL_SAFETY_RESTRICTIONS       = 88522,

    // heroic
    SPELL_SUMMON_SCORIES            = 91839,
    SPELL_SCORIES_SHIELD            = 91815,

    // scoried
    SPELL_FIXATE                    = 91830,

    // Defias Reaper
    SPELL_ON_FIRE                   = 91737,
    SPELL_ENERGIZE                  = 89132,
    SPELL_EXPLODE_SUICIDE           = 91738,
    SPELL_RIDE_VEHICLE              = 86826,
    SPELL_CLEAVE                    = 90980,

    // Fire bunny
    SPELL_ELECTRIC_CHARGE           = 89202,
};

enum Events
{
    EVENT_OVERDRIVE                 = 1,
    EVENT_OVERDRIVE_MOVEMENTS,
    EVENT_OVERDRIVE_END,
    EVENT_HARVEST,
    EVENT_HARVEST_SWEEP,
    EVENT_HARVEST_MOVEMENTS,
    EVENT_SCORIES,

    EVENT_INTRO,
    EVENT_INTRO_FINAL,

    // scories
    EVENT_FIXATE,
    EVENT_FIXATE_CHECK,

    // Defias Reaper
    EVENT_CLEAVE
};

enum Texts
{
    SAY_AGGRO                       = 0,
    EMOTE_OVERDRIVE                 = 1,
    SAY_OVERDRIVE                   = 2,
    SAY_HARVEST                     = 3,
    EMOTE_HARVEST                   = 4,
    SAY_SAFETY                      = 5,
    SAY_DEATH                       = 6,
    SAY_KILLER                      = 7,
};

enum npcs
{
    NPC_REAPER_DEFIAS               = 47403,
    NPC_FRIENDLY_PROTOTYPE          = 49208,
    NPC_HARVEST_DUMMY               = 47468,
    NPC_SCORIES                     = 49229,
};

enum points
{
    POINT_HARVEST = 1,
};

enum Phases
{
    PHASE_INTRO = 2,
    PHASE_FIGHT,
};

Position const reaperSummonPos[4] =
{
    { -182.74f, -565.97f, 19.31f, 3.35f },
    { -205.53f, -552.74f, 19.31f, 4.54f },
    { -228.67f, -565.75f, 19.31f, 5.98f },
    { -229.72f, -590.37f, 19.31f, 0.71f }
};

Position const scorpiesSummonPos[4] =
{
    { -205.56f, -572.14f, 20.98f, 0.00f },
    { -199.27f, -579.92f, 20.98f, 0.00f },
    { -206.17f, -586.03f, 20.98f, 0.00f },
    { -212.79f, -578.70f, 20.98f, 0.00f }
};

Position const introPosOne = { -211.880f, -561.9776f, 20.976f, 1.1710f };

Position const introPosTwo[2] =
{
    { -202.332f, -570.982f, 20.977f, 1.821f },
    { -215.191f, -576.233f, 20.977f, 2.476f }
};

Position const introPosThree[3] =
{
    { -220.687f, -578.071f, 20.977f, 3.128f },
    { -196.289f, -564.827f, 20.977f, 1.133f },
    { -208.065f, -575.389f, 25.883f, 4.393f }
};

static Position const prototypePosition = { -205.83f, -553.79f, 51.23f, 0.60f };

class boss_foe_reaper_5000 : public CreatureScript
{
public:
    boss_foe_reaper_5000() : CreatureScript("boss_foe_reaper_5000") {}

    struct boss_foe_reaper_5000AI : public BossAI
    {
        boss_foe_reaper_5000AI(Creature * creature) : BossAI(creature, BOSS_FOE_REAPER_5000_DATA)
        {
            me->setPowerType(POWER_ENERGY);
            me->SetMaxPower(POWER_ENERGY, 100);
            me->SetPower(POWER_ENERGY, 0);
        }

        void Reset()
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            enraged = false;
            _Reset();

            if (!init)
            {
                init = true;
                introCount = 0;
                for (uint8 i = 0; i < 4; i++)
                    me->SummonCreature(NPC_REAPER_DEFIAS, reaperSummonPos[i]);

                if (IsHeroic()) // this npc is only available on heroic difficulty
                    me->SummonCreature(NPC_FRIENDLY_PROTOTYPE, prototypePosition);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_OFF_LINE, true);
                me->GetPosition(&_targetPos);
                events.SetPhase(PHASE_INTRO);
            }
        }

        void JustSummoned(Creature *summon)
        {
            switch (summon->GetEntry())
            {
                case NPC_HARVEST_DUMMY:
                    Talk(EMOTE_HARVEST);
                    summon->CastSpell(summon, SPELL_HARVEST_READY, true);
                    summon->GetPosition(&_targetPos);
                    events.ScheduleEvent(EVENT_HARVEST_MOVEMENTS, 0);
                    break;
                case NPC_SCORIES:
                    summon->NearTeleportTo(summon->GetPositionX(), summon->GetPositionY(), 19.31f, summon->GetOrientation());
                    summon->SetInCombatWithZone();
                    break;
                case NPC_REAPER_DEFIAS:
                    summon->SetReactState(REACT_AGGRESSIVE);
                    break;
                case NPC_FRIENDLY_PROTOTYPE:
                    summon->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    break;
                case NPC_FIRE_BUNNY:
                    summon->CastSpell(me, SPELL_ELECTRIC_CHARGE, true);
                    break;
                default:
                    break;
            }
            BossAI::JustSummoned(summon);
        }

        void EnterCombat(Unit * who)
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            events.SetPhase(PHASE_FIGHT);
            events.ScheduleEvent(EVENT_OVERDRIVE, 10000);
            events.ScheduleEvent(EVENT_HARVEST, 35000);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_SCORIES, 20000);
            _EnterCombat();
        }

        void JustDied(Unit * /*killer*/)
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            std::list<Creature* > triggerList;
            me->GetCreatureListWithEntryInGrid(triggerList, NPC_FRIENDLY_PROTOTYPE, 100.0f);
            for (std::list<Creature* >::iterator itr = triggerList.begin(); itr != triggerList.end(); itr++)
                if (Creature *bot = *itr)
                    if (!bot->isAlive() || bot->HealthBelowPct(11))
                    {
                        instance->SetData(DATA_PRODIGY, 0);
                        break;
                    }
            Talk(SAY_DEATH);
            _JustDied();
        }

        void SpellHit(Unit* caster, SpellInfo const* spellInfo)
        {
            if (spellInfo->Id == SPELL_ENERGIZE)
            {
                if (me->GetPower(POWER_ENERGY) == me->GetMaxPower(POWER_ENERGY))
                {
                    DoCast(me, SPELL_ACTIVATE);
                    events.ScheduleEvent(EVENT_INTRO, 2000);
                }
            }
        }

        void KilledUnit(Unit * victim)
        {
            Talk(SAY_KILLER);
        }

        void DoMeleeAttackIfReady()
        {
            if (me->getVictim() && me->isAttackReady() && me->IsWithinMeleeRange(me->getVictim()))
            {
                me->CastSpell(me->getVictim(), SPELL_REAPER_STRIKE, true);
                me->resetAttackTimer();
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE && id == POINT_HARVEST)
                events.ScheduleEvent(EVENT_HARVEST_SWEEP, 2000);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (!enraged && me->HealthBelowPctDamaged(30, damage))
            {
                enraged = true;
                Talk(SAY_SAFETY);
                DoCast(SPELL_SAFETY_RESTRICTIONS);
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim() && !events.IsInPhase(PHASE_INTRO))
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                if (me->HasAura(SPELL_OVERDRIVE))
                    me->ClearUnitState(UNIT_STATE_CASTING);
                if (me->HasAura(SPELL_HARVEST_2))
                    me->ClearUnitState(UNIT_STATE_CASTING);
                return;
            }

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_INTRO:
                    {
                        switch (++introCount)
                        {
                            case 1:
                                me->SummonCreature(NPC_FIRE_BUNNY, introPosOne, TEMPSUMMON_TIMED_DESPAWN, 1000);
                                break;
                            case 2:
                                for (uint8 i = 0; i < 2; i++)
                                    me->SummonCreature(NPC_FIRE_BUNNY, introPosTwo[i], TEMPSUMMON_TIMED_DESPAWN, 1000);
                                break;
                            case 3:
                                for (uint8 i = 0; i < 3; i++)
                                    me->SummonCreature(NPC_FIRE_BUNNY, introPosThree[i], TEMPSUMMON_TIMED_DESPAWN, 1000);
                                DoCast(me, SPELL_RED_EYES, true);
                                events.ScheduleEvent(EVENT_INTRO_FINAL, 1000);
                                return;
                            default:
                                break;
                        }
                        events.ScheduleEvent(EVENT_INTRO, 2000);
                        break;
                    }
                    case EVENT_INTRO_FINAL:
                    {
                        Talk(SAY_AGGRO);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->RemoveAura(SPELL_OFF_LINE);
                        me->SetReactState(REACT_DEFENSIVE);
                        break;
                    }
                    case EVENT_OVERDRIVE:
                        Talk(EMOTE_OVERDRIVE);
                        Talk(SAY_OVERDRIVE);
                        me->SetReactState(REACT_PASSIVE);
                        me->CastSpell(me, SPELL_OVERDRIVE);
                        events.ScheduleEvent(EVENT_OVERDRIVE_MOVEMENTS, 2000);
                        events.ScheduleEvent(EVENT_OVERDRIVE_END, 11000);
                        events.ScheduleEvent(EVENT_OVERDRIVE, 60000);
                        break;
                    case EVENT_OVERDRIVE_MOVEMENTS:
                    {
                        Position pos;
                        me->GetFirstCollisionPosition(pos, (float)urand(20, 50) * (float)rand_norm(), (float)rand_norm() * static_cast<float>(2 * M_PI));
                        me->GetMotionMaster()->MovePoint(0, pos);
                        if (me->HasAura(SPELL_OVERDRIVE))
                            events.ScheduleEvent(EVENT_OVERDRIVE_MOVEMENTS, 1000);
                        break;
                    }
                    case EVENT_OVERDRIVE_END:
                        events.CancelEvent(EVENT_OVERDRIVE_MOVEMENTS);
                        me->SetReactState(REACT_AGGRESSIVE);
                        if (Unit *victim = me->getVictim())
                            DoStartMovement(victim);
                        break;
                    case EVENT_HARVEST:
                        Talk(SAY_HARVEST);
                        me->SetReactState(REACT_PASSIVE);
                        DoCastRandom(SPELL_HARVEST, 0.0f);
                        events.ScheduleEvent(EVENT_HARVEST, 60000);
                        break;
                    case EVENT_HARVEST_MOVEMENTS:
                    {
                        me->GetMotionMaster()->MovePoint(POINT_HARVEST, _targetPos);
                        break;
                    }
                    case EVENT_HARVEST_SWEEP:
                    {
                        DoCast(SPELL_HARVEST_SWEEP);
                        me->RemoveAura(SPELL_HARVEST);
                        me->RemoveAura(SPELL_HARVEST_2);
                        me->SetReactState(REACT_AGGRESSIVE);
                        if (me->getVictim())
                            DoStartMovement(me->getVictim());
                        if (Creature *target = me->FindNearestCreature(NPC_HARVEST_DUMMY, 200))
                            target->DespawnOrUnsummon(1);
                        break;
                    }
                    case EVENT_SCORIES:
                    {
                        me->SummonCreature(NPC_SCORIES, scorpiesSummonPos[urand(0, 3)]);
                        //DoCast(me, SPELL_SUMMON_SCORIES, true); replaced with SummonCreature because target_nearby_entry always use the nearest npc there is no random selection at all...
                        events.ScheduleEvent(EVENT_SCORIES, 20000);
                    }
                }
            }
            DoMeleeAttackIfReady();
        }

        private:
            bool enraged = false;
            bool init = false;
            int8 introCount;
            Position _targetPos;
    };

    CreatureAI * GetAI(Creature * creature) const
    {
        return new boss_foe_reaper_5000AI(creature);
    }
};

class npc_scormies : public CreatureScript
{
public:
    npc_scormies() : CreatureScript("npc_scormies") { }

    struct npc_scormiesAI : public ScriptedAI
    {
        npc_scormiesAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_SCORIES_SHIELD, true);
        }

        void Reset()
        {
            _fixateTarget = 0;
            _events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            _events.ScheduleEvent(EVENT_FIXATE, 5000);
        }

        void UpdateAI(const uint32 diff)
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
                    case EVENT_FIXATE:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                        {
                            me->CastSpell(target, SPELL_FIXATE, true);
                            _fixateTarget = target->GetGUID();
                        }
                        _events.ScheduleEvent(EVENT_FIXATE_CHECK, 1000);
                        _events.ScheduleEvent(EVENT_FIXATE, 30000);
                        break;
                    case EVENT_FIXATE_CHECK:
                        if (_fixateTarget != 0)
                        {
                            if (Unit *target = Unit::GetUnit(*me, _fixateTarget))
                            {
                                if (target->HasAura(SPELL_FIXATE) && me->getVictim() != target)
                                    me->AddThreat(target, 50000000.0f);
                                else if (!target->HasAura(SPELL_FIXATE))
                                {
                                    me->getThreatManager().addThreat(target, -90000000);
                                    _fixateTarget = 0;
                                }
                            }
                            else
                                _fixateTarget = 0;
                            if (_fixateTarget)
                                _events.ScheduleEvent(EVENT_FIXATE_CHECK, 1000);
                        }
                        break;
                    default:
                        break;
                }
            }
        }

    private :
        InstanceScript *instance;
        EventMap _events;
        uint64 _fixateTarget;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_scormiesAI (creature);
    }
};

class npc_prototype_reaper : public CreatureScript
{
    public:
        npc_prototype_reaper() : CreatureScript("npc_prototype_reaper") { }

        struct npc_prototype_reaperAI : public ScriptedAI
        {
            npc_prototype_reaperAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->setRegeneratingHealth(false);
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) override
            {
                clicker->CastSpell(me, SPELL_RIDE_VEHICLE, true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_prototype_reaperAI(creature);
        }
};

class npc_defias_reaper : public CreatureScript
{
    public:
        npc_defias_reaper() : CreatureScript("npc_defias_reaper") { }

        struct npc_defias_reaperAI : public ScriptedAI
        {
            npc_defias_reaperAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 10000));
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (!energizeCasted && me->HealthBelowPctDamaged(35, damage))
                {
                    me->setRegeneratingHealth(false);
                    energizeCasted = true;
                    events.Reset();
                    me->RemoveAllAuras();
                    me->CombatStop(true);
                    me->SetReactState(REACT_PASSIVE);
                    if (InstanceScript* instance = me->GetInstanceScript())
                        if (Creature* foeReaper = ObjectAccessor::GetCreature(*me, instance->GetData64(BOSS_FOE_REAPER_5000_DATA)))
                            me->CastSpell(foeReaper, SPELL_ENERGIZE, true);
                    DoCast(me, SPELL_ON_FIRE, true);
                    me->setFaction(35);
                    me->DespawnOrUnsummon(31000);
                    me->CastWithDelay(30000, me, SPELL_EXPLODE_SUICIDE, true);
                    damage = 0;
                }
            }

            void PassengerBoarded(Unit* passenger, int8 seat, bool apply)
            {
                if (!apply && energizeCasted)
                    me->setFaction(35); // restore friendly faction because the vehicle system restores the enemy(original) faction...
            }

            void OnSpellClick(Unit* clicker, bool& /*result*/) override
            {
                clicker->CastSpell(me, SPELL_RIDE_VEHICLE, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(eventId, urand(5000, 10000));
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                bool energizeCasted = false;
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_defias_reaperAI(creature);
        }
};

// 16210
class achievement_prototype_prodigy : public AchievementCriteriaScript
{
    public:
        achievement_prototype_prodigy() : AchievementCriteriaScript("achievement_prototype_prodigy") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (!target)
                return false;
            if (InstanceScript *instance = target->GetInstanceScript())
                return instance->instance->IsHeroic() && instance->GetData(DATA_PRODIGY);
            return false;
        }
};


void AddSC_boss_foe_reaper_5000()
{
    new boss_foe_reaper_5000();
    new npc_scormies();
    new npc_prototype_reaper();
    new npc_defias_reaper();
    new achievement_prototype_prodigy();
}
