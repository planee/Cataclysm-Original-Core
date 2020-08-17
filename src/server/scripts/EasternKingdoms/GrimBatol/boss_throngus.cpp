#include "grimbatol.h"
#include "ScriptPCH.h"

enum ScriptTexts
{
    SAY_AGGRO,
    SAY_KILL,
    SAY_DEATH,
    SAY_SHIELD,
    SAY_DUAL,
    SAY_MACE,
};

enum Equipment
{
    EQUIPMENT_ID_SHIELD    = 40400, 
    EQUIPMENT_ID_SWORD     = 65094, 
    EQUIPMENT_ID_MACE      = 65090,  
};

enum Spells
{
    SPELL_MIGHTY_STOMP            = 74984,
    SPELL_PICK_WEAPON             = 75000,
    SPELL_DUAL_BLADES             = 74981,
    SPELL_ENCUMBERED              = 75007,
    SPELL_IMPALING_SLAM_0         = 75057,
    SPELL_IMPALING_SLAM           = 75056,
    SPELL_DISORIENTING_ROAR       = 74976,
    SPELL_BURNING_SHIELD          = 90819,
    SPELL_PERSONAL_PHALANX        = 74908,
    SPELL_FLAMING_ARROW           = 45101,
    SPELL_FLAMING_ARROW_VISUAL    = 74944, 
    SPELL_CAVE_IN_DUMMY           = 74987,
    SPELL_CAVE_IN                 = 74990,
    SPELL_CAVE_IN_DMG             = 74986,
    SPELL_CAVE_IN_EFF             = 74985,
    SPELL_FIRE_PATCH_AURA         = 90752,
    SPELL_LAVA_PATCH_DMG          = 90754,
    SPELL_FLAMING_ARROW_HC        = 90810,
    SPELL_FLAMING_ARROW_1         = 74944,
    
};

enum Adds
{
    NPC_CAVE_IN_STALKER        = 40228,
    NPC_TWILIGHT_ARCHER        = 40197,
    NPC_FIRE_PATCH             = 48711,
};

enum Events
{
    EVENT_PICK_WEAPON = 1,
    EVENT_DISORIENTING_ROAR,
    EVENT_IMPALING_SLAM,
    EVENT_FLAMING_ARROW,
    EVENT_MIGHTLY_STOMP,
    EVENT_FIRE_ARROW,
};

enum Others
{
    ACTION_MACE = 1,
    ACTION_DUAL_BLADES,
    ACTION_SHIELD,
    ACTION_INIT_ARCHERS,
    ACTION_RESET_ARCHERS,
};

class boss_forgemaster_throngus : public CreatureScript
{
    public:
        boss_forgemaster_throngus() : CreatureScript("boss_forgemaster_throngus") { }

        struct boss_forgemaster_throngusAI : public BossAI
        {
            boss_forgemaster_throngusAI(Creature* creature) : BossAI(creature, DATA_FORGEMASTER_THRONGUS)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->setActive(true);
            }

            uint32 prevWeaponType;
            uint64 targetGUID;

            void Reset() override
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                SetEquipmentSlots(false, 0, 0, 0);
                prevWeaponType = 0;
                targetGUID     = 0;
                me->SetReactState(REACT_AGGRESSIVE);
                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_PICK_WEAPON, 10000);
                events.ScheduleEvent(EVENT_MIGHTLY_STOMP, 15 * IN_MILLISECONDS);

                // Summon Archers
                for (auto&& itr : twilightarcherPos)
                    me->SummonCreature(NPC_TWILIGHT_ARCHER, itr, TEMPSUMMON_MANUAL_DESPAWN);
            }

            uint32 GetData(uint32 type) const override
            {
                if (type == TYPE_WEAPON_DATA)
                    return prevWeaponType;

                return 0;
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);

                if (summon->GetEntry() == NPC_TWILIGHT_ARCHER)
                {
                    summon->SetDisplayId(11686);
                    summon->SetInCombatWithZone();
                }
            }
            
            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();
                summons.DespawnAll();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Talk(SAY_DEATH);
            }

            void KilledUnit(Unit* /*victim*/) override
            {
                Talk(SAY_KILL);
            }

            void UpdateAI(uint32 diff) override
            {
                scheduler.Update(diff);

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PICK_WEAPON:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_NONE);
                            scheduler.CancelAll();
                            events.DelayEvents(3000);
                            DoCast(me, SPELL_PICK_WEAPON);
                            events.ScheduleEvent(EVENT_PICK_WEAPON, 32500);
                            break;
                        case EVENT_IMPALING_SLAM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankSpecTargetSelector()))
                            {
                                DoCast(target, SPELL_IMPALING_SLAM);
                                DoCast(target, SPELL_IMPALING_SLAM_0);
                            }
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                            {
                                DoCast(target, SPELL_IMPALING_SLAM);
                                DoCast(target, SPELL_IMPALING_SLAM_0);
                            }

                            events.ScheduleEvent(EVENT_IMPALING_SLAM, 12000);
                            break;
                        case EVENT_DISORIENTING_ROAR:
                            DoCast(me, SPELL_DISORIENTING_ROAR);
                            events.ScheduleEvent(EVENT_DISORIENTING_ROAR, 12000);
                            break;
                        case EVENT_MIGHTLY_STOMP:
                            if (prevWeaponType == 3) // Not used in shield phase
                            {
                                events.RescheduleEvent(EVENT_MIGHTLY_STOMP, urand(5 * IN_MILLISECONDS, 6 * IN_MILLISECONDS));
                                break;
                            }
                            DoCast(me, SPELL_MIGHTY_STOMP);
                            events.ScheduleEvent(EVENT_MIGHTLY_STOMP, 20 * IN_MILLISECONDS);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            void DoAction(int32 action) override
            {
                events.CancelEvent(EVENT_DISORIENTING_ROAR);
                events.CancelEvent(EVENT_IMPALING_SLAM);

                switch (action)
                {
                    case ACTION_MACE:
                        HandleInitArchers(ACTION_RESET_ARCHERS);
                        me->RemoveChanneledCast(targetGUID);
                        prevWeaponType = 2;
                        DoCast(me, SPELL_ENCUMBERED, true);
                        Talk(SAY_MACE);
                        SetEquipmentSlots(false, EQUIPMENT_ID_MACE, 0, 0);
                        events.ScheduleEvent(EVENT_IMPALING_SLAM, 7000);
                        break;
                    case ACTION_DUAL_BLADES:
                        HandleInitArchers(ACTION_RESET_ARCHERS);
                        me->RemoveChanneledCast(targetGUID);
                        prevWeaponType = 1;
                        DoCast(me, SPELL_DUAL_BLADES, true);
                        Talk(SAY_DUAL);
                        SetEquipmentSlots(false, EQUIPMENT_ID_SWORD, EQUIPMENT_ID_SWORD, 0);
                        events.ScheduleEvent(EVENT_DISORIENTING_ROAR, 7000);
                        break;
                    case ACTION_SHIELD:
                        prevWeaponType = 3;
                        HandleInitArchers(ACTION_INIT_ARCHERS);
                        DoCast(me, SPELL_PERSONAL_PHALANX, true);

                        if (IsHeroic())
                            DoCast(me, SPELL_BURNING_SHIELD);

                        Talk(SAY_SHIELD);
                        SetEquipmentSlots(false, 0, EQUIPMENT_ID_SHIELD, 0);
                        
                        if (Unit* target = me->getVictim())
                            targetGUID = target->GetGUID();

                        me->PrepareChanneledCast(me->GetOrientation());

                        scheduler
                            .Schedule(Milliseconds(1000), [this](TaskContext context)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankSpecTargetSelector()))
                                me->SetFacingTo(me->GetAngle(target));
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                                me->SetFacingTo(me->GetAngle(target));

                            me->HandleEmoteState(EMOTE_ONESHOT_CUSTOM_SPELL_04);

                            context.Repeat(Seconds(10));
                        });
                        break;
                }
            }

            private:
                void HandleInitArchers(int32 action)
                {
                    std::list<Creature*> archersList;
                    GetCreatureListWithEntryInGrid(archersList, me, NPC_TWILIGHT_ARCHER, 200.0f);

                    if (action == ACTION_INIT_ARCHERS && archersList.size() > 5)
                        Trinity::Containers::RandomResizeList(archersList, 5);

                    for (auto&& itr : archersList)
                        itr->AI()->DoAction(action);
                }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_forgemaster_throngusAI>(creature);
        }
};



// Fire Patch 48711
struct npc_throngus_fire_patch : public ScriptedAI
{
    npc_throngus_fire_patch(Creature* creature) : ScriptedAI(creature)
    {
        SetCombatMovement(false);
    }

    void Reset() override
    {
        DoCast(me, SPELL_FIRE_PATCH_AURA);
    }

    void UpdateAI(uint32 /*diff*/) override { }
};

// Twilight Archer 40197
struct npc_throngus_twilight_archer : public SmartCreatureAI
{
    npc_throngus_twilight_archer(Creature* creature) : SmartCreatureAI(creature) 
    {
        SetCombatMovement(false);
    }

    void Reset() override
    {
        events.Reset();
    }

    void DoAction(int32 actionId) override
    {
        switch (actionId)
        {
            case ACTION_INIT_ARCHERS:
                events.ScheduleEvent(EVENT_FIRE_ARROW, urand(1 * IN_MILLISECONDS, 1.5 * IN_MILLISECONDS));
                break;
            case ACTION_RESET_ARCHERS:
                events.Reset();
                break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_FLAMING_ARROW_1, urand(1500, 2500), EVENT_FIRE_ARROW, eventId);
            break;
        }
    }
};

// Cave In Stalker 40228
struct npc_throngus_cave_in_stalker : public ScriptedAI
{
    npc_throngus_cave_in_stalker(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        DoCast(me, SPELL_CAVE_IN_DUMMY, true);
        DoCast(me, SPELL_CAVE_IN, true);

        if (Creature* throngus = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(DATA_FORGEMASTER_THRONGUS) : 0))
            throngus->AI()->JustSummoned(me);
    }

    void UpdateAI(uint32 /*diff*/) override { }
};

std::map<uint32, uint32> invPickWeaponsType=
{
    { 1, ACTION_DUAL_BLADES },
    { 2,        ACTION_MACE },
    { 3,      ACTION_SHIELD },
};

// Pick Weapon 75000
class spell_throngus_pick_weapon : public SpellScript
{
    PrepareSpellScript(spell_throngus_pick_weapon);

    void HandleScriptEffect(SpellEffIndex effIndex)
    {
        if (Creature* caster = GetCaster()->ToCreature())
        {
            std::vector<uint32> weaponComboSpellStorage = { 1, 2, 3 };

            // Remove From Temp container value, that was used before (real random)
            if (caster->AI()->GetData(TYPE_WEAPON_DATA))
                weaponComboSpellStorage.erase(std::find(weaponComboSpellStorage.begin(), weaponComboSpellStorage.end(), caster->AI()->GetData(TYPE_WEAPON_DATA)));

            // Select new spell from updated container
            caster->AI()->DoAction(invPickWeaponsType.find(Trinity::Containers::SelectRandomContainerElement(weaponComboSpellStorage))->second);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_throngus_pick_weapon::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Mightly Stomp 74984
class spell_throngus_mightly_stomp : public SpellScript
{
    PrepareSpellScript(spell_throngus_mightly_stomp);

    std::list<WorldObject*> m_targets;

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        m_targets = targets;

        // Remove whole exclude casters
        targets.remove_if(TankSpecTargetSelector());
        targets.remove_if(MeeleSpecTargetSelector());

        // if we have enough players without tanks and melee
        if (targets.size() >= 1)
        {
            if (targets.size() > 1)
                Trinity::Containers::RandomResizeList(targets, 1);

            return;
        }

        if (!m_targets.empty())
        {
            targets.clear();

            for (auto&& itr : m_targets)
                targets.push_back(itr);

            if (targets.size() > 1)
                Trinity::Containers::RandomResizeList(targets, 1);
        }
    }

    void HandleScriptEffect(SpellEffIndex effIndex)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, SPELL_CAVE_IN_EFF, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_throngus_mightly_stomp::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_throngus_mightly_stomp::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
    }
};

// Disorienting Roar 74976, 90737
class spell_throngus_disorienting_roar : public AuraScript
{
    PrepareAuraScript(spell_throngus_disorienting_roar);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Unit* owner = GetOwner()->ToUnit())
        {
            if (GetStackAmount() > 1)
            {
                SetStackAmount(GetStackAmount() - 1);
                aurEff->GetBase()->RecalculateAmountOfEffects();
            }
        }
    }

    void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (GetId() == 90737)
        {
            SetStackAmount(3);
            aurEff->GetBase()->RecalculateAmountOfEffects();
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_throngus_disorienting_roar::OnProc, EFFECT_0, SPELL_AURA_MOD_SPEED_SLOW_ALL);
        AfterEffectApply += AuraEffectApplyFn(spell_throngus_disorienting_roar::AfterApply, EFFECT_0, SPELL_AURA_MOD_SPEED_SLOW_ALL, AURA_EFFECT_HANDLE_REAL);
    }
};

// Flaming Arrow 74944, 90810
class spell_throngus_flaming_arrow : public SpellScript
{
    PrepareSpellScript(spell_throngus_flaming_arrow);

    std::list<WorldObject*> m_targets;

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        uint8 targetsCount = GetCaster()->GetMap()->IsHeroic() ? 2 : 1;

        if (targets.size() > targetsCount)
            Trinity::Containers::RandomResizeList(targets, targetsCount);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_throngus_flaming_arrow::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

void AddSC_boss_throngus()
{
    new boss_forgemaster_throngus();
    new creature_script<npc_throngus_fire_patch>("npc_throngus_fire_patch");
    new creature_script<npc_throngus_twilight_archer>("npc_throngus_twilight_archer");
    new creature_script<npc_throngus_cave_in_stalker>("npc_throngus_cave_in_stalker");
    new spell_script<spell_throngus_pick_weapon>("spell_throngus_pick_weapon");
    new spell_script<spell_throngus_mightly_stomp>("spell_throngus_mightly_stomp");
    new aura_script<spell_throngus_disorienting_roar>("spell_throngus_disorienting_roar");
    new spell_script<spell_throngus_flaming_arrow>("spell_throngus_flaming_arrow");
}
