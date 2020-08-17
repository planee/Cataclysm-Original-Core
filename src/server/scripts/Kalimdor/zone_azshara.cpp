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

enum twilightCage
{
    NPC_BILGEWATER_LABORER  = 36722,
};

class go_azshara_twilight_cage : public GameObjectScript
{
public:
    go_azshara_twilight_cage() : GameObjectScript("go_azshara_twilight_cage") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        go->UseDoorOrButton(180);
        if (Creature* laborer = go->FindNearestCreature(NPC_BILGEWATER_LABORER, 5.0f, true))
        {
            Position pos;
            laborer->GetNearPosition(pos, 15.00f, 0.00f);
            laborer->GetMotionMaster()->MovePoint(0, pos);
            laborer->DespawnOrUnsummon(10000);
            player->KilledMonsterCredit(laborer->GetEntry(), laborer->GetGUID());
        }
        return true;
    }
};


/*
enum FadeToBlackQuest
{
    NPC_MALICION                = 36649,
    ACTION_ENTER_COMBAT         = 1,
    ACTION_LAND                 = 2,
    POINT_AIR                   = 4,
    POINT_GROUND                = 5,
    SPELL_FLAME_BURST           = 69114,
    SPELL_SHIELD_BREAK          = 69115,
    SPELL_RIDE_VEHICLE          = 43671,
    SPELL_TWILIGHT_BARRIER      = 69135,
    SPELL_SUMMON_KALECGOS       = 69178,
    EVENT_FLAME_BURST           = 1,
    EVENT_TWILIGHT_BARRIER      = 2,
    EVENT_PHASE_1               = 1,
    EVENT_PHASE_2               = 2,
    TALK_VEHICLE_ENTER          = -36897,
    TALK_KATRANA_ENTER_COMBAT   = -77866,
};

class npc_azshara_twilight_lord_katrana : public CreatureScript
{
public:
    npc_azshara_twilight_lord_katrana() : CreatureScript("npc_azshara_twilight_lord_katrana") { }

    struct npc_azshara_twilight_lord_katranaAI : public ScriptedAI
    {
        npc_azshara_twilight_lord_katranaAI(Creature* creature) : ScriptedAI(creature), summons(creature) {}

        void Reset() override
        {
            events.Reset();
            summons.DespawnAll();
            me->SummonCreature(NPC_MALICION, 4766.41f, -7336.79f, 128.913f, 1.35019f, TEMPSUMMON_DEAD_DESPAWN);
        }

        void EnterCombat(Unit* attacker)
        {
            events.SetPhase(EVENT_PHASE_1);
            if (Creature* drake = ObjectAccessor::GetCreature(*me, summons.front()))
                drake->AI()->DoAction(ACTION_ENTER_COMBAT);
            Talk(TALK_KATRANA_ENTER_COMBAT);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void JustDied(Unit* killer) override
        {
            events.Reset();
            if (Creature* drake = ObjectAccessor::GetCreature(*me, summons.front()))
                drake->AI()->DoAction(ACTION_LAND);
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (events.IsInPhase(EVENT_PHASE_1) && me->HealthBelowPctDamaged(60, damage))
            {
                events.SetPhase(EVENT_PHASE_2);
                events.ScheduleEvent(EVENT_TWILIGHT_BARRIER, urand(28000, 32000));
                DoCast(me, SPELL_TWILIGHT_BARRIER, true);
            }
        }

        void SummonedCreatureDamageTaken(Unit* attacker, Creature* summon, uint32& damage, SpellInfo const* spellInfo)
        {
            if (!me->isInCombat())
                me->AI()->AttackStart(attacker);
        }

        void SpellHit(Unit* attacker, SpellInfo const* spellInfo) override
        {
            if (spellInfo->Id == SPELL_SHIELD_BREAK)
                me->RemoveAurasDueToSpell(SPELL_TWILIGHT_BARRIER);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TWILIGHT_BARRIER:
                        DoCast(me, SPELL_TWILIGHT_BARRIER, true);
                        events.ScheduleEvent(EVENT_TWILIGHT_BARRIER, urand(28000, 32000));
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        SummonList summons;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_azshara_twilight_lord_katranaAI(creature);
    }
};

class npc_azshara_malicion : public CreatureScript
{
public:
    npc_azshara_malicion() : CreatureScript("npc_azshara_malicion") { }

    struct npc_azshara_malicionAI : public ScriptedAI
    {
        npc_azshara_malicionAI(Creature* creature) : ScriptedAI(creature) {}

        void IsSummonedBy(Unit* summoner)
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
            case ACTION_ENTER_COMBAT:
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->GetMotionMaster()->MovePoint(POINT_AIR, 4764.34f, -7325.118f, 142.0452f);
                break;
            case ACTION_LAND:
                events.Reset();
                DoCast(me, SPELL_SUMMON_KALECGOS, true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->GetMotionMaster()->MovePoint(POINT_GROUND, 4774.59f, -7320.24f, 130.47f);
                break;
            default:
                break;
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case POINT_AIR:
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetOrientation(6.1050f);
                    events.ScheduleEvent(EVENT_FLAME_BURST, 1000);
                    break;
                case POINT_GROUND:
                    if (Player* player = me->FindNearestPlayer(150.00f))
                        player->CastSpell(me, SPELL_RIDE_VEHICLE, true);
                    Talk(TALK_VEHICLE_ENTER);
                    break;
                default:
                    break;
            }
        }

        void JustDied(Unit* killer) override
        {
            events.Reset();
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FLAME_BURST:
                        if (Player* player = me->FindNearestPlayer(80.00f))
                            DoCast(player, SPELL_FLAME_BURST, true);
                        events.ScheduleEvent(EVENT_FLAME_BURST, 5000);
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
        return new npc_azshara_malicionAI(creature);
    }
};*/

enum Stone_Cold
{
	QUEST_STONE_COLD = 14165,

	NPC_QGFB_KILL_CREDIT = 36450,
	NPC_DROWNING_WATCHMAN = 36440,

	SPELL_RESCUE_DROWNING_WATCHMAN = 68735,
	SPELL_SUMMON_SPARKLES = 69253,
	SPELL_DROWNING = 68730,

	GO_SPARKLES = 197333,

	DROWNING_WATCHMAN_RANDOM_SAY = 0,

	trigger = 34527,
};

class npc_mountainfoot_miner : public CreatureScript
{
public:
	npc_mountainfoot_miner() : CreatureScript("npc_mountainfoot_miner") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_mountainfoot_minerAI(creature);
	}

	struct npc_mountainfoot_minerAI : public ScriptedAI
	{
		npc_mountainfoot_minerAI(Creature* creature) : ScriptedAI(creature)
		{
			reset = true;
			despawn = false;
			exit = false;
			uiDespawnTimer = 10000;
		}

		uint32 uiDespawnTimer;
		bool reset;
		bool despawn;
		bool exit;

		void SpellHit(Unit* caster, const SpellInfo* spell)
		{
			if (spell->Id == 67032)
			{
				despawn = false;
				uiDespawnTimer = 10000;
				me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
				me->EnterVehicle(caster);
			}
			
			if (spell->Id == 67362)
			{
				if (Player* player = caster->ToPlayer())
				player->KilledMonsterCredit(35375, 0);
                const std::string monsterTexts[7] = { "The nightmare is over!", "I'm free!", "Give that man a raise!", "I'm outta here...",
                "That was surprisingly restful.", "No job is worth this!", "I can still clock in for the time I was petrified, right?" };
                me->MonsterSay(monsterTexts[urand(0, 6)].c_str(), LANG_UNIVERSAL, 0);
				me->SetFlag(UNIT_NPC_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveAllAuras();
                me->GetMotionMaster()->MoveRandom(15.0f);
                me->DespawnOrUnsummon(4500);
			}
		}

		void OnExitVehicle(Unit* /*vehicle*/, uint32 /*seatId*/)
		{
			if (!exit)
			{
				float x, y, z, o;
				me->GetPosition(x, y, z, o);
				me->SetHomePosition(x, y, z, o);
				me->Relocate(x, y, z, o);
				reset = true;
				despawn = true;
				Reset();
			}
		}

		void Reset()
		{
			exit = false;

			if (reset)
			{
				me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
				reset = false;
			}
		}

		void UpdateAI(uint32 const diff)
		{
			if (despawn)
			{
				if (uiDespawnTimer <= diff)
				{
					reset = true;
					despawn = false;
					uiDespawnTimer = 10000;
					me->DespawnOrUnsummon();
				}
				else
					uiDespawnTimer -= diff;
			}
		}
	};
};

class npc_cold_blood_trigger : public CreatureScript
{
public:
	npc_cold_blood_trigger() : CreatureScript("npc_cold_blood_trigger") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_cold_blood_triggerAI(creature);
	}

	struct npc_cold_blood_triggerAI : public ScriptedAI
	{
		npc_cold_blood_triggerAI(Creature* creature) : ScriptedAI(creature){ }

		void MoveInLineOfSight(Unit* who)
		{
			if (who->GetEntry() == 35257)
			{
				if (who->IsInWater() || !who->GetVehicle())
					return;

				if (who->GetDistance(2945.745117f, -4981.884766f, 125.976944f) < 10.0f)
					if (Unit* unit = who->GetVehicleBase())
					{
						if (Creature* watchman = who->ToCreature())
						{
							watchman->DespawnOrUnsummon(15000);
							CAST_AI(npc_mountainfoot_miner::npc_mountainfoot_minerAI, watchman->AI())->exit = true;
							CAST_AI(npc_mountainfoot_miner::npc_mountainfoot_minerAI, watchman->AI())->reset = true;
							who->ExitVehicle();
						}

						if (Player* player = unit->ToPlayer())
							player->CompleteQuest(14165); // once we reach our distance, lets complete the quest
					}
			}
		}

		void UpdateAI(uint32 const diff)
		{
			if (!UpdateVictim())
				return;

			DoMeleeAttackIfReady();
		}
	};
};

// Fix for spell 67688, for quest https://project-nightfall.org/bugtracker/issue/158/%5BAzshara%5D-Gunk-in-the-Trunk
class spell_gen_tinkering : public SpellScriptLoader
{
public:
    spell_gen_tinkering() : SpellScriptLoader("spell_gen_tinkering") { }

    class spell_gen_tinkering_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_tinkering_SpellScript);

        bool Validate(SpellInfo const* /*SpellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(67688))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            caster->CastSpell(caster, 67689, false);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_tinkering_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_gen_tinkering_SpellScript();
    }
};

void AddSC_azshara()
{
    new go_azshara_twilight_cage();
	new npc_mountainfoot_miner(); // Cold Blood & Refleshfication quest
	new npc_cold_blood_trigger(); // Cold Blood Quest
    new spell_gen_tinkering();
    /*new npc_azshara_malicion(); // Handled through Smart AI
    new npc_azshara_twilight_lord_katrana();*/ // Handled through Smart AI
}
