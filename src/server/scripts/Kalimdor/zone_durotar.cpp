/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"

/*######
## Quest 25134: Lazy Peons
## npc_lazy_peon
######*/


class npc_lazy_peon : public CreatureScript
{
public:
    enum eLazyPeonYells
    {
        SAY_SPELL_HIT = 0
    };

    enum eEvents
    {
        EVENT_GO_TO_SLEEP = 1,
        EVENT_WORK,
    };

    enum eLazyPeon
    {
        QUEST_LAZY_PEONS = 25134,
        GO_LUMBERPILE = 175784,
        SPELL_BUFF_SLEEP = 17743,
        SPELL_AWAKEN_PEON = 19938
    };

    enum ePoints
    {
        POINT_GO_BACK_TO_WORK = 1,
    };

    npc_lazy_peon() : CreatureScript("npc_lazy_peon") { }

    struct npc_lazy_peonAI : public ScriptedAI
    {
        npc_lazy_peonAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset()
        {
            _nonCombatEvents.ScheduleEvent(EVENT_GO_TO_SLEEP, 1000);
            _nonCombatEvents.ScheduleEvent(EVENT_WORK, 1000);
            _work = false;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type == POINT_MOTION_TYPE)
                if (id == POINT_GO_BACK_TO_WORK)
                    _work = true;
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_AWAKEN_PEON)
                if (Player* l_Player = caster->ToPlayer())
                    if(l_Player->GetQuestStatus(QUEST_LAZY_PEONS) == QUEST_STATUS_INCOMPLETE)
                    {
                        l_Player->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
                        Talk(SAY_SPELL_HIT, caster->GetGUID());
                        me->RemoveAllAuras();

                        if (GameObject* l_LumberPile = me->FindNearestGameObject(GO_LUMBERPILE, 20))
                        {
                            Position l_Position;
                            l_LumberPile->GetPosition(&l_Position);
                            l_Position.m_positionX = l_Position.m_positionX - 1;
                            me->GetMotionMaster()->MovePoint(POINT_GO_BACK_TO_WORK, l_Position);
                        }
                    }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            _nonCombatEvents.Update(p_Diff);

            if (uint32 l_EventID = _nonCombatEvents.ExecuteEvent())
            {
                if (l_EventID == EVENT_GO_TO_SLEEP)
                {
                    _work = false;
                    DoCast(me, SPELL_BUFF_SLEEP);
                    _nonCombatEvents.ScheduleEvent(EVENT_GO_TO_SLEEP, 300000);
                }

                if (l_EventID == EVENT_WORK)
                {
                    if(_work)
                        me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD);

                    _nonCombatEvents.ScheduleEvent(EVENT_WORK, 1000);
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _nonCombatEvents;
        bool _work;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_lazy_peonAI(creature);
    }
};

enum VoodooSpells
{
    SPELL_BREW      = 16712, // Special Brew
    SPELL_GHOSTLY   = 16713, // Ghostly
    SPELL_HEX1      = 16707, // Hex
    SPELL_HEX2      = 16708, // Hex
    SPELL_HEX3      = 16709, // Hex
    SPELL_GROW      = 16711, // Grow
    SPELL_LAUNCH    = 16716, // Launch (Whee!)
};

// 17009
class spell_voodoo : public SpellScriptLoader
{
    public:
        spell_voodoo() : SpellScriptLoader("spell_voodoo") {}

        class spell_voodoo_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_voodoo_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_BREW) || !sSpellMgr->GetSpellInfo(SPELL_GHOSTLY) ||
                    !sSpellMgr->GetSpellInfo(SPELL_HEX1) || !sSpellMgr->GetSpellInfo(SPELL_HEX2) ||
                    !sSpellMgr->GetSpellInfo(SPELL_HEX3) || !sSpellMgr->GetSpellInfo(SPELL_GROW) ||
                    !sSpellMgr->GetSpellInfo(SPELL_LAUNCH))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint32 spellid = RAND(SPELL_BREW, SPELL_GHOSTLY, RAND(SPELL_HEX1, SPELL_HEX2, SPELL_HEX3), SPELL_GROW, SPELL_LAUNCH);
                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, spellid, false);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_voodoo_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_voodoo_SpellScript();
        }
};

void AddSC_durotar()
{
    new npc_lazy_peon();
    new spell_voodoo();
}
