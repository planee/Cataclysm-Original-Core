#include "ScriptPCH.h"
#include "the_stonecore.h"

enum Spells
{
    SPELL_GROUND_SLAM      = 78903,
    SPELL_GROUND_SLAM_HC   = 92410,
    SPELL_SHATTER          = 78807,
    SPELL_PARALYZE         = 92426,
    SPELL_PARALYZE_EFF     = 94661,
    SPELL_ENRAGE           = 80467,
    SPELL_SHIELD           = 78835,
    SPELL_BULWARK          = 78939,

    // Rupture
    SPELL_RUPTURE_EFF      = 95346,
    SPELL_RUPTURE_SUMM     = 95348, // spikes
    SPELL_RUPTURE_TRIGGER  = 92393,
};

enum Events
{
    EVENT_GROUND_SLAM      = 1,
    EVENT_SHATTER          = 2,
    EVENT_PARALYZE         = 3,
    EVENT_SHIELD           = 4,
    EVENT_BULWARK          = 5,
};

enum Yells
{
    TALK_AGGRO,
    TALK_ELEMENTIUM_BULLWARK,
    TALK_ELEMENTIUM_BULLWARK_ANN,
    TALK_DEATH,
    TALK_SLAY,
};

class boss_ozruk : public CreatureScript
{
    public:
        boss_ozruk() : CreatureScript("boss_ozruk") { }

        struct boss_ozrukAI : public BossAI
        {
            boss_ozrukAI(Creature* creature) : BossAI(creature, DATA_OZRUK)
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
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            }

            EventMap events;
            bool bEnrage;
            uint64 targetGUID;

            void Reset() override
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                bEnrage = false;
                targetGUID = 0;
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_SHATTER, 21000);
                events.ScheduleEvent(EVENT_GROUND_SLAM, 7000);
                events.ScheduleEvent(EVENT_BULWARK, 80000);
                events.ScheduleEvent(EVENT_SHIELD, 3000);

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_PARALYZE, 19000);

                Talk(TALK_AGGRO);
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Talk(TALK_DEATH);
            }

            void KilledUnit(Unit* /*victim*/) override
            {
                Talk(TALK_SLAY);
            }

            void UpdateAI(uint32 diff) override
            {
                scheduler.Update(diff);

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(20) && !bEnrage)
                {
                    DoCast(me, SPELL_ENRAGE);
                    bEnrage = true;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GROUND_SLAM:
                            if (Unit* target = me->getVictim())
                            {
                                targetGUID = target->GetGUID();
                                me->PrepareChanneledCast(me->GetAngle(target), SPELL_GROUND_SLAM);
                            }

                            scheduler
                                .SetValidator([this] { return instance && instance->GetBossState(DATA_OZRUK) == IN_PROGRESS; })
                                .Schedule(Milliseconds(3500), [this](TaskContext context)
                            {
                                me->RemoveChanneledCast(targetGUID);
                            });
                            events.ScheduleEvent(EVENT_GROUND_SLAM, 15000);
                            events.RescheduleEvent(EVENT_SHATTER, urand(6000, 7000));
                            break;
                        case EVENT_SHATTER:
                            DoCast(SPELL_SHATTER);
                            events.ScheduleEvent(EVENT_SHATTER, 20000);
                            break;
                        case EVENT_PARALYZE:
                            DoCast(SPELL_PARALYZE);
                            events.ScheduleEvent(EVENT_PARALYZE, 20 * IN_MILLISECONDS);
                            break;
                        case EVENT_SHIELD:
                            DoCast(SPELL_SHIELD);
                            events.ScheduleEvent(EVENT_SHIELD, 15000);
                            break;
                        case EVENT_BULWARK:
                            Talk(TALK_ELEMENTIUM_BULLWARK);
                            Talk(TALK_ELEMENTIUM_BULLWARK_ANN);
                            DoCast(SPELL_BULWARK);
                            events.ScheduleEvent(EVENT_BULWARK, 15000);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_ozrukAI>(creature);
        }
};

// Rapture Controller 49597
struct npc_stonecore_rapture_controller : public ScriptedAI
{
    npc_stonecore_rapture_controller(Creature* creature) : ScriptedAI(creature) { }

    uint32 delay;
    TaskScheduler scheduler;
    float dist;
    float x, y, newX, newY;

    void IsSummonedBy(Unit* summoner) override
    {
        dist = 3.0f;
        x = 0.0f; y = 0.0f;
        newX = 0.0f; newY = 0.0f;

        scheduler
            .SetValidator([this] { return dist < 40.0f; })
            .Schedule(Milliseconds(100), [this](TaskContext context)
        {
            CalculateNestPos(me->GetPositionX(), me->GetPositionY(), dist, me->GetOrientation(), x, y);
            me->CastSpell(x, y, me->GetPositionZ(), SPELL_RUPTURE_SUMM, true);

            for (uint8 i = 0; i < 2; i++)
            {
                CalculateNestPos(x, y, 2.5f, Position::NormalizeOrientation(me->GetOrientation() + i == 0 ? M_PI / 2 : -M_PI / 2), newX, newY);
                me->CastSpell(newX, newY, me->GetPositionZ(), SPELL_RUPTURE_SUMM, true);
            }
            dist += 3.0f;
            context.Repeat(Milliseconds(200));
        });
    }

    void JustSummoned(Creature* summon) override
    {
        summon->CastSpell(summon, SPELL_RUPTURE_TRIGGER, true);
        summon->DespawnOrUnsummon(2 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);
    }

    private:
        void CalculateNestPos(float prevX, float prevY, float dist, float ori, float& x, float& y)
        {
            x = prevX + (dist * cos(ori));
            y = prevY + (dist * sin(ori));
        }
};

// Rupture Trigger 92393
class spell_stonecore_rupture_trigger : public AuraScript
{
    PrepareAuraScript(spell_stonecore_rupture_trigger);

    void HandleAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetOwner()->ToUnit())
            owner->CastSpell(owner, SPELL_RUPTURE_EFF, true);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_stonecore_rupture_trigger::HandleAuraEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
    }
};

// Paralyze 92426
class spell_stonecore_paralyze : public AuraScript
{
    PrepareAuraScript(spell_stonecore_paralyze);

    void HandleAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetOwner()->ToUnit())
        {
            // Save Last Duration on Remove
            if (aurEff->GetBase()->GetDuration() > 0)
                return;

            owner->CastSpell(owner, SPELL_PARALYZE_EFF, true);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_stonecore_paralyze::HandleAuraEffectRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
    }
};


void AddSC_boss_ozruk()
{
    new boss_ozruk();
    new creature_script<npc_stonecore_rapture_controller>("npc_stonecore_rapture_controller");
    new aura_script<spell_stonecore_rupture_trigger>("spell_stonecore_rupture_trigger");
    new aura_script<spell_stonecore_paralyze>("spell_stonecore_paralyze");
}