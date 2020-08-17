/// Scripts
#include "deadmines.h"

/// Game
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"

enum GlubtokSpells
{
    SPELL_ARCANE_DEAD           = 88185,
    SPELL_ARCANE_FIRE_RAY       = 88072,
    SPELL_ARCANE_FROST_RAY      = 88093,
    SPELL_ARCANE_OVERCHARGE     = 88183,
    SPELL_ARCANE_OVERCHARGE_2   = 90520,
    SPELL_ARCANE_POWER          = 88009,
    SPELL_BLINK                 = 87925,
    SPELL_DEEP_FREEZE           = 88000,
    SPELL_FIRE_BLOSSOM          = 88129,
    SPELL_FIRE_BLOSSOM_SUMMON   = 91301,
    SPELL_FIRE_BLOSSOM_VISUAL   = 88164,
    SPELL_FIREWALL_DAMAGE       = 91398,
    SPELL_FIST_OF_FLAME         = 87859,
    SPELL_FIST_OF_FROST         = 87861,
    SPELL_FROST_BLOSSOM         = 88169,
    SPELL_FROST_BLOSSOM_SUMMON  = 91302,
    SPELL_FROST_BLOSSOM_VISUAL  = 88165,
    SPELL_MASS_TELEPORT         = 87999,
    SPELL_TELEPORT_VISUAL       = 88002,
    SPELL_TRANSITION_INVIS      = 90424,
};

enum GlubtokTexts
{
    SAY_AGGRO = 0,
    SAY_FISTS_OF_FLAME,
    SAY_FISTS_OF_FROST,
    SAY_READY,
    SAY_LETS_DO_IT,
    SAY_ARCANE_POWER,
    EMOTE_FLAME_WALL,
    SAY_TOO_MUCH_POWER,
    SAY_KILL,
};

enum GlubtokNpcs
{
    NPC_FIRE_ELEMENTAL          = 48957,
    NPC_FROST_ELEMENTAL         = 48958,
    NPC_FIREWALL_VEHICLE        = 48974,
};

const uint32 firewallPassengers[] =
{
    49042,
    49040,
    48975,
    48976,
    49039,
    49041,
    49042,
    49040
};

enum GlubtokEvents
{
    EVENT_FISTS_OF_FLAME   = 1, // phase 1
    EVENT_FISTS_OF_FROST   = 2, // phase 1
    EVENT_BLINK            = 3, // phase 1
    EVENT_FROST_BLOSSOM    = 4, // phase 2
    EVENT_FIRE_BLOSSOM     = 5, // phase 2 hc only?
    EVENT_ARCANE_POWER     = 6,
    EVENT_SUMMON_FIRE_WALL = 7,

    EVENT_OUTRO,
    EVENT_OUTRO1,
};

enum GlubtokMisc
{
    PHASE_ONE           = 1,
    PHASE_50_PERCENT    = 2,
    PHASE_OUTRO         = 3,
    POINT_CENTER        = 4
};

enum GlubtokActions
{
    ACTION_BLINK        = 1,
};
//Sniff fireWallSpawnPos -193.268f, -442.015f, 53.712f, 1.6929f
// due to some bugs at the vehicle system the passenger model goes underground we need to set the vehicle a little bit higher... a correct fix require a vehicle seat rewrite
Position const fireWallSpawnPos = { -193.4377f, -441.3746f, 59.7092f, 1.8033f };
Position const phaseTwoTeleportPos = { -193.368f, -441.828f, 53.5931f, 1.2998f };

class RotateEvent : public BasicEvent
{
    public:
        RotateEvent(Creature* owner) : _owner(owner) { }

        bool Execute(uint64 execTime, uint32 /*diff*/) override
        {
            _owner->GetMotionMaster()->MoveRotate(20000, ROTATE_DIRECTION_RIGHT);
            _owner->m_Events.AddEvent(this, execTime + 20000);
            return false;
        }
    private :
        Creature* _owner;
};

class boss_glubtok : public CreatureScript
{
    public:
        boss_glubtok() : CreatureScript("boss_glubtok") { }

        struct boss_glubtokAI : public BossAI
        {
            boss_glubtokAI(Creature* creature) : BossAI(creature, BOSS_GLUBTOK_DATA) { }

            void Reset() override
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                instance->SetData(DATA_FIREWALL, 1);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetHover(false);
                die = false;
                BossAI::Reset();
            }

            void EnterCombat(Unit* who) override
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                _EnterCombat();
                Talk(SAY_AGGRO);
                events.SetPhase(PHASE_ONE);
                events.ScheduleEvent(urand(EVENT_FISTS_OF_FLAME, EVENT_FISTS_OF_FROST), 3000, 0, PHASE_ONE);
                events.ScheduleEvent(EVENT_BLINK, urand(5000, 10000), 0, PHASE_ONE);
                BossAI::EnterCombat(who);
            }

            void JustDied(Unit* killer) override
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                _JustDied();
                BossAI::JustDied(killer);
            }

            void KilledUnit(Unit* victim) override
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
                BossAI::KilledUnit(victim);
            }

            void DoAction(int32 const act) override
            {
                if (act == ACTION_BLINK && !events.IsInPhase(PHASE_50_PERCENT))
                {
                    DoCast(SPELL_BLINK);

                    if (IsHeroic())
                        DoResetThreat();
                }
            }

            void EnterEvadeMode() override
            {
                summons.DespawnAll();
                me->SetControlled(false, UNIT_STATE_ROOT);
                me->RemoveAllAuras();
                me->CombatStop();
                _EnterEvadeMode();
                _DespawnAtEvade();
                BossAI::EnterEvadeMode();
            }

            void DamageTaken(Unit* attacker, uint32& damage) override
            {
                if (die)
                {
                    damage = 0;
                    return;
                }

                if (me->HealthBelowPctDamaged(50, damage) && events.IsInPhase(PHASE_ONE))
                {
                    events.Reset();
                    events.SetPhase(PHASE_50_PERCENT);
                    me->SetTarget(0);
                    me->RemoveAllAuras();
                    me->NearTeleportTo(phaseTwoTeleportPos);
                    DoCast(SPELL_TELEPORT_VISUAL);
                    me->SetControlled(true, UNIT_STATE_ROOT);
                    events.ScheduleEvent(EVENT_ARCANE_POWER, 500, 0, PHASE_50_PERCENT);
                }

                if (damage > me->GetHealth())
                {
                    die = true;
                    damage = 0;
                    events.Reset();
                    events.SetPhase(PHASE_OUTRO);
                    me->SetHover(false);
                    me->CastSpell(me, SPELL_ARCANE_OVERCHARGE, true);
                    instance->SetBossState(BOSS_GLUBTOK_DATA, DONE);
                    events.ScheduleEvent(EVENT_OUTRO, 1000);
                }

                BossAI::DamageTaken(attacker, damage);
            }

            void JustSummoned(Creature* summon) override
            {
                switch (summon->GetEntry())
                {
                    case NPC_FIRE_BLOSSOM_BUNNY:
                        summon->CastSpell(summon, SPELL_FIRE_BLOSSOM_VISUAL, true);
                        DoCast(summon, SPELL_FIRE_BLOSSOM, false);
                        break;
                    case NPC_FROST_BLOSSOM_BUNNY:
                        summon->CastSpell(summon, SPELL_FROST_BLOSSOM_VISUAL, true);
                        DoCast(summon, SPELL_FROST_BLOSSOM, false);
                        break;
                    case NPC_FROST_ELEMENTAL:
                    case NPC_FIRE_ELEMENTAL:
                        summon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_DISABLE_MOVE);
                        summon->SetReactState(REACT_AGGRESSIVE);
                        summon->SetInCombatWithZone();
                        break;
                    default:
                        break;
                }
                BossAI::JustSummoned(summon);
            }

            void UpdateAI(uint32 const diff) override
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
                        case EVENT_OUTRO:
                            DoCast(me, SPELL_TRANSITION_INVIS, true);
                            DoCast(me, SPELL_TRANSITION_INVIS, true);
                            DoCast(me, SPELL_ARCANE_OVERCHARGE_2, true);
                            events.ScheduleEvent(EVENT_OUTRO1, 2300);
                            break;
                        case EVENT_OUTRO1:
                            die = false;
                            DoCast(me, SPELL_ARCANE_DEAD, true);
                            break;
                        case EVENT_FISTS_OF_FLAME:
                            Talk(SAY_FISTS_OF_FLAME);
                            DoCast(me, SPELL_FIST_OF_FLAME, false);
                            events.ScheduleEvent(EVENT_FISTS_OF_FROST, 12100, 0, PHASE_ONE);
                            break;
                        case EVENT_FISTS_OF_FROST:
                            Talk(SAY_FISTS_OF_FROST);
                            DoCast(me, SPELL_FIST_OF_FROST, false);
                            events.ScheduleEvent(EVENT_FISTS_OF_FLAME, 12100, 0, PHASE_ONE);
                            break;
                        case EVENT_FROST_BLOSSOM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->SummonCreature(NPC_FROST_BLOSSOM_BUNNY, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 5000);
                            events.ScheduleEvent(EVENT_FIRE_BLOSSOM, 3000, 0, PHASE_50_PERCENT);
                            break;
                        case EVENT_FIRE_BLOSSOM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->SummonCreature(NPC_FIRE_BLOSSOM_BUNNY, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 5000);
                            events.ScheduleEvent(EVENT_FROST_BLOSSOM, 3000, 0, PHASE_50_PERCENT);
                            break;
                        case EVENT_ARCANE_POWER:
                        {
                            Talk(SAY_ARCANE_POWER);
                            DoCast(me, SPELL_ARCANE_POWER, true);
                            //Movement::MoveSplineInit init(me);
                            //init.MoveTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 2.0f, false, true);
                            //init.SetVelocity(1.5f);
                            //init.Launch();
                            me->SetDisableGravity(true);
                            me->SetHover(true);

                            std::list<Creature*> stalkers;
                            me->GetCreatureListWithEntryInGrid(stalkers, NPC_FIRE_BUNNY, 100.0f);
                            for (Creature* stalker : stalkers)
                                stalker->CastSpell(me, SPELL_ARCANE_FIRE_RAY, true);

                            stalkers.clear();

                            me->GetCreatureListWithEntryInGrid(stalkers, NPC_FROST_BUNNY, 100.00f);
                            for (Creature* stalker: stalkers)
                                stalker->CastSpell(me, SPELL_ARCANE_FROST_RAY, true);

                            if (IsHeroic())
                                events.ScheduleEvent(EVENT_SUMMON_FIRE_WALL, 6000, 0, PHASE_50_PERCENT);
                            events.ScheduleEvent(urand(EVENT_FROST_BLOSSOM, EVENT_FIRE_BLOSSOM), 10000, 0, PHASE_50_PERCENT);
                            break;
                        }
                        case EVENT_SUMMON_FIRE_WALL:
                            Talk(EMOTE_FLAME_WALL);

                            if (Creature* firewall = me->SummonCreature(NPC_FIREWALL_VEHICLE, fireWallSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                firewall->m_Events.AddEvent(new RotateEvent(firewall), firewall->m_Events.CalculateTime(1000));
                                firewall->SetDisableGravity(true);
                                Position fireWallPos(*firewall);

                                for (uint8 i = 0; i < 8; i++)
                                {
                                    if (Creature* passenger = me->SummonCreature(firewallPassengers[i], fireWallPos, TEMPSUMMON_MANUAL_DESPAWN))
                                    {
                                        passenger->SetDisableGravity(true);
                                        passenger->CastCustomSpell(VEHICLE_SPELL_RIDE_HARDCODED, SPELLVALUE_BASE_POINT0, i, firewall, true);
                                        passenger->CastWithDelay(1000, passenger, SPELL_FIREWALL_DAMAGE, false);
                                    }
                                }
                            }
                            break;
                    }
                }

                if (!events.IsInPhase(PHASE_50_PERCENT))
                    DoMeleeAttackIfReady();
            }

        private:
            bool die;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_glubtokAI(creature);
        }
};

class npc_anti_evade_ai : public CreatureScript
{
    public:
        npc_anti_evade_ai() : CreatureScript("npc_anti_evade_ai") { }

        struct npc_anti_evade_aiAI : public PassiveAI
        {
            npc_anti_evade_aiAI(Creature* creature) : PassiveAI(creature) { }

            void UpdateAI(uint32 const diff) override { }
            void AttackStart(Unit* target) override { }
            void EnterEvadeMode() override { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_anti_evade_aiAI(creature);
        }
};

// 87925
class spell_glubtok_blink : public SpellScriptLoader
{
    public:
        spell_glubtok_blink() : SpellScriptLoader("spell_glubtok_blink") { }

        class spell_glubtok_blink_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_glubtok_blink_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if([](WorldObject* target)
                {
                    return target->GetTypeId() != TYPEID_PLAYER;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_glubtok_blink_SpellScript::FilterTargets, EFFECT_0, TARGET_DEST_TARGET_RANDOM);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_glubtok_blink_SpellScript();
        }
};

// 87859 87861
class spell_glubtok_fist : public SpellScriptLoader
{
    public:
        spell_glubtok_fist() : SpellScriptLoader("spell_glubtok_fist") { }

        class spell_glubtok_fist_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_glubtok_fist_AuraScript);

            void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (Creature* glubtok = caster->ToCreature())
                        glubtok->AI()->DoAction(ACTION_BLINK);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_glubtok_fist_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_glubtok_fist_AuraScript();
        }
};

class spell_blottom_fire : public SpellScriptLoader
{
    public:
        spell_blottom_fire() : SpellScriptLoader("spell_blottom_fire") { }

        class spell_blottom_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_blottom_fire_SpellScript);

            void HitTargetBunny(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit *caster = GetCaster())
                    if (Unit *trigger = caster->FindNearestCreature(NPC_FIRE_BLOSSOM_BUNNY, 200.0f))
                        caster->CastSpell(trigger, 88173, true);
            }

            void HeroicSummon(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit *caster = GetCaster())
                    if (Unit *trigger = caster->FindNearestCreature(NPC_FIRE_BLOSSOM_BUNNY, 200.0f))
                        caster->CastSpell(trigger, 91301, true);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_blottom_fire_SpellScript::HitTargetBunny, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
                if (m_scriptSpellId == 91286)
                    OnEffectHit += SpellEffectFn(spell_blottom_fire_SpellScript::HeroicSummon, EFFECT_1, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_blottom_fire_SpellScript();
        }
};

class spell_blottom_frost : public SpellScriptLoader
{
    public:
        spell_blottom_frost() : SpellScriptLoader("spell_blottom_frost") { }

        class spell_blottom_frost_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_blottom_frost_SpellScript);

            void HitTargetBunny(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit *caster = GetCaster())
                    if (Unit *trigger = caster->FindNearestCreature(NPC_FROST_BLOSSOM_BUNNY, 200.0f))
                        caster->CastSpell(trigger, 88177, true);
            }

            void HeroicSummon(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit *caster = GetCaster())
                    if (Unit *trigger = caster->FindNearestCreature(NPC_FROST_BLOSSOM_BUNNY, 200.0f))
                        caster->CastSpell(trigger, 91302, true);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_blottom_frost_SpellScript::HitTargetBunny, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
                if (m_scriptSpellId == 91287)
                    OnEffectHit += SpellEffectFn(spell_blottom_frost_SpellScript::HeroicSummon, EFFECT_1, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_blottom_frost_SpellScript();
        }
};

// 16184
class achievement_ready_for_raid : public AchievementCriteriaScript
{
    public:
        achievement_ready_for_raid() : AchievementCriteriaScript("achievement_ready_for_raid") { }

        bool OnCheck(Player* /*source*/, Unit* target) override
        {
            if (!target)
                return false;

            if (InstanceScript* instance = target->GetInstanceScript())
                return instance->GetData(DATA_FIREWALL);

            return false;
        }
};

// 91397
class spell_gl_firewall : public SpellScriptLoader
{
    public:
        spell_gl_firewall() : SpellScriptLoader("spell_gl_firewall") { }

        class spell_gl_firewall_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gl_firewall_SpellScript);

            void CalculateDamage(SpellEffIndex /*effIndex*/)
            {
                if (!GetHitUnit())
                    return;

                if (Unit* caster = GetCaster())
                    if (InstanceScript* instance = caster->GetInstanceScript())
                        instance->SetData(DATA_FIREWALL, 0);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gl_firewall_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gl_firewall_SpellScript();
        }
};

// 91398 - Fire Wall
class spell_gl_firewall_b : public SpellScriptLoader
{
    public:
        spell_gl_firewall_b() : SpellScriptLoader("spell_gl_firewall_b") { }

        class spell_gl_firewall_b_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gl_firewall_b_AuraScript);

            void HandleProc(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                // z position in sniffs is always 55.70924
                GetTarget()->CastSpell(GetTarget()->GetPositionX(), GetTarget()->GetPositionY(), 55.70924f, aurEff->GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gl_firewall_b_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gl_firewall_b_AuraScript();
        }
};

void AddSC_boss_glubtok()
{
    new boss_glubtok();
    new npc_anti_evade_ai();
    new spell_glubtok_blink();
    new spell_glubtok_fist();
    new spell_blottom_fire();
    new spell_blottom_frost();

    new achievement_ready_for_raid();
    new spell_gl_firewall();
    new spell_gl_firewall_b();
}