#include "ScriptPCH.h"
#include "blackrock_caverns.h"

enum eTalks
{
    TALK_INTRO     = 0,
};

enum Events
{
    EVENT_CLEAVE = 1,
    EVENT_BREAK_PRISON,
    EVENT_MOVE,
    EVENT_IMMOLATE,
    EVENT_HEAT_EXCAUSTION,
};

enum Spells
{
    SPELL_EVIL_CLEAVE     = 80606,
    SPELL_SHADOW_PRISON   = 79725,
    SPELL_FURIOUS_RAGE    = 80218,
    SPELL_IMMOLATE        = 76618,
    SPELL_BURNING_HEAT    = 75077,
    SPELL_HEAT_EXCAUSTION = 76617,
};

// Raz the Crazed 39670
struct npc_raz_the_crazed : public ScriptedAI
{
    npc_raz_the_crazed(Creature* creature) : ScriptedAI(creature) { }

    EventMap events, nonCombatEvents;
    uint32 wp;
    bool SecondEvent;

    void InitializeAI() override
    {
        wp = 0;
        SecondEvent = false;

        if (me->GetDBTableGUIDLow())
            DoCast(me, SPELL_SHADOW_PRISON);
    }

    void Reset() override
    {
        events.Reset();
        nonCombatEvents.Reset();

        if (me->GetDBTableGUIDLow())
            nonCombatEvents.ScheduleEvent(EVENT_MOVE, 1500);
    }

    void IsSummonedBy(Unit* summoner) override
    {
        me->GetMotionMaster()->MoveJump(lastStandByKaz[1], 20.0f, 15.0f, EVENT_JUMP + 4);
    }

    void DoAction(int32 actionId) override
    {
        switch (actionId)
        {
            case ACTION_RAZ_BREAK_PRISON:
                me->SetAnimationTier(UnitAnimationTier::Fly);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->SetHover(true);
                nonCombatEvents.ScheduleEvent(EVENT_BREAK_PRISON, 2.5 * IN_MILLISECONDS);
                break;
            case ACTION_RAZ_NEXT_BRIDGE:
                if (SecondEvent)
                    break;
            
                wp = 0;
                SecondEvent = true;
                me->GetMotionMaster()->MoveJump(RazSecondBridgePath[wp], 25.0f, 20.0f, EVENT_JUMP + 2);
                break;
        }
    }

    void EnterCombat(Unit* /*who*/) override
    {
        events.ScheduleEvent(EVENT_CLEAVE, 0.5 * IN_MILLISECONDS);
    }

    void MovementInform(uint32 type, uint32 pointId) override
    {
        switch (pointId)
        {
            case EVENT_JUMP:
                me->SetHomePosition(*me);
                me->setFaction(1665);
                Talk(TALK_INTRO);
                me->SetAnimationTier(UnitAnimationTier::Ground);
                me->SetCanFly(false);
                me->SetDisableGravity(false);
                me->SetHover(false);
                nonCombatEvents.ScheduleEvent(EVENT_MOVE, 2 * IN_MILLISECONDS);
                break;
            case EVENT_JUMP + 1:
                me->GetMotionMaster()->MovementExpired();
                me->GetMotionMaster()->Clear();
                break;
            case EVENT_JUMP + 2:
                me->SetHomePosition(*me);
                Talk(TALK_INTRO);
                nonCombatEvents.ScheduleEvent(EVENT_MOVE, 2 * IN_MILLISECONDS);
                break;
            case EVENT_JUMP + 3:
                me->DespawnOrUnsummon(1000);
                break;
            case EVENT_JUMP + 4:
                me->setFaction(1665);
                break;
        }

        if (type != POINT_MOTION_TYPE)
            return;

        wp++;

        switch (pointId)
        {
            case 7:
                me->GetMotionMaster()->MovementExpired();
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MoveJump(RazFirstBridgePath[wp], 25.0f, 20.0f, EVENT_JUMP + 1);
                break;
            case 3:
                if (SecondEvent)
                {
                    me->setFaction(35);
                    me->GetMotionMaster()->MovementExpired();
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveJump(RazSecondBridgePath[wp], 45.0f, 45.0f, EVENT_JUMP + 3);
                }
                else
                    nonCombatEvents.ScheduleEvent(EVENT_MOVE, urand(100, 200));
                break;
            default:
                nonCombatEvents.ScheduleEvent(EVENT_MOVE, urand(100, 200));
            break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        nonCombatEvents.Update(diff);

        while (uint32 eventId = nonCombatEvents.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_BREAK_PRISON:
                    me->RemoveAurasDueToSpell(SPELL_SHADOW_PRISON);
                    me->GetMotionMaster()->MoveJump(RazFirstBridgePath[0], 25.0f, 20.0f, EVENT_JUMP);
                    break;
                case EVENT_MOVE:
                    if (me->isInCombat())
                        return;
                    me->GetMotionMaster()->MovementExpired();
                    me->GetMotionMaster()->Clear();
                
                    if (me->GetUnitMovementFlags() == MOVEMENTFLAG_WALKING)
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                
                    me->GetMotionMaster()->MovePoint(wp, SecondEvent ? RazSecondBridgePath[wp] : RazFirstBridgePath[wp]);
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId = EVENT_CLEAVE)
            {
                if (Unit* vict = me->getVictim())
                    DoCast(vict, SPELL_EVIL_CLEAVE, true);

                DoCast(me, SPELL_FURIOUS_RAGE, true);
                events.ScheduleEvent(EVENT_CLEAVE, urand(1.5 * IN_MILLISECONDS, 2.5 * IN_MILLISECONDS));
            }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Twilight Portal 51340
struct blackrock_caverns_teleport : public CreatureAI
{
    blackrock_caverns_teleport(Creature* creature) : CreatureAI(creature) { }

    void OnSpellClick(Unit* clicker, bool& /*result*/) override
    {
        if (InstanceScript* instance = me->GetInstanceScript())
            if (instance->GetData(DATA_KARSH) != DONE)
                return;

        if (me->GetDBTableGUIDLow() == 327777)
            clicker->NearTeleportTo(284.838f, 840.364f, 95.920f, 3.881242f, false);
        else if (me->GetDBTableGUIDLow() == 327776)
            clicker->NearTeleportTo(233.555f, 1128.875f, 205.569f, 3.551373f, false);
    }

    void UpdateAI(uint32 /*diff*/) override { }
};

// Conflaguration 39994
struct npc_karsh_conflaguration : public SmartCreatureAI
{
    npc_karsh_conflaguration(Creature* creature) : SmartCreatureAI(creature) { }

    void Reset() override
    {
        events.Reset();
        me->SetReactState(REACT_AGGRESSIVE);
    }

    void EnterCombat(Unit* /*who*/) override
    {
        DoCast(me, SPELL_BURNING_HEAT);
        events.ScheduleEvent(EVENT_IMMOLATE, urand(2000, 4000));
        events.ScheduleEvent(EVENT_HEAT_EXCAUSTION, 5 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        scheduler.Update(diff);
        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_IMMOLATE, urand(10 * IN_MILLISECONDS, 12 * IN_MILLISECONDS), EVENT_IMMOLATE, eventId);
            ExecuteTargetEvent(SPELL_HEAT_EXCAUSTION, urand(5 * IN_MILLISECONDS, 6 * IN_MILLISECONDS), EVENT_HEAT_EXCAUSTION, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Aggro Nearby Targets 80196
class spell_aggro_nearby_target : public SpellScript
{
    PrepareSpellScript(spell_aggro_nearby_target);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (Unit* caster = GetCaster())
            targets.remove_if([=](WorldObject* target) { return !target->ToPlayer(); });
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_aggro_nearby_target::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
    }
};

// AreaTrigger 6014
class AreaTrigger_at_second_bridge_blackrock_caverns : public AreaTriggerScript
{
    public:
        AreaTrigger_at_second_bridge_blackrock_caverns() : AreaTriggerScript("AreaTrigger_at_second_bridge_blackrock_caverns") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) override
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* Raz = ObjectAccessor::GetCreature(*player, instance->GetData64(NPC_RAZ_THE_CRAZED)))
                    Raz->AI()->DoAction(ACTION_RAZ_NEXT_BRIDGE);

            return false;
        }
};

void AddSC_blackrock_caverns()
{
    new creature_script<npc_raz_the_crazed>("npc_raz_the_crazed");
    new creature_script<blackrock_caverns_teleport>("blackrock_caverns_teleport");
    new creature_script<npc_karsh_conflaguration>("npc_karsh_conflaguration");
    new spell_script<spell_aggro_nearby_target>("spell_aggro_nearby_target");
    new AreaTrigger_at_second_bridge_blackrock_caverns();
}