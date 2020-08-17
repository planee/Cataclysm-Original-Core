/*
* Copyright (C) 2013-2014 OMFG.GG Network <http://www.omfg.gg/>
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

#include "GridNotifiers.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "firelands.h"

enum Spells
{
    SPELL_INFERNO_BLADE         = 99350,
    SPELL_DECIMATION_BLADE      = 99352,
    SPELL_DECIMATION_BLADE_25   = 99405,

    SPELL_BLAZE_OF_GLORY        = 99252,
    SPELL_INCENDIARY_SOUL       = 99369,

    SPELL_SHARDS_OF_TORMENT     = 99259,
    SPELL_SHARDS_OF_TORMENT_2   = 99260,
    SPELL_TORMENT_COSMETIC_1    = 99258,
    SPELL_TORMENT               = 99254,
    SPELL_TORMENT_PERIODIC      = 99255,
    SPELL_WAVE_OF_TORMENT       = 99261,
    SPELL_TORMENTED             = 99257,

    SPELL_COUNTDOWN             = 99515,
    SPELL_COUNTDOWN_PERIODIC    = 99516,
    SPELL_COUNTDOWN_DAMAGE      = 99518,
    SPELL_COUNTDOWN_VISUAL      = 99519,

    SPELL_VITAL_SPARK           = 99262,
    SPELL_VITAL_FLAME           = 99263,

    SPELL_BERSERK               = 26662
};

enum Events
{
    EVENT_BLADE                 = 1,
    EVENT_INCENDIARY_SOUL       = 2,
    EVENT_SHARDS_OF_TORMENT     = 3,
    EVENT_COUNTDOWN             = 4,
    EVENT_BERSERK               = 5,

    EVENT_SHARD_SPAWN_EFFECT    = 6,
    EVENT_UNLOCK_YELLSPAM       = 7,
    EVENT_INFERNO_BLADE         = 8
};

enum Emotes
{
    EMOTE_AGGRO             = 0,
    EMOTE_SHARDS_OF_TORMENT = 1,
    EMOTE_INFERNO_BLADE     = 2,
    EMOTE_DECIMATION_BLADE  = 3,
    EMOTE_KILL              = 4,
    EMOTE_ENRAGE            = 5,
    EMOTE_ENRAGE_2          = 6,
    EMOTE_DEATH             = 7,
    ABILITY_INFERNO_BLADE   = 8,
    ABILITY_DECIMATION_BLADE = 9
};

enum Guids
{
    GUID_TORMENTED          = 1,
};

enum Misc
{
    EQUIP_DECIMATION_BLADE  = 71082,
    EQUIP_INFERNO_BLADE     = 71138,
    ACTION_RESTORE_WEAPONS  = 1
};

#define GOSSIP_HELLO_ORB    "<Pick up the orb and shake it vigorously.>"

class boss_baleroc : public CreatureScript
{
public:
    boss_baleroc() : CreatureScript("boss_baleroc") { }

    struct boss_balerocAI : public BossAI
    {
        boss_balerocAI(Creature* creature) : BossAI(creature, DATA_BALOROC) {}

        void MoveInLineOfSight(Unit* who)
        {
            if (!me->getVictim() && me->IsValidAttackTarget(who))
            {
                if (me->IsWithinDistInMap(who, 1.f))
                {
                    AttackStart(who);
                    me->SetInCombatWithZone();
                }
            }
        }

        void Reset() override
        {
            _Reset();
            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
            {
                uint32 preNerfHealth = RAID_MODE(42087080, 133304386, 69916089, 195576075);
                me->SetMaxHealth(preNerfHealth);
                me->SetHealth(preNerfHealth);
            }
            me->SetMaxPower(POWER_RAGE, 0);
            SetEquipmentSlots(true);
            me->SetCanDualWield(true);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLAZE_OF_GLORY);
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
        {
            switch (spell->Id)
            {
                case SPELL_INFERNO_BLADE:
                    SetEquipmentSlots(false, EQUIP_INFERNO_BLADE, EQUIP_UNEQUIP);
                    me->SetCanDualWield(false);
                    break;
                case SPELL_DECIMATION_BLADE:
                case SPELL_DECIMATION_BLADE_25:
                    SetEquipmentSlots(false, EQUIP_DECIMATION_BLADE, EQUIP_UNEQUIP);
                    me->SetCanDualWield(false);
                    break;
                default:
                    break;
            }
        }

        void DoAction(int32 const action)
        {
            if (action == ACTION_RESTORE_WEAPONS)
            {
                SetEquipmentSlots(true);
                me->SetCanDualWield(true);
            }
        }

        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
            Talk(EMOTE_AGGRO);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            events.ScheduleEvent(EVENT_INCENDIARY_SOUL, 8.5*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SHARDS_OF_TORMENT, 5 * IN_MILLISECONDS);
            if (me->GetMap()->IsHeroic())
                events.ScheduleEvent(EVENT_COUNTDOWN, 26 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_INFERNO_BLADE, 30.5*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_BERSERK, 6 * MINUTE*IN_MILLISECONDS);

            // Reset our achievement list. We do this here and not in reset, as the debuff may have been spread after the boss has reset.
            _sharedThePain.clear();
        }

        void KilledUnit(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER && _canYellKilledPlayer)
            {
                _canYellKilledPlayer = false;
                events.ScheduleEvent(EVENT_UNLOCK_YELLSPAM, 8000);
                Talk(EMOTE_KILL);
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
            Talk(EMOTE_DEATH);
            SetEquipmentSlots(true);
            me->SetCanDualWield(true);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLAZE_OF_GLORY);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            if (instance->IsDone(DATA_SHANNOX) && instance->IsDone(DATA_LORD_RHYOLITH) && instance->IsDone(DATA_BETHTILAC) && instance->IsDone(DATA_ALYSRAZOR))
            {
                if (Creature* orb = Creature::GetCreature(*me, instance->GetData64(NPC_MAGMA_ORB)))
                {
                    orb->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    orb->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            if (Creature* majordomo = Creature::GetCreature(*me, instance->GetData64(DATA_MAJORDOMO_STAGHELM)))
                majordomo->AI()->DoAction(ACTION_START_INTRO);

            Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            {
                if (i->getSource()->HasQuestForItem(ITEM_HEART_OF_FLAME))
                {
                    DoCast(me, SPELL_SMOULDERING, true);
                    break;
                }
            }
        }

        void EnterEvadeMode() override
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLAZE_OF_GLORY);
            BossAI::EnterEvadeMode();
        }

        void UpdateAI(uint32 const diff) override
        {
            if (me->isAlive())
                if (!UpdateVictim())
                    return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_INFERNO_BLADE:
                        DoCast(me, SPELL_INFERNO_BLADE);
                        Talk(EMOTE_INFERNO_BLADE);
                        Talk(ABILITY_INFERNO_BLADE);
                        events.ScheduleEvent(EVENT_BLADE, 47 * IN_MILLISECONDS);
                        break;
                    case EVENT_BLADE:
                        switch (urand(1, 2))
                        {
                            case 1:
                            DoCast(me, SPELL_INFERNO_BLADE);
                            Talk(EMOTE_INFERNO_BLADE);
                            Talk(ABILITY_INFERNO_BLADE);
                            break;
                        case 2:
                            DoCast(me, SPELL_DECIMATION_BLADE);
                            Talk(EMOTE_DECIMATION_BLADE);
                            Talk(ABILITY_DECIMATION_BLADE);
                            break;
                        }
                        events.ScheduleEvent(EVENT_BLADE, 47 * IN_MILLISECONDS);
                        break;
                    case EVENT_INCENDIARY_SOUL:
                        if (me->getVictim())
                        {
                            DoCast(me->getVictim(), SPELL_BLAZE_OF_GLORY, false);
                            DoCast(SPELL_INCENDIARY_SOUL);
                        }
                        events.ScheduleEvent(EVENT_INCENDIARY_SOUL, 11.5*IN_MILLISECONDS);
                        break;
                    case EVENT_SHARDS_OF_TORMENT:
                        Talk(EMOTE_SHARDS_OF_TORMENT);
                        DoCast(SPELL_SHARDS_OF_TORMENT);
                        events.ScheduleEvent(EVENT_SHARDS_OF_TORMENT, 34 * IN_MILLISECONDS);
                        break;
                    case EVENT_COUNTDOWN:
                        DoCast(SPELL_COUNTDOWN);
                        events.ScheduleEvent(EVENT_COUNTDOWN, 48 * IN_MILLISECONDS);
                        break;
                    case EVENT_BERSERK:
                        DoCast(SPELL_BERSERK);
                        Talk(EMOTE_ENRAGE);
                        Talk(EMOTE_ENRAGE_2);
                        break;
                    case EVENT_UNLOCK_YELLSPAM:
                        _canYellKilledPlayer = true;
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

        void SetGUID(uint64 guid, int32 type = 0) override
        {
            switch (type)
            {
                case GUID_TORMENTED:
                    ++_sharedThePain[GUID_LOPART(guid)];
                    break;
                default:
                    break;
            }
        }

        bool SharedThePain() const
        {
            for (auto const& i : _sharedThePain)
                if (i.second > 3)
                    return false;
            return true;
        }

    private:
        std::unordered_map<uint32 /*guid*/, uint32 /*tormentCount*/> _sharedThePain;
        bool _canYellKilledPlayer;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetFirelandsAI<boss_balerocAI>(creature);
    }
};

class npc_shard_of_torment : public CreatureScript
{
public:
    npc_shard_of_torment() : CreatureScript("npc_shard_of_torment") { }

    struct npc_shard_of_tormentAI : public ScriptedAI
    {
        npc_shard_of_tormentAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
            _instance = creature->GetInstanceScript();
        }

        void IsSummonedBy(Unit* summoner) override
        {
            if (summoner->GetEntry() == BOSS_BALEROC)
            {
                if (_instance->GetBossState(DATA_BALOROC) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
                DoCast(SPELL_TORMENT_COSMETIC_1);
                _events.ScheduleEvent(EVENT_SHARD_SPAWN_EFFECT, 5000);
                me->SetInCombatWithZone();
                _baleroc = summoner->ToCreature();
            }
            else
                me->DespawnOrUnsummon();
        }

        void KilledUnit(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                if (_baleroc)
                    _baleroc->AI()->KilledUnit(who);
        }

        void UpdateAI(uint32 const diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHARD_SPAWN_EFFECT:
                        me->RemoveAurasDueToSpell(SPELL_TORMENT_COSMETIC_1);
                        DoCast(SPELL_TORMENT);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        InstanceScript* _instance;
        EventMap _events;
        Creature* _baleroc;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_shard_of_tormentAI(creature);
    }
};

class npc_magma_orb : public CreatureScript
{
public:
    npc_magma_orb() : CreatureScript("npc_magma_orb") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        if (action == 1)
        {
            if (InstanceScript* instance = creature->GetInstanceScript())
            {
                if (instance->IsDone(DATA_SHANNOX)
                    && instance->IsDone(DATA_LORD_RHYOLITH)
                    && instance->IsDone(DATA_BETHTILAC)
                    && instance->IsDone(DATA_BALOROC)
                    && !instance->IsDone(DATA_MAJORDOMO_STAGHELM))
                {
                    if (Creature* majordomo = Creature::GetCreature(*creature, instance->GetData64(DATA_MAJORDOMO_STAGHELM)))
                    {
                        majordomo->AI()->DoAction(ACTION_START_INTRO2);
                        creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                    }
                }
            }
        }
        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

};



class spell_countdown_p1 : public SpellScriptLoader
{
public:
    spell_countdown_p1() : SpellScriptLoader("spell_countdown_p1") { }

    class spell_countdown_p1_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_countdown_p1_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_UNIT;
        }

        void CastSpellLink()
        {
            if (target1 && target2)
                target1->CastSpell(target2, SPELL_COUNTDOWN_VISUAL, true);
        }

        void HandleOnEffectHitTarget(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_COUNTDOWN_PERIODIC, true);
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            // Remove current tank if we have one
            if (GetCaster()->getVictim())
                targets.remove(GetCaster()->getVictim());

            if (targets.size() < 2)
            {
                FinishCast(SPELL_FAILED_NO_VALID_TARGETS);
                return;
            }

            Trinity::Containers::RandomResizeList(targets, 2);

            std::list<WorldObject*>::const_iterator itr = targets.begin();
            target1 = (*itr)->ToUnit();
            itr++;
            target2 = (*itr)->ToUnit();
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_countdown_p1_SpellScript::CastSpellLink);
            OnEffectHitTarget += SpellEffectFn(spell_countdown_p1_SpellScript::HandleOnEffectHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_countdown_p1_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }

        Unit* target1;
        Unit* target2;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_countdown_p1_SpellScript();
    }
};

class spell_countdown_p2 : public SpellScriptLoader
{
public:
    spell_countdown_p2() : SpellScriptLoader("spell_countdown_p2") { }

    class spell_countdown_p2_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_countdown_p2_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_CANCEL)
                GetTarget()->CastSpell((Unit*)NULL, SPELL_COUNTDOWN_DAMAGE, true);
            GetTarget()->RemoveAurasDueToSpell(SPELL_COUNTDOWN_VISUAL);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_countdown_p2_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_countdown_p2_AuraScript();
    }
};

class spell_countdown_p3 : public SpellScriptLoader
{
public:
    spell_countdown_p3() : SpellScriptLoader("spell_countdown_p3") { }

    class spell_countdown_p3_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_countdown_p3_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_COUNTDOWN_PERIODIC));
            targets.remove(GetCaster());
        }

        void HandleOnEffectHitTarget(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                caster->RemoveAurasDueToSpell(SPELL_COUNTDOWN_PERIODIC);

            if (Unit* target = GetHitUnit())
                target->RemoveAurasDueToSpell(SPELL_COUNTDOWN_PERIODIC);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_countdown_p3_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            OnEffectHitTarget += SpellEffectFn(spell_countdown_p3_SpellScript::HandleOnEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_countdown_p3_SpellScript();
    }
};

class spell_decimating_strike : public SpellScriptLoader
{
public:
    spell_decimating_strike() : SpellScriptLoader("spell_decimating_strike") { }

    class spell_decimating_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_decimating_strike_SpellScript);

        void SetFinalDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                uint32 damage = target->CountPctFromMaxHealth(GetEffectValue());
                int32 minDamage = GetSpellInfo()->Effects[EFFECT_2].CalcValue();
                SetHitDamage(std::max<int32>(minDamage, damage));
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_decimating_strike_SpellScript::SetFinalDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_decimating_strike_SpellScript();
    }
};

class ShardsOfTormentCheck
{
public:
    explicit ShardsOfTormentCheck(Unit* victim) : _victim(victim) { }

    bool operator()(WorldObject* obj)
    {
        Unit* unit = obj->ToUnit();
        return unit && unit != _victim && !unit->HasAura(SPELL_BLAZE_OF_GLORY);
    }

private:
    Unit* _victim;
};

class spell_shards_of_torment : public SpellScriptLoader
{
public:
    spell_shards_of_torment() : SpellScriptLoader("spell_shards_of_torment") { }

    class spell_shards_of_torment_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_shards_of_torment_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_UNIT;
        }

        void HandleOnEffectHitTarget(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            GetCaster()->CastSpell(GetHitUnit(), SPELL_SHARDS_OF_TORMENT_2, true);
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint8 targetCount = GetCaster()->GetMap()->Is25ManRaid() ? 2 : 1;
            if (targets.size() <= targetCount)
                return;

            targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
            std::list<WorldObject*> ranged, melee;
            std::list<WorldObject*>::iterator itr = targets.begin();
            while (itr != targets.end() && (*itr)->GetExactDist2d(GetCaster()) < 18.6f)
            {
                if (ShardsOfTormentCheck(GetCaster()->getVictim())(*itr))
                    melee.push_back(*itr);
                ++itr;
            }

            while (itr != targets.end())
            {
                if (ShardsOfTormentCheck(GetCaster()->getVictim())(*itr))
                    ranged.push_back(*itr);
                ++itr;
            }

            targets.clear();

            if (!melee.empty())
            {
                WorldObject* target = Trinity::Containers::SelectRandomContainerElement(melee);
                melee.remove(target);
                targets.push_back(target);
                --targetCount;
            }

            while (!ranged.empty() && targetCount)
            {
                WorldObject* target = Trinity::Containers::SelectRandomContainerElement(ranged);
                ranged.remove(target);
                targets.push_back(target);
                --targetCount;
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_shards_of_torment_SpellScript::HandleOnEffectHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_shards_of_torment_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_shards_of_torment_SpellScript();
    }
};

class spell_baleroc_torment : public SpellScriptLoader
{
public:
    spell_baleroc_torment() : SpellScriptLoader("spell_baleroc_torment") { }

    class spell_baleroc_torment_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_baleroc_torment_SpellScript);

        bool Load() override
        {
            _castExplosion = false;
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
            if (Unit* caster = GetCaster())
                targets.remove_if([caster](WorldObject* target) { return target->GetDistance2d(caster) > 15.0f; });
            if (!targets.empty())
                targets.resize(1);
            else
                _castExplosion = true;
        }

        void CheckExplosion(SpellEffIndex /*effIndex*/)
        {
            if (_castExplosion)
                if (Unit* caster = GetCaster())
                    caster->CastSpell((Unit*)NULL, SPELL_WAVE_OF_TORMENT, true);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
            {
                uint64 channelGuid = caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT);
                if (Unit* channelTarget = ObjectAccessor::GetUnit(*caster, channelGuid))
                    if (channelTarget == target)
                        return;
                caster->CastSpell(target, SPELL_TORMENT_PERIODIC, true);
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_baleroc_torment_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHit += SpellEffectFn(spell_baleroc_torment_SpellScript::CheckExplosion, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnEffectHitTarget += SpellEffectFn(spell_baleroc_torment_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }

    private:
        bool _castExplosion;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_baleroc_torment_SpellScript();
    }
};

class spell_baleroc_tormented : public SpellScriptLoader
{
public:
    spell_baleroc_tormented() : SpellScriptLoader("spell_baleroc_tormented") { }

    class spell_baleroc_tormented_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_baleroc_tormented_SpellScript);

        void HandleEffect(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                if (Aura* aura = target->GetAura(GetSpellInfo()->Id))
                    SetHitDamage(GetHitDamage() * aura->GetStackAmount());
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_baleroc_tormented_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_baleroc_tormented_SpellScript();
    }

    class spell_baleroc_tormented_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_baleroc_tormented_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
                GetTarget()->CastSpell(GetTarget(), SPELL_TORMENTED, true);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetSpellInfo() && (eventInfo.GetSpellInfo()->IsAffectingArea() || eventInfo.GetSpellInfo()->Id == 53652))
                return false;

            return (eventInfo.GetTypeMask() & PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS) && !(eventInfo.GetTypeMask() & PROC_FLAG_TAKEN_PERIODIC);
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            if (Unit* caster = eventInfo.GetActor())
            {
                if (!caster->HasAura(SPELL_VITAL_FLAME))
                {
                    uint32 stacks = round(aurEff->GetBase()->GetStackAmount() / uint32((caster->GetMap()->Is25ManRaid() && caster->GetMap()->IsHeroic()) ? 5 : 3));
                    if (stacks != 0)
                        caster->CastCustomSpell(SPELL_VITAL_SPARK, SPELLVALUE_AURA_STACK, stacks, caster, true);
                }
            }
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_baleroc_tormented_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            DoCheckProc += AuraCheckProcFn(spell_baleroc_tormented_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_baleroc_tormented_AuraScript::OnProc, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_baleroc_tormented_AuraScript();
    }
};

class spell_baleroc_tormented_debuff : public SpellScriptLoader
{
public:
    spell_baleroc_tormented_debuff() : SpellScriptLoader("spell_baleroc_tormented_debuff") { }

    class spell_baleroc_tormented_debuff_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_baleroc_tormented_debuff_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (InstanceScript* instance = GetTarget()->GetInstanceScript())
                if (Creature* baleroc = ObjectAccessor::GetCreature(*GetTarget(), instance->GetData64(DATA_BALOROC)))
                    baleroc->AI()->SetGUID(GetTarget()->GetGUID(), GUID_TORMENTED);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_baleroc_tormented_debuff_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_baleroc_tormented_debuff_AuraScript();
    }
};

class spell_baleroc_tormented_heroic : public SpellScriptLoader
{
public:
    spell_baleroc_tormented_heroic() : SpellScriptLoader("spell_baleroc_tormented_heroic") { }

    class spell_baleroc_tormented_heroic_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_baleroc_tormented_heroic_SpellScript);

        bool Load() override
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void HandleOnEffectHitTarget(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (GetCaster()->GetMap()->IsHeroic())
                GetHitUnit()->CastSpell(GetHitUnit(), SPELL_TORMENTED, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_baleroc_tormented_heroic_SpellScript::HandleOnEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_baleroc_tormented_heroic_SpellScript();
    }
};

class spell_vital_spark : public SpellScriptLoader
{
public:
    spell_vital_spark() : SpellScriptLoader("spell_vital_spark") { }

    class spell_vital_spark_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_vital_spark_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (!eventInfo.GetSpellInfo() || eventInfo.GetSpellInfo()->IsAffectingArea() || eventInfo.GetSpellInfo()->Id == 53652)
                return false;

            switch (eventInfo.GetSpellInfo()->Id)
            {
                //those heals are not intentional, so they will be excluded from the proc
                case 48210: //warlock's haunt heal
                case 63106: //warlock's siphon life heal
                case 23880: //warrior's Bloodthirst heal
                case 87023: //mage's cauterize heal
                case 20167: //paladin's seal of insight heal
                case 45470: //death knight's death strike heal
                case 2944:  //priest's devouring plague heal
                case 15290: //priest's vampiric embrace heal
                case 139: //priest's renew heal
                case 96966: //eye of blazing power proc
                case 52752: //shaman's ancestral awakening heal
                case 52042: //shaman's healing stream totem heal
                case 774: //druid rejuvenation heal
                    return false;
                default:
                    break;
            }

            return !(eventInfo.GetTypeMask() & PROC_FLAG_DONE_PERIODIC);
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            if (Unit* caster = GetTarget())
            {
                if (Unit* target = eventInfo.GetActionTarget())
                {
                    if (!caster->HasAura(SPELL_VITAL_FLAME) && target->HasAura(SPELL_BLAZE_OF_GLORY))
                    {
                        caster->CastCustomSpell(SPELL_VITAL_FLAME, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), caster, true);
                        Remove(AURA_REMOVE_BY_DEFAULT);
                    }
                }
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_vital_spark_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_vital_spark_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_vital_spark_AuraScript();
    }
};

class spell_vital_flame : public SpellScriptLoader
{
public:
    spell_vital_flame() : SpellScriptLoader("spell_vital_flame") { }

    class spell_vital_flame_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_vital_flame_AuraScript);

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            GetTarget()->CastCustomSpell(SPELL_VITAL_SPARK, SPELLVALUE_AURA_STACK, aurEff->GetAmount() / 5, GetTarget(), true);
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectApplyFn(spell_vital_flame_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_HEALING_DONE_PERCENT_WITH_SPECIFIC_AURA, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_vital_flame_AuraScript();
    }
};

class spell_baleroc_blades : public SpellScriptLoader
{
public:
    spell_baleroc_blades() : SpellScriptLoader("spell_baleroc_blades") { }

    class spell_baleroc_blades_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_baleroc_blades_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                return;

            if (Creature* baleroc = GetTarget()->ToCreature())
                baleroc->AI()->DoAction(ACTION_RESTORE_WEAPONS);
        }

        void Register() override
        {
            if (m_scriptSpellId == 99352 || m_scriptSpellId == 99405 || m_scriptSpellId == 99352 || m_scriptSpellId == 99405)
                AfterEffectRemove += AuraEffectRemoveFn(spell_baleroc_blades_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PROC_TRIGGER_SPELL_OVERRIDE, AURA_EFFECT_HANDLE_REAL);
            else
                AfterEffectRemove += AuraEffectRemoveFn(spell_baleroc_blades_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL_OVERRIDE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_baleroc_blades_AuraScript();
    }
};

typedef boss_baleroc::boss_balerocAI BalerocAI;

class achievement_share_the_pain : public AchievementCriteriaScript
{
public:
    achievement_share_the_pain() : AchievementCriteriaScript("achievement_share_the_pain") { }

    bool OnCheck(Player* /*source*/, Unit* target) override
    {
        if (!target)
            return false;

        if (BalerocAI* balerocAI = CAST_AI(BalerocAI, target->GetAI()))
            return balerocAI->SharedThePain();

        return false;
    }
};

void AddSC_boss_baleroc()
{
    new boss_baleroc();

    new npc_shard_of_torment();
    new npc_magma_orb();

    new spell_countdown_p1();
    new spell_countdown_p2();
    new spell_countdown_p3();

    new spell_decimating_strike();

    new spell_shards_of_torment();
    new spell_baleroc_torment();
    new spell_baleroc_tormented();
    new spell_baleroc_tormented_heroic();
    new spell_baleroc_tormented_debuff();
    new spell_vital_spark();
    new spell_vital_flame();
    new spell_baleroc_blades();
    new achievement_share_the_pain();
};
