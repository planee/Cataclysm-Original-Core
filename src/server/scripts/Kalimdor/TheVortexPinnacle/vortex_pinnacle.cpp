#include"ScriptPCH.h"
#include "the_vortex_pinnacle.h"

enum Creatures
{
    NPC_SLIPSTREAM_LANDING_ZONE     = 45504,
    NPC_SKYFALL_LAND                = 45981,

    NPC_CLOUD_PRINCE                = 45917,
    NPC_WIPPING_WIND                = 47238,
    NPC_YOUNG_STORM_DRAGON          = 45919,
    NPC_ARMORED_MISTRAL             = 45915,
    NPC_EMPYREAN_ASSASSIN           = 45922,
    NPC_EXECUTOR_OF_THE_CALIPH      = 45928,
    NPC_GUST_SOLDIER                = 45477,
    NPC_LURKING_TEMPEST             = 45704,
    NPC_MINISTER_OF_AIR             = 45930,
    NPC_SERVANT_OF_ASAAD            = 45926,
    NPC_TEMPEST_ADEPT               = 45935,
    NPC_TURBULENT_SQUALL            = 45924,
    NPC_HOWLING_GALE                = 45572,
    NPC_WILD_VORTEX                 = 45912,
    NPC_SKYFALL_STAR                = 45932,
    NPC_GOLDEN_ORB                  = 51157,
    NPC_ITESH                       = 49943,
    NPC_WORLD_TRIGGER_CATCH_FALL    = 21252,
};

enum Spells
{
    // Slipstream
    SPELL_SLIPSTREAM_AURA           = 85021,
    SPELL_SLIPSTREAM_VEH            = 87742,
    SPELL_RIDE_VEH_HARDCOODED       = 46598,

    // Cloud Prince
    SPELL_TYPHOON                   = 88074,
    SPELL_STARFALL                  = 88073,

    // Whipping Wind
    SPELL_WHW_LIGHTNING_BOLT        = 88080,

    // Young Storm Dragon
    SPELL_BRUTAL_STRIKES            = 88192,
    SPELL_BRUTAL_STRIKES_DMG        = 88188,
    SPELL_CHILLING_BLAST            = 88194,
    SPELL_HEALING_WELL              = 88201,

    // Armored Mistral
    SPELL_GALE_STRIKE               = 88061,
    SPELL_RISING_WINDS              = 88057,
    SPELL_STORM_SURGE               = 88055,

    // Empyrean Assassin
    SPELL_LETHARGIC_POISON          = 88184,
    SPELL_VAPOR_FORM                = 88186,

    // Executor of the Caliph
    SPELL_DEVASTATE                 = 78660,
    SPELL_RALLY                     = 87761,
    SPELL_SHOCKWAVE                 = 87759,
    
    // Gust Soldier
    SPELL_AIR_NOVA                  = 87933,
    SPELL_CHARGE                    = 87930,
    SPELL_WIND_BLAST                = 87923,

    // Lurking Tempest
    SPELL_LT_LIGHTNING_BOLT         = 89105,
    SPELL_FEIGN_DEATH               = 85267,
    SPELL_LURK                      = 85467,

    // Minister of Air
    SPELL_LIGHTNING_LASH            = 87762,
    SPELL_LIGHTNING_LASH_DUMMY      = 87765,
    SPELL_LIGHTNING_LASH_DMG        = 88963,
    SPELL_LIGHTNING_NOVA            = 87768,

    // Servant of Asaad
    SPELL_CRUSADER_STRIKE           = 87771,
    SPELL_DIVINE_STORM              = 58112,
    SPELL_HAND_OF_PROTECTION        = 87772,

    // Temple Adept
    SPELL_DESPERATE_SPEED           = 87780,
    SPELL_GREATER_HEAL              = 87779,
    SPELL_HOLY_SMITE                = 88959,

    // Turbulent Squall
    SPELL_ASPHYXIATE                = 88175,
    SPELL_CLOUDBURST                = 88170,
    SPELL_HURRICANE                 = 88171,

    // Wild Vortex
    SPELL_WV_LIGHTNING_BOLT         = 88032,
    SPELL_WIND_SHOCK                = 88029,
    SPELL_CYCLONE                   = 88010,

    // Skyfall Star
    SPELL_ARCANE_BARRAGE            = 87854,

    // Howling Gale
    SPELL_HOWLING_GALE              = 85084,
    SPELL_HOWLING_GALE_1            = 85086,
    SPELL_HOWLING_GALE_2            = 85136,
    SPELL_HOWLING_GALE_3            = 85086,
    SPELL_HOWLING_GALE_DMG          = 85159,

    // Fall Catcher
    SPELL_SLIPSTREAM_BUFF           = 87740,
    SPELL_CATCH_FALL_SUMMON_TRIGGER = 89517,
    SPELL_CATCH_FALL_SUMMON_AURA    = 89522,
    SPELL_CATCH_FALL_VEHICLE        = 89524,
    SPELL_CATCH_FALL_JUMP           = 89526,

    SPELL_ACHIEVEMENT               = 94756,
};

enum Events
{
    EVENT_ARCANE_BARRAGE            = 1,
    EVENT_TYPHOON                   = 2,
    EVENT_STARFALL                  = 3,
    EVENT_WHW_LIGHTNING_BOLT        = 4,
    EVENT_CHILLING_BLAST            = 5,
    EVENT_GALE_STRIKE               = 6,
    EVENT_STORM_SURGE               = 7,
    EVENT_VAPOR_FORM                = 8,
    EVENT_DEVASTATE                 = 9,
    EVENT_SHOCKWAVE                 = 10,
    EVENT_RALLY                     = 11,
    EVENT_AIR_NOVA                  = 12,
    EVENT_WIND_BLAST                = 13,
    EVENT_LIGHTNING_NOVA            = 14,
    EVENT_LIGHTNING_LASH            = 15,
    EVENT_CRUSADER_STRIKE           = 16,
    EVENT_HAND_OF_PROTECTION        = 17,
    EVENT_DESPERATE_SPEED           = 18,
    EVENT_GREATER_HEAL              = 19,
    EVENT_HOLY_SMITE                = 20,
    EVENT_ASPHYXIATE                = 21,
    EVENT_HURRICANE                 = 22,
    EVENT_CLOUDBURST                = 23,
    EVENT_CYCLONE                   = 24,
    EVENT_WIND_SHOCK                = 25,
    EVENT_WV_LIGHTNING_BOLT         = 26,
    EVENT_HOWLING_GALE              = 27,
    EVENT_HOWLING_GALE_DEACTIVATE   = 28,
};

const Position teleportPos[2] = 
{
    {-906.08f, -176.51f, 664.50f, 2.86f},
    {-1193.67f, 472.83f, 634.86f, 0.50f},
};

enum Other
{
    TYPE_SLIPSTREAM    = 1,
};

class SlipstreamOrderPred
{
    public:
        SlipstreamOrderPred(const Creature* object, bool ascending = true) : m_object(object), m_ascending(ascending) {}
        bool operator() (const Creature* a, const Creature* b) const
        {
            return m_ascending ? a->GetDistance(m_object) < b->GetDistance(m_object) :
                a->GetDistance(m_object) > b->GetDistance(m_object);
        }
    private:
        const Creature* m_object;
        const bool m_ascending;
};

class npc_vortex_pinnacle_slipsteam : public CreatureScript
{
    public:
        npc_vortex_pinnacle_slipsteam() : CreatureScript("npc_vortex_pinnacle_slipsteam") { }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            player->CastSpell(player, SPELL_SLIPSTREAM_AURA, true);
            creature->AI()->SetGUID(player->GetGUID());
            creature->AI()->DoAction(ACTION_INIT_SLIPSTREAM_PATH);
            return false;
        }

        struct npc_vortex_pinnacle_slipsteamAI : public ScriptedAI
        {
            npc_vortex_pinnacle_slipsteamAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetCanFly(true);
                SetCombatMovement(false);
            }

            TaskScheduler scheduler;
            uint64 passengerGUID;
            bool hasSecond;
            float myLocateZ;

            void Reset() override
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                DoCast(me, SPELL_SLIPSTREAM_AURA);
                passengerGUID = 0;
                hasSecond = false;
                myLocateZ = me->GetPositionZ();
            }

            void SetGUID(uint64 guid, int32 /*type*/) override
            {
                passengerGUID = guid;
            }

            uint64 GetGUID(int32 /*type*/) const override
            {
                return passengerGUID;
            }

            void DoAction(int32 actionId) override
            {
                if (actionId == ACTION_INIT_SLIPSTREAM_PATH)
                {
                    hasSecond = me->GetPositionX() < -1000.0f;

                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    if (Player* target = ObjectAccessor::GetPlayer(*me, GetGUID(0)))
                        target->CastSpell(me, SPELL_RIDE_VEH_HARDCOODED, true);

                    scheduler
                        .Schedule(Milliseconds(2500), [this](TaskContext context)
                    {
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                        // Select New Slipstream Depend of Encounter Progress
                        std::list<Creature*> slipList;
                        GetCreatureListWithEntryInGrid(slipList, me, NPC_SLIPSTREAM, 180.0f);
                        slipList.remove_if([=](Creature* target) { return target->GetEntry() != NPC_SLIPSTREAM; });
                        slipList.sort(SlipstreamOrderPred(me, true));

                        if (hasSecond)
                            slipList.remove_if([=](Creature* target) { return target->ToUnit() && target->ToUnit()->GetPositionZ() > myLocateZ - 1.0f; });
                        else
                            slipList.remove_if([=](Creature* target) { return target->ToUnit() && target->ToUnit()->GetPositionZ() < myLocateZ + 1.0f; });

                        if (Player* target = ObjectAccessor::GetPlayer(*me, GetGUID(0)))
                        {
                            if (slipList.empty()) // end of path
                            {
                                target->ExitVehicle();

                                if (Creature* endPath = me->FindNearestCreature(NPC_SLIPSTREAM_LANDING_ZONE, 150.0f, true))
                                    target->GetMotionMaster()->MoveJump(*endPath, 30.0f, 30.0f);
                            }
                            else
                            {
                                target->ExitVehicle();
                                target->CastSpell(slipList.front(), SPELL_RIDE_VEH_HARDCOODED, true);
                                slipList.front()->AI()->SetGUID(target->GetGUID());
                                slipList.front()->AI()->DoAction(ACTION_INIT_SLIPSTREAM_PATH);
                            }
                        }

                    });
                }
            }

            void UpdateAI(uint32 diff) override 
            {
                scheduler.Update(diff);
            }
     };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_vortex_pinnacle_slipsteamAI>(creature);
        }
};

// Star Sky 45932
struct npc_skyfall_star : public ScriptedAI
{
    npc_skyfall_star(Creature* creature) : ScriptedAI(creature)
    {
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        SetCombatMovement(false);
    }

    uint64 ownerGUID;
    TaskScheduler scheduler;

    void IsSummonedBy(Unit* summoner) override
    {
        ownerGUID = summoner->GetGUID();
    }

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
        scheduler.CancelAll();
        me->StopMoving();
        me->SetWalk(true);

        scheduler
            .Schedule(Milliseconds(1000), [this](TaskContext context)
        {
            if (Creature* owner = ObjectAccessor::GetCreature(*me, ownerGUID))
                me->GetMotionMaster()->MovePoint(0, owner->GetPositionX() + frand(-10.0f, 10.0f), owner->GetPositionY() + frand(-10.0f, 10.0f), owner->GetPositionZ());

            context.Repeat(Milliseconds(me->GetSplineDuration()));
        });
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->DisappearAndDie();
    }

    void EnterCombat(Unit* who) override
    {    
        std::list<Creature*> starsList;
        GetCreatureListWithEntryInGrid(starsList, me, NPC_STAR_SKY, 20.0f);

        for (auto&& itr : starsList)
            itr->Attack(who, false);

        scheduler
            .Schedule(Milliseconds(1000), Milliseconds(5000), [this](TaskContext context)
        {
            DoCast(SPELL_ARCANE_BARRAGE);
            context.Repeat(Milliseconds(3500), Milliseconds(10500));
        });
    }

    void UpdateAI(uint32 diff) override
    {
        scheduler.Update(diff);
    }
};

class npc_cloud_prince : public CreatureScript
{
    public:
        npc_cloud_prince() : CreatureScript("npc_cloud_prince") { }

        struct npc_cloud_princeAI : public ScriptedAI
        {
            npc_cloud_princeAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                // Wrong in Creature template 
                if (me->GetInstanceScript() && me->GetInstanceScript()->instance->GetSpawnMode() == DUNGEON_DIFFICULTY_NORMAL)
                {
                    // should be 446.780 in 5 normal
                    me->SetCreateHealth(446790);
                    me->SetMaxHealth(446790);
                    me->SetHealth(446790);
                    me->ResetPlayerDamageReq();
                }

                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                events.ScheduleEvent(EVENT_TYPHOON, urand(5000, 7000));
                events.ScheduleEvent(EVENT_STARFALL, urand(7000, 15000));
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
                        case EVENT_TYPHOON:
                            DoCast(SPELL_TYPHOON);
                            events.ScheduleEvent(EVENT_TYPHOON, urand(15000, 20000));
                            break;
                        case EVENT_STARFALL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_STARFALL);
                            events.ScheduleEvent(EVENT_STARFALL, 20000);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_cloud_princeAI>(creature);
        }
};

class npc_whipping_wind : public CreatureScript
{
    public:
        npc_whipping_wind() : CreatureScript("npc_whipping_wind") { }

        struct npc_whipping_windAI : public ScriptedAI
        {
            npc_whipping_windAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                events.ScheduleEvent(EVENT_WHW_LIGHTNING_BOLT, 2000);
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
                        case EVENT_WHW_LIGHTNING_BOLT:
                            DoCast(me->getVictim(), SPELL_WHW_LIGHTNING_BOLT);
                            events.ScheduleEvent(EVENT_WHW_LIGHTNING_BOLT, 2000);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_whipping_windAI>(creature);
        }
};

class npc_young_storm_dragon : public CreatureScript
{
    public:
        npc_young_storm_dragon() : CreatureScript("npc_young_storm_dragon") { }

        struct npc_young_storm_dragonAI : public ScriptedAI
        {
            npc_young_storm_dragonAI(Creature* creature) : ScriptedAI(creature)
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
            }

            EventMap events;

            void Reset() override
            {
                DoCast(me, SPELL_BRUTAL_STRIKES);
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                DoCast(me, SPELL_HEALING_WELL);
                events.ScheduleEvent(EVENT_CHILLING_BLAST, urand(12000, 15000));
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
                        case EVENT_CHILLING_BLAST:
                            DoCast(me->getVictim(), SPELL_CHILLING_BLAST);
                            events.ScheduleEvent(EVENT_CHILLING_BLAST, urand(15000, 18000));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_young_storm_dragonAI>(creature);
        }
};

class npc_armored_mistral : public CreatureScript
{
    public:
        npc_armored_mistral() : CreatureScript("npc_armored_mistral") { }

        struct npc_armored_mistralAI : public SmartCreatureAI
        {
            npc_armored_mistralAI(Creature* creature) : SmartCreatureAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
                me->SetReactState(REACT_AGGRESSIVE);
            }

            void EnterCombat(Unit* /*who*/) override
            {
                me->CallForHelp(16.0f);
                DoCast(me, SPELL_RISING_WINDS);
                events.ScheduleEvent(EVENT_GALE_STRIKE, urand(2000, 4000));
                events.ScheduleEvent(EVENT_STORM_SURGE, urand(10000, 15000));    
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
                    ExecuteTargetEvent(SPELL_GALE_STRIKE, urand(15000, 20000), EVENT_GALE_STRIKE, eventId);
                    ExecuteTargetEvent(SPELL_STORM_SURGE, urand(15000, 20000), EVENT_STORM_SURGE, eventId, PRIORITY_SELF);
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_armored_mistralAI>(creature);
        }
};

class npc_empyrean_assassin : public CreatureScript
{
    public:
        npc_empyrean_assassin() : CreatureScript("npc_empyrean_assassin") { }

        struct npc_empyrean_assassinAI : public ScriptedAI
        {
            npc_empyrean_assassinAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                DoCast(me, SPELL_LETHARGIC_POISON);
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                events.ScheduleEvent(EVENT_VAPOR_FORM, urand(15000, 20000));    
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
                        case EVENT_VAPOR_FORM:
                            DoCast(me, SPELL_VAPOR_FORM);
                            events.ScheduleEvent(EVENT_VAPOR_FORM, urand(20000, 25000));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_empyrean_assassinAI>(creature);
        }
};

class npc_executor_of_the_caliph : public CreatureScript
{
    public:
        npc_executor_of_the_caliph() : CreatureScript("npc_executor_of_the_caliph") { }

        struct npc_executor_of_the_caliphAI : public ScriptedAI
        {
            npc_executor_of_the_caliphAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                me->CallForHelp(23.5f);
                events.ScheduleEvent(EVENT_RALLY, urand(5000, 20000));
                events.ScheduleEvent(EVENT_DEVASTATE, urand(2000, 8000));
                events.ScheduleEvent(EVENT_SHOCKWAVE, urand(12000, 20000));
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
                        case EVENT_RALLY:
                            DoCast(SPELL_RALLY);
                            events.ScheduleEvent(EVENT_RALLY, urand(20000, 30000));
                            break;
                        case EVENT_SHOCKWAVE:
                            DoCast(SPELL_SHOCKWAVE);
                            events.ScheduleEvent(EVENT_SHOCKWAVE, urand(16000, 30000));
                            break;
                        case EVENT_DEVASTATE:
                            DoCast(me->getVictim(), SPELL_DEVASTATE);
                            events.ScheduleEvent(EVENT_DEVASTATE, urand(15000, 18000));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
     };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_executor_of_the_caliphAI>(creature);
        }
};

class npc_gust_soldier : public CreatureScript
{
    public:
        npc_gust_soldier() : CreatureScript("npc_gust_soldier") { }

        struct npc_gust_soldierAI : public ScriptedAI
        {
            npc_gust_soldierAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* who) override
            {
                me->CallForHelp(16.0f);
                DoCast(who, SPELL_CHARGE);
                events.ScheduleEvent(EVENT_AIR_NOVA, urand(5000, 15000));
                events.ScheduleEvent(EVENT_WIND_BLAST, urand(6000, 8000));
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
                    case EVENT_AIR_NOVA:
                        DoCast(SPELL_AIR_NOVA);
                        events.ScheduleEvent(EVENT_AIR_NOVA, urand(10000, 15000));
                        break;
                    case EVENT_WIND_BLAST:
                        DoCast(me->getVictim(), SPELL_WIND_BLAST);
                        events.ScheduleEvent(EVENT_WIND_BLAST, urand(6000, 8000));
                        break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_gust_soldierAI>(creature);
        }
};

class npc_lurking_tempest : public CreatureScript
{
    public:
        npc_lurking_tempest() : CreatureScript("npc_lurking_tempest") { }

        enum iSpells
        {
            SPELL_LIGHTNING_BOLT = 89105,
            SPELL_LURK           = 85467,
            SPELL_FEIGN_DEATH    = 85267,
        };

        enum iEvents
        {
            EVENT_LIGHTNING_BOLT = 1,
            EVENT_LURK           = 2,
        };

        struct npc_lurking_tempestAI : public ScriptedAI
        {
            npc_lurking_tempestAI(Creature* creature) : ScriptedAI(creature) 
            {
                SetCombatMovement(false);
            }

            EventMap events;

            void Reset() override
            {
                // Wrong in Creature template 
                if (me->GetInstanceScript() && me->GetInstanceScript()->instance->GetSpawnMode() == DUNGEON_DIFFICULTY_NORMAL)
                {
                    // should be 223.395 in 5 normal
                    me->SetCreateHealth(223395);
                    me->SetMaxHealth(223395);
                    me->SetHealth(223395);
                    me->ResetPlayerDamageReq();
                }

                me->RemoveAurasDueToSpell(SPELL_LURK);
                me->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                events.Reset();
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) override
            {
                if (me->HasAura(SPELL_LURK))
                    damage = 0;
            }

            void EnterCombat(Unit* /*who*/) override
            {
                events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 1 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_LURK, 3 * IN_MILLISECONDS);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LIGHTNING_BOLT:
                        {
                            DoCast(me->getVictim(), SPELL_LIGHTNING_BOLT);
                            events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 1.6 * IN_MILLISECONDS);
                            break;
                        }
                        case EVENT_LURK:
                        {
                            if (HasAnyPlayerLookAtMe())
                            {
                                me->InterruptNonMeleeSpells(false);
                                events.CancelEvent(EVENT_LIGHTNING_BOLT);
                                DoCast(me, SPELL_LURK);
                                DoCast(me, SPELL_FEIGN_DEATH);
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                            }
                            else
                            {
                                me->RemoveAurasDueToSpell(SPELL_LURK);
                                me->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                                events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 1 * IN_MILLISECONDS);
                            }
                            events.ScheduleEvent(EVENT_LURK, 3 * IN_MILLISECONDS);
                            break;
                        }
                    }
                }
            }

            private:
                bool HasAnyPlayerLookAtMe()
                {
                    std::list<Player*> list;
                    GetPlayerListInGrid(list, me, 40.0f);

                    if (list.empty())
                    {
                        me->AI()->EnterEvadeMode();
                        return false;
                    }

                    list.remove_if([=](Player* target) { return target->isInFront(me, 2.5f); });
                    return list.empty();
                }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_lurking_tempestAI>(creature);
        }
};

// Howling Gale 45572
struct npc_howling_gale : public ScriptedAI
{
    npc_howling_gale(Creature* creature) : ScriptedAI(creature)
    {
        me->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);
    }

    EventMap events;
    bool bCombat, HasDeactivate;

    void Reset() override
    {
        bCombat = false;
        HasDeactivate = false;
        events.Reset();
        DoCast(me, SPELL_HOWLING_GALE);
        events.ScheduleEvent(EVENT_HOWLING_GALE, 1000);
    }

    void JustReachedHome() override
    {
        bCombat = false;
        HasDeactivate = false;
    }

    void EnterEvadeMode() override
    {
        bCombat = false;
        HasDeactivate = false;
        _EnterEvadeMode();
        Reset();
    }

    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
    {
        if (!HasDeactivate)
        {
            HasDeactivate = true;
            events.ScheduleEvent(EVENT_HOWLING_GALE_DEACTIVATE, urand(1500, 2000));
        }
    }

    uint32 GetData(uint32 type) const override
    {
        if (type == TYPE_HOWLING_GALE)
            return  HasDeactivate ? 1 : 0;
        return 0;
    }

    void UpdateAI(uint32 diff) override
    {
        if (bCombat)
        {
            if (!me->GetMap()->GetPlayers().isEmpty())
            {
                uint8 _attackersnum = 0;
                for (auto&& itr : me->GetInstanceScript()->instance->GetPlayers())
                    if (me->GetDistance2d(itr.getSource()) < 30.0f)
                        _attackersnum++;

                if (_attackersnum == 0)
                {
                    EnterEvadeMode();
                    return;
                }
            }
        }

        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_HOWLING_GALE:
                    DoCast(me, SPELL_HOWLING_GALE_1, true);
                    DoCast(me, SPELL_HOWLING_GALE_2, true);
                    DoCast(me, SPELL_HOWLING_GALE_3, true);
                    events.ScheduleEvent(EVENT_HOWLING_GALE, 2500);
                    break;
                case EVENT_HOWLING_GALE_DEACTIVATE:
                    bCombat = true;
                    events.CancelEvent(EVENT_HOWLING_GALE);
                    break;
            }
        }
    }
};

class npc_minister_of_air : public CreatureScript
{
    public:
        npc_minister_of_air() : CreatureScript("npc_minister_of_air") { }

        struct npc_minister_of_airAI : public ScriptedAI
        {
            npc_minister_of_airAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                events.ScheduleEvent(EVENT_LIGHTNING_LASH, urand(4000, 8000));
                events.ScheduleEvent(EVENT_LIGHTNING_NOVA, urand(7000, 10000));

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
                        case EVENT_LIGHTNING_NOVA:
                            DoCast(SPELL_LIGHTNING_NOVA);
                            events.ScheduleEvent(EVENT_LIGHTNING_NOVA, urand(10000, 20000));
                            break;
                        case EVENT_LIGHTNING_LASH:
                            DoCast(SPELL_LIGHTNING_LASH);
                            events.ScheduleEvent(EVENT_LIGHTNING_LASH, urand(8000, 15000));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_minister_of_airAI>(creature);
        }
};

// Servant of Asaad 45926
struct npc_servant_of_asaad : public ScriptedAI
{
    npc_servant_of_asaad(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;
    bool hasTriggered;

    void Reset() override
    {
        hasTriggered = false;
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) override
    {
        me->CallForHelp(23.5f);
        events.ScheduleEvent(EVENT_CRUSADER_STRIKE, urand(3000, 6000));
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage) override
    {
        if (HealthBelowPct(30) && !hasTriggered)
        {
            hasTriggered = true;
            events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, 0.5 * IN_MILLISECONDS);
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
                case EVENT_CRUSADER_STRIKE:
                    DoCast(me->getVictim(), SPELL_CRUSADER_STRIKE);
                    events.ScheduleEvent(EVENT_CRUSADER_STRIKE, urand(8000, 12000));
                    break;
                case EVENT_HAND_OF_PROTECTION:
                    DoCast(me, SPELL_HAND_OF_PROTECTION);
                    events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, urand(20000, 30000));
                    break;
            }
        }
        DoMeleeAttackIfReady();
    }
};

class npc_temple_adept : public CreatureScript
{
    public:
        npc_temple_adept() : CreatureScript("npc_temple_adept") { }

        struct npc_temple_adeptAI : public ScriptedAI
        {
            npc_temple_adeptAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            Creature* _target;

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                me->CallForHelp(23.5f);
                events.ScheduleEvent(EVENT_HOLY_SMITE, urand(5000, 6000));
                events.ScheduleEvent(EVENT_GREATER_HEAL, urand(5000, 6000));
                events.ScheduleEvent(EVENT_DESPERATE_SPEED, urand(10000, 15000));
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
                        case EVENT_HOLY_SMITE:
                            DoCast(me->getVictim(), SPELL_HOLY_SMITE);
                            events.ScheduleEvent(EVENT_HOLY_SMITE, urand(5000, 6000));
                            break;
                        case EVENT_DESPERATE_SPEED:
                            DoCast(me, SPELL_DESPERATE_SPEED);
                            events.ScheduleEvent(EVENT_DESPERATE_SPEED, urand(20000, 30000));
                            break;
                        case EVENT_GREATER_HEAL:
                            if (_target = me->FindNearestCreature(NPC_EXECUTOR_OF_THE_CALIPH, 30.0f))
                                if (_target->GetHealthPct() < 50)
                                    DoCast(_target, SPELL_GREATER_HEAL);
                            else if (_target = me->FindNearestCreature(NPC_MINISTER_OF_AIR, 30.0f))
                                if (_target->GetHealthPct() < 50)
                                    DoCast(_target, SPELL_GREATER_HEAL);
                            else if (_target = me->FindNearestCreature(NPC_SERVANT_OF_ASAAD, 30.0f))
                                if (_target->GetHealthPct() < 50)
                                    DoCast(_target, SPELL_GREATER_HEAL);
                            else if (_target = me->FindNearestCreature(NPC_TEMPEST_ADEPT, 30.0f))
                                if (_target->GetHealthPct() < 50)
                                    DoCast(_target, SPELL_GREATER_HEAL);
                            events.ScheduleEvent(EVENT_GREATER_HEAL, urand(5000, 6000));
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_temple_adeptAI>(creature);
        }
};

class npc_turbulent_squall : public CreatureScript
{
    public:
        npc_turbulent_squall() : CreatureScript("npc_turbulent_squall") { }

        struct npc_turbulent_squallAI : public ScriptedAI
        {
            npc_turbulent_squallAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                me->CallForHelp(16.0f);
                events.ScheduleEvent(EVENT_ASPHYXIATE, urand(3000, 10000));
                events.ScheduleEvent(EVENT_HURRICANE, urand(10000, 20000));
                events.ScheduleEvent(EVENT_CLOUDBURST, urand(5000, 30000));
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
                        case EVENT_ASPHYXIATE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_ASPHYXIATE);
                            events.ScheduleEvent(EVENT_ASPHYXIATE, urand(10000, 20000));
                            break;
                        case EVENT_HURRICANE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_HURRICANE);
                            events.ScheduleEvent(EVENT_HURRICANE, urand(15000, 30000));
                            break;
                        case EVENT_CLOUDBURST:
                            DoCast(SPELL_CLOUDBURST);
                            events.ScheduleEvent(EVENT_CLOUDBURST, urand(20000, 30000));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_turbulent_squallAI>(creature);
        }
};

class npc_wild_vortex : public CreatureScript
{
    public:
        npc_wild_vortex() : CreatureScript("npc_wild_vortex") { }

        struct npc_wild_vortexAI : public ScriptedAI
        {
            npc_wild_vortexAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) override
            {
                me->CallForHelp(16.0f);
                events.ScheduleEvent(EVENT_WIND_SHOCK, urand(5000, 10000));
                events.ScheduleEvent(EVENT_WV_LIGHTNING_BOLT, 3000);
                events.ScheduleEvent(EVENT_CYCLONE, urand(5000, 15000));
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
                        case EVENT_CYCLONE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_CYCLONE);
                            events.ScheduleEvent(EVENT_CYCLONE, urand(10000, 20000));
                            break;
                        case EVENT_WIND_SHOCK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_WIND_SHOCK);
                            events.ScheduleEvent(EVENT_WIND_SHOCK, urand(10000, 20000));
                            break;
                        case EVENT_WV_LIGHTNING_BOLT:
                            DoCast(me->getVictim(), SPELL_WV_LIGHTNING_BOLT);
                            events.ScheduleEvent(EVENT_WV_LIGHTNING_BOLT, 3000);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_wild_vortexAI>(creature);
        }
};

class npc_golden_orb : public CreatureScript
{
    public:
        npc_golden_orb() : CreatureScript("npc_golden_orb") { }

        struct npc_golden_orbAI : public ScriptedAI
        {
            npc_golden_orbAI(Creature* creature) : ScriptedAI(creature) { }

            void OnSpellClick(Unit* /*clicker*/, bool& result) override
            {
                if (!me->IsVisible())
                    return;

                me->GetInstanceScript()->SetData(DATA_GOLDEN_ORB, 1);
                if (me->GetInstanceScript()->GetData(DATA_GOLDEN_ORB) >= 5)
                    DoCastAOE(SPELL_ACHIEVEMENT, true);

                me->SetVisible(false);
                result = true;
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_golden_orbAI>(creature);
        }
};

// Starfall Rotation Manager 80044
struct npc_starfall_rotation_manager : public ScriptedAI
{
    npc_starfall_rotation_manager(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        for (uint8 i = 0; i < 8; i++)
            me->SummonCreature(NPC_STAR_SKY, me->GetPositionX() + frand(-10.0f, 10.0f), me->GetPositionY() + frand(-10.0f, 10.0f), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
    }
};

class npc_fall_catcher_5 : public CreatureScript
{
    public:
        npc_fall_catcher_5() : CreatureScript("npc_fall_catcher_5") { }

        struct npc_fall_catcher_5AI : public CreatureAI
        {
            npc_fall_catcher_5AI(Creature* creature) : CreatureAI(creature) { }

            void IsSummonedBy(Unit* summoner) override
            {
                summoner->SetDisableGravity(true, true);

                float x, y, z, o;
                summoner->GetPosition(x, y, z, o);
                summoner->NearTeleportTo(x, y, z + 1, o);
                summoner->CastSpell(me, SPELL_CATCH_FALL_VEHICLE, true);
                me->GetPosition(x, y, z);
                me->GetMotionMaster()->MovePoint(1, x, y, z + 50);

                summoner->SetDisableGravity(false, true);
            }

            void MovementInform(uint32 type, uint32 pointId) override
            {
                if (type == POINT_MOTION_TYPE && pointId == 1)
                {
                    Unit* summoner = me->ToTempSummon()->GetSummoner();
                    summoner->ExitVehicle(me);
                    summoner->CastSpell(summoner, SPELL_SLIPSTREAM_BUFF, true);

                    // Have to do it manually since cell visiting is limited by grid size for some inexplicable reason
                    float x = -356.958008f;
                    float y = 26.140600f;
                    float z = 627.062012f;
                    float dist = summoner->GetExactDist(x, y, z);

                    Map::PlayerList const& players = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        if (Player* player = itr->getSource())
                        {
                            if (player != summoner && !player->HasAura(SPELL_CATCH_FALL_SUMMON_AURA) && !player->HasAura(SPELL_SLIPSTREAM_BUFF))
                            {
                                float d = summoner->GetExactDist(player);
                                if (d < dist)
                                {
                                    dist = d;
                                    player->GetPosition(x, y, z);
                                }
                            }
                        }
                    }

                    summoner->RemoveAurasDueToSpell(SPELL_CATCH_FALL_SUMMON_AURA);
                    summoner->CastSpell(x, y, z, SPELL_CATCH_FALL_JUMP, true);
                }
            }

            void UpdateAI(uint32 /*diff*/) override { }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_fall_catcher_5AI>(creature);
        }
};

class spell_minister_of_air_lightning_lash : public SpellScriptLoader
{
    public:
        spell_minister_of_air_lightning_lash() : SpellScriptLoader("spell_minister_of_air_lightning_lash") { }


        class spell_minister_of_air_lightning_lash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_minister_of_air_lightning_lash_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_LIGHTNING_LASH_DMG, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_minister_of_air_lightning_lash_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_minister_of_air_lightning_lash_SpellScript();
        }
};

class spell_howling_gale_howling_gale : public SpellScriptLoader
{
    public:
        spell_howling_gale_howling_gale() : SpellScriptLoader("spell_howling_gale_howling_gale") { }
 
        class spell_howling_gale_howling_gale_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_howling_gale_howling_gale_AuraScript)

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (!GetCaster())
                    return;

                //GetCaster()->CastSpell(GetCaster(), SPELL_HOWLING_GALE_1, true);
                //GetCaster()->CastSpell(GetCaster(), SPELL_HOWLING_GALE_2, true);
                //GetCaster()->CastSpell(GetCaster(), SPELL_HOWLING_GALE_3, true);
            }
 
            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_howling_gale_howling_gale_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };
 
        AuraScript* GetAuraScript() const override
        {
            return new spell_howling_gale_howling_gale_AuraScript();
        }
};

// Lurk 85467
class spell_vp_lurk : public AuraScript
{
    PrepareAuraScript(spell_vp_lurk);

    void OnAuraEffectRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetOwner()->ToUnit())
            owner->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
    }

    void OnAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Unit* owner = GetOwner()->ToUnit())
            owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_vp_lurk::OnAuraEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectApply += AuraEffectApplyFn(spell_vp_lurk::OnAuraEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Slipstream 85021
class spell_slipstream_aura : public AuraScript
{
    PrepareAuraScript(spell_slipstream_aura);

    void OnAuraEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (Player* owner = GetOwner()->ToPlayer())
            SetDuration(15 * IN_MILLISECONDS);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_slipstream_aura::OnAuraEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Healing Well 88201
class spell_vortex_pinnacle_healing_well : public SpellScript
{
    PrepareSpellScript(spell_vortex_pinnacle_healing_well);

    void HandleEffectHitTarget(SpellEffIndex eff_idx)
    {
        if (GetCaster() && GetCaster()->GetEntry() == NPC_YOUNG_STORM_DRAGON)
        {
            PreventHitEffect(eff_idx);

            if (Creature* uTrigger = GetCaster()->FindNearestCreature(NPC_UTRIGGER, 10.0f, true))
                uTrigger->CastSpell(uTrigger, GetSpellInfo()->Id, true);
        }
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_vortex_pinnacle_healing_well::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_PERSISTENT_AREA_AURA);
    }
};

// Howling Gale Eff 85159
class spell_vortex_pinnacle_howling_gale_eff : public SpellScript
{
    PrepareSpellScript(spell_vortex_pinnacle_howling_gale_eff);

    void HandleEffectHitTarget(SpellEffIndex eff_idx)
    {
        if (Creature* caster = GetCaster()->ToCreature())
            if (caster->ToCreature()->AI()->GetData(TYPE_HOWLING_GALE))
                PreventHitEffect(eff_idx);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_vortex_pinnacle_howling_gale_eff::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
        OnEffectHitTarget += SpellEffectFn(spell_vortex_pinnacle_howling_gale_eff::HandleEffectHitTarget, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
    }
};

class at_catch_fall_5 : public AreaTriggerScript
{
    public:
        at_catch_fall_5() : AreaTriggerScript("at_catch_fall_5") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*trigger*/) override
        {
            if (player->HasAura(SPELL_CATCH_FALL_SUMMON_AURA) || player->HasAura(SPELL_SLIPSTREAM_AURA))
                return true;

            player->CastSpell(player, SPELL_CATCH_FALL_SUMMON_TRIGGER, true);

            // Prevent Combat if victim has force down
            for (auto&& itr : bossData)
                if (Creature* encounter = ObjectAccessor::GetCreature(*player, player->GetInstanceScript() ? player->GetInstanceScript()->GetData64(itr) : 0))
                    if (encounter->isInCombat() && encounter->getVictim() && encounter->getVictim()->GetGUID() == player->GetGUID())
                        encounter->AI()->EnterEvadeMode();
                        
            return true;
        }
};

void AddSC_vortex_pinnacle()
{
    new npc_vortex_pinnacle_slipsteam();
    new creature_script<npc_skyfall_star>("npc_skyfall_star");
    new npc_cloud_prince();
    new npc_whipping_wind();
    new npc_young_storm_dragon();
    new npc_armored_mistral();
    new npc_empyrean_assassin();
    new npc_executor_of_the_caliph();
    new npc_gust_soldier();
    new creature_script<npc_howling_gale>("npc_howling_gale");
    new npc_lurking_tempest();
    new npc_minister_of_air();
    new creature_script<npc_servant_of_asaad>("npc_servant_of_asaad");
    new npc_temple_adept();
    new npc_turbulent_squall();
    new npc_wild_vortex();
    new npc_golden_orb();
    new npc_fall_catcher_5();
    new creature_script<npc_starfall_rotation_manager>("npc_starfall_rotation_manager");
    new spell_minister_of_air_lightning_lash();
    //new spell_howling_gale_howling_gale();
    new aura_script<spell_vp_lurk>("spell_vp_lurk");
    new aura_script<spell_slipstream_aura>("spell_slipstream_aura");
    new spell_script<spell_vortex_pinnacle_healing_well>("spell_vortex_pinnacle_healing_well");
    new spell_script<spell_vortex_pinnacle_howling_gale_eff>("spell_vortex_pinnacle_howling_gale_eff");
    new at_catch_fall_5();
};