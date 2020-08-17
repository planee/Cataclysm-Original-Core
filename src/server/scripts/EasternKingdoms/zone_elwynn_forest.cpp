/*
 * Copyright (C) 2011-2012 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2012 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"

enum Northshire
{
    NPC_BLACKROCK_BATTLE_WORG = 49871,
    NPC_STORMWIND_INFANTRY    = 49869,
    NPC_BROTHER_PAXTON        = 951,
    NPC_INJURED_SOLDIER_DUMMY = 50378,

    SPELL_SPYING              = 92857,
    SPELL_SNEAKING            = 93046,
    SPELL_SPYGLASS            = 80676,

    SPELL_RENEW               = 93094,
    SPELL_PRAYER_OF_HEALING   = 93091,
    SPELL_FORTITUDE           = 13864,
    SPELL_PENANCE             = 47750,
    SPELL_FLASH_HEAL          = 17843,
    SPELL_RENEWEDLIFE         = 93097,

    ACTION_HEAL               = 1,
    EVENT_HEALED_1            = 1,
    EVENT_HEALED_2            = 2
};

class npc_stormwind_infantry : public CreatureScript
{
public:
    npc_stormwind_infantry() : CreatureScript("npc_stormwind_infantry") { }

    struct npc_stormwind_infantryAI : public ScriptedAI
    {
        npc_stormwind_infantryAI(Creature* creature) : ScriptedAI(creature)
        {
            me->HandleEmoteCommand(EMOTE_STATE_READY1H);
        }

        EventMap events;

        void EnterCombat(Unit * who)
        {
            me->AddUnitState(UNIT_STATE_ROOT);
            me->HandleEmoteCommand(EMOTE_STATE_READY1H);
        }

        void Reset()
        {
            me->HandleEmoteCommand(EMOTE_STATE_READY1H);
			DoMeleeAttackIfReady();
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (attacker->GetEntry() == NPC_BLACKROCK_BATTLE_WORG && damage >= me->GetHealth())
                me->SetHealth(me->GetMaxHealth());
        }

        void UpdateAI(const uint32 diff)
        {
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_stormwind_infantryAI (creature);
    }
};

/*######
## npc_blackrock_battle_worg
######*/

class npc_blackrock_battle_worg : public CreatureScript
{
public:
    npc_blackrock_battle_worg() : CreatureScript("npc_blackrock_battle_worg") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blackrock_battle_worgAI (creature);
    }

    struct npc_blackrock_battle_worgAI : public ScriptedAI
    {
        npc_blackrock_battle_worgAI(Creature* creature) : ScriptedAI(creature) { }

        void DamageTaken(Unit* who, uint32& damage)
        {
            if (who->GetEntry() == NPC_STORMWIND_INFANTRY && damage >= me->GetHealth())
                me->SetHealth(me->GetMaxHealth());

            if (who->GetTypeId() == TYPEID_PLAYER || who->isPet())
            {
                if (Creature* guard = me->FindNearestCreature(NPC_STORMWIND_INFANTRY, 6.0f, true))
                {
                    guard->getThreatManager().resetAllAggro();
                    guard->CombatStop(true);
                }

                me->LowerPlayerDamageReq(me->GetMaxHealth());
                me->getThreatManager().resetAllAggro();
                me->GetMotionMaster()->MoveChase(who);
                me->AI()->AttackStart(who);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
            {
                if (Creature* guard = me->FindNearestCreature(NPC_STORMWIND_INFANTRY, 6.0f, true))
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->AI()->AttackStart(guard);
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

///npc_hogger by Artamedes [6/2/18]
//Positions, not checked with sniffs! guessed with .gps
static Position generalSpawnPosition        = { -10129.539063f, 657.866882f, 35.970474f, 2.182297f };
static Position sorcererSpawnPosition       = { -10133.953125f, 655.467163f, 35.970428f, 2.015881f };
static Position marcusSpawnPosition         = { -10125.661133f, 661.571228f, 35.925201f, 2.496457f };

static Position ragamuffinSpawnPosition_1   = { -10130.856445f, 652.986450f, 35.966274f, 1.893434f }; //Raga should spawwn here, go behind general, then go back here and despawn
static Position ragamuffinSpawnPosition_2   = { -10123.195313f, 661.771423f, 35.884071f, 3.020481f };

static Position ragamuffinRunPosition_1     = { -10133.123047f, 658.377258f, 35.970608f, 1.830695f };
static Position ragamuffinRunPosition_2     = { -10129.954102f, 661.642578f, 35.967899f, 2.956864f };

static Position hoggerEatingPosition        = { -10145.360352f, 667.377380f, 37.532585f, 2.819594f };
static Position hoggerEndingPosition        = { -10137.541016f, 669.465881f, 35.913044f, 5.383512f };

static Position generalPosition_1           = { -10133.885742f, 663.495117f, 35.968323f, 2.151601f };
static Position generalPosition_2           = { -10135.874023f, 667.815247f, 35.914482f, 2.251507f };

static Position marcusWalkPosition_1        = { -10129.338867f, 665.421570f, 35.840855f, 2.913436f };
static Position sorcererWalkPosition_1      = { -10136.454102f, 659.487549f, 35.972000f, 2.061279f };

static Position ratSpawnPosition            = { -10126.856445f, 658.583862f, 35.969078f, 2.998422f };

enum eHoggerEvents : uint32
{
    EVENT_EATING = 1,
    EVENT_RECHECK_EATING,
    EVENT_SUMMON_MINIONS,
    EVENT_UPSET_STOMACH,
    EVENT_VICIOUS_SLICE,
    EVENT_DO_SUMMONS,
};

enum eHoggerTalks : uint32
{
    TALK_AGGRO = 0,             //Grrrr... fresh meat!
    TALK_HELP_HOGGER,           //Yipe! Help Hogger
    TALK_HURT_HOGGER,           //Hogger is eating! Stop him!
    TALK_NO_HURT_HOGGER,        //No hurt Hogger!
    TALK_HOGGER_NOOOO,          //Nooooo...
    TALK_HOGGER_STUNNED,        //Hogger is stunned!
    TALK_HOGGER_GRRR,           //Grrr...

    TALK_HOLD_YOUR_BLADE = 0,   //Hold your blade, adventurer!
    TALK_GENERAL_MARCUS_2,      //This beast leads the Riverpaw gnoll gang and may be the key to ending gnoll aggression in Elwynn.
    TALK_GENERAL_MARCUS_3,      //We're taking him into custody in the name of King Varian Wrynn.
    TALK_GENERAL_MARCUS_4,      //Take us to the Stockades, Andromath.
};

enum eHoggerSpells : uint32
{
    SPELL_EATING = 87351,
    SPELL_SUMMON_MINIONS = 87366,
    SPELL_UPSET_STOMACH = 87352,
    SPELL_VICIOUS_SLICE = 87337,

    SPELL_TELEPORT_AWAY = 64446,
};

enum eHoggerNpcIds : uint32
{
    NPC_HOGGER = 448,
    NPC_MINION_OF_HOGGER = 46932,
    NPC_MAGINOR_DUMAS = 46940,
    NPC_HIGH_SORCERER_ANDROMATH = 46941,
    NPC_GENERAL_MARCUS_JONATHAN = 46942,
    NPC_RAGAMUFFIN = 46943,
    NPC_STORMWIND_RAT = 49540,
};

enum eHoggerActions : uint32 //Called to differentAIs for dialogue
{
    ACTION_START_INTRO = 1, //Called to general marcus jonathan.
    ACTION_RAGAMUFFIN_1 = 2, //Called to first ragamuffin
    ACTION_RAGAMUFFIN_2 = 3, //Called to second ragamuffin
    ACTION_SORC_PREPARE_TELE = 4, //Called to sorcer after dialogue done.
};

///448
class npc_hogger : CreatureScript
{
public:
    npc_hogger() : CreatureScript("npc_hogger") { }

    enum ePoints : uint32
    {
        POINT_EAT = 1,
        POINT_FINISH = 2,
    };

    struct npc_hoggerAI : public ScriptedAI
    {
        npc_hoggerAI(Creature* creature) : ScriptedAI(creature), _preventReset(false) { }

        void Reset() override
        {
            if (_preventReset)
                return;

            _nonCombatEvents.Reset();
            _events.Reset();
            _ending = false;
            _eating = false;

            me->SetVisible(true);
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            me->GetMotionMaster()->MoveRandom(10.0f);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            me->GetMotionMaster()->Clear();
            Talk(TALK_AGGRO);
        }

        void JustDied(Unit* /*killer*/) override
        {
            _preventReset = false;
        }

        void DoAction(const int32 action) override
        {
            if (action == TALK_HOGGER_NOOOO)
                Talk(TALK_HOGGER_NOOOO);

            if (action == TALK_HOGGER_GRRR)
                Talk(TALK_HOGGER_GRRR);
        }

        void DamageTaken(Unit* attacker, uint32 &damage) override
        {
            if (me->HasAura(SPELL_EATING) && me->HealthBelowPctDamaged(21, damage))
            {
                me->RemoveAura(SPELL_EATING);
                DoCast(SPELL_UPSET_STOMACH);
                Talk(TALK_HOGGER_STUNNED);
            }

            if (!_eating && me->HealthBelowPctDamaged(51, damage))
            {
                _eating = true;
                _events.ScheduleEvent(EVENT_SUMMON_MINIONS, 250);
                _events.ScheduleEvent(EVENT_EATING, 500);
            }

            if (damage >= me->GetHealth())
            {
                damage = 0;

                ///<Artamedes: Don't allow attack to non attackable and immune to player control creatures/
                if (Unit* victim = attacker->getVictim())
                    if(victim == me)
                    {
                        attacker->AttackStop();
                        attacker->CastStop();
                    }

                if (!_ending)
                {
                    _events.Reset();

                    _ending = true;
                    _preventReset = true;
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAllAuras();
                    me->AttackStop();
                    Talk(TALK_NO_HURT_HOGGER);
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(POINT_FINISH, hoggerEndingPosition);
                }
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE)
            {
                if (id == POINT_EAT)
                {
                    Talk(TALK_HURT_HOGGER);
                    DoCast(SPELL_EATING);
                    scheduler.Schedule(Milliseconds(500), [this](TaskContext context)
                    {
                        if (!me->HasAura(SPELL_EATING))
                        {
                            me->SetReactState(REACT_AGGRESSIVE);
                        }
                        else
                            context.Repeat(Milliseconds(500));
                    });
                }
                if (id == POINT_FINISH)
                {
                    _nonCombatEvents.ScheduleEvent(EVENT_DO_SUMMONS, 1000);

                    //Give Credit
                    std::list<Player*> players;

                    Trinity::AnyPlayerInObjectRangeCheck checker(me, 35.0f);
                    Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
                    me->VisitNearbyWorldObject(35.0f, searcher);

                    for (Player* player : players)
                        player->KilledMonsterCredit(NPC_HOGGER);
                }
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            _nonCombatEvents.Update(diff);

            scheduler.Update(diff);

            if(uint32 eventId = _nonCombatEvents.ExecuteEvent())
                switch (eventId)
                {
                    case EVENT_DO_SUMMONS:
                        DoDialogueSummons();
                        break;
                }

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            _events.Update(diff);

            if(uint32 eventId = _events.ExecuteEvent())
                switch (eventId)
                {
                    case EVENT_EATING:
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        me->GetMotionMaster()->MovePoint(POINT_EAT, hoggerEatingPosition);
                        break;
                    case EVENT_SUMMON_MINIONS:
                        Talk(TALK_HELP_HOGGER);
                        DoCast(SPELL_SUMMON_MINIONS);
                        break;
                    case EVENT_UPSET_STOMACH:
                        DoCast(SPELL_UPSET_STOMACH);
                        break;
                    case EVENT_VICIOUS_SLICE:
                        DoCast(SPELL_VICIOUS_SLICE);
                        _events.ScheduleEvent(EVENT_VICIOUS_SLICE, urand(10000, 20000));
                        break;
                }

            DoMeleeAttackIfReady();
        }

        void JustSummoned(Creature* creature) override
        {
            if (UpdateVictim())
                if (creature->GetEntry() == NPC_MINION_OF_HOGGER)
                    creature->SetInCombatWith(me->getVictim());
        }

    private:
        EventMap _events;
        EventMap _nonCombatEvents;
        TaskScheduler scheduler;
        bool _eating;
        bool _ending;
        bool _preventReset;

        void DoDialogueSummons()
        {
            me->DespawnCreaturesInArea(NPC_GENERAL_MARCUS_JONATHAN, 100.0f);
            me->DespawnCreaturesInArea(NPC_HIGH_SORCERER_ANDROMATH, 100.0f);
            me->DespawnCreaturesInArea(NPC_MAGINOR_DUMAS, 100.0f);
            me->DespawnCreaturesInArea(NPC_RAGAMUFFIN, 100.0f);
            me->DespawnCreaturesInArea(NPC_STORMWIND_RAT, 100.0f);

            if (Creature* creature = me->SummonCreature(NPC_GENERAL_MARCUS_JONATHAN, generalSpawnPosition))
                creature->GetAI()->DoAction(ACTION_START_INTRO);

            me->SummonCreature(NPC_HIGH_SORCERER_ANDROMATH, sorcererSpawnPosition);
            me->SummonCreature(NPC_MAGINOR_DUMAS, marcusSpawnPosition);

            if (Creature* creature = me->SummonCreature(NPC_RAGAMUFFIN, ragamuffinSpawnPosition_1))
                creature->GetAI()->DoAction(ACTION_RAGAMUFFIN_1);

            if (Creature* creature = me->SummonCreature(NPC_RAGAMUFFIN, ragamuffinSpawnPosition_2))
                creature->GetAI()->DoAction(ACTION_RAGAMUFFIN_2);

            if (Creature* creature = me->SummonCreature(NPC_STORMWIND_RAT, ratSpawnPosition))
                creature->GetMotionMaster()->MoveRandom(5.0f);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_hoggerAI(creature);
    }
};
///46942
class npc_general_marcus_jonathan : CreatureScript
{
public:
    npc_general_marcus_jonathan() : CreatureScript("npc_general_marcus_jonathan") { }

    enum eEvents : uint32
    {
        EVENT_START_INTRO = 1,
        EVENT_DISMOUNT_AND_MOVE = 2,
        EVENT_TALK_2 = 3,
        EVENT_TALK_3 = 4,
        EVENT_TALK_4 = 5,
        EVENT_HOGGER_5 = 6,
    };

    struct npc_general_marcus_jonathanAI : public ScriptedAI
    {
        npc_general_marcus_jonathanAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            me->SetWalk(true);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            _events.Reset();
        }

        void DoAction(const int32 action) override
        {
            if (action == ACTION_START_INTRO)
                _events.ScheduleEvent(EVENT_START_INTRO, 500);
        }

        void UpdateAI(const uint32 diff) override
        {
            _events.Update(diff);

            if (uint32 eventId = _events.ExecuteEvent())
                switch (eventId)
                {
                    case EVENT_START_INTRO:
                        Talk(TALK_HOLD_YOUR_BLADE);
                        _events.ScheduleEvent(EVENT_DISMOUNT_AND_MOVE, 5000);
                        _events.ScheduleEvent(EVENT_TALK_2, 10000);
                        break;
                    case EVENT_DISMOUNT_AND_MOVE:
                        if (Creature* hogger = me->FindNearestCreature(NPC_HOGGER, 50.0f))
                        {
                            hogger->SetFacingToObject(me);
                            hogger->GetAI()->DoAction(TALK_HOGGER_GRRR);
                        }
                        me->Dismount();
                        me->GetMotionMaster()->MovePoint(0, generalPosition_1);
                        break;
                    case EVENT_TALK_2:
                        Talk(TALK_GENERAL_MARCUS_2);
                        _events.ScheduleEvent(EVENT_TALK_3, 12000);
                        me->GetMotionMaster()->MovePoint(0, generalPosition_2);
                        break;
                    case EVENT_TALK_3:
                        Talk(TALK_GENERAL_MARCUS_3);
                        _events.ScheduleEvent(EVENT_TALK_4, 12000);
                        break;
                    case EVENT_TALK_4:
                        Talk(TALK_GENERAL_MARCUS_4);
                        if (Creature* sorc = me->FindNearestCreature(NPC_HIGH_SORCERER_ANDROMATH, 50.0f))
                        {
                            me->SetFacingToObject(sorc);
                            sorc->GetAI()->DoAction(ACTION_SORC_PREPARE_TELE);
                        }
                        _events.ScheduleEvent(EVENT_HOGGER_5, 4000);
                        break;
                    case EVENT_HOGGER_5:
                        if (Creature* hogger = me->FindNearestCreature(NPC_HOGGER, 50.0f))
                            hogger->GetAI()->DoAction(TALK_HOGGER_NOOOO);
                        break;
                }
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_general_marcus_jonathanAI(creature);
    }
};

///46941
class npc_high_sorcerer_andromath : CreatureScript
{
public:
    npc_high_sorcerer_andromath() : CreatureScript("npc_high_sorcerer_andromath") { }

    enum eEvents : uint32
    {
        EVENT_PREPARE_TELE_1 = 1,
        EVENT_PREPARE_TELE_2 = 2,
    };

    enum ePoints : uint32
    {
        POINT_FINISHED_WALK = 1,
    };

    struct npc_high_sorcerer_andromathAI : public ScriptedAI
    {
        npc_high_sorcerer_andromathAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetWalk(true);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            _events.Reset();

            me->GetMotionMaster()->MovePoint(POINT_FINISHED_WALK, sorcererWalkPosition_1);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE)
                if (id == POINT_FINISHED_WALK)
                    if (Creature* hogger = me->FindNearestCreature(NPC_HOGGER, 50.0f))
                        me->SetFacingToObject(hogger);
        }

        void DoAction(const int32 action) override
        {
            if (action == ACTION_SORC_PREPARE_TELE)
                _events.ScheduleEvent(EVENT_PREPARE_TELE_1, 5000);
        }

        void UpdateAI(const uint32 diff) override
        {
            _events.Update(diff);

            if (uint32 eventId = _events.ExecuteEvent())
                switch (eventId)
                {
                    case EVENT_PREPARE_TELE_1:
                        if (Creature* c = me->FindNearestCreature(eHoggerNpcIds::NPC_HOGGER, 50.0f))
                            c->CastSpell(c, SPELL_TELEPORT_AWAY);

                        if (Creature* c = me->FindNearestCreature(eHoggerNpcIds::NPC_GENERAL_MARCUS_JONATHAN, 50.0f))
                            c->CastSpell(c, SPELL_TELEPORT_AWAY);

                        if (Creature* c = me->FindNearestCreature(eHoggerNpcIds::NPC_MAGINOR_DUMAS, 50.0f))
                            c->CastSpell(c, SPELL_TELEPORT_AWAY);

                        DoCast(SPELL_TELEPORT_AWAY);
                        _events.ScheduleEvent(EVENT_PREPARE_TELE_2, 500);
                        break;

                    case EVENT_PREPARE_TELE_2:
                        if (Creature* c = me->FindNearestCreature(eHoggerNpcIds::NPC_HOGGER, 50.0f))
                        {
                            c->SetVisible(false);
                            me->Kill(c);
                        }

                        if (Creature* c = me->FindNearestCreature(eHoggerNpcIds::NPC_GENERAL_MARCUS_JONATHAN, 50.0f))
                            c->DespawnOrUnsummon();

                        if (Creature* c = me->FindNearestCreature(eHoggerNpcIds::NPC_MAGINOR_DUMAS, 50.0f))
                            c->DespawnOrUnsummon();
                        
                        me->DespawnOrUnsummon();

                        break;
                }
        }
        

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_high_sorcerer_andromathAI(creature);
    }
};
///46940
class npc_maginor_dumas : CreatureScript
{
public:
    npc_maginor_dumas() : CreatureScript("npc_maginor_dumas") { }

    struct npc_maginor_dumasAI : public ScriptedAI
    {
        npc_maginor_dumasAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetWalk(true);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->GetMotionMaster()->MovePoint(0, marcusWalkPosition_1);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_maginor_dumasAI(creature);
    }
};
///46943
class npc_hogger_ragamuffin : CreatureScript
{
public:
    npc_hogger_ragamuffin() : CreatureScript("npc_hogger_ragamuffin") { }

    enum eTalks : uint32
    {
        TALK_RAGAMUFFIN_1 = 0,
        TALK_RAGAMUFFIN_2,
    };

    enum ePoints : uint32
    {
        POINT_DESPAWN = 1,
    };

    enum eEvents : uint32
    {
        EVENT_RAGAMUFFIN_1 = 1,
        EVENT_RAGAMUFFIN_2 = 2,
        EVENT_RAGAMUFFIN_3 = 3,
        EVENT_RAGAMUFFIN_4 = 4,
    };

    struct npc_hogger_ragamuffinAI : public ScriptedAI
    {
        npc_hogger_ragamuffinAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetVisible(false);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            _events.Reset();
            _Action = 0;
        }

        void DoAction(const int32 action) override
        {
            switch (action) //compiler will optimize switch
            {
                case ACTION_RAGAMUFFIN_1:
                case ACTION_RAGAMUFFIN_2:
                    _events.ScheduleEvent(EVENT_RAGAMUFFIN_1, 6000);
                    break;
            }

            _Action = action;
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE)
                if (id == POINT_DESPAWN)
                    me->DespawnOrUnsummon();
        }

        void UpdateAI(const uint32 diff) override
        {
            _events.Update(diff);

            if (uint32 eventId = _events.ExecuteEvent())
                switch (eventId)
                {
                    case EVENT_RAGAMUFFIN_1:
                        me->SetVisible(true);
                        _events.ScheduleEvent(EVENT_RAGAMUFFIN_2, 1000);
                        break;

                    case EVENT_RAGAMUFFIN_2:
                        me->GetMotionMaster()->MovePoint(0, _Action == ACTION_RAGAMUFFIN_1 ? ragamuffinRunPosition_1 : ragamuffinRunPosition_2);
                        _events.ScheduleEvent(EVENT_RAGAMUFFIN_3, 1000);
                        break;

                    case EVENT_RAGAMUFFIN_3:
                        if (_Action == ACTION_RAGAMUFFIN_1)
                            Talk(TALK_RAGAMUFFIN_1);
                        else
                            Talk(TALK_RAGAMUFFIN_2);

                        _events.ScheduleEvent(EVENT_RAGAMUFFIN_4, 1500);
                        break;

                    case EVENT_RAGAMUFFIN_4:
                        me->GetMotionMaster()->MovePoint(POINT_DESPAWN, _Action == ACTION_RAGAMUFFIN_1 ? ragamuffinSpawnPosition_1 : ragamuffinSpawnPosition_2);
                        break;
                }
        }

    private:
        EventMap _events;
        uint32 _Action;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_hogger_ragamuffinAI(creature);
    }
};

void AddSC_elwynn_forest()
{
    new npc_blackrock_battle_worg();
    new npc_stormwind_infantry();
    //Hogger
	new npc_hogger();
    new npc_general_marcus_jonathan();
    new npc_high_sorcerer_andromath();
    new npc_maginor_dumas();
    new npc_hogger_ragamuffin();
}
