#include "ScriptPCH.h"
#include "the_stonecore.h"

enum Spells
{
	SPELL_CURSE_OF_BLOOD			= 79345,
	SPELL_ARCANE_SHIELD				= 79050,
	SPELL_ENERGY_SHIELD				= 82858,
    SPELL_SUMMON_GRAVITY_WELL       = 79340,
    SPELL_GRAVITY_WELL_AURA_0       = 79245,
    SPELL_GRAVITY_WELL_AURA_1       = 79244,
    SPELL_GRAVITY_WELL_AURA_1_T     = 79251,
    SPELL_GRAVITY_WELL_AURA_MOD     = 92475,
    SPELL_GRAVITY_WELL_AURA_DMG     = 79249,
    SPELL_GRAVITY_WELL_KNOCKBACK    = 79332,
    SPELL_SUMMON_FOLOWER_EAST       = 79193,
    SPELL_SUMMON_FOLLOWER_WEST      = 79199,

    /*Force Grip Mechanic*/
    SPELL_FORCE_GRIP                = 79351,
    SPELL_FORCE_GRIP_EFF            = 79358,
    SPELL_FORCE_GRIP_EFF_HC         = 92664,
    SPELL_FORCE_GRIP_VEHICLE        = 79354,
    SPELL_SUMMON_CAMERA_BUNNY       = 79360,

    /*Second Phase*/
    SPELL_EARTH_FURY_CASTING_VISUAL = 79002,
    SPELL_SEISMIC_SHARD_SUMMON_1    = 86860,
    SPELL_SEISMIC_SHARD_SUMMON_2    = 86858,
    SPELL_SEISMIC_SHARD_SUMMON_3    = 86856,
    SPELL_SEISMIC_SHARD_VISUAL      = 79009,
    SPELL_SEISMIC_SHARD_TARGETING   = 80511,
    SPELL_SEISMIC_SHARD_MISSLE      = 79014,
    SPELL_SIESMUC_SHARD_EFF         = 79021,

    // Add wave spells
    SPELL_SUMMON_WAVE_SOUTH         = 79200,
    SPELL_SUMMON_WAVE_WEST          = 79196,
    SPELL_SUMMON_ADD_SOUTH          = 79193,
    SPELL_SUMMON_ADD_WEST           = 79199,
};

enum Events
{
    EVENT_CURSE_OF_BLOOD = 1,
    EVENT_SHIELD,
    EVENT_GRAVITY_WELL,
    EVENT_GRAVITY_WELL_1,
    EVENT_SUMMON_ADDS,
    EVENT_FORCE_GRIP,

};

enum Types
{
    TYPE_FORCE_GRIP,
    TYPE_SHARD,
};

enum Yells
{
    TALK_AGGRO,
    TALK_PHASE,
    TALK_SLAY,
    TALK_DEATH,
};

enum Adds
{
    NPC_ADVOUT_FOLLOWER     = 42428,
    NPC_GRAVITY_WELL        = 42499,
};

Position highpriestessazilAddsPos[2] = 
{
    {1263.20f, 966.03f, 205.81f, 0.0f},
    {1278.51f, 1021.72f, 209.08f, 0.0f}
};

Position highpriestessazilStandPos = {1337.79f, 963.39f, 214.18f, 1.8f};

class boss_high_priestess_azil : public CreatureScript
{
    public:
        boss_high_priestess_azil() : CreatureScript("boss_high_priestess_azil") { }

        struct boss_high_priestess_azilAI : public BossAI
        {
            boss_high_priestess_azilAI(Creature* creature) : BossAI(creature, DATA_HIGH_PRIESTESS_AZIL), summons(me)
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
            uint64 targetGUID;
            uint64 gripGUID;
            uint64 shardGUID;

            void Reset() override
            {
                _Reset();

                me->SetReactState(REACT_PASSIVE);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                summons.DespawnAll();
                events.Reset();
                targetGUID = 0;
                shardGUID  = 0;
            }

            void SetGUID(uint64 guid, int32 type) override
            {
                switch (type)
                {
                    case TYPE_SHARD:
                        shardGUID = guid;
                        break;
                    case TYPE_FORCE_GRIP:
                        gripGUID = guid;
                        break;
                }
            }

            uint64 GetGUID(int32 type) const override
            {
                switch (type)
                {
                    case TYPE_SHARD:
                        return shardGUID;
                    case TYPE_FORCE_GRIP:
                        return gripGUID;
                }

                return 0;
            }

            void JustSummoned(Creature* summon) override
            {
                summons.Summon(summon);
                switch (summon->GetEntry())
                {
                    case NPC_ADVOUT_FOLLOWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            summon->AddThreat(target, 10.0f);
                            summon->Attack(target, true);
                            summon->GetMotionMaster()->MoveChase(target);
                        }
                        break;
                }
            }

            void SummonedCreatureDespawn(Creature* summon) override
            {
                summons.Despawn(summon);
            }

            void EnterCombat(Unit* who) override
            {
                Talk(TALK_AGGRO);

                _EnterCombat();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                if (who)
                    targetGUID = who->GetGUID();

                DoCast(me, SPELL_ARCANE_SHIELD);
            }

            void KilledUnit(Unit* /*victim*/) override
            {
                Talk(TALK_SLAY);
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                summons.DespawnAll();
                Talk(TALK_DEATH);
            }

            void DoAction(int32 actionId) override
            {
                if (actionId == ACTION_AZIL_SHIELD)
                    me->GetMotionMaster()->MoveJump(azilShieldPhase[0], 5.0f, 7.5f, EVENT_JUMP);
            }

            void MovementInform(uint32 type, uint32 pointId) override
            {
                switch (pointId)
                {
                    case EVENT_JUMP:
						if (GetDifficulty() == DUNGEON_DIFFICULTY_HEROIC)
						{
							me->RemoveAurasDueToSpell(92667);
							me->RemoveAurasDueToSpell(82858);
						}
						else
						{

						}
						{
							me->RemoveAurasDueToSpell(79050);
							me->RemoveAurasDueToSpell(82858);
						}
                        me->RemoveAurasDueToSpell(SPELL_EARTH_FURY_CASTING_VISUAL);
                        me->RemoveChanneledCast(targetGUID);
                        events.ScheduleEvent(EVENT_SHIELD, 35000);
                        events.ScheduleEvent(EVENT_FORCE_GRIP, 12 * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 3000);
                        events.ScheduleEvent(EVENT_SUMMON_ADDS, urand(5000, 7000));
                        events.ScheduleEvent(EVENT_GRAVITY_WELL, 10000);
                        break;
                    case EVENT_JUMP + 1:
                        DoCast(me, SPELL_EARTH_FURY_CASTING_VISUAL, true);
                        DoCast(me, SPELL_SEISMIC_SHARD_SUMMON_1, true);
                        DoCast(me, SPELL_SEISMIC_SHARD_SUMMON_2, true);
                        DoCast(me, SPELL_SEISMIC_SHARD_SUMMON_3, true);

                        scheduler
                            .Schedule(Milliseconds(6700), [this](TaskContext context)
                        {
                            std::list<Creature*> shardsList;
                            GetCreatureListWithEntryInGrid(shardsList, me, NPC_SIESMIC_SHARD, 200.0f);
                            shardsList.remove_if([=](Creature* target) { return target && target->AI()->GetData(0); });

                            if (!shardsList.empty())
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankSpecTargetSelector()))
                                    shardGUID = target->GetGUID();
                                else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                                    shardGUID = target->GetGUID();

                                Trinity::Containers::SelectRandomContainerElement(shardsList)->AI()->DoAction(ACTION_LAUNCH_SHARD);
                                context.Repeat(Milliseconds(6700));
                            }
                            else
                                DoAction(ACTION_AZIL_SHIELD);
                        });
                        break;
                }
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
                    switch (eventId)
                    {
                        case EVENT_SUMMON_ADDS:
                            for (uint8 i = 0; i < 3; i++)
                                me->SummonCreature(NPC_ADVOUT_FOLLOWER, highpriestessazilAddsPos[urand(0,1)]);

                            events.ScheduleEvent(EVENT_SUMMON_ADDS, urand(5000, 7000));
                            break;
                        case EVENT_CURSE_OF_BLOOD:
                            DoCast(SPELL_CURSE_OF_BLOOD);
                            events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 15000);
                            break;
                        case EVENT_GRAVITY_WELL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, CasterSpecTargetSelector()))
                                DoCast(target, SPELL_SUMMON_GRAVITY_WELL);
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                                DoCast(target, SPELL_SUMMON_GRAVITY_WELL);

                            events.ScheduleEvent(EVENT_GRAVITY_WELL, 10000);
                            break;
                        case EVENT_SHIELD:
                            Talk(TALK_PHASE);
                            DoCast(me, SPELL_ENERGY_SHIELD);
                            me->PrepareChanneledCast(me->GetOrientation());
                            events.Reset();

                            scheduler
                                .Schedule(Milliseconds(2100), [this](TaskContext context)
                            {
                                me->GetMotionMaster()->MoveJump(azilShieldPhase[1], 5.0f, 7.5f, EVENT_JUMP + 1);
                            });
                            break;
                        case EVENT_FORCE_GRIP:
                            if (Unit* target = me->getVictim())
                                DoCast(target, SPELL_FORCE_GRIP);

                            events.ScheduleEvent(EVENT_FORCE_GRIP, 15 * IN_MILLISECONDS);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_high_priestess_azilAI>(creature);
        }
};

// Gravity Well 42499
struct npc_gravity_well : public ScriptedAI
{
    npc_gravity_well(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    uint32 uidespawnTimer;

    void Reset() override
    {
        me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
        uidespawnTimer = 20000;
        events.ScheduleEvent(EVENT_GRAVITY_WELL_1, 3000);
        DoCast(me, SPELL_GRAVITY_WELL_AURA_0);
    }

    void KilledUnit(Unit* /*victim*/) override
    {
        if (Creature* azil = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(DATA_HIGH_PRIESTESS_AZIL) : 0))
            azil->AI()->Talk(TALK_SLAY);
    }

    void UpdateAI(uint32 diff) override
    {
        if (uidespawnTimer <= diff)
            me->DespawnOrUnsummon();
        else
            uidespawnTimer -= diff;

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_GRAVITY_WELL_1)
            {
                me->RemoveAurasDueToSpell(SPELL_GRAVITY_WELL_AURA_0);
                DoCast(me, SPELL_GRAVITY_WELL_KNOCKBACK, true);
                DoCast(me, SPELL_GRAVITY_WELL_AURA_1, true);
            }
            break;
        }
    }
};

// Camera Bunny 42565
struct npc_stonecore_camera_bunny : public ScriptedAI
{
    npc_stonecore_camera_bunny(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 summonerGUID;
    float sPosZ;

    void IsSummonedBy(Unit* summoner) override
    {
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        me->SetHover(true);

        summonerGUID = summoner->GetGUID();
        summoner->CastSpell(me, 46598, true);
        me->DespawnOrUnsummon(5 * IN_MILLISECONDS);
        sPosZ = me->GetPositionZ();

        // Up
        scheduler
            .Schedule(Milliseconds(1200), [this](TaskContext context)
        {
            me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 6.0f);
            context.Repeat(Milliseconds(2400));
        });

        // Down
        scheduler
            .Schedule(Milliseconds(2400), [this](TaskContext context)
        {
            me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), sPosZ);

            if (Creature* azil = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(DATA_HIGH_PRIESTESS_AZIL) : 0))
                if (Unit* target = ObjectAccessor::GetUnit(*me, summonerGUID))
                    azil->CastSpell(target, IsHeroic() ? SPELL_FORCE_GRIP_EFF_HC : SPELL_FORCE_GRIP_EFF, true);

            context.Repeat(Milliseconds(2400));
        });
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);
    }
};

// Siesmic Shard 42355
struct npc_stonecore_siesmic_shard : public ScriptedAI
{
    npc_stonecore_siesmic_shard(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    InstanceScript* instance;
    float x, y, z;
    bool hasActivated;

    void InitializeAI() override
    {
        me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        me->SetHover(true);

        DoCast(me, SPELL_SEISMIC_SHARD_VISUAL, true);
        instance = me->GetInstanceScript();
        x = 0.0f; y = 0.0f; z = 0.0f;
        hasActivated = false;
    }

    void KilledUnit(Unit* /*victim*/) override
    {
        if (Creature* azil = ObjectAccessor::GetCreature(*me, me->GetInstanceScript() ? me->GetInstanceScript()->GetData64(DATA_HIGH_PRIESTESS_AZIL) : 0))
            azil->AI()->Talk(TALK_SLAY);
    }

    uint32 GetData(uint32 type) const override
    {
        return hasActivated ? 1 : 0;
    }

    void DoAction(int32 actionId) override
    {
        if (actionId == ACTION_LAUNCH_SHARD)
        {
            hasActivated = true;

            if (Creature* azil = ObjectAccessor::GetCreature(*me, instance ? instance->GetData64(DATA_HIGH_PRIESTESS_AZIL) : 0))
            {
                if (Unit* target = ObjectAccessor::GetUnit(*me, azil->AI()->GetGUID(TYPE_SHARD)))
                {
                    x = target->GetPositionX(); y = target->GetPositionY(); z = target->GetPositionZ();
                    azil->CastSpell(target, SPELL_SEISMIC_SHARD_TARGETING, true);

                    scheduler
                        .Schedule(Milliseconds(1500), [this](TaskContext context)
                    {
                        me->CastSpell(x, y, z, SPELL_SEISMIC_SHARD_MISSLE, true);
                    });
                }
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);
    }
};

// Gravity Well 79251
class spell_high_priestess_azil_gravity_well_script : public SpellScript
{
    PrepareSpellScript(spell_high_priestess_azil_gravity_well_script);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, SPELL_GRAVITY_WELL_AURA_DMG, true);
    }

    // Hackfix, it can`t handle find targets by unk reason
    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.clear();

        std::list<Player*> pList;
        GetPlayerListInGrid(pList, GetCaster(), 9.05f);

        for (auto&& itr : pList)
            targets.push_back(itr);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_high_priestess_azil_gravity_well_script::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_high_priestess_azil_gravity_well_script::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// Gravity Well Knockback 79332
class spell_high_priestess_azil_gravity_well_knockback : public SpellScript
{
    PrepareSpellScript(spell_high_priestess_azil_gravity_well_knockback);

    // Hackfix, it can`t handle find targets by unk reason
    void SelectTarget(std::list<WorldObject*>& targets)
    {
        targets.clear();

        std::list<Player*> pList;
        GetPlayerListInGrid(pList, GetCaster(), 9.05f);

        for (auto&& itr : pList)
            targets.push_back(itr);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_high_priestess_azil_gravity_well_knockback::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_high_priestess_azil_gravity_well_knockback::SelectTarget, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

// Gravity Well Eff 79249
class spell_azil_gravity_well_eff : public SpellScript
{
    PrepareSpellScript(spell_azil_gravity_well_eff);

    void HandleEffectHitTarget(SpellEffIndex /*eff_idx*/)
    {
        if (Unit* unit = GetHitUnit())
        {
            // 75% damage reduction after 9.5 yards distance
            float dist = unit->GetExactDist2d(GetCaster());
            uint32 defaultDamage = unit->GetMap()->IsHeroic() ? 50000 : 25000;

            int32 reduction = dist >= 9.5 ? defaultDamage * 0.25 : defaultDamage * (1 - ((dist * 2.5) / 100));
            SetHitDamage(reduction);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_azil_gravity_well_eff::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Energy Shield 79050, 92667
class spell_stonecore_energy_shield : public SpellScript
{
    PrepareSpellScript(spell_stonecore_energy_shield);

    void HandleAfterCast()
    {
        if (Creature* caster = GetCaster()->ToCreature())
            caster->AI()->DoAction(ACTION_AZIL_SHIELD);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_stonecore_energy_shield::HandleAfterCast);
    }
};

// Force Grip 79351
class spell_stonecore_force_grip : public AuraScript
{
    PrepareAuraScript(spell_stonecore_force_grip);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* owner = GetOwner()->ToUnit())
            {
                owner->CastSpell(owner, SPELL_SUMMON_CAMERA_BUNNY, true);

                if (caster->ToCreature())
                    caster->ToCreature()->AI()->SetGUID(owner->GetGUID(), TYPE_FORCE_GRIP);
            }
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* owner = GetOwner()->ToCreature())
            if (Unit* gripTarget = ObjectAccessor::GetUnit(*owner, owner->AI()->GetGUID(TYPE_FORCE_GRIP)))
                gripTarget->ExitVehicle();
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_stonecore_force_grip::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_stonecore_force_grip::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Siesmic Shard 79009
class spell_stonecore_siesmic_shard : public AuraScript
{
    PrepareAuraScript(spell_stonecore_siesmic_shard);

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Creature* owner = GetOwner()->ToCreature())
            owner->DespawnOrUnsummon(1 * IN_MILLISECONDS);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_stonecore_siesmic_shard::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_boss_azil()
{
    new boss_high_priestess_azil();
    new creature_script<npc_gravity_well>("npc_gravity_well");
    new creature_script<npc_stonecore_camera_bunny>("npc_stonecore_camera_bunny");
    new creature_script<npc_stonecore_siesmic_shard>("npc_stonecore_siesmic_shard");
    new spell_script<spell_high_priestess_azil_gravity_well_script>("spell_high_priestess_azil_gravity_well_script");
    new spell_script<spell_high_priestess_azil_gravity_well_knockback>("spell_high_priestess_azil_gravity_well_knockback");
    new spell_script<spell_azil_gravity_well_eff>("spell_azil_gravity_well_eff");
    new spell_script<spell_stonecore_energy_shield>("spell_stonecore_energy_shield");
    new aura_script<spell_stonecore_force_grip>("spell_stonecore_force_grip");
    new aura_script<spell_stonecore_siesmic_shard>("spell_stonecore_siesmic_shard");
}