/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "MotionMaster.h"
#include "ScriptedCreature.h"
#include "the_stockade.h"

enum Spells
{
    SPELL_WILDLY_STABBING = 86726,
    SPELL_SWEEP = 86729,
    SPELL_VANISH = 55964,
    SPELL_SHADOWSTEP = 55966
};

enum Events
{
    EVENT_WILDLY_STABBING = 1,
    EVENT_SWEEP,
    EVENT_VANISH,
    EVENT_ATTACK_RANDOM,

    EVENT_MORTIMER_MOLOCH_EMOTE,
    EVENT_MORTIMER_MOLOCH_DEATH
};

enum Says
{
    SAY_PULL = 0, //Allow me to introduce myself. I am Randolph Moloch and I will be killing you all today.
    SAY_VANISH = 1, // Area Trigger: %s vanishes!
    SAY_DEATH = 2, //My epic schemes, my great plans! Gone!

    MORTIMER_MOLOCH_DEATH = 0, // %s collapses from a heart attack!
    MORTIMER_MOLOCH_EMOTE = 1, //Egad! My sophisticated heart!
};

enum Points
{
    POINT_FINISH = 0,
};

Position const mortimerMolochPos = { 145.5811f, 0.7059f, -25.606f, 6.2f };

struct boss_randolph_moloch : public ScriptedAI
{
    public:
        boss_randolph_moloch(Creature* creature) : ScriptedAI(creature)
        {
            firstVanish = false;
            secondVanish = false;
        }

        void Reset()
        {
            events.Reset();
            m_Scheduler.CancelAll();
            firstVanish = false;
            secondVanish = false;
        }

        void EnterCombat(Unit* /*who*/) override
        {
            Talk(SAY_PULL);

            events.ScheduleEvent(EVENT_WILDLY_STABBING, 4000, 5000);
            events.ScheduleEvent(EVENT_SWEEP, 2000, 3000);
        }

        void JustSummoned(Creature* summon) override
        {
            CreatureAI::JustSummoned(summon);

            if (summon->GetEntry() == NPC_MORTIMER_MOLOCH)
            {
                summon->SetWalk(true);
                summon->GetMotionMaster()->MovePoint(POINT_FINISH, *me);
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(SAY_DEATH);

            me->SummonCreature(NPC_MORTIMER_MOLOCH, mortimerMolochPos);
        }

        void UpdateAI(const uint32 diff) override
        {
            m_Scheduler.Update(diff);

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WILDLY_STABBING:
                        DoCastVictim(SPELL_WILDLY_STABBING);
                        events.RescheduleEvent(EVENT_WILDLY_STABBING, 8000, 12000);
                        break;
                    case EVENT_SWEEP:
                        DoCastVictim(SPELL_SWEEP);
                        events.ScheduleEvent(EVENT_SWEEP, 6000, 7000);
                        break;
                    case EVENT_VANISH:
                    {
                        Talk(SAY_VANISH);
                        me->RemoveAllAuras();
                        DoCast(me, SPELL_VANISH);
                        me->SetReactState(REACT_PASSIVE);
                        me->SetInCombatState(true); // Prevents the boss from resetting

                        m_Scheduler.Schedule(Seconds(2), [this](TaskContext /*p_Context*/)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_SHADOWSTEP, true);

                            me->SetReactState(REACT_AGGRESSIVE);

                            me->SetInCombatWithZone();
                        });

                        break;
                    }

                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (me->HealthBelowPctDamaged(71, damage) && me->HealthAbovePct(59) && !firstVanish)
            {
                firstVanish = true;
                events.ScheduleEvent(EVENT_VANISH, 1000);
            }

            if (me->HealthBelowPctDamaged(41, damage) && me->HealthAbovePct(29) && !secondVanish)
            {
                secondVanish = true;
                events.ScheduleEvent(EVENT_VANISH, 1000);
            }
        }

    private:
        EventMap events;
        bool firstVanish, secondVanish;
        TaskScheduler m_Scheduler;
};

struct npc_mortimer_moloch : public ScriptedAI
{
    public:
        npc_mortimer_moloch(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE)
                if (id == POINT_FINISH)
                    _events.ScheduleEvent(EVENT_MORTIMER_MOLOCH_EMOTE, 4000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MORTIMER_MOLOCH_EMOTE:
                        Talk(MORTIMER_MOLOCH_EMOTE);
                        _events.ScheduleEvent(EVENT_MORTIMER_MOLOCH_DEATH, 3000);
                        break;
                    case EVENT_MORTIMER_MOLOCH_DEATH:
                        Talk(MORTIMER_MOLOCH_DEATH);
                        me->Kill(me, true);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        EventMap _events;
};

void AddSC_boss_randolph_moloch()
{
    RegisterCreatureAI(boss_randolph_moloch);
    RegisterCreatureAI(npc_mortimer_moloch);
}