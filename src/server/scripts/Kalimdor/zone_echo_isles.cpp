//////////////////////////////////////////////////////////////////////////
///
///  PROJECT AZENOR
/// COPYRIGHT 2018-2019
/// ALL RIGHTS RESERVED.
///
//////////////////////////////////////////////////////////////////////////

/// Game
#include "GameObjectAI.h"
#include "MoveSplineInit.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Vehicle.h"
#include "WorldSession.h"

enum eShared
{
    ActionStartEvents    = 1,
    ActionSetGUID        = 1337,
    ActionOnDeathOfScout = 1500,

    NpcEchoIslesQuestBunny = 38003,
};

/// Capative Spritescale Scout - 38142
struct npc_captive_spitescale_scout : public ScriptedAI
{
    enum eTalks
    {
        TalkMovementInform = 0,
    };

    enum eMovementInforms
    {
        MovementInformMoveToCenter = 1,
    };

    enum eSpells
    {
        SpellFrostShock = 15089,
    };

    public:

        npc_captive_spitescale_scout(Creature* creature) : ScriptedAI(creature),
            m_EventLocked(false), m_ScoutLocked(false), m_ScoutIndex(0), m_GuidLocked(false), m_JailorGuid(0)
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void DoAction(const int32 p_Action) override
        {
            if (p_Action == ActionStartEvents)
            {
                if (m_EventLocked)
                    return;

                m_EventLocked = true;

                static const Position l_CenterPositions[] = 
                {
                    {-1146.754f, -5530.905f, 08.106f, 2.982f },
                    {-1148.483f, -5417.083f, 10.598f, 5.067f },
                };

                me->GetMotionMaster()->MovePoint(MovementInformMoveToCenter, m_ScoutIndex == 7 ? l_CenterPositions[1] : l_CenterPositions[0]);
            }
        }

        void SetData(uint32 p_Id, uint32 p_Data) override
        {
            if (p_Id == ActionSetGUID)
            {
                if (m_ScoutLocked)
                    return;

                m_ScoutLocked = true;

                m_ScoutIndex = p_Data;
            }
        }

        void SetGUID(uint64 p_Guid, int32 p_Id) override
        {
            if (p_Id == ActionSetGUID)
            {
                if (m_GuidLocked)
                    return;

                m_GuidLocked = true;

                m_JailorGuid = p_Guid;
            }
        }

        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type != POINT_MOTION_TYPE)
                return;

            /// Update when moving to middle
            if (p_Id == MovementInformMoveToCenter)
            {
                Talk(TalkMovementInform);
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->SetHomePosition(*me);
            }
        }

    	void Reset() override
    	{
            /// Reset events on reset
            m_Events.Reset();

            if (m_EventLocked)
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    	}

    	void JustDied(Unit* killer) override
    	{
            /// Call jailor's AI to do action when we die.
            if (Creature* l_Jailor = ObjectAccessor::GetCreature(*me, m_JailorGuid))
                l_Jailor->GetAI()->DoAction(ActionOnDeathOfScout);
    	}

        void EnterCombat(Unit* /*p_Provoker*/) override
        {
            /// Schedule frost shock on enter combat
            m_Events.ScheduleEvent(SpellFrostShock, 2000, 4000);
        }
    
    	void UpdateAI(const uint32 p_Diff) override
    	{
            /// Update scheduler
            m_Scheduler.Update(p_Diff);

            /// Check if we have victim
            if (!UpdateVictim())
                return;

            /// Update Events
            m_Events.Update(p_Diff);

            /// Check for event to execute
            if (uint32 l_EventId = m_Events.ExecuteEvent())
            {
                /// Cast frost shock at victim, and do it again between 5 and 6 sec
                if (l_EventId == SpellFrostShock)
                {
                    DoCastVictim(SpellFrostShock);
                    m_Events.ScheduleEvent(SpellFrostShock, 5000, 6000);
                }
            }

            DoMeleeAttackIfReady();
    	}

    private:
        /// Schedulers
        TaskScheduler m_Scheduler;
        EventMap m_Events;

        /// Variables
        bool m_EventLocked;
        bool m_ScoutLocked;
        bool m_GuidLocked;
        uint32 m_ScoutIndex;
        ObjectGuid m_JailorGuid;
};

/// Darkspear Jailor - 39062
class npc_darkspear_jailor : public CreatureScript
{
    public:
    	npc_darkspear_jailor() : CreatureScript("npc_darkspear_jailor"){ }
    
    	enum eTrollQuests
    	{
    		/// Proving Pit quests
    		QUEST_PROVING_PIT_ROGU = 24774,
    		QUEST_PROVING_PIT_MAGE = 24754,
    		QUEST_PROVING_PIT_SHAM = 24762,
    		QUEST_PROVING_PIT_HUNT = 24780,
    		QUEST_PROVING_PIT_PRIE = 24786,
    		QUEST_PROVING_PIT_WARR = 24642,
    		QUEST_PROVING_PIT_DRUI = 24768,
    		QUEST_PROVING_PIT_WARL = 26276,
    	};

    	enum eEvents
    	{
    		/// Darkspear Jailor
    		EVENT_MOVE_TO_CAGE_1 = 1,
    		EVENT_MOVE_TO_CAGE_2,
    		EVENT_OPEN_CAGE,
    		EVENT_MOVE_BACK_1,
    		EVENT_MOVE_BACK_2,
    		EVENT_SUMMON_SPITESCALE_SCOUT,
    		EVENT_RESET_POS,
    	};
    
        enum eNpcText
        {
            NpcTextDarkspearJailor = 15251,
        };
    
        enum eCredits
        {
            CreditTalkToJailor = 39062,
        };

        enum eTalks
        {
            TalkGetIntoThePit = 1,
            TalkWellDone,
        };

        enum eGameObjects
        {
            GoDarkspearCage = 201968,
        };

        enum eCreatures
        {
            NpcSpitescaleScout      = 38142,
            NpcDarkspearJailor      = 39062,
            NpcLegatiRogueTrainer   = 38244,
        };

    	struct npc_darkspear_jailorAI : public ScriptedAI
    	{
            public:
    	    	npc_darkspear_jailorAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_JailorIndex(0), m_Activated(false), m_SpitescaleScout(0), m_InvokerGUID(0)
                {
                    /// Creature seems to never attack
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                void DoAction(const int32 p_Action) override
                {
                    static Position const TrollwayPos[] =
                    {
                        { -1159.222f, -5519.436f, 12.128f, 0.000f },        ///< 0
                        { -1152.798f, -5519.026f, 11.984f, 0.000f },        ///< 1
                    };

                    static Position const TrollwayPosByRogue[] =
                    {
                        /// Jailor near rogue trainer
                        { -1137.437f, -5430.574f, 13.640f, 0.000f },        ///< 0
                        { -1136.318f, -5417.105f, 13.270f, 0.000f },        ///< 1
                    };

                    if (p_Action == ActionStartEvents)
                    {
                        /// If we are already activated don't do anything more
                        if (m_Activated)
                            return;

                        /// Set activated to true
                        m_Activated = true;

                        /// Remove gossip flag
                        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                        if (me->FindNearestCreature(NpcLegatiRogueTrainer, 30.0f))
                            m_JailorIndex = 7;

                        /// Move 2 paths in one :)
                        Movement::MoveSplineInit l_Init(me);

                        l_Init.Path().push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));

                        if (m_JailorIndex == 7)
                        {
                            l_Init.Path().push_back(G3D::Vector3(TrollwayPosByRogue[0].m_positionX, TrollwayPosByRogue[0].m_positionY, TrollwayPosByRogue[0].m_positionZ));
                            l_Init.Path().push_back(G3D::Vector3(TrollwayPosByRogue[1].m_positionX, TrollwayPosByRogue[1].m_positionY, TrollwayPosByRogue[1].m_positionZ));
                        }
                        else
                        {
                            l_Init.Path().push_back(G3D::Vector3(TrollwayPos[0].m_positionX, TrollwayPos[0].m_positionY, TrollwayPos[0].m_positionZ));
                            l_Init.Path().push_back(G3D::Vector3(TrollwayPos[1].m_positionX, TrollwayPos[1].m_positionY, TrollwayPos[1].m_positionZ));
                        }

                        /// Set catmullrom movement
                        l_Init.SetSmooth();

                        /// Launch new movespline
                        l_Init.Launch();

                        /// Schedule events
                        m_Scheduler.Schedule(Seconds(3), [this](TaskContext /*p_Context*/)
                        {
                            if (GameObject* l_Cage = me->FindNearestGameObject(GoDarkspearCage, 10.0f))
                            {
                                l_Cage->SetLootState(GO_READY);
                                l_Cage->UseDoorOrButton();
                            }

                            /// ONESHOT_USESTANDING
                            me->HandleEmoteCommand(432);
                        });

                        m_Scheduler.Schedule(Seconds(4), [this](TaskContext /*p_Context*/)
                        {
                            if (Creature* l_Scout = ObjectAccessor::GetCreature(*me, m_SpitescaleScout))
                            {
                                l_Scout->AI()->SetData(ActionSetGUID, m_JailorIndex);
                                l_Scout->AI()->SetGUID(me->GetGUID(), ActionSetGUID);
                                l_Scout->AI()->DoAction(ActionStartEvents);
                            }
                        });

                        m_Scheduler.Schedule(Seconds(5), [this](TaskContext /*p_Context*/)
                        {
                            Movement::MoveSplineInit l_Init(me);

                            l_Init.Path().push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));

                            if (m_JailorIndex == 7)
                            {
                                l_Init.Path().push_back(G3D::Vector3(TrollwayPosByRogue[1].m_positionX, TrollwayPosByRogue[1].m_positionY, TrollwayPosByRogue[1].m_positionZ));
                                l_Init.Path().push_back(G3D::Vector3(TrollwayPosByRogue[0].m_positionX, TrollwayPosByRogue[0].m_positionY, TrollwayPosByRogue[0].m_positionZ));
                            }
                            else
                            {
                                l_Init.Path().push_back(G3D::Vector3(TrollwayPos[1].m_positionX, TrollwayPos[1].m_positionY, TrollwayPos[1].m_positionZ));
                                l_Init.Path().push_back(G3D::Vector3(TrollwayPos[0].m_positionX, TrollwayPos[0].m_positionY, TrollwayPos[0].m_positionZ));
                            }
                            
                            auto const& l_HomePos = me->GetHomePosition();

                            l_Init.Path().push_back(G3D::Vector3(l_HomePos.m_positionX, l_HomePos.m_positionY, l_HomePos.m_positionZ));

                            l_Init.SetFacing(l_HomePos.GetOrientation());

                            /// Set catmullrom movement
                            l_Init.SetSmooth();

                            l_Init.SetWalk(true);

                            /// Launch new movespline
                            l_Init.Launch();
                        });
                    }

                    /// Do talk and reset the script
                    if (p_Action == ActionOnDeathOfScout)
                    {
                        Talk(TalkWellDone, m_InvokerGUID);
                        ResetScript();
                    }
                }

                /// Set Invoker GUID for last talk
                void SetGUID(uint64 p_Guid, int32 p_Id) override
                {
                    if (p_Id == ActionSetGUID)
                    {
                        m_InvokerGUID = p_Guid;
                    }
                }

                /// Resets the script and spawns a npc scout
                void ResetScript()
                {
                    static Position const ScoutSpawnPositions[] =
                    {
                        { -1150.283f, -5519.264f, 12.03508f, 4.729842f },
                        { -1135.868f, -5414.137f, 13.35231f, 3.368485f },
                    };

                    me->SetWalk(false);

                    m_InvokerGUID = 0;

                    if (me->FindNearestCreature(NpcLegatiRogueTrainer, 30.0f, true))
                        m_JailorIndex = 7;

                    if (m_Activated)
                    {
                        if (GameObject* l_Cage = me->FindNearestGameObject(GoDarkspearCage, 30.0f))
                        {
                            l_Cage->SetLootState(GO_READY);
                            l_Cage->UseDoorOrButton();
                        }
                    }

                    if (Creature* l_Scout = me->FindNearestCreature(NpcSpitescaleScout, 50.0f))
                    {
                        m_SpitescaleScout = l_Scout->GetGUID();
                        return;
                    }

                    m_Activated = false;

                    if (Creature* l_Scout = me->SummonCreature(NpcSpitescaleScout, ScoutSpawnPositions[m_JailorIndex == 7 ? 1 : 0], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
                        m_SpitescaleScout = l_Scout->GetGUID();

                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
    
    	    	void Reset() override
    	    	{
                    /// Needed because this creature may get created before rogue trainer
                    m_Scheduler.Schedule(Seconds(1), [this](TaskContext /*p_Context*/)
                    {
                        ResetScript();
                    });
    	    	}

    	    	void UpdateAI(const uint32 p_Diff) override
    	    	{
                    m_Scheduler.Update(p_Diff);
    	    	}

            private:
                /// Schedulers
                TaskScheduler   m_Scheduler;

                /// Variables
                uint8           m_JailorIndex;
                bool            m_Activated;
                ObjectGuid      m_SpitescaleScout;
                ObjectGuid      m_InvokerGUID;
    	};

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            p_Player->PrepareGossipMenu(p_Creature);

            /// Give player and his group credit
            p_Player->KilledMonsterCreditGroupNear(CreditTalkToJailor);

            if (p_Player->GetQuestStatus(24786) == QUEST_STATUS_INCOMPLETE || p_Player->GetQuestStatus(24754) == QUEST_STATUS_INCOMPLETE ||
                p_Player->GetQuestStatus(24762) == QUEST_STATUS_INCOMPLETE || p_Player->GetQuestStatus(24774) == QUEST_STATUS_INCOMPLETE ||
                p_Player->GetQuestStatus(26276) == QUEST_STATUS_INCOMPLETE || p_Player->GetQuestStatus(24642) == QUEST_STATUS_INCOMPLETE ||
                p_Player->GetQuestStatus(24768) == QUEST_STATUS_INCOMPLETE || p_Player->GetQuestStatus(24780) == QUEST_STATUS_INCOMPLETE)
            {
                /// ADD TO NPC TEXT!
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I'm ready to face my challenge.", GOSSIP_SENDER_MAIN, ActionStartEvents);
            }

            p_Player->SEND_GOSSIP_MENU(NpcTextDarkspearJailor, p_Creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
        {
            ///if (p_Action == ActionStartEvents)
            {
                /// CastAI to do the action
                p_Creature->GetAI()->DoAction(ActionStartEvents);

                p_Creature->AI()->Talk(TalkGetIntoThePit, p_Player->GetGUID());
                p_Creature->AI()->SetGUID(p_Player->GetGUID(), ActionSetGUID);
            }

            p_Player->CLOSE_GOSSIP_MENU();
            return true;
        }
    
    	CreatureAI* GetAI(Creature* creature) const override
    	{
    		return new npc_darkspear_jailorAI(creature);
    	}
};

#pragma region (BreakingTheLineWaypoints)

/// Breaking the line
Position const FirstMonsterMoveWaypoint[] =
{
    { -1312.475f, -5560.088f, 21.13631f },
    { -1291.725f, -5562.338f, 21.13631f },
    { -1277.725f, -5572.088f, 21.13631f },
    { -1266.725f, -5583.588f, 20.88631f },
};

/// Face
Position const SecondMonsterMoveWaypoint[] = 
{
    { -1251.18f, -5587.85f, 20.8667f },
};

/// JumpGravity = 19.26112
/// Flags - 0x2000000 : Parabolic
Position const JumpPositionMonsterMoveWaypoint[] = 
{
    { -1217.77f, -5601.54f, 14.3751f },
};

///Flags - 0x500000 - UncompressedPath, SmoothGroundPath
Position const ThirdMonsterMoveWaypoint[] =
{
    { -1198.44f, -5610.4f,   13.8719f }, ///< 0
    { -1172.69f, -5617.15f,  14.6219f }, ///< 1
    ///{ -1136.19f, -5617.15f,  14.743278f }, ///< 2
    { -1136.19f, -5617.65f,  14.8719f }, ///< 3
    { -1105.19f, -5619.9f,   14.6219f }, ///< 4
    { -1058.44f, -5646.4f,   15.3719f }, ///< 5
    { -1040.44f, -5645.15f,  14.3719f }, ///< 6
    { -1024.19f, -5631.4f,   14.8719f }, ///< 7
    { -1017.44f, -5605.15f,  15.1219f }, ///< 8
    { -1004.94f, -5587.4f,   14.6219f }, ///< 9
    { -988.69f,  -5578.4f,   15.1219f }, ///< 10
    { -967.44f,  -5571.4f,   14.3719f }, ///< 11
};

/// Face
Position const FourthMonsterMoveWaypointFace[] =
{
    { -967.44f, -5571.4f, 14.3719f },
};

/// JumpGravity = 18.03737
/// Flags - 0x2000000 : Parabolic
Position const SecondJumpPositionMonsterMoveWaypoint[] =
{
    { -935.613f, -5556.77f, -0.1313f },
};

///Flags - 0x600000 - UncompressedPath, CanSwim
Position const FourthMonsterMoveWaypoint[] =
{
    { -934.441f, -5554.75f, -0.381686f }, ///< 0
    { -932.941f, -5552.25f, -0.841686f }, ///< 1
    { -930.941f, -5548.75f, -0.131686f  }, ///< 2
    { -927.833f, -5543.52f, -0.308285f  }, ///< 3
    ///{ -927.333f, -5542.77f, -1.30828f   }, ///< 4
    { -917.083f, -5524.77f, -0.808285f  }, ///< 5
    { -879.615f, -5520.75f, -0.881592f  }, ///< 6
    { -870.330f, -5523.85f, -1.11469f   }, ///< 7
    { -842.181f, -5532.68f, -0.774456f  }, ///< 8
    { -845.681f, -5533.43f, -0.774456f  }, ///< 9
    { -833.431f, -5539.43f, -0.774456f  }, ///< 10
    { -831.181f, -5541.43f, -0.274456f  }, ///< 11
    { -827.931f, -5543.68f,  0.225544f  }, ///< 12
    { -824.931f, -5545.68f,  0.725544f  }, ///< 13
    { -821.181f, -5548.68f,  1.47554f   }, ///< 14
    { -819.681f, -5549.68f,  1.47554f   }, ///< 15
    { -818.681f, -5550.68f,  1.47554f   }, ///< 16
    { -816.681f, -5550.18f,  1.47554f   }, ///< 17
    { -815.431f, -5550.93f,  1.97554f   }, ///< 18
    { -813.681f, -5551.18f,  3.22554f   }, ///< 19
    { -812.681f, -5551.68f,  3.72554f   }, ///< 20
    { -811.681f, -5552.43f,  3.97554f   }, ///< 21
    { -810.531f, -5553.52f,  4.06578f   }, ///< 22
};

///Flags - UncompressedPath - UncompressedPath
Position const FifthMonsterMoveWaypoint[] =
{
    { -811.637f, -5552.79f,  3.99417f  }, ///< 0
    { -812.637f, -5552.29f,  3.74417f  }, ///< 1
    { -813.137f, -5552.29f,  3.74417f  }, ///< 2
    { -813.637f, -5551.54f,  3.49417f  }, ///< 3
    { -814.887f, -5551.04f,  2.49417f  }, ///< 4
    { -816.137f, -5550.29f,  1.49417f  }, ///< 5
    { -816.637f, -5549.29f,  1.49417f  }, ///< 6
    { -824.137f, -5546.29f,  0.744173f }, ///< 7
    { -828.887f, -5544.54f,  0.244173f }, ///< 8
    { -833.387f, -5542.79f, -0.50827f  }, ///< 9
    { -854.730f, -5534.92f, -1.03044f  }, ///< 10
    { -855.534f, -5523.04f, -0.889496f }, ///< 11
    { -913.337f, -5519.66f, -1.24855f  }, ///< 12
};

#pragma endregion

/// Bloodtalon Thrasher - 38991
struct npc_bloodtalon_thrasher : public ScriptedAI
{
    public:
        npc_bloodtalon_thrasher(Creature* p_Creature) : ScriptedAI(p_Creature),
            m_PlayerGuid(0), m_CurrentPathId(0)
        {
            me->DisableChangeAI = true;
        }

        void OnEnterVehicle(Unit* p_Unit) override
        {
            /// First path
            if (m_CurrentPathId == 0)
            {
                me->getHostileRefManager().setOnlineOfflineState(false);
                p_Unit->getHostileRefManager().setOnlineOfflineState(false);

                m_PlayerGuid = p_Unit->GetGUID();

                Movement::MoveSplineInit l_Init(me, 5000000);

                l_Init.Path().push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));

                for (auto const& l_Path : FirstMonsterMoveWaypoint)
                    l_Init.Path().push_back(G3D::Vector3(l_Path.m_positionX, l_Path.m_positionY, l_Path.m_positionZ));

                /// Set catmullrom movement
                l_Init.SetSmooth();

                /// Launch new movespline
                l_Init.Launch();

                /// Set next path ID
                m_CurrentPathId = 1;
            }
        }

        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type != MOVE_SPLINE_DONE_VEHICLE)
                return;

            switch (m_CurrentPathId)
            {
                case 1:
                {
                    {
                        Movement::MoveSplineInit l_Init(me, 5000000);

                        l_Init.SetFacing(G3D::Vector3(SecondMonsterMoveWaypoint[0].m_positionX, SecondMonsterMoveWaypoint[0].m_positionY, SecondMonsterMoveWaypoint[0].m_positionZ));

                        /// Launch new movespline
                        l_Init.Launch();
                    }

                    {
                        Movement::MoveSplineInit l_Init(me, 5000000);

                        l_Init.MoveTo(G3D::Vector3(JumpPositionMonsterMoveWaypoint[0].m_positionX, JumpPositionMonsterMoveWaypoint[0].m_positionY, JumpPositionMonsterMoveWaypoint[0].m_positionZ), false);

                        l_Init.SetParabolic(19.26112f, 0.0f);

                        /// Launch new movespline
                        l_Init.Launch();

                        m_CurrentPathId = 3;
                    }
                    break;
                }

                case 3:
                {
                    Movement::MoveSplineInit l_Init(me, 5000000);

                    l_Init.Path().push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));

                    for (auto const& l_Path : ThirdMonsterMoveWaypoint)
                        l_Init.Path().push_back(G3D::Vector3(l_Path.m_positionX, l_Path.m_positionY, l_Path.m_positionZ));

                    /// Set catmullrom movement
                    l_Init.SetSmooth();

                    /// Launch new movespline
                    l_Init.Launch();

                    m_CurrentPathId = 4;
                    break;
                }

                case 4:
                {
                    {
                        Movement::MoveSplineInit l_Init(me, 5000000);

                        l_Init.SetFacing(G3D::Vector3(FourthMonsterMoveWaypointFace[0].m_positionX, FourthMonsterMoveWaypointFace[0].m_positionY, FourthMonsterMoveWaypointFace[0].m_positionZ));

                        /// Launch new movespline
                        l_Init.Launch();
                    }

                    {
                        Movement::MoveSplineInit l_Init(me, 5000000);

                        l_Init.MoveTo(G3D::Vector3(SecondJumpPositionMonsterMoveWaypoint[0].m_positionX, SecondJumpPositionMonsterMoveWaypoint[0].m_positionY, SecondJumpPositionMonsterMoveWaypoint[0].m_positionZ), false);

                        l_Init.SetParabolic(18.03737f, 0.0f);

                        /// Launch new movespline
                        l_Init.Launch();

                        m_CurrentPathId = 6;
                    }

                    break;
                }

                case 6:
                {
                    Movement::MoveSplineInit l_Init(me, 5000000);

                    l_Init.Path().push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));

                    for (auto const& l_Path : FourthMonsterMoveWaypoint)
                        l_Init.Path().push_back(G3D::Vector3(l_Path.m_positionX, l_Path.m_positionY, l_Path.m_positionZ));

                    l_Init.SetSwim(true);
                    l_Init.SetSmooth();

                    /// Launch new movespline
                    l_Init.Launch();

                    m_CurrentPathId = 7;

                    break;
                }

                case 7:
                {
                    Movement::MoveSplineInit l_Init(me, 5000000);

                    for (auto const& l_Path : FifthMonsterMoveWaypoint)
                        l_Init.Path().push_back(G3D::Vector3(l_Path.m_positionX, l_Path.m_positionY, l_Path.m_positionZ));

                    /// Launch new movespline
                    l_Init.Launch();

                    if (Player* l_Player = ObjectAccessor::GetPlayer(*me, m_PlayerGuid))
                    {
                        l_Player->getHostileRefManager().setOnlineOfflineState(true);
                        l_Player->ExitVehicle();
                    }

                    m_Scheduler.Schedule(Seconds(7), [this](TaskContext /*p_Context*/)
                    {
                        me->DespawnOrUnsummon(1000);
                    });

                    break;
                }

                default:
                    break;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);
        }

    private:
        /// Schedulers
        TaskScheduler m_Scheduler;

        /// Variables
        bool        m_Locked;
        uint32      m_CurrentPathId;
        ObjectGuid  m_PlayerGuid;
};

/// Zuni - 37988
struct npc_zuni_37988 : public PassiveAI
{
    enum eTalks
    {
        TalkLine0,
        TalkLine1,
    };

    public:
        npc_zuni_37988(Creature* p_Creature) : PassiveAI(p_Creature)
        {
            me->setActive(true);
        }

        void Reset()
        {
            m_Scheduler.Schedule(Seconds(7), [this](TaskContext /*p_Context*/)
            {
                me->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);

                if (Unit* l_Owner = ObjectAccessor::GetUnit(*me, me->GetCharmerOrOwnerGUID()))
                    me->SetFacingToObject(l_Owner);
                else if (me->isSummon() && me->ToTempSummon()->GetSummoner())
                    me->SetFacingToObject(me->ToTempSummon()->GetSummoner());
            });

            m_Scheduler.Schedule(Seconds(8), [this](TaskContext /*p_Context*/)
            {
                Talk(TalkLine0);
            });

            m_Scheduler.Schedule(Milliseconds(12500), [this](TaskContext /*p_Context*/)
            {
                Movement::MoveSplineInit l_Init(me);

                l_Init.Path().push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));

                /// See waypoints entry 3798800
                /// When function is made in MotionMaster, move it!

                l_Init.Path().push_back(G3D::Vector3(-1171.6f, -5275.5f, 0.875105f));
                l_Init.Path().push_back(G3D::Vector3(-1170.57f, -5283.05f, 1.24778f));
                l_Init.Path().push_back(G3D::Vector3(-1169.18f, -5295.41f, 3.89082f));
                l_Init.Path().push_back(G3D::Vector3(-1170.49f, -5308.3f, 8.49512f));
                l_Init.Path().push_back(G3D::Vector3(-1171.77f, -5321.09f, 12.2087f));
                l_Init.Path().push_back(G3D::Vector3(-1172.98f, -5333.17f, 14.4536f));
                l_Init.Path().push_back(G3D::Vector3(-1168.9f, -5343.4f, 15.1187f));
                l_Init.Path().push_back(G3D::Vector3(-1165.36f, -5355.14f, 14.6408f));
                l_Init.Path().push_back(G3D::Vector3(-1164.11f, -5364.4f, 14.5861f));
                l_Init.Path().push_back(G3D::Vector3(-1163.89f, -5374.46f, 12.6836f));
                l_Init.Path().push_back(G3D::Vector3(-1166.16f, -5386.9f, 12.0185f));

                l_Init.SetSmooth();

                l_Init.Launch();
            });

            m_Scheduler.Schedule(Seconds(28), [this](TaskContext /*p_Context*/)
            {
                Talk(TalkLine1);
            });
            
            m_Scheduler.Schedule(Seconds(33), [this](TaskContext /*p_Context*/)
            {
                me->DespawnOrUnsummon();
            });
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);
        }

    private:
        /// Schedulers
        TaskScheduler m_Scheduler;
};

/// Zuni - 38930
struct npc_zuni_38930 : public ScriptedAI
{
    enum eTalks
    {
        TalkLine1,
        TalkLine2,
        TalkLine3,
        TalkLine4,
    };

    enum ePoints
    {
        PointZalazaneFall = 1,
    };

    public:
        npc_zuni_38930(Creature* p_Creature) : ScriptedAI(p_Creature), m_Started(false), m_MovementInformStartedPointA(false), m_MovementInformStartedPointB(false)
        {
            me->setActive(true);
        }

        void Reset() override
        {
            /// Incase reset is called twice, lock it (Probably won't happen)
            if (m_Started)
                return;

            m_Started = true;

            if (me->isSummon() && me->ToTempSummon()->GetSummoner())
                me->GetMotionMaster()->MoveFollow(me->ToTempSummon()->GetSummoner(), 0.5f, me->GetFollowAngle());

            m_Scheduler.Schedule(Seconds(1), [this](TaskContext /*p_Context*/)
            {
                Talk(TalkLine1);
            });

            m_Scheduler.Schedule(Seconds(7), [this](TaskContext /*p_Context*/)
            {
                Talk(TalkLine2);
            });

            m_Scheduler.Schedule(Seconds(14), [this](TaskContext /*p_Context*/)
            {
                Talk(TalkLine3);
            });
        }

        /// Called from areatrigger SAI Script && Invoker target type on quest accept
        void SetData(uint32 p_Id, uint32 p_Data) override
        {
            me->GetMotionMaster()->Clear();

            if (p_Id == 1 && p_Data == 0 && !m_MovementInformStartedPointA)
            {
                m_MovementInformStartedPointA = true;

                m_Scheduler.Schedule(Seconds(1), [this](TaskContext /*p_Context*/)
                {
                    Talk(TalkLine4);
                });

                m_Scheduler.Schedule(Seconds(2), [this](TaskContext /*p_Context*/)
                {
                    static const Position l_LeaveIsleWaypointPos[] =
                    {
                        { -1544.06f, -5292.3f, 8.54982f },
                        { -1640.31f, -5291.09f,  8.04982f },
                        { -1518.40f, -5283.79f,  6.94804f },
                        { -1515.15f, -5281.29f,  6.19804f },
                        { -1510.40f, -5277.79f,  5.69804f },
                        { -1505.15f, -5274.04f,  4.94804f },
                        { -1500.79f, -5270.65f,  4.15815f },
                        { -1497.54f, -5268.40f,  3.40815f },
                        { -1495.04f, -5266.65f,  2.65815f },
                        { -1492.04f, -5264.15f,  1.40815f },
                        { -1490.29f, -5263.15f,  0.65815f },
                        { -1487.79f, -5261.41f,  0.15815f },
                        { -1484.79f, -5259.41f, -0.59184f },
                        { -1483.04f, -5258.15f, -0.09184f },
                        { -1470.62f, -5249.48f, -2.39069f },
                        { -1469.37f, -5248.48f, -2.39069f },
                        { -1456.22f, -5239.68f, -2.05244f },
                        { -1449.41f, -5235.17f, -1.30116f },
                        { -1442.91f, -5230.67f, -0.301181f },
                        { -1439.66f, -5228.67f, -0.0511602f},
                        { -1437.16f, -5226.92f, -0.80116f  },
                        { -1433.91f, -5224.92f, -0.0511602f},
                        { -1432.16f, -9223.67f, 0.448841f  },
                        { -1429.66f, -5221.92f, 1.19884f  },
                        { -1427.16f, -5220.42f, 1.69884f  },
                        { -1426.09f, -5219.65f, 1.93012f  },
                    };

                    Movement::MoveSplineInit l_Init(me);

                    for (auto const& l_Path : l_LeaveIsleWaypointPos)
                        l_Init.Path().push_back(G3D::Vector3(l_Path.GetPositionX(), l_Path.GetPositionY(), l_Path.GetPositionZ()));

                    l_Init.Launch();
                });

                m_Scheduler.Schedule(Seconds(21), [this](TaskContext /*p_Context*/)
                {
                    me->DespawnOrUnsummon();
                });
            }
            else if (p_Id == 2 && p_Data == 0 && !m_MovementInformStartedPointB)
            {
                m_MovementInformStartedPointB = true;

                static const Position l_AreaTriggerPos = { -1442.626953f, -5329.797852f, 3.735778f };
                if (me->GetDistance(l_AreaTriggerPos) > 25.0f)
                    me->NearTeleportTo(l_AreaTriggerPos);

                static const Position l_FirstIsleWaypoint[] =
                {
                    {-1448.444f, -5328.11f, 3.313706f},
                    {-1452.444f, -5327.11f, 3.313706f},
                    {-1456.194f, -5326.11f, 3.063706f},
                    {-1460.194f, -5325.11f, 2.813706f},
                    {-1463.944f, -5324.36f, 2.563706f},
                    {-1467.944f, -5323.36f, 2.563706f},
                    {-1471.694f, -5322.36f, 2.563706f},
                    {-1475.694f, -5321.36f, 2.563706f},
                    {-1479.694f, -5320.36f, 2.563706f},
                    {-1483.444f, -5319.61f, 2.563706f},
                    {-1487.444f, -5318.61f, 2.563706f},
                    {-1491.194f, -5317.61f, 2.313706f},
                    {-1495.194f, -5316.61f, 3.063706f},
                    {-1498.944f, -5315.61f, 4.063706f},
                    {-1502.694f, -5314.86f, 4.813706f},
                    {-1506.444f, -5313.86f, 6.063706f},
                    {-1510.444f, -5312.86f, 6.563706f},
                    {-1514.194f, -5312.11f, 6.563706f},
                    {-1517.944f, -5310.61f, 6.813706f},
                    {-1525.444f, -5307.61f, 7.313706f},
                    {-1525.444f, -5307.61f, 7.313706f},
                    {-1529.194f, -5306.11f, 7.563706f},
                    {-1532.694f, -5304.36f, 7.813706f},
                    {-1536.444f, -5302.86f, 8.313706f},
                    {-1540.194f, -5301.36f, 8.813706f},
                    {-1543.944f, -5299.86f, 9.063706f},
                    {-1547.444f, -5298.36f, 9.313706f},
                    {-1551.194f, -5296.86f, 9.563706f},
                    {-1553.900f, -5295.58f, 9.110053f},
                };

                Movement::MoveSplineInit l_Init(me);

                for (auto const& l_Path : l_FirstIsleWaypoint)
                    l_Init.Path().push_back(G3D::Vector3(l_Path.GetPositionX(), l_Path.GetPositionY(), l_Path.GetPositionZ()));

                l_Init.Launch();

                m_Scheduler.Schedule(Seconds(20), [this](TaskContext /*p_Context*/)
                {
                    /// Seems like orientation isn't hardcoded, he faces the bunny
                    if (Creature* l_Bunny = me->FindNearestCreature(NpcEchoIslesQuestBunny, 10.0f))
                        me->SetFacingToObject(l_Bunny);
                });
            }
        }

        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type != POINT_MOTION_TYPE)
                return;

            if (p_Id == PointZalazaneFall)
            {
                m_Scheduler.Schedule(Seconds(1), [this](TaskContext /*p_Context*/)
                {
                    me->DespawnOrUnsummon();
                });
            }
        }
        
        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);
        }

    private:
        /// Schedulers
        TaskScheduler m_Scheduler;

        /// Variables
        bool m_Started;
        bool m_MovementInformStartedPointA;
        bool m_MovementInformStartedPointB;
};

/// Swiftclaw - 37989
struct npc_swiftclaw_37989 : public PassiveAI
{
    enum eSwiftclaw
    {
        PathSwiftclaw = 37989,
    };

    public:
        npc_swiftclaw_37989(Creature* p_Creature) : PassiveAI(p_Creature) { }

        void Reset()
        {
            me->GetMotionMaster()->MovePath(PathSwiftclaw, true);
            me->setActive(true);
        }
};

/// Swiftclaw - 38002
struct npc_swiftclaw_38002 : public VehicleAI
{
    /// NOTES: NEEDS IMPROVEMENTS
    /// THIS IS HACKED ATM

    enum eSwiftclaw
    {
        PathSwiftclaw = 37989,
    };

    enum eSpells
    {
        SpellRaptorRoot         = 70927,
        SpellRaptorLassoCredit  = 70943,
        SpellRaptorTurninCredit = 70941,
        SpellEjectAllPassengers = 50630,
    };

    enum eTalks
    {
        TalkLineOne,
    };

    enum ePoints
    {
        PointAtFire = 1,
    };

    enum eNpcs
    {
        NpcNotera = 38988,
    };

    public:
        npc_swiftclaw_38002(Creature* p_Creature) : VehicleAI(p_Creature), m_Locked(false)
        {
            me->DisableChangeAI = true;
        }

        void IsSummonedBy(Unit* /*p_Summoner*/) override
        {
            static const Position l_SwiftclawPath[] =
            {
                { -1523.94f, -5287.3f,  7.18772f },
                { -1522.94f, -5283.49f, 7.18772f },
                { -1523.94f, -5278.8f,  6.93772f },
                { -1520.94f, -5273.3f,  6.68772f },
                { -1519.69f, -5270.8f,  6.43772f },
                { -1518.69f, -5266.55f, 6.43772f },
                { -1516.68f, -5264.37f, 6.32617f },
                { -1520.35f, -5257.4f,  5.77623f },
                { -1529.6f,  -5251.65f, 5.77623f },
                { -1546.13f, -5243.79f, 6.6781f  },
                { -1553.38f, -5239.36f, 4.75678f },
                { -1557.38f, -5236.29f, 4.25678f },
                { -1566.53f, -5230.86f, 3.55353f },
                { -1571.3f,  -5227.36f, 3.30353f },
                { -1588.62f, -5226.9f,  3.58985f },
                { -1590.62f, -5233.65f, 4.58986f },
                { -1592.51f, -5240.3f,  5.26926f },
                { -1590.51f, -5245.8f,  5.52693f },
                { -1588.76f, -5252.3f,  6.27693f },
                { -1587.12f, -5256.35f, 6.60561f },
                { -1576.37f, -5261.35f, 7.10561f },
                { -1565.87f, -5266.6f,  7.60561f },
                { -1558.64f, -5270.46f, 7.97374f },
                { -1555.39f, -5276.46f, 8.47374f },
                { -1550.77f, -5285.72f, 9.11867f },
                { -1552.21f, -5291.72f, 9.26187f },
                { -1555.22f, -5303.75f, 8.73295f },
                { -1562.54f, -5319.57f, 8.2565f  },
                { -1576.29f, -5324.78f, 7.94616f },
                { -1586.76f, -5322.35f, 7.4979f  },
                { -1589.27f, -5311.73f, 7.71858f },
                { -1588.77f, -5299.98f, 8.46857f },
                { -1591.58f, -5282.3f,  7.98663f },
                { -1593.83f, -5281.8f,  7.73663f },
                { -1600.84f, -5282.3f,  7.23663f },
                { -1606.58f, -5279.55f, 6.48663f },
                { -1616.33f, -5275.55f, 6.23663f },
                { -1621.33f, -5272.55f, 5.73663f },
                { -1624.84f, -5271.3f,  5.23663f },
                { -1628.58f, -5272.54f, 4.23663f },
                { -1637.92f, -5298.38f, 2.42262f },
                { -1620.12f, -5337.79f, 5.41152f },
                { -1544.06f, -5326.36f, 7.4163f  },
                { -1533.47f, -5334.75f, 7.23221f },
                { -1532.97f, -5342.5f,  6.48221f },
                { -1539.72f, -5351.25f, 6.23221f },
                { -1541.92f, -5354.65f, 5.77246f },
                { -1536.17f, -5361.4f,  5.27246f },
                { -1532.17f, -5366.65f, 5.22461f },
                { -1527.68f, -5364.26f, 5.80379f },
                { -1523.68f, -5361.26f, 6.30379f },
                { -1516.84f, -5355.94f, 6.54045f },
                { -1514.34f, -5348.69f, 5.79045f },
                { -1512.43f, -5342.87f, 5.38673f },
                { -1508.93f, -5332.87f, 5.88673f },
                { -1512.93f, -5329.62f, 6.38673f },
                { -1518.43f, -5325.62f, 6.88673f },
                { -1531.28f, -5314.96f, 7.62983f },
                { -1536.78f, -5310.71f, 8.12983f },
                { -1542.78f, -5305.96f, 8.62983f },
                { -1550.53f, -5299.71f, 9.37983f },
                { -1565.6f,  -5291.41f, 8.74975f },
                { -1576.53f, -5285.68f, 8.49784f },
                { -1585.78f, -5272.17f, 7.98737f },
                { -1563.26f, -5265.67f, 7.73737f },
                { -1555.93f, -5266.58f, 7.52478f },
                { -1549.68f, -5268.83f, 7.77478f },
                { -1544.68f, -5275.77f, 7.77478f },
                { -1542.18f, -5279.77f, 7.77478f },
                { -1537.43f, -5282.77f, 7.52478f },
                { -1535.19f, -5284.55f, 7.68772f },
                { -1531.94f, -5287.3f,  7.43772f },
            };

            Movement::MoveSplineInit l_Init(me);

            for (auto const& l_Path : l_SwiftclawPath)
                l_Init.Path().push_back(G3D::Vector3(l_Path.GetPositionX(), l_Path.GetPositionY(), l_Path.GetPositionZ()));

            l_Init.SetCyclic();

            l_Init.Launch();
        }

        void SpellHit(Unit* p_Unit, SpellInfo const* p_SpellInfo) override
        {
            if (p_SpellInfo->Id == SpellRaptorRoot)
            {
                me->GetMotionMaster()->Clear();
                me->SetRooted(true);

                p_Unit->CastSpell(me, 82316, true);
                ///DoCast(p_Unit, 82316);



                /// Uncomment if you want crash :)
                ////// 52391
                ///if (Creature* l_Creature = me->FindNearestCreature(37995, 50.0f))
                ///{
                ///    l_Creature->EnterVehicle(me, 0);
                ///    l_Creature->CastSpell(me, 70918, true);
                ///    l_Creature->CastSpell(me, 70919, true);
                ///    me->AddAura(70295, p_Unit);
                ///}

                ///if (p_Unit)
                ///    p_Unit->EnterVehicle(me);
            }
        }

        void PassengerBoarded(Unit* p_Unit, int8 /*p_SeatId*/, bool p_Apply) override
        {
            if (p_Apply)
            {
                if (m_Locked)
                    return;

                m_Locked = true;

                me->SetRooted(false);

                ///p_Unit->CastSpell(me, SpellRaptorRoot, true);

                DoCast(p_Unit, SpellRaptorLassoCredit);

                /// TODO : Figure out why spell above doesn't work
                ///if (Player* l_Player = p_Unit->ToPlayer())
                ///    l_Player->KilledMonsterCredit(37989);

                Talk(TalkLineOne);
            }
            else
            {
                if (!me->FindNearestCreature(eNpcs::NpcNotera, 30.0f))
                {
                    if (Player* l_Player = p_Unit->ToPlayer())
                        l_Player->FailQuest(24626);

                    me->DespawnOrUnsummon(1000);
                    return;
                }

                /// TODO : Figure out why spell in SAI doesn't work too
                ///if (Player* l_Player = p_Unit->ToPlayer())
                ///    l_Player->KilledMonsterCredit(38002);

                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(PointAtFire, -1307.224609f, -5454.918457f, 14.691741f);

                me->DespawnOrUnsummon(60000);

                m_Scheduler.Schedule(Seconds(3), [this](TaskContext /*p_Context*/)
                {
                    me->SetHomePosition(*me);
                    me->GetMotionMaster()->MoveRandom(5.0f);
                });
            }
        }

        void MotionInform(uint32 p_Type, uint32 p_Id)
        {
            if (p_Type != POINT_MOTION_TYPE)
                return;

            if (p_Id == PointAtFire)
            {
            }
        }

        void SetData(uint32 p_Id, uint32 p_Data) override
        {
            if (p_Id == 1 && p_Data == 1)
            {
                DoCast(SpellEjectAllPassengers);

                if (Unit* l_Summoner = me->ToTempSummon()->GetOwner())
                    l_Summoner->CastSpell(l_Summoner, eSpells::SpellRaptorTurninCredit, true);

            }
        }

        void OnSpellClick(Unit* /*p_Clicker*/, bool& p_Result) override
        {
            p_Result = false;
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);
        }

    private:
        /// Schedulers
        TaskScheduler m_Scheduler;

        /// Variables
        bool m_Locked;
};

class item_territorial_fetish : public ItemScript
{
    enum eNpcs
    {
        NpcSpitescaleBunny = 38560,
    };

    enum eSpells
    {
        SpellTerrorialFetish = 72072,
    };

    public:
        item_territorial_fetish() : ItemScript("item_territorial_fetish") { }

        bool OnUse(Player* p_Player, Item* /*p_Item*/, SpellCastTargets const& /*p_Targets*/)
        {
            if (Creature* l_Creature = p_Player->FindNearestCreature(eNpcs::NpcSpitescaleBunny, 4.0f))
            {
                if (l_Creature->HasAura(SpellTerrorialFetish))
                {
                    p_Player->GetSession()->SendNotification("This Spietscale Flag has already been marked!");
                    return true;
                }
            }
            else
            {
                p_Player->GetSession()->SendNotification("You are not near a Spitescale Flag");
                return true;
            }

            return false;
        }
};

/// 38560 - Spitescale Flag Bunny
struct npc_spitescale_flag_bunny : public ScriptedAI
{
    enum eSpells
    {
        SpellPlaceTerritorialFetish = 72070,
        SpellTerritorialFetish      = 72072,
    };

    public:
        npc_spitescale_flag_bunny(Creature* p_Creature) : ScriptedAI(p_Creature) { }


        void SpellHit(Unit* /*p_Attacker*/, SpellInfo const* p_Spell) override
        {
            if (p_Spell->Id == SpellPlaceTerritorialFetish)
            {
                DoCast(me, SpellTerritorialFetish);

                m_Scheduler.Schedule(Seconds(60), [this](TaskContext /*p_Context*/)
                {
                    me->RemoveAura(SpellTerritorialFetish);
                });
            }
        }


        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);
        }

    private:
        /// Shedulers
        TaskScheduler m_Scheduler;
};

/////////////////////////////////////////////////////////////////////////////////////////////
/// An Ancient Enemy
/////////////////////////////////////////////////////////////////////////////////////////////

enum eKilledMonsterCredits
{
    SpeakWithVolJin = 38225,
};

enum eNpcs
{
    /// Allies
    NpcVolJin                       = 38225,
    NpcVanira                       = 38437,
    NpcZuni                         = 38423,

    /// Enemies
    NpcZaraJira                     = 38306,
    NpcManifestationOfTheSeaWitch   = 38302,
    NpcFiresOfTheSeas               = 38542,

    /// Trigger/Bunnies
    NpcSeaWitchChannelBunny         = 38452,
};

enum ePhases
{
    PhaseAll,
    PhaseOne,
    PhaseTwo,
    PhaseThree,
    PhaseFour,
};

enum eActions
{
    ActionSeaWitchDoTalk0 = 1,
    ActionSeaWitchDoTalk2,

    ActionVaniraDoTalk1,
    ActionVaniraDoTalk2,
    ActionVaniraDoTalk3,
    ActionVaniraDoTalk4,

    ActionVolJinDoTalk1,
    ActionVolJinDoTalk2,
    ActionVolJinDoTalk3,
};

enum eSharedSpells
{
    SpellFreezingTouch = 73004,
};

struct AncientEnemyAI : public ScriptedAI
{
    public:
        AncientEnemyAI(Creature* p_Creature, bool p_CanMelee = false) : ScriptedAI(p_Creature), m_CanMelee(p_CanMelee) { }

        /// Don't allow combat movement
        void AttackStart(Unit* p_Who) override
        {
            if (p_Who && me->Attack(p_Who, true))
            {
                if (me->GetMotionMaster()->GetMotionSlotType(MOTION_SLOT_ACTIVE) == POINT_MOTION_TYPE)
                    me->GetMotionMaster()->MovementExpired();

                if (m_CanMelee)
                    me->GetMotionMaster()->MoveChase(p_Who);
            }
        }

        bool CanAttackTarget(Unit const* p_Target) const override
        {
            uint32 l_Entry = p_Target->GetEntry();

            switch (l_Entry)
            {
                case NpcVolJin:
                case NpcVanira:
                case NpcZuni:
                case NpcZaraJira:
                    return true;
            }

            return false;
        }

        void JustRespawned() override
        {
            me->RemoveAllAuras();

            /// Event cleanup
            m_Events.Reset();
            m_Events.SetPhase(ePhases::PhaseOne);
        }

        /// Schedulers
        TaskScheduler m_Scheduler;
        EventMap m_Events;

        /// Variables
        bool m_CanMelee;
};

/// Vol'Jin - 38225
struct npc_voljin_38225 : public AncientEnemyAI
{
    enum eSpells
    {
        SpellShadowShock    = 73087,
        SpellShoot          = 20463,
        SpellDelugeOfShadow = 72044,
    };

    enum ePoints
    {
        PointMoveAfterGossipSelect = 1,
        PointOnDataSet,
    };

    enum eTalks
    {
        TalkSpeak0,
        TalkSpeak1, ///< It be done. Our ancient enemy is defeated.
        TalkSpeak2,
        TalkSpeak3,
        TalkSpeak4,
    };

    public:
        npc_voljin_38225(Creature* p_Creature) : AncientEnemyAI(p_Creature),
            m_GossipClicked(false)
        {
            me->SetReactState(ReactStates::REACT_DEFENSIVE);
        }

        void JustRespawned() override
        {
            AncientEnemyAI::JustRespawned();
            m_GossipClicked = false;
        }

        void DoAction(const int32 p_Action) override
        {
            if (p_Action == eActions::ActionVolJinDoTalk1)
            {
                Talk(eTalks::TalkSpeak1);
            }

            if (p_Action == eActions::ActionVolJinDoTalk2)
            {
                Talk(eTalks::TalkSpeak2);
            }

            if (p_Action == eActions::ActionVolJinDoTalk3)
            {
                Talk(eTalks::TalkSpeak3);
            }
        }

        void Reset()
            {
                me->SetWalk(true);
            }

        void sGossipHello(Player* p_Player) override
            {
                p_Player->KilledMonsterCreditGroupNear(SpeakWithVolJin);
            }

        void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 /*p_Action*/) override
        {
            if (m_GossipClicked)
                return;

            m_GossipClicked = true;
        
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            static const Position l_PositionWalkRun(-721.712f, -5597.0f, 25.4988f, 0.894059f);

            me->GetMotionMaster()->MovePoint(PointMoveAfterGossipSelect, l_PositionWalkRun);

            if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                l_Vanira->AI()->SetData(0, 1);
        }

        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type != POINT_MOTION_TYPE)
                return;

            if (p_Id == ePoints::PointMoveAfterGossipSelect)
            {
                Talk(eTalks::TalkSpeak0);
                me->SetHomePosition(*me);
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 333);

                m_Scheduler.Schedule(Seconds(10), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        l_Zara->AI()->DoAction(eActions::ActionSeaWitchDoTalk0);
                });

                m_Scheduler.Schedule(Seconds(11), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Zuni = me->FindNearestCreature(eNpcs::NpcZuni, 75.0f))
                        l_Zuni->AI()->SetData(1, 1);
                });

                m_Scheduler.Schedule(Seconds(23), [this](TaskContext /*p_Context*/)
                {
                    std::vector<Object*> l_Objects;

                    Trinity::AllWorldObjectsInRange u_check(me, 50.0f);
                    Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, l_Objects, u_check);
                    Cell::VisitAllObjects(me, searcher, 50.0f);

                    for (auto l_Object : l_Objects)
                    {
                        if (!l_Object)
                            continue;

                        Creature* l_Creature = l_Object->ToCreature();
                        if (!l_Creature)
                            continue;

                        l_Creature->AI()->SetData(5, 5);
                    }

                    if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        l_Zara->RemoveFlag(UNIT_FIELD_FLAGS, 33024);

                    if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                        l_Vanira->RemoveFlag(UNIT_FIELD_FLAGS, 131970);

                    if (Creature* l_Zuni = me->FindNearestCreature(eNpcs::NpcZuni, 50.0f))
                        l_Zuni->RemoveFlag(UNIT_FIELD_FLAGS, 131970);

                    me->RemoveFlag(UNIT_FIELD_FLAGS, 131970);
                });

                m_Scheduler.Schedule(Milliseconds(23200), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        DoCast(l_Zara, eSpells::SpellShoot);
                });

                m_Scheduler.Schedule(Milliseconds(23400), [this](TaskContext /*p_Context*/)
                {
                    std::vector<Object*> l_Objects;

                    Trinity::AllWorldObjectsInRange u_check(me, 90.0f);
                    Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(me, l_Objects, u_check);
                    Cell::VisitAllObjects(me, searcher, 90.0f);

                    for (auto l_Object : l_Objects)
                    {
                        if (!l_Object)
                            continue;

                        Creature* l_Creature = l_Object->ToCreature();
                        if (!l_Creature)
                            continue;

                        l_Creature->AI()->SetData(2, 10);
                    }

                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 376);
                });
            }

            if (p_Id == ePoints::PointOnDataSet)
            {
                me->DespawnOrUnsummon();
            }
        }

        void SetData(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type == 1)
            {
                if (p_Id == 8)
                {
                    Talk(eTalks::TalkSpeak4);
                    m_Events.SetPhase(ePhases::PhaseTwo);
                    if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        DoCast(l_Zara, eSpells::SpellDelugeOfShadow);
                }

                if (p_Id == 9)
                {
                    me->InterruptNonMeleeSpells(false, eSpells::SpellDelugeOfShadow, true);
                }

                if (p_Id == 10)
                {
                    m_Events.SetPhase(ePhases::PhaseTwo);
                    if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        DoCast(l_Zara, eSpells::SpellDelugeOfShadow);
                }

                if (p_Id == 11)
                {
                    me->InterruptNonMeleeSpells(false, eSpells::SpellDelugeOfShadow, true);

                    m_Scheduler.Schedule(Milliseconds(500), [this](TaskContext /*p_Context*/)
                    {
                        Talk(eTalks::TalkSpeak1);
                    });
                }
            }

            if (p_Type == 3)
            {
                if (p_Id == 5)
                {
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, 131970);
                    me->GetMotionMaster()->MovePoint(ePoints::PointOnDataSet, { -737.527f, -5609.75f, 25.4431f });
                }

                if (p_Id == 6)
                {
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, 898);
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                }
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            if (p_Who->GetEntry() == eNpcs::NpcZaraJira)
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(eSpells::SpellShadowShock, urand(1000, 1500), 0, ePhases::PhaseOne);
                m_Events.ScheduleEvent(eSpells::SpellShoot, urand(1000, 1500), 0, ePhases::PhaseOne);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);

            if (!UpdateVictim())
                return;

            m_Events.Update(p_Diff);

            if (uint32 l_EventId = m_Events.ExecuteEvent())
            {
                switch (l_EventId)
                {
                    case SpellShadowShock:
                        DoCastVictim(SpellShadowShock);
                        m_Events.ScheduleEvent(SpellShadowShock, urand(7000, 8000), 0, ePhases::PhaseOne);
                        break;

                    case SpellShoot:
                        DoCastVictim(SpellShoot);
                        m_Events.ScheduleEvent(SpellShoot, 1700, 0, ePhases::PhaseOne);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        /// Varaiables
        bool m_GossipClicked;
};

/// Vanira - 38437
struct npc_vanira_38437 : public AncientEnemyAI
{
    enum eSpells
    {
        SpellVanirasRecall      = 73334,
        SpellManaStreamTotem    = 73393,
        SpellHealingStreamTotem = 71984,
    };

    enum ePoints
    {
        PointMoveFirst = 1,
        PointMoveSecond,
        PointMoveToZuni,
    };

    enum eTalks
    {
        TalkSpeak0,
        TalkSpeak1,
        TalkSpeak2,
        TalkSpeak3,
        TalkSpeak4,
    };

    public:
        npc_vanira_38437(Creature* p_Creature) : AncientEnemyAI(p_Creature, true)
        {
            me->SetReactState(ReactStates::REACT_DEFENSIVE);
        }

        void Reset() override
        {
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
            me->SetWalk(true);
        }

        void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 /*p_Action*/) override
        {
            p_Player->CastSpell(p_Player, eSpells::SpellVanirasRecall, true);
        }

        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type != POINT_MOTION_TYPE)
                return;

            if (p_Id == ePoints::PointMoveFirst)
            {
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 333);

                if (Creature* l_Zarajira = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                    me->SetFacingToObject(l_Zarajira, true);

                me->SetHomePosition(*me);
            }

            if (p_Id == ePoints::PointMoveSecond)
            {
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 8, 0);
            }

            if (p_Id == ePoints::PointMoveToZuni)
            {
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 16);
            }
        }

        void DoAction(const int32 p_Action) override
        {
            switch (p_Action)
            {
                case eActions::ActionVaniraDoTalk1:
                    Talk(eTalks::TalkSpeak1);

                    if (Creature* l_Zuni = me->FindNearestCreature(eNpcs::NpcZuni, 50.0f))
                        me->GetMotionMaster()->MovePoint(PointMoveToZuni, *l_Zuni);

                    break;

                case eActions::ActionVaniraDoTalk2:
                    Talk(eTalks::TalkSpeak2);
                    break;

                case eActions::ActionVaniraDoTalk3:
                    Talk(eTalks::TalkSpeak3);
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);

                    if (Creature* l_Zarajira = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f, false))
                        me->SetFacingToObject(l_Zarajira, true);

                    break;

                case eActions::ActionVaniraDoTalk4:
                    Talk(eTalks::TalkSpeak4);
                    break;
            }
        }

        void SetData(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type == 0)
            {
                if (p_Id == 1)
                {
                    me->GetMotionMaster()->MovePoint(ePoints::PointMoveFirst, { -720.612f, -5600.87f, 25.4991f, 0.860107f });
                }
            }

            if (p_Type == 2)
            {
                if (p_Id == 10)
                {
                    if (Creature* l_Zarajira = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        me->AI()->AttackStart(l_Zarajira);

                    Talk(eTalks::TalkSpeak0);
                }

                if (p_Id == 9)
                {
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, 131970);
                }

                if (p_Id == 8)
                {
                    m_Events.SetPhase(PhaseTwo);
                    me->GetMotionMaster()->MovePoint(ePoints::PointMoveSecond, -712.649f, -5581.2f, 25.5008f);
                }

                if (p_Id == 7)
                {
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            if (p_Who->GetEntry() == eNpcs::NpcZaraJira)
            {
                m_Events.Reset();

                m_Events.SetPhase(ePhases::PhaseOne);
                m_Events.ScheduleEvent(eSpells::SpellManaStreamTotem, urand(5000, 6000), 0, ePhases::PhaseOne);
                m_Events.ScheduleEvent(eSpells::SpellHealingStreamTotem, urand(1000, 2000), 0, ePhases::PhaseOne);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);
            
            if (!UpdateVictim())
                return;

            m_Events.Update(p_Diff);

            if (uint32 l_EventId = m_Events.ExecuteEvent())
            {
                switch (l_EventId)
                {
                    case eSpells::SpellManaStreamTotem:
                        DoCast(me, eSpells::SpellManaStreamTotem, true);
                        m_Events.ScheduleEvent(eSpells::SpellManaStreamTotem, urand(22000, 23000), 0, ePhases::PhaseOne);
                        break;
                    case eSpells::SpellHealingStreamTotem:
                        DoCast(me, eSpells::SpellHealingStreamTotem, true);
                        m_Events.ScheduleEvent(eSpells::SpellHealingStreamTotem, 12000, 0, ePhases::PhaseOne);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
};

/// Zuni - 38423
struct npc_zuni_38423 : public AncientEnemyAI
{
    enum eTalks
    {
        TalkSpeak0,
    };

    enum ePoints
    {
        PointMoveFirst = 1,
        PointMoveThird,
    };

    enum eSpells
    {
        SpellLightningBolt  = 73254,
        SpellEarthShock     = 73255,
        SpellFrostExplosion = 69252,
        SpellPermFeignDeath = 29266,
    };

    public:
        npc_zuni_38423(Creature* p_Creature) : AncientEnemyAI(p_Creature) 
        {
            me->SetReactState(ReactStates::REACT_DEFENSIVE);
        }

        void Reset() override
        {
            me->SetWalk(true);
        }

        void SetData(uint32 p_Type, uint32 p_Id) override
        {
            switch (p_Type)
            {
                case 1:
                {
                    if (p_Id == 1)
                    {
                        static const Position l_MovePos(-730.897f, -5596.44f, 25.4994f, 0.540472f);

                        me->GetMotionMaster()->MovePoint(ePoints::PointMoveFirst, l_MovePos);
                        me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 1, 0);
                    }

                    break;
                }

                case 2:
                {
                    if (p_Id == 10)
                    {
                        if (Creature* l_Zarajira = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                            me->AI()->AttackStart(l_Zarajira);

                        /// incr event phase
                    }

                    break;
                }

                case 3:
                {
                    if (p_Id == 5)
                    {
                        me->RemoveAura(eSharedSpells::SpellFreezingTouch);
                    }

                    /// Zuni dies event
                    if (p_Id == 6)
                    {
                        m_Events.SetPhase(ePhases::PhaseTwo);
                        me->SetFlag(UNIT_FIELD_FLAGS, 131970);
                        me->CombatStop();

                        m_Scheduler.Schedule(Milliseconds(1500), [this](TaskContext /*p_Context*/)
                        {
                            me->RemoveAura(eSharedSpells::SpellFreezingTouch);
                            Talk(eTalks::TalkSpeak0);
                            me->SetWalk(false);

                            me->GetMotionMaster()->MovePoint(ePoints::PointMoveThird, { -712.893f, -5578.28f, 25.5029f });
                        });
                    }

                    break;
                }
            }
        }

        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type != POINT_MOTION_TYPE)
                return;

            if (p_Id == ePoints::PointMoveFirst)
            {
                me->SetHomePosition(*me);
            }

            if (p_Id == ePoints::PointMoveThird)
            {
                if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                {
                    l_Zara->AI()->DoAction(eActions::ActionSeaWitchDoTalk2);
                    l_Zara->AI()->SetData(2, 11);
                }

                m_Scheduler.Schedule(Milliseconds(3500), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                        l_Vanira->AI()->DoAction(eActions::ActionVaniraDoTalk1);
                });

                m_Scheduler.Schedule(Milliseconds(4500), [this](TaskContext /*p_Context*/)
                {
                    DoCast(me, eSpells::SpellFrostExplosion);

                    if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        l_Zara->CastSpell(me, eSpells::SpellFrostExplosion, true);
                });

                m_Scheduler.Schedule(Seconds(6), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        me->Kill(l_Zara);

                    me->AddAura(eSpells::SpellPermFeignDeath, me);
                });

                m_Scheduler.Schedule(Milliseconds(7500), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 100.0f))
                        l_VolJin->AI()->SetData(3, 6);

                    if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                    {
                        l_Vanira->RemoveAura(SpellFreezingTouch);
                        l_Vanira->AI()->SetData(2, 9);
                    }
                });

                m_Scheduler.Schedule(Seconds(8), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 50.0f))
                        l_VolJin->AI()->DoAction(eActions::ActionVolJinDoTalk1);

                    if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                        l_Vanira->AI()->SetData(2, 8);
                });

                m_Scheduler.Schedule(Seconds(12), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 50.0f))
                        l_VolJin->AI()->DoAction(eActions::ActionVolJinDoTalk2);
                });

                m_Scheduler.Schedule(Seconds(23), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 50.0f))
                        l_VolJin->AI()->DoAction(eActions::ActionVolJinDoTalk3);
                });

                m_Scheduler.Schedule(Seconds(24), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 50.0f))
                        l_VolJin->AI()->SetData(3, 5);
                });

                m_Scheduler.Schedule(Seconds(28), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                    {
                        l_Vanira->AI()->DoAction(eActions::ActionVaniraDoTalk2);
                        l_Vanira->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 8, 0);
                    }
                });

                m_Scheduler.Schedule(Milliseconds(36500), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                        l_Vanira->AI()->DoAction(eActions::ActionVaniraDoTalk3);
                });

                m_Scheduler.Schedule(Seconds(45), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                    {
                        l_Vanira->AI()->DoAction(eActions::ActionVaniraDoTalk4);
                        l_Vanira->AI()->SetData(2, 7);
                    }
                });

                m_Scheduler.Schedule(Seconds(75), [this](TaskContext /*p_Context*/)
                {
                    if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                        l_Vanira->DespawnOrUnsummon();

                    if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                        l_Zara->DespawnOrUnsummon();

                    if (Creature* l_SeaWitch = me->FindNearestCreature(eNpcs::NpcSeaWitchChannelBunny, 50.0f))
                        l_SeaWitch->DespawnOrUnsummon();

                    me->DespawnOrUnsummon();
                });
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            me->SetWalk(false);

            if (p_Who->GetEntry() == eNpcs::NpcZaraJira)
            {
                m_Events.Reset();

                m_Events.SetPhase(ePhases::PhaseOne);
                m_Events.ScheduleEvent(eSpells::SpellLightningBolt, urand(800, 1000), 0, PhaseOne);
                m_Events.ScheduleEvent(eSpells::SpellEarthShock, urand(100, 500), 0, PhaseOne);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);

            if (!UpdateVictim())
                return;

            m_Events.Update(p_Diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 l_EventId = m_Events.ExecuteEvent())
            {
                switch (l_EventId)
                {
                    case eSpells::SpellLightningBolt:
                        DoCastVictim(eSpells::SpellLightningBolt, true);
                        m_Events.ScheduleEvent(eSpells::SpellLightningBolt, 1700);
                        break;
                    case eSpells::SpellEarthShock:
                        DoCastVictim(eSpells::SpellEarthShock, true);
                        m_Events.ScheduleEvent(eSpells::SpellEarthShock, 8000);
                        break;
                }
            }
        }
};

/// Zara'Jira - 38306
struct npc_zarajira_38306 : public AncientEnemyAI
{
    enum eTalks
    {
        TalkSpeak0,
        TalkSpeak1,
        TalkSpeak2,
        TalkSpeak3,
    };

    enum eSpells
    {
        SpellFrostbolt     = 46987,
        SpellFreezingBurst = 73297,
        SpellFrozenTorment = 72045,
        SeaWitchKillCredit = 73534,
    };

    enum eEvents
    {
        EventUpdateVolJinThreat = 1,
        EventSummonManifestation,
        EventPhaseFourEvents,
    };

    public:
        npc_zarajira_38306(Creature* p_Creature) : AncientEnemyAI(p_Creature), m_Summons(p_Creature), m_DontDie(true), m_ManifestationNumber(0), m_Counter(0), m_AlreadyDidP4(false), m_50PctEvent(false), m_10PctEvent(false)
        {
            me->SetReactState(ReactStates::REACT_DEFENSIVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void JustRespawned() override
        {
            AncientEnemyAI::JustRespawned();

            m_DontDie = true;
            m_Counter = 0;
            m_ManifestationNumber = 0;
            m_AlreadyDidP4 = false;
            m_10PctEvent = false;
            m_50PctEvent = false;
        }

        void Reset() override
        {
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            DoCastAOE(eSpells::SeaWitchKillCredit, true);

            m_Summons.DespawnAll();
        }

        void JustSummoned(Creature* p_Creature) override
        {
            m_Summons.Summon(p_Creature);
        }

        void DoAction(const int32 p_Action) override
        {
            switch (p_Action)
            {
                case eActions::ActionSeaWitchDoTalk0:
                    Talk(eTalks::TalkSpeak0);
                    break;
                case eActions::ActionSeaWitchDoTalk2:
                    Talk(eTalks::TalkSpeak2);
                    break;
            }
        }

        void DamageTaken(Unit* /*p_Attacker*/, uint32 &p_Damage) override
        {
            if (p_Damage >= me->GetHealth() && m_DontDie)
                p_Damage = 0;

            if (me->HealthBelowPctDamaged(51, p_Damage) && !m_50PctEvent)
            {
                p_Damage = 0;
                m_50PctEvent = true;

                if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 30.0f))
                    l_VolJin->AI()->SetData(1, 8);

                DoCast(me, eSpells::SpellFreezingBurst, true);

                static const Position l_FiresOfTheSeaPos[] = 
                {
                    { -711.141f, -5574.35f, 26.0530f },
                    { -706.938f, -5578.52f, 26.0191f },
                    { -700.205f, -5579.72f, 26.0177f },
                };

                for (auto const& l_Pos : l_FiresOfTheSeaPos)
                    me->SummonCreature(eNpcs::NpcFiresOfTheSeas, l_Pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 0);

                DoCast(me, eSpells::SpellFrozenTorment, true);
            }

            if (me->HealthBelowPctDamaged(10, p_Damage) && !m_10PctEvent)
            {
                p_Damage = 0;
                m_10PctEvent = true;

                static const Position l_FiresOfTheSeaPos[] =
                {
                    { -711.141f, -5574.35f, 26.0530f },
                    { -706.938f, -5578.52f, 26.0191f },
                    { -700.205f, -5579.72f, 26.0177f },
                };

                for (auto const& l_Pos : l_FiresOfTheSeaPos)
                    me->SummonCreature(eNpcs::NpcFiresOfTheSeas, l_Pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 0);

                m_Events.SetPhase(ePhases::PhaseFour);
                m_Events.ScheduleEvent(eEvents::EventPhaseFourEvents, 100, 0, ePhases::PhaseFour);
            }
        }

        void SetData(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type == 2)
            {
                if (p_Id == 10)
                {
                    ///m_Events.SetPhase(ePhases::PhaseTwo);
                }
            }

            /// Called from spell click
            if (p_Type == 12)
            {
                if (p_Id == 12)
                {
                    m_Counter++;

                    if (m_Counter >= 3 && !m_AlreadyDidP4)
                    {
                        m_Events.SetPhase(ePhases::PhaseTwo);
                        m_Events.ScheduleEvent(eEvents::EventPhaseFourEvents, 100);

                        m_Events.ScheduleEvent(eEvents::EventSummonManifestation, urand(3000, 4000), 0, ePhases::PhaseOne);
                        m_Events.ScheduleEvent(eEvents::EventSummonManifestation, 11000, 0, ePhases::PhaseOne);
                        m_Events.ScheduleEvent(eEvents::EventSummonManifestation, 18000, 0, ePhases::PhaseOne);
                    }
                }
            }
        }

        void EnterCombat(Unit* /*p_Who*/) override
        {
            m_Events.Reset();
            
            m_DontDie = true;
            m_50PctEvent = false;
            m_ManifestationNumber = 0;

            m_Events.SetPhase(ePhases::PhaseOne);
            m_Events.ScheduleEvent(eSpells::SpellFrostbolt, 0, 0, ePhases::PhaseOne);
            m_Events.ScheduleEvent(eEvents::EventUpdateVolJinThreat, 500, 0, ePhases::PhaseOne);
            m_Events.ScheduleEvent(eEvents::EventSummonManifestation, urand(3000, 4000), 0, ePhases::PhaseOne);
            m_Events.ScheduleEvent(eEvents::EventSummonManifestation, 11000, 0, ePhases::PhaseOne);
            m_Events.ScheduleEvent(eEvents::EventSummonManifestation, 18000, 0, ePhases::PhaseOne);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            m_Scheduler.Update(p_Diff);

            if (!UpdateVictim())
                return;

            m_Events.Update(p_Diff);

            if (uint32 l_EventId = m_Events.ExecuteEvent())
            {
                switch (l_EventId)
                {
                    case eSpells::SpellFrostbolt:

                        if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 30.0f))
                            DoCast(l_VolJin, eSpells::SpellFrostbolt, true);

                        m_Events.ScheduleEvent(eSpells::SpellFrostbolt, 2500, 0, ePhases::PhaseOne);
                        break;

                    case eEvents::EventUpdateVolJinThreat:

                        if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 30.0f))
                            me->AddThreat(l_VolJin, 400.0f);

                        m_Events.ScheduleEvent(eEvents::EventUpdateVolJinThreat, 4000, 0, ePhases::PhaseOne);

                        break;

                    case eEvents::EventSummonManifestation:
                    {
                        static const Position l_ManifestationSpawnPos[] = 
                        {
                            { -691.657f, -5573.86f, 25.476f, 3.67f },
                            { -710.433f, -5564.67f, 26.104f, 4.61f },
                            { -732.502f, -5568.09f, 25.510f, 5.139f },
                        };

                        if (m_ManifestationNumber < 3)
                            me->SummonCreature(eNpcs::NpcManifestationOfTheSeaWitch, l_ManifestationSpawnPos[m_ManifestationNumber++], TEMPSUMMON_CORPSE_TIMED_DESPAWN , 8000);

                        break;
                    }

                    case eEvents::EventPhaseFourEvents:
                    {
                        Talk(eTalks::TalkSpeak1);

                        me->InterruptNonMeleeSpells(false, SpellFrozenTorment, true);

                        if (Creature* l_ChannelBunny = me->FindNearestCreature(eNpcs::NpcSeaWitchChannelBunny, 10.0f))
                            l_ChannelBunny->DespawnOrUnsummon();

                        /// Remove all the auras
                        me->RemoveAura(eSharedSpells::SpellFreezingTouch);

                        if (Creature* l_Vanira = me->FindNearestCreature(eNpcs::NpcVanira, 50.0f))
                            l_Vanira->RemoveAura(SpellFreezingTouch);

                        if (Creature* l_Zuni = me->FindNearestCreature(eNpcs::NpcZuni, 50.0f))
                            l_Zuni->RemoveAura(SpellFreezingTouch);

                        if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 30.0f))
                            l_VolJin->AI()->SetData(1, 10);

                        m_Scheduler.Schedule(Seconds(10), [this](TaskContext /*p_Context*/)
                        {
                            DoCast(me, eSpells::SpellFreezingBurst, true);

                            me->SummonCreature(eNpcs::NpcSeaWitchChannelBunny, *me, TEMPSUMMON_MANUAL_DESPAWN);

                            DoCast(me, eSpells::SpellFrozenTorment, true);

                            m_Events.SetPhase(ePhases::PhaseOne);

                            if (Creature* l_VolJin = me->FindNearestCreature(eNpcs::NpcVolJin, 30.0f))
                                l_VolJin->AI()->SetData(1, m_AlreadyDidP4 ? 10 : 9);
                        });

                        if (m_AlreadyDidP4)
                        {
                            /// Kill Zuni event
                            m_Scheduler.Schedule(Seconds(12), [this](TaskContext /*p_Context*/)
                            {
                                if (Creature* l_Zuni = me->FindNearestCreature(eNpcs::NpcZuni, 30.0f))
                                    l_Zuni->AI()->SetData(3, 6);
                            });
                        }

                        /// For next time
                        m_AlreadyDidP4 = true;

                        break;
                    }
                }
            }
        }

    private:
        SummonList m_Summons;
        bool m_DontDie;
        bool m_50PctEvent;
        bool m_10PctEvent;
        bool m_AlreadyDidP4;
        uint32 m_ManifestationNumber;
        uint32 m_Counter;
};

/// Fire of the Seas - 38542
struct npc_fire_of_the_seas_38542 : public ScriptedAI
{
    enum eSpells
    {
        SpellFireOfTheSeasEnergyBeam = 73294,
        SpellFireOfTheSeas = 72250,
    };

    public:
        npc_fire_of_the_seas_38542(Creature* p_Creature) : ScriptedAI(p_Creature), m_Lock(false) { }

        void IsSummonedBy(Unit* /*p_Summoner*/) override
        {
            me->AddAura(SpellFireOfTheSeas, me);

            if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                DoCast(l_Zara, SpellFireOfTheSeasEnergyBeam, true);
        }

        void OnSpellClick(Unit* /*p_Clicker*/, bool& /*p_Result*/) override
        {
            if (m_Lock)
                return;

            m_Lock = true;

            me->InterruptNonMeleeSpells(false, SpellFireOfTheSeasEnergyBeam);

            if (Creature* l_Zara = me->FindNearestCreature(eNpcs::NpcZaraJira, 50.0f))
                l_Zara->AI()->SetData(12, 12);

            me->DespawnOrUnsummon();
        }

    private:
        bool m_Lock;

};

void AddSC_echo_isles()
{
    /// UPDATED
    new npc_darkspear_jailor();                                   ///< (39062) 5-11-19 by Artamedes
    RegisterCreatureAI(npc_captive_spitescale_scout);             ///< (38142) 5-11-19 by Artamedes
    RegisterCreatureAI(npc_bloodtalon_thrasher);                  ///< (38991) 5-12-19 by Artamedes
    RegisterCreatureAI(npc_zuni_37988);                           ///< (37988) 5-14-19 by Artamedes
    RegisterCreatureAI(npc_zuni_38930);                           ///< (38930) 5-14-19 by Artamedes
    RegisterCreatureAI(npc_swiftclaw_37989);                      ///< (37989) 5-15-19 by Artamedes
    RegisterCreatureAI(npc_swiftclaw_38002);                      ///< (38002) 5-15-19 by Artamedes
    new item_territorial_fetish();
    RegisterCreatureAI(npc_spitescale_flag_bunny);                ///< (38560) 5-17-19 by Artamedes

    /// An Ancient Enemy
    RegisterCreatureAI(npc_voljin_38225);           ///< (38225) 5-15-19 by Artamedes
    RegisterCreatureAI(npc_vanira_38437);           ///< (38437) 5-15-19 by Artamedes
    RegisterCreatureAI(npc_zuni_38423);             ///< (38423) 5-15-19 by Artamedes
    RegisterCreatureAI(npc_zarajira_38306);         ///< (38306) 5-15-19 by Artamedes
    RegisterCreatureAI(npc_fire_of_the_seas_38542); ///< (38542) 5-15-19 by Artamedes
}