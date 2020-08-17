#include "grimbatol.h"
#include "ScriptPCH.h"

enum ScriptTexts
{
    SAY_AGGRO    = 0,
    SAY_KILL     = 1,
    SAY_DEATH    = 2,
    SAY_BLITZ    = 3,    
    SAY_ADDS     = 4,
    SAY_EVENT_1  = 5,    
    SAY_EVENT_2  = 6,    
};

enum Spells
{
    SPELL_BLEEDING_WOUND        = 74846,
    SPELL_GROUND_SIEGE_SELECTOR = 74636,
    SPELL_GROUND_SIEGE          = 74634,
    SPELL_GROUND_SIEGE_SUMM     = 74639,
    SPELL_BLITZ                 = 74670,
    SPELL_BLITZ_DMG             = 74675,
    SPELL_BLITZ_SUMM            = 74669,
    SPELL_FRENZY                = 74853,
    SPELL_CLAW_PUNCTURE         = 76507,
    SPELL_MODGUD_MALICE         = 74699,
    SPELL_MODGUD_MALICE_AURA    = 90170,
    SPELL_MODGUD_MALADY         = 74837,
};

enum Events
{
    EVENT_BLEEDING_WOUND       = 1,
    EVENT_GROUND_SIEGE         = 2,
    EVENT_BLITZ                = 3,
    EVENT_CLAW_PUNCTURE        = 4,
    EVENT_ADDS                 = 5,
};

enum Adds
{
    NPC_GROUND_SIEGE_STALKER    = 40030,
    NPC_BLITZ_STALKER           = 40040,
    NPC_MALIGNANT_TROGG         = 39984,
    NPC_TROGG_DWELLER           = 45467,
};

const Position troggPos[2] =
{
    { -722.15f, -442.53f, 268.77f, 0.54f },
    { -702.22f, -450.9f,  268.77f, 1.34f }
};

class boss_general_umbriss : public CreatureScript
{
    public:
        boss_general_umbriss() : CreatureScript("boss_general_umbriss"){}

        struct boss_general_umbrissAI : public BossAI
        {
            boss_general_umbrissAI(Creature *creature) : BossAI(creature, DATA_GENERAL_UMBRISS)
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

            bool bEnrage;
            uint64 targetGUID;

            void Reset() override
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                bEnrage = false;
                targetGUID = 0;
                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_ADDS, 30000);
                events.ScheduleEvent(EVENT_GROUND_SIEGE, 10000);
                events.ScheduleEvent(EVENT_BLEEDING_WOUND, 5000);
                events.ScheduleEvent(EVENT_BLITZ, 23000);
                Talk(SAY_AGGRO);
            }

            void JustDied(Unit* killer) override
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Talk(SAY_DEATH);
                summons.DespawnAll();
            }

            void KilledUnit(Unit* /*victim*/) override
            {
                Talk(SAY_KILL);
            }
            
            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);

                switch (summon->GetEntry())
                {
                    case NPC_MALIGNANT_TROGG:
                    case NPC_TROGG_DWELLER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            summon->AI()->AttackStart(target);
                        break;
                    case NPC_BLITZ_STALKER:
                        me->CastSpell(summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ(), SPELL_BLITZ, false);
                        break;
                    case NPC_GROUND_SIEGE_STALKER:
                        if (Unit* target = me->getVictim())
                            targetGUID = target->GetGUID();

                        me->PrepareChanneledCast(me->GetAngle(summon), SPELL_GROUND_SIEGE);

                        scheduler
                            .Schedule(Milliseconds(3000), [this](TaskContext context)
                        {
                            me->RemoveChanneledCast(targetGUID);
                        });
                        break;
                }
            }

            void SummonedCreatureDespawn(Creature* summon) override
            {
                summons.Despawn(summon);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!instance || !UpdateVictim())
                    return;

                if (me->GetDistance(me->GetHomePosition()) > 60.0f)
                {
                    EnterEvadeMode();
                    return;
                }

                if (HealthBelowPct(30) && !bEnrage)
                {
                    bEnrage = true;
                    DoCast(me, SPELL_FRENZY);
                    return;
                }

                scheduler.Update(diff);
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLEEDING_WOUND:
                            if (Unit* target = me->getVictim())
                                DoCast(target, SPELL_BLEEDING_WOUND);

                            events.ScheduleEvent(EVENT_BLEEDING_WOUND, 25000);
                            break;
                        case EVENT_GROUND_SIEGE:
                            events.DelayEvents(1 * IN_MILLISECONDS);
                            DoCast(me, SPELL_GROUND_SIEGE_SELECTOR, true);
                            events.ScheduleEvent(EVENT_GROUND_SIEGE, 18000);
                            break;
                        case EVENT_BLITZ:
                            Talk(SAY_BLITZ);
                            events.DelayEvents(2 * IN_MILLISECONDS);

                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, CasterSpecTargetSelector()))
                                me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_BLITZ_SUMM, true);
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                                me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_BLITZ_SUMM, true);

                            events.ScheduleEvent(EVENT_BLITZ, 23000);
                            break;
                        case EVENT_ADDS:
                            if (!bEnrage)
                            {
                                for(uint8 i = 0; i < 3; i++)
                                me->SummonCreature(NPC_TROGG_DWELLER, troggPos[0]);
                                me->SummonCreature(NPC_MALIGNANT_TROGG, troggPos[1]);
                                Talk(SAY_ADDS);
                            }
                            events.ScheduleEvent(EVENT_ADDS, 45000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_general_umbrissAI>(creature);
        }
};

// Malignant Trogg 39984
struct npc_malignant_trogg : public ScriptedAI
{
    npc_malignant_trogg(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() override
    {
        DoCast(me, SPELL_MODGUD_MALICE);
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_CLAW_PUNCTURE, 5000);
    }

    void JustDied(Unit* /*killer*/) override
    {
        DoCast(me, SPELL_MODGUD_MALADY, true);

        if (IsHeroic())
            DoCast(me, SPELL_MODGUD_MALICE_AURA, true);
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
            switch (eventId)
            {
                case EVENT_CLAW_PUNCTURE:
                    if (Unit* target = me->getVictim())
                        DoCast(target, SPELL_CLAW_PUNCTURE);
                
                    events.ScheduleEvent(EVENT_CLAW_PUNCTURE, urand(5000, 10000));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Trogg Dweller 45467
struct npc_umbriss_trogg_dweller : public ScriptedAI
{
    npc_umbriss_trogg_dweller(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_CLAW_PUNCTURE, 5000);
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
            switch (eventId)
            {
                case EVENT_CLAW_PUNCTURE:
                    if (Unit* target = me->getVictim())
                        DoCast(target, SPELL_CLAW_PUNCTURE);
                
                    events.ScheduleEvent(EVENT_CLAW_PUNCTURE, urand(5000, 10000));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Bleeding Wound 91937, 74846
class spell_umbriss_bleeding_wound : public AuraScript
{
    PrepareAuraScript(spell_umbriss_bleeding_wound);

    void OnUpdate(AuraEffect* aurEff, const uint32 diff)
    {
        if (GetOwner()->ToUnit() && GetOwner()->ToUnit()->HealthAbovePct(90))
            GetOwner()->ToUnit()->RemoveAurasDueToSpell(GetSpellInfo()->Id);
    }

    void Register() override
    {
        OnEffectUpdate += AuraEffectUpdateFn(spell_umbriss_bleeding_wound::OnUpdate, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

// Ground Siege Selector 74636
class spell_ground_siege_selector : public SpellScript
{
    PrepareSpellScript(spell_ground_siege_selector);

    std::list<WorldObject*> m_targets;

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        m_targets = targets;

        // Remove whole exclude tank
        targets.remove_if(TankSpecTargetSelector());


        // if we have enough players without tanks
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

    void HandleEffectHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_GROUND_SIEGE_SUMM, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ground_siege_selector::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_ground_siege_selector::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

void AddSC_boss_general_umbriss()
{
    new boss_general_umbriss();
    new creature_script<npc_malignant_trogg>("npc_malignant_trogg");
    new creature_script<npc_umbriss_trogg_dweller>("npc_umbriss_trogg_dweller");
    new aura_script<spell_umbriss_bleeding_wound>("spell_umbriss_bleeding_wound");
    new spell_script<spell_ground_siege_selector>("spell_ground_siege_selector");
}
