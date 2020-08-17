#include "ScriptPCH.h"
#include "blackrock_caverns.h"

enum ScriptTexts
{
    SAY_AGGRO    = 0,
    SAY_KILL     = 1,
    SAY_CHAINS   = 2,
    SAY_DEATH    = 3,
};

enum Spells
{
    SPELL_CALL_FOR_HELP                         = 82137,
    SPELL_CHAINS_OF_WOE                         = 75539,
    SPELL_WOUNDING_STRIKE                       = 75571,
    SPELL_THE_SKULLCRACKER                      = 75543,
    SPELL_QUAKE                                 = 75272,
    SPELL_SKULLCRACKER                          = 95326,
    SPELL_CHAINS_OF_WOE_0                       = 75441, // cast by chain
    SPELL_CHAINS_OF_WOE_1                       = 82189,
    SPELL_CHAINS_OF_WOE_TELE                    = 75437,
    SPELL_CHAINS_OF_WOE_TELE_0                  = 75464,
    SPELL_CHAINS_OF_WOE_ROOT                    = 82192,
};
 
enum Adds
{
    NPC_QUAKE                                   = 40401,
    NPC_CHAINS_OF_WOE                           = 40447,
    NPC_ANGERED_EARTH                           = 50376,
}; 

enum Events
{
    EVENT_WOUNDING_STRIKE                       = 1,
    EVENT_QUAKE                                 = 2,
    EVENT_CHAINS_OF_WOE                         = 3,
    EVENT_SKULLCRACKER                          = 4,
};

enum Actions
{
    ACTION_REMOVE_SKULLCRACKER = 0,
};
 
class boss_romogg_bonecrusher : public CreatureScript
{
    public:
        boss_romogg_bonecrusher() : CreatureScript("boss_romogg_bonecrusher") { }
     
        struct boss_romogg_bonecrusherAI : public BossAI
        {
            boss_romogg_bonecrusherAI(Creature* creature) : BossAI(creature, DATA_ROMOGG)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);   
                me->setActive(true);
            }

            void Reset() override
            {
                _Reset();
                stage      = 0;
                HealthPct  = 66.0f;
                targetGUID = 0;
                summons.DespawnAll();
                me->SetReactState(REACT_AGGRESSIVE);
                me->ClearUnitState(UNIT_STATE_ROOT); // temp hackfix

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAINS_OF_WOE_ROOT);
                }
            }
     
            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);
                switch (summon->GetEntry())
                {
                    case NPC_ANGERED_EARTH:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        {
                            summon->AddThreat(target, 10.0f);
                            summon->Attack(target, true);
                            summon->GetMotionMaster()->MoveChase(target);
                        }
                        break;
                    case NPC_CHAINS_OF_WOE:
                        summon->SetDisplayId(38330);
                        DoZoneInCombat(summon, 200.0f);
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        summon->CastSpell(summon, SPELL_CHAINS_OF_WOE_0, true);
                        break;
                }        
            }

            void DoAction(int32 actionId) override
            {
                if (actionId == ACTION_REMOVE_SKULLCRACKER)
                {
                    me->ClearUnitState(UNIT_STATE_ROOT); // temp hackfix
                    me->RemoveChanneledCast(targetGUID);
                    events.ScheduleEvent(EVENT_WOUNDING_STRIKE, urand(5000, 7000));
                    events.ScheduleEvent(EVENT_QUAKE, urand(12000, 20000));
                }
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();
                DoCast(me, SPELL_CALL_FOR_HELP);
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_WOUNDING_STRIKE, urand(5000, 7000));
                events.ScheduleEvent(EVENT_QUAKE, urand(18000, 20000));
                DoZoneInCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();
                Talk(SAY_DEATH);
                summons.DespawnAll();

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAINS_OF_WOE_ROOT);
                }
            }
     
            void KilledUnit(Unit* /*victim*/) override
            {
                Talk(SAY_KILL);
            }

            void EnterEvadeMode() override
            {
                me->GetMotionMaster()->Clear();
                summons.DespawnAll();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                _EnterEvadeMode();
                Reset();

                me->ClearUnitState(UNIT_STATE_ROOT); // temp hackfix
            }

            void DamageTaken(Unit* attacker, uint32& damage) override
            {
                if (HealthPct < 33.0f)
                    return;

                if (HealthBelowPct(HealthPct) && !me->HasUnitState(UNIT_STATE_CASTING))
                {
                    HealthPct -= 33.0f;
                    Talk(SAY_CHAINS);
                    events.Reset();

                    if (Unit* vict = me->getVictim())
                        targetGUID = vict->GetGUID();

                    me->AddUnitState(UNIT_STATE_ROOT); // temp hackfix
                    me->PrepareChanneledCast(frand(0.0f, 2 * M_PI), SPELL_CHAINS_OF_WOE);
                }
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

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WOUNDING_STRIKE:
                            DoCast(me->getVictim(), SPELL_WOUNDING_STRIKE);
                            events.ScheduleEvent(EVENT_WOUNDING_STRIKE, urand(6000, 7000));
                            break;
                        case EVENT_QUAKE:
                            DoCast(me, SPELL_QUAKE);
                            if (IsHeroic())
                            {
                                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                                if (me->GetMap()->IsDungeon() && !players.isEmpty())
                                    for (auto&& ref : players)
                                        if (Player* player = ref.getSource())
                                            me->SummonCreature(NPC_ANGERED_EARTH, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
                            }
                            events.ScheduleEvent(EVENT_QUAKE, urand(18000, 20000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();  
            }

            private:
                uint8 stage;
                uint64 targetGUID;
                float HealthPct;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_romogg_bonecrusherAI>(creature);
        }
};

// Chains of Woe 75539
class spell_chains_of_woe : public SpellScript
{
    PrepareSpellScript(spell_chains_of_woe);

    void HandleAfterCast()
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(caster, SPELL_THE_SKULLCRACKER, false);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_chains_of_woe::HandleAfterCast);
    }
};

// Chains of Woe Effect 82189
class spell_chains_of_woe_eff : public SpellScript
{
    PrepareSpellScript(spell_chains_of_woe_eff);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                if (!target->HasAura(SPELL_CHAINS_OF_WOE_ROOT))
                    target->CastSpell(caster, SPELL_CHAINS_OF_WOE_TELE_0, true);

    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_chains_of_woe_eff::HandleEffectHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Chains of Woe Teleport 75464
class spell_chains_of_woe_teleport : public SpellScript
{
    PrepareSpellScript(spell_chains_of_woe_teleport);

    void HandleEffectHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(caster, SPELL_CHAINS_OF_WOE_ROOT, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_chains_of_woe_teleport::HandleEffectHit, EFFECT_0, SPELL_EFFECT_TELEPORT_UNITS);
    }
};

// Skullcracker 75543, 93453
class spell_skullcracker : public SpellScript
{
    PrepareSpellScript(spell_skullcracker);

    void HandleAfterCast()
    {
        if (Creature* caster = GetCaster()->ToCreature())
            caster->AI()->DoAction(ACTION_REMOVE_SKULLCRACKER);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_skullcracker::HandleAfterCast);
    }
};

// Skullcracker Eff 93454
class spell_skullcracker_eff : public SpellScript
{
    PrepareSpellScript(spell_skullcracker_eff);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if([=](WorldObject* target) { return target && target->ToCreature() && target->ToCreature()->GetEntry() != NPC_ROMOGG; });
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_skullcracker_eff::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

// Chains of Woe Caster Aura 75441
class spell_chains_of_woe_caster_aura : public AuraScript
{
    PrepareAuraScript(spell_chains_of_woe_caster_aura);

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* caster = GetCaster())
            if (InstanceScript* instance = caster->GetInstanceScript())
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAINS_OF_WOE_ROOT);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_chains_of_woe_caster_aura::OnAuraEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_boss_romogg_bonecrusher()
{
    new boss_romogg_bonecrusher();
    new spell_script<spell_chains_of_woe>("spell_chains_of_woe");
    new spell_script<spell_chains_of_woe_eff>("spell_chains_of_woe_eff");
    new spell_script<spell_chains_of_woe_teleport>("spell_chains_of_woe_teleport");
    new spell_script<spell_skullcracker>("spell_skullcracker");
    new spell_script<spell_skullcracker_eff>("spell_skullcracker_eff");
    new aura_script<spell_chains_of_woe_caster_aura>("spell_chains_of_woe_caster_aura");
}
