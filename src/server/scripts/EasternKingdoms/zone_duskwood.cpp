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
SDName: Duskwood
SD%Complete: 100
SDComment: Quest Support:8735
SDCategory: Duskwood
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"

enum Yells
{
    YELL_TWILIGHTCORRUPTOR_RESPAWN                                  = 0,
    YELL_TWILIGHTCORRUPTOR_AGGRO                                    = 1,
    YELL_TWILIGHTCORRUPTOR_KILL                                     = 2,
};


/*######
# at_twilight_grove
######*/

class at_twilight_grove : public AreaTriggerScript
{
public:
    at_twilight_grove() : AreaTriggerScript("at_twilight_grove") { }

    bool OnTrigger(Player* player, const AreaTriggerEntry* /*at*/)
    {
        if (player->HasQuestForItem(21149))
        {
            if (Unit* TCorrupter = player->SummonCreature(15625, -10328.16f, -489.57f, 49.95f, 0, TEMPSUMMON_MANUAL_DESPAWN, 60000))
            {
                TCorrupter->setFaction(14);
                TCorrupter->SetMaxHealth(832750);
            }
            if (Creature* CorrupterSpeaker = player->SummonCreature(1, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ()-1, 0, TEMPSUMMON_TIMED_DESPAWN, 15000))
            {
                CorrupterSpeaker->SetName("Twilight Corrupter");
                CorrupterSpeaker->SetVisible(true);
                CorrupterSpeaker->AI()->Talk(YELL_TWILIGHTCORRUPTOR_RESPAWN, player->GetGUID());
            }
        }
        return false;
    };
};

/*######
# boss_twilight_corrupter
######*/

#define SPELL_SOUL_CORRUPTION 25805
#define SPELL_CREATURE_OF_NIGHTMARE 25806
#define SPELL_LEVEL_UP 24312

class boss_twilight_corrupter : public CreatureScript
{
public:
    boss_twilight_corrupter() : CreatureScript("boss_twilight_corrupter") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_twilight_corrupterAI (creature);
    }

    struct boss_twilight_corrupterAI : public ScriptedAI
    {
        boss_twilight_corrupterAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 SoulCorruption_Timer;
        uint32 CreatureOfNightmare_Timer;
        uint8 KillCount;

        void Reset()
        {
            SoulCorruption_Timer = 15000;
            CreatureOfNightmare_Timer = 30000;
            KillCount = 0;
        }
        void EnterCombat(Unit* /*who*/)
        {
            Talk(YELL_TWILIGHTCORRUPTOR_AGGRO);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
            {
                ++KillCount;
                Talk(YELL_TWILIGHTCORRUPTOR_KILL, victim->GetGUID());

                if (KillCount == 3)
                {
                    DoCast(me, SPELL_LEVEL_UP, true);
                    KillCount = 0;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (SoulCorruption_Timer <= diff)
            {
                DoCastVictim(SPELL_SOUL_CORRUPTION);
                SoulCorruption_Timer = rand()%4000+15000; //gotta confirm Timers
            } else SoulCorruption_Timer-=diff;

            if (CreatureOfNightmare_Timer <= diff)
            {
                DoCastVictim(SPELL_CREATURE_OF_NIGHTMARE);
                CreatureOfNightmare_Timer = 45000; //gotta confirm Timers
            } else CreatureOfNightmare_Timer-=diff;
            DoMeleeAttackIfReady();
        };
    };
};

enum PotWorgen
{
    SPELL_HARRIS_AMPULE = 82058
};

class npc_lurking_potion : public CreatureScript
{
public:
    npc_lurking_potion() : CreatureScript("npc_lurking_potion") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lurking_potionAI(pCreature);
    }

    struct npc_lurking_potionAI : public ScriptedAI
    {
        npc_lurking_potionAI(Creature* creature) : ScriptedAI(creature) {}

        bool bCast;
        bool bSay;

        void Reset()
        {
            bSay = false;
            bCast = false;
            me->SetRespawnTime(10);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        }

        void SpellHit(Unit* caster, const SpellInfo* pSpell)
        {
            if (pSpell->Id == SPELL_HARRIS_AMPULE && caster->GetTypeId() == TYPEID_PLAYER && HealthBelowPct(25))
            {
                caster->ToPlayer()->KilledMonsterCredit(43860, 0);
                me->DespawnOrUnsummon(2500);
            }
        }
        void UpdateAI(const uint32 uiDiff)
        {
            if (Player* pPlayer = me->FindNearestPlayer(10.0f))
            {
                if (pPlayer->GetQuestStatus(26720) == QUEST_STATUS_INCOMPLETE)
                {
                    if (!bCast)
                    {
                        me->CastSpell(pPlayer, 81957, true);
                        me->Attack(pPlayer, true);
                        bCast = true;
                    }


                    if (HealthBelowPct(25))
                    {
                        if (!bSay)
                        {
                            me->MonsterTextEmote("The worgen stares and hesitates!", 0, true);
                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                            pPlayer->KilledMonsterCredit(43860, 0);// Hack since the spell hit not working
                            me->DespawnOrUnsummon(2500);
                            bSay = true;
                        }
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class item_harris_ampule : public ItemScript
{
    enum eNpcs
    {
        NpcLurkingWorgen = 43814,
    };

    enum eSpells
    {
        SpellHarrisAmpule = 82058,
    };

    public:
        item_harris_ampule() : ItemScript("item_harris_ampule") { }

        bool OnUse(Player* p_Player, Item* /*p_Item*/, SpellCastTargets const& p_Targets) override
        {
            if (Creature* l_Worgen = p_Player->FindNearestCreature(eNpcs::NpcLurkingWorgen, 25.0f))
            {
                if (l_Worgen->HealthBelowPct(30))
                    return false;

                p_Player->GetSession()->SendNotification("The Lurking Worgen is not weak enough!");
                return true;
            }

            p_Player->GetSession()->SendNotification("You can't use this right now!");
            return true;
        }
};

void AddSC_duskwood()
{
    new boss_twilight_corrupter();
    new at_twilight_grove();
    new npc_lurking_potion();
    new item_harris_ampule();
}
