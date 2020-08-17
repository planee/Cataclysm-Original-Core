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

#include "PassiveAI.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "deadmines.h"

enum Spells
{
    SPELL_SUMMON_CAULDRON           = 89250,
    SPELL_CAULDRON_SPAWN_VISUAL     = 89251,
    SPELL_CAULDRON_FIRE             = 89252,
    SPELL_THROW_MURLOC              = 90680,

    // food npcs
    SPELL_ROTTEN_AURA               = 89734,
    SPELL_FED                       = 89267,
    SPELL_ILLNESS                   = 89732,
    SPELL_GOOD_FOOD_AURA            = 89730,
    SPELL_BABY_MURLOC_DANCE         = 25165,

    SPELL_CAULDRON_FIRE_DAMAGE      = 89262,

    // Achievement
    SPELL_COOKIE_ACHIEVEMENT_AURA   = 95650,
};

uint32 throwFoodSpells[12] =
{
    90557,
    90560,
    90556,
    90559,
    89263,
    90555,
    90606,
    90603,
    89739,
    90605,
    90602,
    90604
};

enum Texts
{
   EMOTE_START             = 0,
};

enum Events
{
   EVENT_THROW_FOOD         = 1,
   EVENT_SUMMON_VEHICLE     = 2,
   EVENT_ENRAGE             = 3,
   EVENT_CHECK_WIPE         = 4,
};

enum npcs
{
    NPC_CAULDRON        = 47754,
    NPC_BABY_MURLOCK    = 48672,
};

enum points
{
    POINT_CAULDRON = 1,
};

class boss_captain_cookie : public CreatureScript
{
    public:
        boss_captain_cookie() : CreatureScript("boss_captain_cookie") {}

    struct boss_captain_cookieAI : public BossAI
    {
        boss_captain_cookieAI(Creature * creature) : BossAI(creature, BOSS_CAPTAIN_COOKIE)
        {
            me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_CAULDRON_FIRE_DAMAGE, true);
        }

        void Reset()
        {
            _eventStarted = false;
            _enrage = false;
			// DoCast(me, 89339); // Darkness spell
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            _Reset();
        }

        void MoveInLineOfSight(Unit* target) override
        {
            BossAI::MoveInLineOfSight(target);

            if (target->GetTypeId() != TYPEID_PLAYER || _eventStarted || !me->IsVisible())
                return;

            if (me->GetExactDist2d(target) <= 6.00f)
            {
				// DoCast(89339);
                _eventStarted = true;
                me->SetReactState(REACT_PASSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                me->GetMotionMaster()->MovePoint(POINT_CAULDRON, -64.46f, -820.76f, 41.14f, 0.0f);
                Talk(-48276, 0, CHAT_MSG_RAID_BOSS_EMOTE);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
			// me->RemoveAura(89339); // Darkness spell
            _EnterCombat();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (Creature* paper = me->FindNearestCreature(NPC_VANESSA_PAPER, 50.0f))
                paper->AI()->Talk(0);
            _JustDied();

            /// If we are not in heroic, finish LFG dungeon here
            if(!IsHeroic())
                if (InstanceScript* l_Instance = me->GetInstanceScript())
                    l_Instance->FinishLfgDungeon(me);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE && id == POINT_CAULDRON)
            {
                DoCastAOE(SPELL_COOKIE_ACHIEVEMENT_AURA);
                events.ScheduleEvent(EVENT_ENRAGE, 180000);
                events.ScheduleEvent(EVENT_THROW_FOOD, 5000);
                events.ScheduleEvent(EVENT_CHECK_WIPE, 1000);
                events.ScheduleEvent(EVENT_SUMMON_VEHICLE, 250);
            }
        }

        void JustSummoned(Creature* summon)
        {
            switch (summon->GetEntry())
            {
                case NPC_CAULDRON:
                    summon->CastSpell(summon, SPELL_CAULDRON_SPAWN_VISUAL, true);
                    summon->CastSpell(summon, SPELL_CAULDRON_FIRE, true);
                    break;
                case NPC_BABY_MURLOCK:
                    summon->CastSpell(summon, SPELL_BABY_MURLOC_DANCE, true);
                    break;
                default:
                    break;
            }
            BossAI::JustSummoned(summon);
        }

        void UpdateAI(uint32 const diff)
        {
            if (!me->HasReactState(REACT_PASSIVE) && !me->HasUnitMovementFlag(MOVEMENTFLAG_TRACKING_TARGET))
            {
                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    if (Unit* victim = me->SelectVictim())
                        AttackStart(victim);
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_THROW_FOOD:
                    {
                        DoCastRandom(_enrage ? SPELL_THROW_MURLOC : throwFoodSpells[urand(0,11)], 0.0f);
                        events.ScheduleEvent(EVENT_THROW_FOOD, 2000);
                        break;
                    }
                    case EVENT_SUMMON_VEHICLE:
                    {
                        Position pos(*me);
                        if (Creature* cauldron = me->SummonCreature(47754, pos))
                            me->EnterVehicle(cauldron, 0);
                        me->SetReactState(REACT_AGGRESSIVE);
                        break;
                    }
                    case EVENT_ENRAGE:
                        _enrage = true;
                        break;
                    case EVENT_CHECK_WIPE:
                    {
                        Player* player = 0;
                        Trinity::AnyPlayerInObjectRangeCheck u_check(me, 50.0f, true);
                        Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, player, u_check);
                        me->VisitNearbyObject(50.0f, searcher);
                        if (!player)
                        {
                            events.Reset();
                            summons.DespawnAll();
                            EnterEvadeMode();
                        }
                        else
                            events.ScheduleEvent(EVENT_CHECK_WIPE, 1000);
                        break;
                    }
                }
            }
        }

        private:
            bool _enrage;
            bool _eventStarted = false;
    };

    CreatureAI * GetAI(Creature * creature) const
    {
        return new boss_captain_cookieAI(creature);
    }
};

class npc_food : public CreatureScript
{
    public:
        npc_food() : CreatureScript("npc_food") {}

        struct npc_foodAI : public PassiveAI
        {
            npc_foodAI(Creature* creature) : PassiveAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_CAULDRON_FIRE_DAMAGE, true);
            }

            void Reset()
            {
                if (IsRottenFood())
                    me->AddAura(SPELL_ROTTEN_AURA, me);
                else
                    me->AddAura(SPELL_GOOD_FOOD_AURA, me);
            }

            void IsSummonedBy(Unit* /*who*/)
            {
                if (InstanceScript* instance = me->GetInstanceScript())
                    if (Creature *creature = Unit::GetCreature(*me, instance->GetData64(BOSS_CAPTAIN_COOKIE)))
                        creature->AI()->JustSummoned(me);
            }

            bool IsRottenFood()
            {
                switch (me->GetEntry())
                {
                    case 48302:
                    case 48295:
                    case 48276:
                    case 48299:
                    case 48293:
                    case 48298:
                        return true;
                }
                return false;
            }

            void OnSpellClick(Unit* player, bool& result)
            {
                if (Aura* grow = player->GetAura(me->GetMap()->IsHeroic() ? 92066 : 89732))
                {
                    if (grow->GetStackAmount() > 1)
                        player->RemoveAura(SPELL_COOKIE_ACHIEVEMENT_AURA);

                    if (!IsRottenFood())
                        grow->ModStackAmount(-1);
                }

                me->DespawnOrUnsummon(100);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_foodAI(creature);
        }
};

class npc_cc_baby_murlock : public CreatureScript
{
    public:
        npc_cc_baby_murlock() : CreatureScript("npc_cc_baby_murlock") {}

        struct npc_cc_baby_murlockAI : public PassiveAI
        {
            npc_cc_baby_murlockAI(Creature* creature) : PassiveAI(creature) {}

            void IsSummonedBy(Unit* /*who*/)
            {
                if (InstanceScript* instance = me->GetInstanceScript())
                    if (Creature* captain = Unit::GetCreature(*me, instance->GetData64(BOSS_CAPTAIN_COOKIE)))
                        captain->AI()->JustSummoned(me);
            }
        };

        CreatureAI* GetAI(Creature *pCreature) const
        {
            return new npc_cc_baby_murlockAI(pCreature);
        }
};

void AddSC_boss_captain_cookie()
{
    new boss_captain_cookie();
    new npc_food();
    new npc_cc_baby_murlock();
}
