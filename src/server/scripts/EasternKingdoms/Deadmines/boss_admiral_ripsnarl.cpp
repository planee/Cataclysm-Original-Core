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
#include "SpellScript.h"
#include "deadmines.h"

enum Spells
{
   SPELL_RIPSNARL_CANON_KILL         = 95408, // hit 48266 NPC_DEFIAS_CANON 10:52:30.005
   SPELL_SWIPE                       = 88839, // 10:52:30.009 10:52:30.013
   SPELL_GO_FOR_THE_THROAT           = 88836,
   SPELL_THIRST_FOR_BLOOD            = 88736, // trigger SPELL_THIRST_FOR_BLOOD_TRIGGER
   SPELL_THIRST_FOR_BLOOD_TRIGGER    = 88737, // 10:52:30.010 10:52:30.012

   // heroic
   SPELL_THE_FOG                     = 88755,
   SPELL_SUMMON_VAPOR                = 88831,

   SPELL_THE_FOG_SCREEN_EFFECT       = 89247,

   // Vapors Spells ( Only Heroic)
   SPELL_CONDENSATION                = 92013,
   SPELL_SWIRLING_VAPOR              = 92007,
   SPELL_CONDENSING_VAPOR            = 92008,
   SPELL_FREEZING_VAPOR              = 92011,
   SPELL_COALESCE                    = 92042,

   SPELL_VAPOR_ACHIEVEMENT           = 95647,
   SPELL_VAPOR_AURA                  = 95503,
};

enum npcs
{
    NPC_ADMIRAL_RIPSNARL = 47626,
    NPC_DEFIAS_CANON     = 48266,
    NPC_VAPOR            = 47714,
    NPC_BUNNY_FOG        = 45979,
};

enum events
{
    EVENT_SWIPE = 1,
    EVENT_GO_FOR_THE_THROAT,
    EVENT_GO_INTO_FOG,
    EVENT_VAPOR_SUMMON,
    EVENT_GROUP_VAPOR,

    // vapor
    EVENT_CONDENSATION,
    EVENT_SWIRLING_VAPOR,
    EVENT_CONDENSING_VAPOR,
    EVENT_FREEZING_VAPOR,
    EVENT_COALESCE,
};

enum points
{
    POINT_ESCAPE_PLAYERS = 1,
};

class boss_admiral_ripsnarl : public CreatureScript
{
public:
    boss_admiral_ripsnarl() : CreatureScript("boss_admiral_ripsnarl") { }

    struct boss_admiral_ripsnarlAI : public BossAI
    {
        boss_admiral_ripsnarlAI(Creature* creature) : BossAI(creature, BOSS_ADMIRAL_RISNARL) { }

        void Reset()
        {
            canAttack = true;
            me->CastSpell(me, SPELL_THIRST_FOR_BLOOD, true);
            phase1 = false;
            phase2 = false;
            phase3 = false;
            phase4 = false;
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THE_FOG_SCREEN_EFFECT);
            if (Creature* fog = Unit::GetCreature(*me, _fogGUID))
                fog->RemoveAura(SPELL_THE_FOG);
            else if (Creature* fog = me->FindNearestCreature(NPC_BUNNY_FOG, 200.0f))
            {
                _fogGUID = fog->GetGUID();
                fog->RemoveAura(SPELL_THE_FOG);
            }
            me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            _Reset();
        }

        void EnterCombat(Unit * /*who*/)
        {
            DoCast(SPELL_RIPSNARL_CANON_KILL);
            Talk(0);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            events.ScheduleEvent(EVENT_SWIPE, 5000);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_GO_FOR_THE_THROAT, 10000);
            _EnterCombat();
        }

        void DamageTaken(Unit* caster, uint32& damage)
        {
            if ((me->HealthBelowPctDamaged(75, damage) && !phase1) || (me->HealthBelowPctDamaged(50, damage) && !phase2) || (me->HealthBelowPctDamaged(25, damage) && !phase3))
            {
                if (!phase1)
                {
                    Talk(2);
                    instance->DoCastSpellOnPlayers(SPELL_THE_FOG_SCREEN_EFFECT);
                    if (Creature* fog = Unit::GetCreature(*me, _fogGUID))
                        fog->CastSpell(fog, SPELL_THE_FOG, true);
                    events.ScheduleEvent(EVENT_VAPOR_SUMMON, 5000);
                    phase1 = true;
                }
                else if (!phase2)
                    phase2 = true;
                else if (!phase3)
                    phase3 = true;

                events.ScheduleEvent(EVENT_GO_INTO_FOG, 0);
            }

            if (IsHeroic() && me->HealthBelowPctDamaged(10, damage) && !phase4)
            {
                Talk(5);
                events.ScheduleEvent(EVENT_GROUP_VAPOR, 0);
                phase4 = true;
            }
        }

        void JustDied(Unit * /*killer*/)
        {
            Talk(6);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THE_FOG_SCREEN_EFFECT);
            if (Creature* fog = Unit::GetCreature(*me, _fogGUID))
                fog->RemoveAura(SPELL_THE_FOG);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            _JustDied();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE && id == POINT_ESCAPE_PLAYERS)
            {
                Talk(3);
                me->SetVisible(false);
            }
        }


        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SWIPE:
                        DoCastVictim(SPELL_SWIPE);
                        events.ScheduleEvent(EVENT_SWIPE, 5000);
                        break;
                    case EVENT_GO_FOR_THE_THROAT:
                        if (me->HasReactState(REACT_PASSIVE))
                        {
                            me->SetVisible(true);
                            events.CancelEvent(EVENT_VAPOR_SUMMON);
                            events.ScheduleEvent(EVENT_GROUP_VAPOR, 0);
                            events.ScheduleEvent(EVENT_VAPOR_SUMMON, 5000);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        }
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        {
                            canAttack = true;
                            me->NearTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ() + 0.5f, target->GetOrientation());
                            me->CastSpell(target, SPELL_GO_FOR_THE_THROAT, true);
                        }
                        events.ScheduleEvent(EVENT_GO_FOR_THE_THROAT, 10000);
                        break;
                    case EVENT_GO_INTO_FOG:
                    {
                        Talk(1);
                        canAttack = false;
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        events.CancelEvent(EVENT_GO_FOR_THE_THROAT);
                        events.CancelEvent(EVENT_SWIPE);
                        events.ScheduleEvent(EVENT_GO_FOR_THE_THROAT, 30000);
                        events.ScheduleEvent(EVENT_SWIPE, 40000);
                        Position pos, rPos;
                        me->GetPosition(&pos);
                        me->GetRandomPoint(pos, 15.0f, rPos);
                        me->GetMotionMaster()->MovePoint(POINT_ESCAPE_PLAYERS, rPos);
                        break;
                    }
                    case EVENT_GROUP_VAPOR:
                        DoCastRandom(SPELL_SUMMON_VAPOR, 0.0f);
                        DoCastRandom(SPELL_SUMMON_VAPOR, 0.0f);
                        DoCastRandom(SPELL_SUMMON_VAPOR, 0.0f);
                        break;
                    case EVENT_VAPOR_SUMMON:
                        DoCastRandom(SPELL_SUMMON_VAPOR, 0.0f);
                        events.ScheduleEvent(EVENT_VAPOR_SUMMON, 5000);
                        break;
                    default:
                        break;
                }
            }

            if (canAttack)
                DoMeleeAttackIfReady();
        }

        private :
            bool phase1, phase2, phase3, phase4, canAttack;
            uint64 _fogGUID = 0;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_admiral_ripsnarlAI (creature);
    }
};

// 95647
class npc_vapor_ripsnarl : public CreatureScript
{
    public:
        npc_vapor_ripsnarl() : CreatureScript("npc_vapor_ripsnarl") { }

        struct npc_vapor_ripsnarlAI : public ScriptedAI
        {
            npc_vapor_ripsnarlAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()) { }

            void Reset()
            {
                _events.Reset();
                validHF = false;
                me->CastSpell(me, SPELL_VAPOR_AURA, true);
				if (IsHeroic())
                _events.ScheduleEvent(EVENT_CONDENSATION, 8000);
                _events.ScheduleEvent(EVENT_SWIRLING_VAPOR, 2000);
                _events.ScheduleEvent(EVENT_CONDENSING_VAPOR, 2000);
                _events.ScheduleEvent(EVENT_FREEZING_VAPOR, 2000);
                _events.ScheduleEvent(EVENT_COALESCE, 2000);
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (spell && spell->Id == SPELL_COALESCE && !validHF)
                {
                    me->CastSpell(me, SPELL_VAPOR_ACHIEVEMENT, true);
                    validHF = true;
                }
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
                        case EVENT_CONDENSATION:
                            me->CastSpell(me, SPELL_CONDENSATION, true);
                            break;
                        case EVENT_SWIRLING_VAPOR:
                            DoCast(SPELL_SWIRLING_VAPOR);
                            _events.ScheduleEvent(EVENT_SWIRLING_VAPOR, 4000);
                            break;
                        case EVENT_CONDENSING_VAPOR:
                            DoCast(SPELL_CONDENSING_VAPOR);
                            _events.ScheduleEvent(EVENT_CONDENSING_VAPOR, 2000);
                            break;
                        case EVENT_FREEZING_VAPOR:
                            DoCast(SPELL_FREEZING_VAPOR);
                            _events.ScheduleEvent(EVENT_FREEZING_VAPOR, 2000);
                            break;
                        case EVENT_COALESCE:
                            DoCast(SPELL_COALESCE);
                            _events.ScheduleEvent(EVENT_COALESCE, 2000);
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private :
                InstanceScript *instance;
                EventMap _events;
                bool validHF;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_vapor_ripsnarlAI (creature);
        }
};

class spell_rp_thirst_for_blood : public SpellScriptLoader
{
    public:
        spell_rp_thirst_for_blood() : SpellScriptLoader("spell_rp_thirst_for_blood") { }

        class spell_rp_thirst_for_blood_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rp_thirst_for_blood_AuraScript);

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                if (Unit* target = GetCaster())
                {
                    if (target->HasSpellCooldown(SPELL_THIRST_FOR_BLOOD_TRIGGER))
                        return;

                    target->CastSpell(target, SPELL_THIRST_FOR_BLOOD_TRIGGER, true);
                    target->AddSpellCooldown(SPELL_THIRST_FOR_BLOOD_TRIGGER, 0, time(NULL) + 1);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_rp_thirst_for_blood_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rp_thirst_for_blood_AuraScript();
        }
};

void AddSC_boss_admiral_ripsnarl()
{
    new boss_admiral_ripsnarl();
    new npc_vapor_ripsnarl();
    new spell_rp_thirst_for_blood();
}
