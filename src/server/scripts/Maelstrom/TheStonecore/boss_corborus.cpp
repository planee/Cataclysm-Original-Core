#include "ScriptPCH.h"
#include "the_stonecore.h"

enum Spells
{
    SPELL_CRYSTAL_BARRAGE           = 86881,
    SPELL_CRYSTAL_BARRAGE_SUM       = 92012,
    SPELL_CRYSTAL_SHARD_DMG         = 92122,
    SPELL_DUMPENING_WAVE            = 82415,
    SPELL_SUBMERGE                  = 53421,
    SPELL_ROCK_BORE                 = 80028,
                                    
    /*Thrashing Charge*/            
    SPELL_THRASHING_CHARGE_EFF      = 81828, // cone AOE
    SPELL_THRASHING_CHARGE_EFF_H    = 92651,
    SPELL_THRASHING_CHARGE_PATH     = 81801,
    SPELL_THRASHING_CHARGE_SELECTOR = 81838,
    SPELL_THRASHING_CHARGE_TELE     = 81839,
    SPELL_THRASHING_CHARGE_SUM      = 81816,
};

enum Events
{
    EVENT_CRYSTAL_BARRAGE         = 1,
    EVENT_DUMPENING_WAVE          = 2,
    EVENT_MERGE                   = 3,
    EVENT_SUBMERGE                = 4,
    EVENT_CRYSTAL_BARRAGE_H       = 5,
    EVENT_THRASHING_CHARGE        = 6,
    EVENT_ROCK_BORER              = 7,
    EVENT_ROCK_BORE               = 8,
    EVENT_CRYSTAL_SHARD_MOVE      = 9,
};

enum Adds
{
    NPC_ROCK_BORER                = 43917,
    NPC_CRYSTAL_SHARD             = 49267,
    NPC_THRASHING_CHARGE          = 43743,
};

class boss_corborus : public CreatureScript
{
    public:
        boss_corborus() : CreatureScript("boss_corborus") { }

        struct boss_corborusAI : public BossAI
        {
            boss_corborusAI(Creature* creature) : BossAI(creature, DATA_CORBORUS), summons(me)
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
            SummonList summons;
            uint8 stage;
            uint64 borrowGUID;
            Position barragePos;
            bool inMerge;
            float x, y;

            void Reset() override
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                stage = 0;
                inMerge = true;
                x = 0.0f; y = 0.0f;
                borrowGUID = 0;
                me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);    
                events.Reset();
                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_DUMPENING_WAVE, 5000);
                events.ScheduleEvent(EVENT_CRYSTAL_BARRAGE, 7000);
                events.ScheduleEvent(EVENT_SUBMERGE, 35000);
            }

            uint64 GetGUID(int32 /*type*/) const override
            {
                return borrowGUID;
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);

                switch (summon->GetEntry())
                {
                    case NPC_THRASHING_CHARGE:
                        borrowGUID = summon->GetGUID();
                        break;
                    case NPC_ROCK_BORER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankSpecTargetSelector()))
                        {
                            summon->AddThreat(target, 10.0f);
                            summon->Attack(target, true);
                            summon->GetMotionMaster()->MoveChase(target);
                        }
                        else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                        {
                            summon->AddThreat(target, 10.0f);
                            summon->Attack(target, true);
                            summon->GetMotionMaster()->MoveChase(target);
                        }
                        break;
                    case NPC_CRYSTAL_SHARD:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankSpecTargetSelector()))
                        {
                            summon->AddThreat(target, 10.0f);
                            summon->Attack(target, true);
                            summon->GetMotionMaster()->MoveChase(target);
                        }
                        else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                        {
                            summon->AddThreat(target, 10.0f);
                            summon->Attack(target, true);
                            summon->GetMotionMaster()->MoveChase(target);
                        }
                        break;
                }
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                summons.DespawnAll();
            }

            void SummonedCreatureDespawn(Creature* summon) override
            {
                summons.Despawn(summon);
            }

            void MovementInform(uint32 type, uint32 pointId) override
            {
                if (pointId == EVENT_CHARGE)
                {
                    me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI / 2));
                    me->SetHomePosition(*me);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                }
            }

            void UpdateAI(uint32 const diff) override
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
                        case EVENT_DUMPENING_WAVE:
                            DoCast(SPELL_DUMPENING_WAVE);
                            events.ScheduleEvent(EVENT_DUMPENING_WAVE, 10000);
                            break;
                        case EVENT_CRYSTAL_BARRAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            {
                                DoCast(target, SPELL_CRYSTAL_BARRAGE);
                                if (IsHeroic())
                                {
                                    barragePos = *target;
                                    events.ScheduleEvent(EVENT_CRYSTAL_BARRAGE_H, 4000);
                                }    
                            }
                            events.ScheduleEvent(EVENT_CRYSTAL_BARRAGE, 15000);
                            break;
                        case EVENT_CRYSTAL_BARRAGE_H:
                            for (uint8 i = 0; i < 5; i++)
                            {
                                me->CastSpell(
                                    barragePos.GetPositionX() + frand(-3.0f, 3.0f),
                                    barragePos.GetPositionY() + frand(-3.5f, 3.5f),
                                    barragePos.GetPositionZ(),
                                    SPELL_CRYSTAL_BARRAGE_SUM, true);
                            }  
                            break;
                        case EVENT_SUBMERGE:
                            inMerge = true;
                            me->RemoveAllAuras();
                            me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            me->PrepareChanneledCast(me->GetOrientation());
                            me->HandleEmoteState(EMOTE_STATE_SUBMERGED);
                            events.Reset();
                            events.ScheduleEvent(EVENT_THRASHING_CHARGE, 3000);
                            events.ScheduleEvent(EVENT_MERGE, 20000);

                            scheduler
                                .Schedule(Milliseconds(3000), [this](TaskContext context)
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankSpecTargetSelector()))
                                {
                                    me->SummonCreature(NPC_ROCK_BORER,
                                        target->GetPositionX() + urand(3, 5),
                                        target->GetPositionY() + urand(3, 5),
                                        target->GetPositionZ(), 0.0f);
                                }
                                else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                                {
                                    me->SummonCreature(NPC_ROCK_BORER,
                                        target->GetPositionX() + urand(3, 5),
                                        target->GetPositionY() + urand(3, 5),
                                        target->GetPositionZ(), 0.0f);
                                }

                                context.Repeat(Milliseconds(1800));
                            });
                            break;
                        case EVENT_MERGE:
                            inMerge = false;
                            scheduler.CancelAll();
                            me->HandleEmoteState(EMOTE_STATE_POINT);
                            me->RemoveChanneledCast();
                            me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            events.Reset();
                            events.ScheduleEvent(EVENT_DUMPENING_WAVE, 5000);
                            events.ScheduleEvent(EVENT_CRYSTAL_BARRAGE, 7000);
                            events.ScheduleEvent(EVENT_SUBMERGE, 30000);
                            break;
                        case EVENT_THRASHING_CHARGE:
                            DoCast(me, SPELL_THRASHING_CHARGE_SELECTOR, true);
                            events.ScheduleEvent(EVENT_THRASHING_CHARGE, 4500);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_corborusAI>(creature);
        }
};

// Rock Borer 43917
struct npc_rock_borer : public ScriptedAI
{
    npc_rock_borer(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_ROCK_BORE, 2000);
    }

    void UpdateAI(uint32 const diff) override
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
                case EVENT_ROCK_BORE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_ROCK_BORE);
                
                    events.ScheduleEvent(EVENT_ROCK_BORE, 5000);
                    return;
            }
        }
        DoMeleeAttackIfReady();
    }
};

// Crystal Shard 49267
struct npc_crystal_shard : public ScriptedAI
{
    npc_crystal_shard(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        me->SetSpeed(MOVE_WALK, 0.5f);
        me->SetSpeed(MOVE_RUN, 0.5f);
    }

    void UpdateAI(uint32 const /*diff*/) override
    {
        if (!UpdateVictim())
            return;

        if (Unit* target = me->SelectNearestTarget(3.0f))
        {
            DoCast(target, SPELL_CRYSTAL_SHARD_DMG);
            me->DespawnOrUnsummon();
        }
    }
};

class ThrashingChargeDistanceOrderPred
{
    public:
        ThrashingChargeDistanceOrderPred(const WorldObject* object, bool ascending = true) : m_object(object), m_ascending(ascending) {}
        bool operator() (const WorldObject* a, const WorldObject* b) const
        {
            return m_ascending ? a->GetDistance(m_object) < b->GetDistance(m_object) :
                a->GetDistance(m_object) > b->GetDistance(m_object);
        }
    private:
        const WorldObject* m_object;
        const bool m_ascending;
};

// Thrashing Charge Selector 81838
class spell_stonecore_thrashing_charge_selector : public SpellScript
{
    PrepareSpellScript(spell_stonecore_thrashing_charge_selector);

    std::list<WorldObject*> m_targets;

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (Creature* caster = GetCaster()->ToCreature())
        {
            if (Unit* target = GetHitUnit())
            {
                caster->SetFacingToObject(target);

                // Path Triggers
                target->CastSpell(target, SPELL_THRASHING_CHARGE_SUM, true);

                caster->CastSpell(caster, SPELL_THRASHING_CHARGE_PATH, false);
                caster->AI()->DoAction(ACTION_CORBORUS_CONE);
            }
        }
    }

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        m_targets = targets;

        targets.remove_if(TankSpecTargetSelector());

        if (GetCaster() && !targets.empty())
        {
            // Try to select farthest victim
            targets.sort(ThrashingChargeDistanceOrderPred(GetCaster(), false));
            targets.resize(1);
            return;
        }

        targets.clear();
        targets = m_targets;

        if (targets.size() > 1)
            Trinity::Containers::RandomResizeList(targets, 1);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_stonecore_thrashing_charge_selector::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_stonecore_thrashing_charge_selector::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// Thrashing Charge Path 81801
class spell_stonecore_thrashing_charge_path : public SpellScript
{
    PrepareSpellScript(spell_stonecore_thrashing_charge_path);

    std::list<WorldObject*> m_targets;

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (Creature* caster = GetCaster()->ToCreature())
        {
            caster->CastSpell(caster, caster->GetMap()->IsHeroic() ? SPELL_THRASHING_CHARGE_EFF_H : SPELL_THRASHING_CHARGE_EFF, true);

            // Teleport to burrower stalker
            if (Creature* stalker = ObjectAccessor::GetCreature(*caster, caster->AI()->GetGUID()))
                caster->CastSpell(stalker, SPELL_THRASHING_CHARGE_TELE, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_stonecore_thrashing_charge_path::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

void AddSC_boss_corborus()
{
    new boss_corborus();
    new creature_script<npc_rock_borer>("npc_rock_borer");
    new creature_script<npc_crystal_shard>("npc_crystal_shard");
    new spell_script<spell_stonecore_thrashing_charge_selector>("spell_stonecore_thrashing_charge_selector");
    new spell_script<spell_stonecore_thrashing_charge_path>("spell_stonecore_thrashing_charge_path");
}