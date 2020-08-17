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
    SPELL_SUMMON_WORGEN_SPIRIT          = 93857,
    SPELL_VEIL_OF_SHADOW                = 23224,
    SPELL_CURSED_VEIL                   = 93956,
    SPELL_SUMMON_SPIRIT_OF_ODO_MISSILE  = 93859,
    SPELL_SUMMON_SPIRIT_OF_ODO          = 93864,
    SPELL_SUMMON_NANDO_MISSILE          = 93896,
    SPELL_SUMMON_NANDO                  = 93899,
    SPELL_SUMMON_BUTCHER_MISSILE        = 93921,
    SPELL_SUMMON_BUTCHER                = 93924,
    SPELL_SUMMON_RETHILGORE_MISSILE     = 93925,
    SPELL_SUMMON_RETHILGORE             = 93927,

    // Odo
    SPELL_HOWLING_RAGE                  = 93931,
    SPELL_BLINDING_SHADOWS              = 93952,

    // Butcher
    SPELL_SPECTRAL_RUSH                 = 93914,
    SPELL_BUTCHER_DRAIN                 = 7486,

    // Rethilgore
    SPELL_SOUL_DRAIN                    = 93863,

    // Wolf Master Nandos
    SPELL_SUMMON_LUPINE_SPIRIT          = 94199,
    SPELL_CLAW                          = 91776,

    // Warlord
    SPELL_TELEPORT_VISUAL               = 34776,
};

enum Events
{
    EVENT_VEIL_OF_SHADOW                = 1,
    EVENT_CURSED_VEIL                   = 2,

    // Odo
    EVENT_HOWLING_RAGE                  = 3,
    EVENT_BLINDING_SHADOWS              = 4,

    // Butcher
    EVENT_SPECTRAL_RUSH                 = 5,

    // Rethilgore
    EVENT_SOUL_DRAIN                    = 6,

    // Wolf Master Nandos
    EVENT_CLAW                          = 7,
};

enum Texts
{
    SAY_AGGRO                           = 0,
    SAY_DEATH                           = 1,
    SAY_KILLER_1                        = 2,
    SAY_KILLER_2                        = 3
};

enum Summons
{
    NPC_WORGEN_SPIRIT_ODO               = 50934,
    NPC_WORGEN_SPIRIT_NANDO             = 51047,
    NPC_WORGEN_SPIRIT_BUTCHER           = 51080,
    NPC_WORGEN_SPIRIT_RETHILGORE        = 51085,
};

Position const VeteranSpawnPositions [4] =
{
    { -287.351f, 2303.370f, 90.688f, 0.643f }, // 47030
    { -287.839f, 2301.810f, 90.688f, 4.768f },
    { -288.764f, 2303.980f, 90.690f, 0.572f },
    { -289.359f, 2302.430f, 90.690f, 4.921f },
};

uint32 const VeteranMovementPathOneSize = 7;
const G3D::Vector3 VeteranMovementPathOne[VeteranMovementPathOneSize] =
{
    { -291.244f, 2300.450f, 93.347f },
    { -287.351f, 2303.370f, 90.688f },
    { -283.458f, 2306.290f, 88.030f },
    { -277.922f, 2307.350f, 83.988f },
    { -271.365f, 2306.200f, 81.358f },
    { -267.021f, 2304.470f, 79.602f },
    { -259.259f, 2301.610f, 75.433f }
};

uint32 const VeteranMovementPathTwoSize = 6;
const G3D::Vector3 VeteranMovementPathTwo[VeteranMovementPathTwoSize] =
{
    { -288.286f, 2309.700f, 95.037f },
    { -287.839f, 2301.810f, 90.688f },
    { -287.392f, 2293.920f, 86.340f },
    { -281.845f, 2289.350f, 82.034f },
    { -275.943f, 2287.120f, 80.583f },
    { -266.472f, 2283.320f, 75.437f }
};

uint32 const VeteranMovementPathThreeSize = 5;
const G3D::Vector3 VeteranMovementPathThree[VeteranMovementPathThreeSize] =
{
    { -294.502f, 2300.280f, 93.998f },
    { -288.764f, 2303.980f, 90.690f },
    { -283.026f, 2307.680f, 87.382f },
    { -277.884f, 2308.680f, 83.987f },
    { -269.799f, 2305.930f, 81.175f }
};

uint32 const VeteranMovementPathFourSize = 5;
const G3D::Vector3 VeteranMovementPathFour[VeteranMovementPathFourSize] =
{
    { -291.150f, 2310.890f, 94.959f },
    { -289.359f, 2302.430f, 90.690f },
    { -287.568f, 2293.970f, 86.420f },
    { -281.344f, 2289.050f, 81.742f },
    { -276.958f, 2287.400f, 81.110f }
};

Position const CommanderSpawnPosition = { -260.385f, 2290.040f, 75.082f, 2.775f };
Position const CromushSpawnPosition = { -269.828f, 2293.459f, 77.568f, 5.951f };

Position const PackleaderSpawnPosition = { -276.547f, 2298.070f, 96.826f, 5.873f };
Position const PackleaderJumpPosition = { -268.177f, 2294.440f, 77.487f, 0.000f };

Position const BloodfangBerserkerSpawnPosition [6] =
{
    { -287.359f, 2313.400f, 92.714f, 4.398f },
    { -293.950f, 2316.140f, 92.783f, 4.433f },
    { -292.418f, 2310.590f, 90.834f, 4.345f },
    { -216.639f, 2279.100f, 95.984f, 2.986f },
    { -231.135f, 2275.780f, 96.042f, 2.454f },
    { -242.517f, 2281.800f, 96.408f, 2.640f }
};

Position const BloodfangJumpPositions [3] =
{
    { -234.196f, 2281.850f, 76.297f, 0.000f },
    { -240.969f, 2283.850f, 76.320f, 0.000f },
    { -255.816f, 2289.090f, 76.344f, 0.000f }
};

class HordeOutroEvent : public BasicEvent
{
    public:
        HordeOutroEvent(Creature* owner) : _owner(owner)  {}

        bool Execute(uint64 execTime, uint32 /*diff*/)
        {
            switch (++cnt)
            {
                case 1:
                    // Need to remove the flags, otherwise npc is talkable and will stop the event
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    _owner->RemoveAllAuras();
                    sCreatureTextMgr->SendBroadcast(_owner, 47466, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 9000);
                    return false;
                case 2:
                    _warlord = _owner->SummonCreature(NPC_HIGH_WARLORD_CROMUSH, CromushSpawnPosition);
                    if (_warlord)
                        _warlord->CastSpell(_warlord, SPELL_TELEPORT_VISUAL, true);
                    _owner->m_Events.AddEvent(this, execTime + 3000);
                    return false;
                case 3:
                    sCreatureTextMgr->SendBroadcast(_owner, 47467, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 3000);
                    return false;
                case 4:
                    if (_warlord)
                        sCreatureTextMgr->SendBroadcast(_warlord, 47468, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 5000);
                    return false;
                case 5:
                    sCreatureTextMgr->SendBroadcast(_owner, 47469, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 9000);
                    return false;
                case 6:
                    sCreatureTextMgr->SendBroadcast(_owner, 47470, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 3000);
                    return false;
                case 7:
                    if (_warlord)
                        sCreatureTextMgr->SendBroadcast(_warlord, 47471, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 6000);
                    return false;
                case 8:
                    if (_warlord)
                        sCreatureTextMgr->SendBroadcast(_warlord, 47472, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 5000);
                    return false;
                case 9:
                    if (_warlord)
                        sCreatureTextMgr->SendBroadcast(_warlord, 47473, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    _owner->m_Events.AddEvent(this, execTime + 9000);
                    // Path has ended - should now have a gossip and quest again.
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _owner->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    return false;
                case 10:
                    sCreatureTextMgr->SendBroadcast(_owner, 47479, 0, CHAT_MSG_MONSTER_SAY, TEXT_RANGE_NORMAL);
                    break;
                default:
                    break;
            }
            return true;
        }

        private:
            Creature* _owner;
            Creature* _warlord;
            uint8 cnt = 0;
};

class boss_baron_silverlaine : public CreatureScript
{
    public:
        boss_baron_silverlaine() : CreatureScript("boss_baron_silverlaine") { }

        struct boss_baron_silverlaineAI : public BossAI
        {
            boss_baron_silverlaineAI(Creature* creature) : BossAI(creature, DATA_BARON_SILVERLAINE_EVENT) { }

            void Reset()
            {
                _Reset();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE);
                _firstGhostTriggered = false;
                _secondGhostTriggered = false;
                _thirdGhostTriggered = false;
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);
                _EnterCombat();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE);

                events.ScheduleEvent(EVENT_VEIL_OF_SHADOW, 10000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_CURSED_VEIL, 10000);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE);
                Talk(SAY_DEATH);

                if (instance->GetData(TEAM_IN_INSTANCE) == ALLIANCE)
                {
                    if (Creature* packleader = me->SummonCreature(NPC_PACKLEADER_IVAR_BLOODFANG, PackleaderSpawnPosition))
                    {
                        packleader->GetMotionMaster()->MoveJump(PackleaderJumpPosition, 15.00f, 20.00f);
                        packleader->TalkWithDelay(3000, -47157, 0);
                    }


                    for (uint8 i = 0; i < 6; i++)
                    {
                        if (Creature* bloodfang = me->SummonCreature(NPC_BLOODFANG_BERSERKER, BloodfangBerserkerSpawnPosition[i]))
                        {
                            switch (i)
                            {
                                case 3:
                                    bloodfang->GetMotionMaster()->MoveJump(BloodfangJumpPositions[0], 15.00f, 20.00f);
                                    break;
                                case 4:
                                    bloodfang->GetMotionMaster()->MoveJump(BloodfangJumpPositions[1], 15.00f, 20.00f);
                                    break;
                                case 5:
                                    bloodfang->GetMotionMaster()->MoveJump(BloodfangJumpPositions[2], 15.00f, 20.00f);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
                else
                {
                    if (Creature* commander = me->SummonCreature(NPC_DEATHSTALKER_COMMANDER_BELMOT, CommanderSpawnPosition))
                        commander->m_Events.AddEvent(new HordeOutroEvent(commander), commander->m_Events.CalculateTime(2000));

                    for (uint8 i = 0; i < 4; i++)
                    {
                        if (Creature* veteran = me->SummonCreature(NPC_VETERAN_FORSAKEN_TROOPER, VeteranSpawnPositions[i]))
                        {
                            veteran->SetWalk(false);
                            switch (i)
                            {
                                case 0:
                                    veteran->GetMotionMaster()->MoveSmoothPath(VeteranMovementPathOne, VeteranMovementPathOneSize);
                                    break;
                                case 1:
                                    veteran->GetMotionMaster()->MoveSmoothPath(VeteranMovementPathTwo, VeteranMovementPathTwoSize);
                                    break;
                                case 2:
                                    veteran->GetMotionMaster()->MoveSmoothPath(VeteranMovementPathThree, VeteranMovementPathThreeSize);
                                    break;
                                case 3:
                                    veteran->GetMotionMaster()->MoveSmoothPath(VeteranMovementPathFour, VeteranMovementPathFourSize);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }

            void JustSummoned(Creature* summon)
            {
                switch (summon->GetEntry())
                {
                    case NPC_WORGEN_SPIRIT_ODO:
                        summon->CastSpell(summon, SPELL_SUMMON_SPIRIT_OF_ODO, true);
                        break;
                    case NPC_WORGEN_SPIRIT_NANDO:
                        summon->CastSpell(summon, SPELL_SUMMON_NANDO, true);
                        break;
                    case NPC_WORGEN_SPIRIT_BUTCHER:
                        summon->CastSpell(summon, SPELL_SUMMON_BUTCHER, true);
                        break;
                    case NPC_WORGEN_SPIRIT_RETHILGORE:
                        summon->CastSpell(summon, SPELL_SUMMON_RETHILGORE, true);
                        break;
                    default:
                        break;
                }
                BossAI::JustSummoned(summon);
            }

            void KilledUnit(Unit* victim)
            {
                Talk(RAND(SAY_KILLER_1, SAY_KILLER_2));
            }

            void DamageTaken(Unit* /*dealer*/, uint32& damage)
            {
                if (IsHeroic())
                {
                    if (!_firstGhostTriggered && me->HealthBelowPctDamaged(90, damage))
                    {
                        _firstGhostTriggered = true;
                        DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
                    }
                    else if (!_secondGhostTriggered && me->HealthBelowPctDamaged(60, damage))
                    {
                        _secondGhostTriggered = true;
                        DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
                    }
                    else if (!_thirdGhostTriggered && me->HealthBelowPctDamaged(30, damage))
                    {
                        _thirdGhostTriggered = true;
                        DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
                    }
                }
                else
                {
                    if (!_firstGhostTriggered && HealthBelowPct(70))
                    {
                        _firstGhostTriggered = true;
                        DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
                    }
                    else if (!_secondGhostTriggered && HealthBelowPct(35))
                    {
                        _secondGhostTriggered = true;
                        DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_VEIL_OF_SHADOW:
                            DoCast(SPELL_VEIL_OF_SHADOW);
                            events.ScheduleEvent(EVENT_VEIL_OF_SHADOW, urand(10000, 20000));
                            break;
                        case EVENT_CURSED_VEIL:
                            DoCast(me, SPELL_CURSED_VEIL);
                            events.ScheduleEvent(EVENT_CURSED_VEIL, urand(20000, 40000));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                bool _firstGhostTriggered;
                bool _secondGhostTriggered;
                bool _thirdGhostTriggered;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_baron_silverlaineAI(creature);
        }
};

class npc_odo_the_blindwatcher : public CreatureScript
{
    public:
        npc_odo_the_blindwatcher() : CreatureScript("npc_odo_the_blindwatcher") { }

        struct npc_odo_the_blindwatcherAI : public ScriptedAI
        {
            npc_odo_the_blindwatcherAI(Creature* creature) : ScriptedAI(creature) {}

            void IsSummonedBy(Unit* /*summoner*/)
            {
                if (Creature* baron = ObjectAccessor::GetCreature(*me, me->GetInstanceScript()->GetData64(DATA_BARON_SILVERLAINE)))
                    baron->AI()->JustSummoned(me);
            }

            void Reset()
            {
                _events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                _events.ScheduleEvent(EVENT_HOWLING_RAGE, 5000);
                _events.ScheduleEvent(EVENT_BLINDING_SHADOWS, urand(5000, 15000));
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
                        case EVENT_HOWLING_RAGE:
                            DoCast(me, SPELL_HOWLING_RAGE, true);
                            _events.ScheduleEvent(EVENT_HOWLING_RAGE, urand(5000, 19000));
                            break;
                        case EVENT_BLINDING_SHADOWS:
                            DoCastVictim(SPELL_BLINDING_SHADOWS);
                            _events.ScheduleEvent(EVENT_BLINDING_SHADOWS, 10000);
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_odo_the_blindwatcherAI (creature);
        }
};

class npc_razorclaw_the_butcher : public CreatureScript
{
    public:
        npc_razorclaw_the_butcher() : CreatureScript("npc_razorclaw_the_butcher") { }

        struct npc_razorclaw_the_butcherAI : public ScriptedAI
        {
            npc_razorclaw_the_butcherAI(Creature* creature) : ScriptedAI(creature) {}

            void IsSummonedBy(Unit* /*summoner*/)
            {
                if (Creature* baron = ObjectAccessor::GetCreature(*me, me->GetInstanceScript()->GetData64(DATA_BARON_SILVERLAINE)))
                    baron->AI()->JustSummoned(me);
            }

            void Reset()
            {
                _events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_BUTCHER_DRAIN, true);
                _events.ScheduleEvent(EVENT_SPECTRAL_RUSH, 5000);
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
                        case EVENT_SPECTRAL_RUSH:
                        {
                            std::list<Player*> players;
                            Trinity::AnyPlayerInObjectRangeCheck checker(me, 45.00f);
                            Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
                            me->VisitNearbyWorldObject(45.00f, searcher);
                            players.sort(Trinity::ObjectDistanceOrderPred(me, false));

                            if (!players.empty())
                                DoCast(players.front(), SPELL_SPECTRAL_RUSH, true);
                            _events.ScheduleEvent(EVENT_SPECTRAL_RUSH, urand(10000, 15000));
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_razorclaw_the_butcherAI (creature);
        }
};

class npc_rethilgore : public CreatureScript
{
    public:
        npc_rethilgore() : CreatureScript("npc_rethilgore") { }

        struct npc_rethilgoreAI : public ScriptedAI
        {
            npc_rethilgoreAI(Creature* creature) : ScriptedAI(creature) {}

            void IsSummonedBy(Unit* /*summoner*/)
            {
                if (Creature* baron = ObjectAccessor::GetCreature(*me, me->GetInstanceScript()->GetData64(DATA_BARON_SILVERLAINE)))
                    baron->AI()->JustSummoned(me);
            }

            void Reset()
            {
                _events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                _events.ScheduleEvent(EVENT_SOUL_DRAIN, 5000);
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
                        case EVENT_SOUL_DRAIN:
                        {
                            DoCastRandom(SPELL_SOUL_DRAIN, 40.00f, false);
                            _events.ScheduleEvent(EVENT_SOUL_DRAIN, 6000);
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_rethilgoreAI (creature);
        }
};

class npc_wolf_master_nandos : public CreatureScript
{
    public:
        npc_wolf_master_nandos() : CreatureScript("npc_wolf_master_nandos") { }

        struct npc_wolf_master_nandosAI : public ScriptedAI
        {
            npc_wolf_master_nandosAI(Creature* creature) : ScriptedAI(creature) {}

            void IsSummonedBy(Unit* /*summoner*/)
            {
                if (Creature* baron = ObjectAccessor::GetCreature(*me, me->GetInstanceScript()->GetData64(DATA_BARON_SILVERLAINE)))
                    baron->AI()->JustSummoned(me);
            }

            void Reset()
            {
                _events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                for (uint8 i = 0; i < 3; i++)
                    DoCast(me, SPELL_SUMMON_LUPINE_SPIRIT, true);

                _events.ScheduleEvent(EVENT_CLAW, 5000);
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
                        case EVENT_CLAW:
                        {
                            DoCastVictim(SPELL_CLAW, true);
                            _events.ScheduleEvent(EVENT_CLAW, urand(5000, 10000));
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wolf_master_nandosAI (creature);
        }
};

class spell_summon_worgen_spirits : public SpellScriptLoader
{
    public:
        spell_summon_worgen_spirits() : SpellScriptLoader("spell_summon_worgen_spirits") { }

        class spell_summon_worgen_spirits_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_summon_worgen_spirits_SpellScript);

            void AfterCast(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->CastSpell(GetCaster(), RAND(SPELL_SUMMON_SPIRIT_OF_ODO_MISSILE, SPELL_SUMMON_NANDO_MISSILE, SPELL_SUMMON_BUTCHER_MISSILE, SPELL_SUMMON_RETHILGORE_MISSILE), true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_summon_worgen_spirits_SpellScript::AfterCast, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_summon_worgen_spirits_SpellScript();
        }
};

void AddSC_boss_baron_silverlaine()
{
    new boss_baron_silverlaine();
    new npc_odo_the_blindwatcher();
    new npc_razorclaw_the_butcher();
    new npc_rethilgore();
    new npc_wolf_master_nandos();
    new spell_summon_worgen_spirits();

}