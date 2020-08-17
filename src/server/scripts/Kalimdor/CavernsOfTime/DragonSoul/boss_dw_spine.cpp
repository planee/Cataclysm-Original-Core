#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "PassiveAI.h"
#include "SpellScript.h"
#include "MoveSplineInit.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "dragonsoul.h"

enum SpellsTick
{
    SPELL_SPAWNING_CONTROLLER       = 105003, // caster 53879 "Deathwing"
    SPELL_CHECK_FOR_PLAYERS         = 109022, // caster 53879 "Deathwing"
    SPELL_PET_HACK                  = 95278,  // caster 53879 "Deathwing"
    SPELL_SUMMON_SLIME              = 104999, // caster 53879 "Deathwing"
    SPELL_ROLL_CONTROL              = 105036, // caster 53879 "Deathwing"
    SPELL_ROLL_CONTROL_ROLL         = 105740, // caster 53879 "Deathwing"
    SPELL_ROLL_CONTROL_AURA         = 105777,
    SPELL_RIDE_VEHICLE_HARDCODED    = 46598,  // caster 53879 "Deathwing" - target 56161
    SPELL_TELEPORT_ALL_TO_GUNSHIP   = 106051, // cast on wipe

    SPELL_DW_TALK                   = 106300,
    SPELL_DW_ROAR                   = 106302,
    SPELL_DW_PLAY_MOVIE             = 104574,

    SPELL_ACTIVATE_SPAWMER          = 100341, // caster 56161 vehicle "Corruption" - target 53888
    SPELL_EJECT_ALL_PASSENGERS      = 68576,  // caster 56161 vehicle "Corruption"

    SPELL_SUMMON_GRASPING_TENDRIS   = 105510, // caster 53888 "Spawner"
    SPELL_GRASPING_TENDRIS_AURA     = 105563,
    SPELL_SUMMON_ELEMENTARY         = 105000, // caster 53888 "Spawner"

    SPELL_SEARING_PLASMA_INIT       = 109379, // caster 53891 "Corruption"
    SPELL_SEARING_PLASMA_TR         = 105479, // caster 53891 "Corruption"
    SPELL_FIERY_GRIP                = 105490, // caster 53891 "Corruption"

    SPELL_DECREASE_CRIT             = 64481,  // caster 53889 "Corrupted Blood"
    SPELL_BURST                     = 105219, // caster 53889 "Corrupted Blood"
    SPELL_RESIDUE                   = 105223, // caster 53889 "Corrupted Blood"
    SPELL_ABSORBED_BLOOD            = 105248, // caster 53889 "Corrupted Blood" - target vehicle 53890
    SPELL_ENERGIZE                  = 109083, // caster 53889 "Corrupted Blood" - target vehicle 53890

    SPELL_ACT_BLOOD_ABSORB_BAR      = 109329, // caster 53890 "Hideous Amalgamation"
    SPELL_ABSORB_BLOOD              = 105244, // caster 53890 "Hideous Amalgamation"
    SPELL_SUPERHEATED_NUCLEUS       = 105834, // caster 53890 "Hideous Amalgamation"
    SPELL_NUCLEAR_BLAST             = 105845, // caster 53890 "Hideous Amalgamation"
    SPELL_NUCLEAR_BLAST_TRIGGERED   = 105846, // caster 53890 "Hideous Amalgamation" - target NPC_BURNING_TENDONS_L
    SPELL_DEGRADATION               = 106005, // caster 53890 "Hideous Amalgamation" - HEROIC MODE
    SPELL_BLOOD_CORRUPTION_DEATH    = 106199, // caster 53890 "Hideous Amalgamation" - HEROIC MODE
    SPELL_BLOOD_CORRUPTION_EARTH    = 106200, // caster 53890 "Hideous Amalgamation" - HEROIC MODE
    SPELL_BLOOD_OF_NELTHARION       = 106213, // caster owner of SPELL_BLOOD_CORRUPTION_EARTH - HEROIC MODE
    SPELL_BLOOD_OF_DEATHWING        = 106201, // caster owner of SPELL_BLOOD_CORRUPTION_DEATH - HEROIC MODE
    SPELL_SEAL_ARMOR_BREACH_L       = 105848, // caster 53890 "Burning Tendons"
    SPELL_SEAL_ARMOR_BREACH_R       = 105847, // caster 53890 "Burning Tendons"
};

enum Texts
{
    EMOTE_WARN_ROLL_LEFT    = -54875,
    EMOTE_WARN_ROLL_RIGHT   = -54878,
    EMOTE_ROLL_LEFT         = -54879,
    EMOTE_ROLL_RIGHT        = -54880,
};

enum Events
{
    EVENT_SEARING_PLASNA            = 1,
    EVENT_FIERY_GRIP                = 2,
    EVENT_ACTIVATE_CORRUPTION       = 3,
    EVENT_SUMMON_CORRUPTED_BLOOD    = 4,
    EVENT_SEND_ENCOUNTER_FRAME      = 5,
    EVENT_PLAYER_CHECK              = 6,
    EVENT_BLOOD_CORRUPTION          = 7,
    EVENT_SUMMON_SLIME              = 8,
    EVENT_SUMMON_SLIME_2            = 9,
    EVENT_SUMMON_SLIME_3            = 10
};

enum Points
{
    POINT_SPAWNER = 1,
};

enum Actions
{
    ACTION_CORRUPTION                       = 0,
    ACTION_FINISH_FIGHT                     = 1,
    ACTION_ACTIVATE_CORRUPTION              = 2,
    ACTION_ROLL_RIGHT                       = 3,
    ACTION_ROLL_LEFT                        = 4,
    ACTION_CORRUPTED_BLOOD                  = 5,
    ACTION_ACTIVATE_FIRST_CORRUPTION        = 6,
    ACTION_REACTIVATE_ACTIVATE_CORRUPTION   = 7
};

enum Animations
{
    ANIM_BACKPLATE_RIGHT        = 1677,
    ANIM_LOOSE_BACKPLATE_RIGHT  = 1678,
    ANIM_LOOSE_BACKPLATE_LEFT   = 1680,
    ANIM_BACKPLATE_LEFT         = 1681
};

static const std::vector<uint8> rollRight = { 1, 0, 217, 0, 0, 0 };
static const std::vector<uint8> rollLeft = { 1, 0, 216, 0, 0, 0 };

Creature* GetNearCorruption(Unit* searcher, bool alive = true, float range = 5.0f)
{
    Creature* curruption = nullptr;
    curruption = searcher->FindNearestCreature(NPC_CORRUPTION, range, alive);
    if (!curruption)
        curruption = searcher->FindNearestCreature(NPC_CORRUPTION_VEH_L, range, alive);
    if (!curruption)
        curruption = searcher->FindNearestCreature(NPC_CORRUPTION_VEH_R, range, alive);
    return curruption;
}

class boss_dw_spine : public CreatureScript
{
public:
    boss_dw_spine() : CreatureScript("boss_dw_spine") { }

    struct boss_dw_spineAI : public BossAI
    {
        boss_dw_spineAI(Creature* creature) : BossAI(creature, DATA_SPINE_OF_DEATHWING)
        {
            init = true;
        }

        void ActivateNextCorruption(std::list<Creature*> spawnerList, bool first)
        {
            uint8 count = first ? 4 : 2;
            spawnerList.sort(Trinity::ObjectDistanceOrderPred(me));
            spawnerList.remove_if([](Creature* crea)
            {
                return crea->AI()->GetGUID();
            });
            if (spawnerList.size() > (count))
                spawnerList.resize(count);
            count = 0;
            for (Creature* crea : spawnerList)
            {
                if (first && count < 2)
                    crea->AI()->DoAction(ACTION_ACTIVATE_FIRST_CORRUPTION);
                else
                    crea->AI()->DoAction(ACTION_ACTIVATE_CORRUPTION);
                count++;
            }
        }

        void SendRoll(bool left)
        {
            if (!left)
            {
                Talk(EMOTE_ROLL_RIGHT, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                // me->SendMapObjectEvents(6574436, 1, 0, 60); // head right
                me->SendMapObjectEvents(6574436, rollRight);// roll right
            }
            else
            {
                Talk(EMOTE_ROLL_LEFT, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                // me->SendMapObjectEvents(6574436, 1, 0, 60);
                me->SendMapObjectEvents(6574436, rollLeft);// roll left
            }
            DoCast(SPELL_ROLL_CONTROL_ROLL);
            instance->SetData(DATA_DW_SPINE_ROLL_INFO, 0);

            std::list<Creature*> creatureList;
            GetCreatureListWithEntryInGrid(creatureList, me, NPC_HIDEOUS_AMALGAMATION, 200.0f);
            // GetCreatureListWithEntryInGrid(creatureList, me, NPC_CORRUPTED_BLOOD, 200.0f);
            for (Creature* crea : creatureList)
                crea->GetMotionMaster()->ForceMoveJump(-13875.01f, -13769.59f, 285.1783f, 15.0f, 20.0f);
        }

        void Reset() override
        {
            _Reset();
            me->RemoveAllAuras();
            me->CombatStop();
            corruptedBlood = false;
            std::list<Creature*> spawnerList;
            GetCreatureListWithEntryInGrid(spawnerList, me, NPC_DW_SPINE_SPAWNER, 200.0f);
            auto copyList = spawnerList;
            for (Creature* crea : spawnerList)
                crea->AI()->Reset();
            ActivateNextCorruption(copyList, true);

            std::list<Creature*> trendonList;
            GetCreatureListWithEntryInGrid(trendonList, me, NPC_BURNING_TENDONS_L, 200.0f);
            GetCreatureListWithEntryInGrid(trendonList, me, NPC_BURNING_TENDONS_R, 200.0f);
            for (Creature* crea : trendonList)
            {
                crea->Respawn();
                crea->SetHealth(crea->GetMaxHealth());
            }

            for (uint8 i = 0; i < 3; i++)
                if (GameObject* go = instance->GetGameObject(DATA_DW_SPINE_BACKPLATE_1 + i))
                    go->SetGoState(GO_STATE_READY);
            instance->SetData(DATA_DW_SPINE_BACKPLATE_CNT, 0);
            instance->SetData(DATA_DW_SPINE_ROLL_INFO, 0);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEGRADATION);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOOD_OF_NELTHARION);
            me->SetHealth(me->GetMaxHealth());
            RemoveEncounterFrame();
        }

        void MoveInLineOfSight(Unit* victim)
        {
            if (victim->GetTypeId() != TYPEID_PLAYER)
                return;

            if (init)
            {
                EnterEvadeMode();
                init = false;
                return;
            }

            if (!me->isInCombat())
                EnterCombat(victim);
        }

        void DoAction(int32 const actionid) override
        {
            switch (actionid)
            {
                case ACTION_CORRUPTION:
                {
                    std::list<Creature*> spawnerList;
                    GetCreatureListWithEntryInGrid(spawnerList, me, NPC_DW_SPINE_SPAWNER, 200.0f);
                    ActivateNextCorruption(spawnerList, false);
                    me->SetHealth(me->GetHealth() - me->GetMaxHealth() / 3);
                    phase += 1;
                    break;
                }
                case ACTION_FINISH_FIGHT:
                {
                    DoCast(SPELL_DW_PLAY_MOVIE);
                    me->DespawnCreaturesInArea(NPC_CORRUPTION);
                    me->DespawnCreaturesInArea(NPC_CORRUPTED_BLOOD);
                    me->DespawnCreaturesInArea(NPC_HIDEOUS_AMALGAMATION);
                    me->DespawnCreaturesInArea(NPC_CORRUPTION_VEH_L);
                    me->DespawnCreaturesInArea(NPC_CORRUPTION_VEH_R);
                    me->DespawnOrUnsummon(1000);
                    RemoveEncounterFrame();
                    uint32 spellId = sSpellMgr->GetSpellIdForDifficulty(SPELL_SEARING_PLASMA_TR, me);
                    instance->DoRemoveAurasDueToSpellOnPlayers(spellId);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEGRADATION);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOOD_OF_NELTHARION);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOOD_CORRUPTION_DEATH, AURA_REMOVE_BY_ENEMY_SPELL);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOOD_OF_DEATHWING, AURA_REMOVE_BY_ENEMY_SPELL);
                    instance->SetBossState(DATA_SPINE_OF_DEATHWING, IsHeroic() ? DONE_HM : DONE);
                    instance->instance->LoadGrid(-12077.301f, 12165.879f); //make sure chest is loaded
                    if (GameObject* chest = instance->GetGameObject((instance->GetData(DATA_RAID_MODE) == RAID_MODE_LFR) ? DATA_SPINE_CHEST_LFR : DATA_SPINE_CHEST))
                    {
                        chest->SetRespawnTime(chest->GetRespawnDelay());
                        chest->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        if (Player* player = me->FindNearestPlayer(200.0f))
                            chest->SetLootRecipient(player);
                    }

                    instance->CompleteGuildCriteriaForGuildGroup(18486);

                    if (instance->GetData(DATA_RAID_MODE) == RAID_MODE_NORMAL)
                    {
                        Map::PlayerList const &playerList = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
                        {
                            if (Player* player = i->getSource())
                            {
                                player->ModifyCurrency(CURRENCY_TYPE_VALOR_POINTS, Is25ManRaid() ? 13500 : 11500);
                                player->ModifyCurrency(CURRENCY_TYPE_MOTE_OF_DARKNESS, 1);
                            }
                        }
                    }
                    break;
                }
                case ACTION_ROLL_RIGHT:
                    SendRoll(false);
                    break;
                case ACTION_ROLL_LEFT:
                    SendRoll(true);
                    break;
                case ACTION_CORRUPTED_BLOOD:
                    if (!corruptedBlood)
                    {
                        events.ScheduleEvent(EVENT_SUMMON_CORRUPTED_BLOOD, 9000);
                        corruptedBlood = true;
                    }
                    break;
            }
        }

        void EnterCombat(Unit* victim) override
        {
            DoCast(SPELL_SPAWNING_CONTROLLER);
            DoCast(SPELL_ROLL_CONTROL);
            DoCast(SPELL_PET_HACK);
            AddEncounterFrame();
            events.ScheduleEvent(EVENT_SEND_ENCOUNTER_FRAME, 1000);
            events.ScheduleEvent(EVENT_PLAYER_CHECK, 5000);
            phase = 1;
            _corruptedSummoned = 0;
            _EnterCombat();
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_CORRUPTED_BLOOD:
                    {
                        std::list<Creature*> spawnerList;
                        GetCreatureListWithEntryInGrid(spawnerList, me, NPC_DW_SPINE_SPAWNER, 200.0f);
                        spawnerList.remove_if([](WorldObject* target)
                        {
                            return !target->ToUnit()->HasAura(SPELL_SUMMON_GRASPING_TENDRIS);
                        });

                        if (!spawnerList.empty())
                        {
                            Unit* target = Trinity::Containers::SelectRandomContainerElement(spawnerList);
                            if (target)
                                target->CastSpell(target, SPELL_SUMMON_SLIME, true);
                        }

                        uint32 timer = 9000;
                        if (_corruptedSummoned <= 90)
                            _corruptedSummoned++;

                        if (IsHeroic())
                        {
                            if (_corruptedSummoned <= 9)
                                timer = 9000;
                            else if (_corruptedSummoned <= 9+11)
                                timer = 8000;
                            else if (_corruptedSummoned <= 9+11+13)
                                timer = 7000;
                            else if (_corruptedSummoned <= 9+11+13+15)
                                timer = 6000;
                            else if (_corruptedSummoned <= 9+11+13+15+26)
                                timer = 5000;
                            else if (_corruptedSummoned <= 9+11+13+15+26+12)
                                timer = 4000;
                            else if (_corruptedSummoned <= 9+11+13+15+26+12+4)
                                timer = 3000;
                            else
                                timer = 2000;
                        }

                        events.ScheduleEvent(EVENT_SUMMON_CORRUPTED_BLOOD, timer);
                        break;
                    }
                    case EVENT_SEND_ENCOUNTER_FRAME:
                        AddEncounterFrame();
                        events.ScheduleEvent(EVENT_SEND_ENCOUNTER_FRAME, 5000);
                        break;
                    case EVENT_PLAYER_CHECK:
                        DoCastAOE(SPELL_CHECK_FOR_PLAYERS);
                        events.ScheduleEvent(EVENT_PLAYER_CHECK, 5000);
                        break;
                    default:
                        break;
                }
            }
        }

        private:
            bool init;
            bool corruptedBlood;
            uint32 phase;
            uint32 _corruptedSummoned;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_dw_spineAI(creature);
    }
};

class npc_ds_corruption : public CreatureScript
{
public:
    npc_ds_corruption() : CreatureScript("npc_ds_corruption") {}

    struct npc_ds_corruptionAI : public Scripted_NoMovementAI
    {
        npc_ds_corruptionAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            instance = me->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void Reset() override
        {
            damageTaken = 0;
            casts = 0;
            isUnactive = false;
        }

        void JustDied(Unit* killer) override
        {
            Creature* spawner = me->FindNearestCreature(NPC_DW_SPINE_SPAWNER, 5.0f);
            if (!spawner)
                return;

            spawner->SetInCombatWithZone();
            spawner->CastSpell(spawner, SPELL_SUMMON_GRASPING_TENDRIS, true);
            spawner->CastWithDelay(urand(4500, 5000), spawner, SPELL_SUMMON_ELEMENTARY, true);
            me->CastSpell(spawner, SPELL_ACTIVATE_SPAWMER, true);
            if (Creature* deathwing = instance->GetCreature(DATA_SPINE_OF_DEATHWING))
                deathwing->AI()->DoAction(ACTION_CORRUPTED_BLOOD);

            Creature* corruption = GetNearCorruption(me, true, 200.0f);
            if (!corruption)
            {
                std::list<Creature*>creatureList;
                GetCreatureListWithEntryInGrid(creatureList, me, NPC_DW_SPINE_SPAWNER, 200.0f);
                creatureList.remove_if([spawner](Creature* crea)
                {
                    return !crea->AI()->GetGUID() || crea == spawner;
                });
                if (!creatureList.empty())
                    corruption = Trinity::Containers::SelectRandomContainerElement(creatureList);
                if (corruption) // actually now its the spawner
                {
                    corruption->RemoveAllAuras();
                    corruption->AI()->DoAction(ACTION_ACTIVATE_CORRUPTION);
                }
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage)
        {
            if (me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                if ((damageTaken += damage) > CalculatePct(me->GetMaxHealth(), 20))
                    me->InterruptSpell(CURRENT_CHANNELED_SPELL);
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case ACTION_ACTIVATE_FIRST_CORRUPTION:
                    isUnactive = true;
                    break;
                case ACTION_REACTIVATE_ACTIVATE_CORRUPTION:
                    // should trigger event npcs removed from tentacle
                    isUnactive = false;
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!me->IsVisible() || !UpdateVictim() || isUnactive)
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (instance->GetData(DATA_RAID_MODE) != RAID_MODE_LFR)
            {
                if (casts < (Is25ManRaid() ? 2 : 4))
                {
                    DoCast(SPELL_SEARING_PLASMA_INIT);
                    ++casts;
                }
                else
                {
                    damageTaken = 0;
                    casts = 0;
                    DoCast(SPELL_FIERY_GRIP);
                }
            }
        }

    private:
        InstanceScript* instance;
        uint32 damageTaken;
        uint8 casts;
        bool isUnactive;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_corruptionAI(creature);
    }
};

class npc_ds_hideous_amalgamation : public CreatureScript
{
public:
    npc_ds_hideous_amalgamation() : CreatureScript("npc_ds_hideous_amalgamation") {}

    struct npc_ds_hideous_amalgamationAI : public ScriptedAI
    {
        npc_ds_hideous_amalgamationAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            DoCast(SPELL_ACT_BLOOD_ABSORB_BAR);
            DoCast(SPELL_ABSORB_BLOOD);
            if (Unit* target = me->FindNearestPlayer(200.0f))
                AttackStart(target);
            canCastBlast = true;
            if (IsHeroic())
                events.ScheduleEvent(EVENT_BLOOD_CORRUPTION, 30000);
        }

        void JustDied(Unit* killer) override
        {
            if (IsHeroic())
                DoCast(me, SPELL_DEGRADATION, true);
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (me->HealthBelowPctDamaged(1, damage))
            {
                if (canCastBlast && me->HasAura(SPELL_SUPERHEATED_NUCLEUS))
                {
                    canCastBlast = false;
                    damage = 0;
                    me->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, true);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->SetReactState(REACT_PASSIVE);
                    me->CombatStop();
                    if (IsHeroic())
                        DoCast(me, SPELL_DEGRADATION, true);
                    DoCast(SPELL_NUCLEAR_BLAST);
                }
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == EFFECT_MOTION_TYPE && id == EVENT_JUMP)
                me->DespawnOrUnsummon();
        }

        void UpdateAI(uint32 const diff) override
        {
            events.Update(diff);

            while (uint32 eventid = events.ExecuteEvent())
            {
                switch (eventid)
                {
                case EVENT_BLOOD_CORRUPTION:
                    DoCastAOE(SPELL_BLOOD_CORRUPTION_DEATH);
                    events.ScheduleEvent(EVENT_BLOOD_CORRUPTION, 30000);
                    break;
                default:
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
        bool canCastBlast;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_hideous_amalgamationAI(creature);
    }
};

class npc_ds_corrupted_blood : public CreatureScript
{
public:
    npc_ds_corrupted_blood() : CreatureScript("npc_ds_corrupted_blood") {}

    struct npc_ds_corrupted_bloodAI : public ScriptedAI
    {
        npc_ds_corrupted_bloodAI(Creature* creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        void IsSummonedBy(Unit* /*summoner*/)
        {
            DoCast(SPELL_DECREASE_CRIT);
            canExplode = true;
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (!canExplode)
            {
                damage = 0;
            }
            else if (me->HealthBelowPctDamaged(1, damage))
            {
                canExplode = false;
                me->SetReactState(REACT_PASSIVE);
                me->CombatStop();
                damage = 0;
                // EnterEvadeMode();
                DoCast(SPELL_BURST);
                DoCast(me, SPELL_RESIDUE, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

                if (Creature* spawner = me->FindNearestCreature(NPC_DW_SPINE_SPAWNER, 100.00f))
                {
                    me->SetWalk(true);
                    Position pos(*spawner);
                    pos.m_positionZ = 264.50f;
                    me->GetMotionMaster()->MovePoint(POINT_SPAWNER, pos);
                }
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE && id == POINT_SPAWNER)
            {
                if (Creature* spawner = me->FindNearestCreature(NPC_DW_SPINE_SPAWNER, 100.00f))
                    spawner->CastSpell(spawner, SPELL_SUMMON_SLIME, true);

                me->DespawnOrUnsummon();
            }
        }
    private:
        bool canExplode;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_corrupted_bloodAI(creature);
    }
};

class npc_ds_burning_tendon : public CreatureScript
{
public:
    npc_ds_burning_tendon() : CreatureScript("npc_ds_burning_tendon") {}

    struct npc_ds_burning_tendonAI : public Scripted_NoMovementAI
    {
        npc_ds_burning_tendonAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->SetHealth(me->GetMaxHealth());
            instance = me->GetInstanceScript();
        }

        void Reset() override
        {
            RemoveEncounterFrame();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
        }

        void JustDied(Unit* killer) override
        {
            if (GameObject* backplate = instance->GetGameObject(DATA_DW_SPINE_BACKPLATE_1 + instance->GetData(DATA_DW_SPINE_BACKPLATE_CNT)))
            {
                if (me->GetEntry() == NPC_BURNING_TENDONS_L)
                    backplate->SendActivateAnimKit(ANIM_LOOSE_BACKPLATE_LEFT);
                else
                    backplate->SendActivateAnimKit(ANIM_LOOSE_BACKPLATE_RIGHT);

                backplate->SetGoState(GO_STATE_ACTIVE);
            }
            if (instance->GetData(DATA_DW_SPINE_BACKPLATE_CNT) == 2)
            {
                if (Creature* spine = instance->GetCreature(DATA_SPINE_OF_DEATHWING))
                    spine->AI()->DoAction(ACTION_FINISH_FIGHT);
                return;
            }
            if (Creature* spine = instance->GetCreature(DATA_SPINE_OF_DEATHWING))
                spine->AI()->DoAction(ACTION_CORRUPTION);
            instance->SetData(DATA_DW_SPINE_BACKPLATE_CNT, instance->GetData(DATA_DW_SPINE_BACKPLATE_CNT) + 1);
            if (Creature* tendron = me->FindNearestCreature(me->GetEntry() == NPC_BURNING_TENDONS_L ? NPC_BURNING_TENDONS_R : NPC_BURNING_TENDONS_L, 20.0f))
                tendron->DespawnOrUnsummon();
            RemoveEncounterFrame();
        }

    private:
        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_burning_tendonAI(creature);
    }
};

class npc_ds_spine_spawner : public CreatureScript
{
public:
    npc_ds_spine_spawner() : CreatureScript("npc_ds_spine_spawner") {}

    struct npc_ds_spine_spawnerAI : public Scripted_NoMovementAI
    {
        npc_ds_spine_spawnerAI(Creature* creature) : Scripted_NoMovementAI(creature), summons(me)
        {
            instance = me->GetInstanceScript();
        }

        void Reset() override
        {
            if (instance->GetBossState(DATA_SPINE_OF_DEATHWING) == NOT_STARTED)
            {
                summons.DespawnAll();
                me->RemoveAllAuras();
                corruptionGUID = 0;
            }
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case ACTION_ACTIVATE_CORRUPTION:
                    if (Creature* corruption = DoSummon(NPC_CORRUPTION, *me, 300000, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT))
                        corruptionGUID = corruption->GetGUID();
                    break;
                case ACTION_ACTIVATE_FIRST_CORRUPTION:
                    if (Creature* corruption = DoSummon(NPC_CORRUPTION, *me, 300000, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT))
                    {
                        corruptionGUID = corruption->GetGUID();
                        corruption->AI()->DoAction(ACTION_ACTIVATE_FIRST_CORRUPTION);
                    }
                    break;
                default:
                    break;
            }
        }

        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            summon->SetInCombatWithZone();
            me->SetInCombatWithZone();
        }

        uint64 GetGUID(int32 type = 0) const override
        {
            return corruptionGUID;
        }

    private:
        SummonList summons;
        InstanceScript* instance;
        uint64 corruptionGUID;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ds_spine_spawnerAI(creature);
    }
};

// 109022
class spell_dw_spine_check_player : public SpellScriptLoader
{
public:
    spell_dw_spine_check_player() : SpellScriptLoader("spell_dw_spine_check_player") { }

    class spell_dw_spine_check_player_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dw_spine_check_player_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
            {
                GetCaster()->CastSpell((Unit*)NULL, SPELL_TELEPORT_ALL_TO_GUNSHIP, true);
                GetCaster()->ToCreature()->AI()->EnterEvadeMode();
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dw_spine_check_player_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dw_spine_check_player_SpellScript();
    }
};

// 105241
class spell_ds_absorb_blood : public SpellScriptLoader
{
public:
    spell_ds_absorb_blood() : SpellScriptLoader("spell_ds_absorb_blood") {}

    class spell_ds_absorb_blood_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_absorb_blood_SpellScript);

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            uint8 stacks = 0;

            if (Aura* blood = GetCaster()->GetAura(SPELL_ABSORBED_BLOOD))
                stacks = blood->GetStackAmount();

            if (stacks < 9)
            {
                GetHitUnit()->CastSpell(GetCaster(), SPELL_ABSORBED_BLOOD, true);
                GetHitUnit()->CastSpell(GetCaster(), SPELL_ENERGIZE, true);
                GetHitCreature()->DespawnOrUnsummon(1000);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_absorb_blood_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_absorb_blood_SpellScript();
    }
};

// 105248
class spell_ds_absorb_blood_aura : public SpellScriptLoader
{
public:
    spell_ds_absorb_blood_aura() : SpellScriptLoader("spell_ds_absorb_blood_aura") { }

    class spell_ds_absorb_blood_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_absorb_blood_aura_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SUPERHEATED_NUCLEUS))
                return false;

            return true;
        }

        void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (GetStackAmount() == 9)
            {
                GetOwner()->ToUnit()->CastSpell(GetOwner()->ToUnit(), SPELL_SUPERHEATED_NUCLEUS, true);
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_ds_absorb_blood_aura_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_absorb_blood_aura_AuraScript();
    }
};

// 109379
class spell_ds_searing_plasma : public SpellScriptLoader
{
public:
    spell_ds_searing_plasma() : SpellScriptLoader("spell_ds_searing_plasma") {}

    class spell_ds_searing_plasma_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_searing_plasma_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Trinity::UnitAuraCheck(true, sSpellMgr->GetSpellIdForDifficulty(SPELL_SEARING_PLASMA_TR, GetCaster())));
            if (!targets.empty())
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->CastSpell(GetHitUnit(), GetEffectValue(), true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_searing_plasma_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_ds_searing_plasma_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_searing_plasma_SpellScript();
    }
};

// 105845
class spell_ds_nuclear_blast : public SpellScriptLoader
{
public:
    spell_ds_nuclear_blast() : SpellScriptLoader("spell_ds_nuclear_blast") {}

    class spell_ds_nuclear_blast_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_nuclear_blast_SpellScript);

        void HandleAfterCast()
        {
            GetCaster()->CastSpell(GetCaster(), SPELL_NUCLEAR_BLAST_TRIGGERED, true);
            GetCaster()->ToCreature()->DespawnOrUnsummon(500);
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_ds_nuclear_blast_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_ds_nuclear_blast_SpellScript();
    }
};

// 105846
class spell_ds_nuclear_blast_triggered : public SpellScriptLoader
{
public:
    spell_ds_nuclear_blast_triggered() : SpellScriptLoader("spell_ds_nuclear_blast_triggered") { }

    class spell_ds_nuclear_blast_triggered_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_nuclear_blast_triggered_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SEAL_ARMOR_BREACH_L) ||
                !sSpellMgr->GetSpellInfo(SPELL_SEAL_ARMOR_BREACH_R))
                return false;

            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (Unit* caster = GetCaster())
            {
                targets.remove_if([caster](WorldObject* target){
                    return caster->GetDistance2d(target) > 10.00f;
                });
            }

            targets.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));
            if (targets.size() > 1)
                targets.resize(1);
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
            {
                if (GameObject* backplate = instance->GetGameObject(DATA_DW_SPINE_BACKPLATE_1 + instance->GetData(DATA_DW_SPINE_BACKPLATE_CNT)))
                {
                    if (GetHitUnit()->GetEntry() == NPC_BURNING_TENDONS_L)
                    {
                        GetHitUnit()->CastSpell(GetHitCreature(), SPELL_SEAL_ARMOR_BREACH_L, false);
                        backplate->SendActivateAnimKit(ANIM_BACKPLATE_LEFT);
                    }
                    else
                    {
                        GetHitUnit()->CastSpell(GetHitCreature(), SPELL_SEAL_ARMOR_BREACH_R, false);
                        backplate->SendActivateAnimKit(ANIM_BACKPLATE_RIGHT);
                    }
                    GetHitUnit()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    GetHitCreature()->AI()->AddEncounterFrame();
                }
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_nuclear_blast_triggered_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_ds_nuclear_blast_triggered_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_nuclear_blast_triggered_SpellScript();
    }
};

// 104621
class spell_dw_spine_roll_control : public SpellScriptLoader
{
public:
    spell_dw_spine_roll_control() : SpellScriptLoader("spell_dw_spine_roll_control") { }

    class spell_dw_spine_roll_control_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dw_spine_roll_control_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROLL_CONTROL_ROLL))
                return false;

            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            isLeft = 0;
            isRight = 0;
            isMiddle = 0;

            targets.remove_if([](WorldObject* target)
            {
                return target->ToUnit()->HasAura(SPELL_ROLL_CONTROL_AURA);
            });

            for (WorldObject const* obj : targets)
            {
                if (obj->GetPositionX() <= -13861.0f)
                    ++isLeft;

                if (obj->GetPositionX() >= -13848.0f)
                    ++isRight;

                if (obj->GetPositionX() > -13861.0f && obj->GetPositionX() < -13848.0f)
                    ++isMiddle;
            }
        }

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
            {
                if (isLeft > (isRight + isMiddle))
                {
                    if (instance->GetData(DATA_DW_SPINE_ROLL_INFO) != ACTION_ROLL_LEFT)
                    {
                        GetCaster()->ToCreature()->AI()->Talk(EMOTE_WARN_ROLL_LEFT, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        std::list<Creature*> creatureList;
                        GetCaster()->GetCreatureListWithEntryInGrid(creatureList, NPC_CORRUPTION, 200.0f);
                        for (auto target : creatureList)
                            target->AI()->DoAction(ACTION_REACTIVATE_ACTIVATE_CORRUPTION);
                    }
                    instance->SetData(DATA_DW_SPINE_ROLL_INFO, ACTION_ROLL_LEFT);
                }
                else if (isRight > (isLeft + isMiddle))
                {
                    if (instance->GetData(DATA_DW_SPINE_ROLL_INFO) != ACTION_ROLL_RIGHT)
                    {
                        GetCaster()->ToCreature()->AI()->Talk(EMOTE_WARN_ROLL_RIGHT, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                        std::list<Creature*> creatureList;
                        GetCaster()->GetCreatureListWithEntryInGrid(creatureList, NPC_CORRUPTION, 200.0f);
                        for (auto target : creatureList)
                            target->AI()->DoAction(ACTION_REACTIVATE_ACTIVATE_CORRUPTION);
                    }
                    instance->SetData(DATA_DW_SPINE_ROLL_INFO, ACTION_ROLL_RIGHT);
                }
                else
                {
                    instance->SetData(DATA_DW_SPINE_ROLL_INFO, 0);
                    // GetCaster()->ToCreature()->AI()->Talk(EMOTE_WARN_ROLL_RIGHT, 0, CHAT_MSG_RAID_BOSS_EMOTE);
                }
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dw_spine_roll_control_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHit += SpellEffectFn(spell_dw_spine_roll_control_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
        }

    private:
        uint8 isLeft;
        uint8 isRight;
        uint8 isMiddle;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dw_spine_roll_control_SpellScript();
    }
};

// 105740
class spell_dw_spine_roll_control_roll : public SpellScriptLoader
{
public:
    spell_dw_spine_roll_control_roll() : SpellScriptLoader("spell_dw_spine_roll_control_roll") { }

    class spell_dw_spine_roll_control_roll_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dw_spine_roll_control_roll_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            uint32 graspingTendris = sSpellMgr->GetSpellIdForDifficulty(SPELL_GRASPING_TENDRIS_AURA, GetCaster());
            uint32 fieryGrip = sSpellMgr->GetSpellIdForDifficulty(SPELL_FIERY_GRIP, GetCaster());

            targets.remove_if([graspingTendris, fieryGrip](WorldObject* target)
            {
                return target->ToUnit()->HasAura(graspingTendris) || target->ToUnit()->HasAura(fieryGrip) || target->ToUnit()->HasAura(SPELL_ROLL_CONTROL_AURA);
            });

            for (auto target : targets)
                target->ToUnit()->GetMotionMaster()->ForceMoveJump(-13875.01f, -13769.59f, 285.1783f, 15.0f, 20.0f);

        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dw_spine_roll_control_roll_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dw_spine_roll_control_roll_SpellScript();
    }
};

// 105777
class spell_ds_roll_control_aura : public SpellScriptLoader
{
public:
    spell_ds_roll_control_aura() : SpellScriptLoader("spell_ds_roll_control_aura") { }

    class spell_ds_roll_control_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_roll_control_aura_AuraScript);

        void Remove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            GetTarget()->Kill(GetTarget());
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_ds_roll_control_aura_AuraScript::Remove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_roll_control_aura_AuraScript();
    }
};

// 105847, 105848
class spell_ds_seal_armor_breach : public SpellScriptLoader
{
public:
    spell_ds_seal_armor_breach() : SpellScriptLoader("spell_ds_seal_armor_breach") { }

    class spell_ds_seal_armor_breach_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_seal_armor_breach_AuraScript);

        void AfterApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            // GetCaster()->ToCreature()->AI()->EnterEvadeMode();
            // GetCaster()->SetHealth(GetCaster()->GetMaxHealth());
            GetCaster()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            GetCaster()->ToCreature()->AI()->RemoveEncounterFrame();
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
            {
                if (GameObject* backplate = instance->GetGameObject(DATA_DW_SPINE_BACKPLATE_1 + instance->GetData(DATA_DW_SPINE_BACKPLATE_CNT)))
                {
                    backplate->SetGoState(GO_STATE_READY);
                }
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(spell_ds_seal_armor_breach_AuraScript::AfterApply, EFFECT_2, SPELL_AURA_SCHOOL_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_seal_armor_breach_AuraScript();
    }
};

// 106199, 106200
class spell_ds_blood_corruption_aura : public SpellScriptLoader
{
public:
    spell_ds_blood_corruption_aura() : SpellScriptLoader("spell_ds_blood_corruption_aura") { }

    class spell_ds_blood_corruption_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ds_blood_corruption_aura_AuraScript);

        bool Validate(SpellInfo const* spellInfo) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_BLOOD_OF_NELTHARION) ||
                !sSpellMgr->GetSpellInfo(SPELL_BLOOD_OF_DEATHWING))
                return false;

            return true;
        }

        void Dispel(DispelInfo* dispelInfo)
        {
            // Blood Corruption: Death generally must be dispelled 3-4 times before it turns into Blood Corruption: Earth
            uint32 aura = SPELL_BLOOD_CORRUPTION_DEATH;
            if (GetEffect(EFFECT_0)->GetAmount() >= 3 && (urand(0,100) < 50))
                aura = SPELL_BLOOD_CORRUPTION_EARTH;

            CustomSpellValues values;
            values.AddSpellMod(SPELLVALUE_DURATON, GetDuration());
            values.AddSpellMod(SPELLVALUE_MAXDURATION, GetMaxDuration());
            values.AddSpellMod(SPELLVALUE_BASE_POINT0, GetEffect(EFFECT_0)->GetAmount() + 1);
            // hotfix 4.3.4: Blood Corruption: Earth will no longer morph into Blood Corruption: Death when dispelled.
            GetOwner()->ToUnit()->CastCustomSpell((GetId() == SPELL_BLOOD_CORRUPTION_EARTH) ? GetId() : aura, values, (Unit*)NULL, TRIGGERED_FULL_MASK, NULL, NULL, GetCasterGUID());
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_ENEMY_SPELL)
            {
                GetTarget()->CastSpell(GetTarget(), GetId() == SPELL_BLOOD_CORRUPTION_DEATH ? SPELL_BLOOD_OF_DEATHWING : SPELL_BLOOD_OF_NELTHARION, true);
                // Blood Corruption: Earth now applies two stacks of Blood of Neltharion when it expires on 25-player heroic difficulty.
                if (GetTarget()->GetMap()->GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC && GetId() == SPELL_BLOOD_CORRUPTION_EARTH)
                    GetTarget()->CastSpell(GetTarget(), SPELL_BLOOD_OF_NELTHARION, true);
            }
        }

        void Register() override
        {
            OnDispel += AuraDispelFn(spell_ds_blood_corruption_aura_AuraScript::Dispel);
            OnEffectRemove += AuraEffectRemoveFn(spell_ds_blood_corruption_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    class spell_ds_blood_corruption_aura_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_blood_corruption_aura_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targetlist)
        {
            uint8 count = GetCaster()->ToCreature() ? (GetCaster()->GetMap()->Is25ManRaid() ? 3 : 1) : 1;
            targetlist.remove_if(Trinity::UnitAuraCheck(true, GetSpellInfo()->Id == SPELL_BLOOD_CORRUPTION_DEATH ? SPELL_BLOOD_CORRUPTION_EARTH : SPELL_BLOOD_CORRUPTION_DEATH));
            targetlist.remove(GetCaster());
            if (!targetlist.empty())
                Trinity::Containers::RandomResizeList(targetlist, count);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ds_blood_corruption_aura_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ds_blood_corruption_aura_AuraScript();
    }

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_blood_corruption_aura_SpellScript();
    }
};

class spell_ds_energize_spine : public SpellScriptLoader
{
public:
    spell_ds_energize_spine() : SpellScriptLoader("spell_ds_energize_spine") { }

    class spell_ds_energize_spine_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ds_energize_spine_SpellScript);

        void HandleHit(SpellEffIndex /*effIndex*/)
        {
            int32 alternativePower = std::min((GetHitUnit()->GetPower(POWER_ALTERNATE_POWER) + 1), 9);
            GetHitUnit()->SetPower(POWER_ALTERNATE_POWER, alternativePower);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_ds_energize_spine_SpellScript::HandleHit, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_ds_energize_spine_SpellScript();
    }
};

void AddSC_boss_dw_spine()
{
    new boss_dw_spine();
    new npc_ds_corruption();
    new npc_ds_corrupted_blood();
    new npc_ds_hideous_amalgamation();
    new npc_ds_burning_tendon();
    new npc_ds_spine_spawner();
    new spell_dw_spine_check_player();
    new spell_ds_absorb_blood();
    new spell_ds_absorb_blood_aura();
    new spell_ds_nuclear_blast();
    new spell_ds_nuclear_blast_triggered();
    new spell_dw_spine_roll_control();
    new spell_dw_spine_roll_control_roll();
    new spell_ds_roll_control_aura();
    new spell_ds_searing_plasma();
    new spell_ds_seal_armor_breach();
    new spell_ds_blood_corruption_aura();
    new spell_ds_energize_spine();
};
