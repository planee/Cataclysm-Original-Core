/*
* Copyright (C) 2013-2014 OMFG.GG Network <http://www.omfg.gg>
* Copyright (C) 2012-2013 Reign of Darkness Network <http://reignofdarkness.net/>
*
* Este codigo es de uso exclusivo y privado para la red de gaming
* Reign of Darkness Network esta totalmente prohibida su distribucion y
* uso en proyectos ajenos, si has obtenido o encontrado este codigo
* publicamente envianos un email a administracion@reignofdarkness.net
* indicandonos como y donde has obtenido este codigo
*
* Recuerda, no distribuyas, no compartas y no subas este codigo a ningun
* lugar publico, usa siempre nuestros repositorios Git!
*/

#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "firelands.h"

/*
 * Rewritten by: Onkelz
 * Base from: Forgottenlands and Reign of Darkness Network
 * Script Status: 99%
 * Todo:
 * - Fix hover issues (after wipe the boss position is bugged)
 * - Fix spinners lift spiky movement
 */

enum Spells
{
    // Bethilac
    SPELL_MANA_BURN                               = 99191,
    SPELL_ZERO_MANA_REGEN                         = 96301,
    SPELL_EMBER_FLARE                             = 98934,
    SPELL_METEOR_BURN_AURA                        = 99071,
    SPELL_METEOR_BURN                             = 99073,
    SPELL_CONSUME                                 = 99304,
    SPELL_SMOLDERING_DEVASTATION                  = 99052,
    SPELL_VENOM_RAIN                              = 99333,
    SPELL_METEOR_BURN_VISUAL                      = 99071,
    SPELL_FRENZY                                  = 99497,
    SPELL_KISS                                    = 99476,
    SPELL_WEB_SILK                                = 100048,
    SPELL_THE_WIDOWS_KISS                         = 99506,
    SPELL_RIDE_VEHICLE                            = 99372,
    SPELL_VALID_HOST_START                        = 101039,
    SPELL_VALID_HOST_END                          = 101047,

    // Ciderweb Spinner
    SPELL_BURNING_ACID                            = 98471,
    SPELL_FIERY_WEB_SPIN                          = 97202,

    //Cinderweb Spiderling
    SPELL_SEEPING_VENOM                           = 97079,
    SPELL_EGG_SATCHEL                             = 97118,
    SPELL_CREEPER                                 = 98858,
    SPELL_INVISIBILITY                            = 98929,

    // cinderweb drone
    SPELL_BOILING_SPATTER                         = 99463,
    SPELL_FIXATE                                  = 99526, // heroic only
    SPELL_ENERGIZE                                = 99211,
    SPELL_POUNCE                                  = 99273, // for what is this?
    SPELL_LEECH_VENOM                             = 99411,

    SPELL_SPIDERWEB_FILAMENT_1                    = 98149, // Graphic of ground web signal
    SPELL_SPIDERWEB_FILAMENT_2                    = 98623, // Graphic filament (to any target)
    SPELL_SPIDERWEB_FILAMENT_3                    = 98153, // Graphic filamente (pet to summoner)
    SPELL_SPIDERWEB_FILAMENT_4                    = 97196, // stalker animation
    SPELL_CREATE_CHITINOUS_FRAGMENT               = 100520,

    // spiderling stalker
    SPELL_SUMMON_CINDERWEB_SPINNER                = 98872,
    SPELL_SUMMON_CINDERWEB_SPIDERLING             = 98895,
    SPELL_SUMMON_CINDERWEB_DRONE                  = 98878,
    SPELL_SUMMON_BANELING                         = 100053, // heroic only

    // broodling
    SPELL_VOLATILE_BURST                          = 99990,
    SPELL_VOLATILE_POISON                         = 99276,
    SPELL_BROODLING_CHANCRE_AURA                  = 99982,

    SPELL_XXX_FIXATE                              = 99999, // or maybe not but the ex aura make me thinking it has something todoo somewhere here
    // Meteor
    SPELL_METEOR_BLOCKER_AURA                     = 99146,
};

enum Events
{
    EVENT_SUMMON_CINDERWEB_SPINNER = 1,
    EVENT_VENOM_RAIN,
    EVENT_SMOLDERING_DEVASTATION,
    EVENT_METEOR_BURN,
    EVENT_SUMMON_SPIDERLING,
    EVENT_SUMMON_DRONE,
    EVENT_FRENZY,
    EVENT_KISS,
    EVENT_EMBER_FLARE,
    GO_HOME,
    EVENT_POINT_GROUND,
    EVENT_GO_DOWN,
    EVENT_CONSUME_BETHTILAC,
    EVENT_CHECK_UPPER,
    EVENT_PLAYER_ALIVE_CHECK,
    EVENT_START_PHASE_TWO_ATTACK,

    // spinner events
    EVENT_BURNING_ACID,
    EVENT_FIERY_WEB_SPIN,
    EVENT_IS_TAUNT,
    EVENT_SPINNER_MOVE_DOWN,

    // Spiderling events
    EVENT_GET_TARGET,
    EVENT_VENOM,
    EVENT_PLAYER_RANGE_CHECK,

    // Drone events
    EVENT_BOILING_SPATTER,
    EVENT_BURNIGN_ACID_DRONE,
    EVENT_CONSUME,
    EVENT_CHECK_DRONE_ENERGY,
    EVENT_FIXATE,
    EVENT_REMOVE_IMMUNITY,

    // Lift events
    EVENT_CHECK_MIDDLE_HOLE,

    // Webrip events
    EVENT_METEOR_DUMMY,
    EVENT_CLEAR_PLAYER_LIST,

    // Stalker
    EVENT_ENABLE_DRONE_SUMMON,

    // broodling
    EVENT_CHECK_PLAYERS_IN_RANGE
};

enum Points
{
    POINT_UP              = 1,
    POINT_DOWN            = 2,
    POINT_SPINNER_MEDIATE = 3,
    POINT_GROUND          = 4,
    POINT_FALL = 1005,
};

enum Emotes
{
    EMOTE_SPINNER         = 0,
    EMOTE_DEVASTATION     = 1
};


enum spellGroups
{
    GROUP_PHASE_ALL = 0,
    GROUP_PHASE_ONE = 1,
    GROUP_PHASE_TWO = 2
};

enum Phases
{
    PHASE_ALL       = 0,
    PHASE_ONE       = 1,
    PHASE_SWITCH    = 2,
    PHASE_TWO       = 3,
    PHASE_NONE      = 4,
};

enum eActions
{
    ACTION_SUMMON_SPIDERLING = 0,
    ACTION_SUMMON_DRONE      = 1,
    ACTION_RESET_EVENTS      = 2,
    ACTION_DEVASTATION       = 3,
    ACTION_SUMMON_SPINNERS   = 4,
    ACTION_ACHIEVMENT_FAIL   = 5
};

static const Position downPos          = { 62.877f, 387.539f, 74.244f, 3.727f };
static const Position centerPos        = { 60.318f, 385.556f, 74.041f, 3.682f };

class FieryWebSpinSelector : public std::unary_function<Unit*, bool>
{
public:
    FieryWebSpinSelector(Unit* creature) : _creature(creature) { }

    bool operator()(Unit* unit) const
    {
        if (unit->GetTypeId() != TYPEID_PLAYER || unit->HasAura(SPELL_FIERY_WEB_SPIN) || unit->HasAura(SPELL_WEB_SILK) || unit->GetExactDist2d(_creature) >= 150.0f)
            return false;
        return true;
    }
private:
    Unit* _creature;
};

class boss_bethtilac : public CreatureScript
{
    public:
        boss_bethtilac() : CreatureScript("boss_bethtilac") { }

        struct boss_bethtilacAI : public BossAI
        {
            boss_bethtilacAI(Creature* creature) : BossAI(creature, DATA_BETHTILAC) { }

            void MoveInLineOfSight(Unit* who)
            {
                if (!me->getVictim() && me->IsValidAttackTarget(who))
                {
                    if (me->IsWithinDistInMap(who, 30.f))
                    {
                        AttackStart(who);
                        me->SetInCombatWithZone();
                    }
                }
            }

            void startStalkerEvent(int32 action)
            {
                std::list<Creature*> stalkerList;
                std::list<Creature*> targetList;
                GetCreatureListWithEntryInGrid(stalkerList, me, NPC_SPIDERLING_STALKER, 200.0f);

                for (std::list<Creature*>::const_iterator itr = stalkerList.begin(); itr != stalkerList.end(); ++itr)
                {
                    if (action == ACTION_RESET_EVENTS
                        || (action == ACTION_SUMMON_SPINNERS && (*itr)->GetPositionZ() > 90.0f)
                        || (action == ACTION_SUMMON_SPIDERLING && (*itr)->GetPositionZ() < 90.0f && !((*itr)->GetPositionX() >= 10.00f && (*itr)->GetPositionX() <= 11.00f))
                        || (action == ACTION_SUMMON_DRONE && (*itr)->GetPositionX() >= 10.00f && (*itr)->GetPositionX() <= 11.00f))
                        targetList.push_back((*itr));
                }

                if (targetList.empty())
                    return;

                if (action == ACTION_SUMMON_SPIDERLING)
                    Trinity::Containers::RandomResizeList(targetList, Is25ManRaid() ? 3 : 1);

                if (action == ACTION_SUMMON_SPINNERS)
                    Trinity::Containers::RandomResizeList(targetList, RAID_MODE(2, 4, 2, 5));

                for (std::list<Creature*>::const_iterator itr = targetList.begin(); itr != targetList.end(); ++itr)
                    (*itr)->AI()->DoAction(action);
            }

            void applyPlayerVehicle(bool apply)
            {
                if (apply)
                {
                    std::list<Player*> TargetList;
                    Map::PlayerList const& Players = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
                        if (Player* player = itr->getSource())
                            me->AddAura(urand(SPELL_VALID_HOST_START, SPELL_VALID_HOST_END), player);
                }
                else
                    for (uint32 i = 101039; i < 101048; i++)
                        instance->DoRemoveAurasDueToSpellOnPlayers(i);
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);

                if (summon->GetEntry() == NPC_WEB_RIP)
                {
                    summon->AddAura(SPELL_METEOR_BURN_VISUAL, summon);
                    return;
                }

                DoZoneInCombat(summon);
            }

            void Reset() override
            {
                _Reset();
                if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
                {
                    uint32 preNerfHealth = RAID_MODE(20816178, 62549585, 32740009, 98422127);
                    me->SetMaxHealth(preNerfHealth);
                    me->SetHealth(preNerfHealth);
                }
                events.SetPhase(PHASE_NONE);
                startStalkerEvent(ACTION_RESET_EVENTS);
                me->ClearUnitState(UNIT_STATE_ROOT);
                me->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                me->setPowerType(POWER_MANA);
                me->SetMaxPower(POWER_MANA, 8200);
                me->SetPower(POWER_MANA, 8200);
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 50331648);
                me->AddUnitState(MOVEMENTFLAG_SWIMMING);
                me->SetHover(true);
                me->Relocate(me->GetHomePosition());
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                if (Creature* lift = me->FindNearestCreature(NPC_BETHTILAC_VEHICLE, 30.0f, true))
                    me->CastSpell(lift, SPELL_SPIDERWEB_FILAMENT_2, true);
                spinnerCount = 0;
                numDevastation = 0;
                DeathFromAbove = true;
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_POWER_BURN, true);
                me->SetReactState(REACT_AGGRESSIVE);
            }

            void EnterEvadeMode() override
            {
                me->ClearUnitState(UNIT_STATE_ROOT);
                events.SetPhase(PHASE_NONE);
                //me->Relocate(me->GetPositionX(), me->GetPositionY(), 75.0f, me->GetOrientation());
                applyPlayerVehicle(false);
                BossAI::EnterEvadeMode();
            }

            void JustReachedHome() override
            {
                me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 80.00f, me->GetOrientation());
                BossAI::JustReachedHome();
            }

            void EnterCombat(Unit* victim) override
            {
                _EnterCombat();
                applyPlayerVehicle(true);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                me->ClearUnitState(MOVEMENTFLAG_SWIMMING);
                me->setActive(true);
                me->SetHover(false);
                me->SetReactState(REACT_PASSIVE);
                DoCast(me, SPELL_MANA_BURN, true);
                DoCast(me, SPELL_ZERO_MANA_REGEN, true);
                if (Creature* vehicle = me->FindNearestCreature(NPC_BETHTILAC_VEHICLE, 150.0f))
                    DoCast(vehicle, SPELL_RIDE_VEHICLE, true);

                events.SetPhase(PHASE_ONE);
                events.ScheduleEvent(EVENT_CHECK_UPPER, 5000, GROUP_PHASE_ONE, PHASE_ONE);
                events.ScheduleEvent(EVENT_VENOM_RAIN, 2000, GROUP_PHASE_ONE, PHASE_ONE);
                events.ScheduleEvent(EVENT_SUMMON_CINDERWEB_SPINNER, 12000, GROUP_PHASE_ONE, PHASE_ONE);
                events.ScheduleEvent(EVENT_METEOR_BURN, 10000, GROUP_PHASE_ONE, PHASE_ONE);
                events.ScheduleEvent(EVENT_SUMMON_SPIDERLING, 12000, GROUP_PHASE_ONE, PHASE_ONE);
                events.ScheduleEvent(EVENT_SUMMON_DRONE, 43000, GROUP_PHASE_ONE, PHASE_ONE); // 31sec after spiderling
                events.ScheduleEvent(EVENT_EMBER_FLARE, 3000, GROUP_PHASE_TWO, PHASE_ONE);
                events.ScheduleEvent(EVENT_PLAYER_ALIVE_CHECK, 1000, GROUP_PHASE_ALL);
            }

            void JustDied(Unit* killer) override
            {
                _JustDied();
                applyPlayerVehicle(false);

                Position pos(*me);
                pos.m_positionZ = 75.0f;
                me->ExitVehicle(&pos);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->HasQuestForItem(ITEM_HEART_OF_FLAME))
                    {
                        DoCast(me, SPELL_SMOULDERING, true);
                        break;
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage) override
            {
                if (events.IsInPhase(PHASE_ONE) && HealthBelowPct(11))
                {
                    events.SetPhase(PHASE_SWITCH);
                    me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                    me->ClearUnitState(UNIT_STATE_ROOT);
                    events.CancelEventGroup(GROUP_PHASE_ONE);
                    events.ScheduleEvent(GO_HOME, 100, GROUP_PHASE_TWO);
                }
            }

            void MovementInform(uint32 type, uint32 id) override
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id == POINT_DOWN)
                    events.ScheduleEvent(EVENT_GO_DOWN, 0);
            }

            void DoAction(int32 const action) override
            {
                switch (action)
                {
                    case ACTION_DEVASTATION:
                        if (events.IsInPhase(PHASE_ONE))
                        {
                            if (canCastDevastation)
                            {
                                canCastDevastation = false;
                                events.ScheduleEvent(EVENT_SMOLDERING_DEVASTATION, 500);
                                if (++numDevastation >= 3 && events.IsInPhase(PHASE_ONE))
                                {
                                    me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                                    me->ClearUnitState(UNIT_STATE_ROOT);
                                    events.CancelEventGroup(GROUP_PHASE_ONE);
                                    events.ScheduleEvent(GO_HOME, 8000, GROUP_PHASE_TWO);
                                    events.SetPhase(PHASE_SWITCH);
                                }
                            }
                        }
                        break;
                    case ACTION_ACHIEVMENT_FAIL:
                        DeathFromAbove = false;
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type) const override
            {
                if (type == DATA_DEATH_FROM_ABOVE)
                    return DeathFromAbove;

                return 0;
            }

            void UpdateAI(const uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events.IsInPhase(PHASE_ONE) && me->GetPositionZ() <= 108.0f && me->isAlive())
                    me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 110.0f, me->GetOrientation());
                else if (events.IsInPhase(PHASE_TWO) && me->GetPositionZ() >= 100.0f && me->isAlive())
                    me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), 74.0f, me->GetOrientation());

                while (uint32 eventid = events.ExecuteEvent())
                {
                    switch (eventid)
                    {
                        case EVENT_PLAYER_ALIVE_CHECK:
                        {
                            bool reset = true;
                            Map::PlayerList const& Players = me->GetMap()->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
                                if (Player* player = itr->getSource())
                                    if (me->GetExactDist2d(player) <= 300.0f && player->isAlive() && !player->isGameMaster())
                                        reset = false;

                            if (reset)
                                EnterEvadeMode();
                            else
                                events.ScheduleEvent(EVENT_PLAYER_ALIVE_CHECK, 1000, GROUP_PHASE_ALL);
                            break;
                        }
                        case EVENT_VENOM_RAIN:
                        {
                            if (!SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, SPELL_WEB_SILK))
                            {
                                DoCast(me, SPELL_VENOM_RAIN);
                                me->SetReactState(REACT_PASSIVE);
                                me->AddUnitState(UNIT_STATE_ROOT);
                            }
                            else
                                me->ClearUnitState(UNIT_STATE_ROOT);

                            me->RemoveAurasDueToSpell(SPELL_SPIDERWEB_FILAMENT_2);
                            events.ScheduleEvent(EVENT_VENOM_RAIN, 3000, GROUP_PHASE_ONE);
                            break;
                        }
                        case EVENT_SUMMON_CINDERWEB_SPINNER:
                        {
                            Talk(EMOTE_SPINNER);
                            canCastDevastation = true;
                            startStalkerEvent(ACTION_SUMMON_SPINNERS);
                            break;
                        }
                        case GO_HOME:
                            if (Creature* lift = Creature::GetCreature(*me, instance->GetData64(NPC_BETHTILAC_VEHICLE)))
                                lift->CastSpell(me, SPELL_SPIDERWEB_FILAMENT_2, true);
                            me->GetMotionMaster()->MovePoint(POINT_DOWN, 55.738f, 384.017f, 110.80f, false);
                            break;
                        case EVENT_SMOLDERING_DEVASTATION:
                            Talk(EMOTE_DEVASTATION);
                            DoCastAOE(SPELL_SMOLDERING_DEVASTATION);
                            if (events.IsInPhase(PHASE_ONE))
                                events.ScheduleEvent(EVENT_SUMMON_CINDERWEB_SPINNER, 15000, GROUP_PHASE_ONE);
                            break;
                        case EVENT_METEOR_BURN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, SPELL_WEB_SILK))
                                DoCast(target, SPELL_METEOR_BURN, true);
                            events.ScheduleEvent(EVENT_METEOR_BURN, urand(15000, 18000), GROUP_PHASE_ONE);
                            break;
                        case EVENT_SUMMON_SPIDERLING:
                            startStalkerEvent(ACTION_SUMMON_SPIDERLING);
                            events.ScheduleEvent(EVENT_SUMMON_SPIDERLING, 30500, GROUP_PHASE_ONE); // 30, 60, 91, 121, 152
                            break;
                        case EVENT_SUMMON_DRONE:
                            startStalkerEvent(ACTION_SUMMON_DRONE);
                            events.ScheduleEvent(EVENT_SUMMON_DRONE, urand(50000, 60000), GROUP_PHASE_ONE); // 50-60 (random) sec after the first summon (taken from sniffs)
                            break;
                        case EVENT_EMBER_FLARE:
                            DoCastAOE(SPELL_EMBER_FLARE, true);
                            events.ScheduleEvent(EVENT_EMBER_FLARE, 6000, GROUP_PHASE_TWO);
                            break;
                        case EVENT_FRENZY:
                            if (me->CanFly())
                            {
                                me->RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                                me->SetCanFly(false);
                            }
                            me->RemoveAurasDueToSpell(SPELL_SPIDERWEB_FILAMENT_2);
                            me->AddAura(SPELL_FRENZY, me);
                            events.ScheduleEvent(EVENT_FRENZY, 5000, GROUP_PHASE_TWO);
                            break;
                        case EVENT_KISS:
                            DoCastVictim(SPELL_KISS, true);
                            events.ScheduleEvent(EVENT_KISS, 32000, GROUP_PHASE_TWO);
                            break;
                        case EVENT_CONSUME_BETHTILAC:
                            if (Creature* spiderling = me->FindNearestCreature(RAID_MODE(52447, 53579, 53580, 53581), 5.0f, true))
                                DoCast(spiderling, SPELL_CONSUME);
                            events.ScheduleEvent(EVENT_CONSUME_BETHTILAC, 1000);
                            break;
                        case EVENT_GO_DOWN:
                            me->SetReactState(REACT_PASSIVE);
                            if (Creature* vehicle = me->FindNearestCreature(NPC_BETHTILAC_VEHICLE, 150.0f))
                                DoCast(vehicle, SPELL_RIDE_VEHICLE, true);
                            events.ScheduleEvent(EVENT_POINT_GROUND, 500);
                            break;
                        case EVENT_POINT_GROUND:
                        {
                            me->ExitVehicle(&downPos);
                            events.SetPhase(PHASE_TWO);
                            events.ScheduleEvent(EVENT_FRENZY, 8000, GROUP_PHASE_TWO, PHASE_TWO);
                            events.ScheduleEvent(EVENT_KISS, 16000, GROUP_PHASE_TWO, PHASE_TWO);
                            events.ScheduleEvent(EVENT_CONSUME_BETHTILAC, 9000, GROUP_PHASE_TWO, PHASE_TWO);
                            events.ScheduleEvent(EVENT_START_PHASE_TWO_ATTACK, 2000, GROUP_PHASE_TWO, PHASE_TWO);
                            break;
                        }
                        case EVENT_START_PHASE_TWO_ATTACK:
                            me->SetReactState(REACT_AGGRESSIVE);
                            if (me->getVictim())
                            {
                                me->GetMotionMaster()->MoveChase(me->getVictim());
                                AttackStart(me->getVictim());
                            }
                            break;
                        case EVENT_CHECK_UPPER:
                            if (numDevastation != 3)
                            {
                                DoCast(me, SPELL_WEB_SILK, true);

                                //Players part
                                std::list<Player*> TargetList = me->GetPlayersInRange(500.00f, true);
                                TargetList.remove_if(Trinity::UnitAuraCheck(false, SPELL_WEB_SILK, me->GetGUID()));

                                if (TargetList.empty())
                                {
                                    me->SetReactState(REACT_PASSIVE);
                                    me->AttackStop();
                                    if (me->GetExactDist2d(55.738f, 384.017f) >= 5.0f)
                                        me->GetMotionMaster()->MovePoint(0, 55.738f, 384.017f, 110.80f, false);
                                }
                                else
                                {
                                    me->SetReactState(REACT_AGGRESSIVE);
                                    if (Unit* target = me->getVictim())
                                    {
                                        if (target->GetPositionZ() < 100.0f && events.IsInPhase(PHASE_ONE))
                                        {
                                            me->getThreatManager().addThreat(target, -90000000);
                                            if (!TargetList.empty())
                                            {
                                                if (Player* player = Trinity::Containers::SelectRandomContainerElement(TargetList))
                                                {
                                                    me->Attack(player, true);
                                                    me->GetMotionMaster()->MoveChase(player);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_UPPER, 500, GROUP_PHASE_ONE);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                uint8 numDevastation;
                uint8 spinnerCount;
                Unit* liftoff;
                bool canCastDevastation, DeathFromAbove;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_bethtilacAI(creature);
        }
};

class npc_cinderweb_spinner : public CreatureScript
{
    public:
        npc_cinderweb_spinner() : CreatureScript("npc_cinderweb_spinner") { }

        struct npc_cinderweb_spinnerAI : public ScriptedAI
        {
            npc_cinderweb_spinnerAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset() override
            {
                events.Reset();
                Downed = false;
                me->SetReactState(REACT_PASSIVE);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->AddUnitState(MOVEMENTFLAG_SWIMMING);
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 50331648);
                _filamentGUID = 0;
                //if (Unit* target = me->SelectNearestPlayer(200.0f))
                //    me->Attack(target, false);
            }

            void AttackStart(Unit* who) override
            {
                if (!who)
                    return;

                if (who->GetPositionZ() > 100.0f)
                {
                    me->getThreatManager().addThreat(who, -90000000);
                    AttackStartNoMove(who);
                }
                else
                    ScriptedAI::AttackStart(who);
            }

            void JustDied(Unit* killer) override
            {
                SummonFilament();

                me->SetVisible(false);
            }

            void SpellHit(Unit* attacker, SpellInfo const* spellInfo) override
            {
                wasTaunt = false;
                for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    if (spellInfo->Effects[i].Effect == SPELL_EFFECT_ATTACK_ME || spellInfo->Effects[i].Effect == SPELL_EFFECT_THREAT || spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_MOD_TAUNT)
                        wasTaunt = true;

                if (wasTaunt && !Downed)
                {
                    Downed = true;
                    me->GetMotionMaster()->MoveFall(POINT_FALL);
                    _attackerGUID = attacker->GetGUID();
                    events.CancelEvent(EVENT_FIERY_WEB_SPIN);
                    me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                    SummonFilament();
                }
            }

            void SummonFilament()
            {
                if (!_filamentGUID)
                {
                    Position pos;
                    me->GetPosition(&pos);
                    pos.m_positionZ = 105.0f;
                    if (Creature* filament = me->SummonCreature(NPC_SPIDERWEB_FILAMENT, pos, TEMPSUMMON_TIMED_DESPAWN, 22000, 0))
                    {
                        filament->SetReactState(REACT_PASSIVE);
                        filament->AddAura(SPELL_SPIDERWEB_FILAMENT_1, filament);
                        filament->GetMotionMaster()->MoveFall();
                        pos.m_positionZ += 6.0f;
                        if (Creature* dest = filament->SummonCreature(53237, pos, TEMPSUMMON_TIMED_DESPAWN, 22000, 0))
                        {
                            dest->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE);
                            dest->CastSpell(filament, SPELL_SPIDERWEB_FILAMENT_2, true);
                        }
                        _filamentGUID = filament->GetGUID();
                    }
                }
            }

            void MovementInform(uint32 type, uint32 id) override
            {
                if (type != EFFECT_MOTION_TYPE)
                    return;

                if (id == POINT_FALL)
                    events.ScheduleEvent(EVENT_IS_TAUNT, 0);
            }


            void IsSummonedBy(Unit* summoner) override
            {
                DoCast(me, SPELL_EGG_SATCHEL, true);
                if (Creature* eggSatchel = me->FindNearestCreature(52528, 5.0f))
                    eggSatchel->CastSpell(me, SPELL_SPIDERWEB_FILAMENT_2, true);
                me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() - 5.0f, me->GetOrientation(), false);
                me->GetMotionMaster()->MovePoint(POINT_SPINNER_MEDIATE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() - 15.0f);
            }

            void EnterCombat(Unit* who) override
            {
                events.ScheduleEvent(EVENT_BURNING_ACID, 2000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_FIERY_WEB_SPIN, urand(5000, 10000));
            }

            void UpdateAI(const uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventid = events.ExecuteEvent())
                {
                    switch (eventid)
                    {
                        case EVENT_BURNING_ACID:
                            if(!Downed)
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true, -SPELL_WEB_SILK))
                                    DoCast(target, SPELL_BURNING_ACID);
                                events.ScheduleEvent(EVENT_BURNING_ACID, 2000, true);
                            }
                            break;
                        case EVENT_FIERY_WEB_SPIN:
                            if (!Downed)
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, FieryWebSpinSelector(me)))
                                    DoCast(target, SPELL_FIERY_WEB_SPIN);
                                events.ScheduleEvent(EVENT_FIERY_WEB_SPIN, 25000);
                            }
                            break;
                        case EVENT_IS_TAUNT:
                        {
                            me->RemoveAurasDueToSpell(SPELL_SPIDERWEB_FILAMENT_2);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->SetCanFly(false);
                            me->SetDisableGravity(false);
                            me->RemoveUnitMovementFlag(MOVEMENTFLAG_SWIMMING);
                            me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                            if (Unit *attacker = Unit::GetPlayer(*me, _attackerGUID))
                                me->Attack(attacker, true);
                            else if (Unit *attacker = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                me->Attack(attacker, true);
                            if (me->getVictim())
                                me->GetMotionMaster()->MoveChase(me->getVictim());
                            break;
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }
            private:
                InstanceScript* instance;
                EventMap events;
                bool Downed, wasTaunt;
                uint64 _attackerGUID, _filamentGUID;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_cinderweb_spinnerAI(creature);
        }
};

class LiftMovementEvent : public BasicEvent
{
public:
    LiftMovementEvent(Creature* owner) : _owner(owner) { }

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        Position pos;
        _owner->GetPosition(&pos);
        pos.m_positionZ = 107.443f;
        _owner->GetMotionMaster()->MovePoint(POINT_UP, pos, false, 15.0f);
        return true;
    }

private:
    Creature* _owner;
};

class npc_spirderweb_filament : public CreatureScript
{
public:
    npc_spirderweb_filament() : CreatureScript("npc_spirderweb_filament") { }

    struct npc_spirderweb_filamentAI : public ScriptedAI
    {
        npc_spirderweb_filamentAI(Creature* creature) : ScriptedAI(creature) {}

        void IsSummonedBy(Unit* summoner) override
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == POINT_UP)
            {
                if (Vehicle* vehicle = me->GetVehicleKit())
                {
                    if (Unit* target = vehicle->GetPassenger(0))
                    {
                        Position pos(*me);
                        pos.m_positionZ = 115.0f;
                        target->ExitVehicle(&pos);
                    }
                    me->DespawnOrUnsummon(500);
                }
            }
        }

        void OnSpellClick(Unit* clicker, bool& result) override
        {
            if (!result)
                return;

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->RemoveAura(SPELL_SPIDERWEB_FILAMENT_1);
            if (Vehicle* vehicle = clicker->GetVehicleKit())
                vehicle->RemoveAllPassengers();
            me->m_Events.AddEvent(new LiftMovementEvent(me), me->m_Events.CalculateTime(1000));
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_spirderweb_filamentAI(creature);
    }
};

class npc_cinderweb_spiderling : public CreatureScript
{
public:
    npc_cinderweb_spiderling() : CreatureScript("npc_cinderweb_spiderling") { }

    struct npc_cinderweb_spiderlingAI : public ScriptedAI
    {
        npc_cinderweb_spiderlingAI(Creature * creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_GET_TARGET, 100);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            me->SetHomePosition(centerPos);
            events.ScheduleEvent(EVENT_GET_TARGET, 100);
            me->SetWalk(true);
        }

        bool CanAIAttack(Unit const* target) const override
        {
            if (target->GetPositionZ() >= 90.0f || target->HasAura(SPELL_WEB_SILK))
                return false;
            return true;
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->DespawnOrUnsummon(1000);
            events.Reset();
        }

        void UpdateAI(uint32 const diff) override
        {
            if (Unit* victim = me->SelectVictim())
                AttackStart(victim);

            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_GET_TARGET:
                        if (!me->GetVehicle())
                        {
                            std::list<Creature*> drones;
                            me->GetCreatureListWithEntryInGrid(drones, NPC_CINDERWEB_DRONE, 200.0f);
                            if (!drones.empty())
                            {
                                drones.remove_if([](Creature* drone)
                                {
                                    return (!drone->isAlive() || drone->GetPositionZ() >= 100.00f);
                                });
                            }

                            if (!drones.empty())
                            {
                                if (drones.size() > 1)
                                    drones.sort(Trinity::ObjectDistanceOrderPred(me));
                                me->GetMotionMaster()->MoveChase(*drones.begin());
                                me->getThreatManager().addThreat(*drones.begin(), 1000000);
                            }
                            else
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST, 0, 200.0f, true, -SPELL_WEB_SILK))
                                {
                                    if (me->GetDistance2d(target) <= 6.00f)
                                    {
                                        if (target->GetVehicleKit() && target->GetVehicleKit()->HasEmptySeat(0))
                                            DoCast(target, SPELL_SEEPING_VENOM, true);
                                    }
                                    else
                                    {
                                        me->GetMotionMaster()->MoveChase(target);
                                        me->SetInCombatWith(target);
                                        me->getThreatManager().tauntApply(target);
                                        ScriptedAI::AttackStart(target);
                                    }
                                }
                                else
                                    me->GetMotionMaster()->MovePoint(0, centerPos);
                            }
                        }
                        events.ScheduleEvent(EVENT_GET_TARGET, 1000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_cinderweb_spiderlingAI(creature);
    }
};

class npc_cinderweb_drone : public CreatureScript
{
    public:
        npc_cinderweb_drone() : CreatureScript("npc_cinderweb_drone") { }

        struct npc_cinderweb_droneAI : public ScriptedAI
        {
            npc_cinderweb_droneAI(Creature * creature) : ScriptedAI(creature), instance(me->GetInstanceScript()) { }

            bool CanAIAttack(Unit const* target) const override
            {
                if (target->GetPositionZ() >= 85.00f)
                    return false;
                return true;
            }

            void EnterEvadeMode() override
            {
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void IsSummonedBy(Unit* summoner) override
            {
                DoCast(me, SPELL_ENERGIZE, true);
                DoCast(me, SPELL_ZERO_MANA_REGEN, true);
                me->SetHomePosition(centerPos);
                me->GetMotionMaster()->MovePoint(0, centerPos, false);
                me->SetMaxPower(POWER_MANA, 85);
                me->SetPower(POWER_MANA, 85);
            }

            void EnterCombat(Unit* who) override
            {
                events.ScheduleEvent(EVENT_BOILING_SPATTER, 7000);
                events.ScheduleEvent(EVENT_BURNIGN_ACID_DRONE, 3000);
                events.ScheduleEvent(EVENT_CONSUME, 1000);
                events.ScheduleEvent(EVENT_CHECK_DRONE_ENERGY, 1000);
                events.ScheduleEvent(EVENT_CHECK_UPPER, 1000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_FIXATE, 15000);
            }

            void MovementInform(uint32 type, uint32 id) override
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id == POINT_UP)
                {
                    if (Creature* boss = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_BETHTILAC)))
                        DoCast(boss, SPELL_LEECH_VENOM, false);
                    me->DespawnOrUnsummon(20000);
                }
            }

            void JustDied(Unit* killer) override
            {
                for (size_t i = 0; i < 3; i++)
                    DoCast(me, SPELL_CREATE_CHITINOUS_FRAGMENT, true);
            }

            void UpdateAI(const uint32 diff) override
            {
                UpdateVictim();
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventid = events.ExecuteEvent())
                {
                    switch (eventid)
                    {
                        case EVENT_BURNIGN_ACID_DRONE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true, -SPELL_WEB_SILK))
                                DoCast(target, SPELL_BURNING_ACID);
                            events.ScheduleEvent(EVENT_BURNIGN_ACID_DRONE, urand(6000, 8000));
                            break;
                        case EVENT_BOILING_SPATTER:
                            DoCastVictim(SPELL_BOILING_SPATTER);
                            events.ScheduleEvent(EVENT_BOILING_SPATTER, urand(10000, 13000));
                            break;
                        case EVENT_CONSUME:
                            if (Creature* spiderling = me->FindNearestCreature(NPC_CINDERWEB_SPIDERLING, 10.0f, true))
                                DoCast(spiderling, SPELL_CONSUME);
                            events.ScheduleEvent(EVENT_CONSUME, 1100);
                            break;
                        case EVENT_CHECK_DRONE_ENERGY:
                            if (me->GetPower(POWER_MANA) == 0) // @TODO: implement spinning up event - spells not in sniff - These large spiders climb out of caves below the Cinderweb. When their Fire Energy fully depletes, the Cinderweb Drone climbs up to Beth'tilac and siphons Fire Energy from her.
                            {
                                me->SetHealth(me->GetMaxHealth());
                                events.Reset();
                                me->CombatStop(true);
                                me->SetReactState(REACT_PASSIVE);
                                me->SetCanFly(true);
                                me->SetDisableGravity(true);
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                Position pos(*me);
                                pos.m_positionZ = 112.00f;
                                me->GetMotionMaster()->MovePoint(POINT_UP, pos);
                                break;
                            }
                            events.ScheduleEvent(EVENT_CHECK_DRONE_ENERGY, 1000);
                            break;
                        case EVENT_FIXATE:
                            DoCastRandom(SPELL_FIXATE, 100.0f, false, -SPELL_WEB_SILK);
                            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                            events.ScheduleEvent(EVENT_FIXATE, 40000);
                            events.ScheduleEvent(EVENT_REMOVE_IMMUNITY, 10000);
                            break;
                        case EVENT_REMOVE_IMMUNITY:
                            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        private:
            EventMap events;
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_cinderweb_droneAI(creature);
        }
};

class npc_web_rip : public CreatureScript
{
public:
    npc_web_rip() : CreatureScript("npc_web_rip") { }

    struct npc_web_ripAI : public ScriptedAI
    {
        npc_web_ripAI(Creature* creature) : ScriptedAI(creature) { }

        void JustDied(Unit* target) override {}

        void IsSummonedBy(Unit* summoner) override
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->AddAura(SPELL_METEOR_BURN_AURA, me);
        }

        void Reset()
        {
            if (!me->HasAura(SPELL_METEOR_BURN_AURA))
                me->AddAura(SPELL_METEOR_BURN_AURA, me);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_web_ripAI(creature);
    }
};

class npc_lift_controller : public CreatureScript
{
public:
    npc_lift_controller() : CreatureScript("npc_lift_controller") { }

    struct npc_lift_controllerAI : public ScriptedAI
    {
        npc_lift_controllerAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetDisableGravity(true);
            me->SetReactState(REACT_PASSIVE);
            me->AddAura(SPELL_SPIDERWEB_FILAMENT_4, me);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetEntry() == BOSS_BETHTILAC)
                me->SummonCreature(NPC_CINDERWEB_SPINNER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() - 6.00f, 0, TEMPSUMMON_CORPSE_DESPAWN);
            else if (summoner->GetEntry() == NPC_CINDERWEB_DRONE)
                me->CastSpell(summoner, SPELL_SPIDERWEB_FILAMENT_2, true);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_lift_controllerAI(creature);
    }
};

class ReenablePathFindingEvent : public BasicEvent
{
public:
    ReenablePathFindingEvent(Creature* owner) : _owner(owner) { }

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        _owner->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);
        return true;
    }

private:
    Creature* _owner;
};

class npc_spiderling_stalker : public CreatureScript
{
public:
    npc_spiderling_stalker() : CreatureScript("npc_spiderling_stalker") { }

    struct npc_spiderling_stalkerAI : public ScriptedAI
    {
        npc_spiderling_stalkerAI(Creature* creature) : ScriptedAI(creature), summons(me) {}

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case ACTION_SUMMON_SPIDERLING:
                    DoCast(me, SPELL_SUMMON_CINDERWEB_SPIDERLING, true);
                    if (IsHeroic())
                        DoCast(me, SPELL_SUMMON_BANELING, true);
                    break;
                case ACTION_SUMMON_DRONE:
                    events.ScheduleEvent(EVENT_SUMMON_DRONE, 100);
                    break;
                case ACTION_SUMMON_SPINNERS:
                    spinnerCount = 0;
                    events.ScheduleEvent(EVENT_SUMMON_CINDERWEB_SPINNER, urand(100, 2000));
                    break;
                case ACTION_RESET_EVENTS:
                    me->RemoveAllAuras();
                    events.Reset();
                    summons.DespawnAll();
                    break;
                default:
                    break;
            }
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            if (summon->GetEntry() != NPC_CINDERWEB_SPINNER)
            {
                summon->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                summon->m_Events.AddEvent(new ReenablePathFindingEvent(summon), summon->m_Events.CalculateTime(2000));
            }
            summon->SetInCombatWithZone();
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_SUMMON_DRONE:
                        DoCast(me, SPELL_SUMMON_CINDERWEB_DRONE, true);
                        break;
                    case EVENT_SUMMON_CINDERWEB_SPINNER:
                        DoCast(me, SPELL_SUMMON_CINDERWEB_SPINNER, true);
                        if (++spinnerCount < 3)
                            events.ScheduleEvent(EVENT_SUMMON_CINDERWEB_SPINNER, urand(8000, 10000));
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        EventMap events;
        SummonList summons;
        uint8 spinnerCount;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_spiderling_stalkerAI(creature);
    }
};

class npc_engorged_broodling : public CreatureScript
{
public:
    npc_engorged_broodling() : CreatureScript("npc_engorged_broodling") { }

    struct npc_engorged_broodlingAI : public ScriptedAI
    {
        npc_engorged_broodlingAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* summoner) override
        {
            me->CastSpell(me, SPELL_BROODLING_CHANCRE_AURA, true);
            me->SetInCombatWithZone();
            events.ScheduleEvent(EVENT_CHECK_PLAYERS_IN_RANGE, 1000);
            // missed the spell visual that link player to the npc
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true, -SPELL_WEB_SILK))
            {
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                me->getThreatManager().addThreat(target, 100000.0f);
                me->getThreatManager().tauntApply(target);
                AttackStart(target);
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->CastSpell(me, SPELL_VOLATILE_POISON, true);
            me->CastSpell(me, SPELL_VOLATILE_BURST, true);
            me->DespawnOrUnsummon(30000);
        }

        bool CanAIAttack(Unit const* target) const override
        {
            if (target->GetPositionZ() >= 90.0f || target->HasAura(SPELL_WEB_SILK))
                return false;
            return true;
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!me->GetVehicle())
                if (Unit* victim = me->SelectVictim())
                    AttackStart(victim);

            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_CHECK_PLAYERS_IN_RANGE:
                        if (Player* player = me->FindNearestPlayer(5.0f))
                            me->Kill(me);
                        events.ScheduleEvent(EVENT_CHECK_PLAYERS_IN_RANGE, 500);
                        break;
                }
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_engorged_broodlingAI(creature);
    }
};

class spell_bethtilac_mana_burn : public SpellScriptLoader
{
public:
    spell_bethtilac_mana_burn() : SpellScriptLoader("spell_bethtilac_mana_burn") { }

    class spell_bethtilac_mana_burn_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_bethtilac_mana_burn_AuraScript);

        void OnPeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->GetTypeId() != TYPEID_UNIT)
                    return;

                if (caster->GetPower(POWER_MANA) == 0)
                    caster->GetAI()->DoAction(ACTION_DEVASTATION);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_bethtilac_mana_burn_AuraScript::OnPeriodicTick, EFFECT_0, SPELL_AURA_POWER_BURN);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_bethtilac_mana_burn_AuraScript();
    }
};

class spell_bethtilac_fiery_web_silk : public SpellScriptLoader
{
public:
    spell_bethtilac_fiery_web_silk() : SpellScriptLoader("spell_bethtilac_fiery_web_silk") { }

    class spell_bethtilac_fiery_web_silk_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_bethtilac_fiery_web_silk_AuraScript);

        void OnPeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if (GetTarget()->GetPositionZ() <= 108.0f)
            {
                if (Player* player = GetTarget()->ToPlayer())
                    player->GetMotionMaster()->MoveJump(62.273f, 390.738f, 74.04f, 13.0f, 13.7f);
                Remove(AURA_REMOVE_BY_DEFAULT);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_bethtilac_fiery_web_silk_AuraScript::OnPeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_bethtilac_fiery_web_silk_AuraScript();
    }

    class spell_bethtilac_fiery_web_silk_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_bethtilac_fiery_web_silk_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove_if([](WorldObject* target)
            {
                return target->GetPositionZ() < 109.00f;
            });
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_bethtilac_fiery_web_silk_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_bethtilac_fiery_web_silk_SpellScript();
    }
};

// useed by bethtilac consume, drone consume and fixate cast
class spell_consume : public SpellScriptLoader
{
public:
    spell_consume() : SpellScriptLoader("spell_consume") { }

    class spell_consume_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_consume_SpellScript);

        void OnHitTarget(SpellEffIndex /*effIndex*/)
        {
            GetHitUnit()->CastSpell(GetCaster(), GetSpellInfo()->Effects[EFFECT_0].BasePoints, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_consume_SpellScript::OnHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_consume_SpellScript;
    }
};

class spell_smoldering_devastation : public SpellScriptLoader
{
public:
    spell_smoldering_devastation() : SpellScriptLoader("spell_smoldering_devastation") { }

    class spell_smoldering_devastation_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_smoldering_devastation_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target) { return target->GetPositionZ() < 100.0f; });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_smoldering_devastation_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_smoldering_devastation_SpellScript();
    }
};


class spell_meteor_burn_force_cast : public SpellScriptLoader
{
public:
    spell_meteor_burn_force_cast() : SpellScriptLoader("spell_meteor_burn_force_cast") { }

    class spell_meteor_burn_force_cast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_meteor_burn_force_cast_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove_if([](WorldObject* target)
            {
                return target->GetTypeId() != TYPEID_PLAYER;
            });
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_meteor_burn_force_cast_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_meteor_burn_force_cast_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_meteor_burn_force_cast_SpellScript();
    }
};

class achievement_death_from_above : public AchievementCriteriaScript
 {
    public:
        achievement_death_from_above() : AchievementCriteriaScript("achievement_death_from_above") { }

        bool OnCheck(Player* /*player*/, Unit* target) override
        {
            return target && target->GetAI()->GetData(DATA_DEATH_FROM_ABOVE);
        }
};

void AddSC_boss_bethtilac()
{
    if (VehicleSeatEntry* vehSeat = const_cast<VehicleSeatEntry*>(sVehicleSeatStore.LookupEntry(9928)))
        vehSeat->m_flags |= VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT;

    new boss_bethtilac();
    new npc_cinderweb_spinner();
    new npc_spirderweb_filament();
    new npc_cinderweb_spiderling();
    new npc_cinderweb_drone();
    new npc_web_rip();
    new npc_lift_controller();
    new npc_spiderling_stalker();
    new npc_engorged_broodling();
    new spell_bethtilac_mana_burn();
    new spell_bethtilac_fiery_web_silk();
    new spell_consume();
    new spell_smoldering_devastation();
    new spell_meteor_burn_force_cast();
    new achievement_death_from_above();
}
