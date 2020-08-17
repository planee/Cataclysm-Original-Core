/*
* Copyright (C) 2013-2014 OMFG.GG Network <http://www.omfg.gg>
* Copyright (C) 2012-2013 Reign of Darkness Network <http://reignofdarkness.net/>
*
* Este codigo es de uso exclusivo y privado para la red de gaming
* Reign of Darkness Network esta totalmente prohibida su distribucion y
* uso en proyectos ajenos, si has obtenido o encontrado este codigo
* publicamente envianos un email a administracion@reignofdarkness.net
* indicandonos como y donde has obtenido este codigo
*
* Recuerda, no distribuyas, no compartas y no subas este codigo a ningun
* lugar publico, usa siempre nuestros repositorios Git!
*/

#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "firelands.h"

enum Yells
{
    SAY_INTRO_1     = 0,
    SAY_INTRO_2     = 1,
    SAY_INTRO_3     = 2,

    SAY_AGGRO       = 3,
    SAY_ON_DEAD     = 4,
    SAY_ON_KILL     = 5,

    SAY_SCORPION    = 6,
    SAY_SCORPION2   = 7,
    SAY_CAT         = 8,
    SAY_CAT2        = 9,

    SAY_SEEDS       = 10,
    SAY_ORBS        = 11
};

enum Spells
{
    // all phases
    SPELL_ZERO_POWER                = 72242,
    SPELL_FORM_CONTROLLER           = 98386, // unused - forms are checked with clump check + doaction
    SPELL_CLUMP_CHECK               = 98399,
    SPELL_BERSERK                   = 47008,

    // cat phase
    SPELL_CAT_FORM                  = 98374,
    SPELL_LEAPING_FLAMES            = 98476,
    SPELL_LEAPING_FLAMES_SUMMON     = 101222,
    SPELL_LEAPING_FLAMES_GROUND     = 98535,

    // scorpion phase
    SPELL_SCORPION_FORM             = 98379,
    SPELL_FLAME_SCYTE               = 98474,
    SPELL_REVITALIZE                = 104915,

    // druid phase
    SPELL_FIERY_CYCLONE             = 98443,
    SPELL_SEARING_SEEDS             = 98450,
    SPELL_SEARING_SEEDS_DMG         = 98620,

    // both forms
    SPELL_ADRENALINE                = 97238,
    SPELL_FURY                      = 97235,

    // burning orbs
    SPELL_BURNING_ORBS              = 98451,
    SPELL_BURNING_ORBS_SUMMON       = 98565,
    SPELL_BURNING_ORBS_VISUAL       = 98583,
    SPELL_BURNING_ORBS_DMG          = 98584,

    // heroic
    SPELL_CONCENTRATION             = 98229,
    SPELL_UNCOMMON_CONCENTRATION    = 98254,
    SPELL_RARE_CONCENTRATION        = 98253,
    SPELL_EPIC_CONCENTRATION        = 98252,
    SPELL_LEGENDARY_CONCENTRATION   = 98245,

    // intro
    SPELL_TRIGGER_BRIDGE_INTRO      = 101035
};

enum Events
{
    EVENT_BERSERK                   = 1,
    EVENT_INTRO_1                   = 2,
    EVENT_INTRO_2                   = 3,
    EVENT_INTRO_3                   = 4,
    EVENT_SUMMON_ORBS               = 5,
    EVENT_CHECK_ENERGY              = 6,
    EVENT_INTRO_CINEMATIC_START     = 7,
    EVENT_INTRO_CINEMATIC_END       = 8,
    EVENT_INTRO_CINEMATIC_BRIDGE    = 9,
    EVENT_START_CAT_PHASE           = 10,
    EVENT_START_SCORPION_PHASE      = 11,
    EVENT_START_DRUID_PHASE         = 12,
    EVENT_CLUMP_CHECK               = 13,
    EVENT_START_INTRO               = 14,
    // Spirit of the Flame
    EVENT_JUMP_TARGET               = 15,

    // Burning Orb
    EVENT_ORB_DAMAGE                = 16,

    EVENT_ALLOW_FORM_SWITCH         = 17,
    EVENT_CONCENTRATION             = 18
};

enum MajordomoPhase
{
    PHASE_NORMAL   = 1,
    PHASE_DRUID    = 2,
    PHASE_CAT      = 3,
    PHASE_SCORPION = 4
};

enum MajorActions
{
    ACTION_START_CAT_PHASE      = 0,
    ACTION_START_SCORPION_PHASE = 1
};

class casterTargetSelector
{
public:
    explicit casterTargetSelector(Unit* caster) : _caster(caster){};

    bool operator()(WorldObject* target) const
    {
        if (target->GetTypeId() != TYPEID_PLAYER)
            return false;

        return target->ToPlayer()->HasCasterSpec() && _caster->GetDistance2d(target) <= 90.0f;
    }

private:
    Unit const* _caster;
};

class boss_majordomo_staghelm : public CreatureScript
{
    public:
        boss_majordomo_staghelm() : CreatureScript("boss_majordomo_staghelm"){}

    struct boss_majordomo_staghelmAI : public BossAI
    {
        boss_majordomo_staghelmAI(Creature* creature) : BossAI(creature, DATA_MAJORDOMO_STAGHELM)
        {
            introDone = false;
            druidsDead = false;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6);
        }

        void Reset() override
        {
            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
            {
                uint32 preNerfHealth = RAID_MODE(50962596, 178540842, 124694974, 432087265);
                me->SetMaxHealth(preNerfHealth);
                me->SetHealth(preNerfHealth);
            }
            me->SetByteValue(UNIT_FIELD_BYTES_0, 1, 4);
            me->setPowerType(POWER_ENERGY);
            me->SetPower(POWER_ENERGY, 0);
            canSwitchForm = true;
            transformcount = 0;
            if (IsHeroic())
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONCENTRATION);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_UNCOMMON_CONCENTRATION);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RARE_CONCENTRATION);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EPIC_CONCENTRATION);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LEGENDARY_CONCENTRATION);
            }
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            DoCast(me, SPELL_ZERO_POWER, true);
            _Reset();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_POWER_BURN, true);
        }

        void EnterEvadeMode() override
        {
            me->GetMotionMaster()->MoveTargetedHome();
            _EnterEvadeMode();
        }

        void JustReachedHome()
        {
            // Despawn leaping flames ground effect after boss reset
            uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_LEAPING_FLAMES_GROUND, me);
            std::list<WorldObject*> targetList;
            Trinity::AllWorldObjectsInRange objects(me, 250.0f);
            Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects, GRID_MAP_TYPE_MASK_DYNAMICOBJECT);
            me->VisitNearbyObject(250.0f, searcher);
            for (auto objects : targetList)
            {
                if (DynamicObject* dynObj = objects->ToDynObject())
                {
                    if (dynObj->GetSpellId() == spellId)
                        dynObj->Remove();
                }
            }

            BossAI::JustReachedHome();
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_ON_KILL);
        }

        void JustSummoned(Creature* summon) override
        {
            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
            {
                float div = 1.0f;
                switch (summon->GetEntry())
                {
                    case NPC_DRUID_OF_THE_FLAME:
                        div = 0.85f;
                        break;
                    case NPC_SPIRIT_OF_THE_FLAME:
                        div = IsHeroic() ? 0.85f : 0.75f;
                        break;
                }
                uint32 preNerfHealth = summon->GetMaxHealth() / div;
                summon->SetMaxHealth(preNerfHealth);
                summon->SetHealth(preNerfHealth);
            }

            summons.Summon(summon);
            summon->setActive(true);

            if (me->isInCombat())
                summon->AI()->DoZoneInCombat();

            if (summon->GetEntry() == NPC_SPIRIT_OF_THE_FLAME)
            {
                ThreatContainer::StorageType const &threatlist = me->getThreatManager().getThreatList();
                ThreatContainer::StorageType::const_iterator itr = threatlist.begin();
                for (itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                {
                    if (Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                    {
                        if (unit->isAlive())
                        {
                            float threat = me->getThreatManager().getThreat(unit);
                            summon->AddThreat(unit, threat);
                        }
                    }
                }
            }
        }

        void JustDied(Unit * /*victim*/) override
        {
            if (IsHeroic())
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONCENTRATION);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_UNCOMMON_CONCENTRATION);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RARE_CONCENTRATION);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EPIC_CONCENTRATION);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LEGENDARY_CONCENTRATION);
            }
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            Talk(SAY_ON_DEAD);
            _JustDied();

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->HasQuestForItem(ITEM_HEART_OF_FLAME))
                {
                    DoCast(me, SPELL_SMOULDERING, true);
                    break;
                }
            }
        }

        void EnterCombat(Unit* who) override
        {
            if (IsHeroic())
                events.ScheduleEvent(EVENT_CONCENTRATION, 1000);

            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            events.ScheduleEvent(EVENT_CLUMP_CHECK, 2000);
            events.ScheduleEvent(EVENT_CHECK_ENERGY, 3000);
            events.ScheduleEvent(EVENT_BERSERK, 10 * MINUTE * IN_MILLISECONDS);
            events.SetPhase(PHASE_NORMAL);
            me->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            _EnterCombat();
        }


        void DoAction(int32 action) override
        {
            if (action == ACTION_START_CAT_PHASE && events.IsInPhase(PHASE_CAT) ||
                action == ACTION_START_SCORPION_PHASE && events.IsInPhase(PHASE_SCORPION) ||
                events.IsInPhase(PHASE_DRUID) ||
                !canSwitchForm)
                return;

            if (transformcount >= 2)
            {
                canSwitchForm = false;
                events.SetPhase(PHASE_DRUID);
                events.ScheduleEvent(EVENT_START_DRUID_PHASE, 500);
                events.ScheduleEvent(action == ACTION_START_CAT_PHASE ? EVENT_START_CAT_PHASE : EVENT_START_SCORPION_PHASE, 4500);
                events.ScheduleEvent(EVENT_ALLOW_FORM_SWITCH, 10000);
                transformcount = 0;
                return;
            }

            switch (action)
            {
                case ACTION_START_INTRO:
                    events.ScheduleEvent(EVENT_START_INTRO, 10000);
                    break;
                case ACTION_START_INTRO2:
                    events.ScheduleEvent(EVENT_INTRO_3, 5000);
                    events.ScheduleEvent(EVENT_INTRO_CINEMATIC_START, 1);
                    break;
                case ACTION_START_CAT_PHASE:
                    canSwitchForm = false;
                    transformcount++;
                    events.SetPhase(PHASE_CAT);
                    events.ScheduleEvent(EVENT_START_CAT_PHASE, 500);
                    events.ScheduleEvent(EVENT_ALLOW_FORM_SWITCH, 10000);
                    break;
                case ACTION_START_SCORPION_PHASE:
                    canSwitchForm = false;
                    transformcount++;
                    events.SetPhase(PHASE_SCORPION);
                    events.ScheduleEvent(EVENT_START_SCORPION_PHASE, 500);
                    events.ScheduleEvent(EVENT_ALLOW_FORM_SWITCH, 10000);
                    break;
                default:
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == EFFECT_MOTION_TYPE && id == EVENT_JUMP)
            {
                if (Unit* target = me->FindNearestCreature(NPC_DESTINATION_STALKER, 30.0f))
                    DoCast(target, SPELL_LEAPING_FLAMES_GROUND, true);
                else
                    DoCast(me, SPELL_LEAPING_FLAMES_GROUND, true);
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!druidsDead && !me->FindNearestCreature(NPC_DRUID_OF_THE_FLAME, 200.0f, true)
                && instance->IsDone(DATA_BALOROC)
                && instance->IsDone(DATA_LORD_RHYOLITH)
                && instance->IsDone(DATA_SHANNOX)
                && instance->IsDone(DATA_BETHTILAC)
                && instance->IsDone(DATA_ALYSRAZOR))
            {
                druidsDead = true;
                introDone = true;
                Talk(SAY_AGGRO);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6);
                me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                me->GetMotionMaster()->MovePoint(0, 520.879f, -61.858f, 83.947f);
                me->SetHomePosition(520.879f, -61.858f, 83.947f, 3.14159f);
                me->SetReactState(REACT_AGGRESSIVE);
            }

            if (!UpdateVictim() && introDone)
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_INTRO:
                        events.ScheduleEvent(EVENT_INTRO_1, 1);
                        me->SetReactState(REACT_PASSIVE);
                        break;
                    case EVENT_INTRO_1:
                        TalkToMap(SAY_INTRO_1);
                        events.ScheduleEvent(EVENT_INTRO_2, 12000);
                        break;
                    case EVENT_INTRO_2:
                        TalkToMap(SAY_INTRO_2);
                        break;
                    case EVENT_INTRO_3:
                        TalkToMap(SAY_INTRO_3);
                        break;
                    case EVENT_CHECK_ENERGY:
                    {
                        if (events.IsInPhase(PHASE_SCORPION) && me->GetPower(POWER_ENERGY) == 100)
                            DoCast(me, SPELL_FLAME_SCYTE);

                        if (events.IsInPhase(PHASE_CAT) && me->GetPower(POWER_ENERGY) == 100)
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, casterTargetSelector(me));

                            if (!target && me->getVictim())
                                target = me->getVictim();

                            if (target)
                            {
                                me->SummonCreature(NPC_DESTINATION_STALKER, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ() - 1.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 4000);
                                DoCast(target, SPELL_LEAPING_FLAMES);
                            }
                        }
                        events.ScheduleEvent(EVENT_CHECK_ENERGY, 200);
                        break;
                    }
                    case EVENT_INTRO_CINEMATIC_START:
                    {
                        Map::PlayerList const& players = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            if (Player* player = itr->getSource())
                                player->SendCinematicStart(MOVIE_ID_BRIDGE_INTRO);

                        events.ScheduleEvent(EVENT_INTRO_CINEMATIC_BRIDGE, 3000);
                        events.ScheduleEvent(EVENT_INTRO_CINEMATIC_END, 20000);
                        break;
                    }
                    case EVENT_INTRO_CINEMATIC_BRIDGE:
                        if (Creature* trigger = me->SummonCreature(NPC_INVISIBLE_STALKER, 250.753f, -64.0087f, 66.0648f, 0, TEMPSUMMON_TIMED_DESPAWN, 120000))
                        {
                            trigger->SetCanFly(true);
                            trigger->SetDisableGravity(true);
                            trigger->CastSpell(trigger, SPELL_TRIGGER_BRIDGE_INTRO, true);
                        }
                        break;
                    case EVENT_INTRO_CINEMATIC_END:
                        if (GameObject* go = ObjectAccessor::GetGameObject(*me, instance->GetData64(GO_SULFURON_BRIDGE)))
                            go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                        introDone = true;
                        break;
                    case EVENT_SUMMON_ORBS:
                    {
                        uint8 count = RAID_MODE(2, 5, 2, 5);
                        for (uint8 i = 0; i < count; ++i)
                            DoCast(me, SPELL_BURNING_ORBS_SUMMON, true);
                        break;
                    }
                    case EVENT_START_CAT_PHASE:
                        if (!events.IsInPhase(PHASE_CAT))
                            events.SetPhase(PHASE_CAT);
                        me->RemoveAurasDueToSpell(SPELL_ADRENALINE);
                        me->SetPower(POWER_ENERGY, 0);
                        me->RemoveAurasDueToSpell(SPELL_SCORPION_FORM);
                        DoCast(me, SPELL_FURY, true);
                        DoCast(me, SPELL_CAT_FORM, true);
                        Talk(SAY_CAT);
                        Talk(SAY_CAT2);
                        break;
                    case EVENT_START_SCORPION_PHASE:
                        if (!events.IsInPhase(PHASE_SCORPION))
                            events.SetPhase(PHASE_SCORPION);
                        me->RemoveAurasDueToSpell(SPELL_ADRENALINE);
                        me->SetPower(POWER_ENERGY, 0);
                        me->RemoveAurasDueToSpell(SPELL_CAT_FORM);
                        DoCast(me, SPELL_FURY, true);
                        DoCast(me, SPELL_SCORPION_FORM, true);
                        Talk(SAY_SCORPION);
                        Talk(SAY_SCORPION2);
                        break;
                    case EVENT_START_DRUID_PHASE:
                        me->RemoveAurasDueToSpell(SPELL_ADRENALINE);
                        me->SetPower(POWER_ENERGY, 0);
                        DoCast(me, SPELL_FIERY_CYCLONE, true);
                        if (me->HasAura(SPELL_CAT_FORM))
                        {
                            me->RemoveAurasDueToSpell(SPELL_CAT_FORM);
                            Talk(SAY_SEEDS);
                            DoCast(me, SPELL_SEARING_SEEDS);
                        }

                        if (me->HasAura(SPELL_SCORPION_FORM))
                        {
                            me->RemoveAurasDueToSpell(SPELL_SCORPION_FORM);
                            Talk(SAY_ORBS);
                            DoCast(me, SPELL_BURNING_ORBS);
                            events.ScheduleEvent(EVENT_SUMMON_ORBS, 500);
                        }
                        break;
                    case EVENT_CLUMP_CHECK:
                        DoCastVictim(SPELL_CLUMP_CHECK);
                        events.ScheduleEvent(EVENT_CLUMP_CHECK, 1000);
                        break;
                    case EVENT_ALLOW_FORM_SWITCH:
                        canSwitchForm = true;
                        break;
                    case EVENT_CONCENTRATION:
                        DoCastAOE(SPELL_CONCENTRATION);
                        events.ScheduleEvent(EVENT_CONCENTRATION, 1000);
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

        private:
            bool introDone, druidsDead, canSwitchForm;
            uint32 transformcount;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_majordomo_staghelmAI(creature);
        }
};

class npc_burning_orb : public CreatureScript //53216
{
    public:
        npc_burning_orb() : CreatureScript("npc_burning_orb") { }

    struct npc_burning_orbAI : public ScriptedAI
    {
        npc_burning_orbAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* summoner) override
        {
            DoZoneInCombat();
            me->AddAura(SPELL_BURNING_ORBS_VISUAL, me);
        }

        void EnterCombat(Unit* who) override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
            events.ScheduleEvent(EVENT_ORB_DAMAGE, 3500);
        }

        void UpdateAI(const uint32 diff) override
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ORB_DAMAGE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST, 0, 200, true))
                            DoCast(target, SPELL_BURNING_ORBS_DMG);
                        events.ScheduleEvent(EVENT_ORB_DAMAGE, 3500);
                        break;
                }
            }
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_burning_orbAI(creature);
    }
};

class spell_searing_seeds : public SpellScriptLoader
{
public:
    spell_searing_seeds() : SpellScriptLoader("spell_searing_seeds") { }

    class spell_searing_seeds_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_searing_seeds_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SEARING_SEEDS))
                return false;
            return true;
        }

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            int32 duration = urand(10000, 60000);
            if (Aura* aura = aurEff->GetBase())
                aura->SetDuration(duration);
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetUnitOwner())
                target->CastSpell(target, SPELL_SEARING_SEEDS_DMG, true);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_searing_seeds_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_searing_seeds_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    class spell_searing_seeds_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_searing_seeds_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                return target->GetTypeId() == TYPEID_PLAYER
                    && target->ToPlayer()->HasTankSpec();
            });
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_searing_seeds_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_searing_seeds_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_searing_seeds_AuraScript();
    }
};

class spell_clump_check : public SpellScriptLoader
{
public:
    spell_clump_check() : SpellScriptLoader("spell_clump_check") { }

    class spell_clump_check_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_clump_check_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            scorpionForm = (targets.size() >= (GetSpellInfo()->Id == SPELL_CLUMP_CHECK ? 7u : 18u));
        }

        void HandleEffect(SpellEffIndex /*effIndex*/)
        {
            if (Creature* caster = GetCaster()->ToCreature())
            {
                if (scorpionForm)
                    caster->AI()->DoAction(ACTION_START_SCORPION_PHASE);
                else
                    caster->AI()->DoAction(ACTION_START_CAT_PHASE);
            }
        }

        private:
            bool scorpionForm;

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_clump_check_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY_COUNT_TARGETS);
            OnEffectHit += SpellEffectFn(spell_clump_check_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_clump_check_SpellScript();
    }
};

class spell_concentration : public SpellScriptLoader
{
public:
    spell_concentration() : SpellScriptLoader("spell_concentration") { }

    class spell_concentration_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_concentration_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (SpellInfo const* spellInfo = eventInfo.GetDamageInfo()->GetSpellInfo())
                if (spellInfo->Id == 98021 || spellInfo->Id == 49016 || spellInfo->Id == 32409)
                    return false;

            if (eventInfo.GetTypeMask() & PROC_FLAG_TAKEN_DAMAGE)
                return true;

            return false;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            if (Unit* target = GetTarget())
            {
                target->SetPower(POWER_ALTERNATE_POWER, 0);
                target->RemoveAurasDueToSpell(SPELL_UNCOMMON_CONCENTRATION);
                target->RemoveAurasDueToSpell(SPELL_RARE_CONCENTRATION);
                target->RemoveAurasDueToSpell(SPELL_EPIC_CONCENTRATION);
                target->RemoveAurasDueToSpell(SPELL_LEGENDARY_CONCENTRATION);
            }
        }

        void onPeriodicTick(AuraEffect const* /*aurEff*/)
        {
            if (Unit* target = GetTarget())
            {
                uint8 newPower = std::min(target->GetPower(POWER_ALTERNATE_POWER) + 5, 100);
                target->SetPower(POWER_ALTERNATE_POWER, newPower);

                if (newPower >= 25 && newPower < 50 && !target->HasAura(SPELL_UNCOMMON_CONCENTRATION))
                    target->AddAura(SPELL_UNCOMMON_CONCENTRATION, target);
                else if (newPower >= 50 && newPower < 75 && !target->HasAura(SPELL_RARE_CONCENTRATION))
                {
                    target->RemoveAurasDueToSpell(SPELL_UNCOMMON_CONCENTRATION);
                    target->AddAura(SPELL_RARE_CONCENTRATION, target);
                }
                else if (newPower >= 75 && newPower < 100 && !target->HasAura(SPELL_EPIC_CONCENTRATION))
                {
                    target->RemoveAurasDueToSpell(SPELL_RARE_CONCENTRATION);
                    target->AddAura(SPELL_EPIC_CONCENTRATION, target);
                }
                else if (newPower == 100 && !target->HasAura(SPELL_LEGENDARY_CONCENTRATION))
                {
                    target->RemoveAurasDueToSpell(SPELL_EPIC_CONCENTRATION);
                    target->AddAura(SPELL_LEGENDARY_CONCENTRATION, target);
                }
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_concentration_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_concentration_AuraScript::OnProc, EFFECT_2, SPELL_AURA_MOD_THREAT);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_concentration_AuraScript::onPeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_concentration_AuraScript();
    }
};

class spell_leaping_flames : public SpellScriptLoader
{
public:
    spell_leaping_flames() : SpellScriptLoader("spell_leaping_flames") { }

    class spell_leaping_flames_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_leaping_flames_SpellScript);

        void HandleOnCast()
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, GetSpellInfo()->Effects[EFFECT_0].BasePoints, true);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_leaping_flames_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_leaping_flames_SpellScript();
    }
};

class spell_leaping_flames_damage : public SpellScriptLoader
{
public:
    spell_leaping_flames_damage() : SpellScriptLoader("spell_leaping_flames_damage") { }

    class spell_leaping_flames_damage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_leaping_flames_damage_AuraScript);

        void HandleEffectCalcAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
                if (AuraEffect* fury = caster->GetAuraEffect(SPELL_FURY, EFFECT_0))
                    AddPct(amount, fury->GetAmount());

            canBeRecalculated = false;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_leaping_flames_damage_AuraScript::HandleEffectCalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_leaping_flames_damage_AuraScript();
    }
};

class spell_flame_scythe : public SpellScriptLoader
{
public:
    spell_flame_scythe() : SpellScriptLoader("spell_flame_scythe") { }

    class spell_flame_scythe_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_flame_scythe_SpellScript);

        bool Load() override
        {
            energizeAmount = 0;
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!targets.empty())
            {
                if (Unit* caster = GetCaster())
                {
                    uint8 hitCount = targets.size();
                    uint8 minCount = caster->GetMap()->Is25ManRaid() ? 18 : 7;
                    if (hitCount < minCount)
                    {
                        energizeAmount = (minCount - hitCount) * 10;
                        std::min(energizeAmount, 60);
                    }
                }
            }
        }

        void RewardExtraEnergy()
        {
            if (energizeAmount > 0)
                if (Unit* caster = GetCaster())
                    caster->CastCustomSpell(caster, SPELL_REVITALIZE, &energizeAmount, 0, 0, true);
        }

        int32 energizeAmount;

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_flame_scythe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_54);
            AfterCast += SpellCastFn(spell_flame_scythe_SpellScript::RewardExtraEnergy);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_flame_scythe_SpellScript();
    }
};

class spell_kneel_to_the_flame: public SpellScriptLoader // 99705
{
public:
    spell_kneel_to_the_flame () : SpellScriptLoader("spell_kneel_to_the_flame") { }

    class spell_kneel_to_the_flameSpellScript: public SpellScript
    {
        PrepareSpellScript(spell_kneel_to_the_flameSpellScript);

        void CalculateDamage(SpellEffIndex /*effIndex*/)
        {
            if (!GetHitUnit())
                return;

            if (GetHitUnit()->getStandState() == UNIT_STAND_STATE_KNEEL)
                SetHitDamage(0);
            else
            {
                if (Creature *caster = GetCaster()->ToCreature())
                    if (InstanceScript *instance = caster->GetInstanceScript())
                        instance->SetData(DATA_KNEEL_TO_FLAME, 0);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_kneel_to_the_flameSpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript () const
    {
        return new spell_kneel_to_the_flameSpellScript();
    }
};

class spell_fandrall_channelled : public SpellScriptLoader // 99750
{
public:
    spell_fandrall_channelled() : SpellScriptLoader("spell_fandrall_channelled") { }

    class spell_fandrall_channelled_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fandrall_channelled_SpellScript);

        void Activate(SpellEffIndex index)
        {
            PreventHitDefaultEffect(index);
            if (GameObject *go = GetHitGObj())
                if (InstanceScript *instance = go->GetInstanceScript())
                    if (Player *player = GetCaster()->ToPlayer())
                    {
                        GameObjectTemplate const* info = go->GetGOInfo();
                        player->CastSpell(player, info->summoningRitual.spellId, true);
                        if (!instance->GetData(DATA_KNEEL_TO_FLAME))
                        {
                            go->SetLootState(GO_JUST_DEACTIVATED);
                            if (Creature *fandrallFlame = go->FindNearestCreature(NPC_FANDRALS_FLAME, 10.0f))
                                fandrallFlame->CastSpell(fandrallFlame, SPELL_FANDRALL_VISUEL_ACTIVATION_AURA, true);
                        }
                    }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_fandrall_channelled_SpellScript::Activate, EFFECT_1, SPELL_EFFECT_ACTIVATE_OBJECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_fandrall_channelled_SpellScript();
    }
};

void AddSC_boss_majordomo_staghelm()
{
    new boss_majordomo_staghelm();
    new npc_burning_orb();
    new spell_searing_seeds();
    new spell_clump_check();
    new spell_concentration();
    new spell_leaping_flames();
    new spell_leaping_flames_damage();
    new spell_flame_scythe();
    new spell_kneel_to_the_flame();
    new spell_fandrall_channelled();
}
