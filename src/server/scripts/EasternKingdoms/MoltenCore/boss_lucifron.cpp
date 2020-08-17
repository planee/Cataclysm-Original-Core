/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Boss_Lucifron
SD%Complete: 100
SDComment:
SDCategory: Molten Core
EndScriptData */

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "molten_core.h"

enum Spells
{
    SPELL_IMPENDING_DOOM    = 19702,
    SPELL_LUCIFRON_CURSE    = 19703,
    SPELL_SHADOW_SHOCK      = 20603,
	DOMINATE_MIND           = 20604,
	CLEAVE                  = 20605,
};

enum Events
{
    EVENT_IMPENDING_DOOM    = 1,
    EVENT_LUCIFRON_CURSE    = 2,
    EVENT_SHADOW_SHOCK      = 3,
	EVENT_DOMINATE_MIND     = 4,
	EVENT_CLEAVE            = 5,
	SUMMON_EVENT
};

class boss_lucifron : public CreatureScript
{
    public:
        boss_lucifron() : CreatureScript("boss_lucifron") { }

        struct boss_lucifronAI : public BossAI
        {
            boss_lucifronAI(Creature* creature) : BossAI(creature, BOSS_LUCIFRON)
            {
            }

			uint64 flamewakerGUID[2]; //  array should be at least 2 in size looking at it using [0], [1]

			void Reset()
			{
				events.Reset();					
				DoAction(SUMMON_EVENT);				
			}

            void EnterCombat(Unit* victim)
            {
                BossAI::EnterCombat(victim);
                events.ScheduleEvent(EVENT_IMPENDING_DOOM, 10000);
                events.ScheduleEvent(EVENT_LUCIFRON_CURSE, 15000);
                events.ScheduleEvent(EVENT_SHADOW_SHOCK, 6000);
            }

			void DoAction(const int32 type)
			{

				if (type == SUMMON_EVENT)
				{
					flamewakerGUID[0] = 0;
					flamewakerGUID[1] = 0;
					
					   for (int i = 0; i < 2; i++)
					   {
						   if (Creature * flamewaker = me->SummonCreature(FLAMEWAKER_PROTECTOR, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
						      flamewakerGUID[i] = flamewaker->GetGUID();
					   }			

				if (Creature *firstflamew = Unit::GetCreature(*me, flamewakerGUID[0]))
					firstflamew->GetMotionMaster()->MoveFollow(me, 1.0f, -0.58875f, MOTION_SLOT_ACTIVE);
				if (Creature *Secoundflamew = Unit::GetCreature(*me, flamewakerGUID[1]))
					Secoundflamew->GetMotionMaster()->MoveFollow(me, 1.0f, 0.58875f, MOTION_SLOT_ACTIVE);
				}
			}
			
            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
				{
					if (!me->FindNearestCreature(FLAMEWAKER_PROTECTOR, 100.01f))
						DoAction(SUMMON_EVENT);											
                    return;
				}

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_IMPENDING_DOOM:
                            DoCastVictim(SPELL_IMPENDING_DOOM);
                            events.ScheduleEvent(EVENT_IMPENDING_DOOM, 20000);
                            break;
                        case EVENT_LUCIFRON_CURSE:
                            DoCastVictim(SPELL_LUCIFRON_CURSE);
                            events.ScheduleEvent(EVENT_LUCIFRON_CURSE, 15000);
                            break;
                        case EVENT_SHADOW_SHOCK:
                            DoCastVictim(SPELL_SHADOW_SHOCK);
                            events.ScheduleEvent(EVENT_SHADOW_SHOCK, 6000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_lucifronAI(creature);
        }
};

//Flamewaker Protector 
class npc_flamewaker_protector : public CreatureScript
{
public:
	npc_flamewaker_protector() : CreatureScript("npc_flamewaker_protector") { }

	struct npc_flamewaker_protectorAI : public ScriptedAI
	{
		npc_flamewaker_protectorAI(Creature* creature) : ScriptedAI(creature) {}

		EventMap events;

		void Reset()
		{
			events.Reset();
		}

		void EnterCombat(Unit* victim)
		{
			CreatureAI::EnterCombat(victim);
			events.ScheduleEvent(EVENT_CLEAVE, 6000);
			events.ScheduleEvent(EVENT_DOMINATE_MIND, 15000);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())				
			{
				if (!me->FindNearestCreature(NPC_LUCIFRON, 30.01f))
					me->DespawnOrUnsummon(10);
				return;
			}

			events.Update(diff);

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_CLEAVE:
					DoCastVictim(CLEAVE);
					events.ScheduleEvent(EVENT_CLEAVE, 6000);
					break;
				case EVENT_DOMINATE_MIND:
					if (Creature *lucifron = me->FindNearestCreature(NPC_LUCIFRON, 30.0f))
					{						
					       DoCastVictim(DOMINATE_MIND);
					}
					events.ScheduleEvent(EVENT_DOMINATE_MIND, 15000);
					break;
				default:
					break;
				}
			}

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_flamewaker_protectorAI(creature);
	}
};


void AddSC_boss_lucifron()
{
    new boss_lucifron();
	new npc_flamewaker_protector();
}
