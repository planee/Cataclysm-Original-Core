
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "deadmines.h"

enum Spells
{
    SPELL_CHEST_BOMB        = 88352,
    SPELL_CHEST_BOMB_EXPLOSION        = 88250,
    SPELL_EXPLODE           = 91566,

    SPELL_OAF_GRAB_TARGETING = 88289, // DUMMY bp SPELL_LUMBERING_CHARGE
    SPELL_LUMBERING_CHARGE            = 88288, // trigger SPELL_LUMBERING_FORCE_RIDE
    SPELL_LUMBERING_FORCE_RIDE        = 88278,

    SPELL_LUMBERING_CHARGE_DEST = 88295,

    SPELL_LUMBERING_CATCH_PLAYER = 88277, // cast by script effec
    SPELL_LUMBERING_CRASH_PLAYER = 88300,

    SPELL_HELIX_LUMBERING_PROTECTION = 90546,

    SPELL_HELIX_RIDE_FACE_AURA = 88351,
    SPELL_HELIX_BOMB_EMOTE = 91572,

    SPELL_NIGHTMARE_AURA = 92566,
    SPELL_HELIX_CANCEL_NIGHTMARE_AURA = 92585,

    // helix part


    SPELL_HELIX_LAUNCH_DUMMY = 88126,
    SPELL_HELIX_SET_VEHICLE = 88337,
    SPELL_HELIX_RIDE_FACING_TARGET = 88349, // dummy SPELL_HELIX_RIDE_VEHICLE
    SPELL_HELIX_RIDE_VEHICLE = 88360,
    SPELL_HELIX_RIDE_FACE_TIMER_AURA = 88351,

    SPELL_HELIX_BOMB_TARGET_SELECTOR = 88268, // dummy bp SPELL_HELIX_LAUNCH_BOMB
    SPELL_HELIX_LAUNCH_BOMB = 88264, // trigger SPELL_HELIC_SUMMON_BOMB
    SPELL_HELIX_SUMMON_BOMB = 88265,

    SPELL_HELIX_TALK_EMOTE = 79506,

    SPELL_HELIX_BOMB_ACTIVATION_VISUAL_YELLOW = 88315,
    SPELL_HELIX_BOMB_ACTIVATION_VISUAL_ORANGE = 88316,
    SPELL_HELIX_BOMB_ACTIVATION_VISUAL_RED    = 88317,
    SPELL_BOMB_ACTIVATE                       = 88319,
    SPELL_BOMB_PERIODIC_TRIGGER               = 88329, // trigger SPELL_HELIX_BOMB_EXPLODE
    SPELL_HELIX_BOMB_EXPLODE                  = 88321,
};

enum Events
{
    EVENT_CHEST_BOMB    = 1,
    EVENT_OAF_SMASH     = 2,
    EVENT_CHARGE_PLAYER        = 3,
    EVENT_HELIX_BOMB,
    EVENT_RIDE_PLAYERS,
    EVENT_RIDE_PLAYERS_END,
    EVENT_HELIX_TEAM,
    EVENT_START_CHARGE,
};

enum NPCs
{
    NPC_HELIX_CREW          = 49136,
    NPC_LUMBERING           = 47297,
    NPC_HELIX_BOMB          = 47314,
    NPC_MINE_RAT            = 51462,
    NPC_HELIX_TEAM          = 49139,
};

enum Phases
{
    PHASE_LUMBERING = 1,
    PHASE_HELIX = 2,

    POINT_CHARGE_WITH_PLAYER = 3,
    POINT_START_CHARGE = 4,
    ACTION_RIDE_END = 5,
    ACTION_RESTART_COMBAT = 6,
};

static const Position chargeEvent[2] =
{
    { -289.808f, -527.215f, 49.802f, 1.6231f }, // charge start
    { -289.587f, -489.575f, 49.912f, 4.7123f }  // charge dest

};

class BombActivationEvent : public BasicEvent
{
public:
    BombActivationEvent(Creature* owner, uint8 count) : _owner(owner), _count(count) { }

    bool Execute(uint64 execTime, uint32 /*diff*/) override
    {
        switch (_count)
        {
            case 0:
            case 1:
                _owner->CastSpell(_owner, SPELL_HELIX_BOMB_ACTIVATION_VISUAL_YELLOW, true);
                break;
            case 2:
            case 3:
                _owner->CastSpell(_owner, SPELL_HELIX_BOMB_ACTIVATION_VISUAL_ORANGE, true);
                break;
            case 4:
            case 5:
                _owner->CastSpell(_owner, SPELL_HELIX_BOMB_ACTIVATION_VISUAL_RED, true);
                break;
            case 6:
                _owner->CastSpell(_owner, SPELL_BOMB_ACTIVATE, true);
                _owner->CastSpell(_owner, SPELL_BOMB_PERIODIC_TRIGGER, true);
                break;
            case 19:
                _owner->CastSpell(_owner, SPELL_HELIX_BOMB_EXPLODE, true);
                _owner->DespawnOrUnsummon(1000);
                return true;
            default:
                break;
        }

        ++_count;
        _owner->m_Events.AddEvent(this, execTime + 1000);
        return false;
    }

private:
    Creature* _owner;
    uint8 _count;
};

class TeamBombLaunchEvent : public BasicEvent
{
public:
    TeamBombLaunchEvent(Creature* owner) : _owner(owner) { }

    bool Execute(uint64 execTime, uint32 /*diff*/) override
    {
        if (_owner && _owner->isAlive())
        {
            _owner->AI()->Talk(0);
            _owner->CastSpell(_owner, SPELL_HELIX_BOMB_TARGET_SELECTOR, true);
            _owner->m_Events.AddEvent(this, execTime + urand(6000, 10000));
        }
        return false;
    }

private:
    Creature* _owner;
};

class boss_helix_gearbreaker : public CreatureScript
{
public:
    boss_helix_gearbreaker() : CreatureScript("boss_helix_gearbreaker") { }

    struct boss_helix_gearbreakerAI : public BossAI
    {
        boss_helix_gearbreakerAI(Creature* creature) : BossAI(creature, BOSS_HELIX_DATA) { }

        void Reset()
        {
            targetGUID = 0;
            isOnPlayer = false;
            _Reset();
            InitVehicle();
        }

        void InitVehicle()
        {
            if (Vehicle *veh = me->GetVehicle())
                if (Unit *baseVeh = veh->GetBase())
                    if (baseVeh->GetTypeId() == TYPEID_UNIT)
                        baseVeh->ToCreature()->DespawnOrUnsummon();
            if (Creature *lumbering = me->SummonCreature(NPC_LUMBERING, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation()))
                me->CastCustomSpell(VEHICLE_SPELL_RIDE_HARDCODED, SPELLVALUE_BASE_POINT0, 1, lumbering, false);
        }

        void EnterCombat(Unit* who)
        {
            Talk(0);
            if (Vehicle *veh = me->GetVehicle())
                if (Unit *baseVeh = veh->GetBase())
                    if (baseVeh->GetTypeId() == TYPEID_UNIT)
                    {
                        baseVeh->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                        baseVeh->ToCreature()->SetInCombatWithZone();
                    }
            events.SetPhase(PHASE_LUMBERING);
            events.ScheduleEvent(EVENT_HELIX_BOMB, 5000, 0, PHASE_LUMBERING);
            events.ScheduleEvent(EVENT_CHARGE_PLAYER, 15000, 0, PHASE_LUMBERING);
            events.ScheduleEvent(EVENT_RIDE_PLAYERS, 25000, 0, PHASE_LUMBERING);
            if (IsHeroic())
                events.ScheduleEvent(EVENT_HELIX_TEAM, 30000);
            _EnterCombat();
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if (summon->GetEntry() == NPC_LUMBERING)
            {
                Talk(2);
                events.SetPhase(PHASE_HELIX);
                events.ScheduleEvent(EVENT_RIDE_PLAYERS, 5000, 0, PHASE_HELIX);
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(4);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_HELIX_SET_VEHICLE);
            _JustDied();
        }

        void KilledUnit(Unit* victim)
        {

        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (events.IsInPhase(PHASE_LUMBERING))
                damage = 0;
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {

        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_RIDE_END:
                {
                    if (me->GetVehicle())
                        if (me->GetVehicle()->GetBase())
                            targetGUID = me->GetVehicle()->GetBase()->GetGUID();
                    me->ExitVehicle();
                    if (events.IsInPhase(PHASE_LUMBERING))
                        if (Creature *lumbering = me->FindNearestCreature(NPC_LUMBERING, 200, true))
                            me->CastCustomSpell(VEHICLE_SPELL_RIDE_HARDCODED, SPELLVALUE_BASE_POINT0, 1, lumbering, false);
                    if (IsHeroic())
                        events.ScheduleEvent(EVENT_CHEST_BOMB, 100);
                    break;
                }
            }
        }

        void JustSummoned(Creature* summon)
        {
            switch (summon->GetEntry())
            {
                case NPC_HELIX_BOMB:
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_DISABLE_MOVE);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->m_Events.AddEvent(new BombActivationEvent(summon, 0), summon->m_Events.CalculateTime(1));
                    break;
                case NPC_HELIX_TEAM:
                    summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_DISABLE_MOVE);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->m_Events.AddEvent(new TeamBombLaunchEvent(summon), summon->m_Events.CalculateTime(urand(10000, 15000)));
                    break;
            }
            BossAI::JustSummoned(summon);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;
        }


        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_HELIX_BOMB:
                        Talk(1);
                        DoCast(SPELL_HELIX_BOMB_TARGET_SELECTOR);
                        events.ScheduleEvent(EVENT_HELIX_BOMB, 10000, 0, PHASE_LUMBERING);
                        break;
                    case EVENT_CHEST_BOMB:
                    {
                        if (Unit *target = Unit::GetUnit(*me, targetGUID))
                        {
                            Talk(3, targetGUID);
                            me->CastSpell(target, SPELL_CHEST_BOMB, true);
                        }
                        break;
                    }
                    case EVENT_OAF_SMASH:
                        break;
                    case EVENT_CHARGE_PLAYER:
                        DoCastRandom(SPELL_OAF_GRAB_TARGETING, 150.0f, false);
                        events.ScheduleEvent(EVENT_CHARGE_PLAYER, 20000);
                        break;
                    case EVENT_RIDE_PLAYERS:
                        DoCast(SPELL_HELIX_SET_VEHICLE);
                        DoCast(SPELL_HELIX_RIDE_FACING_TARGET);
                        isOnPlayer = true;
                        if (events.IsInPhase(PHASE_LUMBERING))
                            events.ScheduleEvent(EVENT_RIDE_PLAYERS, 60000, 0, PHASE_LUMBERING);
                        else
                            events.ScheduleEvent(EVENT_RIDE_PLAYERS, 12000, 0, PHASE_HELIX);
                        events.ScheduleEvent(EVENT_RIDE_PLAYERS_END, 10000);
                        break;
                    case EVENT_RIDE_PLAYERS_END:
                        isOnPlayer = false;
                        break;
                    case EVENT_HELIX_TEAM:
                        me->SummonCreature(NPC_HELIX_TEAM, -291.08f, -504.09f, 60.21f, 4.60f);
                        me->SummonCreature(NPC_HELIX_TEAM, -292.68f, -503.73f, 60.20f, 4.47f);
                        me->SummonCreature(NPC_HELIX_TEAM, -287.47f, -503.27f, 60.38f, 1.67f);
                        me->SummonCreature(NPC_HELIX_TEAM, -283.84f, -503.37f, 60.44f, 1.90f);
                        events.ScheduleEvent(EVENT_HELIX_TEAM, 60000);
                        break;
                }
            }

            if (!events.IsInPhase(PHASE_LUMBERING) || isOnPlayer)
                DoMeleeAttackIfReady();
        }

    private:
        uint64 targetGUID;
        bool isOnPlayer;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_helix_gearbreakerAI(creature);
    }
};


class boss_helix_lumbering : public CreatureScript
{
public:
    boss_helix_lumbering() : CreatureScript("boss_helix_lumbering") { }

    struct boss_helix_lumberingAI : public ScriptedAI
    {
        boss_helix_lumberingAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            me->SetReactState(REACT_AGGRESSIVE);
            _events.Reset();
        }

        void EnterEvadeMode()
        {
            if (me->GetVehicleKit())
                me->GetVehicleKit()->RemoveAllPassengers();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            if (id == POINT_START_CHARGE)
                _events.ScheduleEvent(EVENT_START_CHARGE, 1);
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_RESTART_COMBAT:
                {
                    if (me->getVictim())
                    {
                        DoStartMovement(me->getVictim());
                        for (int i = 0; i < 5; i++)
                            if (Creature *rat = me->SummonCreature(NPC_MINE_RAT, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation()))
                                rat->AI()->AttackStart(me->getVictim());
                    }
                    break;
                }
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_CHARGE:
                        DoCast(SPELL_LUMBERING_CHARGE_DEST);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_helix_lumberingAI(creature);
    }
};

class BombTargetSelector
{
public:
    explicit BombTargetSelector(Unit* unit) : _oaf(unit){};

    bool operator()(WorldObject* target) const
    {
        if (_oaf && _oaf->getVictim() == target) // phase 1 check
            return true;
        return !target->ToUnit() || target->ToUnit()->HasAura(SPELL_LUMBERING_CATCH_PLAYER);
    }

private:
    Unit const* _oaf;
};

// 88268
class spell_helix_bomb_dummy : public SpellScriptLoader
{
public:
    spell_helix_bomb_dummy() : SpellScriptLoader("spell_helix_bomb_dummy") { }

    class spell_helix_bomb_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_helix_bomb_dummy_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit *caster = GetCaster())
            {
                Unit *oaf = NULL;
                if (Vehicle *veh = caster->GetVehicle())
                    oaf = veh->GetBase();
                targets.remove_if(BombTargetSelector(oaf));
            }
            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, uint32(GetEffectValue()), true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_helix_bomb_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_helix_bomb_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_helix_bomb_dummy_SpellScript();
    }
};

// 88349
class spell_helix_ride_face_dummy : public SpellScriptLoader
{
public:
    spell_helix_ride_face_dummy() : SpellScriptLoader("spell_helix_ride_face_dummy") { }

    class spell_helix_ride_face_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_helix_ride_face_dummy_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit *caster = GetCaster())
            {
                Unit *oaf = NULL;
                if (Vehicle *veh = caster->GetVehicle())
                    oaf = veh->GetBase();
                targets.remove_if(BombTargetSelector(oaf));
            }
            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (Unit *caster = GetCaster())
                if (Unit *target = GetHitUnit())
                {
                    caster->CastSpell(target, SPELL_HELIX_RIDE_FACE_TIMER_AURA, true);
                    uint32 spellId = GetSpellInfo()->Effects[effIndex].CalcValue();
                    caster->CastSpell(target, spellId, true);
                }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_helix_ride_face_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_helix_ride_face_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_helix_ride_face_dummy_SpellScript();
    }
};

// 88351
class spell_helix_ride_timer : public SpellScriptLoader
{
public:
    spell_helix_ride_timer() : SpellScriptLoader("spell_helix_ride_timer") { }

    class spell_helix_ride_timer_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_helix_ride_timer_AuraScript);

        void Remove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit *caster = GetCaster())
                if (caster->GetTypeId() == TYPEID_UNIT)
                    caster->ToCreature()->AI()->DoAction(ACTION_RIDE_END);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_helix_ride_timer_AuraScript::Remove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_helix_ride_timer_AuraScript();
    }
};

// 88352
class spell_chess_bomb_periodic : public SpellScriptLoader
{
public:
    spell_chess_bomb_periodic() : SpellScriptLoader("spell_chess_bomb_periodic") { }

    class spell_chess_bomb_periodic_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_chess_bomb_periodic_AuraScript);

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            return true;
        }

        void HandleEffectPeriodic(AuraEffect const* aurEff)
        {
            if (Unit* target = GetUnitOwner())
                target->CastSpell(target, SPELL_CHEST_BOMB_EXPLOSION, true);
        }


        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_chess_bomb_periodic_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_chess_bomb_periodic_AuraScript();
    }
};

// 88289
class spell_oaf_grab_targeting : public SpellScriptLoader
{
public:
    spell_oaf_grab_targeting() : SpellScriptLoader("spell_oaf_grab_targeting") { }

    class spell_oaf_grab_targeting_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_oaf_grab_targeting_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit *caster = GetCaster())
            {
                Unit *oaf = NULL;
                if (Vehicle *veh = caster->GetVehicle())
                    oaf = veh->GetBase();
                targets.remove_if(BombTargetSelector(oaf));
            }
            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (Unit *caster = GetCaster())
                if (Unit *target = GetHitUnit())
                    if (Vehicle *veh = caster->GetVehicle())
                        if (Unit *oaf = veh->GetBase())
                        {
                            if (oaf->ToCreature())
                                oaf->ToCreature()->AI()->Talk(0);
                            uint32 spellId = GetSpellInfo()->Effects[effIndex].CalcValue();
                            oaf->CastSpell(target, spellId, true);
                        }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_oaf_grab_targeting_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_oaf_grab_targeting_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_oaf_grab_targeting_SpellScript();
    }
};

// 88278
class spell_oaf_force_players_ride : public SpellScriptLoader
{
public:
    spell_oaf_force_players_ride() : SpellScriptLoader("spell_oaf_force_players_ride") { }

    class spell_oaf_force_players_ride_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_oaf_force_players_ride_SpellScript);

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            return true;
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            if (Unit *caster = GetCaster())
                if (Unit *target = GetHitUnit())
                {
                    uint32 spellId = GetSpellInfo()->Effects[effIndex].CalcValue();
                    target->CastSpell(caster, spellId, true);
                    if (Creature *oaf = caster->ToCreature())
                        oaf->GetMotionMaster()->MovePoint(POINT_START_CHARGE, -290.09f, -530.12f, 49.60f);
                }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_oaf_force_players_ride_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_oaf_force_players_ride_SpellScript();
    }
};

// 88300
class spell_oaf_crash_players : public SpellScriptLoader
{
public:
    spell_oaf_crash_players() : SpellScriptLoader("spell_oaf_crash_players") { }

    class spell_oaf_crash_players_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_oaf_crash_players_SpellScript);

        void HandleAfterHit()
        {
            if (Unit *caster = GetCaster())
                if (caster->GetTypeId() == TYPEID_UNIT)
                    caster->ToCreature()->AI()->DoAction(ACTION_RESTART_COMBAT);
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_oaf_crash_players_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_oaf_crash_players_SpellScript();
    }
};

class spell_oaf_bomb_trigger_aura : public SpellScriptLoader
{
public:
    spell_oaf_bomb_trigger_aura() : SpellScriptLoader("spell_oaf_bomb_trigger_aura") { }

    class spell_oaf_bomb_trigger_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_oaf_bomb_trigger_aura_AuraScript);

        bool Load()
        {
            hit = false;
            return true;
        }

        void HandlePeriodic(AuraEffect const* /*aurEff*/)
        {
            if (Unit *caster = GetCaster())
            {
                if (!caster->FindNearestPlayer(1) || hit)
                    PreventDefaultAction();
                else if (caster->GetTypeId() == TYPEID_UNIT)
                {
                    hit = true;
                    caster->ToCreature()->DespawnOrUnsummon(1000);
                }
            }
        }

    private :
        bool hit;

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_oaf_bomb_trigger_aura_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_oaf_bomb_trigger_aura_AuraScript();
    }
};

// 47314
class npc_helix_bomb : public CreatureScript
{
public:
    npc_helix_bomb() : CreatureScript("npc_helix_bomb") {}

    struct npc_helix_bombAI : public ScriptedAI
    {
        npc_helix_bombAI(Creature * creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void Reset()
        {
        }

        void IsSummonedBy(Unit *)
        {
            if (instance)
                if (Creature *helix = Unit::GetCreature(*me, instance->GetData64(BOSS_HELIX_DATA)))
                    helix->AI()->JustSummoned(me);
        }

    private:
        InstanceScript *instance;
    };

    CreatureAI * GetAI(Creature * creature) const
    {
        return new npc_helix_bombAI(creature);
    }
};


void AddSC_boss_helix_gearbreaker()
{
    new boss_helix_gearbreaker();
    new boss_helix_lumbering();
    new spell_helix_bomb_dummy();
    new spell_helix_ride_face_dummy();
    new spell_helix_ride_timer();
    new spell_chess_bomb_periodic();
    new spell_oaf_grab_targeting();
    new spell_oaf_force_players_ride();
    new spell_oaf_crash_players();
    new spell_oaf_bomb_trigger_aura();
    new npc_helix_bomb();
}
