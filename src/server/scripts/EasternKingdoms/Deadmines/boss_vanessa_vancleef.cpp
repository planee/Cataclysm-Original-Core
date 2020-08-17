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
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "deadmines.h"

enum Spells
{
   SPELL_SUMMON_SMALL_MOONBEAM     = 98724,
   SPELL_SUMMON_ENFORCER           = 92616,
   SPELL_SUMMON_SORCER             = 92617,

   SPELL_DEFLECTION_2              = 92615,
   SPELL_DEFLECTION                = 92614,

   SPELL_DEADLY_BLADES             = 92622, // trigger SPELL_BACKSLASH_TARGET
   SPELL_BACKSLASH_TARGET          = 92620, // dummy SPELL_BACKSLASH_TARGET
   SPELL_BACKSLASH                 = 92619,

   SPELL_VENGEANCE_OF_VANCLEEF     = 95542,

   SPELL_SUMMON_SHADOW_GUARD       = 92618,

   SPELL_POWDER_EXPLOSION          = 96283,

   // 45979
   SPELL_FIERY_BLAZE               = 93484, //  Entry: 45979
   SPELL_FIERY_BLAZE_DAMAGE        = 93485, //  Entry: 45979

   // 49850
   SPELL_CHARGE                    = 90928, // ?? casted by ype: Unit Entry: 49850
   SPELL_BLOOD_BAIN                = 90925,
   SPELL_BLOOD_BAIN_2              = 90923,
   SPELL_TEMERITE                  = 90929,
   // 49852
   SPELL_DISSIMULATION             = 90954,
   SPELL_EVASION                   = 90958,
   SPELL_SHADOWSTEP                = 90956,
   SPELL_SINISTER_STRIKE           = 90951,
   SPELL_LAME_TURNING              = 90961,
   // 49854
   SPELL_BLOOD_LIGHTING            = 90938,
   SPELL_RAGE_ZONE                 = 90932,
   // 49550
   SPELL_CLICK_ME                  = 95527,
   SPELL_RIDE_VEHICLE              = 89731,

   SPELL_BLOOD_WASH                = 90946,
};

enum vancleefNpcs
{
    NPC_GENERAL_BUNNY             = 45979,
    NPC_DEFIAS_ENFORCER           = 49850,
    NPC_DEFIAS_SHADOWGUARD        = 49852,
    NPC_ROPE                      = 49550,
    NPC_VANESSA_ROPE_ANCHOR       = 49552,

};

enum vancleefEvents
{
    EVENT_DEFLECTION = 1,
    EVENT_DEADLY_BLADE,
    EVENT_FLAME,
    EVENT_ACTIVATE_FIRE,
    EVENT_ACTIVATE_ROPE,
    EVENT_ACTIVATE_FIRE_END,
    EVENT_VENGEANCE_OF_VANCLEEF,
    EVENT_SUMMON_SHADOWGUARD,
    EVENT_SUMMON_ENFORCER,
    EVENT_SUMMON_SORCER,
    EVENT_POWDER_EXPLOSION,
    EVENT_CHECK_NEAR_PLAYER,
    EVENT_OUTRO,
    EVENT_OUTRO_1,
    POINT_ROPE_FAR,
    POINT_ROPE_ORIG,

    EVENT_TEMERITE,
    EVENT_BLOOD_BAIN,

    EVENT_BLOOD_LIGHTING,
    EVENT_RAGE_ZONE,
    EVENT_BLOOD_WASH,

    EVENT_SINISTER_STRIKE,
    EVENT_SHADOW_STEP,
    EVENT_DISSIMULATION,
    EVENT_LAME_TURNING,
};

Position const vancleefEventFire =  {-76.16f, -819.90f, 56.40f, 0.0f};

#define FLAMMES_NBR 50

Position const flammesPos[50] =
{
    {-100.849f, -703.773f, 9.294073f, 0.0f},
    {-88.4253f, -724.722f, 8.675033f, 0.0f},
    {-72.559f, -731.221f, 8.586903f, 0.0f},
    {-30.6337f, -727.731f, 8.521023f, 0.0f},
    {-49.3264f, -730.056f, 9.320483f, 0.0f},

    {-12.0556f, -740.252f, 9.109463f, 0.0f},
    {0.512153f, -768.229f, 9.801343f, 0.0f},
    {-52.13195f, -816.1077f, 51.65916f, 0.0f},
    {-96.04688f, -694.434f, 8.508634f, 0.0f},
    {-70.9809f, -795.5f, 39.14447f, 0.0f},

    {-66.18229f, -789.2778f, 39.78834f, 0.0f},
    {-75.50694f, -787.1597f, 39.16325f, 0.0f},
    {-46.48438f, -791.8073f, 39.12098f, 0.0f},
    {-64.02257f, -797.4323f, 39.12309f, 0.0f},
    {-38.11458f, -795.2952f, 39.45137f, 0.0f},

    {-70.9809f, -795.5f, 39.14447f, 0.0f},
    {-66.18229f, -789.2778f, 39.78834f, 0.0f},
    {-75.50694f, -787.1597f, 39.16325f, 0.0f},
    {-46.48438f, -791.8073f, 39.12098f, 0.0f},
    {-64.02257f, -797.4323f, 39.12309f, 0.0f},

    {-38.11458f, -795.2952f, 32.45137f, 0.0f},
    {-70.9809f, -795.5f, 32.14447f, 0.0f},
    {-66.18229f, -789.2778f, 32.78834f, 0.0f},
    {-75.50694f, -787.1597f, 32.16325f, 0.0f},
    {-46.48438f, -791.8073f, 32.12098f, 0.0f},

    {-64.02257f, -797.4323f, 32.12309f, 0.0f},
    {-38.11458f, -795.2952f, 19.45137f, 0.0f},
    {-70.9809f, -795.5f, 19.14447f, 0.0f},
    {-66.18229f, -789.2778f, 19.78834f, 0.0f},
    {-75.50694f, -787.1597f, 19.16325f, 0.0f},

    {-46.48438f, -791.8073f, 19.12098f, 0.0f},
    {-64.02257f, -797.4323f, 19.12309f, 0.0f},
    {-95.849f, -703.773f, 9.294073f, 0.0f},
    {-83.4253f, -724.722f, 8.675033f, 0.0f},
    {-67.559f, -731.221f, 8.586903f, 0.0f},

    {-25.6337f, -727.731f, 8.521023f, 0.0f},
    {-43.3264f, -730.056f, 9.320483f, 0.0f},
    {-5.0556f, -740.252f, 9.109463f, 0.0f},
    {2.512153f, -768.229f, 9.801343f, 0.0f},
    {-95.04688f, -694.434f, 8.508634f, 0.0f},

    {-55.29f, -788.46f, 38.94f, 0.0f},
    {-86.29f, -797.46f, 37.24f, 0.0f},
    {-106.31f, -792.07f, 28.22f, 0.0f},
    {-83.55f, -780.84f, 26.62f, 0.0f},
    {-124.80f, -793.47f, 17.0f, 0.0f},

    {-55.29f, -788.46f, 32.94f, 0.0f},
    {-86.29f, -797.46f, 32.24f, 0.0f},
    {-106.31f, -792.07f, 19.22f, 0.0f},
    {-83.55f, -780.84f, 19.62f, 0.0f},
    {-124.80f, -793.47f, 17.0f, 0.0f},
};

#define FIGHT_FLAMMES_NBR 45

Position const fightFlammesPos[45] =
{
    {-75.13f, -810.34f, 40.17f, 0.0f},
    {-101.12f, -813.30f, 38.2f, 0.0f},
    {-102.95f, -819.37f, 38.04f, 0.0f},
    {-100.33f, -826.24f, 38.23f, 0.0f},
    {-94.05f, -830.19f, 38.70f, 0.0f},

    {-53.02f, -809.1f, 42.23f, 0.0f},
    {-46.68f, -808.97f, 42.83f, 0.0f},
    {-33.68f, -814.40f, 40.44f, 0.0f},
    {-33.68f, -814.40f, 47.44f, 0.0f},
    {-107.40f, -827.68f, 30.30f, 0.0f},

    {-99.17f, -833.52f, 30.65f, 0.0f},
    {-94.41f, -836.89f, 30.93f, 0.0f},
    {-53.59f, -841.63f, 33.54f, 0.0f},
    {-45.66f, -838.46f, 34.15f, 0.0f},
    {-38.77f, -829.36f, 34.59f, 0.0f},

    {-45.20f, -845.15f, 23.06f, 0.0f},
    {-55.29f, -847.36f, 22.53f, 0.0f},
    {-80.29f, -847.32f, 18.58f, 0.0f},
    {-92.84f, -843.78f, 21.23f, 0.0f},
    {-99.39f, -840.64f, 21.09f, 0.0f},

    {-106.49f, -836.40f, 20.96f, 0.0f},
    {-112.20f, -832.48f, 20.10f, 0.0f},
    {-125.22f, -826.18f, 16.87f, 0.0f},
    {-122.74f, -835.18f, 16.90f, 0.0f},
    {-114.95f, -842.11f, 16.90f, 0.0f},

    {-105.15f, -848.39f, 16.98f, 0.0f},
    {-96.62f, -851.95f, 17.10f, 0.0f},
    {-91.10f, -853.79f, 17.17f, 0.0f},
    {-86.62f, -855.95f, 17.20f, 0.0f},
    {-81.10f, -857.79f, 17.27f, 0.0f},

    {-81.62f, -858.95f, 17.20f, 0.0f},
    {-76.10f, -858.79f, 17.27f, 0.0f},
    {-71.62f, -858.95f, 17.20f, 0.0f},
    {-66.10f, -858.79f, 17.27f, 0.0f},
    {-61.62f, -858.95f, 17.20f, 0.0f},

    {-56.15f, -857.39f, 19.98f, 0.0f},
    {-51.62f, -855.95f, 19.10f, 0.0f},
    {-46.10f, -853.79f, 19.17f, 0.0f},
    {-41.62f, -851.95f, 19.20f, 0.0f},
    {-36.10f, -848.79f, 19.27f, 0.0f},

    {-31.15f, -846.39f, 19.98f, 0.0f},
    {-26.62f, -842.15f, 19.10f, 0.0f},
    {-21.10f, -838.89f, 19.17f, 0.0f},
    {-16.62f, -834.65f, 19.20f, 0.0f},
    {-11.10f, -830.79f, 19.27f, 0.0f},
};

#define TEMP_FLAMMES_NBR 27

Position const tempFlammesPos[27] =
{
    {-73.389f, -838.31f, 40.4f, 0.0f},
    {-69.389f, -838.31f, 40.7f, 0.0f},
    {-65.389f, -838.31f, 41.1f, 0.0f},
    {-61.389f, -837.31f, 41.5f, 0.0f},
    {-58.389f, -834.31f, 41.7f, 0.0f},

    {-73.389f, -834.31f, 41.4f, 0.0f},
    {-69.389f, -833.31f, 41.7f, 0.0f},
    {-65.389f, -834.31f, 41.4f, 0.0f},
    {-61.389f, -833.31f, 41.5f, 0.0f},
    {-58.389f, -832.31f, 41.7f, 0.0f},

    {-73.389f, -827.31f, 41.4f, 0.0f},
    {-69.389f, -827.31f, 41.7f, 0.0f},
    {-65.389f, -827.31f, 41.4f, 0.0f},
    {-61.389f, -827.31f, 41.5f, 0.0f},
    {-58.389f, -827.31f, 41.7f, 0.0f},

    {-81.57f, -807.92f, 39.67f, 0.0f},
    {-88.90f, -807.93f, 39.67f, 0.0f},
    {-95.62f, -908.84f, 38.5f, 0.0f},
    {-49.36f, -831.14f, 41.62f, 0.0f},
    {-63.10f, -819.36f, 41.26f, 0.0f},

    {-60.46f, -814.94f, 41.52f, 0.0f},
    {-63.77f, -808.26f, 41.23f, 0.0f},
    {-66.69f, -813.01f, 40.96f, 0.0f},
    {-71.48f, -819.45f, 40.49f, 0.0f},
    {-83.17f, -819.23f, 39.53f, 0.0f},

    {-73.17f, -844.01f, 32.21f, 0.0f},
    {-65.17f, -843.01f, 32.71f, 0.0f},
};

class boss_vanessa_vancleef : public CreatureScript
{
public:
    boss_vanessa_vancleef() : CreatureScript("boss_vanessa_vancleef") { }

    struct boss_vanessa_vancleefAI : public BossAI
    {
        boss_vanessa_vancleefAI(Creature* creature) : BossAI(creature, BOSS_VANCLEEF) { }

        void Reset()
        {
            firstFlames = false;
            secondFlames = false;
            phaseCbt = false;
            endEvent = false;
            if (instance && instance->IsDone(BOSS_EVENT_VANCLEEF))
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_DEFENSIVE);
            }
            BossAI::Reset();
        }

        void JustDied(Unit* killer)
        {
            if (instance)
            {
                Position pos;
                me->GetPosition(&pos);
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->getSource())
                            if (player->isAlive())
                                player->ExitVehicle(&pos);

                instance->FinishLfgDungeon(me);
            }
            _JustDied();
        }

        void JustSummoned(Creature* summon)
        {
            switch (summon->GetEntry())
            {
                case NPC_GENERAL_BUNNY:
                {
                    summon->CastSpell(summon, SPELL_FIERY_BLAZE, true);
                    break;
                }
                case NPC_ROPE:
                {
                    if (Creature *marker = summon->FindNearestCreature(NPC_VANESSA_ROPE_ANCHOR, 200))
                        summon->CastSpell(marker, SPELL_ROPE_RAY, true);
                    SetInact(true);
                    break;
                }
            }
            BossAI::JustSummoned(summon);
        }

        void EnterCombat(Unit * /*who*/)
        {
            Talk(0);
            for (unsigned int cnt = 0; cnt < FLAMMES_NBR; cnt++)
                me->SummonCreature(NPC_GENERAL_BUNNY, flammesPos[cnt]);
            StartEvents();
            events.ScheduleEvent(EVENT_SUMMON_ENFORCER, 15000);
            _EnterCombat();
        }

        void StartEvents()
        {
            if (!secondFlames)
                events.ScheduleEvent(EVENT_DEFLECTION, 5000);
            events.ScheduleEvent(EVENT_DEADLY_BLADE, 15000);
        }

        void DamageTaken(Unit* caster, uint32& damage)
        {
            if (phaseCbt)
            {
                SetInact(false);
                phaseCbt = false;
            }

            if (!firstFlames && HealthBelowPct(50))
            {
                events.ScheduleEvent(EVENT_FLAME, 0);
                firstFlames = true;
            }

            if (!secondFlames && HealthBelowPct(25))
            {
                events.ScheduleEvent(EVENT_FLAME, 0);
                secondFlames = true;
            }

            if (damage > me->GetHealth())
            {
                damage = 0;
                if (!endEvent)
                {
                    events.Reset();
                    SetInact(true);
                    Talk(6);
                    events.ScheduleEvent(EVENT_OUTRO, 5000);
                }
                endEvent = true;
            }
        }

        void DespawnCreatures(uint32 entry, float distance)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, me, entry, distance);

            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }

        void SummonRopes()
        {
            me->SummonCreature(NPC_ROPE, -62.17014f, -839.8438f, 41.48514f, 5.044002f);
            me->SummonCreature(NPC_ROPE, -64.97049f, -840.0087f, 41.22668f, 4.834562f);
            me->SummonCreature(NPC_ROPE, -67.79166f, -840.1736f, 40.96664f, 4.712389f);
            me->SummonCreature(NPC_ROPE, -70.62153f, -840.0104f, 40.71846f, 4.537856f);
            me->SummonCreature(NPC_ROPE, -73.42361f, -839.8646f, 40.47259f, 4.34587f);
        }

        void ActivateFire(bool on)
        {
            if (on)
            {
                if (!secondFlames)
                    for (unsigned int cnt = 0; cnt < FIGHT_FLAMMES_NBR; cnt++)
                        me->SummonCreature(NPC_GENERAL_BUNNY, fightFlammesPos[cnt]);
                _tempFlame.clear();
                for (unsigned int cnt = 0; cnt < TEMP_FLAMMES_NBR; cnt++)
                    if (Creature *c = me->SummonCreature(NPC_GENERAL_BUNNY, tempFlammesPos[cnt]))
                        _tempFlame.push_back(c->GetGUID());
            }
            else
                for (std::list<uint64 >::iterator itr = _tempFlame.begin(); itr != _tempFlame.end(); itr++)
                    if (Creature *flame = Unit::GetCreature(*me, *itr))
                        flame->DespawnOrUnsummon(1000);
        }

        void SetInact(bool on)
        {
            if (on)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveAllAuras();
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                me->StopMoving();
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MoveIdle();
            }
            else
            {
                DespawnCreatures(NPC_ROPE, 200);
                events.CancelEvent(EVENT_VENGEANCE_OF_VANCLEEF);
                me->SetReactState(REACT_AGGRESSIVE);
                if (me->getVictim())
                {
                    DoStartMovement(me->getVictim());
                    AttackStart(me->getVictim());
                }
                StartEvents();
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
                    case EVENT_DEFLECTION:
                        me->CastSpell(me, SPELL_DEFLECTION, true);
                        events.ScheduleEvent(EVENT_DEFLECTION, 30000);
                        break;
                    case EVENT_DEADLY_BLADE:
                        DoCast(SPELL_DEADLY_BLADES);
                        events.ScheduleEvent(EVENT_DEADLY_BLADE, 20000);
                        break;
                    case EVENT_FLAME:
                        events.Reset();
                        me->NearTeleportTo(vancleefEventFire.m_positionX, vancleefEventFire.m_positionY, vancleefEventFire.m_positionZ, vancleefEventFire.m_orientation);
                        SetInact(true);
                        if (!secondFlames)
                            Talk(2);
                        else
                            Talk(4);
                        events.ScheduleEvent(EVENT_ACTIVATE_ROPE, 6000);
                        break;
                    case EVENT_ACTIVATE_ROPE:
                        if (!secondFlames)
                            Talk(3);
                        else
                            Talk(5);
                        SummonRopes();
                        events.ScheduleEvent(EVENT_ACTIVATE_FIRE, 5000);
                        break;
                    case EVENT_ACTIVATE_FIRE:
                        ActivateFire(true);
                        SetInact(true);
                        events.ScheduleEvent(EVENT_ACTIVATE_FIRE_END, 3000);
                        break;
                    case EVENT_ACTIVATE_FIRE_END:
                        ActivateFire(false);
                        SetInact(true);
                        me->GetMotionMaster()->MoveJump(-64.5677f, -820.161f, 41.123f, 20.0f, 20.0f);
                        phaseCbt = true;
                        events.ScheduleEvent(EVENT_VENGEANCE_OF_VANCLEEF, 2000);
                        break;
                    case EVENT_VENGEANCE_OF_VANCLEEF:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        DoCast(SPELL_VENGEANCE_OF_VANCLEEF);
                        events.ScheduleEvent(EVENT_VENGEANCE_OF_VANCLEEF, 1000);
                        break;
                    case EVENT_SUMMON_ENFORCER:
                        Talk(1);
                        DoCast(SPELL_SUMMON_ENFORCER);
                        events.ScheduleEvent(EVENT_SUMMON_SHADOWGUARD, 8000);
                        break;
                    case EVENT_SUMMON_SHADOWGUARD:
                        Talk(1);
                        DoCast(SPELL_SUMMON_SHADOW_GUARD);
                        events.ScheduleEvent(EVENT_SUMMON_SORCER, 8000);
                        break;
                    case EVENT_SUMMON_SORCER:
                        Talk(1);
                        DoCast(SPELL_SUMMON_SORCER);
                        events.ScheduleEvent(EVENT_SUMMON_ENFORCER, 15000);
                        break;
                    case EVENT_OUTRO:
                        Talk(7);
                        events.ScheduleEvent(EVENT_OUTRO_1, 4000);
                        break;
                    case EVENT_OUTRO_1:
                        Talk(8);
                        SummonRopes();
                        events.ScheduleEvent(EVENT_POWDER_EXPLOSION, 5000);
                        break;
                    case EVENT_POWDER_EXPLOSION:
                        Talk(9);
                        DoCast(SPELL_POWDER_EXPLOSION);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        bool firstFlames, secondFlames, phaseCbt, endEvent;
        std::list<uint64 > _tempFlame;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_vanessa_vancleefAI (creature);
    }
};

// 92620
class spell_vc_backslash_target_selector : public SpellScriptLoader
{
public:
    spell_vc_backslash_target_selector() : SpellScriptLoader("spell_vc_backslash_target_selector") { }

    class NonPreviousTarget
    {
        public:
            explicit NonPreviousTarget(Unit* unit) : _lastTarget(unit){};

            bool operator()(WorldObject* target) const
            {
                return _lastTarget == target;
            }

        private:
            Unit const* _lastTarget;
    };


    class spell_vc_backslash_target_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_vc_backslash_target_selector_SpellScript);

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            return true;
        }

        bool Load()
        {
            previousTarget = NULL;
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            if (unitList.size() > 1)
                unitList.remove_if(NonPreviousTarget(previousTarget));
            if (!unitList.empty())
                Trinity::Containers::RandomResizeList(unitList, 1);
        }


        void HandleDummyEffect(SpellEffIndex effIndex)
        {
            if (Unit *caster = GetCaster())
                if (Unit *target = GetHitUnit())
                {
                    previousTarget = target;
                    uint32 spellId = GetSpellInfo()->Effects[effIndex].CalcValue();
                    caster->CastSpell(target, spellId, true);
                }
        }

    private:
        Unit *previousTarget;

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_vc_backslash_target_selector_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_vc_backslash_target_selector_SpellScript::HandleDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_vc_backslash_target_selector_SpellScript();
    }
};

// 49550
class npc_vancleef_rope : public CreatureScript
{
public:
    npc_vancleef_rope() : CreatureScript("npc_vancleef_rope") { }

    class BackHome : public BasicEvent
    {
        public:
            BackHome(Unit* owner, Position const& cu) : _owner(owner), _spawnPos(cu)
            {
            }

            bool Execute(uint64 /*execTime*/, uint32 /*diff*/)
            {
                _owner->GetMotionMaster()->MovePoint(POINT_ROPE_ORIG, _spawnPos, false, 12.0f);
                return true;
            }

        private:
            Unit* _owner;
            Position _spawnPos;
    };


    struct npc_vancleef_ropeAI : public PassiveAI
    {
        npc_vancleef_ropeAI(Creature* creature) : PassiveAI(creature)
        {
            instance = me->GetInstanceScript();
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }

        void OnSpellClick(Unit* player, bool& result)
        {
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }


        void PassengerBoarded(Unit* player, int8 seatId, bool apply)
        {
            if (!apply)
            {
                player->SetDisableGravity(false);
                player->SendMovementDisableGravity();
                return;
            }
            player->SetDisableGravity(true);
            player->SendMovementDisableGravity();
            Position pos;
            me->GetNearPoint2D(pos.m_positionX, pos.m_positionY, 40.0f, me->GetOrientation());
            pos.m_positionZ = me->GetPositionZ() + 2.0f;
            me->GetNearPoint2D(_spawnPos.m_positionX, _spawnPos.m_positionY, 5.0f, me->GetOrientation() + M_PI);
            _spawnPos.m_positionZ = me->GetPositionZ() + 5.0f;
            me->GetMotionMaster()->MovePoint(POINT_ROPE_FAR, pos, false, 12.0f);
        }

        void MovementInform(uint32 type, uint32 point)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            if (point == POINT_ROPE_FAR)
                me->m_Events.AddEvent(new BackHome(me, _spawnPos), me->m_Events.CalculateTime(100));
            else if (point == POINT_ROPE_ORIG)
                me->DespawnOrUnsummon(1000);
        }

        void UpdateAI(const uint32 diff) {}

        private:
            InstanceScript* instance;
            Position _spawnPos;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_vancleef_ropeAI (creature);
    }
};

// 49850
class npc_vc_enforcer : public CreatureScript
{
public:
    npc_vc_enforcer() : CreatureScript("npc_vc_enforcer") { }

    struct npc_vc_enforcerAI : public ScriptedAI
    {
        npc_vc_enforcerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset()
        {
            _events.Reset();
            _events.ScheduleEvent(EVENT_BLOOD_BAIN, 5000);
            _events.ScheduleEvent(EVENT_TEMERITE, 9000);
        }

        void EnterCombat(Unit * who)
        {
            me->CastSpell(who, SPELL_CHARGE, true);
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
                    case EVENT_BLOOD_BAIN:
                        DoCast(SPELL_BLOOD_BAIN);
                        _events.ScheduleEvent(EVENT_BLOOD_BAIN, 10000);
                        break;
                    case EVENT_TEMERITE:
                        DoCast(SPELL_TEMERITE);
                        _events.ScheduleEvent(EVENT_TEMERITE, 10000);
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
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_vc_enforcerAI (creature);
    }
};

// 49854
class npc_vc_sorcer : public CreatureScript
{
public:
    npc_vc_sorcer() : CreatureScript("npc_vc_sorcer") { }

    struct npc_vc_sorcerAI : public ScriptedAI
    {
        npc_vc_sorcerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset()
        {
            _events.Reset();
            _events.ScheduleEvent(EVENT_BLOOD_LIGHTING, 4000);
            _events.ScheduleEvent(EVENT_RAGE_ZONE, 2000);
            _events.ScheduleEvent(EVENT_BLOOD_WASH, 15000);
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
                    case EVENT_BLOOD_LIGHTING:
                        DoCast(SPELL_BLOOD_LIGHTING);
                        _events.ScheduleEvent(EVENT_BLOOD_LIGHTING, 4000);
                        break;
                    case EVENT_RAGE_ZONE:
                        DoCastRandom(SPELL_RAGE_ZONE, 0.0f);
                        _events.ScheduleEvent(EVENT_RAGE_ZONE, 15000);
                        break;
                    case EVENT_BLOOD_WASH:
                    {
                        Creature *target =  me->FindNearestCreature(NPC_DEFIAS_ENFORCER, 50.0f);
                        if (!target)
                            target = me->FindNearestCreature(NPC_DEFIAS_ENFORCER, 50.0f);
                        if (!target)
                            target = me->FindNearestCreature(NPC_DEFIAS_SHADOWGUARD, 50.0f);
                        if (!target)
                            target = me->FindNearestCreature(NPC_VANCLEEF, 100.0f);
                        if (target)
                            DoCast(target, SPELL_BLOOD_WASH);
                        _events.ScheduleEvent(EVENT_BLOOD_WASH, 15000);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

    private :
        InstanceScript *instance;
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_vc_sorcerAI (creature);
    }
};

// 49852
class npc_vc_shadowguard : public CreatureScript
{
public:
    npc_vc_shadowguard() : CreatureScript("npc_vc_shadowguard") { }

    struct npc_vc_shadowguardAI : public ScriptedAI
    {
        npc_vc_shadowguardAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset()
        {
            _events.Reset();
            _events.ScheduleEvent(EVENT_SINISTER_STRIKE, 8000);
            _events.ScheduleEvent(EVENT_SHADOW_STEP, 2000);
            _events.ScheduleEvent(EVENT_DISSIMULATION, 10000);
            _events.ScheduleEvent(EVENT_LAME_TURNING, 15000);
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
                case EVENT_SINISTER_STRIKE:
                    DoCastVictim(SPELL_SINISTER_STRIKE);
                    _events.ScheduleEvent(EVENT_SINISTER_STRIKE, 10000);
                    break;
                case EVENT_SHADOW_STEP:
                    DoCast(SPELL_SHADOWSTEP);
                    _events.ScheduleEvent(EVENT_SHADOW_STEP, 10000);
                    break;
                case EVENT_DISSIMULATION:
                    DoCast(SPELL_DISSIMULATION);
                    if (HealthBelowPct(30))
                        DoCast(SPELL_EVASION);
                    _events.ScheduleEvent(EVENT_DISSIMULATION, 7000);
                    break;
                case EVENT_LAME_TURNING:
                    DoCastVictim(SPELL_LAME_TURNING);
                    _events.ScheduleEvent(EVENT_LAME_TURNING, 15000);
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
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_vc_shadowguardAI (creature);
    }
};

void AddSC_boss_vanessa_vancleef()
{
    new spell_vc_backslash_target_selector();
    new boss_vanessa_vancleef();
    new npc_vancleef_rope();
    new npc_vc_enforcer();
    new npc_vc_sorcer();
    new npc_vc_shadowguard();
}
