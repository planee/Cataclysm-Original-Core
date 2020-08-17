//////////////////////////////////////////////////////////////////////////
///
///  PROJECT AZENOR
/// COPYRIGHT 2018-2019
/// ALL RIGHTS RESERVED.
///
//////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_SMARTSCRIPT_H
#define TRINITY_SMARTSCRIPT_H

/// Common
#include "Common.h"

/// Shared
#include "ByteBuffer.h"

/// Game
#include "SmartScriptMgr.h"

/// Forward Declarations

/// Structures
struct AreaTriggerEntry;

/// Classes
class Creature;
class GameObject;
class Player;
class Quest;
class Spell;
class Object;
class Unit;

class SmartScript
{
    public:
        /// Constructors
        SmartScript();

        /// SMART_SCRIPT_TYPE_CREATURE
        SmartScript(Creature* p_Creature);

        /// SMART_SCRIPT_TYPE_GAMEOBJECT
        SmartScript(GameObject* p_GameObject);

        /// SMART_SCRIPT_TYPE_AREATRIGGER
        SmartScript(AreaTriggerEntry const* p_AreaTriggerEntry, Player* p_Player);

        /// SMART_SCRIPT_TYPE_QUEST
        SmartScript(Quest const* p_Quest, Player* p_Player);

        /// SMART_SCRIPT_TYPE_EVENT
        /// SMART_SCRIPT_TYPE_SPELL
        /// SMART_SCRIPT_TYPE_WAYPOINT
        /// @p_Type - Type fo SmartScript
        /// @p_EntryId - EventID/SpellID/WaypointID
        /// @p_Source - Object who called this event/spell/waypoint
        /// @p_Target - hit target
        /// @p_EffIndex - Used only from spells, effIndex was triggered from
        SmartScript(SmartScriptType p_Type, uint32 p_EntryId, Object* p_Source, Object* p_Target, uint32 p_EffIndex = 0);

        /// Deconstructor
        ~SmartScript();

        ////////////////////////////////////////////////////////////////////
        /// Script Creations
        void OnInitialize();
        void GetScript();
        void FillScript(SmartAIEventList p_EventList);

        ////////////////////////////////////////////////////////////////////
        /// Process (events, actions, and timed actions)
        void ProcessEventsFor(SMART_EVENT e, Unit* unit = nullptr, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, const SpellInfo* spell = nullptr, GameObject* p_GameObject = nullptr);
        void ProcessEvent(SmartScriptHolder& e, Unit* unit = nullptr, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, const SpellInfo* spell = nullptr, GameObject* p_GameObject = nullptr);
        void ProcessAction(SmartScriptHolder& e, Unit* unit = nullptr, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, const SpellInfo* spell = nullptr, GameObject* p_GameObject = nullptr);
        void ProcessTimedAction(SmartScriptHolder& e, uint32 const& min, uint32 const& max, Unit* unit = nullptr, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, const SpellInfo* spell = nullptr, GameObject* p_GameObject = nullptr);

        ////////////////////////////////////////////////////////////////////
        /// Timers
        bool CheckTimer(SmartScriptHolder const& e) const;
        void UpdateTimer(SmartScriptHolder& e, uint32 const diff);
        static void RecalcTimer(SmartScriptHolder& e, uint32 min, uint32 max);
        static void InitTimer(SmartScriptHolder& e);

        ////////////////////////////////////////////////////////////////////
        /// Targets
        void GetTargets(ObjectVector& targets, SmartScriptHolder const& e, Unit* invoker = nullptr) const;
        void GetWorldObjectsInDist(ObjectVector& objects, float dist) const;

        ////////////////////////////////////////////////////////////////////
        void InstallTemplate(SmartScriptHolder const& e);
        static SmartScriptHolder CreateSmartEvent(SMART_EVENT e, uint32 event_flags, uint32 event_param1, uint32 event_param2, uint32 event_param3, uint32 event_param4, uint32 event_param5, SMART_ACTION action, uint32 action_param1, uint32 action_param2, uint32 action_param3, uint32 action_param4, uint32 action_param5, uint32 action_param6, SMARTAI_TARGETS t, uint32 target_param1, uint32 target_param2, uint32 target_param3, uint32 phaseMask = 0);
        void AddEvent(SMART_EVENT e, uint32 event_flags, uint32 event_param1, uint32 event_param2, uint32 event_param3, uint32 event_param4, uint32 event_param5, SMART_ACTION action, uint32 action_param1, uint32 action_param2, uint32 action_param3, uint32 action_param4, uint32 action_param5, uint32 action_param6, SMARTAI_TARGETS t, uint32 target_param1, uint32 target_param2, uint32 target_param3, uint32 phaseMask = 0);
        
        ////////////////////////////////////////////////////////////////////
        /// Waypoints
        void SetPathId(uint32 id) { mPathId = id; }
        uint32 GetPathId() const { return mPathId; }

        WorldObject* GetBaseObject() const;

        static bool IsUnit(WorldObject* p_Object);
        static bool IsPlayer(WorldObject* p_Object);
        static bool IsCreature(WorldObject* p_Object);
        static bool IsCharmedCreature(WorldObject* p_Object);
        static bool IsGameObject(WorldObject* p_Object);

        void OnUpdate(const uint32 diff);
        void OnMoveInLineOfSight(Unit* who);

        Unit* DoSelectLowestHpFriendly(float range, uint32 MinHPDiff) const;
        void DoFindFriendlyCC(std::vector<Creature*>& creatures, float range) const;
        void DoFindFriendlyMissingBuff(std::vector<Creature*>& creatures, float range, uint32 spellid) const;
        Unit* DoFindClosestFriendlyInRange(float range, bool playerOnly) const;

        bool IsSmartAI(Creature* c, bool silent = false);
        bool IsSmartAI(GameObject* g, bool silent = false);
        bool IsSmartAI(bool silent = false);

        void StoreTargetVector(ObjectVector const& targets, uint32 id);
        ObjectVector const* GetStoredTargetVector(uint32 id, WorldObject const& ref) const;

        void StoreCounter(uint32 id, uint32 value, uint32 reset);
        uint32 GetCounterValue(uint32 id) const;

        GameObject* FindGameObjectNear(WorldObject* searchObject, uint32 guid) const;
        Creature* FindCreatureNear(WorldObject* searchObject, uint32 guid) const;

        void OnReset();
        void ResetBaseObject();

        //TIMED_ACTIONLIST (script type 9 aka script9)
        void SetTimedActionList(SmartScriptHolder& e, uint32 entry);
        Unit* GetLastInvoker(Unit* invoker = nullptr) const;
        uint64 mLastInvoker;
        typedef std::unordered_map<uint32, uint32> CounterMap;
        CounterMap mCounterList;
        std::vector<uint64> m_SpellHitTargets;

        uint32 GetEndTime() const
        {
            return m_EndTime;
        }

        uint32 m_TimePassed;

    private:
        void IncPhase(int32 p = 1)
        {
            if (p >= 0)
                mEventPhase += (uint32)p;
            else
                DecPhase(abs(p));
        }

        void DecPhase(int32 p = 1) { mEventPhase  -= (mEventPhase < (uint32)p ? (uint32)p - mEventPhase : (uint32)p); }
        bool IsInPhase(uint32 p) const { return (1 << (mEventPhase - 1)) & p; }
        void SetPhase(uint32 p = 0) { mEventPhase = p; }

        SmartAIEventList mEvents;
        SmartAIEventList mInstallEvents;
        SmartAIEventList mTimedActionList;
        bool isProcessingTimedActionList;

        /// Objects
        Unit                  * me;                 ///<    Used by lots of stuff
        GameObject            * go;                 ///<  1 SMART_SCRIPT_TYPE_GAMEOBJECT
        AreaTriggerEntry const* trigger;            ///<  2 SMART_SCRIPT_TYPE_AREATRIGGER
        uint32                  m_QuestId;          ///<  5 SMART_SCRIPT_TYPE_QUEST

        uint32                  m_EntryId;          ///< old *_scripts Merge to SAI
        Object                * m_Source;
        Object                * m_Target;
        
        /// uint64s
        uint64 m_CreatureOrginalGUID;           ///<  0 SMART_SCRIPT_TYPE_CREATURE
        uint64 m_GameObjectOriginalGUID;        ///<  1 SMART_SCRIPT_TYPE_GAMEOBJECT
        uint64 m_PlayerOrignalGUID;             ///<  5 SMART_SCRIPT_TYPE_QUEST

        SmartScriptType mScriptType;
        uint32 mEventPhase;

        std::unordered_map<int32, int32> mStoredDecimals;
        uint32 mPathId;
        SmartAIEventList mStoredEvents;
        std::list<uint32>mRemIDs;

        uint32 mTextTimer;
        uint32 mLastTextID;
        uint64 mTextGUID;
        uint32 mTalkerEntry;
        bool mUseTextTimer;

        ObjectVectorMap _storedTargets;

        SMARTAI_TEMPLATE mTemplate;
        void InstallEvents();

        void RemoveStoredEvent(uint32 id);

        SmartScriptHolder FindLinkedEvent(uint32 link);

        uint32 m_EndTime;
        uint32 m_EffIndex;

        bool m_ShouldPreventEventPhaseReset;
};

#endif
