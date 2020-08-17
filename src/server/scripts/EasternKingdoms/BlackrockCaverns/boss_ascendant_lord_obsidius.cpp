#include "ScriptPCH.h"
#include "blackrock_caverns.h"

enum ScriptTexts
{
    SAY_AGGRO                   = 0,
    SAY_KILL                    = 1,
    SAY_TRANSFORMATION          = 2,
    SAY_DEATH                   = 3,
};
enum Spells
{
    SPELL_STONEBLOW             = 76185,
    SPELL_TWILIGHT_CORRUPTION   = 76188,
    SPELL_THUNDERCLAP           = 76186,
    SPELL_TWITCHY               = 76167,
    SPELL_SHADOW_OF_OBSIDIUS    = 76164,
    SPELL_CREPUSCULAR_VEIL      = 76189,
    SPELL_STOP_HEART            = 82393,
};

enum Events
{
    EVENT_STONEBLOW             = 1,
    EVENT_TWILIGHT_CORRUPTION   = 2,
    EVENT_THUNDERCLAP           = 3,
    EVENT_CREPUSCULAR_VEIL      = 4,
};

enum Adds
{
    NPC_SHADOW_OF_OBSIDIUS  = 40817,
};

const Position shadowofobsidiusPos[3] = 
{
    {328.19f, 561.97f, 66.0f, 4.50f},
    {335.24f, 556.08f, 66.0f, 1.64f},
    {330.25f, 549.84f, 66.f, 3.10f}
};
class boss_ascendant_lord_obsidius : public CreatureScript
{
    public:
        boss_ascendant_lord_obsidius() : CreatureScript("boss_ascendant_lord_obsidius") { }
     
        struct boss_ascendant_lord_obsidiusAI : public BossAI
        {
            boss_ascendant_lord_obsidiusAI(Creature* creature) : BossAI(creature, DATA_ASCENDANT_LORD_OBSIDIUS)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->setActive(true);
                hasTriggered = false;

                // Cosmetic Event
                for (auto&& itr : twilightDefenderPos)
                    me->SummonCreature(NPC_TWILIGHT_ELEMENT_WARDEN, itr, TEMPSUMMON_MANUAL_DESPAWN);
            }

            bool hasTriggered;

            void Reset() override
            {
                _Reset();
                summons.DespawnAll();

                if (IsHeroic())
                    for (uint8 i = 0; i < 3; i++)
                        shadows[i] = me->SummonCreature(NPC_SHADOW_OF_OBSIDIUS, shadowofobsidiusPos[i]);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_STONEBLOW, 6000);
                events.ScheduleEvent(EVENT_TWILIGHT_CORRUPTION, 20000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_THUNDERCLAP, 6000);
                Talk(SAY_AGGRO);
                DoZoneInCombat();

                scheduler
                    .Schedule(Milliseconds(6000), [this](TaskContext context)
                {
                    DoCast(me, SPELL_STOP_HEART);
                });
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();
                Talk(SAY_DEATH);
                summons.DespawnAll();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
     
            void KilledUnit(Unit* /*victim*/) override
            {
                Talk(SAY_KILL);
            }

            void SummonedCreatureDespawn(Creature* summon) override
            {
                summons.Despawn(summon);
            }

            void DoAction(int32 actionId) override
            {
                if (actionId == ACTION_RAZ_LAST_STAND && !hasTriggered)
                {
                    hasTriggered = true;
                    me->SummonCreature(NPC_RAZ_THE_CRAZED, lastStandByKaz[0], TEMPSUMMON_MANUAL_DESPAWN);
                }
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);

                if (me->isInCombat())
                    summon->SetInCombatWithZone();
            }

            void UpdateAI(uint32 diff) override
            {
                scheduler.Update(diff);

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
                        case EVENT_STONEBLOW:
                            DoCast(me->getVictim(), SPELL_STONEBLOW);
                            events.ScheduleEvent(EVENT_STONEBLOW, 6000);
                            break;
                        case EVENT_TWILIGHT_CORRUPTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_TWILIGHT_CORRUPTION);
                            events.ScheduleEvent(EVENT_TWILIGHT_CORRUPTION, 20000);
                            break;
                        case EVENT_THUNDERCLAP:
                            DoCast(me, SPELL_THUNDERCLAP);
                            events.ScheduleEvent(EVENT_THUNDERCLAP, 15000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            private:
                Creature* shadows[3];
        };
     
        CreatureAI* GetAI(Creature* creature) const
        {
            return GetInstanceAI<boss_ascendant_lord_obsidiusAI>(creature);
        }
};

// Shadow of Obsidius 40817
struct npc_shadow_of_obsidius : public ScriptedAI
{
    npc_shadow_of_obsidius(Creature* creature) : ScriptedAI(creature)
    {
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
    }

    EventMap events;

    void Reset() override
    {
        DoCast(me, SPELL_TWITCHY);
        DoCast(me, SPELL_SHADOW_OF_OBSIDIUS);
    }

    void EnterCombat(Unit* attacker) override
    {
        events.ScheduleEvent(EVENT_CREPUSCULAR_VEIL, 3900);
    }

    void DamageTaken(Unit* attacker, uint32& /*damage*/) override
    {
        if (me->getVictim() != attacker)
        {
            DoResetThreat();
            me->AddThreat(attacker, 1000000.0f);
            me->Attack(attacker, true);
            me->GetMotionMaster()->MoveChase(attacker);
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
            switch (eventId)
            {
                case EVENT_CREPUSCULAR_VEIL:
                    DoCast(me->getVictim(), SPELL_CREPUSCULAR_VEIL);
                    events.ScheduleEvent(EVENT_CREPUSCULAR_VEIL, 3900);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Stop Heart 82393
class spell_obsidius_stop_heart : public AuraScript
{
    PrepareAuraScript(spell_obsidius_stop_heart);

    void OnAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetOwner()->ToUnit())
            owner->Kill(owner);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_obsidius_stop_heart::OnAuraEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// AreaTrigger 6016
class AreaTrigger_at_before_obsidius : public AreaTriggerScript
{
    public:
        AreaTrigger_at_before_obsidius() : AreaTriggerScript("AreaTrigger_at_before_obsidius") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) override
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* obsidius = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_ASCENDANT_LORD_OBSIDIUS)))
                    obsidius->AI()->DoAction(ACTION_RAZ_LAST_STAND);

            return false;
        }
};

void AddSC_boss_ascendant_lord_obsidius()
{
    new boss_ascendant_lord_obsidius();
    new creature_script<npc_shadow_of_obsidius>("npc_shadow_of_obsidius");
    new aura_script<spell_obsidius_stop_heart>("spell_obsidius_stop_heart");
    new AreaTrigger_at_before_obsidius();
}
