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

#include "CreatureTextMgr.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "shadowfang_keep.h"

enum Spells
{
    SPELL_CONJURE_FROST_MIXTURE            = 93505,
    SPELL_CONJURE_POISONOUS_MIXTURE        = 93697,
    SPELL_CONJURE_MYSTERY_TOXIN            = 93563,
    SPELL_TOXIC_COAGULANT_GREEN            = 93572,
    SPELL_TOXIC_COAGULANT_RED              = 93573,
    SPELL_ICE_SHARDS                       = 93527,
    SPELL_TOXIC_COAGULANT_TRIGGERED        = 93617,
    SPELL_STEALTH                          = 1784,
};

enum Events
{
    EVENT_CONJURE_POISONOUS_MIXTURE        = 1,
    EVENT_CONJURE_FROST_MIXTURE,
    EVENT_CONJURE_MYSTERY_TOXIN,
    EVENT_ICE_SHARDS,
};

enum Texts
{
    SAY_AGGRO                              = 0,
    SAY_DEATH                              = 1,
    SAY_KILLER_1                           = 2,
    SAY_KILLER_2                           = 3,
};

Position const LeaderOutroSpawnPositionAshbury = { -172.883f, 2178.641f, 129.331f, 0.000f };
Position const WarlordJumpPosition = { -130.894f, 2166.810f, 138.697f, 0.000f };
Position const OutroGatePosition = { -134.169f, 2164.679f, 128.942f, 0.000f };
Position const HordeOutroDespawnPosition = { -118.906f, 2171.68f, 139.807f, 0.00f };
Position const WarlordSpawnPosition = { -137.908f, 2169.580f, 137.632f, 2.775f };

uint32 const LeaderMovePathSize = 6;
const G3D::Vector3 LeaderMovePath[LeaderMovePathSize] =
{
    { -175.323f, 2174.19f, 129.421f },
    { -173.059f, 2178.32f, 129.338f },
    { -170.795f, 2182.45f, 129.255f },
    { -164.780f, 2180.16f, 129.260f },
    { -153.594f, 2175.79f, 128.448f },
    { -142.101f, 2171.24f, 128.201f }
};

uint32 const WarlordMovePathSize = 3;
const G3D::Vector3 WarlordMovePath[WarlordMovePathSize] =
{
    { -130.133f, 2166.44f, 138.697f },
    { -121.577f, 2162.46f, 138.697f },
    { -118.906f, 2171.68f, 139.807f },
};

Position const VeteranSpawnPositions[4] =
{
    { -131.438f, 2174.300f, 138.779f, 3.473f },
    { -131.498f, 2170.570f, 138.779f, 3.124f },
    { -134.287f, 2164.360f, 138.779f, 2.373f },
    { -136.832f, 2161.719f, 138.779f, 2.059f }
};

Position const BerserkerSpawnPositions[8] =
{
    { -148.575f, 2178.920f, 128.284f, 5.078f },
    { -152.111f, 2170.610f, 128.284f, 0.366f },
    { -143.906f, 2167.560f, 128.284f, 1.972f },
    { -156.882f, 2177.600f, 128.779f, 5.864f },
    { -151.531f, 2162.950f, 128.779f, 1.082f },
    { -141.684f, 2183.090f, 128.779f, 4.293f },
    { -82.664f,  2157.446f, 155.795f, 3.281f },
    { -136.068f, 2168.840f, 128.779f, 2.722f },
};

class HordeOutroEventWalden : public BasicEvent
{
    public:
        HordeOutroEventWalden(Creature* owner) : _owner(owner)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            switch (++cnt)
            {
                case 1:
                    // Need to remove the flags, otherwise npc is talkable and will stop the event
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    _owner->RemoveAllAuras();
                    sCreatureTextMgr->SendBroadcast(_owner, 47486, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 5000);
                    return false;
                case 2:
                    if (Creature* warlord = _owner->FindNearestCreature(NPC_HIGH_WARLORD_CROMUSH, 100.00f))
                        sCreatureTextMgr->SendBroadcast(warlord, 47487, 0, CHAT_MSG_MONSTER_YELL, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 6000);
                    return false;
                case 3:
                    if (Creature* warlord = _owner->FindNearestCreature(NPC_HIGH_WARLORD_CROMUSH, 100.00f))
                        warlord->GetMotionMaster()->MoveJump(WarlordJumpPosition, 15.00f, 15.00f);
                    _owner->m_Events.AddEvent(this, execTime + 1000);
                    return false;
                case 4:
                {
                    if (Creature* warlord = _owner->FindNearestCreature(NPC_HIGH_WARLORD_CROMUSH, 100.00f))
                    {
                        std::list<Creature*> trooperList;
                        Trinity::AllCreaturesOfEntryInRange checker(warlord, NPC_VETERAN_FORSAKEN_TROOPER, 20.0f);
                        Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(warlord, trooperList, checker);
                        warlord->VisitNearbyObject(20.0f, searcher);

                        for (Creature* trooper : trooperList)
                        {
                            trooper->SetWalk(false);
                            trooper->GetMotionMaster()->MovePoint(0, HordeOutroDespawnPosition);
                        }

                        warlord->SetWalk(false);
                        warlord->GetMotionMaster()->MovePoint(0, HordeOutroDespawnPosition);
                    }

                    _owner->m_Events.AddEvent(this, execTime + 3000);
                    return false;
                }
                case 5:
                    if (Creature* warlord = _owner->FindNearestCreature(NPC_HIGH_WARLORD_CROMUSH, 100.00f))
                    {
                        std::list<Creature*> trooperList;
                        Trinity::AllCreaturesOfEntryInRange checker(warlord, NPC_VETERAN_FORSAKEN_TROOPER, 20.0f);
                        Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(warlord, trooperList, checker);
                        warlord->VisitNearbyObject(20.0f, searcher);

                        for (Creature* trooper : trooperList)
                            trooper->DespawnOrUnsummon();

                        warlord->DespawnOrUnsummon();
                    }
                    _owner->m_Events.AddEvent(this, execTime + 4000);
                    return false;
                case 6:
                    sCreatureTextMgr->SendBroadcast(_owner, 47489, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->SetWalk(true);
                    _owner->GetMotionMaster()->MovePoint(0, OutroGatePosition);
                    _owner->m_Events.AddEvent(this, execTime + 4000);
                    return false;
                case 7:
                    if (GameObject* gate = _owner->FindNearestGameObject(GO_SORCERS_GATE, 50.00f))
                        gate->SetGoState(GO_STATE_ACTIVE);
                    // Path has ended - should now have a gossip and quest again.
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    break;
                default:
                    break;
            }
            return true;
        }

        private:
            Creature* _owner;
            uint8 cnt = 0;
};

class AllianceOutroEventWalden : public BasicEvent
{
    public:
        AllianceOutroEventWalden(Creature* owner) : _owner(owner)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            switch (++cnt)
            {
                case 1:
                    // Need to remove the flags, otherwise npc is talkable and will stop the event
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    sCreatureTextMgr->SendBroadcast(_owner, 47366, 0, CHAT_MSG_MONSTER_YELL, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 7000);
                    return false;
                case 2:
                    sCreatureTextMgr->SendBroadcast(_owner, 47378, 0, CHAT_MSG_MONSTER_YELL, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 18000);
                    return false;
                case 3:
                    sCreatureTextMgr->SendBroadcast(_owner, 47384, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->SetWalk(true);
                    _owner->GetMotionMaster()->MovePoint(0, OutroGatePosition);
                    _owner->m_Events.AddEvent(this, execTime + 5000);
                    return false;
                case 4:
                    if (GameObject* gate = _owner->FindNearestGameObject(GO_SORCERS_GATE, 50.00f))
                        gate->SetGoState(GO_STATE_ACTIVE);
                    _owner->m_Events.AddEvent(this, execTime + 1000);
                    // Path has ended - should now have a gossip and quest again.
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    return false;
                case 5:
                    _owner->SetFacingTo(2.6830f);
                    break;
                default:
                    break;
            }
            return true;
        }

        private:
            Creature* _owner;
            uint8 cnt = 0;
};

class MovementEvent : public BasicEvent
{
    public:
        MovementEvent(Creature* owner) : _owner(owner)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            switch (++cnt)
            {
                case 1:
                {
                    Position pos(-141.369f, 2171.334f, 128.200f, 0.000f);
                    _owner->GetMotionMaster()->MovePoint(0, pos);
                    _owner->m_Events.AddEvent(this, execTime + 750);
                    return false;
                }
                case 2:
                    _owner->GetMotionMaster()->MoveJump(WarlordJumpPosition, 15.00f, 15.00f);
                    _owner->ToCreature()->DespawnOrUnsummon(2000);
                    break;
                default:
                    break;
            }
            return true;
        }

        private:
            Creature* _owner;
            uint8 cnt = 0;
};

class boss_lord_walden : public CreatureScript
{
    public:
        boss_lord_walden() : CreatureScript("boss_lord_walden") { }

        struct boss_lord_waldenAI : public BossAI
        {
            boss_lord_waldenAI(Creature* creature) : BossAI(creature, DATA_LORD_WALDEN_EVENT) { }

            void InitializeAI() override
            {
                if (me->isAlive())
                {
                    if (instance->GetData(TEAM_IN_INSTANCE) == HORDE)
                    {
                        for (uint8 i = 0; i < 4; i++)
                            if (Creature* trooper = me->SummonCreature(NPC_VETERAN_FORSAKEN_TROOPER, VeteranSpawnPositions[i]))
                                trooper->SetReactState(REACT_PASSIVE);

                        if (Creature* warlord = me->SummonCreature(NPC_HIGH_WARLORD_CROMUSH, WarlordSpawnPosition))
                            warlord->SetReactState(REACT_PASSIVE);
                    }
                }
                BossAI::InitializeAI();
            }

            void Reset()
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_CONJURE_FROST_MIXTURE, urand(10000, 20000));
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 25000);
                    events.ScheduleEvent(EVENT_CONJURE_POISONOUS_MIXTURE, 5000);
            }

            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH);
                _JustDied();

                if (instance->GetData(TEAM_IN_INSTANCE) == ALLIANCE)
                {
                    if (Creature* packleader = me->SummonCreature(NPC_PACKLEADER_IVAR_BLOODFANG, LeaderOutroSpawnPositionAshbury))
                    {
                        packleader->SetWalk(false);
                        packleader->GetMotionMaster()->MoveSmoothPath(LeaderMovePath, LeaderMovePathSize);
                        packleader->m_Events.AddEvent(new AllianceOutroEventWalden(packleader), packleader->m_Events.CalculateTime(7000));
                    }

                    for (uint8 i = 0; i < 8; i++)
                    {
                        if (Creature* berserk = me->SummonCreature(NPC_BLOODFANG_BERSERKER, BerserkerSpawnPositions[i]))
                        {
                            berserk->m_Events.AddEvent(new MovementEvent(berserk), berserk->m_Events.CalculateTime(15000 + (i * 500)));
                            berserk->CastSpell(berserk, SPELL_STEALTH, true);
                        }
                    }
                }
                else
                {
                    if (Creature* deathstalker = me->SummonCreature(NPC_DEATHSTALKER_COMMANDER_BELMOT, LeaderOutroSpawnPositionAshbury))
                    {
                        deathstalker->SetWalk(false);
                        deathstalker->GetMotionMaster()->MoveSmoothPath(LeaderMovePath, LeaderMovePathSize);
                        deathstalker->m_Events.AddEvent(new HordeOutroEventWalden(deathstalker), deathstalker->m_Events.CalculateTime(7000));
                    }
                }
            }

            void KilledUnit(Unit* victim)
            {
                Talk(RAND(SAY_KILLER_1, SAY_KILLER_2));
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == NPC_VETERAN_FORSAKEN_TROOPER || summon->GetEntry() == NPC_HIGH_WARLORD_CROMUSH)
                    return;

                BossAI::JustSummoned(summon);

                if (summon->GetEntry() == NPC_TOXIN_TRIGGER)
                {
                    float x, y, z;
                    me->GetPosition(x, y, z);
                    summon->SetCanFly(true);
                    summon->SetDisableGravity(true);
                    summon->NearTeleportTo(x, y, z+5.0f, 0.0f);

                    if (HealthAbovePct(35))
                    {
                        if (roll_chance_i(75))
                            summon->CastSpell(summon, SPELL_TOXIC_COAGULANT_GREEN, false);
                        else
                            summon->CastSpell(summon, SPELL_TOXIC_COAGULANT_RED, false);
                    }
                    else
                    {
                        if (roll_chance_i(75))
                            summon->CastSpell(summon, SPELL_TOXIC_COAGULANT_RED, false);
                        else
                            summon->CastSpell(summon, SPELL_TOXIC_COAGULANT_GREEN, false);
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if(!UpdateVictim())
                    return;

                events.Update(diff);

                if(me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                    case EVENT_ICE_SHARDS:
                        DoCast(SPELL_ICE_SHARDS);
                        break;
                    case EVENT_CONJURE_MYSTERY_TOXIN:
                        DoCast(SPELL_CONJURE_MYSTERY_TOXIN);
                        events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 45000);
                        events.ScheduleEvent(EVENT_ICE_SHARDS, 10000);
                        break;
                    case EVENT_CONJURE_FROST_MIXTURE:
                        DoCastRandom(SPELL_CONJURE_FROST_MIXTURE, 50);
                        events.ScheduleEvent(EVENT_CONJURE_FROST_MIXTURE, urand(10000, 20000));
                        break;
                    case EVENT_CONJURE_POISONOUS_MIXTURE:
                        DoCastRandom(SPELL_CONJURE_POISONOUS_MIXTURE, 50.0f);
                        events.ScheduleEvent(EVENT_CONJURE_POISONOUS_MIXTURE, urand(15000, 30000));
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_lord_waldenAI(creature);
        }
};

class spell_toxic_coagulant_green : public SpellScriptLoader
{
public:
    spell_toxic_coagulant_green() : SpellScriptLoader("spell_toxic_coagulant_green") { }

    class spell_toxic_coagulant_green_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_toxic_coagulant_green_AuraScript)

        void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
        {
            Unit* target = GetTarget();
            if (!target)
                return;

            if (target->isMoving())
                target->RemoveAuraFromStack(SPELL_TOXIC_COAGULANT_TRIGGERED);

            if (Aura* aura = GetAura())
                if (aura->GetStackAmount() == 3)
                {
                    target->CastSpell(target, 93660);
                    aura->Remove(AURA_REMOVE_BY_DEFAULT);
                }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_toxic_coagulant_green_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_toxic_coagulant_green_AuraScript();
    }
};

class NotMovingFilter
{
public:
    bool operator()(WorldObject* target) const
    {
        return target->ToUnit() && target->ToUnit()->isMoving() == false;
    }
};

class spell_toxic_coagulant_red : public SpellScriptLoader
{
public:
    spell_toxic_coagulant_red() : SpellScriptLoader("spell_toxic_coagulant_red") { }

    class spell_toxic_coagulant_red_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_toxic_coagulant_red_SpellScript);

        void FilterTargetsInitial(std::list<WorldObject*>& unitList)
        {
            unitList.remove_if(NotMovingFilter());

            targetList.clear();
            targetList.insert(targetList.end(), unitList.begin(), unitList.end());
        }

        void FilterTargetsSubsequential(std::list<WorldObject*>& unitList)
        {
            unitList.clear();
            unitList.insert(unitList.end(), targetList.begin(), targetList.end());
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_toxic_coagulant_red_SpellScript::FilterTargetsInitial, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_toxic_coagulant_red_SpellScript::FilterTargetsSubsequential, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_toxic_coagulant_red_SpellScript::FilterTargetsSubsequential, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
        }
        std::list<WorldObject*> targetList;
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_toxic_coagulant_red_SpellScript();
    }
};

void AddSC_boss_lord_walden()
{
    new boss_lord_walden();
    new spell_toxic_coagulant_green();
    new spell_toxic_coagulant_red();
}