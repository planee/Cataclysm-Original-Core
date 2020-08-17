#include "grimbatol.h"
#include "Spell.h"
#include "ScriptPCH.h"
#include "Vehicle.h"

enum ScriptTexts
{
    // drahga
    SAY_AGGRO,
    SAY_KILL,
    SAY_DEATH,
    SAY_ADDS,
    SAY_VALIONA,
                                       
    // valiona                         
    SAY_ENTER = 0,
    SAY_LEAVE,
};

enum Spells
{
    SPELL_BURNING_SHADOWBOLT           = 75245,
    SPELL_INVOCATION_OF_FLAME          = 75222,
    SPELL_INVOCATION_OF_FLAME_SUMM     = 75218,
    SPELL_INVOCATION_OF_FLAME_EFF      = 75232,
    SPELL_FLAMING_FIXATE               = 82850,
    SPELL_INVOKED_FLAME                = 75235,
    SPELL_SUPERNOVA                    = 75238,
    SPELL_TWILIGHT_PROTECTION          = 76303,
    SPELL_TWILIGHT_SHIFT               = 75328,
    SPELL_SHREDDING_SWIPE              = 75271,
    SPELL_SEEPING_TWILIGHT_DUMMY       = 75318,
    SPELL_SEEPING_TWILIGHT             = 75274,
    SPELL_SEEPING_TWILIGHT_DMG         = 75317,
    SPELL_VALIONAS_FLAME               = 75321,
    SPELL_DEVOURING_FLAMES             = 90950,
};

enum Adds
{
    NPC_INVOCATION_OF_FLAME_STALKER    = 40355,
    NPC_INVOKED_FLAMING_SPIRIT         = 40357,
    NPC_VALIONA                        = 40320,
    NPC_SEEPING_TWILIGHT               = 40365,
    NPC_DEVOURING_FLAMES               = 48798,
};

enum Events
{
    EVENT_BURNING_SHADOWBOLT = 1,
    EVENT_INVOCATION_OF_FLAME,
    EVENT_SELECT_TARGET,
    EVENT_VALIONAS_FLAME,
    EVENT_SHREDDING_SWIPE,
    EVENT_VALIONA_MOVE,
};

enum Actions
{
    ACTION_VALIONA_LEAVE = 1,
    ACTION_VALIONA_COMAT_INIT,
};

class boss_drahga_shadowburner : public CreatureScript
{
    public:
        boss_drahga_shadowburner() : CreatureScript("boss_drahga_shadowburner") { }

        struct boss_drahga_shadowburnerAI : public BossAI
        {
            boss_drahga_shadowburnerAI(Creature* creature) : BossAI(creature, DATA_DRAHGA_SHADOWBURNER)
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

            uint8 stage;
            uint64 targetGUID;

            void Reset() override
            {
                _Reset();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                me->ExitVehicle();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                summons.DespawnAll();
                stage = 0;
                targetGUID = 0;
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
            {
                if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL))
                    if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_BURNING_SHADOWBOLT)
                        for (uint8 i = 0; i < 3; ++i)
                            if (spell->Effects[i].Effect == SPELL_EFFECT_INTERRUPT_CAST)
                                me->InterruptSpell(CURRENT_GENERIC_SPELL);
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);
            }

            void DamageTaken(Unit* attacker, uint32& damage) override
            {
                if ((me->HealthBelowPct(30) || damage >= me->GetHealth()) && stage == 0)
                {
                    damage = 0;

                    if (me->getVictim())
                        targetGUID = me->getVictim()->GetGUID();

                    stage = 1;
                    DoCast(me, SPELL_TWILIGHT_PROTECTION, true);
                    me->SetReactState(REACT_PASSIVE);
                    me->GetMotionMaster()->MovePoint(0, DrahgaLeavePoint);
                    events.CancelEvent(EVENT_BURNING_SHADOWBOLT);
                    events.CancelEvent(EVENT_INVOCATION_OF_FLAME);
                }

                if (me->GetVehicle())
                    damage = 0;
            }

            void DoAction(int32 actionId) override
            {
                switch (actionId)
                {
                    case ACTION_VALIONA_COMAT_INIT:
                        events.ScheduleEvent(EVENT_BURNING_SHADOWBOLT, urand(2000, 5000));
                        events.ScheduleEvent(EVENT_INVOCATION_OF_FLAME, 10000);
                        break;
                    case ACTION_VALIONA_LEAVE:
                        me->RemoveChanneledCast(targetGUID);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        break;
                }
            }

            void SummonedCreatureDespawn(Creature* summon) override
            {
                summons.Despawn(summon);
            }

            void EnterCombat(Unit* /*who*/) override
            {
                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_BURNING_SHADOWBOLT, urand(2000, 5000));
                events.ScheduleEvent(EVENT_INVOCATION_OF_FLAME, 10000);
            }
            
            void JustDied(Unit* /*killer*/) override
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

            void MovementInform(uint32 type, uint32 pointId) override
            {
                if (type == POINT_MOTION_TYPE && pointId == 0)
                {
                    if (Creature* valiona = me->SummonCreature(NPC_VALIONA, drahgavalionaPos[0],TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        Talk(SAY_VALIONA);
                        valiona->SetReactState(REACT_PASSIVE);
                        valiona->SetCanFly(true);
                        me->EnterVehicle(valiona, 0);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    }
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
                        case EVENT_BURNING_SHADOWBOLT:
                            DoCast(me, SPELL_BURNING_SHADOWBOLT);
                            events.ScheduleEvent(EVENT_BURNING_SHADOWBOLT, urand(2 * IN_MILLISECONDS, 2.5 *IN_MILLISECONDS));
                            break;
                        case EVENT_INVOCATION_OF_FLAME:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, CasterSpecTargetSelector()))
                            {
                                DoCast(me, SPELL_INVOCATION_OF_FLAME_SUMM);
                                Talk(SAY_ADDS);
                            }
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                            {
                                DoCast(me, SPELL_INVOCATION_OF_FLAME_SUMM);
                                Talk(SAY_ADDS);
                            }

                            events.ScheduleEvent(EVENT_INVOCATION_OF_FLAME, 20000);
                            break;
                    }
                }
                if (!me->GetVehicle())
                    DoMeleeAttackIfReady();

                EnterEvadeIfOutOfCombatArea(diff);
            }

        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_drahga_shadowburnerAI>(creature);
        }
};

class npc_drahga_valiona : public CreatureScript
{
    public:
        npc_drahga_valiona() : CreatureScript("npc_drahga_valiona") { }

        struct npc_drahga_valionaAI : public ScriptedAI
        {
            npc_drahga_valionaAI(Creature* creature) : ScriptedAI(creature), summons(me)
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
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;
            EventMap events, nonCombatEvents;
            SummonList summons;
            uint8 stage, wp;

            void IsSummonedBy(Unit* summoner) override
            {
                wp = 1;
                nonCombatEvents.ScheduleEvent(EVENT_VALIONA_MOVE, 100);
                me->SetCanFly(true);
                me->SetDisableGravity(true);

                me->SetAnimationTier(UnitAnimationTier::Ground);
                me->SetCanFly(false);
                me->SetDisableGravity(false);
                me->SetHover(false);
            }

            void Reset() override
            {
                summons.DespawnAll();
                events.Reset();
                stage = 0;
                wp = 1;
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon) override
            {
                summons.Despawn(summon);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_VALIONAS_FLAME, urand(10000, 15000));
                events.ScheduleEvent(EVENT_SHREDDING_SWIPE, urand(8000, 10000));
            }

            void MovementInform(uint32 type, uint32 pointId) override
            {
                if (type == POINT_MOTION_TYPE)
                {
                    wp++;

                    switch (pointId)
                    {
                        case 1002:
                            me->DespawnOrUnsummon();
                            break;
                        case 1:
                            nonCombatEvents.ScheduleEvent(EVENT_VALIONA_MOVE, 100);
                            break;
                        case 2:
                            me->SetCanFly(false);
                            me->SetDisableGravity(false);
                            me->SetReactState(REACT_AGGRESSIVE);

                            if (Player* itr = me->FindNearestPlayer(80.0f))
                                me->AI()->AttackStart(itr);

                            if (Creature* drahga = Unit::GetCreature(*me, instance->GetData64(DATA_DRAHGA_SHADOWBURNER)))
                                drahga->AI()->DoAction(ACTION_VALIONA_COMAT_INIT);
                            break;
                    }
                }
            }

            void EnterEvadeMode() override
            {
                if (me->GetVehicleKit())
                    me->GetVehicleKit()->RemoveAllPassengers();
                me->DespawnOrUnsummon();
            }

            void DamageTaken(Unit* attacker, uint32& damage) override
            {
                if ((me->HealthBelowPct(30) || damage >= me->GetHealth()) && stage == 0)
                {
                    stage = 1;
                    events.Reset();
                    DoCast(me, SPELL_TWILIGHT_SHIFT, true);

                    me->SetReactState(REACT_PASSIVE);
                    if (me->GetVehicleKit())
                        me->GetVehicleKit()->RemoveAllPassengers();

                    me->SetCanFly(true);
                    me->SetDisableGravity(true);
                    me->GetMotionMaster()->MovePoint(1002, ValioneLeavePoint);

                    if (Creature* drahga = Unit::GetCreature(*me, instance->GetData64(DATA_DRAHGA_SHADOWBURNER)))
                    {
                        drahga->RemoveAurasDueToSpell(SPELL_TWILIGHT_PROTECTION);
                        drahga->AI()->DoAction(ACTION_VALIONA_LEAVE);
                    }

                    Talk(SAY_LEAVE);
                    return;
                }
            }

            void UpdateAI(uint32 diff) override
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    if (eventId == EVENT_VALIONA_MOVE)
                    {
                        me->GetMotionMaster()->MovementExpired();
                        me->GetMotionMaster()->MovePoint(wp, drahgavalionaPos[wp]);
                    }
                    break;
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_VALIONAS_FLAME:
                            if (IsHeroic())
                                DoCast(me, SPELL_DEVOURING_FLAMES);
                            else if (Unit* target = me->getVictim())
                                DoCast(target, SPELL_VALIONAS_FLAME);

                            events.ScheduleEvent(EVENT_VALIONAS_FLAME, urand(15000, 22000));
                            break;
                        case EVENT_SHREDDING_SWIPE:
                            if (Unit* target = me->getVictim())
                                DoCast(target, SPELL_SHREDDING_SWIPE, true);

                            events.ScheduleEvent(EVENT_SHREDDING_SWIPE, urand(12000, 16000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_drahga_valionaAI>(creature);
        }
};

// Invocation of Flame Stalker 40355
struct npc_invocation_of_flame_stalker : public ScriptedAI
{
    npc_invocation_of_flame_stalker(Creature* creature) : ScriptedAI(creature)
    {
        instance = creature->GetInstanceScript();
    }

    InstanceScript* instance;

    void JustSummoned(Creature* summon) override
    {
        switch (summon->GetEntry())
        {
            case NPC_INVOKED_FLAMING_SPIRIT:
                summon->SetInCombatWithZone();
                break;
        }
    }

    void Reset() override
    {
        DoCast(me, SPELL_INVOCATION_OF_FLAME);
        DoCast(me, SPELL_INVOCATION_OF_FLAME_EFF, true);
    }
};

// Invoked Flaming Spirit 40357
struct npc_invoked_flaming_spirit : public ScriptedAI
{
    npc_invoked_flaming_spirit(Creature* creature) : ScriptedAI(creature)
    {
        me->SetSpeed(MOVE_RUN, 0.45f);
        me->SetSpeed(MOVE_WALK, 0.45f);
        me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
    }

    TaskScheduler scheduler;

    void Reset() override
    {
        me->SetWalk(true);
        me->AddUnitState(UNIT_STATE_ROOT);

        if (Creature* drahga = Unit::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(DATA_DRAHGA_SHADOWBURNER) : 0))
            drahga->AI()->JustSummoned(me);

        // DB hasn`t HC spawn for this creature
        if (IsHeroic())
        {
            // should be 77.0k in heroic
            me->SetCreateHealth(77000);
            me->SetMaxHealth(77000);
            me->SetHealth(77000);
            me->ResetPlayerDamageReq();
        }

    }

    void EnterCombat(Unit* /*who*/) override
    {
        DoCast(me, SPELL_INVOKED_FLAME);

        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            me->ClearUnitState(UNIT_STATE_ROOT);

            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, CasterSpecTargetSelector()))
            {
                SetGazeOn(target);
                DoCast(target, SPELL_FLAMING_FIXATE);
                me->ClearUnitState(UNIT_STATE_CASTING);
            }
            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
            {
                SetGazeOn(target);
                DoCast(target, SPELL_FLAMING_FIXATE);
                me->ClearUnitState(UNIT_STATE_CASTING);
            }
        });
    }

    void JustDied(Unit* /*target*/) override
    {
        me->DisappearAndDie();
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Seeping Twilight 40365
struct npc_seeping_twilight : public ScriptedAI
{
    npc_seeping_twilight(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        DoCast(me, SPELL_SEEPING_TWILIGHT_DUMMY);
        DoCast(me, SPELL_SEEPING_TWILIGHT);
    }
};

// Supernova  75238, 90972
class spell_drahga_supernova : public SpellScript
{
    PrepareSpellScript(spell_drahga_supernova);

    void HandleScriptEffect(SpellEffIndex effIndex)
    {
        if (Creature* caster = GetCaster()->ToCreature())
            caster->DisappearAndDie();
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_drahga_supernova::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// Burning Shadowbolt 75245, 90915
class spell_drahga_burning_shadowbolt : public SpellScript
{
    PrepareSpellScript(spell_drahga_burning_shadowbolt);

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        if (targets.size() > 1)
            Trinity::Containers::RandomResizeList(targets, 1);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_drahga_burning_shadowbolt::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

void AddSC_boss_drahga_shadowburner()
{
    new boss_drahga_shadowburner();
    new creature_script<npc_invocation_of_flame_stalker>("npc_invocation_of_flame_stalker");
    new creature_script<npc_invoked_flaming_spirit>("npc_invoked_flaming_spirit");
    new npc_drahga_valiona();
    new creature_script<npc_seeping_twilight>("npc_seeping_twilight");
    new spell_script<spell_drahga_supernova>("spell_drahga_supernova");
    new spell_script<spell_drahga_burning_shadowbolt>("spell_drahga_burning_shadowbolt");
}
