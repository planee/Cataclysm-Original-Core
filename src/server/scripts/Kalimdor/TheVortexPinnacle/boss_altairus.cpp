#include"ScriptPCH.h"
#include"the_vortex_pinnacle.h"

enum Spells
{
    SPELL_CALL_OF_WIND            = 88244,
    SPELL_CALL_OF_WIND_DUMMY_1    = 88276,
    SPELL_CALL_OF_WIND_DUMMY_2    = 88772,
    SPELL_DOWNWIND_OF_ALTAIRUS    = 88286,
    SPELL_UPWIND_OF_ALTAIRUS      = 88282,
    SPELL_CHILLING_BREATH         = 88308,
    SPELL_CHILLING_BREATH_DUMMY   = 88322,
    SPELL_LIGHTNING_BLAST         = 88357,
    SPELL_LIGHTNING_BLAST_DUMMY   = 88332,
    SPELL_TWISTER_AURA            = 88313,
    SPELL_TWISTER_AURA_DMG        = 88314,
    
};

enum Events
{
    EVENT_CALL_OF_WIND = 1,
    EVENT_CHILLING_BREATH,
    EVENT_LIGHTNING_BLAST,
    EVENT_CHECK_FACING,
    EVENT_RESET_WIND,
    EVENT_TWISTING_WINDS,
};

enum Actions
{
    ACTION_INIT_TWIST,
    ACTION_RESET_TWIST,
};

class boss_altairus : public CreatureScript
{
    public:
        boss_altairus() : CreatureScript("boss_altairus") { }

        struct boss_altairusAI : public BossAI
        {
            boss_altairusAI(Creature* creature) : BossAI(creature, DATA_ALTAIRUS)
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
                me->setActive(true);
            }

            Creature* _aircurrent;
            uint8 _twisternum;

            void Reset() override
            {
                _Reset();
                _twisternum = 0;

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                // Should have Fly anim in combat too
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->SetHover(true);
            }
    
            void EnterCombat(Unit* /*who*/) override
            {
                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_CHILLING_BREATH, urand(5000, 10000));
                events.ScheduleEvent(EVENT_CALL_OF_WIND, 2000);
                events.ScheduleEvent(EVENT_CHECK_FACING, 2500);

                if (IsHeroic())
                {
                    HandleTwisting(3);
                    events.ScheduleEvent(EVENT_TWISTING_WINDS, 6.5 * IN_MILLISECONDS);
                }

                DoZoneInCombat();
                instance->SetBossState(DATA_ALTAIRUS, IN_PROGRESS);
            }

            void EnterEvadeMode() override
            {
                ScriptedAI::EnterEvadeMode();

                std::list<Creature*> twisterList;
                GetCreatureListWithEntryInGrid(twisterList, me, NPC_TWISTER, 100.0f);

                for (auto&& itr : twisterList)
                    itr->AI()->DoAction(ACTION_RESET_TWIST);
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                std::list<Creature*> twisterList;
                GetCreatureListWithEntryInGrid(twisterList, me, NPC_TWISTER, 100.0f);

                for (auto&& itr : twisterList)
                    itr->AI()->DoAction(ACTION_RESET_TWIST);
            }
            
            bool CheckOrientation(float player, float creature)
            {
                float _cur, _up, _down;
                
                if (creature > M_PI)
                    _cur = creature - M_PI;
                else
                    _cur = creature + M_PI;
                

                _up = _cur + 1.0f;
                _down = _cur - 1.0f;

                if (player > _down && player < _up)
                    return true;
                else
                    return false;
            }

            void UpdateAI(const uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->getVictim() && me->getVictim()->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 55.0f)
                {
                    DoCast(me->getVictim(), SPELL_LIGHTNING_BLAST);
                    return;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHILLING_BREATH:
                            if (Unit* target  = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_CHILLING_BREATH);
                            events.ScheduleEvent(EVENT_CHILLING_BREATH, urand(10000, 16000));
                            break;
                        case EVENT_RESET_WIND:
                            if (_aircurrent)
                                _aircurrent->DespawnOrUnsummon();
                            events.DelayEvents(1000);
                            events.ScheduleEvent(EVENT_CALL_OF_WIND, 800);
                            break;
                        case EVENT_CALL_OF_WIND:
                            _aircurrent = me->SummonCreature(NPC_AIR_CURRENT,
                                me->GetPositionX(),
                                me->GetPositionY(),
                                me->GetPositionZ(),
                                orientations[urand(0, 3)]);
                            events.ScheduleEvent(EVENT_RESET_WIND, 18000);
                            break;
                        case EVENT_CHECK_FACING:
                            if (me->GetMap()->GetPlayers().isEmpty() || !_aircurrent)
                                break;

                            for (Map::PlayerList::const_iterator itr = me->GetMap()->GetPlayers().begin(); itr != me->GetMap()->GetPlayers().end(); ++itr) 
                            {
                                if (CheckOrientation(itr->getSource()->GetOrientation(), _aircurrent->GetOrientation()))
                                {
                                    itr->getSource()->RemoveAurasDueToSpell(SPELL_DOWNWIND_OF_ALTAIRUS);
                                    me->AddAura(SPELL_UPWIND_OF_ALTAIRUS, itr->getSource());
                                }
                                else
                                {
                                    itr->getSource()->RemoveAurasDueToSpell(SPELL_UPWIND_OF_ALTAIRUS);
                                    me->AddAura(SPELL_DOWNWIND_OF_ALTAIRUS, itr->getSource());
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_FACING, 3000);
                            break;
                        case EVENT_TWISTING_WINDS:
                            HandleTwisting(1);
                            events.ScheduleEvent(EVENT_TWISTING_WINDS, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            private:
                void HandleTwisting(uint8 count)
                {
                    std::list<Creature*> twisterList;
                    GetCreatureListWithEntryInGrid(twisterList, me, NPC_TWISTER, 100.0f);
                    twisterList.remove_if([=](WorldObject* target) { return target && target->ToCreature() && target->ToCreature()->HasAura(SPELL_TWISTER_AURA); });

                    if (twisterList.size() > count)
                        Trinity::Containers::RandomResizeList(twisterList, count);

                    for (auto&& itr : twisterList)
                        itr->AI()->DoAction(ACTION_INIT_TWIST);
                }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_altairusAI>(creature);
        }
};

// Air Current 47305
struct npc_air_current : public ScriptedAI
{
    npc_air_current(Creature* creature) : ScriptedAI(creature)
    {
        me->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);
    }

    void Reset() override
    {
        //DoCast(me, SPELL_CALL_OF_WIND_DUMMY_1);
        DoCast(me, SPELL_CALL_OF_WIND_DUMMY_2);
        //DoCast(me, SPELL_CALL_OF_WIND);
    }

    void UpdateAI(uint32 const /*diff*/) override { }
};

// Twister 47342
struct npc_altairus_twister : public ScriptedAI
{
    npc_altairus_twister(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    float x, y;

    void InitializeAI() override
    {
        me->SetCanFly(true);
        me->SetDisableGravity(true);
        me->SetHover(true);
    }

    void DoAction(int32 const actionId) override
    {
        switch (actionId)
        {
            case ACTION_INIT_TWIST:
                DoCast(me, SPELL_TWISTER_AURA);
                x = me->GetPositionX(); y = me->GetPositionY();

                scheduler
                    .Schedule(Milliseconds(1000), [this](TaskContext context)
                {
                    float postX = x + frand(-10.0f, 10.0f);
                    float postY = y + frand(-10.0f, 10.0f);
                    me->GetMotionMaster()->MovePoint(0, postX, postY, me->GetMap()->GetHeight(postX, postY, me->GetPositionZ(), true, 5.0f), me->GetOrientation());
                    context.Repeat(Milliseconds(me->GetSplineDuration()));
                });
                break;
            case ACTION_RESET_TWIST:
                scheduler.CancelAll();
                me->RemoveAurasDueToSpell(SPELL_TWISTER_AURA);
                me->CombatStop(true);
                break;
        }
    }

    void UpdateAI(uint32 const diff) override
    {
        scheduler.Update(diff);
    }
};

void AddSC_boss_altairus()
{
    new boss_altairus();
    new creature_script<npc_air_current>("npc_air_current");
    new creature_script<npc_altairus_twister>("npc_altairus_twister");
}