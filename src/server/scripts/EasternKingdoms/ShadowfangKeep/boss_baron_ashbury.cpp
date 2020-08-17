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
#include "SpellScript.h"
#include "shadowfang_keep.h"

enum Spells
{
    // Normal Mode
    SPELL_ASPHYXIATE                       = 93423,
    SPELL_PAIN_AND_SUFFERING               = 93581,
    SPELL_PAIN_AND_SUFFERING_HC            = 93712,
    SPELL_STAY_OF_EXECUTION                = 93468,
    SPELL_STAY_OF_EXECUTION_HC             = 93705,

    // Heroic Mode
    SPELL_DARK_ARCHANGEL_FORM              = 93757,
    SPELL_CALAMITY                         = 93812,
    SPELL_WRACKING_PAIN                    = 93720,

    // Outro
    SPELL_CLEAVE                           = 87719,
};

enum Events
{
    EVENT_ASPHYXIATE                       = 1,
    EVENT_PAIN_AND_SUFFERING,
    EVENT_STAY_OF_EXECUTION,
    EVENT_WRACKING_PAIN,
    EVENT_CONTINUE_COMBAT
};

enum Texts
{
    SAY_AGGRO,
    SAY_DEATH,
    SAY_KILLER_1,
    SAY_KILLER_2,
    SAY_ARCHANGEL,
    SAY_STAYEXE,
    SAY_ASPHY
};

enum Misc
{
    POINT_EVENT_JUMP  = 1,
};

Position const AllianceOutroSpawnPositions[7] =
{
    { -239.936f,  2116.160f, 87.087f, 2.740f }, // 47006
    { -248.965f,  2113.260f, 87.096f, 2.984f }, // 47027
    { -246.389f,  2112.020f, 87.093f, 2.775f },
    { -247.602f,  2112.600f, 87.095f, 2.792f },
    { -227.457f,  2149.910f, 90.707f, 2.792f },
    { -229.707f,  2144.310f, 90.707f, 2.792f },
    { -235.010f,  2136.480f, 87.098f, 2.792f }
};

Position const AllianceOutroJumpPositions[4] =
{
    { -254.559f, 2121.700f, 81.179f, 7.212f },
    { -253.896f, 2116.940f, 81.179f, 7.212f }, // -248.965 Y: 2113.26 Z: 87.09624
    { -258.332f, 2119.020f, 81.179f, 7.323f }, // -246.389 Y: 2112.02 Z: 87.09373
    { -256.205f, 2118.070f, 81.179f, 7.212f }  // -247.602 Y: 2112.6 Z: 87.09563
};

uint32 const OutroMovementPathSize = 12;
const G3D::Vector3 OutroMovementPath[OutroMovementPathSize] =
{
    { -241.8400f, 2156.70f, 90.624f },
    { -253.9495f, 2124.95f, 81.651f },
    { -253.6995f, 2131.45f, 81.651f },
    { -252.1995f, 2135.70f, 83.651f },
    { -250.4495f, 2140.70f, 86.151f },
    { -248.9495f, 2143.95f, 87.151f },
    { -242.4495f, 2141.70f, 87.151f },
    { -237.9495f, 2140.45f, 87.151f },
    { -236.9495f, 2144.70f, 88.401f },
    { -235.4495f, 2150.95f, 90.901f },
    { -242.9495f, 2153.95f, 90.901f },
    { -241.8600f, 2156.54f, 90.624f }
};

uint32 const OutroMovementPathTwoSize = 5;
const G3D::Vector3 OutroMovementPathTwo[OutroMovementPathTwoSize] =
{
    { -228.681f, 2146.920f, 90.624f },
    { -241.260f, 2154.060f, 90.874f },
    { -237.260f, 2151.810f, 90.874f },
    { -233.260f, 2149.310f, 90.874f },
    { -228.654f, 2147.133f, 90.624f }
};

Position const HordeOutroSpawnPositions[5] =
{
    { -244.814f, 2127.150f, 87.012f, 3.682f },
    { -240.047f, 2143.160f, 87.096f, 4.450f },
    { -233.597f, 2140.910f, 87.096f, 4.398f },
    { -237.592f, 2150.780f, 90.707f, 4.555f },
    { -233.040f, 2149.040f, 90.707f, 4.345f }
};

Position const DeathStalkerJumpPos = { -256.849f, 2117.160f, 81.179f, 1.308f };

class DelayedJumpEvent : public BasicEvent
{
    public:
        DelayedJumpEvent(Creature* owner, Position const jumpPos) : _owner(owner), _jumpPos(jumpPos)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            if (++cnt == 1)
            {
                _owner->GetMotionMaster()->MoveJump(_jumpPos, 15.00f, 20.00f);
                _owner->m_Events.AddEvent(this, execTime + 1500);
                return false;
            }
            else
                _owner->SetFacingTo(_jumpPos.GetOrientation());
            return true;
        }

        private:
            Creature* _owner;
            Position const _jumpPos;
            uint8 cnt = 0;
};

class AllianceOutroEventAshbury : public BasicEvent
{
    public:
        AllianceOutroEventAshbury(Creature* owner) : _owner(owner)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            switch (++cnt)
            {
                case 1:
                    // Need to remove the flags, otherwise npc is talkable and will stop the event
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    _owner->GetMotionMaster()->MoveSmoothPath(OutroMovementPath, OutroMovementPathSize);
                    _owner->m_Events.AddEvent(this, execTime + 7000);
                    return false;
                case 2:
                    sCreatureTextMgr->SendBroadcast(_owner, 47365, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 3000);
                    return false;
                case 3:
                    _owner->CastSpell(_owner, SPELL_CLEAVE, true);
                    _owner->m_Events.AddEvent(this, execTime + 1000);
                    return false;
                case 4:
                    if (GameObject* door = _owner->FindNearestGameObject(GO_BARON_ASHBURY_DOOR, 10.00f))
                        door->SetGoState(GO_STATE_ACTIVE);
                    _owner->m_Events.AddEvent(this, execTime + 1000);
                    return false;
                case 5:
                    _owner->GetMotionMaster()->MoveSmoothPath(OutroMovementPathTwo, OutroMovementPathTwoSize);
                    _owner->m_Events.AddEvent(this, execTime + 2000);
                    // Path has ended - should now have a gossip and quest again.
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    return false;
                case 6:
                    _owner->SetFacingTo(2.7396f);
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

class HordeOutroEventAshbury : public BasicEvent
{
    public:
        HordeOutroEventAshbury(Creature* owner) : _owner(owner)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            switch (++cnt)
            {
                case 1:
                    // Need to remove the flags, otherwise npc is talkable and will stop the event
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    _owner->GetMotionMaster()->MoveSmoothPath(OutroMovementPath, OutroMovementPathSize);
                    _owner->m_Events.AddEvent(this, execTime + 9000);
                    return false;
                case 2:
                    sCreatureTextMgr->SendBroadcast(_owner, 47442, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 7000);
                    return false;
                case 3:
                    if (GameObject* door = _owner->FindNearestGameObject(GO_BARON_ASHBURY_DOOR, 10.00f))
                        door->SetGoState(GO_STATE_ACTIVE);
                    sCreatureTextMgr->SendBroadcast(_owner, 47443, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    // _owner->EmoteWithDelay(1, 69); need to investigate if it should play emote
                    _owner->m_Events.AddEvent(this, execTime + 4000);
                    return false;
                case 4:
                   // _owner->EmoteWithDelay(100, 0); need to investigate if it should play emote
                    _owner->GetMotionMaster()->MoveSmoothPath(OutroMovementPathTwo, OutroMovementPathTwoSize);
                    _owner->m_Events.AddEvent(this, execTime + 2000);
                    // Path has ended - should now have a gossip and quest again.
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    return false;
                case 5:
                    _owner->SetFacingTo(2.7396f);
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

class boss_baron_ashbury : public CreatureScript
{
    public:
        boss_baron_ashbury() : CreatureScript("boss_baron_ashbury") { }

        struct boss_baron_ashburyAI : public BossAI
        {
            boss_baron_ashburyAI(Creature* creature) : BossAI(creature, DATA_BARON_ASHBURY_EVENT) { }

            void Reset()
            {
                _Reset();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE);
                me->SetReactState(REACT_AGGRESSIVE);
                enraged = false;
                me->RemoveAllAuras();
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WRACKING_PAIN);
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);
                events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, 10000);
                events.ScheduleEvent(EVENT_ASPHYXIATE, 18000);
                if (IsHeroic()) // HC only. Shouldn't happen in the normal (low level version)
                events.ScheduleEvent(EVENT_WRACKING_PAIN, 60000);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE);
                Talk(SAY_DEATH);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DARK_ARCHANGEL_FORM);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WRACKING_PAIN);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CALAMITY);

                if (instance->GetData(TEAM_IN_INSTANCE) == ALLIANCE)
                {
                    if (Creature* packleader = me->SummonCreature(NPC_PACKLEADER_IVAR_BLOODFANG, AllianceOutroSpawnPositions[0]))
                    {
                        packleader->m_Events.AddEvent(new DelayedJumpEvent(packleader, AllianceOutroJumpPositions[0]), packleader->m_Events.CalculateTime(2000));
                        packleader->m_Events.AddEvent(new AllianceOutroEventAshbury(packleader), packleader->m_Events.CalculateTime(48000));
                        packleader->TalkWithDelay(10000, -68367, 0, CHAT_MSG_MONSTER_SAY);
                        packleader->TalkWithDelay(23000, -47100, 0, CHAT_MSG_MONSTER_SAY);
                        packleader->TalkWithDelay(34000, -47102, 0, CHAT_MSG_MONSTER_SAY);
                        packleader->TalkWithDelay(42000, -47103, 0, CHAT_MSG_MONSTER_SAY);
                    }

                    for (uint8 i = 1; i < 7; i++)
                    {
                        if (Creature* bloodfang = me->SummonCreature(NPC_BLOODFANG_BERSERKER, AllianceOutroSpawnPositions[i]))
                        {
                            if (i < 4)
                            {
                                if (i == 3)
                                    bloodfang->TalkWithDelay(22000, -47190, 0, CHAT_MSG_MONSTER_SAY);
                                bloodfang->m_Events.AddEvent(new DelayedJumpEvent(bloodfang, AllianceOutroJumpPositions[i]), bloodfang->m_Events.CalculateTime(500 + (i * 300)));
                            }
                        }
                    }

                }
                else
                {
                    if (Creature* deathstalker = me->SummonCreature(NPC_DEATHSTALKER_COMMANDER_BELMOT, HordeOutroSpawnPositions[0]))
                    {
                        deathstalker->RemoveAllAuras();
                        deathstalker->m_Events.AddEvent(new DelayedJumpEvent(deathstalker, DeathStalkerJumpPos), deathstalker->m_Events.CalculateTime(2000));
                        deathstalker->m_Events.AddEvent(new HordeOutroEventAshbury(deathstalker), deathstalker->m_Events.CalculateTime(17000));
                        deathstalker->TalkWithDelay(7000, -47440, 0, CHAT_MSG_MONSTER_SAY);
                        deathstalker->TalkWithDelay(14000, -47441, 0, CHAT_MSG_MONSTER_SAY);
                    }

                    for (uint8 i = 1; i < 5; i++)
                        me->SummonCreature(NPC_VETERAN_FORSAKEN_TROOPER, HordeOutroSpawnPositions[i]);
                }
            }

            void KilledUnit(Unit* victim)
            {
                Talk(RAND(SAY_KILLER_1, SAY_KILLER_2));
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
            {
                if (IsHeroic() && !enraged && HealthBelowPct(20))
                {
                    enraged = true;
                    Talk(SAY_ARCHANGEL);
                    DoCast(me, SPELL_DARK_ARCHANGEL_FORM);
                }
            }

            void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
            {
                for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    if (spell->Effects[i].Effect == SPELL_EFFECT_INTERRUPT_CAST || spell->Effects[i].Mechanic == MECHANIC_INTERRUPT)
                        me->SetReactState(REACT_AGGRESSIVE);
            }

            void UpdateAI(uint32 const diff)
            {
                if(!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                    case EVENT_ASPHYXIATE:
                        Talk(SAY_ASPHY);
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        DoCast(SPELL_ASPHYXIATE);
                        events.ScheduleEvent(EVENT_STAY_OF_EXECUTION, 6500);
                        events.ScheduleEvent(EVENT_ASPHYXIATE, urand(40000, 45000));
                        break;
                    case EVENT_PAIN_AND_SUFFERING:
                        DoCastRandom(SPELL_PAIN_AND_SUFFERING, 50, true);
                        events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, 30000);
                        break;
                    case EVENT_STAY_OF_EXECUTION:
                        Talk(SAY_STAYEXE);
                        DoCast(SPELL_STAY_OF_EXECUTION);
                        events.ScheduleEvent(EVENT_CONTINUE_COMBAT, 8500);
                        break;
                    case EVENT_WRACKING_PAIN:
                        DoCast(SPELL_WRACKING_PAIN);
                        events.ScheduleEvent(EVENT_WRACKING_PAIN, 45000);
                        break;
                    case EVENT_CONTINUE_COMBAT:
                        me->SetReactState(REACT_AGGRESSIVE);
                        break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                bool enraged;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_baron_ashburyAI(creature);
        }
};

class spell_ashbury_archangel : public SpellScriptLoader
{
    public:
        spell_ashbury_archangel() : SpellScriptLoader("spell_ashbury_archangel") { }

        class spell_ashbury_archangel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashbury_archangel_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (Unit* target = GetHitUnit())
                {
                    target->CastSpell(target, uint32(GetEffectValue()), true);
                    target->CastSpell(target, SPELL_CALAMITY, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_ashbury_archangel_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ashbury_archangel_SpellScript();
        }
};

class spell_ashbury_asphyxoate_periodic : public SpellScriptLoader
{
public:
    spell_ashbury_asphyxoate_periodic() : SpellScriptLoader("spell_ashbury_asphyxoate_periodic") { }

    class spell_ashbury_asphyxoate_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ashbury_asphyxoate_periodic_AuraScript);

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            InitialHealth = GetTarget()->GetHealth();
        }

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            PreventDefaultAction();

            if (aurEff->GetTickNumber() == 1)
                GetTarget()->CastSpell((Unit*)NULL, 93422); // ASPHYXIATE_STOP (stunned spell)

            int32 damage = int32((float)InitialHealth / 6.0f + 1.0f);

            if (damage >= int32(GetTarget()->GetHealth()))
                damage = GetTarget()->GetHealth() - 1;

            uint32 triggerSpell = GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell;
            GetTarget()->CastCustomSpell(triggerSpell, SPELLVALUE_BASE_POINT0, damage, GetTarget(), true, NULL, aurEff, GetCasterGUID());

        }

        int32 InitialHealth;

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_ashbury_asphyxoate_periodic_AuraScript::HandleEffectApply, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ashbury_asphyxoate_periodic_AuraScript::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ashbury_asphyxoate_periodic_AuraScript();
    }
};

void AddSC_boss_baron_ashbury()
{
    new boss_baron_ashbury();
    new spell_ashbury_archangel();
    new spell_ashbury_asphyxoate_periodic();
}
