
#include "GridNotifiers.h"
#include "PassiveAI.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "firelands.h"

enum Texts
{
    TALK_INTRO      = 0,
    TALK_AGGRO      = 1,
    TALK_RUPTURE    = 2,
    TALK_FACERAGE   = 3,
    TALK_RIP_DEAD   = 4,
    TALK_RAGE_DEAD  = 5,
    TALK_DEATH      = 6,
    TALK_SPEAR_TOSS = 7,
    TALK_KILL       = 8,
    TALK_EVADE      = 9
};

enum Spells
{
    //Shannox
    SPELL_ARCTIC_SLASH              = 99931,
    SPELL_BERSERK                   = 26662,
    SPELL_CALL_SPEAR                = 100663,
    SPELL_SPEAR_VISUAL              = 100005,   // Used to make the spear visible
    SPELL_HURL_SPEAR                = 100002,   // Dummy Effect & Damage
    SPELL_HURL_SPEAR_SUMMON         = 99978,    // Summons the Spear... Useless
    SPELL_HURL_SPEAR_DUMMY_SCRIPT   = 100031,
    SPELL_MAGMA_RUPTURE_SHANNOX     = 99840,
    SPELL_FRENZY_SHANNOX            = 100522,
    SPELL_IMMOLATION_TRAP           = 99839,
    SPELL_CRYSTAL_PRISON_TRAP       = 99836,

    // Riplimb
    SPELL_LIMB_RIP                  = 99832,
    SPELL_DOGGED_DETERMINATION      = 101111,
    SPELL_FETCH_SPEAR               = 100026,

    // Rageface
    SPELL_FACE_RAGE_JUMP            = 99945,
    SPELL_FACE_RAGE                 = 99947,
    SPELL_FACE_RAGE_BUFF            = 100129,
    SPELL_RAGE                      = 100415,

    // Both Dogs
    SPELL_FRENZIED_DEVOLUTION       = 100064,
    SPELL_FEEDING_FRENZY_H          = 100655,
    SPELL_WARY                      = 100167, // Buff when the Dog gets in a Trap

    // Misc
    SPELL_SEPERATION_ANXIETY        = 99835,

    //Spear Abilities
    SPELL_MAGMA_FLARE               = 100495, // Fire damage within 50 yards.
    SPELL_MAGMA_RUPTURE             = 100003, // Calls forth magma eruptions to damage nearby foes.
    SPELL_MAGMA_RUPTURE_VISUAL      = 99841,
    SPELL_SPEAR_TARGET              = 99988,
    SPELL_FLAME_STRIKE_MARKER       = 92211,

    //Traps Abilities
    CRYSTAL_PRISON_EFFECT           = 99837,
    IMMOLATION_TRAP                 = 99838,
    SPELL_CREATE_EMBERSTONE_FRAGMENT = 100518,
};

enum Events
{
    //Shannox
    EVENT_IMMOLATION_TRAP = 1,                       // Every 10s
    EVENT_ARCING_SLASH,                              // Every 12s
    EVENT_SUMMON_SPEAR,
    EVENT_SUMMON_CRYSTAL_PRISON,                     // Every 25s
    EVENT_HURL_SPEAR_OR_MAGMA_RUPTURE,               // Every 42s
    EVENT_HURL_SPEAR_2,                              // Used to call for the spear
    EVENT_HURL_SPEAR_3,                              // Unused for getting back riplimb
    EVENT_BERSERK,                                   // After 10m
    EVENT_CHECK_PET_DISTANCE,
    EVENT_CHECK_LOCATION,
    EVENT_SUMMON_SPIRAL,
    EVENT_REVIVE_RIPLIMB,

    //Riplimb
    EVENT_LIMB_RIP,                                  // i Dont know...
    EVENT_RIPLIMB_RESPAWN_H,
    EVENT_TAKING_SPEAR_DELAY,
    EVENT_RETRIEVE_SPEAR,

    //Rageface
    EVENT_CHANGE_TARGET,

    // Trigger for the Crystal Trap
    EVENT_CRYSTAL_TRAP_TRIGGER,
    EVENT_IMMOLATION_TRAP_TRIGGER,

    // Trigger for self Dispawn (Crystal Prison and Immolation Trap)
    EVENT_CRYSTAL_PRISON_DESPAWN,
    EVENT_IMMOLATION_TRAP_DESPAWN
};

enum ShannoxActions
{
    ACTION_RETRIEVE_SPEAR = 0,
    ACTION_TAKE_SPEAR,
    ACTION_RETURN_SPEAR
};

#define EQUIPMENT_SPEAR  53000
#define QUEST_DELEGATION 29234

class boss_shannox : public CreatureScript
{
public:
    boss_shannox() : CreatureScript("boss_shannox") {}

    struct boss_shannoxAI : public BossAI
    {
        boss_shannoxAI(Creature* creature) : BossAI(creature, DATA_SHANNOX) {}

        void ResetAchievement() // hackfix (not implemented) - reset achievement criteria for achievement "Bucket List"
        {
            instance->DoUpdateWorldState(5984, 0);
            instance->DoUpdateWorldState(5983, 0);
            instance->DoUpdateWorldState(5986, 0);
            instance->DoUpdateWorldState(5987, 0);
            instance->DoUpdateWorldState(5985, 0);
            bethtilacsLair = false;
            flamebreach = false;
            pathOfCorruption = false;
            theRidgeOfAncientFlame = false;
            Shatterstone = false;
            me->SetSpeed(MOVE_WALK, 3.2f);
            me->SetSpeed(MOVE_RUN, 1.71429f);
        }

        void Reset() override
        {
            if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
            {
                uint32 preNerfHealth = RAID_MODE(24069760, 81597402, 33669965, 114236357);
                me->SetMaxHealth(preNerfHealth);
                me->SetHealth(preNerfHealth);
            }
            _Reset();
            ResetAchievement();
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
            me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            me->GetMotionMaster()->MovePath(PATH_SHANNOX, true);
            me->SummonCreature(NPC_RIPLIMB, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
            me->SummonCreature(NPC_RAGEFACE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            SetEquipmentSlots(true, EQUIPMENT_SPEAR);
            canCastArcingSlash = false;
        }

        bool CanAIAttack(Unit const* target) const override
        {
            return !target->HasAura(CRYSTAL_PRISON_EFFECT);
        }

        void JustSummoned(Creature* summon) override
        {
            summon->setActive(true);

            switch (summon->GetEntry())
            {
                case NPC_RIPLIMB:
                    if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
                    {
                        uint32 preNerfHealth = summon->GetMaxHealth() / 0.85f;
                        summon->SetMaxHealth(preNerfHealth);
                        summon->SetHealth(preNerfHealth);
                    }
                    summon->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                    summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                    summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                    summon->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                    summon->GetMotionMaster()->MoveFollow(me, 1.0f, M_PI);
                    break;
                case NPC_RAGEFACE:
                    if (sWorld->getBoolConfig(CONFIG_DISABLE_420_NERFS))
                    {
                        uint32 preNerfHealth = summon->GetMaxHealth() / 0.85f;
                        summon->SetMaxHealth(preNerfHealth);
                        summon->SetHealth(preNerfHealth);
                    }
                    summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                    summon->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                    summon->GetMotionMaster()->MoveFollow(me, 1.0f, M_PI / 2);
                    break;
                case NPC_SHANNOX_SPEAR:
                    DoCast(summon, SPELL_HURL_SPEAR);
                    break;
                default:
                    break;
            }

            BossAI::JustSummoned(summon);
        }

        void SummonedCreatureDespawn(Creature* summon) override
        {
            if (summon->GetEntry() == NPC_RIPLIMB || summon->GetEntry() == NPC_RAGEFACE)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, summon);
            BossAI::SummonedCreatureDespawn(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit* killer) override
        {
            if (summon->GetEntry() == NPC_RIPLIMB || summon->GetEntry() == NPC_RAGEFACE)
            {
                DoCast(me, SPELL_FRENZY_SHANNOX, true);
                Talk(summon->GetEntry() == NPC_RIPLIMB ? TALK_RIP_DEAD : TALK_RAGE_DEAD);
            }

            if (summon->GetEntry() == NPC_RIPLIMB && !hasSpear)
                me->AI()->DoAction(ACTION_TAKE_SPEAR);

            if (summon->GetEntry() == NPC_RIPLIMB && IsHeroic())
                events.ScheduleEvent(EVENT_REVIVE_RIPLIMB, 30000);

            BossAI::SummonedCreatureDies(summon, killer);
        }

        void DoAction(int32 const actionId) override
        {
            switch (actionId)
            {
                case ACTION_TAKE_SPEAR:
                    if (Creature* riplimb = Creature::GetCreature(*me, instance->GetData64(NPC_RIPLIMB)))
                    {
                        if (Creature* spear = me->SummonCreature(NPC_SHANNOX_GRIP_SPEAR, riplimb->GetPositionX(), riplimb->GetPositionY(), riplimb->GetPositionZ(), riplimb->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 1000))
                        {
                            spear->AddAura(SPELL_SPEAR_VISUAL, spear);
                            DoCast(spear, SPELL_CALL_SPEAR, true);
                        }
                    }
                    SetEquipmentSlots(true);
                    hasSpear = true;
                    break;
            }
        }

        void DamageTaken(Unit* who, uint32 &damage) override
        {
            if (me->HealthBelowPctDamaged(30, damage) && !IsHeroic())
            {
                for (SummonList::iterator i = summons.begin(); i != summons.end(); ++i)
                    if (Creature* summon = ObjectAccessor::GetCreature(*me, *i))
                        if (summon->GetEntry() == NPC_RIPLIMB || summon->GetEntry() == NPC_RAGEFACE)
                            if (summon->isAlive())
                                summon->AddAura(SPELL_FRENZIED_DEVOLUTION, summon);
            }
        }

        void EnterCombat(Unit* victim) override
        {
            Talk(TALK_AGGRO);
            hasSpear = true;
            me->CallForHelp(40.0f);
            me->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            events.ScheduleEvent(EVENT_IMMOLATION_TRAP, 10000);
            events.ScheduleEvent(EVENT_ARCING_SLASH, 12000);
            events.ScheduleEvent(EVENT_HURL_SPEAR_OR_MAGMA_RUPTURE, 20000);
            events.ScheduleEvent(EVENT_SUMMON_CRYSTAL_PRISON, 25000);
            events.ScheduleEvent(EVENT_CHECK_PET_DISTANCE, 1000);
            events.ScheduleEvent(EVENT_BERSERK, 10 * MINUTE * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CHECK_LOCATION, 1000);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            for (SummonList::iterator i = summons.begin(); i != summons.end(); ++i)
                if (Creature* summon = ObjectAccessor::GetCreature(*me, *i))
                    if (summon->GetEntry() == NPC_RIPLIMB || summon->GetEntry() == NPC_RAGEFACE)
                        if (summon->isAlive())
                        {
                            summon->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, summon);
                            summon->AI()->AttackStart(victim);
                        }
            ResetAchievement();
            _EnterCombat();
        }

        void JustDied(Unit* killer) override
        {
            Talk(TALK_DEATH);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
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

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() != TYPEID_PLAYER || killtimer > 0)
                return;

            Talk(TALK_KILL);
            killtimer = 4000;
        }

        void EnterEvadeMode() override
        {
            Talk(TALK_EVADE);
            BossAI::EnterEvadeMode();
        }

        void DoMeleeAttackIfReady()
        {
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            Unit* victim = me->getVictim();
            //Make sure our attack is ready and we aren't currently casting before checking distance
            if (me->isAttackReady() && me->IsWithinMeleeRange(victim))
            {
                if (canCastArcingSlash)
                {
                    DoCastVictim(SPELL_ARCTIC_SLASH);
                    canCastArcingSlash = false;
                }
                else
                    me->AttackerStateUpdate(victim);
                me->resetAttackTimer();
            }

            if (me->haveOffhandWeapon() && me->isAttackReady(OFF_ATTACK) && me->IsWithinMeleeRange(victim))
            {
                me->AttackerStateUpdate(victim, OFF_ATTACK);
                me->resetAttackTimer(OFF_ATTACK);
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            if (killtimer >= diff)
                killtimer -= diff;

            events.Update(diff);

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                    case EVENT_ARCING_SLASH:
                        if (hasSpear)
                            canCastArcingSlash = true;
                        events.ScheduleEvent(EVENT_ARCING_SLASH, 11000);
                        break;
                    case EVENT_HURL_SPEAR_OR_MAGMA_RUPTURE:
                        if (Creature* riplimb = Creature::GetCreature(*me, instance->GetData64(NPC_RIPLIMB)))
                        {
                            if (riplimb->isDead())
                            {
                                DoCastVictim(SPELL_MAGMA_RUPTURE_SHANNOX);
                                events.ScheduleEvent(EVENT_HURL_SPEAR_OR_MAGMA_RUPTURE, urand(15000, 18000));
                                events.ScheduleEvent(EVENT_SUMMON_SPIRAL, 1500);
                            }
                            else
                            {
                                Talk(TALK_SPEAR_TOSS);
                                events.ScheduleEvent(EVENT_HURL_SPEAR_OR_MAGMA_RUPTURE, 42000);
                                DoCast(riplimb, SPELL_HURL_SPEAR_SUMMON, true);
                                riplimb->AI()->DoAction(ACTION_RETRIEVE_SPEAR);
                                SetEquipmentSlots(false, 0, 0, 0);
                                hasSpear = false;
                            }
                        }
                        break;
                    case EVENT_SUMMON_SPIRAL:
                    {
                        float X, Y, Z;
                        float degree = 0;
                        for (float width = 0; width <= 48; width += 0.3f)
                        {
                            degree += 0.174f;
                            if (degree >= 6.28f)
                                degree -= 6.28f;
                            Y = sin(degree)*width;
                            X = cos(degree)*width;
                            Z = me->GetMap()->GetHeight(me->GetPositionX() + X, me->GetPositionY() + Y, me->GetPositionZ());
                            me->CastSpell(me->GetPositionX() + X, me->GetPositionY() + Y, Z, SPELL_MAGMA_RUPTURE_VISUAL, true);
                        }
                        break;
                    }
                    case EVENT_SUMMON_CRYSTAL_PRISON:
                        DoCastRandom(SPELL_CRYSTAL_PRISON_TRAP, -5.0f);
                        events.ScheduleEvent(EVENT_SUMMON_CRYSTAL_PRISON, urand(23000, 27000));
                        break;
                    case EVENT_IMMOLATION_TRAP:
                        DoCastRandom(SPELL_IMMOLATION_TRAP, -5.0f);
                        events.ScheduleEvent(EVENT_IMMOLATION_TRAP, urand(10000, 35000));
                        break;
                    case EVENT_RIPLIMB_RESPAWN_H:
                        if (Creature* riplimb = Creature::GetCreature(*me, instance->GetData64(NPC_RIPLIMB)))
                        {
                            riplimb->Respawn();
                            riplimb->SetInCombatWithZone();
                        }
                        break;
                    case EVENT_CHECK_PET_DISTANCE:
                        if (Creature* riplimb = Creature::GetCreature(*me, instance->GetData64(NPC_RIPLIMB)))
                            if (riplimb->isAlive() && riplimb->GetDistance2d(me) >= 60.f && !me->HasAura(SPELL_SEPERATION_ANXIETY))
                                DoCast(me, SPELL_SEPERATION_ANXIETY);

                        if (Creature* rageface = Creature::GetCreature(*me, instance->GetData64(NPC_RAGEFACE)))
                            if (rageface->isAlive() && rageface->GetDistance2d(me) >= 60.f && !me->HasAura(SPELL_SEPERATION_ANXIETY))
                                DoCast(me, SPELL_SEPERATION_ANXIETY);

                        events.ScheduleEvent(EVENT_CHECK_PET_DISTANCE, 500);
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        for (SummonList::iterator i = summons.begin(); i != summons.end(); ++i)
                            if (Creature* summon = ObjectAccessor::GetCreature(*me, *i))
                                if (summon->GetEntry() == NPC_RIPLIMB || summon->GetEntry() == NPC_RAGEFACE)
                                    if (summon->isAlive())
                                        summon->CastSpell(summon, SPELL_BERSERK, true);
                        break;
                    case EVENT_CHECK_LOCATION:
                    {
                        switch (me->GetAreaId())
                        {
                            case 5821:
                                instance->DoUpdateWorldState(5986, 1);
                                pathOfCorruption = true;
                                break;
                            case 5764:
                                instance->DoUpdateWorldState(5984, 1);
                                bethtilacsLair = true;
                                break;
                            case 5765:
                                instance->DoUpdateWorldState(5983, 1);
                                flamebreach = true;
                                break;
                            case 5791:
                                instance->DoUpdateWorldState(5987, 1);
                                theRidgeOfAncientFlame = true;
                                break;
                            case 5766:
                                instance->DoUpdateWorldState(5985, 1);
                                Shatterstone = true;
                                break;
                            default:
                                break;
                        }
                        events.ScheduleEvent(EVENT_CHECK_LOCATION, 1000);
                        break;
                    }
                    case EVENT_REVIVE_RIPLIMB:
                    {
                        uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_FRENZY_SHANNOX, me);
                        me->RemoveAuraFromStack(spellId);
                        for (SummonList::iterator i = summons.begin(); i != summons.end(); ++i)
                        {
                            if (Creature* summon = ObjectAccessor::GetCreature(*me, *i))
                            {
                                if (summon->GetEntry() == NPC_RIPLIMB)
                                {
                                    summon->setDeathState(JUST_RESPAWNED);
                                    summon->SetInCombatWithZone();
                                }
                            }
                        }
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }

        bool checkBucketList() const
        {
            return bethtilacsLair && flamebreach && pathOfCorruption && theRidgeOfAncientFlame && Shatterstone;
        }

    private:
        bool hasSpear, canCastArcingSlash;
        bool bethtilacsLair, flamebreach, pathOfCorruption, theRidgeOfAncientFlame, Shatterstone;
        uint32 killtimer;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_shannoxAI(creature);
    }
};

class npc_rageface : public CreatureScript
{
public:
    npc_rageface() : CreatureScript("npc_rageface"){}

    struct npc_ragefaceAI : public ScriptedAI
    {
        npc_ragefaceAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            DoZoneInCombat();
            me->CallForHelp(20.0f);
            events.ScheduleEvent(EVENT_CHANGE_TARGET, urand(2000, 5000));
            if (IsHeroic())
                DoCast(me, SPELL_FEEDING_FRENZY_H);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == EFFECT_MOTION_TYPE && id == EVENT_JUMP)
                DoCastVictim(SPELL_FACE_RAGE);
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->RemoveAurasDueToSpell(SPELL_FEEDING_FRENZY_H);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (Creature* shannox = Creature::GetCreature(*me, instance->GetData64(DATA_SHANNOX)))
                if (me->GetDistance2d(shannox) > 60.0f && !me->HasAura(SPELL_SEPERATION_ANXIETY))
                    DoCast(me, SPELL_SEPERATION_ANXIETY, true);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHANGE_TARGET:
                        if (Creature* shannox = Creature::GetCreature(*me, instance->GetData64(DATA_SHANNOX)))
                        {
                            Unit* target = shannox->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me, true));
                            if (!target) // Retail Verified - if there is no other target the maintank is hit
                                target = shannox->getVictim();

                            if (target)
                            {
                                me->getThreatManager().resetAllAggro();
                                me->AddThreat(target, 50000000.0f);
                                DoCast(target, SPELL_FACE_RAGE_JUMP);
                                shannox->AI()->Talk(TALK_FACERAGE);
                            }
                        }
                        events.ScheduleEvent(EVENT_CHANGE_TARGET, 30000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ragefaceAI(creature);
    }
};

class npc_riplimb : public CreatureScript //Riplimb
{
public:
    npc_riplimb() : CreatureScript("npc_riplimb") { }

    struct npc_riplimbAI : public ScriptedAI
    {
        npc_riplimbAI(Creature *creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void Reset() override
        {
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
            me->RemoveAllAuras();
            events.Reset();
            movementResetNeeded = false; // Is needed for correct execution of the Phases
            inTakingSpearPhase = false; // Is needed for correct execution of the Phases
        }

        void EnterCombat(Unit* /*who*/) override
        {
            DoZoneInCombat();
            me->CallForHelp(20.0f);
            events.ScheduleEvent(EVENT_LIMB_RIP, 12000); //TODO Find out the correct Time
            if (IsHeroic())
                DoCast(me, SPELL_FEEDING_FRENZY_H);
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->RemoveAurasDueToSpell(SPELL_FEEDING_FRENZY_H);
        }

        void DoAction(int32 const actionId) override
        {
            switch (actionId)
            {
                case ACTION_RETRIEVE_SPEAR:
                    events.ScheduleEvent(EVENT_RETRIEVE_SPEAR, 7000);
                    break;
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (Creature* shannox = Creature::GetCreature(*me, instance->GetData64(DATA_SHANNOX)))
                if (me->GetDistance2d(shannox) > 60.0f && !me->HasAura(SPELL_SEPERATION_ANXIETY))
                    DoCast(me, SPELL_SEPERATION_ANXIETY, true);

            if (me->HasAura(CRYSTAL_PRISON_EFFECT))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RETRIEVE_SPEAR:
                        me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
                        me->SetReactState(REACT_PASSIVE);
                        DoCast(me, SPELL_DOGGED_DETERMINATION, true);
                        if (Creature* spear = me->FindNearestCreature(NPC_SHANNOX_SPEAR, 200.0f, true))
                        {
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(0, spear->GetPositionX(), spear->GetPositionY(), spear->GetPositionZ());
                            events.CancelEvent(EVENT_LIMB_RIP);
                            events.ScheduleEvent(EVENT_TAKING_SPEAR_DELAY, 500);
                        }
                        break;
                    case EVENT_LIMB_RIP:
                        DoCastVictim(SPELL_LIMB_RIP);
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                        events.ScheduleEvent(EVENT_LIMB_RIP, urand(6000, 10000));
                        break;
                    case EVENT_TAKING_SPEAR_DELAY:
                        if (Creature* spear = me->FindNearestCreature(NPC_SHANNOX_SPEAR, 200.0f, true))
                        {
                            if (me->GetExactDist2d(spear) <= 1.0f)
                            {
                                DoCast(me, SPELL_FETCH_SPEAR);
                                spear->DespawnOrUnsummon();
                                events.ScheduleEvent(EVENT_HURL_SPEAR_3, 500);
                            }
                            else
                            {
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MovePoint(0, spear->GetPositionX(), spear->GetPositionY(), spear->GetPositionZ());
                                events.ScheduleEvent(EVENT_TAKING_SPEAR_DELAY, 500);
                            }
                        }
                        break;
                    case EVENT_HURL_SPEAR_3:
                        if (Creature* shannox = Creature::GetCreature(*me, instance->GetData64(DATA_SHANNOX)))
                        {
                            if (!me->HasAura(CRYSTAL_PRISON_EFFECT))
                            {
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MovePoint(0, shannox->GetPositionX(), shannox->GetPositionY(), shannox->GetPositionZ());
                            }

                            if (me->GetExactDist2d(shannox) < 15.0f)
                            {
                                me->RemoveAurasDueToSpell(SPELL_FETCH_SPEAR);
                                me->RemoveAurasDueToSpell(SPELL_DOGGED_DETERMINATION);
                                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, false);
                                shannox->GetAI()->DoAction(ACTION_TAKE_SPEAR);
                                me->SetReactState(REACT_AGGRESSIVE);
                                if (!me->HasAura(CRYSTAL_PRISON_EFFECT))
                                {
                                    me->GetMotionMaster()->Clear();
                                    me->GetMotionMaster()->MoveChase(me->getVictim());
                                }
                                events.ScheduleEvent(EVENT_LIMB_RIP, 12000);
                            }
                            else
                                events.ScheduleEvent(EVENT_HURL_SPEAR_3, 500);
                        }
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
        bool movementResetNeeded;
        bool inTakingSpearPhase;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_riplimbAI(creature);
    }
};

class npc_shannox_spear : public CreatureScript //Shannox Spear
{
public:
    npc_shannox_spear() : CreatureScript("npc_shannox_spear") { }

    struct npc_shannox_spearAI : public ScriptedAI
    {
        npc_shannox_spearAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        void InitializeAI() override
        {
            casted = false;
            timer = 4000;
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->AddAura(SPELL_FLAME_STRIKE_MARKER, me);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (timer <= diff && !casted)
            {
                casted = true;
                float X;
                float Y;
                float Z;
                float degree = 0;

                // Calculate a very large spiral
                for (float width = 0; width <= 48; width += 0.3f)
                {
                    degree += 0.174f;
                    if (degree >= 6.28f)
                        degree -= 6.28f;
                    Y = sin(degree)*width;
                    X = cos(degree)*width;
                    Z = me->GetMap()->GetHeight(me->GetPositionX() + X, me->GetPositionY() + Y, me->GetPositionZ());
                    me->GetMap();
                    me->CastSpell(me->GetPositionX() + X, me->GetPositionY() + Y, Z, SPELL_MAGMA_RUPTURE_VISUAL, true);
                }
                if (Creature* shannox = Creature::GetCreature(*me, instance->GetData64(DATA_SHANNOX)))
                    shannox->AI()->Talk(TALK_RUPTURE);

                me->AddAura(SPELL_SPEAR_VISUAL, me);
                me->RemoveAura(SPELL_FLAME_STRIKE_MARKER);
            }
            else
                timer -= diff;
        }

    private:
        uint32 timer;
        bool casted;
        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_shannox_spearAI(creature);
    }
};


class PetDamageEvent : public BasicEvent
{
public:
    PetDamageEvent(Unit* target) : prison(target) { }

    bool Execute(uint64 execTime, uint32 /*diff*/) override
    {
        prison->DealDamage(prison, prison->CountPctFromMaxHealth(10));
        prison->m_Events.AddEvent(this, execTime + 1000);
        return false;
    }
private:
    Unit* prison;
};

class npc_crystal_trap : public CreatureScript //Crystal Trap
{
public:
    npc_crystal_trap() : CreatureScript("npc_crystal_trap"){}

    struct npc_crystal_trapAI : public ScriptedAI
    {
        npc_crystal_trapAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* owner) override
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE);
            isDespawning = false;
            events.ScheduleEvent(EVENT_CRYSTAL_TRAP_TRIGGER, 2000);
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            if (isDespawning)
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CRYSTAL_TRAP_TRIGGER:
                    {
                        std::list<WorldObject*> targetList;
                        Trinity::AllWorldObjectsInRange objects(me, 0.5f);
                        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects);
                        me->VisitNearbyObject(0.5f, searcher);
                        targetList.sort(Trinity::ObjectDistanceOrderPred(me));
                        for (std::list<WorldObject*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                        {
                            if (Unit* target = (*i)->ToUnit())
                            {
                                if (!isDespawning && !target->HasAura(sSpellMgr->GetSpellIdForDifficulty(SPELL_WARY, me)) && target->isAlive())
                                {
                                    if (target->GetTypeId() == TYPEID_UNIT && (target->GetEntry() == NPC_RAGEFACE || target->GetEntry() == NPC_RIPLIMB) || target->GetTypeId() == TYPEID_PLAYER)
                                    {
                                        if (!IsHeroic() || IsHeroic() && !target->HasAura(SPELL_DOGGED_DETERMINATION))
                                        {
                                            if (Creature* prison = me->SummonCreature(NPC_CRYSTAL_PRISON, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN))
                                            {
                                                target->SummonGameObject(GO_ICE_BLOCK_COLLISION, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, DAY);
                                                prison->CastSpell(target, CRYSTAL_PRISON_EFFECT, true);
                                                if (target->GetTypeId() == TYPEID_UNIT)
                                                {
                                                    if (target->GetEntry() == NPC_RIPLIMB || target->GetEntry() == NPC_RAGEFACE)
                                                    {
                                                        target->CastSpell(target, SPELL_WARY, true);
                                                        prison->m_Events.AddEvent(new PetDamageEvent(prison), prison->m_Events.CalculateTime(1000));
                                                    }
                                                }
                                                isDespawning = true;
                                                me->DespawnOrUnsummon(100);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        events.ScheduleEvent(EVENT_CRYSTAL_TRAP_TRIGGER, 250);
                        break;
                    }
                }
            }
        }

    private:
        bool isDespawning;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_crystal_trapAI(creature);
    }
};

class npc_immolation_trap : public CreatureScript //Immolation Trap
{
public:
    npc_immolation_trap() : CreatureScript("npc_immolation_trap") { }

    struct npc_immolation_trapAI : public ScriptedAI
    {
        npc_immolation_trapAI(Creature* creature) : ScriptedAI(creature) { }

        void IsSummonedBy(Unit* owner) override
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE);
            isDespawning = false;
            events.ScheduleEvent(EVENT_IMMOLATION_TRAP_TRIGGER, 2000);
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            if (isDespawning)
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_IMMOLATION_TRAP_TRIGGER:
                    {
                        std::list<WorldObject*> targetList;
                        Trinity::AllWorldObjectsInRange objects(me, 0.5f);
                        Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, targetList, objects);
                        me->VisitNearbyObject(0.5f, searcher);
                        targetList.sort(Trinity::ObjectDistanceOrderPred(me));
                        for (std::list<WorldObject*>::const_iterator i = targetList.begin(); i != targetList.end(); ++i)
                        {
                            if (Unit* target = (*i)->ToUnit())
                            {
                                if (!isDespawning && !target->HasAura(sSpellMgr->GetSpellIdForDifficulty(SPELL_WARY, me)) && target->isAlive())
                                {
                                    if (target->GetTypeId() == TYPEID_UNIT && (target->GetEntry() == NPC_RAGEFACE || target->GetEntry() == NPC_RIPLIMB) || target->GetTypeId() == TYPEID_PLAYER)
                                    {
                                        if (target->GetTypeId() == TYPEID_UNIT)
                                            target->CastSpell(target, SPELL_WARY, true);

                                        DoCast(target, IMMOLATION_TRAP, true);
                                        isDespawning = true;
                                        me->DespawnOrUnsummon(100);
                                    }
                                }
                            }
                        }
                        events.ScheduleEvent(EVENT_IMMOLATION_TRAP_TRIGGER, 250);
                        break;
                    }
                }
            }
        }

    private:
        bool isDespawning;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_immolation_trapAI(creature);
    }
};

class spell_crystal_prison_effect : public SpellScriptLoader
{
public:
    spell_crystal_prison_effect() : SpellScriptLoader("spell_crystal_prison_effect") { }

private:
    class spell_crystal_prison_effect_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_crystal_prison_effect_AuraScript);

        void HandleEffectPeriodic(AuraEffect const* aurEff)
        {
            if (!GetCaster() || !GetCaster()->isAlive())
            {
                Map::PlayerList const &PlayerList = GetUnitOwner()->GetMap()->GetPlayers();
                if (!PlayerList.isEmpty())
                {
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if (Player* player = i->getSource())
                        {
                            if (player->GetQuestStatus(QUEST_DELEGATION) == QUEST_STATUS_INCOMPLETE) // legendary questline
                            {
                                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_CREATE_EMBERSTONE_FRAGMENT, true);
                                break;
                            }
                        }
                    }
                }
                Remove(AURA_REMOVE_BY_DEFAULT);
            }
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (Unit* owner = GetUnitOwner())
                owner->RemoveAllGameObjects();
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_crystal_prison_effect_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_crystal_prison_effect_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_crystal_prison_effect_AuraScript();
    }
};

class spell_face_rage : public SpellScriptLoader
{
public:
    spell_face_rage() : SpellScriptLoader("spell_face_rage") { }

    class spell_face_rage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_face_rage_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetDamageInfo())
                if (int(eventInfo.GetDamageInfo()->GetDamage()) >= GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetTarget()))
                    return true;
            return false;
        }

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            if (Unit* caster = GetCaster())
            {
                caster->RemoveAurasDueToSpell(sSpellMgr->GetSpellIdForDifficulty(SPELL_FACE_RAGE_BUFF, caster));
                caster->InterruptNonMeleeSpells(false, SPELL_FACE_RAGE);

                if (Unit* attacker = eventInfo.GetDamageInfo()->GetAttacker())
                {
                    caster->getThreatManager().resetAllAggro();
                    caster->AddThreat(attacker, 5000.0f);
                    caster->CastSpell(attacker, SPELL_RAGE, true);
                    caster->GetMotionMaster()->MoveChase(attacker);
                    caster->Attack(attacker, true);
                }
            }
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_face_rage_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(spell_face_rage_AuraScript::OnProc, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_face_rage_AuraScript();
    }
};

class spell_face_rage_damage : public SpellScriptLoader
{
public:
    spell_face_rage_damage() : SpellScriptLoader("spell_face_rage_damage") { }

    class spell_face_rage_damage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_face_rage_damage_AuraScript);

        void onPeriodicTick(AuraEffect const* aurEff)
        {
            if (AuraEffect* effect = GetAura()->GetEffect(EFFECT_1))
                effect->SetAmount(aurEff->GetBaseAmount() + (3000 * aurEff->GetTickNumber()));
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_face_rage_damage_AuraScript::onPeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_face_rage_damage_AuraScript();
    }
};

class spell_call_spear : public SpellScriptLoader
{
public:
    spell_call_spear() : SpellScriptLoader("spell_call_spear") { }

    class spell_call_spear_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_call_spear_SpellScript);

        void HandleScript(SpellEffIndex effIndex)
        {
            GetHitUnit()->CastSpell(GetCaster(), GetSpellInfo()->Effects[EFFECT_0].BasePoints, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_call_spear_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_call_spear_SpellScript();
    }
};

class spell_dogged_determination : public SpellScriptLoader
{
public:
    spell_dogged_determination() : SpellScriptLoader("spell_dogged_determination") { }

    class spell_dogged_determination_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dogged_determination_AuraScript);

        void OnPeriodic(AuraEffect const* /*aurEff*/)
        {
            if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_0))
            {
                int32 newAmount = std::min(aurEff->GetAmount() + 5, 100);
                aurEff->SetAmount(newAmount);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dogged_determination_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dogged_determination_AuraScript();
    }
};

class spell_shannox_rage : public SpellScriptLoader
{
public:
    spell_shannox_rage() : SpellScriptLoader("spell_shannox_rage") { }

    class spell_shannox_rage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_shannox_rage_SpellScript);

        void OnHitTarget(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->TauntApply(GetHitUnit());
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_shannox_rage_SpellScript::OnHitTarget, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    class spell_shannox_rage_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_shannox_rage_AuraScript);

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (Unit* caster = GetCaster())
                caster->TauntFadeOut(GetTarget());
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_shannox_rage_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_shannox_rage_SpellScript;
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_shannox_rage_AuraScript();
    }
};

typedef boss_shannox::boss_shannoxAI ShannoxAI;
class achievement_bucket_list : public AchievementCriteriaScript
{
public:
    achievement_bucket_list() : AchievementCriteriaScript("achievement_bucket_list") { }

    bool OnCheck(Player* /*source*/, Unit* target) override
    {
        if (!target)
            return false;

        if (ShannoxAI* shannoxAI = CAST_AI(ShannoxAI, target->GetAI()))
            return shannoxAI->checkBucketList();

        return false;
    }
};

class npc_firelands_teleporter : public CreatureScript
{
public:
    npc_firelands_teleporter() : CreatureScript("npc_firelands_teleporter") {}

    struct npc_firelands_teleporterAI : public PassiveAI
    {
        npc_firelands_teleporterAI(Creature* creature) : PassiveAI(creature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_HIDE_NAMEPLATE | UNIT_FLAG2_UNK2);
        }

        void DoAction(int32 const action) override
        {
            if (action == ACTION_ENABLE_TELEPORT)
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void OnSpellClick(Unit* player, bool& result) override
        {
            if (InstanceScript* instance = me->GetInstanceScript())
                if (instance->IsEncounterInProgress())
                    return;

            if (me->GetEntry() == NPC_FIRELANDS_TELEPORT_SHANNOX)
                player->NearTeleportTo(362.688f, -98.440f, 78.271f, 1.540690f);
            else
                player->NearTeleportTo(-358.892f, 206.179f, 51.5846f, 3.103633f);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_firelands_teleporterAI(creature);
    }
};

void AddSC_boss_shannox()
{
    new boss_shannox();
    new npc_rageface();
    new npc_riplimb();
    new npc_shannox_spear();
    new npc_crystal_trap();
    new npc_immolation_trap();
    new spell_crystal_prison_effect();
    new spell_face_rage();
    new spell_face_rage_damage();
    new spell_call_spear();
    new spell_dogged_determination();
    new spell_shannox_rage();
    new achievement_bucket_list();
    new npc_firelands_teleporter();
};
