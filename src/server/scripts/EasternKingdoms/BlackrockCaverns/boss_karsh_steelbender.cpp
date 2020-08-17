#include "ScriptPCH.h"
#include "blackrock_caverns.h"

enum ScriptTexts
{
    SAY_AGGRO     = 0,
    SAY_KILL      = 1,
    SAY_BURN      = 2,
    SAY_DEATH     = 3,
};

enum Spells
{
    SPELL_CLEAVE                                = 15284,
    SPELL_QUICKSILVER_ARMOR                     = 75842,
    SPELL_HEAT_WAVE                             = 75851,
    SPELL_SUPERHEATED_QUICKSILVER_ARMOR         = 75846,
    SPELL_SUPERHEATED_QUICKSILVER_ARMOR_HC      = 93567,
    SPELL_SUPERHEATED_QUICKSILVER_ARMOR_0       = 76015,
    SPELL_LAVA_SPOUT                            = 76007,
    SPELL_LAVA_SPOUT_HC                         = 93565,
    SPELL_BURNING_METAL                         = 76002,
    SPELL_LAVA_POOL                             = 90391,
    SPELL_BURNING_HEAT                          = 75077,
    SPELL_COOLED                                = 82287,

    SPELL_BURNING                               = 77490,
};

enum Events
{
     EVENT_CLEAVE          = 1,
     EVENT_HEAT_ARMOR      = 2,
};

class boss_karsh_steelbender : public CreatureScript
{
    public:
        boss_karsh_steelbender() : CreatureScript("boss_karsh_steelbender") { }

        struct boss_karsh_steelbenderAI : public BossAI
        {
            boss_karsh_steelbenderAI(Creature* creature) : BossAI(creature, DATA_KARSH)
            {
               me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
               me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
               me->setActive(true);
            }

            void Reset() override
            {
                _Reset();
                DoCast(me, SPELL_QUICKSILVER_ARMOR);
                bHeat = false;

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                std::list<Creature*> qicksilverList;
                GetCreatureListWithEntryInGrid(qicksilverList, me, NPC_QUICKSILVER, 200.0f);

                for (auto&& itr : qicksilverList)
                    itr->AI()->Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 7000));
                events.ScheduleEvent(EVENT_HEAT_ARMOR, 1000);
                Talk(SAY_AGGRO);
                DoZoneInCombat();
            }

            void DamageDealt(Unit* victim, uint32& /*damage*/, DamageEffectType damageType) override
            {
                if (!victim || damageType != DIRECT_DAMAGE)
                    return;

                if (!me->HasAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR) && !me->HasAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR_HC))
                    return;

                DoCast(victim, SPELL_BURNING_METAL, true);
            }

            void DoAction(int32 actionId) override
            {
                if (actionId == ACTION_SUPERHEATED_ARMOR_REMOVE)
                    HandleOverheatForge();
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);

                if (summon->GetEntry() == NPC_BOUND_FLAMES)
                    summon->SetInCombatWithZone();
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();
                Talk(SAY_DEATH);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
     
            void KilledUnit(Unit* /*victim*/) override
            {
                Talk(SAY_KILL);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->GetDistance(me->GetHomePosition()) > 60.0f)
                {
                    EnterEvadeMode();
                    return;
                }
     
                bHeat = (me->GetDistance(centerkarshPos) <= 6.0f) ? true : false;

                if (!me->HasAura(SPELL_QUICKSILVER_ARMOR) && !me->HasAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR) && !me->HasAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR_HC))
                    DoCast(me, SPELL_QUICKSILVER_ARMOR);
                if (me->HasAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR) || me->HasAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR_HC))
                    me->RemoveAurasDueToSpell(SPELL_QUICKSILVER_ARMOR);

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CLEAVE:
                            DoCast(me->getVictim(), SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 7000));
                            break;
                        case EVENT_HEAT_ARMOR:
                            if (bHeat)
                            {
                                DoCast(me, SPELL_HEAT_WAVE);
                                DoCast(me, IsHeroic() ? SPELL_SUPERHEATED_QUICKSILVER_ARMOR_HC : SPELL_SUPERHEATED_QUICKSILVER_ARMOR);
                            }
                            events.ScheduleEvent(EVENT_HEAT_ARMOR, 1000);
                            break;
                    }
                    DoMeleeAttackIfReady();
                }
            }

            private:
                bool bHeat;      

                void HandleOverheatForge()
                {
                    float m_ori = 0.0f;
                    float x, y;
                    std::list<uint32> boundFlamePos = { 1, 3, 5, 7, 9, 11, 13 };
                    Trinity::Containers::RandomResizeList(boundFlamePos, 3);

                    for (uint8 i = 0; i < 15; i++)
                    {
                        x = centerkarshPos.GetPositionX() + (32.5f * cos(Position::NormalizeOrientation(m_ori)));
                        y = centerkarshPos.GetPositionY() + (32.5f * sin(Position::NormalizeOrientation(m_ori)));

                        m_ori += M_PI / 6;
                        
                        if (Creature* lavaSpout = me->SummonCreature(NPC_LAVA_SPOUT_TRIGGER, x, y, me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 2000))
                            lavaSpout->CastSpell(lavaSpout, IsHeroic() ? SPELL_LAVA_SPOUT_HC : SPELL_LAVA_SPOUT);

                        if (IsHeroic() && std::find(boundFlamePos.begin(), boundFlamePos.end(), i) != boundFlamePos.end())
                            me->SummonCreature(NPC_BOUND_FLAMES, x, y, me->GetPositionZ(), TEMPSUMMON_MANUAL_DESPAWN);
                    }
                }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_karsh_steelbenderAI>(creature);
        }
};

// Quicksilver 40004
struct npc_karsh_quicksilver : public SmartCreatureAI
{
    npc_karsh_quicksilver(Creature* creature) : SmartCreatureAI(creature) { }

    void Reset() override
    {
        if (!me->isAlive())
            me->Respawn();

        DoCast(me, SPELL_COOLED, true);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    }

    void DoAction(int32 actionId) override
    {
        if (actionId == ACTION_QUICKSILVER)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            me->RemoveAurasDueToSpell(SPELL_COOLED);
            me->SetInCombatWithZone();
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Bound Flames 50417
struct npc_karsh_bound_flames : public ScriptedAI
{
    npc_karsh_bound_flames(Creature* creature) : ScriptedAI(creature) { }

    bool hasTriggered;

    void Reset() override
    {
        hasTriggered = false;
    }

    void DamageTaken(Unit* attacker, uint32& damage) override
    {
        if (hasTriggered)
            damage = 0;

        if (damage >= me->GetHealth() && !hasTriggered)
        {
            damage = 0;
            hasTriggered = 0;

            me->PrepareChanneledCast(me->GetOrientation());
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            me->SetStandState(UNIT_STAND_STATE_DEAD);

            DoCast(me, SPELL_LAVA_POOL, true);
        }
    }
};

// Heat Wave 75851, 76002
class spell_karsh_heat_wave : public SpellScript
{
    PrepareSpellScript(spell_karsh_heat_wave);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        float mod = (float)(GetCaster()->GetAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR) ? GetCaster()->GetAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR)->GetStackAmount() : GetCaster()->GetAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR_HC) ? GetCaster()->GetAura(SPELL_SUPERHEATED_QUICKSILVER_ARMOR_HC)->GetStackAmount() : 0);
        int32 recalculatedDamage = GetHitDamage() + (int32)((GetCaster()->GetMap()->IsHeroic() ? 2000 : 1000) * mod);
        SetHitDamage(recalculatedDamage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_karsh_heat_wave::HandleEffectHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Superheated Quicksilver Armor 75846, 93567
class spell_superheated_quicksilver_armor : public AuraScript
{
    PrepareAuraScript(spell_superheated_quicksilver_armor);

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Creature* owner = GetOwner()->ToCreature())
            owner->AI()->DoAction(ACTION_SUPERHEATED_ARMOR_REMOVE);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_superheated_quicksilver_armor::OnAuraEffectRemove, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
    }
};

// Burning Heat 82301
class spell_karsh_burning_heat : public SpellScript
{
    PrepareSpellScript(spell_karsh_burning_heat);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Creature* target = GetHitCreature())
            target->AI()->DoAction(ACTION_QUICKSILVER);
    }

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_QUICKSILVER; });

        if (targets.size() > 5)
            Trinity::Containers::RandomResizeList(targets, 5);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_karsh_burning_heat::HandleEffectHit, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_karsh_burning_heat::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

// Burning Heat 82301
class spell_karsh_burning_heat_aura : public AuraScript
{
    PrepareAuraScript(spell_karsh_burning_heat_aura);

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Creature* owner = GetOwner()->ToCreature())
        {
            owner->AI()->Reset();
            owner->AI()->EnterEvadeMode();
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_karsh_burning_heat_aura::OnAuraEffectRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
    }
};
 
void AddSC_boss_karsh_steelbender()
{
    new boss_karsh_steelbender();
    new creature_script<npc_karsh_bound_flames>("npc_karsh_bound_flames");
    new creature_script<npc_karsh_quicksilver>("npc_karsh_quicksilver");
    new spell_script<spell_karsh_heat_wave>("spell_karsh_heat_wave");
    new aura_script<spell_superheated_quicksilver_armor>("spell_superheated_quicksilver_armor");
    new spell_script<spell_karsh_burning_heat>("spell_karsh_burning_heat");
    new aura_script<spell_karsh_burning_heat_aura>("spell_karsh_burning_heat_aura");
}
