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

enum Spells
{
	SPELL_BOULDER_SMASH = 107597,
	SPELL_DUST_STORM = 107682,
	SPELL_SHADOW_BOLT = 95440,

	SPELL_DRENCHED = 107801,
	SPELL_FLOOD = 107797,

	SPELL_TWILIGHT_SUBMISSION = 108183,
	SPELL_TWILIGHT_VOLLEY = 108172,

	SPELL_TWILIGHT_CORRUPTION = 107852,
	SPELL_TWILIGHT_RAGE = 107872,

	SPELL_SHADOW_GAZE = 109391,

	SPELL_SLUDGE_SPEW = 110102,
	SPELL_WILD_FLAIL = 109199,
};

enum Events
{
	EVENT_BOULDER_SMASH = 1,
	EVENT_DUST_STORM = 2,
	EVENT_SHADOW_BOLT = 3,

	EVENT_DRENCHED = 4,
	EVENT_FLOOD = 5,

	EVENT_TWILIGHT_SUBMISSION = 6,
	EVENT_TWILIGHT_VOLLEY = 7,

	EVENT_TWILIGHT_CORRUPTION = 8,
	EVENT_TWILIGHT_RAGE = 9,

	EVENT_SHADOW_GAZE = 10,

	EVENT_SLUDGE_SPEW = 11,
	EVENT_WILD_FLAIL = 12,

};

// 57158
class npc_ds_earthen_destroyer : public CreatureScript
{
public:
	npc_ds_earthen_destroyer() : CreatureScript("npc_ds_earthen_destroyer") {}

	struct npc_ds_earthen_destroyerAI : public ScriptedAI
	{
		npc_ds_earthen_destroyerAI(Creature* creature) : ScriptedAI(creature) { }

		void EnterCombat(Unit* victim) override
		{
			events.ScheduleEvent(EVENT_BOULDER_SMASH, 5000);
			events.ScheduleEvent(EVENT_DUST_STORM, 15000);
			events.ScheduleEvent(EVENT_SHADOW_BOLT, 2000);
		}

		void UpdateAI(uint32 const diff) override
		{
			if (!me->IsVisible() || !UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_BOULDER_SMASH:
					DoCastRandom(SPELL_BOULDER_SMASH, 90.0f);
					events.ScheduleEvent(EVENT_BOULDER_SMASH, urand(10000, 12000));
					break;
				case EVENT_DUST_STORM:
					DoCast(SPELL_DUST_STORM);
					events.ScheduleEvent(EVENT_DUST_STORM, urand(10000, 15000));
					break;
				case EVENT_SHADOW_BOLT:
					DoCastRandom(SPELL_SHADOW_BOLT, 90.0f);
					events.ScheduleEvent(EVENT_BOULDER_SMASH, urand(5000, 10000));
					break;
				default:
					break;
				}
			}
			DoMeleeAttackIfReady();
		}

	private:
		EventMap events;
		InstanceScript* instance;
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_ds_earthen_destroyerAI(creature);
	}
};

// 57160
class npc_ds_ancient_water_lord : public CreatureScript
{
public:
	npc_ds_ancient_water_lord() : CreatureScript("npc_ds_ancient_water_lord") {}

	struct npc_ds_ancient_water_lordAI : public ScriptedAI
	{
		npc_ds_ancient_water_lordAI(Creature* creature) : ScriptedAI(creature) { }

		void EnterCombat(Unit* victim) override
		{
			events.ScheduleEvent(EVENT_DRENCHED, 5000);
			events.ScheduleEvent(EVENT_FLOOD, 15000);
		}

		void UpdateAI(uint32 const diff) override
		{
			if (!me->IsVisible() || !UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_DRENCHED:
					DoCastRandom(SPELL_DRENCHED, 90.0f);
					events.ScheduleEvent(EVENT_DRENCHED, urand(5000, 8000));
					break;
				case EVENT_FLOOD:
					DoCastRandom(SPELL_FLOOD, 90.0f);
					events.ScheduleEvent(EVENT_FLOOD, urand(10000, 15000));
					break;
				default:
					break;
				}
			}
			DoMeleeAttackIfReady();
		}

	private:
		EventMap events;
		InstanceScript* instance;
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_ds_ancient_water_lordAI(creature);
	}
};

// 57280
class npc_ds_twilight_siege_captain : public CreatureScript
{
public:
	npc_ds_twilight_siege_captain() : CreatureScript("npc_ds_twilight_siege_captain") {}

	struct npc_ds_twilight_siege_captainAI : public ScriptedAI
	{
		npc_ds_twilight_siege_captainAI(Creature* creature) : ScriptedAI(creature) { }

		void EnterCombat(Unit* victim) override
		{
			events.ScheduleEvent(EVENT_TWILIGHT_SUBMISSION, 2000);
			events.ScheduleEvent(EVENT_TWILIGHT_VOLLEY, 5000);
		}

		void UpdateAI(uint32 const diff) override
		{
			if (!me->IsVisible() || !UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_TWILIGHT_SUBMISSION:
					DoCastRandom(SPELL_TWILIGHT_SUBMISSION, 100.0f);
					events.ScheduleEvent(EVENT_TWILIGHT_SUBMISSION, urand(6000, 8000));
					break;
				case EVENT_TWILIGHT_VOLLEY:
					DoCastRandom(SPELL_TWILIGHT_VOLLEY, 35.0f);
					events.ScheduleEvent(EVENT_TWILIGHT_VOLLEY, urand(10000, 12000));
					break;
				default:
					break;
				}
			}
			DoMeleeAttackIfReady();
		}

	private:
		EventMap events;
		InstanceScript* instance;
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_ds_twilight_siege_captainAI(creature);
	}
};

// UPDATE `creature_template` SET `flags_extra` = 128 WHERE `entry` IN(57231,57295);

// 57159
class npc_ds_earthen_solider : public CreatureScript
{
public:
	npc_ds_earthen_solider() : CreatureScript("npc_ds_earthen_solider") {}

	struct npc_ds_earthen_soliderAI : public ScriptedAI
	{
		npc_ds_earthen_soliderAI(Creature* creature) : ScriptedAI(creature) { }

		void EnterCombat(Unit* victim) override
		{
			events.ScheduleEvent(EVENT_SHADOW_BOLT, 5000);
			events.ScheduleEvent(EVENT_TWILIGHT_CORRUPTION, 8000);
			events.ScheduleEvent(EVENT_TWILIGHT_RAGE, 25000);
		}

		void UpdateAI(uint32 const diff) override
		{
			if (!me->IsVisible() || !UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_SHADOW_BOLT:
					DoCastRandom(SPELL_SHADOW_BOLT, 90.0f);
					events.ScheduleEvent(EVENT_SHADOW_BOLT, urand(3000, 5000));
					break;
				case EVENT_TWILIGHT_CORRUPTION:
					DoCast(SPELL_TWILIGHT_CORRUPTION);
					events.ScheduleEvent(EVENT_TWILIGHT_CORRUPTION, urand(6000, 8000));
					break;
				case EVENT_TWILIGHT_RAGE:
					DoCast(SPELL_TWILIGHT_RAGE);
					events.ScheduleEvent(EVENT_TWILIGHT_RAGE, 24000);
					break;
				default:
					break;
				}
			}
			DoMeleeAttackIfReady();
		}

	private:
		EventMap events;
		InstanceScript* instance;
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_ds_earthen_soliderAI(creature);
	}
};

// 57875
class npc_ds_eye_of_gorath_trash : public CreatureScript
{
public:
	npc_ds_eye_of_gorath_trash() : CreatureScript("npc_ds_eye_of_gorath_trash") {}

	struct npc_ds_eye_of_gorath_trashAI : public Scripted_NoMovementAI
	{
		npc_ds_eye_of_gorath_trashAI(Creature* creature) : Scripted_NoMovementAI(creature) { }

		void EnterCombat(Unit* victim) override
		{
			events.ScheduleEvent(EVENT_SHADOW_GAZE, 3000);
		}

		void UpdateAI(uint32 const diff) override
		{
			if (!me->IsVisible() || !UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_SHADOW_GAZE:
					DoCastRandom(SPELL_SHADOW_GAZE, 100.0f);
					events.ScheduleEvent(EVENT_SHADOW_GAZE, urand(5000, 8000));
					break;
				default:
					break;
				}
			}
			DoMeleeAttackIfReady();
		}

	private:
		EventMap events;
		InstanceScript* instance;
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_ds_eye_of_gorath_trashAI(creature);
	}
};

// 57877
class npc_ds_flail_of_gorath_trash : public CreatureScript
{
public:
	npc_ds_flail_of_gorath_trash() : CreatureScript("npc_ds_flail_of_gorath_trash") {}

	struct npc_ds_flail_of_gorath_trashAI : public Scripted_NoMovementAI
	{
		npc_ds_flail_of_gorath_trashAI(Creature* creature) : Scripted_NoMovementAI(creature) { }

		void EnterCombat(Unit* victim) override
		{
			events.ScheduleEvent(EVENT_SLUDGE_SPEW, 2000);
			events.ScheduleEvent(EVENT_WILD_FLAIL, 5000);
		}

		void UpdateAI(uint32 const diff) override
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_SLUDGE_SPEW:
					DoCastRandom(SPELL_SLUDGE_SPEW, 200.0f);
					events.ScheduleEvent(EVENT_SLUDGE_SPEW, urand(10000, 12000));
					break;
				case EVENT_WILD_FLAIL:
					DoCast(SPELL_WILD_FLAIL);
					events.ScheduleEvent(EVENT_WILD_FLAIL, urand(6000, 8000));
					break;
				default:
					break;
				}
			}
			DoMeleeAttackIfReady();
		}

	private:
		EventMap events;
		InstanceScript* instance;
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_ds_flail_of_gorath_trashAI(creature);
	}
};

enum Globules
{
	ACID = 57333,
	SHADOWED = 57388,
	COBALT = 57384,
	GLOWING = 57387,
	CRIMSON = 57386,
	DARK = 57382,

	SPELL_MANA_DIFFUSION = 108228,
	SPELL_GLOWING_BLOOD = 108221,

	EVENT_1 = 1,
	EVENT_2 = 2,
};

// 57333 57388 57384 57387 57386 57382
class npc_ds_globule_trash : public CreatureScript
{
public:
	npc_ds_globule_trash() : CreatureScript("npc_ds_globule_trash") {}

	struct npc_ds_globule_trashAI : public ScriptedAI
	{
		npc_ds_globule_trashAI(Creature* creature) : ScriptedAI(creature) { }

		void EnterCombat(Unit* victim) override
		{

			switch (me->GetEntry())
			{
			case COBALT:
				//case GLOWING: temp disabled because the effect triggers from npc to npc -> every creature at this map get this buff it is impossible to remove this aura
				DoCast(me->GetEntry() == COBALT ? 108223 : 108221);
				break;
			case DARK:
				events.ScheduleEvent(EVENT_1, 2000);
				break;
			case ACID:
			case SHADOWED:
			case CRIMSON:
				events.ScheduleEvent(EVENT_2, urand(2000, 6000));
				break;
			default:
				break;
			}
		}

		void JustDied(Unit* killer) override
		{
			if (me->GetEntry() == COBALT)
				DoCast(SPELL_MANA_DIFFUSION);
		}

		void UpdateAI(uint32 const diff) override
		{
			if (!me->IsVisible() || !UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_1:
					DoCastRandom(108922, 20.0f);
					if (me->HasAura(SPELL_GLOWING_BLOOD))
						events.ScheduleEvent(EVENT_1, urand(1500, 3000));
					else
						events.ScheduleEvent(EVENT_1, urand(3000, 6000));
					break;
				case EVENT_2:
				{
					uint32 spellId = 0;
					switch (me->GetEntry())
					{
					case ACID:
						spellId = 108419;
						break;
					case SHADOWED:
						spellId = 108220;
						break;
					case CRIMSON:
						spellId = 108218;
					default:
						break;
					}
					DoCastRandom(spellId, 40.0f);
					if (me->HasAura(SPELL_GLOWING_BLOOD))
						events.ScheduleEvent(EVENT_2, urand(5000, 7500));
					else
						events.ScheduleEvent(EVENT_2, urand(10000, 15000));
					break;
				}
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
		return new npc_ds_globule_trashAI(creature);
	}
};

enum LordMisc
{
	TALK_WE_WILL_SEE_CLASS = -56587,
	MENU_ID_RAID_NERF = 13357
};

class npc_ds_lord_afrasastrasz : public CreatureScript
{
public:
	npc_ds_lord_afrasastrasz() : CreatureScript("npc_ds_lord_afrasastrasz") { }

	struct npc_ds_lord_afrasastraszAI : public ScriptedAI
	{
		npc_ds_lord_afrasastraszAI(Creature* creature) : ScriptedAI(creature) { }

		void sGossipSelect(Player* player, uint32 menuId, uint32 /*gossipId*/) override
		{
			if (menuId == MENU_ID_RAID_NERF)
			{
				Talk(TALK_WE_WILL_SEE_CLASS, player->GetGUID());
				if (InstanceScript* instance = me->GetInstanceScript())
					instance->SetData(DATA_RAID_NERF, RAID_NERF_INACTIVE);
				player->PlayerTalkClass->SendCloseGossip();
			}
		}
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new npc_ds_lord_afrasastraszAI(creature);
	}
};

void AddSC_instance_ds_trash()
{
	new npc_ds_earthen_destroyer();
	new npc_ds_ancient_water_lord();
	new npc_ds_twilight_siege_captain();
	new npc_ds_earthen_solider();
	new npc_ds_eye_of_gorath_trash();
	new npc_ds_flail_of_gorath_trash();
	new npc_ds_globule_trash();
	new npc_ds_lord_afrasastrasz();
}