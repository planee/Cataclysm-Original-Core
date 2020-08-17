#include "ScriptPCH.h"
#include "the_stonecore.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"

enum Spells
{
    // Crystalspawn Giant
    SPELL_QUAKE                 = 81008,
    H_SPELL_QUAKE               = 92631,
                                
    // Imp                      
    SPELL_FELL_FIREBALL         = 80344,
    H_SPELL_FELL_FIREBALL       = 92638, 
                                
    // Millhouse Manastorm      
    SPELL_BLUR                  = 81216,
    SPELL_MILL_FEAR             = 81442,
    SPELL_FROSTBOLT_VOLLEY      = 81440,           
    H_SPELL_FROSTBOLT_VOLLEY    = 92642,
    SPELL_IMPENDING_DOOM        = 86830,
    SPELL_SHADOW_BOLT           = 81439,           
    H_SPELL_SHADOW_BOLT         = 92641,
    SPELL_SHADOWFURY            = 81441,
    H_SPELL_SHADOWFURY          = 92644,
    SPELL_TIGULE                = 81220,
    SPELL_IMPENDING_DOOM_VISUAL = 86838,
    SPELL_IMPENDING_DOOM_EFF    = 86851,

    // Stonecore Berserker
    SPELL_SCHARGE               = 81574,
    SPELL_SPINNING_SLASH        = 81568,
                                
    // Stonecore Bruiser        
    SPELL_BODY_SLAM             = 80180,
    SPELL_SHOCKWAVE             = 80195,
    H_SPELL_SHOCKWAVE           = 92640,
                                
    // Stonecore Earthshaper    
    SPELL_DUST_STORM            = 81463,
    SPELL_FORCE_OF_EARTH        = 81459,
    SPELL_GROUND_SHOCK          = 81530,       
    H_SPELL_GROUND_SHOCK        = 92628,
    SPELL_LAVA_BURST            = 81576,
    H_SPELL_LAVA_BURST          = 92626, 
                                
    // Stonecore Flayer         
    SPELL_FLAY                  = 79922,
                                
    // Stonecore Magmalord (    
    SPELL_IGNITE                = 80151,
    H_SPELL_IGNITE              = 92636,
    SPELL_MAGMA_ERUPTION        = 80038,
                                
    // Stonecore Rift Conjure   r 
    SPELL_DEMON_PORTAL          = 80308,
    SPELL_SHADOWBOLT            = 80279,             
    H_SPELL_SHADOWBOLT          = 92637,
                                
    //Stonecore Sentry          
                                
    // Stonecore Warbringer     
    SPELL_CLEAVE                = 15496,
    SPELL_RAGE                  = 80158,
};

enum eEvents
{
    EVENT_NONE,
    EVENT_QUAKE,
    EVENT_FELL_FIREBALL,
    EVENT_BLUR,
    EVENT_MILL_FEAR,
    EVENT_FROSTBOLT_VOLLEY,
    EVENT_IMPENDING_DOOM,
    EVENT_SHADOW_BOLT,
    EVENT_SHADOWFURY,
    EVENT_TIGULE,
    EVENT_ROCK_BORE,
    EVENT_SCHARGE,
    EVENT_SPINNING_SLASH,
    EVENT_BODY_SLAM,
    EVENT_SHOCKWAVE,
    EVENT_DUST_STORM,
    EVENT_FORCE_OF_EARTH,
    EVENT_GROUND_SHOCK,
    EVENT_LAVA_BURST,
    EVENT_FLAY,
    EVENT_IGNITE,
    EVENT_MAGMA_ERUPTION,
    EVENT_DEMON_PORTAL,
    EVENT_SHADOWBOLT,
    EVENT_CLEAVE,
    EVENT_RAGE,
    EVENT_COSMETIC_MOVE,
    EVENT_CALL_CORBORUS,
};

enum Actions
{
    ACTION_START_INTRO,
};

// Crystalspawn Giant 42810
struct npc_crystalspawn_giant : public SmartCreatureAI
{
    npc_crystalspawn_giant(Creature* creature) : SmartCreatureAI(creature) { }

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_QUAKE, urand(5000, 6000));
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
            ExecuteTargetEvent(SPELL_QUAKE, urand(5000, 6000), EVENT_QUAKE, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Imp 43014
struct npc_impp : public ScriptedAI
{
    npc_impp(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_FELL_FIREBALL, 1000);
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
                case EVENT_FELL_FIREBALL:
                    if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankSpecTargetSelector()))
                        DoCast(target, SPELL_FELL_FIREBALL);
                    else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                        DoCast(target, SPELL_FELL_FIREBALL);

                    events.RescheduleEvent(EVENT_FELL_FIREBALL, 1000);
                    return;
            }
        }

        DoMeleeAttackIfReady();
    }
};

enum ScriptTexts
{
    SAY_EVENT_1    = 0,
    SAY_EVENT_2    = 1,
    SAY_EVENT_3    = 2,
    SAY_DOOM       = 3,
};

const Position millhousemanastormscPos[3] = 
{
    { 987.67f,  882.45f, 303.37f, 2.07f },
    { 1075.72f, 862.74f, 291.48f, 2.86f },
    { 1151.45f, 885.74f, 284.96f, 3.36f }
};

// Millhouse Manastorm 43391
struct npc_sc_millhouse_manastorm : public ScriptedAI
{
    npc_sc_millhouse_manastorm(Creature* creature) : ScriptedAI(creature) { }

    EventMap events, nonCombatEvents;
    uint8 stage;
    TaskScheduler scheduler;

    void InitializeAI() override
    {
        stage = 1;
        Reset();
    }

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_MILL_FEAR, 10000);
        events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 7000 + rand() % 10000);
        events.ScheduleEvent(EVENT_SHADOW_BOLT, 1000);
        events.ScheduleEvent(EVENT_SHADOWFURY, 5000 + rand() % 15000);
    }

    void DamageTaken(Unit* attacker, uint32& damage) override
    {
        if ((damage >= me->GetHealth() || me->HealthBelowPct(51)) && !me->HasAura(SPELL_BLUR) && stage < 4)
        {
            damage = 0;
            Reset();
            me->RemoveAllAuras();
            Talk(stage - 1);
            DoCast(me, SPELL_BLUR, true);
            me->StopMoving();
            me->CastStop();
            me->PrepareChanneledCast(me->GetOrientation());
            stage++;

            Movement::MoveSplineInit init{ me };

            for (uint8 i = stage < 3 ? 0 : stage - 1; i < stage; i++)
            {
                if (i == 1)
                    continue;

                if (stage < 3)
                {
                    init.MoveTo(ManaStormPathEvent[i].GetPositionX(), ManaStormPathEvent[i].GetPositionY(), ManaStormPathEvent[i].GetPositionZ());
                    init.SetSmooth();
                    init.Launch();

                    scheduler
                        .Schedule(Milliseconds(me->GetSplineDuration()), [this, i](TaskContext context)
                    {
                        me->StopMoving();
                        Movement::MoveSplineInit newInit{ me };
                        newInit.MoveTo(ManaStormPathEvent[i + 1].GetPositionX(), ManaStormPathEvent[i + 1].GetPositionY(), ManaStormPathEvent[i + 1].GetPositionZ());

                        newInit.SetSmooth();
                        newInit.Launch();

                        nonCombatEvents.ScheduleEvent(EVENT_COSMETIC_MOVE, me->GetSplineDuration());
                    });

                    return;
                }
                else
                    init.MoveTo(ManaStormPathEvent[i].GetPositionX(), ManaStormPathEvent[i].GetPositionY(), ManaStormPathEvent[i].GetPositionZ());
            }

            init.SetSmooth();
            init.Launch();
            nonCombatEvents.ScheduleEvent(EVENT_COSMETIC_MOVE, me->GetSplineDuration());
        }
    }

    void MovementInform(uint32 type, uint32 pointId) override
    {
        if (pointId == EVENT_JUMP)
            me->DespawnOrUnsummon();
    }

    void DoAction(int32 actionId)
    {
        // in case if areatrigger
        if (actionId == ACTION_MANASTORM_PULL)
        {
            nonCombatEvents.Reset();
            me->InterruptNonMeleeSpells(true, SPELL_IMPENDING_DOOM);
            me->RemoveAllAuras();
        }
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);
        nonCombatEvents.Update(diff);

        while (uint32 eventId = nonCombatEvents.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_COSMETIC_MOVE:
                    me->SetHomePosition(*me);
                    me->RemoveAurasDueToSpell(SPELL_BLUR);
                    me->StopMoving();
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetFacingTo(Position::NormalizeOrientation(me->GetOrientation() - M_PI));
                    EnterEvadeMode();
                
                    // If Last Stand - Launch Impending Doom and Interrupt after 15s (or areatrigger)
                    if (stage > 3)
                    {
                        Talk(3); // DOOOOM!
                        DoCast(me, SPELL_IMPENDING_DOOM_VISUAL, true);
                        DoCast(me, SPELL_IMPENDING_DOOM);
                        nonCombatEvents.ScheduleEvent(EVENT_CALL_CORBORUS, 15 * IN_MILLISECONDS);
                    }
                    break;
                case EVENT_CALL_CORBORUS:
                    me->InterruptNonMeleeSpells(true, SPELL_IMPENDING_DOOM);
                    me->RemoveAllAuras();

                    if (me->GetInstanceScript())
                        me->GetInstanceScript()->SetData(DATA_CORBORUS, SPECIAL);
                    break;
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
                case EVENT_IMPENDING_DOOM:
                    me->Kill(me);
                    break;
                case EVENT_MILL_FEAR:
                    if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_MILL_FEAR);
                    events.RescheduleEvent(EVENT_MILL_FEAR, 10000);
                    return;
                case EVENT_SHADOW_BOLT:
                    if (Unit* target = me->getVictim())
                        DoCast(target, SPELL_SHADOW_BOLT);
                
                    events.RescheduleEvent(EVENT_SHADOWBOLT, 1000);
                    return;
                case EVENT_FROSTBOLT_VOLLEY:
                    if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_FROSTBOLT_VOLLEY);
                    events.RescheduleEvent(EVENT_FROSTBOLT_VOLLEY, rand() % 15000);
                    return;
                case EVENT_SHADOWFURY:
                    if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_SHADOWFURY);
                    events.RescheduleEvent(SPELL_SHADOWFURY, 5000 + rand() % 15000);
                    return;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Stonecore Teleport 51396, 51397
struct npc_stonecore_instance_portal : public CreatureAI
{
    npc_stonecore_instance_portal(Creature* creature) : CreatureAI(creature) { }

    void OnSpellClick(Unit* clicker, bool& /*result*/) override
    {
        if (InstanceScript* instance = me->GetInstanceScript())
            if (instance->GetBossState(DATA_SLABHIDE) != DONE)
                return;

        if (me->GetEntry() == 51396)
            clicker->NearTeleportTo(1313.197f, 1236.351f, 246.957f, 4.733236f, false);
        else if (me->GetEntry() == 51397)
            clicker->NearTeleportTo(853.575f, 999.710f, 317.326f, 4.591864f, false);
    }

    void UpdateAI(uint32 /*diff*/) override { }
};

// Stonecore Flayer 42808
struct npc_stonecore_flayer : public ScriptedAI
{
    npc_stonecore_flayer(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    TaskScheduler scheduler;
    uint64 targetGUID;

    void Reset() override
    {
        events.Reset();
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_FLAY, urand(4.5 * IN_MILLISECONDS, 9 * IN_MILLISECONDS));
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
            if (eventId == EVENT_FLAY)
            {
                if (Unit* target = me->getVictim())
                {
                    targetGUID = target->GetGUID();
                    me->PrepareChanneledCast(me->GetAngle(target), SPELL_FLAY);
                }

                scheduler
                    .SetValidator([this] { return me->isInCombat(); })
                    .Schedule(Milliseconds(6000), [this](TaskContext context)
                {
                    me->RemoveChanneledCast(targetGUID);
                });

                events.ScheduleEvent(EVENT_FLAY, urand(9.5 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

struct cultist_baseAI : public ScriptedAI
{
    cultist_baseAI(Creature* creature) : ScriptedAI(creature), summons(me) { }

    EventMap events;
    SummonList summons;
    bool hasMight;

    void Reset() override
    {
        events.Reset();
        hasMight = false;
    }

    void MovementInform(uint32 type, uint32 pointId) override
    {
        if (pointId == EVENT_JUMP)
            me->DespawnOrUnsummon();
    }
};

// Stonecore Warbringer 42696
struct npc_stonecore_warbringer : public cultist_baseAI
{
    npc_stonecore_warbringer(Creature* creature) : cultist_baseAI(creature) { }

    void Reset() override
    {
        events.Reset();
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_CLEAVE, urand(4.5 * IN_MILLISECONDS, 9 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_RAGE, urand(10 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
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
                case EVENT_CLEAVE:
                    if (Unit* target = me->getVictim())
                        DoCast(target, SPELL_CLEAVE);

                    events.ScheduleEvent(EVENT_CLEAVE, urand(4.5 * IN_MILLISECONDS, 9 * IN_MILLISECONDS));
                    break;
                case EVENT_RAGE:
                    DoCast(me, SPELL_RAGE);
                    events.ScheduleEvent(EVENT_RAGE, 10 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Stonecore Earthshaper 43537
struct npc_stonecore_earthshaper : public cultist_baseAI
{
    npc_stonecore_earthshaper(Creature* creature) : cultist_baseAI(creature) { }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_FORCE_OF_EARTH, 15 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_LAVA_BURST, urand(10 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_GROUND_SHOCK, urand(1.5 * IN_MILLISECONDS, 2.5 * IN_MILLISECONDS));
    }

    void DoAction(int32 actionId) override
    {
        if (actionId == ACTION_START_INTRO)
        {
            hasMight = true;
            events.ScheduleEvent(EVENT_DUST_STORM, 2 * IN_MILLISECONDS);
        }
    }

    void AttackStart(Unit* target) override
    {
        if (!target)
            return;

        if (me->Attack(target, false))
            DoStartNoMovement(target);
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
                case EVENT_LAVA_BURST:
                    if (Unit* target = me->getVictim())
                        DoCast(target, IsHeroic() ? H_SPELL_LAVA_BURST : SPELL_LAVA_BURST);

                    events.ScheduleEvent(EVENT_LAVA_BURST, urand(10 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                    break;
                case EVENT_GROUND_SHOCK:
                    me->StopMoving();

                    if (Unit* vict = me->getVictim())
                    {
                        if (me->GetExactDist2d(vict) > 45.0f)
                            DoStartMovement(vict);
                        else
                            DoStartNoMovement(vict);
                    }

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankSpecTargetSelector()))
                        DoCast(target, IsHeroic() ? H_SPELL_GROUND_SHOCK : SPELL_GROUND_SHOCK);
                    else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f,true))
                        DoCast(target, IsHeroic() ? H_SPELL_GROUND_SHOCK : SPELL_GROUND_SHOCK);

                    events.ScheduleEvent(EVENT_GROUND_SHOCK, urand(2.5 * IN_MILLISECONDS, 3 * IN_MILLISECONDS));
                    break;
                case EVENT_FORCE_OF_EARTH:
                    if (hasMight)
                        break;

                    DoCast(me, SPELL_FORCE_OF_EARTH);
                    events.ScheduleEvent(EVENT_FORCE_OF_EARTH, 10 * IN_MILLISECONDS); // in interrupt case
                    break;
                case EVENT_DUST_STORM:
                    DoCast(me, SPELL_DUST_STORM);
                    events.ScheduleEvent(EVENT_DUST_STORM, 13.5 * IN_MILLISECONDS); // in interrupt case
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Stonecore Berserker 43430
struct npc_stonecore_berserker : public cultist_baseAI
{
    npc_stonecore_berserker(Creature* creature) : cultist_baseAI(creature) { }

    void EnterCombat(Unit* who) override
    {
        DoCast(who, SPELL_SCHARGE);
        events.ScheduleEvent(EVENT_SCHARGE, 10 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_SPINNING_SLASH, urand(5.5 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
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
                case EVENT_SCHARGE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, CasterSpecTargetSelector()))
                        DoCast(target, SPELL_SCHARGE);

                    events.ScheduleEvent(EVENT_SCHARGE, 10 * IN_MILLISECONDS);
                    break;
                case EVENT_SPINNING_SLASH:
                    DoCast(me, SPELL_SPINNING_SLASH);
                    events.ScheduleEvent(EVENT_SPINNING_SLASH, urand(5.5 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Stonecore Sentry 42695
struct npc_stonecore_sentry : public ScriptedAI
{
    npc_stonecore_sentry(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 targetGUID;

    void Reset() override
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
        targetGUID = 0;
    }

    void EnterCombat(Unit* who) override
    {
        if (who)
            targetGUID = who->GetGUID();

        // Find near warbringer and move to call help
        if (Creature* warbringer = me->FindNearestCreature(NPC_STONECORE_WARBRINGER, 50.0f, true))
        {
            Movement::MoveSplineInit init{ me };
            init.MoveTo(warbringer->GetPositionX() + frand(-5.0f, 5.0f), warbringer->GetPositionY() + frand(-3.0f, 3.0f), warbringer->GetPositionZ());
            init.Launch();

            scheduler
                .SetValidator([this] { return me->isInCombat(); })
                .Schedule(Milliseconds(me->GetSplineDuration()), [this](TaskContext context)
            {
                me->CallForHelp(15.0f);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                me->RemoveChanneledCast(targetGUID);
            });
        }
        else
            me->RemoveChanneledCast(targetGUID);
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

// Stonecore Magmalord 42789
struct npc_stonecore_magmalord : public ScriptedAI
{
    npc_stonecore_magmalord(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_IGNITE, urand(1.5 * IN_MILLISECONDS, 3.5 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_MAGMA_ERUPTION, urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
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
                case EVENT_MAGMA_ERUPTION:
                    DoCast(me, SPELL_MAGMA_ERUPTION);
                    events.ScheduleEvent(EVENT_MAGMA_ERUPTION, 13.5 * IN_MILLISECONDS);
                    break;
                case EVENT_IGNITE:
                    if (Unit* target = me->getVictim())
                        DoCast(target, SPELL_IGNITE);

                    events.ScheduleEvent(EVENT_IGNITE, urand(4 * IN_MILLISECONDS, 6 * IN_MILLISECONDS));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Stonecore Rift Conjurer 42691
struct npc_stonecore_rift_conjurer : public ScriptedAI
{
    npc_stonecore_rift_conjurer(Creature* creature) : ScriptedAI(creature), summons(me) { }

    EventMap events;
    SummonList summons;

    void Reset() override
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_DEMON_PORTAL, 12 * IN_MILLISECONDS);
        events.ScheduleEvent(EVENT_SHADOWBOLT, urand(1.5 * IN_MILLISECONDS, 2.5 * IN_MILLISECONDS));
    }

    void EnterEvadeMode() override
    {
        summons.DespawnAll();
    }

    void JustSummoned(Creature* summon) override
    {
        if (summon)
            summons.Summon(summon);
    }

    void AttackStart(Unit* target) override
    {
        if (!target)
            return;

        if (me->Attack(target, false))
            DoStartNoMovement(target);
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
                case EVENT_SHADOWBOLT:
                    me->StopMoving();
                
                    if (Unit* vict = me->getVictim())
                    {
                        if (me->GetExactDist2d(vict) > 45.0f)
                            DoStartMovement(vict);
                        else
                            DoStartNoMovement(vict);
                    }
                
                    if (Unit* target = me->getVictim())
                        DoCast(target, IsHeroic() ? H_SPELL_SHADOWBOLT : SPELL_SHADOW_BOLT);
                
                    events.ScheduleEvent(EVENT_SHADOWBOLT, urand(2.5 * IN_MILLISECONDS, 3 * IN_MILLISECONDS));
                    break;
                case EVENT_DEMON_PORTAL:
                    DoCast(me, SPELL_DEMON_PORTAL);
                    events.ScheduleEvent(EVENT_DEMON_PORTAL, 15 * IN_MILLISECONDS);
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Stonecore Bruiser 42692
struct npc_stonecore_bruiser : public ScriptedAI
{
    npc_stonecore_bruiser(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    TaskScheduler scheduler;
    uint64 targetGUID;

    void Reset() override
    {
        events.Reset();
        targetGUID = 0;
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_SHOCKWAVE, urand(6.5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_BODY_SLAM, urand(8 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
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
                case EVENT_SHOCKWAVE:
                    if (Unit* target = me->getVictim())
                    {
                        targetGUID = 0;
                        me->PrepareChanneledCast(me->GetAngle(target), SPELL_SHOCKWAVE);

                        scheduler
                            .SetValidator([this] { return me->isInCombat(); })
                            .Schedule(Milliseconds(1500), [this](TaskContext context)
                        {
                            me->RemoveChanneledCast(targetGUID);
                        });
                    }

                    events.ScheduleEvent(EVENT_SHOCKWAVE, urand(6.5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                    break;
                case EVENT_BODY_SLAM:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, CasterSpecTargetSelector()))
                        DoCast(target, SPELL_BODY_SLAM);
                    else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                        DoCast(target, SPELL_BODY_SLAM);

                    events.ScheduleEvent(EVENT_BODY_SLAM, urand(8 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Force of Earth 81459
class spell_stonecore_force_of_earth : public SpellScript
{
    PrepareSpellScript(spell_stonecore_force_of_earth);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (Creature* caster = GetCaster()->ToCreature())
            caster->AI()->DoAction(ACTION_START_INTRO);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_stonecore_force_of_earth::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// Magma Eruption Eff 80039
class spell_stonecore_magma_eruption_eff : public SpellScript
{
    PrepareSpellScript(spell_stonecore_magma_eruption_eff);

    void SelectTargets(SpellDestination& dest)
    {
        Position newPos = { dest._position.GetPositionX(), dest._position.GetPositionY(), GetCaster()->GetPositionZ(), 0 };
        dest.Relocate(newPos);
    }

    void Register() override
    {
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_stonecore_magma_eruption_eff::SelectTargets, EFFECT_0, TARGET_DEST_CASTER_RANDOM);
    }
};

// Quake 81008, 92631
class spell_stonecore_quake : public SpellScript
{
    PrepareSpellScript(spell_stonecore_quake);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        // In Jump
        targets.remove_if([=](WorldObject* target) { return target && target->ToPlayer() && target->ToPlayer()->HasUnitMovementFlag(MOVEMENTFLAG_FALLING); });
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_stonecore_quake::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// AreaTrigger 6076
class AreaTrigger_at_behind_corborus : public AreaTriggerScript
{
    public:
        AreaTrigger_at_behind_corborus() : AreaTriggerScript("AreaTrigger_at_behind_corborus") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) override
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                instance->SetData(DATA_CORBORUS, SPECIAL);

            return false;
        }
};

void AddSC_the_stonecore()
{
    new creature_script<npc_crystalspawn_giant>("npc_crystalspawn_giant");
    new creature_script<npc_impp>("npc_impp");
    new creature_script<npc_sc_millhouse_manastorm>("npc_sc_millhouse_manastorm");
    new creature_script<npc_stonecore_instance_portal>("npc_stonecore_instance_portal");
    new creature_script<npc_stonecore_flayer>("npc_stonecore_flayer");
    new creature_script<npc_stonecore_warbringer>("npc_stonecore_warbringer");
    new creature_script<npc_stonecore_earthshaper>("npc_stonecore_earthshaper");
    new creature_script<npc_stonecore_berserker>("npc_stonecore_berserker");
    new creature_script<npc_stonecore_sentry>("npc_stonecore_sentry");
    new creature_script<npc_stonecore_magmalord>("npc_stonecore_magmalord");
    new creature_script<npc_stonecore_rift_conjurer>("npc_stonecore_rift_conjurer");
    new creature_script<npc_stonecore_bruiser>("npc_stonecore_bruiser");
    new spell_script<spell_stonecore_force_of_earth>("spell_stonecore_force_of_earth");
    new spell_script<spell_stonecore_magma_eruption_eff>("spell_stonecore_magma_eruption_eff");
    new spell_script<spell_stonecore_quake>("spell_stonecore_quake");
    new AreaTrigger_at_behind_corborus();
}
