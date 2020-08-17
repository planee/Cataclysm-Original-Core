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
    SPELL_CURSED_BULLETS                   = 93629,
    SPELL_MORTAL_WOUND                     = 93675,
    SPELL_PISTOL_BARRAGE                   = 93520,
    SPELL_PISTOL_BARRAGE_DAMAGE            = 93564,
    SPELL_PISTOL_BARRAGE_TARGETS           = 96343,
    SPELL_SUMMON_GHOUL_CHANNELED           = 93707,
    SPELL_SUMMON_GHOUL_1                   = 93709,
    SPELL_SUMMON_GHOUL_2                   = 93714,
    SPELL_ACHIEVEMENT_CREDIT               = 95929
};

enum Events
{
    EVENT_CURSED_BULLETS                   = 1,
    EVENT_MORTAL_WOUND,
    EVENT_PISTOL_BARRAGE,
    EVENT_SUMMON_BLOODTHIRSTY_GHOULS
};

enum eTexts
{
    SAY_AGGRO_A,
    SAY_AGGRO_H,
    SAY_DEATH,
    SAY_KILLER_1,
    SAY_KILLER_2
};

struct preEvent
{
    uint32 npcEntry;
    float x;
    float y;
    float z;
    float o;
};

static preEvent preEventSpawns [5] =
{
    { NPC_HIGH_WARLORD_CROMUSH, -110.026f, 2158.989f, 155.761f, 6.178f },
    { NPC_SPITEBONE_FLAYER,     -139.524f, 2166.899f, 155.761f, 3.001f },
    { NPC_SPITEBONE_FLAYER,     -123.457f, 2167.040f, 155.761f, 3.001f },
    { NPC_DREAD_SCRYER,         -124.629f, 2170.639f, 155.761f, 4.171f },
    { NPC_DREAD_SCRYER,         -142.750f, 2165.459f, 155.761f, 0.855f }
};

Position const LeaderSpawnPosition = { -169.020f, 2186.280f, 151.979f, 4.799f };
Position const DoorPosition = { -123.495f, 2164.065f, 155.928f, 0.000f };

uint32 const LeaderPathSize = 5;
const G3D::Vector3 LeaderPath[LeaderPathSize] =
{
    { -131.832f, 2166.28f, 155.678f },
    { -168.332f, 2181.78f, 152.428f },
    { -162.582f, 2179.28f, 152.428f },
    { -154.582f, 2176.03f, 155.428f },
    { -141.582f, 2171.03f, 155.928f }
};

class HordeIntroEventGodfrey : public BasicEvent
{
    public:
        HordeIntroEventGodfrey(Creature* owner) : _owner(owner)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            switch (++cnt)
            {
                case 1:
                    // Need to remove the flags, otherwise npc is talkable and will stop the event
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    _owner->SetWalk(true);
                    _owner->GetMotionMaster()->MovePoint(0, DoorPosition);
                    _owner->m_Events.AddEvent(this, execTime + 4000);
                    return false;
                case 2:
                    sCreatureTextMgr->SendBroadcast(_owner, 47500, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 7000);
                    return false;
                case 3:
                    if (GameObject* door = _owner->FindNearestGameObject(GO_LORD_GODFREY_DOOR, 50.00f))
                        door->SetGoState(GO_STATE_ACTIVE);
                    sCreatureTextMgr->SendBroadcast(_owner, 47502, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 6000);
                    return false;
                case 4:
                    if (Creature* warlord = _owner->FindNearestCreature(NPC_HIGH_WARLORD_CROMUSH, 100.00f))
                        sCreatureTextMgr->SendBroadcast(warlord, 47504, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 3000);
                    return false;
                case 5:
                    sCreatureTextMgr->SendBroadcast(_owner, 47508, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 2000);
                    return false;
                case 6:
                    sCreatureTextMgr->SendBroadcast(_owner, 47511, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
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

class AllianceIntroEventGodfrey : public BasicEvent
{
    public:
        AllianceIntroEventGodfrey(Creature* owner) : _owner(owner)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            switch (++cnt)
            {
                case 1:
                    // Need to remove the flags, otherwise npc is talkable and will stop the event
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    _owner->SetWalk(true);
                    _owner->GetMotionMaster()->MovePoint(0, DoorPosition);
                    _owner->m_Events.AddEvent(this, execTime + 4000);
                    return false;
                case 2:
                    sCreatureTextMgr->SendBroadcast(_owner, 47403, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 7000);
                    return false;
                case 3:
                    sCreatureTextMgr->SendBroadcast(_owner, 47404, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 2000);
                    return false;
                case 4:
                    if (GameObject* door = _owner->FindNearestGameObject(GO_LORD_GODFREY_DOOR, 50.00f))
                        door->SetGoState(GO_STATE_ACTIVE);
                    _owner->m_Events.AddEvent(this, execTime + 4000);
                    // Path has ended - should now have a gossip and quest again.
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    return false;
                case 5:
                    sCreatureTextMgr->SendBroadcast(_owner, 47418, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
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

class boss_lord_godfrey : public CreatureScript
{
    public:
        boss_lord_godfrey() : CreatureScript("boss_lord_godfrey") {}

        struct boss_lord_godfreyAI : public BossAI
        {
            boss_lord_godfreyAI(Creature* creature) : BossAI(creature, DATA_LORD_GODFREY_EVENT)
            {
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_PISTOL_BARRAGE_DAMAGE, true);
                me->ApplySpellImmune(0, IMMUNITY_ID, 93784, true);
            }

            void InitializeAI() override
            {
                if (instance->GetData(DATA_GODFREY_PRE_EVENT) == NOT_STARTED)
                {
                    for (uint8 i = 0; i < 5; i++)
                    {
                        if (i == 0 && instance->GetData(TEAM_IN_INSTANCE) == ALLIANCE)
                            continue;

                        me->SummonCreature(preEventSpawns[i].npcEntry, preEventSpawns[i].x, preEventSpawns[i].y, preEventSpawns[i].z, preEventSpawns[i].o);
                    }
                }
                BossAI::InitializeAI();
            }

            void Reset()
            {
                if (instance->GetBossState(DATA_GODFREY_PRE_EVENT) == DONE)
                    if (GameObject* door = me->FindNearestGameObject(GO_LORD_GODFREY_DOOR, 100.00f))
                        door->SetGoState(GO_STATE_ACTIVE);

                _Reset();
            }

            void EnterCombat(Unit* who)
            {
                _EnterCombat();
                Talk(instance->GetData(TEAM_IN_INSTANCE) == TEAM_HORDE ? SAY_AGGRO_H : SAY_AGGRO_A);
                events.ScheduleEvent(EVENT_SUMMON_BLOODTHIRSTY_GHOULS, 5000);
                events.ScheduleEvent(EVENT_MORTAL_WOUND, 25000);
                events.ScheduleEvent(EVENT_PISTOL_BARRAGE, 10000);
                events.ScheduleEvent(EVENT_CURSED_BULLETS, 15000);
                Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    i->getSource()->ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_CRITERIA_CONDITION_NONE, SPELL_ACHIEVEMENT_CREDIT, true);

                if (GameObject* door = me->FindNearestGameObject(GO_LORD_GODFREY_DOOR, 100.00f))
                    door->SetGoState(GO_STATE_READY);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_DEATH);

                if (GameObject* door = me->FindNearestGameObject(GO_LORD_GODFREY_DOOR, 100.00f))
                    door->SetGoState(GO_STATE_ACTIVE);

                // Horde version spawn at the end
                if (instance->GetData(TEAM_IN_INSTANCE) == HORDE)
                    me->SummonCreature(NPC_DEATHSTALKER_COMMANDER_BELMOT, -111.545921f, 2155.107666f, 155.678696f, 6.227050f, TEMPSUMMON_TIMED_DESPAWN, 4360000);
                
                // Alliance version spawn at the end
                if (instance->GetData(TEAM_IN_INSTANCE) == ALLIANCE)
                    me->SummonCreature(NPC_PACKLEADER_IVAR_BLOODFANG, -111.545921f, 2155.107666f, 155.678696f, 6.227050f, TEMPSUMMON_TIMED_DESPAWN, 4360000);

                instance->FinishLfgDungeon(me);
            }

            void KilledUnit(Unit* victim)
            {
                Talk(RAND(SAY_KILLER_1, SAY_KILLER_2));
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == NPC_HIGH_WARLORD_CROMUSH || summon->GetEntry() == NPC_SPITEBONE_FLAYER || summon->GetEntry() == NPC_DREAD_SCRYER)
                {
                    if (summon->GetEntry() != NPC_HIGH_WARLORD_CROMUSH)
                        aliveSummons++;
                    return;
                }

                BossAI::JustSummoned(summon);
            }

            void SummonedCreatureDies(Creature* summoned, Unit* killer)
            {
                if (IsHeroic() && summoned->GetEntry() == 50561 && killer == me) // Bullet Time Achievement
                    instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT_CREDIT);

                if (summoned->GetEntry() == NPC_SPITEBONE_FLAYER || summoned->GetEntry() == NPC_DREAD_SCRYER)
                {
                    if (--aliveSummons <= 0)
                    {
                        instance->SetData(DATA_GODFREY_PRE_EVENT, DONE);
                        if (instance->GetData(TEAM_IN_INSTANCE) == ALLIANCE)
                        {
                            if (Creature* bloodfang = me->SummonCreature(NPC_PACKLEADER_IVAR_BLOODFANG, LeaderSpawnPosition))
                            {
                                bloodfang->GetMotionMaster()->MoveSmoothPath(LeaderPath, LeaderPathSize);
                                bloodfang->m_Events.AddEvent(new AllianceIntroEventGodfrey(bloodfang), bloodfang->m_Events.CalculateTime(5000));
                            }
                        }
                        else
                        {
                            if (Creature* belmont = me->SummonCreature(NPC_DEATHSTALKER_COMMANDER_BELMOT, LeaderSpawnPosition))
                            {
                                belmont->GetMotionMaster()->MoveSmoothPath(LeaderPath, LeaderPathSize);
                                belmont->m_Events.AddEvent(new HordeIntroEventGodfrey(belmont), belmont->m_Events.CalculateTime(12000));
                            }
                        }
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if(me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_SUMMON_BLOODTHIRSTY_GHOULS:
                            DoCast(me, SPELL_SUMMON_GHOUL_CHANNELED, true);
                            events.DelayEvents(3000);
                            events.ScheduleEvent(EVENT_SUMMON_BLOODTHIRSTY_GHOULS, 30000);
                            return;
                        case EVENT_MORTAL_WOUND:
                            DoCastVictim(SPELL_MORTAL_WOUND, true);
                            events.ScheduleEvent(EVENT_MORTAL_WOUND, 7000);
                            break;
                        case EVENT_PISTOL_BARRAGE:
                            DoCast(SPELL_PISTOL_BARRAGE_TARGETS);
                            DoCastVictim(SPELL_PISTOL_BARRAGE);
                            events.ScheduleEvent(EVENT_PISTOL_BARRAGE, 30000);
                            break;
                        case EVENT_CURSED_BULLETS:
                            DoCastRandom(SPELL_CURSED_BULLETS, 100.0f);
                            events.ScheduleEvent(EVENT_CURSED_BULLETS, 15000);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                int8 aliveSummons = 0;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_lord_godfreyAI(creature);
        }
};

class spell_godfrey_summon_ghouls : public SpellScriptLoader
{
public:
    spell_godfrey_summon_ghouls() : SpellScriptLoader("spell_godfrey_summon_ghouls") { }

    class spell_godfrey_summon_ghouls_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_godfrey_summon_ghouls_AuraScript);

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            GetCaster()->CastSpell(GetCaster(), RAND(SPELL_SUMMON_GHOUL_1, SPELL_SUMMON_GHOUL_2), true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_godfrey_summon_ghouls_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_godfrey_summon_ghouls_AuraScript();
    }
};

class spell_pistol_barrage_trigger : public SpellScriptLoader
{
public:
    spell_pistol_barrage_trigger() : SpellScriptLoader("spell_pistol_barrage_trigger") { }

    class spell_pistol_barrage_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pistol_barrage_trigger_SpellScript);

        void SpawnInAngle(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            uint32 entry = uint32(GetSpellInfo()->Effects[effIndex].MiscValue);
            for(uint8 i = 0; i < 6; ++i)
            {
                float angle = -5 * M_PI / 24 + M_PI * i / 12;
                float x, y, z;
                GetCaster()->GetClosePoint(x, y, z, GetCaster()->GetObjectSize(), 30.0f, angle);
                GetCaster()->SummonCreature(entry, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(spell_pistol_barrage_trigger_SpellScript::SpawnInAngle, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pistol_barrage_trigger_SpellScript();
    }
};


class spell_pistol_barrage : public SpellScriptLoader
{
public:
    spell_pistol_barrage() : SpellScriptLoader("spell_pistol_barrage") { }

    class spell_pistol_barrage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pistol_barrage_AuraScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PISTOL_BARRAGE))
                return false;
            return true;
        }

        void OnPeriodic(AuraEffect const* aurEff)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell((Unit*)NULL, SPELL_PISTOL_BARRAGE_DAMAGE, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_pistol_barrage_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_pistol_barrage_AuraScript();
    }
};

class spell_cursed_bullets : public SpellScriptLoader
{
public:
    spell_cursed_bullets() : SpellScriptLoader("spell_cursed_bullets") { }

    class spell_cursed_bullets_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_cursed_bullets_AuraScript);

        void HandleEffectPeriodicUpdate(AuraEffect* aurEff)
        {
            uint32 bp = (GetOwner()->GetMap()->IsHeroic() ? 10000 : 50);
            aurEff->SetAmount(bp * aurEff->GetTickNumber());
        }

        void Register()
        {
            OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_cursed_bullets_AuraScript::HandleEffectPeriodicUpdate, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_cursed_bullets_AuraScript();
    }
};

void AddSC_boss_lord_godfrey()
{
    new boss_lord_godfrey();
    new spell_godfrey_summon_ghouls();
    new spell_pistol_barrage_trigger();
    new spell_pistol_barrage();
    new spell_cursed_bullets();
}
