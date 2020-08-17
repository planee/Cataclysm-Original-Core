//////////////////////////////////////////////////////////////////////////
///
///  PROJECT AZENOR
/// COPYRIGHT 2018-2019
/// ALL RIGHTS RESERVED.
///
//////////////////////////////////////////////////////////////////////////

/// Common
#include "Common.h"

/// Game
#include "Cell.h"
#include "CellImpl.h"
#include "Creature.h"
#include "CreatureTextMgr.h"
#include "GossipDef.h"
#include "GridDefines.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "InstanceScript.h"
#include "Language.h"
#include "MoveSplineInit.h"
#include "ObjectDefines.h"
#include "ObjectMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SmartAI.h"
#include "SmartScript.h"
#include "SpellMgr.h"
#include "QuestDef.h"
#include "Transport.h"
#include "PlayerTextMgr.h"
#include "Vehicle.h"
#include "Movement/Waypoints/CustomWaypointMgr.h"
/// C++
#include <algorithm>
#include <array>

SmartScript::SmartScript()
{
    go                              = nullptr;
    me                              = nullptr;
    trigger                         = nullptr;
    m_Target                        = nullptr;
    mEventPhase                     = 0;
    mPathId                         = 0;
    mTextTimer                      = 0;
    mLastTextID                     = 0;
    mUseTextTimer                   = false;
    mTalkerEntry                    = 0;
    mTemplate                       = SMARTAI_TEMPLATE_BASIC;
    mScriptType                     = SMART_SCRIPT_TYPE_CREATURE;
    isProcessingTimedActionList     = false;
    m_QuestId                       = 0;
    m_EntryId                       = 0;
    m_EndTime                       = 0;
    m_TimePassed                    = 0;
    m_EffIndex                      = 0;
    m_ShouldPreventEventPhaseReset  = false;

    /// DONT MERGE INTO MOP
    mLastInvoker                    = 0;
    m_CreatureOrginalGUID           = 0;
    m_GameObjectOriginalGUID        = 0;
    m_PlayerOrignalGUID             = 0;
    mTextGUID                       = 0;
}

SmartScript::SmartScript(Creature* p_Creature) : SmartScript()
{
    m_Source = p_Creature;
    me = p_Creature;
    mScriptType = SMART_SCRIPT_TYPE_CREATURE;

    OnInitialize();
}

SmartScript::SmartScript(GameObject* p_GameObject) : SmartScript()
{
    m_Source = p_GameObject;
    go = p_GameObject;
    mScriptType = SMART_SCRIPT_TYPE_GAMEOBJECT;

    OnInitialize();
}

SmartScript::SmartScript(AreaTriggerEntry const* p_AreaTriggerEntry, Player* p_Player) : SmartScript()
{
    trigger = p_AreaTriggerEntry;
    m_Source = p_Player;
    me = p_Player;
    mScriptType = SMART_SCRIPT_TYPE_AREATRIGGER;

    OnInitialize();
}

SmartScript::SmartScript(Quest const* p_Quest, Player* p_Player) : SmartScript()
{
    m_QuestId = p_Quest->GetQuestId();
    m_Source = p_Player;
    me = p_Player;
    mScriptType = SMART_SCRIPT_TYPE_QUEST;

    OnInitialize();
}

SmartScript::SmartScript(SmartScriptType p_Type, uint32 p_EntryId, Object* p_Source, Object* p_Target, uint32 p_EffIndex) : SmartScript()
{
    mScriptType = p_Type;
    m_EntryId = p_EntryId;

    if (p_Source)
    {
        m_Source = p_Source;
        me = p_Source->ToUnit();
        go = p_Source->ToGameObject();
    }

    if(p_Target)
        m_Target = p_Target;

    m_EffIndex = p_EffIndex;

    OnInitialize();
}

SmartScript::~SmartScript()
{
}

// @todo this is an utter clusterfuck in terms of design - why in the world does this thing side effect?
// seriously, WHO WRITES THIS SHIT
bool SmartScript::IsSmartAI(Creature* p_Creature, bool silent)
{
    if (!p_Creature)
        return false;

    bool smart = true;
    if (!dynamic_cast<SmartAI*>(p_Creature->AI()))
        smart = false;

    return smart;
}

// @todo this, too
bool SmartScript::IsSmartAI(GameObject* g, bool silent)
{
    if (!g)
        return false;

    bool smart = true;
    if (!dynamic_cast<SmartGameObjectAI*>(g->AI()))
        smart = false;

    return smart;
}

bool SmartScript::IsSmartAI(bool silent)
{
    if (me && me->IsCreature())
        return IsSmartAI(me->ToCreature(), silent);
    if (go)
        return IsSmartAI(go, silent);

    return false;
}

void SmartScript::StoreTargetVector(ObjectVector const& targets, uint32 id)
{
    // insert or replace
    _storedTargets.erase(id);
    _storedTargets.emplace(id, ObjectGuidVector(targets));
}

ObjectVector const* SmartScript::GetStoredTargetVector(uint32 id, WorldObject const& ref) const
{
    auto itr = _storedTargets.find(id);
    if (itr != _storedTargets.end())
        return itr->second.GetObjectVector(ref);
    return nullptr;
}

void SmartScript::StoreCounter(uint32 id, uint32 value, uint32 reset)
{
    CounterMap::iterator itr = mCounterList.find(id);
    if (itr != mCounterList.end())
    {
        if (reset == 0)
            itr->second += value;
        else
            itr->second = value;
    }
    else
        mCounterList.insert(std::make_pair(id, value));

    ProcessEventsFor(SMART_EVENT_COUNTER_SET, nullptr, id);
}

uint32 SmartScript::GetCounterValue(uint32 id) const
{
    CounterMap::const_iterator itr = mCounterList.find(id);
    if (itr != mCounterList.end())
        return itr->second;
    return 0;
}

GameObject* SmartScript::FindGameObjectNear(WorldObject* searchObject, uint32 guid) const
{
    GameObject* gameObject = nullptr;

    CellCoord p(Trinity::ComputeCellCoord(searchObject->GetPositionX(), searchObject->GetPositionY()));
    Cell cell(p);

    Trinity::GameObjectWithDbGUIDCheck goCheck(*searchObject, guid);
    Trinity::GameObjectSearcher<Trinity::GameObjectWithDbGUIDCheck> checker(searchObject, gameObject, goCheck);

    TypeContainerVisitor<Trinity::GameObjectSearcher<Trinity::GameObjectWithDbGUIDCheck>, GridTypeMapContainer > objectChecker(checker);
    cell.Visit(p, objectChecker, *searchObject->GetMap(), *searchObject, searchObject->GetGridActivationRange());

    return gameObject;
}

Creature* SmartScript::FindCreatureNear(WorldObject* searchObject, uint32 guid) const
{
    Creature* creature = nullptr;
    CellCoord p(Trinity::ComputeCellCoord(searchObject->GetPositionX(), searchObject->GetPositionY()));
    Cell cell(p);

    Trinity::CreatureWithDbGUIDCheck target_check(searchObject, guid);
    Trinity::CreatureSearcher<Trinity::CreatureWithDbGUIDCheck> checker(searchObject, creature, target_check);

    TypeContainerVisitor<Trinity::CreatureSearcher <Trinity::CreatureWithDbGUIDCheck>, GridTypeMapContainer > unit_checker(checker);
    cell.Visit(p, unit_checker, *searchObject->GetMap(), *searchObject, searchObject->GetGridActivationRange());

    return creature;
}

void SmartScript::OnReset()
{
    /// Reset our base object, this function will clear originalGUID and set our me and go pointers back
    ResetBaseObject();

    /// Iterate mEvents
    for (auto &l_Event : mEvents)
    {
        /// If we don't have event_flag SMART_EVENT_FLAG_DONT_RESET
        if (!(l_Event.m_Event.event_flags & SMART_EVENT_FLAG_DONT_RESET))
        {
            /// Recalculate our timers
            InitTimer(l_Event);
            
            /// Since we reset, we can run again
            l_Event.runOnce = false;
        }
    }

    /// Process any events for SMART_EVENT_RESET (if they exist)
    ProcessEventsFor(SMART_EVENT_RESET);

    /// Clear lastInvokerGUID and counterList
    mLastInvoker = 0;
    mCounterList.clear();
}

void SmartScript::ResetBaseObject()
{
    WorldObject* lookupRoot = me;

    if (!lookupRoot)
        lookupRoot = go;

    if (lookupRoot)
    {
        if (m_CreatureOrginalGUID == 0)
            if (Unit* l_Unit = ObjectAccessor::GetUnit(*lookupRoot, m_CreatureOrginalGUID))
                me = l_Unit;

        if (m_GameObjectOriginalGUID == 0)
            if (GameObject* l_GameObject = ObjectAccessor::GetGameObject(*lookupRoot, m_GameObjectOriginalGUID))
                go = l_GameObject;
    }

    m_GameObjectOriginalGUID = 0;
    m_CreatureOrginalGUID = 0;
}

void SmartScript::ProcessEventsFor(SMART_EVENT p_SmartEvent, Unit* p_Unit, uint32 p_Var0, uint32 p_Var1, bool p_BoolVar, const SpellInfo* p_SpellInfo, GameObject* p_GameObject)
{
    /// Iterate through all our events
    for (auto& l_Event : mEvents)
    {
        /// Create a variable of our EventType because we use it in more than 1 spot.
        uint32 l_EventType = l_Event.GetEventType();
        if (l_EventType == SMART_EVENT_LINK)            ///< Special handling for SMART_EVENT_LINK (done internally)
            continue;

        /// Check if our EventType is this smart event
        if (l_EventType == p_SmartEvent)
        {
            /// Check if the ConditionMgr allows us to complete this event
            ConditionList l_Conds = sConditionMgr->GetConditionsForSmartEvent(l_Event.entryOrGuid, l_Event.event_id, l_Event.source_type);
            ConditionSourceInfo l_Info = ConditionSourceInfo(p_Unit, GetBaseObject());

            if (sConditionMgr->IsObjectMeetToConditions(l_Info, l_Conds))
            {
                /// Pass our reference of l_Event and rest of our args
                ProcessEvent(l_Event, p_Unit, p_Var0, p_Var1, p_BoolVar, p_SpellInfo, p_GameObject);
            }
        }
    }
}

void SmartScript::ProcessAction(SmartScriptHolder& p_ScriptHolder, Unit* unit, uint32 var0, uint32 var1, bool bvar, const SpellInfo* p_SpellInfo, GameObject* p_GameObject)
{
    // calc random
    if (p_ScriptHolder.GetEventType() != SMART_EVENT_LINK && p_ScriptHolder.m_Event.event_chance < 100 && p_ScriptHolder.m_Event.event_chance)
    {
        uint32 rnd = urand(1, 100);
        if (p_ScriptHolder.m_Event.event_chance <= rnd)
            return;
    }
    p_ScriptHolder.runOnce = true;//used for repeat check

    if (unit)
        mLastInvoker = unit->GetGUID();

    ObjectVector targets;
    GetTargets(targets, p_ScriptHolder, unit);

    switch (p_ScriptHolder.GetActionType())
    {
        case SMART_ACTION_TALK:
        {
            Unit* talker = p_ScriptHolder.target.type == 0 ? me : nullptr;
            Unit* talkTarget = nullptr;

            for (WorldObject* target : targets)
            {
                if (IsCreature(target) && !target->ToCreature()->isPet()) // Prevented sending text to pets.
                {
                    if (p_ScriptHolder.action.talk.useTalkTarget)
                    {
                        talker = me;
                        talkTarget = target->ToCreature();
                    }
                    else
                        talker = target->ToCreature();

                    break;
                }
                else if (IsPlayer(target))
                {
                    talker = me;
                    talkTarget = target->ToPlayer();
                    break;
                }
            }

            if (!talkTarget)
                talkTarget = GetLastInvoker();

            if (!talker)
                break;

            mTalkerEntry = talker->GetEntry();
            mLastTextID = p_ScriptHolder.action.talk.textGroupID;
            mTextTimer = p_ScriptHolder.action.talk.duration;
            mUseTextTimer = true;
            if (talker && talker->ToCreature())
                sCreatureTextMgr->SendChat(talker->ToCreature(), uint8(p_ScriptHolder.action.talk.textGroupID), talkTarget ? talkTarget->GetGUID() : 0);
            break;
        }

        case SMART_ACTION_SIMPLE_TALK:
        {
            for (WorldObject* target : targets)
            {
                if (IsCreature(target))
                    sCreatureTextMgr->SendChat(target->ToCreature(), uint8(p_ScriptHolder.action.talk.textGroupID), IsPlayer(GetLastInvoker()) ? GetLastInvoker()->GetGUID() : 0);
                else if (IsPlayer(target) && me)
                {
                    Unit* templastInvoker = GetLastInvoker();
                    ///@todo - SendChat should pass unit as param for optimization, rather than it having to access ObjectAccessor
                    if(me->ToCreature())
                        sCreatureTextMgr->SendChat(me->ToCreature(), uint8(p_ScriptHolder.action.talk.textGroupID), IsPlayer(templastInvoker) ? templastInvoker->GetGUID() : 0, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_NORMAL, 0, TEAM_OTHER, false, target->ToPlayer());
                }
            }
            break;
        }

        case SMART_ACTION_PLAY_EMOTE:
        {
            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    target->ToUnit()->HandleEmoteCommand(p_ScriptHolder.action.emote.emote);
                }
            }
            break;
        }

        case SMART_ACTION_SOUND:
        {
            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    if (p_ScriptHolder.action.sound.distance == 1)
                        target->PlayDistanceSound(p_ScriptHolder.action.sound.sound, p_ScriptHolder.action.sound.onlySelf ? target->ToPlayer() : nullptr);
                    else
                        target->PlayDirectSound(p_ScriptHolder.action.sound.sound, p_ScriptHolder.action.sound.onlySelf ? target->ToPlayer() : nullptr);

                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_SOUND: target: %s (GuidLow: %u), sound: %u, onlyself: %u, distance: %u",
                        target->GetName().c_str(), target->GetGUIDLow(), p_ScriptHolder.action.sound.sound, p_ScriptHolder.action.sound.onlySelf, p_ScriptHolder.action.sound.distance);
                }
            }
            break;
        }

        case SMART_ACTION_SET_FACTION:
        {
            for (WorldObject* target : targets)
            {
                if (IsCreature(target))
                {
                    if (p_ScriptHolder.action.faction.factionID)
                    {
                        target->ToCreature()->setFaction(p_ScriptHolder.action.faction.factionID);
                        TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_SET_FACTION: Creature entry %u, GuidLow %u set faction to %u",
                            target->GetEntry(), target->GetGUIDLow(), p_ScriptHolder.action.faction.factionID);
                    }
                    else
                    {
                        if (CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(target->ToCreature()->GetEntry()))
                        {
                            if (target->ToCreature()->getFaction() != ci->faction_A)
                            {
                                target->ToCreature()->setFaction(ci->faction_A);
                            }
                        }
                    }
                }
            }
            break;
        }

        case SMART_ACTION_MORPH_TO_ENTRY_OR_MODEL:
        {
            for (WorldObject* target : targets)
            {
                if (!IsCreature(target))
                    continue;

                if (p_ScriptHolder.action.morphOrMount.creature || p_ScriptHolder.action.morphOrMount.model)
                {
                    //set model based on entry from creature_template
                    if (p_ScriptHolder.action.morphOrMount.creature)
                    {
                        if (CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(p_ScriptHolder.action.morphOrMount.creature))
                        {
                            uint32 displayId = ObjectMgr::ChooseDisplayId(0, ci);
                            target->ToCreature()->SetDisplayId(displayId);
                            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_MORPH_TO_ENTRY_OR_MODEL: Creature entry %u, GuidLow %u set displayid to %u",
                                target->GetEntry(), target->GetGUIDLow(), displayId);
                        }
                    }
                    //if no param1, then use value from param2 (modelId)
                    else
                    {
                        target->ToCreature()->SetDisplayId(p_ScriptHolder.action.morphOrMount.model);
                        TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_MORPH_TO_ENTRY_OR_MODEL: Creature entry %u, GuidLow %u set displayid to %u",
                            target->GetEntry(), target->GetGUIDLow(), p_ScriptHolder.action.morphOrMount.model);
                    }
                }
                else
                {
                    target->ToCreature()->DeMorph();
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_MORPH_TO_ENTRY_OR_MODEL: Creature entry %u, GuidLow %u demorphs.",
                        target->GetEntry(), target->GetGUIDLow());
                }
            }
            break;
        }

        case SMART_ACTION_FAIL_QUEST:
        {
            for (WorldObject* target : targets)
            {
                if (IsPlayer(target))
                {
                    target->ToPlayer()->FailQuest(p_ScriptHolder.action.quest.quest);
                }
            }
            break;
        }

        case SMART_ACTION_ADD_QUEST:
        {
            uint32 l_QuestId = p_ScriptHolder.action.AddQuest.QuestID;
            bool l_CheckRequirements = p_ScriptHolder.action.AddQuest.CheckRequirements;
            bool l_ShowPopup = p_ScriptHolder.action.AddQuest.ShowPopup;

            Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_QuestId);

            if (!l_Quest)
            {
                TC_LOG_ERROR("smartscript", "SMART_ACTION_ADD_QUEST: Invalid QuestID: %u. entryorguid: %d", l_QuestId, p_ScriptHolder.entryOrGuid);
                break;
            }

            for (WorldObject* l_Target : targets)
            {
                Player* l_Player = l_Target->ToPlayer();
                if (!l_Player)
                    continue;

                if (l_CheckRequirements)
                {
                    if (!l_Player->CanAddQuest(l_Quest, true))
                        continue;
                }

                if (l_ShowPopup)
                    l_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, GetBaseObject()->GetGUID(), false, true);

                l_Player->AddQuestAndCheckCompletion(l_Quest, GetBaseObject());
            }

            TC_LOG_DEBUG("smartscript", "SMART_ACTION_ADD_QUEST: QuestID: %u CheckRequirements: %u ShowPopup: %u", l_QuestId, l_CheckRequirements ? 1 : 0, l_ShowPopup ? 1 : 0);
            break;
        }

        case SMART_ACTION_SET_REACT_STATE:
        {
            for (WorldObject* target : targets)
            {
                if (!IsCreature(target))
                    continue;

                target->ToCreature()->SetReactState(ReactStates(p_ScriptHolder.action.react.state));
            }
            break;
        }

        case SMART_ACTION_RANDOM_EMOTE:
        {
            std::vector<uint32> emotes;
            std::copy_if(std::begin(p_ScriptHolder.action.randomEmote.emotes), std::end(p_ScriptHolder.action.randomEmote.emotes),
                std::back_inserter(emotes), [](uint32 emote) { return emote != 0; });

            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    uint32 emote = Trinity::Containers::SelectRandomContainerElement(emotes);
                    target->ToUnit()->HandleEmoteCommand(emote);
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_RANDOM_EMOTE: Creature guidLow %u handle random emote %u",
                        target->GetGUIDLow(), emote);
                }
            }
            break;
        }

        case SMART_ACTION_THREAT_ALL_PCT:
        {
            if (!me)
                break;

            std::list<HostileReference*> const& threatList = me->getThreatManager().getThreatList();
            for (std::list<HostileReference*>::const_iterator i = threatList.begin(); i != threatList.end(); ++i)
            {
                if (Unit* target = Unit::GetUnit(*me, (*i)->getUnitGuid()))
                {
                    me->getThreatManager().modifyThreatPercent(target, p_ScriptHolder.action.threatPCT.threatINC ? (int32)p_ScriptHolder.action.threatPCT.threatINC : -(int32)p_ScriptHolder.action.threatPCT.threatDEC);
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_THREAT_ALL_PCT: Creature guidLow %u modify threat for unit %u, value %i",
                        me->GetGUIDLow(), target->GetGUIDLow(), p_ScriptHolder.action.threatPCT.threatINC ? (int32)p_ScriptHolder.action.threatPCT.threatINC : -(int32)p_ScriptHolder.action.threatPCT.threatDEC);
                }
            }
            break;
        }

        case SMART_ACTION_THREAT_SINGLE_PCT:
        {
            if (!me)
                break;

            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    me->getThreatManager().modifyThreatPercent(target->ToUnit(), std::max<int32>(-100, int32(p_ScriptHolder.action.threatPCT.threatINC) - int32(p_ScriptHolder.action.threatPCT.threatDEC)));
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_THREAT_SINGLE_PCT: Creature guidLow %u modify threat for unit %u, value %i",
                        me->GetGUIDLow(), target->GetGUIDLow(), int32(p_ScriptHolder.action.threatPCT.threatINC) - int32(p_ScriptHolder.action.threatPCT.threatDEC));
                }
            }
            break;
        }

        case SMART_ACTION_CALL_AREAEXPLOREDOREVENTHAPPENS:
        {
            for (WorldObject* target : targets)
            {
                // Special handling for vehicles
                if (IsUnit(target))
                    if (Vehicle* vehicle = target->ToUnit()->GetVehicleKit())
                        for (SeatMap::iterator it = vehicle->Seats.begin(); it != vehicle->Seats.end(); ++it)
                            if (Player* player = ObjectAccessor::GetPlayer(*target, it->second.Passenger.Guid))
                                player->AreaExploredOrEventHappens(p_ScriptHolder.action.quest.quest);

                if (IsPlayer(target))
                {
                    target->ToPlayer()->AreaExploredOrEventHappens(p_ScriptHolder.action.quest.quest);

                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_CALL_AREAEXPLOREDOREVENTHAPPENS: Player guidLow %u credited quest %u",
                        target->GetGUIDLow(), p_ScriptHolder.action.quest.quest);
                }
            }
            break;
        }

        /// TODO THIS IS HACK
        /// MoP has TC phasing from master branch. Cata still uses old system
        /// Eventually integrate it.. ?
        case SMART_ACTION_SET_INGAME_PHASE_ID:
        {
            for (WorldObject* target : targets)
            {
                if (GameObject* l_GameObject = target->ToGameObject())
                    l_GameObject->SetPhaseMask(p_ScriptHolder.action.ingamePhaseId.id, true);
                else if(Unit* l_Unit = target->ToUnit())
                    l_Unit->SetPhaseMask(p_ScriptHolder.action.ingamePhaseId.id, true);
            }
            break;
        }

        case SMART_ACTION_SET_INGAME_PHASE_GROUP:
        {
            ///for (WorldObject* target : targets)
            ///{
            ///    if (p_ScriptHolder.action.ingamePhaseGroup.apply == 1)
            ///        PhasingHandler::AddPhaseGroup(target, p_ScriptHolder.action.ingamePhaseGroup.groupId, true);
            ///    else
            ///        PhasingHandler::RemovePhaseGroup(target, p_ScriptHolder.action.ingamePhaseGroup.groupId, true);
            ///}

            TC_LOG_ERROR("smartai", "Unhandled action called SMART_ACTION_SET_INGAME_PHASE_GROUP");
            break;
        }

        case SMART_ACTION_CAST:
        {
            if (targets.empty())
                break;

            if (p_ScriptHolder.action.cast.targetsLimit > 0 && targets.size() > p_ScriptHolder.action.cast.targetsLimit)
                Trinity::Containers::RandomResize(targets, p_ScriptHolder.action.cast.targetsLimit);

            for (WorldObject* target : targets)
            {
                // may be nullptr
                if (go)
                    go->CastSpell(target->ToUnit(), p_ScriptHolder.action.cast.spell);

                if (!IsUnit(target))
                    continue;

                if (!(p_ScriptHolder.action.cast.castFlags & SMARTCAST_AURA_NOT_PRESENT) || !target->ToUnit()->HasAura(p_ScriptHolder.action.cast.spell))
                {
                    TriggerCastFlags triggerFlag = TRIGGERED_NONE;
                    if (p_ScriptHolder.action.cast.castFlags & SMARTCAST_TRIGGERED)
                    {
                        if (p_ScriptHolder.action.cast.triggerFlags)
                            triggerFlag = TriggerCastFlags(p_ScriptHolder.action.cast.triggerFlags);
                        else
                            triggerFlag = TRIGGERED_FULL_MASK;
                    }

                    if (p_ScriptHolder.action.cast.castFlags & SMARTCAST_IGNORE_PREVIOUS)
                        triggerFlag = TRIGGERED_IGNORE_CAST_IN_PROGRESS;

                    Unit* l_Caster = me;

                    if (p_ScriptHolder.action.cast.castFlags & SMARTCAST_TARGET_CAST_SPELL)
                        l_Caster = target->ToUnit();

                    if (l_Caster)
                    {
                        if (p_ScriptHolder.action.cast.castFlags & SMARTCAST_INTERRUPT_PREVIOUS)
                            l_Caster->InterruptNonMeleeSpells(false);

                        if (p_ScriptHolder.action.cast.castFlags & SMARTCAST_COMBAT_MOVE)
                        {
                            // If cast flag SMARTCAST_COMBAT_MOVE is set combat movement will not be allowed
                            // unless target is outside spell range, out of mana, or LOS.

                            bool allowMove = false;
                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(p_ScriptHolder.action.cast.spell);
                            int32 mana = l_Caster->GetPower(POWER_MANA);
                            spellInfo->CalcPowerCost(l_Caster, spellInfo->GetSchoolMask());

                            if (l_Caster->GetDistance(target) > spellInfo->GetMaxRange(true) ||
                                l_Caster->GetDistance(target) < spellInfo->GetMinRange(true) ||
                                !l_Caster->IsWithinLOSInMap(target) ||
                                l_Caster->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED))
                                allowMove = true;

                            if (Creature* l_Creature = l_Caster->ToCreature())
                                ENSURE_AI(SmartAI, l_Creature->AI())->SetCombatMove(allowMove);
                        }

                        l_Caster->CastSpell(target->ToUnit(), p_ScriptHolder.action.cast.spell, triggerFlag);
                    }
                    else if (go)
                        go->CastSpell(target->ToUnit(), p_ScriptHolder.action.cast.spell); ///< @TODO: missing triggerFlag arg
                }
            }
            break;
        }

        case SMART_ACTION_INVOKER_CAST:
        {
            Unit* tempLastInvoker = GetLastInvoker(unit);
            if (!tempLastInvoker)
                break;

            if (targets.empty())
                break;

            if (p_ScriptHolder.action.cast.targetsLimit)
                Trinity::Containers::RandomResize(targets, p_ScriptHolder.action.cast.targetsLimit);

            for (WorldObject* target : targets)
            {
                if (!IsUnit(target))
                    continue;

                if (!(p_ScriptHolder.action.cast.castFlags & SMARTCAST_AURA_NOT_PRESENT) || !target->ToUnit()->HasAura(p_ScriptHolder.action.cast.spell))
                {
                    if (p_ScriptHolder.action.cast.castFlags & SMARTCAST_INTERRUPT_PREVIOUS)
                        tempLastInvoker->InterruptNonMeleeSpells(false);

                    TriggerCastFlags triggerFlag = TRIGGERED_NONE;
                    if (p_ScriptHolder.action.cast.castFlags & SMARTCAST_TRIGGERED)
                    {
                        if (p_ScriptHolder.action.cast.triggerFlags)
                            triggerFlag = TriggerCastFlags(p_ScriptHolder.action.cast.triggerFlags);
                        else
                            triggerFlag = TRIGGERED_FULL_MASK;
                    }

                    tempLastInvoker->CastSpell(target->ToUnit(), p_ScriptHolder.action.cast.spell, triggerFlag);
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_INVOKER_CAST: Invoker %u casts spell %u on target %u with castflags %u",
                        tempLastInvoker->GetGUIDLow(), p_ScriptHolder.action.cast.spell, target->GetGUIDLow(), p_ScriptHolder.action.cast.castFlags);
                }
            }
            break;
        }

        case SMART_ACTION_ADD_AURA:
        {
            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    target->ToUnit()->AddAura(p_ScriptHolder.action.cast.spell, target->ToUnit());
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_ADD_AURA: Adding aura %u to unit %u",
                        p_ScriptHolder.action.cast.spell, target->GetGUIDLow());
                }
            }
            break;
        }

        case SMART_ACTION_ACTIVATE_GOBJECT:
        {
            for (WorldObject* target : targets)
            {
                if (IsGameObject(target))
                {
                    // Activate
                    target->ToGameObject()->SetLootState(GO_READY);
                    target->ToGameObject()->UseDoorOrButton(0, false, unit);
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_ACTIVATE_GOBJECT. Gameobject %u (entry: %u) activated",
                        target->GetGUIDLow(), target->GetEntry());
                }
            }
            break;
        }

        case SMART_ACTION_RESET_GOBJECT:
        {
            for (WorldObject* target : targets)
            {
                if (IsGameObject(target))
                {
                    target->ToGameObject()->ResetDoorOrButton();
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_RESET_GOBJECT. Gameobject %u (entry: %u) reset",
                        target->GetGUIDLow(), target->GetEntry());
                }
            }
            break;
        }

        case SMART_ACTION_SET_EMOTE_STATE:
        {
            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    target->ToUnit()->SetUInt32Value(UNIT_NPC_EMOTESTATE, p_ScriptHolder.action.emote.emote);
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_SET_EMOTE_STATE. Unit %u set emotestate to %u",
                        target->GetGUIDLow(), p_ScriptHolder.action.emote.emote);
                }
            }
            break;
        }

        case SMART_ACTION_SET_UNIT_FLAG:
        {
            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    if (!p_ScriptHolder.action.unitFlag.type)
                    {
                        target->ToUnit()->SetFlag(UNIT_FIELD_FLAGS, p_ScriptHolder.action.unitFlag.flag);
                        TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_SET_UNIT_FLAG. Unit %u added flag %u to UNIT_FIELD_FLAGS",
                            target->GetGUIDLow(), p_ScriptHolder.action.unitFlag.flag);
                    }
                    else
                    {
                        target->ToUnit()->SetFlag(UNIT_FIELD_FLAGS_2, p_ScriptHolder.action.unitFlag.flag);
                        TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_SET_UNIT_FLAG. Unit %u added flag %u to UNIT_FIELD_FLAGS_2",
                            target->GetGUIDLow(), p_ScriptHolder.action.unitFlag.flag);
                    }
                }
            }
            break;
        }

        case SMART_ACTION_REMOVE_UNIT_FLAG:
        {
            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    if (!p_ScriptHolder.action.unitFlag.type)
                    {
                        target->ToUnit()->RemoveFlag(UNIT_FIELD_FLAGS, p_ScriptHolder.action.unitFlag.flag);
                        TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_REMOVE_UNIT_FLAG. Unit %u removed flag %u to UNIT_FIELD_FLAGS",
                            target->GetGUIDLow(), p_ScriptHolder.action.unitFlag.flag);
                    }
                    else
                    {
                        target->ToUnit()->RemoveFlag(UNIT_FIELD_FLAGS_2, p_ScriptHolder.action.unitFlag.flag);
                        TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_REMOVE_UNIT_FLAG. Unit %u removed flag %u to UNIT_FIELD_FLAGS_2",
                            target->GetGUIDLow(), p_ScriptHolder.action.unitFlag.flag);
                    }
                }
            }
            break;
        }

        /// No targets
        case SMART_ACTION_AUTO_ATTACK:
        {
            if (!IsSmartAI())
                break;

            if (Creature* l_Creature = me->ToCreature())
                ENSURE_AI(SmartAI, l_Creature->AI())->SetAutoAttack(p_ScriptHolder.action.autoAttack.attack != 0);
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_AUTO_ATTACK: Creature: %u bool on = %u",
                me->GetGUIDLow(), p_ScriptHolder.action.autoAttack.attack);
            break;
        }

        /// No targets
        case SMART_ACTION_ALLOW_COMBAT_MOVEMENT:
        {
            if (!IsSmartAI())
                break;

            bool move = p_ScriptHolder.action.combatMove.move != 0;
            if (Creature* l_Creature = me->ToCreature())
                ENSURE_AI(SmartAI, l_Creature->AI())->SetCombatMove(move);
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_ALLOW_COMBAT_MOVEMENT: Creature %u bool on = %u",
                me->GetGUIDLow(), p_ScriptHolder.action.combatMove.move);
            break;
        }

        /// No targets
        case SMART_ACTION_SET_EVENT_PHASE:
        {
            if (!GetBaseObject())
                break;

            SetPhase(p_ScriptHolder.action.setEventPhase.phase);
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_SET_EVENT_PHASE: Creature %u set event phase %u",
                GetBaseObject()->GetGUIDLow(), p_ScriptHolder.action.setEventPhase.phase);
            break;
        }

        case SMART_ACTION_INC_EVENT_PHASE:
        {
            if (!GetBaseObject())
                break;

            IncPhase(p_ScriptHolder.action.incEventPhase.inc);
            DecPhase(p_ScriptHolder.action.incEventPhase.dec);
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_INC_EVENT_PHASE: Creature %u inc event phase by %u, "
                "decrease by %u", GetBaseObject()->GetGUIDLow(), p_ScriptHolder.action.incEventPhase.inc, p_ScriptHolder.action.incEventPhase.dec);
            break;
        }

        case SMART_ACTION_EVADE:
        {
            if (!me)
                break;

            if (Creature* l_Creature = me->ToCreature())
                l_Creature->AI()->EnterEvadeMode();
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_EVADE: Creature %u EnterEvadeMode", me->GetGUIDLow());
            break;
        }

        case SMART_ACTION_FLEE_FOR_ASSIST:
        {
            if (!me)
                break;

            if (Creature* l_Creature = me->ToCreature())
                l_Creature->DoFleeToGetAssistance();

            if (p_ScriptHolder.action.flee.withEmote)
            {
                BroadcastTextBuilder builder(me, CHAT_MSG_MONSTER_EMOTE, me->GetEntry(), LANG_FLEE, 0);
                sCreatureTextMgr->SendChatPacket(me, builder, CHAT_MSG_MONSTER_EMOTE);
            }
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_FLEE_FOR_ASSIST: Creature %u DoFleeToGetAssistance", me->GetGUIDLow());
            break;
        }

        case SMART_ACTION_CALL_GROUPEVENTHAPPENS:
        {
            if (!unit)
                break;

            if (IsPlayer(unit) && GetBaseObject())
            {
                unit->ToPlayer()->GroupEventHappens(p_ScriptHolder.action.quest.quest, GetBaseObject());
                TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction: SMART_ACTION_CALL_GROUPEVENTHAPPENS: Player %u, group credit for quest %u",
                    unit->GetGUIDLow(), p_ScriptHolder.action.quest.quest);
            }

            // Special handling for vehicles
            if (Vehicle* vehicle = unit->GetVehicleKit())
                for (SeatMap::iterator it = vehicle->Seats.begin(); it != vehicle->Seats.end(); ++it)
                    if (Player* player = ObjectAccessor::FindPlayer(it->second.Passenger.Guid))
                        player->GroupEventHappens(p_ScriptHolder.action.quest.quest, GetBaseObject());
            break;
        }

        case SMART_ACTION_COMBAT_STOP:
        {
            for (WorldObject* l_Target : targets)
            {
                Unit* l_Unit = l_Target->ToUnit();
                if (!l_Unit)
                    continue;

                /// cause combat stop, threat clear, HP regen stop, motion master clear(any movement) AND will not evade.
                l_Unit->DeleteThreatList();

                if (Creature* l_Creature = l_Unit->ToCreature())
                    if (CreatureAI* l_AI = l_Creature->AI())
                        l_AI->IgnoreEvade(true);

                l_Unit->CombatStop(true);
            }
            break;
        }

        case SMART_ACTION_REMOVE_AURAS:
        {
            uint32 l_SpellId = p_ScriptHolder.action.RemoveAuras.SpellId;
            uint32 l_Stacks = p_ScriptHolder.action.RemoveAuras.Stacks;

            for (WorldObject* l_Target : targets)
            {
                Unit* l_Unit = l_Target->ToUnit();

                if (!l_Unit)
                    continue;

                if (l_SpellId)
                {
                    if (l_Stacks)
                    {
                        if (Aura* l_Aura = l_Unit->GetAura(l_SpellId))
                            l_Aura->ModCharges(-static_cast<int32>(l_Stacks), AURA_REMOVE_BY_EXPIRE);
                    }
                    else
                    {
                        l_Unit->RemoveAurasDueToSpell(l_SpellId);
                    }
                }
                else
                {
                    l_Unit->RemoveAllAuras();
                }
            }
            break;
        }

        case SMART_ACTION_FOLLOW:
        {
            if (!IsSmartAI())
                break;

            Creature* l_Creature = me->ToCreature();

            /// Should never happen
            if (!l_Creature)
                break;

            if (targets.empty())
            {
                ENSURE_AI(SmartAI, l_Creature->AI())->StopFollow(false);
                break;
            }

            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                {
                    float angle = p_ScriptHolder.action.follow.angle > 6 ? (p_ScriptHolder.action.follow.angle * M_PI / 180.0f) : p_ScriptHolder.action.follow.angle;
                    ENSURE_AI(SmartAI, l_Creature->AI())->SetFollow(target->ToUnit(), float(p_ScriptHolder.action.follow.dist) + 0.1f, angle, p_ScriptHolder.action.follow.credit, p_ScriptHolder.action.follow.entry, p_ScriptHolder.action.follow.creditType);
                    TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction: SMART_ACTION_FOLLOW: Creature %u following target %u",
                        me->GetGUIDLow(), target->GetGUIDLow());
                    break;
                }
            }
            break;
        }

        case SMART_ACTION_RANDOM_PHASE:
        {
            if (!GetBaseObject())
                break;

            std::vector<uint32> phases;
            std::copy_if(std::begin(p_ScriptHolder.action.randomPhase.phases), std::end(p_ScriptHolder.action.randomPhase.phases),
                std::back_inserter(phases), [](uint32 phase) { return phase != 0; });

            uint32 phase = Trinity::Containers::SelectRandomContainerElement(phases);
            SetPhase(phase);
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction: SMART_ACTION_RANDOM_PHASE: Creature %u sets event phase to %u",
                GetBaseObject()->GetGUIDLow(), phase);
            break;
        }

        case SMART_ACTION_RANDOM_PHASE_RANGE:
        {
            if (!GetBaseObject())
                break;

            uint32 phase = urand(p_ScriptHolder.action.randomPhaseRange.phaseMin, p_ScriptHolder.action.randomPhaseRange.phaseMax);
            SetPhase(phase);
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction: SMART_ACTION_RANDOM_PHASE_RANGE: Creature %u sets event phase to %u",
                GetBaseObject()->GetGUIDLow(), phase);
            break;
        }

        case SMART_ACTION_CALL_KILLEDMONSTER:
        {
            if (p_ScriptHolder.target.type == SMART_TARGET_NONE || p_ScriptHolder.target.type == SMART_TARGET_SELF) // Loot recipient and his group members
            {
                if (!me)
                    break;

                if (Creature* l_Creature = me->ToCreature())
                {
                    if (Player* player = l_Creature->GetLootRecipient())
                    {
                        player->RewardPlayerAndGroupAtEvent(p_ScriptHolder.action.killedMonster.creature, player);
                        TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction: SMART_ACTION_CALL_KILLEDMONSTER: Player %u, Killcredit: %u",
                            player->GetGUIDLow(), p_ScriptHolder.action.killedMonster.creature);
                    }
                }
            }
            else // Specific target type
            {
                for (WorldObject* target : targets)
                {
                    if (IsPlayer(target))
                    {
                        target->ToPlayer()->KilledMonsterCredit(p_ScriptHolder.action.killedMonster.creature);
                        TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction: SMART_ACTION_CALL_KILLEDMONSTER: Player %u, Killcredit: %u",
                            target->GetGUIDLow(), p_ScriptHolder.action.killedMonster.creature);
                    }
                    else if (IsUnit(target)) // Special handling for vehicles
                        if (Vehicle* vehicle = target->ToUnit()->GetVehicleKit())
                            for (SeatMap::iterator seatItr = vehicle->Seats.begin(); seatItr != vehicle->Seats.end(); ++seatItr)
                                if (Player* player = ObjectAccessor::GetPlayer(*target, seatItr->second.Passenger.Guid))
                                    player->KilledMonsterCredit(p_ScriptHolder.action.killedMonster.creature);
                }
            }
            break;
        }

        case SMART_ACTION_SET_INST_DATA:
        {
            WorldObject* obj = GetBaseObject();
            if (!obj)
                obj = unit;

            if (!obj)
                break;

            InstanceScript* instance = obj->GetInstanceScript();
            if (!instance)
            {
                TC_LOG_ERROR("sql.sql", "SmartScript: Event %u attempt to set instance data without instance script. EntryOrGuid %d", p_ScriptHolder.GetEventType(), p_ScriptHolder.entryOrGuid);
                break;
            }

            instance->SetData(p_ScriptHolder.action.setInstanceData.field, p_ScriptHolder.action.setInstanceData.data);
            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction: SMART_ACTION_SET_INST_DATA: Field: %u, data: %u",
                p_ScriptHolder.action.setInstanceData.field, p_ScriptHolder.action.setInstanceData.data);
            break;
        }

        case SMART_ACTION_SET_INST_DATA64:
        {
            WorldObject* obj = GetBaseObject();
            if (!obj)
                obj = unit;

            if (!obj)
                break;

            InstanceScript* instance = obj->GetInstanceScript();
            if (!instance)
            {
                TC_LOG_ERROR("sql.sql", "SmartScript: Event %u attempt to set instance data without instance script. EntryOrGuid %d", p_ScriptHolder.GetEventType(), p_ScriptHolder.entryOrGuid);
                break;
            }

            if (targets.empty())
                break;

            instance->SetData64(p_ScriptHolder.action.setInstanceData64.field, targets.front()->GetGUID());
            break;
        }

        case SMART_ACTION_UPDATE_TEMPLATE:
        {
            if (!me || me->GetEntry() == p_ScriptHolder.action.updateTemplate.creature)
                break;

            if (Creature* l_Creature = me->ToCreature())
            {
                l_Creature->UpdateEntry(p_ScriptHolder.action.updateTemplate.creature, p_ScriptHolder.action.updateTemplate.team ? HORDE : ALLIANCE, nullptr, p_ScriptHolder.action.updateTemplate.updateLevel);
                TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction: SMART_ACTION_UPDATE_TEMPLATE: Creature %u, Template: %u, Team: %u",
                    me->GetGUIDLow(), me->GetEntry(), p_ScriptHolder.action.updateTemplate.team ? HORDE : ALLIANCE);
            }
            break;
        }

        case SMART_ACTION_DIE:
        {
            if (!me)
                break;

            for (WorldObject* l_Target : targets)
            {
                Unit* l_Unit = l_Target->ToUnit();

                if (!l_Unit || l_Unit->isDead())
                    continue;

                me->Kill(l_Unit);
            }
            break;
        }

        case SMART_ACTION_SET_IN_COMBAT_WITH_ZONE:
        {
            for (WorldObject* l_Target : targets)
            {
                if (Creature* l_Creature = l_Target->ToCreature())
                    l_Creature->SetInCombatWithZone();
            }
            break;
        }

        case SMART_ACTION_CALL_FOR_HELP:
        {
            float l_Range = static_cast<float>(p_ScriptHolder.action.callHelp.range);
            bool l_WithEmote = p_ScriptHolder.action.callHelp.withEmote > 0;

            for (WorldObject* l_Target : targets)
            {
                Creature* l_Creature = l_Target->ToCreature();

                if (!l_Creature)
                    continue;

                l_Creature->CallForHelp(l_Range);
            }

            if (me)
            {
                if (l_WithEmote)
                {
                    BroadcastTextBuilder builder(me, CHAT_MSG_MONSTER_EMOTE, me->GetEntry(), LANG_FLEE, 0);
                    sCreatureTextMgr->SendChatPacket(me, builder, CHAT_MSG_MONSTER_EMOTE);
                }
            }
            break;
        }

        case SMART_ACTION_SET_SHEATH:
        {
            SheathState l_Sheath = static_cast<SheathState>(p_ScriptHolder.action.setSheath.sheath);

            for (WorldObject* l_Target : targets)
            {
                Unit* l_Unit = l_Target->ToUnit();

                if (!l_Unit)
                    continue;

                l_Unit->SetSheath(l_Sheath);
            }
            break;
        }

        case SMART_ACTION_FORCE_DESPAWN:
        {
            // there should be at least a world update tick before despawn, to avoid breaking linked actions
            uint32 despawnDelay = p_ScriptHolder.action.forceDespawn.delay;
            if (despawnDelay <= 0)
                despawnDelay = 1;

            Seconds forceRespawnTimer(p_ScriptHolder.action.forceDespawn.forceRespawnTimer);

            for (WorldObject* target : targets)
            {
                if (Creature* creature = target->ToCreature())
                    creature->DespawnOrUnsummon(despawnDelay);
                else if (GameObject* goTarget = target->ToGameObject())
                    goTarget->SetRespawnTime(despawnDelay);
            }
            break;
        }

        case SMART_ACTION_MOUNT_TO_ENTRY_OR_MODEL:
        {
            for (WorldObject* target : targets)
            {
                if (!IsUnit(target))
                    continue;

                if (p_ScriptHolder.action.morphOrMount.creature || p_ScriptHolder.action.morphOrMount.model)
                {
                    if (p_ScriptHolder.action.morphOrMount.creature > 0)
                    {
                        if (CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(p_ScriptHolder.action.morphOrMount.creature))
                            target->ToUnit()->Mount(ObjectMgr::ChooseDisplayId(0, cInfo));
                    }
                    else
                        target->ToUnit()->Mount(p_ScriptHolder.action.morphOrMount.model);
                }
                else
                    target->ToUnit()->Dismount();
            }
            break;
        }

        case SMART_ACTION_SET_INVINCIBILITY_HP_LEVEL:
        {
            if (!me && !me->ToCreature())
                break;

            SmartAI* ai = CAST_AI(SmartAI, me->ToCreature()->AI());
            if (!ai)
                break;

            if (p_ScriptHolder.action.invincHP.percent)
                ai->SetInvincibilityHpLevel(me->ToCreature()->CountPctFromMaxHealth(p_ScriptHolder.action.invincHP.percent));
            else
                ai->SetInvincibilityHpLevel(p_ScriptHolder.action.invincHP.minHP);

            break;
        }

        case SMART_ACTION_SET_DATA:
        {
            for (WorldObject* target : targets)
            {
                if (Creature* cTarget = target->ToCreature())
                {
                    CreatureAI* ai = cTarget->AI();
                    if (IsSmartAI(cTarget))
                        ENSURE_AI(SmartAI, ai)->SetData(p_ScriptHolder.action.setData.field, p_ScriptHolder.action.setData.data);
                    else
                        ai->SetData(p_ScriptHolder.action.setData.field, p_ScriptHolder.action.setData.data);
                }
                else if (GameObject* oTarget = target->ToGameObject())
                {
                    GameObjectAI* ai = oTarget->AI();
                    if (IsSmartAI(oTarget))
                        ENSURE_AI(SmartGameObjectAI, ai)->SetData(p_ScriptHolder.action.setData.field, p_ScriptHolder.action.setData.data);
                    else
                        ai->SetData(p_ScriptHolder.action.setData.field, p_ScriptHolder.action.setData.data);
                }
            }
            break;
        }

        case SMART_ACTION_MOVE_FORWARD:
        {
            for (WorldObject* target : targets)
            {
                if (!IsCreature(target))
                    continue;

                //if (!(e.event.event_flags & SMART_EVENT_FLAG_WHILE_CHARMED) && IsCharmedCreature(target))
                //    continue;

                Position pos = *target;

                // Use forward/backward/left/right cartesian plane movement
                float x, y, z, o;
                o = pos.GetOrientation();
                x = pos.GetPositionX() + (std::cos(o - (M_PI / 2))*p_ScriptHolder.target.x) + (std::cos(o)*p_ScriptHolder.target.y);
                y = pos.GetPositionY() + (std::sin(o - (M_PI / 2))*p_ScriptHolder.target.x) + (std::sin(o)*p_ScriptHolder.target.y);
                z = pos.GetPositionZ() + p_ScriptHolder.target.z;
                target->ToCreature()->GetMotionMaster()->MovePoint(SMART_RANDOM_POINT, x, y, z);
            }
            break;
        }

        case SMART_ACTION_SET_VISIBILITY:
        {
            for (WorldObject* target : targets)
            {
                if (IsUnit(target))
                    target->ToUnit()->SetVisible(p_ScriptHolder.action.visibility.state ? true : false);
            }
            break;
        }

        case SMART_ACTION_SET_ACTIVE:
        {
            if (!targets.empty())
            {
                for (WorldObject* target : targets)
                {
                    target->setActive(p_ScriptHolder.action.active.state ? true : false);
                }
            }
            else
            {
                if (auto l_BaseObject = GetBaseObject())
                {
                    l_BaseObject->setActive(p_ScriptHolder.action.active.state ? true : false);
                }
            }

            break;
        }

        case SMART_ACTION_ATTACK_START:
        {
            if (!me)
                break;

            Creature* l_Creature = me->ToCreature();
            if (!l_Creature)
                break;

            if (targets.empty())
                break;

            // attack random target
            if (Unit* target = Trinity::Containers::SelectRandomContainerElement(targets)->ToUnit())
                l_Creature->AI()->AttackStart(target);
            break;
        }

        case SMART_ACTION_SUMMON_CREATURE:
        {
            if (!GetBaseObject())
                break;

            uint32 l_Entry = p_ScriptHolder.action.summonCreature.Entry;
            TempSummonType l_SummonType = static_cast<TempSummonType>(p_ScriptHolder.action.summonCreature.SummonType);
            uint32 l_Duration = p_ScriptHolder.action.summonCreature.Duration;
            bool   l_AttackInvoker = p_ScriptHolder.action.summonCreature.AttackInvoker > 0;
            uint32 l_SummonField = p_ScriptHolder.action.summonCreature.SummonField;

            Creature* l_Summon = nullptr;

            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_POSITION)
            {
                Position l_Pos(p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z, p_ScriptHolder.target.o);

                l_Summon = GetBaseObject()->SummonCreature(l_Entry, l_Pos, l_SummonType, l_Duration);
            }
            else
            {
                for (WorldObject* l_Target : targets)
                {
                    Position l_Pos = *l_Target;

                l_Pos.m_positionX += p_ScriptHolder.target.x;
                l_Pos.m_positionY += p_ScriptHolder.target.y;
                l_Pos.m_positionZ += p_ScriptHolder.target.z;
                l_Pos.m_orientation += p_ScriptHolder.target.o;

                l_Target->SummonCreature(l_Entry, l_Pos, l_SummonType, l_Duration);
            }
        }

        if (l_AttackInvoker)
            if (l_Summon && l_Summon->AI() && unit)
                l_Summon->AI()->AttackStart(unit);

        if (me)
            if (Creature* l_Creature = me->ToCreature())
                if (CreatureAI* l_AI = l_Creature->AI())
                    l_AI->SetData(l_SummonField, 0);

            break;
        }

        case SMART_ACTION_SUMMON_GO:
        {
            if (!GetBaseObject())
                break;

            float x, y, z, o;

            for (WorldObject* target : targets)
            {
                target->GetPosition(x, y, z, o);
                x += p_ScriptHolder.target.x;
                y += p_ScriptHolder.target.y;
                z += p_ScriptHolder.target.z;
                o += p_ScriptHolder.target.o;
                GetBaseObject()->SummonGameObject(p_ScriptHolder.action.summonGO.entry, x, y, z, o, 0, 0, 0, 0, p_ScriptHolder.action.summonGO.despawnTime);
            }

            if (p_ScriptHolder.GetTargetType() != SMART_TARGET_POSITION)
                break;

            GetBaseObject()->SummonGameObject(p_ScriptHolder.action.summonGO.entry, p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z, p_ScriptHolder.target.o, 0, 0, 0, 0, p_ScriptHolder.action.summonGO.despawnTime);
            break;
        }

        case SMART_ACTION_KILL_UNIT:
        {
            for (WorldObject* target : targets)
            {
                Unit* l_Unit = target->ToUnit();
                if (!l_Unit)
                    continue;

                l_Unit->Kill(l_Unit);
            }
            break;
        }

        case SMART_ACTION_INSTALL_AI_TEMPLATE:
        {
            InstallTemplate(p_ScriptHolder);
            break;
        }

        case SMART_ACTION_ADD_ITEM:
        {
            for (WorldObject* target : targets)
            {
                if (!IsPlayer(target))
                    continue;

                target->ToPlayer()->AddItem(p_ScriptHolder.action.item.entry, p_ScriptHolder.action.item.count);
            }
            break;
        }

        case SMART_ACTION_REMOVE_ITEM:
        {
            for (WorldObject* target : targets)
            {
                if (!IsPlayer(target))
                    continue;

                target->ToPlayer()->DestroyItemCount(p_ScriptHolder.action.item.entry, p_ScriptHolder.action.item.count, true);
            }
            break;
        }

        case SMART_ACTION_STORE_VARIABLE_DECIMAL:
        {
            if (mStoredDecimals.find(p_ScriptHolder.action.storeVar.id) != mStoredDecimals.end())
                mStoredDecimals.erase(p_ScriptHolder.action.storeVar.id);
            mStoredDecimals[p_ScriptHolder.action.storeVar.id] = p_ScriptHolder.action.storeVar.number;
            break;
        }

        case SMART_ACTION_STORE_TARGET_LIST:
        {
            StoreTargetVector(targets, p_ScriptHolder.action.storeTargets.id);
            break;
        }

        case SMART_ACTION_TELEPORT:
        {
            uint32 l_MapId = p_ScriptHolder.action.teleport.mapID;

            for (WorldObject* target : targets)
            {
                if (IsPlayer(target))
                    target->ToPlayer()->TeleportTo(l_MapId, p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z, p_ScriptHolder.target.o);
                else if (IsCreature(target))
                    target->ToCreature()->NearTeleportTo(p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z, p_ScriptHolder.target.o);
            }
            break;
        }

        case SMART_ACTION_SET_RUN:
        {
            bool l_Run = p_ScriptHolder.action.setRun.run > 0;

            for (WorldObject* l_Target : targets)
            {
                Creature* l_Creature = l_Target->ToCreature();

                if (!l_Creature)
                    continue;

                if (!IsSmartAI(l_Creature, false))
                    break;

                CAST_AI(SmartAI, l_Creature->AI())->SetRun(l_Run);
            }

            break;
        }

        case SMART_ACTION_SET_FLY:
        {
            bool l_Fly = p_ScriptHolder.action.setFly.fly > 0;
            uint32 l_Type = p_ScriptHolder.action.setFly.Type;

            enum eFlyTypes
            {
                DisableGravity,
                CanFly,
            };

            for (WorldObject* l_Target : targets)
            {
                Unit* l_Unit = l_Target->ToUnit();

                if (Creature* l_Creature = l_Unit->ToCreature())
                    l_Creature->SetIgnoreUpdateMovementFlag(l_Fly);

                if (l_Type == eFlyTypes::DisableGravity)
                    l_Unit->SetDisableGravity(l_Fly);
                else if (l_Type == eFlyTypes::CanFly)
                    l_Unit->SetCanFly(l_Fly);
            }

            TC_LOG_DEBUG("smartscript", "SMART_ACTION_SET_FLY: Fly: %u, Type: %u", l_Fly ? 1 : 0, l_Type);

            break;
        }

        case SMART_ACTION_SET_SWIM:
        {
            if (!IsSmartAI())
                break;

            if (Creature* l_Creature = me->ToCreature())
                CAST_AI(SmartAI, l_Creature->AI())->SetSwim(p_ScriptHolder.action.setSwim.swim ? true : false);
            break;
        }

        case SMART_ACTION_SET_COUNTER:
        {
            StoreCounter(p_ScriptHolder.action.setCounter.counterId, p_ScriptHolder.action.setCounter.value, p_ScriptHolder.action.setCounter.reset);
            break;
        }

        case SMART_ACTION_WP_START:
        {
            if (!IsSmartAI())
                break;

            if (Creature* l_Creature = me->ToCreature())
            {
				bool run = p_ScriptHolder.action.wpStart.run ? true : false;
                uint32 entry = p_ScriptHolder.action.wpStart.pathID;
                bool repeat = p_ScriptHolder.action.wpStart.canRepeat ? true : false;
                StoreTargetVector(targets, SMART_ESCORT_TARGETS);
                l_Creature->SetReactState((ReactStates)p_ScriptHolder.action.wpStart.reactState);
                CAST_AI(SmartAI, l_Creature->AI())->StartPath(run, entry, repeat, unit);

                uint32 DespawnTime = p_ScriptHolder.action.wpStart.despawnTime;
                CAST_AI(SmartAI, l_Creature->AI())->SetDespawnTime(DespawnTime);
            }
            break;
        }

        case SMART_ACTION_WP_PAUSE:
        {
            if (!IsSmartAI())
                break;

            if (Creature* l_Creature = me->ToCreature())
            {
                uint32 delay = p_ScriptHolder.action.wpPause.delay;
                CAST_AI(SmartAI, l_Creature->AI())->PausePath(delay, p_ScriptHolder.GetEventType() == SMART_EVENT_WAYPOINT_REACHED ? false : true);
            }

            break;
        }

        case SMART_ACTION_WP_STOP:
        {
            if (!IsSmartAI())
                break;

            if (Creature* l_Creature = me->ToCreature())
            {
                uint32 DespawnTime = p_ScriptHolder.action.wpStop.despawnTime;
                uint32 quest = p_ScriptHolder.action.wpStop.quest;
                bool fail = p_ScriptHolder.action.wpStop.fail ? true : false;
                CAST_AI(SmartAI, l_Creature->AI())->StopPath(DespawnTime, quest, fail);
            }
            break;
        }

        case SMART_ACTION_WP_RESUME:
        {
            if (!IsSmartAI())
                break;

            if (Creature* l_Creature = me->ToCreature())
                CAST_AI(SmartAI, l_Creature->AI())->ResumePath();
            break;
        }

        case SMART_ACTION_SET_ORIENTATION:
        {
            if (!me)
                break;

            float l_HomePos = me->ToCreature() ? me->ToCreature()->GetHomePosition().GetOrientation() : me->GetOrientation();

            /// This depends on whet target script have if SMART_TARGET_SELF than Facing will be set like in HomePosition, When SMART_TARGET_POSITION you need to set target_o. 0 = North, West = 1.5, South = 3, East = 4.5
            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_SELF)
                me->SetFacingTo(l_HomePos);
            else if (p_ScriptHolder.GetTargetType() == SMART_TARGET_POSITION)
                me->SetFacingTo(p_ScriptHolder.target.o);
            else if (!targets.empty())
                me->SetFacingToObject(targets.front());
            break;
        }

        case SMART_ACTION_PLAYMOVIE:
        {
            for (WorldObject* target : targets)
            {
                if (!IsPlayer(target))
                    continue;

                target->ToPlayer()->SendMovieStart(p_ScriptHolder.action.movie.entry);
            }
            break;
        }

        case SMART_ACTION_MOVE_TO_POS:
        {
            if (!me)
                break;

            WorldObject* target = nullptr;

            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_RANGE       || p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_GUID ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_DISTANCE    || p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_RANGE ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_GUID      || p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_DISTANCE ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_CREATURE     || p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_GAMEOBJECT ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_OWNER_OR_SUMMONER    || p_ScriptHolder.GetTargetType() == SMART_TARGET_ACTION_INVOKER ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_ENEMY        || p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_FRIENDLY)
            {
                // we want to move to random element
                if (!targets.empty())
                    target = Trinity::Containers::SelectRandomContainerElement(targets);
            }

            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_POSITION)
            {
                Position l_CurrentPos;

                me->GetPosition(&l_CurrentPos);

                SmartTargetPositionTypes::Type l_PositionType = static_cast<SmartTargetPositionTypes::Type>(p_ScriptHolder.target.position.Position);
                float l_Distance = static_cast<float>(p_ScriptHolder.target.position.Distance);
                float l_Speed = static_cast<float>(p_ScriptHolder.target.position.Speed);
                bool l_ContactDistance = p_ScriptHolder.action.MoveToPos.ContactDistance >= 1;

                if (p_ScriptHolder.action.MoveToPos.useWalk)
                    me->SetWalk(true);

                switch (l_PositionType)
                {
                    default:
                    case SmartTargetPositionTypes::Position:
                        l_CurrentPos.Relocate(p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z);
                        break;

                    case SmartTargetPositionTypes::TransportPosition:
                        if (Transport* l_Transport = me->GetTransport())
                            l_Transport->CalculatePassengerPosition(l_CurrentPos.m_positionX, l_CurrentPos.m_positionY, l_CurrentPos.m_positionZ, l_CurrentPos.m_orientation);
                        break;

                    case SmartTargetPositionTypes::HomePosition:
                        if(Creature* l_Creature = me->ToCreature())
                            l_CurrentPos = l_Creature->GetHomePosition();
                        break;

                    case SmartTargetPositionTypes::GroundPosition:
                        l_CurrentPos.m_positionZ = me->GetMap()->GetHeight(l_CurrentPos.m_positionX, l_CurrentPos.m_positionY, l_CurrentPos.m_positionZ);
                        break;

                    case SmartTargetPositionTypes::Foward:
                        l_CurrentPos.m_positionX += l_Distance * std::cos(l_CurrentPos.m_orientation);
                        l_CurrentPos.m_positionY += l_Distance * std::sin(l_CurrentPos.m_orientation);
                        l_CurrentPos.m_orientation = l_CurrentPos.GetAngle(l_CurrentPos.m_positionX, l_CurrentPos.m_positionY);
                        break;

                    case SmartTargetPositionTypes::Backward:
                        l_CurrentPos.m_positionX -= l_Distance * std::cos(l_CurrentPos.m_orientation);
                        l_CurrentPos.m_positionY -= l_Distance * std::sin(l_CurrentPos.m_orientation);
                        l_CurrentPos.m_orientation = l_CurrentPos.GetAngle(l_CurrentPos.m_positionX, l_CurrentPos.m_positionY);
                        break;

                    case SmartTargetPositionTypes::Up:
                        l_CurrentPos.m_positionZ += l_Distance;
                        break;

                    case SmartTargetPositionTypes::Down:
                        l_CurrentPos.m_positionZ -= l_Distance;
                        break;

                        /// Only used by one entry, need to save position when spawning in creature..
                    case SmartTargetPositionTypes::SpawnPosition:

                        if (Creature* l_Creature = me->ToCreature())
                            l_CurrentPos = l_Creature->GetHomePosition();

                        break;
                }

                if (l_ContactDistance)
                {
                    l_CurrentPos.m_positionX -= CONTACT_DISTANCE * std::cos(l_CurrentPos.m_orientation);
                    l_CurrentPos.m_positionY -= CONTACT_DISTANCE * std::sin(l_CurrentPos.m_orientation);
                }

                if (l_PositionType == SmartTargetPositionTypes::Foward || l_PositionType == SmartTargetPositionTypes::Backward)
                {
                    l_CurrentPos.m_positionZ = me->GetMap()->GetHeight(l_CurrentPos.m_positionX, l_CurrentPos.m_positionY, l_CurrentPos.m_positionZ);
                }

                /// Artamedes: Make sure we in LoS
                // me->GetNearPoint(me, l_CurrentPos.m_positionX, l_CurrentPos.m_positionY, l_CurrentPos.m_positionZ, 1.0f, 1.0f, 1.0f); // will break Move to PoS

                switch (p_ScriptHolder.action.MoveToPos.MovementType)
                {
                    case 1: ///< TakeOff
                        me->GetMotionMaster()->MoveTakeoff(p_ScriptHolder.action.MoveToPos.pointId, l_CurrentPos);
                        break;

                    case 2: ///< Land
                        me->GetMotionMaster()->MoveLand(p_ScriptHolder.action.MoveToPos.pointId, l_CurrentPos);
                        break;

                    case 3: ///< Charge
                        me->GetMotionMaster()->MoveCharge(l_CurrentPos.GetPositionX(), l_CurrentPos.GetPositionY(), l_CurrentPos.GetPositionZ(), l_Speed, p_ScriptHolder.action.MoveToPos.pointId, p_ScriptHolder.action.MoveToPos.disablePathfinding == 0);
                        break;

                    case 0:
                    default:
                        me->GetMotionMaster()->MovePoint(p_ScriptHolder.action.MoveToPos.pointId, l_CurrentPos, p_ScriptHolder.action.MoveToPos.disablePathfinding == 0, l_Speed);
                        break;
                }
                break;
            }

            if (!target)
                me->GetMotionMaster()->MovePoint(p_ScriptHolder.action.MoveToPos.pointId, p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z, p_ScriptHolder.action.MoveToPos.disablePathfinding == 0);
            else
                me->GetMotionMaster()->MovePoint(p_ScriptHolder.action.MoveToPos.pointId, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), p_ScriptHolder.action.MoveToPos.disablePathfinding == 0);

            break;
        }

        case SMART_ACTION_RESPAWN_TARGET:
        {
            for (WorldObject* target : targets)
            {
                if (IsCreature(target))
                    target->ToCreature()->Respawn();
                else if (IsGameObject(target))
                    target->ToGameObject()->SetRespawnTime(p_ScriptHolder.action.RespawnTarget.goRespawnTime);
            }
            break;
        }

        case SMART_ACTION_CLOSE_GOSSIP:
        {
            for (WorldObject* target : targets)
            {
                if (Player* l_Player = target->ToPlayer())
                    l_Player->PlayerTalkClass->SendCloseGossip();
            }
            break;
        }

        case SMART_ACTION_EQUIP:
        {
            for (WorldObject* target : targets)
            {
                if (Creature* npc = target->ToCreature())
                {
                    std::array<uint32, MAX_EQUIPMENT_ITEMS> slot;
                    if (int8 equipId = static_cast<int8>(p_ScriptHolder.action.equip.entry)) ///< equipId is not used (CATACLYSM)
                    {
                        EquipmentInfo const* eInfo = sObjectMgr->GetEquipmentInfo(npc->GetEntry());
                        if (!eInfo)
                        {
                            TC_LOG_ERROR("sql.sql", "SmartScript: SMART_ACTION_EQUIP uses non-existent equipment info id %u for creature %u", equipId, npc->GetEntry());
                            break;
                        }

                        npc->SetCurrentEquipmentId(equipId);

                        std::copy(std::begin(eInfo->ItemEntry), std::end(eInfo->ItemEntry), std::begin(slot));
                    }
                    else
                    {
                        slot[0] = p_ScriptHolder.action.equip.slot1;
                        slot[1] = p_ScriptHolder.action.equip.slot2;
                        slot[2] = p_ScriptHolder.action.equip.slot3;
                    }

                    for (uint32 i = 0; i < MAX_EQUIPMENT_ITEMS; ++i)
                        if (!p_ScriptHolder.action.equip.mask || (p_ScriptHolder.action.equip.mask & (1 << i)))
                            npc->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + i, slot[i]);
                }
            }
            break;
        }

        case SMART_ACTION_CREATE_TIMED_EVENT:
        {
            SmartEvent ne;
            ne.type = (SMART_EVENT)SMART_EVENT_UPDATE;
            ne.event_chance = p_ScriptHolder.action.timeEvent.chance;
            if (!ne.event_chance) ne.event_chance = 100;

            ne.minMaxRepeat.min = p_ScriptHolder.action.timeEvent.min;
            ne.minMaxRepeat.max = p_ScriptHolder.action.timeEvent.max;
            ne.minMaxRepeat.repeatMin = p_ScriptHolder.action.timeEvent.repeatMin;
            ne.minMaxRepeat.repeatMax = p_ScriptHolder.action.timeEvent.repeatMax;

            ne.event_flags = 0;
            if (!ne.minMaxRepeat.repeatMin && !ne.minMaxRepeat.repeatMax)
                ne.event_flags |= SMART_EVENT_FLAG_NOT_REPEATABLE;

            SmartAction ac;
            ac.type = (SMART_ACTION)SMART_ACTION_TRIGGER_TIMED_EVENT;
            ac.timeEvent.id = p_ScriptHolder.action.timeEvent.id;

            SmartScriptHolder ev;
            ev.m_Event = ne;
            ev.event_id = p_ScriptHolder.action.timeEvent.id;
            ev.target = p_ScriptHolder.target;
            ev.action = ac;
            InitTimer(ev);
            mStoredEvents.push_back(ev);
            break;
        }

        case SMART_ACTION_TRIGGER_TIMED_EVENT:
            ProcessEventsFor((SMART_EVENT)SMART_EVENT_TIMED_EVENT_TRIGGERED, nullptr, p_ScriptHolder.action.timeEvent.id);
            break;

        case SMART_ACTION_REMOVE_TIMED_EVENT:
            mRemIDs.push_back(p_ScriptHolder.action.timeEvent.id);
            break;

        case SMART_ACTION_OVERRIDE_SCRIPT_BASE_OBJECT:
        {
            for (WorldObject* target : targets)
            {
                if (IsCreature(target))
                {
                    if (!m_CreatureOrginalGUID && me)
                        m_CreatureOrginalGUID = me->GetGUID();
                    if (!m_GameObjectOriginalGUID && go)
                        m_GameObjectOriginalGUID = go->GetGUID();
                    go = nullptr;
                    me = target->ToCreature();
                    break;
                }
                else if (IsGameObject(target))
                {
                    if (!m_CreatureOrginalGUID && me)
                        m_CreatureOrginalGUID = me->GetGUID();
                    if (!m_GameObjectOriginalGUID && go)
                        m_GameObjectOriginalGUID = go->GetGUID();
                    go = target->ToGameObject();
                    me = nullptr;
                    break;
                }
            }
            break;
        }

        case SMART_ACTION_RESET_SCRIPT_BASE_OBJECT:
        {
            ResetBaseObject();
            break;
        }

        case SMART_ACTION_CALL_SCRIPT_RESET:
        {
            if (p_ScriptHolder.GetEventType() == SMART_EVENT_RESET)
            {
                TC_LOG_ERROR("smartai", "Wrong event type called!, SMART_EVENT_RESET on SMART_ACTION_CALL_SCRIPT_RESET");
                break;
            }

            if (m_ShouldPreventEventPhaseReset)
                SetPhase(0);

            bool l_ResetTAL = p_ScriptHolder.action.CallScriptReset.ResetTAL > 0;

            OnReset();

            break;
        }

        case SMART_ACTION_SET_RANGED_MOVEMENT:
        {
            if (!IsSmartAI())
                break;

            float attackDistance = float(p_ScriptHolder.action.setRangedMovement.distance);
            float attackAngle = float(p_ScriptHolder.action.setRangedMovement.angle) / 180.0f * static_cast<float>(M_PI);

            for (WorldObject* target : targets)
            {
                if (Creature* creature = target->ToCreature())
                    if (IsSmartAI(creature) && creature->getVictim())
                        if (ENSURE_AI(SmartAI, creature->AI())->CanCombatMove())
                            creature->GetMotionMaster()->MoveChase(creature->getVictim(), attackDistance, attackAngle);
            }
            break;
        }

        case SMART_ACTION_CALL_TIMED_ACTIONLIST:
        {
            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_NONE)
            {
                TC_LOG_ERROR("sql.sql", "SmartScript: Entry %d SourceType %u Event %u Action %u is using TARGET_NONE(0) for Script9 target. Please correct target_type in database.", p_ScriptHolder.entryOrGuid, p_ScriptHolder.GetScriptType(), p_ScriptHolder.GetEventType(), p_ScriptHolder.GetActionType());
                break;
            }

            for (WorldObject* target : targets)
            {
                if (Creature* creature = target->ToCreature())
                {
                    if (IsSmartAI(creature))
                        ENSURE_AI(SmartAI, creature->AI())->SetTimedActionList(p_ScriptHolder, p_ScriptHolder.action.timedActionList.id, GetLastInvoker());
                }
                else if (GameObject* goTarget = target->ToGameObject())
                {
                    if (IsSmartAI(goTarget))
                        ENSURE_AI(SmartGameObjectAI, goTarget->AI())->SetTimedActionList(p_ScriptHolder, p_ScriptHolder.action.timedActionList.id, GetLastInvoker());
                }
            }
            break;
        }

        case SMART_ACTION_SET_NPC_FLAG:
        {
            for (WorldObject* target : targets)
                if (IsCreature(target))
                    target->ToUnit()->SetUInt32Value(UNIT_NPC_FLAGS, p_ScriptHolder.action.unitFlag.flag);
            break;
        }

        case SMART_ACTION_ADD_NPC_FLAG:
        {
            for (WorldObject* target : targets)
                if (IsCreature(target))
                    target->ToUnit()->SetFlag(UNIT_NPC_FLAGS, p_ScriptHolder.action.unitFlag.flag);
            break;
        }

        case SMART_ACTION_REMOVE_NPC_FLAG:
        {
            for (WorldObject* target : targets)
                if (IsCreature(target))
                    target->ToUnit()->RemoveFlag(UNIT_NPC_FLAGS, p_ScriptHolder.action.unitFlag.flag);
            break;
        }

        case SMART_ACTION_CROSS_CAST:
        {
            if (targets.empty())
                break;

            ObjectVector casters;
            GetTargets(casters, CreateSmartEvent(SMART_EVENT_UPDATE_IC, 0, 0, 0, 0, 0, 0, SMART_ACTION_NONE, 0, 0, 0, 0, 0, 0, (SMARTAI_TARGETS)p_ScriptHolder.action.crossCast.targetType, p_ScriptHolder.action.crossCast.targetParam1, p_ScriptHolder.action.crossCast.targetParam2, p_ScriptHolder.action.crossCast.targetParam3, 0), unit);

            for (WorldObject* caster : casters)
            {
                if (!IsUnit(caster))
                    continue;

                Unit* casterUnit = caster->ToUnit();

                bool interruptedSpell = false;

                for (WorldObject* target : targets)
                {
                    if (!IsUnit(target))
                        continue;

                    if (!(p_ScriptHolder.action.crossCast.castFlags & SMARTCAST_AURA_NOT_PRESENT) || !target->ToUnit()->HasAura(p_ScriptHolder.action.crossCast.spell))
                    {
                        if (!interruptedSpell && p_ScriptHolder.action.crossCast.castFlags & SMARTCAST_INTERRUPT_PREVIOUS)
                        {
                            casterUnit->InterruptNonMeleeSpells(false);
                            interruptedSpell = true;
                        }

                        casterUnit->CastSpell(target->ToUnit(), p_ScriptHolder.action.crossCast.spell, (p_ScriptHolder.action.crossCast.castFlags & SMARTCAST_TRIGGERED) != 0);
                    }
                }
            }
            break;
        }

        case SMART_ACTION_CALL_RANDOM_TIMED_ACTIONLIST:
        {
            std::vector<uint32> actionLists;
            std::copy_if(std::begin(p_ScriptHolder.action.randTimedActionList.actionLists), std::end(p_ScriptHolder.action.randTimedActionList.actionLists),
                std::back_inserter(actionLists), [](uint32 actionList) { return actionList != 0; });

            uint32 id = Trinity::Containers::SelectRandomContainerElement(actionLists);
            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_NONE)
            {
                TC_LOG_ERROR("sql.sql", "SmartScript: Entry %d SourceType %u Event %u Action %u is using TARGET_NONE(0) for Script9 target. Please correct target_type in database.", p_ScriptHolder.entryOrGuid, p_ScriptHolder.GetScriptType(), p_ScriptHolder.GetEventType(), p_ScriptHolder.GetActionType());
                break;
            }

            for (WorldObject* target : targets)
            {
                if (Creature* creature = target->ToCreature())
                {
                    if (IsSmartAI(creature))
                        ENSURE_AI(SmartAI, creature->AI())->SetTimedActionList(p_ScriptHolder, id, GetLastInvoker());
                }
                else if (GameObject* goTarget = target->ToGameObject())
                {
                    if (IsSmartAI(goTarget))
                        ENSURE_AI(SmartGameObjectAI, goTarget->AI())->SetTimedActionList(p_ScriptHolder, id, GetLastInvoker());
                }
            }
            break;
        }

        case SMART_ACTION_CALL_RANDOM_RANGE_TIMED_ACTIONLIST:
        {
            uint32 id = urand(p_ScriptHolder.action.randTimedActionList.actionLists[0], p_ScriptHolder.action.randTimedActionList.actionLists[1]);
            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_NONE)
            {
                TC_LOG_ERROR("sql.sql", "SmartScript: Entry %d SourceType %u Event %u Action %u is using TARGET_NONE(0) for Script9 target. Please correct target_type in database.", p_ScriptHolder.entryOrGuid, p_ScriptHolder.GetScriptType(), p_ScriptHolder.GetEventType(), p_ScriptHolder.GetActionType());
                break;
            }

            for (WorldObject* target : targets)
            {
                if (Creature* creature = target->ToCreature())
                {
                    if (IsSmartAI(creature))
                        ENSURE_AI(SmartAI, creature->AI())->SetTimedActionList(p_ScriptHolder, id, GetLastInvoker());
                }
                else if (GameObject* goTarget = target->ToGameObject())
                {
                    if (IsSmartAI(goTarget))
                        ENSURE_AI(SmartGameObjectAI, goTarget->AI())->SetTimedActionList(p_ScriptHolder, id, GetLastInvoker());
                }
            }
            break;
        }

        case SMART_ACTION_ACTIVATE_TAXI:
        {
            for (WorldObject* target : targets)
                if (IsPlayer(target))
                    target->ToPlayer()->ActivateTaxiPathTo(p_ScriptHolder.action.taxi.id);
            break;
        }

        case SMART_ACTION_RANDOM_MOVE:
        {
            for (WorldObject* target : targets)
            {
                if (IsCreature(target))
                {
                    Creature* l_Creature = target->ToCreature();

                    uint32 l_WalkChance = p_ScriptHolder.action.moveRandom.walkChance;

                    bool l_Run = l_WalkChance == 0;
                    bool l_SkipWalkChance = p_ScriptHolder.action.moveRandom.skipWalkChance == 100;

                    if (!l_Run)
                        l_Run = urand(0, l_WalkChance);

                    if (p_ScriptHolder.action.moveRandom.distance)
                    {
                        l_Creature->StopMoving();
                        l_Creature->GetMotionMaster()->Clear();
                        l_Creature->GetMotionMaster()->MoveRandom((float)p_ScriptHolder.action.moveRandom.distance, l_Run, l_SkipWalkChance);
                    }
                    else
                        l_Creature->GetMotionMaster()->MoveIdle();
                }
            }
            break;
        }

        case SMART_ACTION_SET_UNIT_FIELD_ANIM_TIER:
        {
            for (WorldObject* target : targets)
                if (IsUnit(target))
                    target->ToUnit()->SetByteFlag(UNIT_FIELD_ANIM_TIER, p_ScriptHolder.action.setunitByte.type, p_ScriptHolder.action.setunitByte.byte1);
            break;
        }

        case SMART_ACTION_REMOVE_UNIT_FIELD_ANIM_TIER:
        {
            for (WorldObject* target : targets)
                if (IsUnit(target))
                    target->ToUnit()->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, p_ScriptHolder.action.delunitByte.type, p_ScriptHolder.action.delunitByte.byte1);
            break;
        }

        case SMART_ACTION_INTERRUPT_SPELL:
        {
            for (WorldObject* target : targets)
                if (IsUnit(target))
                    target->ToUnit()->InterruptNonMeleeSpells(p_ScriptHolder.action.interruptSpellCasting.withDelayed != 0, p_ScriptHolder.action.interruptSpellCasting.spell_id, p_ScriptHolder.action.interruptSpellCasting.withInstant != 0);
            break;
        }

        case SMART_ACTION_SEND_GO_CUSTOM_ANIM:
        {
            for (WorldObject* target : targets)
                if (IsGameObject(target))
                    target->ToGameObject()->SendCustomAnim(p_ScriptHolder.action.sendGoCustomAnim.anim);
            break;
        }

        case SMART_ACTION_SET_STAND_STATE:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    me->SetByteValue(UNIT_FIELD_ANIM_TIER, 0, p_ScriptHolder.action.raw.param1);

            break;
        }

        case SMART_ACTION_APPLY_STAND_FLAGS:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    me->SetByteValue(UNIT_FIELD_ANIM_TIER, 1, p_ScriptHolder.action.raw.param1);

            break;
        }

        case SMART_ACTION_APPLY_STAND_MISC_FLAGS:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    me->SetByteValue(UNIT_FIELD_ANIM_TIER, 2, p_ScriptHolder.action.raw.param1);

            break;
        }

        case SMART_ACTION_SET_DYNAMIC_FLAG:
        {
            for (WorldObject* target : targets)
                if (IsUnit(target))
                    target->ToUnit()->SetUInt32Value(UNIT_DYNFLAG_NONE, p_ScriptHolder.action.unitFlag.flag);

            break;
        }

        case SMART_ACTION_ADD_DYNAMIC_FLAG:
        {
            for (WorldObject* target : targets)
                if (IsUnit(target))
                    target->ToUnit()->SetFlag(UNIT_DYNFLAG_NONE, p_ScriptHolder.action.unitFlag.flag);

            break;
        }

        case SMART_ACTION_REMOVE_DYNAMIC_FLAG:
        {
            for (WorldObject* target : targets)
                if (IsUnit(target))
                    target->ToUnit()->RemoveFlag(UNIT_DYNFLAG_NONE, p_ScriptHolder.action.unitFlag.flag);

            break;
        }

        case SMART_ACTION_JUMP_TO_POS:
        {
            if (!targets.empty())
            {
                for (WorldObject* target : targets)
                    if (Creature* creature = target->ToCreature())
                        creature->GetMotionMaster()->MoveJump(Position(p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z), float(p_ScriptHolder.action.jump.speedxy), float(p_ScriptHolder.action.jump.speedz), p_ScriptHolder.action.jump.Id); // @todo add optional jump orientation support?
            }
            else
            {
                if (me)
                    me->GetMotionMaster()->MoveJump(Position(p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z), float(p_ScriptHolder.action.jump.speedxy), float(p_ScriptHolder.action.jump.speedz), p_ScriptHolder.action.jump.Id); // @todo add optional jump orientation support?
            }

            break;
        }

        case SMART_ACTION_GO_SET_LOOT_STATE:
        {
            if (!targets.empty())
            {
                for (WorldObject* target : targets)
                {
                    if (IsGameObject(target))
                        target->ToGameObject()->SetLootState((LootState)p_ScriptHolder.action.setGoLootState.state);
                }
            }
            else
            {
                if (go)
                    go->SetLootState((LootState)p_ScriptHolder.action.setGoLootState.state);
            }
            break;
        }

        case SMART_ACTION_SEND_TARGET_TO_TARGET:
        {
            WorldObject* ref = GetBaseObject();
            if (!ref)
                ref = unit;

            if (!ref)
                break;

            ObjectVector const* storedTargets = GetStoredTargetVector(p_ScriptHolder.action.sendTargetToTarget.id, *ref);
            if (!storedTargets)
                break;

            for (WorldObject* target : targets)
            {
                if (IsCreature(target))
                {
                    if (SmartAI* ai = CAST_AI(SmartAI, target->ToCreature()->AI()))
                        ai->GetScript()->StoreTargetVector(ObjectVector(*storedTargets), p_ScriptHolder.action.sendTargetToTarget.id);   // store a copy of target list
                    else
                        TC_LOG_ERROR("sql.sql", "SmartScript: Action target for SMART_ACTION_SEND_TARGET_TO_TARGET is not using SmartAI, skipping");
                }
                else if (IsGameObject(target))
                {
                    if (SmartGameObjectAI* ai = CAST_AI(SmartGameObjectAI, target->ToGameObject()->AI()))
                        ai->GetScript()->StoreTargetVector(ObjectVector(*storedTargets), p_ScriptHolder.action.sendTargetToTarget.id);   // store a copy of target list
                    else
                        TC_LOG_ERROR("sql.sql", "SmartScript: Action target for SMART_ACTION_SEND_TARGET_TO_TARGET is not using SmartGameObjectAI, skipping");
                }
            }
            break;
        }

        case SMART_ACTION_SEND_GOSSIP_MENU:
        {
            if (!GetBaseObject())
                break;

            TC_LOG_DEBUG("scripts.ai", "SmartScript::ProcessAction:: SMART_ACTION_SEND_GOSSIP_MENU: gossipMenuId %d, gossipNpcTextId %d",
                p_ScriptHolder.action.sendGossipMenu.gossipMenuId, p_ScriptHolder.action.sendGossipMenu.gossipNpcTextId);

            for (WorldObject* target : targets)
            {
                if (Player* player = target->ToPlayer())
                {
                    if (p_ScriptHolder.action.sendGossipMenu.gossipMenuId)
                        player->PrepareGossipMenu(GetBaseObject(), p_ScriptHolder.action.sendGossipMenu.gossipMenuId, true);
                    else
                        player->PlayerTalkClass->ClearMenus();

                    player->SEND_GOSSIP_MENU(p_ScriptHolder.action.sendGossipMenu.gossipNpcTextId, GetBaseObject()->GetGUID());
                }
            }
            break;
        }

        case SMART_ACTION_SET_HOME_POS:
        {
            for (WorldObject* target : targets)
            {
                if (IsCreature(target))
                {
                    if (p_ScriptHolder.GetTargetType() == SMART_TARGET_SELF)
                        target->ToCreature()->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                    else if (p_ScriptHolder.GetTargetType() == SMART_TARGET_POSITION)
                        target->ToCreature()->SetHomePosition(p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z, p_ScriptHolder.target.o);
                    else if (p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_RANGE || p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_GUID ||
                        p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_DISTANCE || p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_RANGE ||
                        p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_GUID || p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_DISTANCE ||
                        p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_CREATURE || p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_GAMEOBJECT ||
                        p_ScriptHolder.GetTargetType() == SMART_TARGET_OWNER_OR_SUMMONER || p_ScriptHolder.GetTargetType() == SMART_TARGET_ACTION_INVOKER ||
                        p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_ENEMY || p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_FRIENDLY)
                    {
                        target->ToCreature()->SetHomePosition(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation());
                    }
                    else
                        TC_LOG_ERROR("sql.sql", "SmartScript: Action target for SMART_ACTION_SET_HOME_POS is invalid, skipping");
                }
            }
            break;
        }

        case SMART_ACTION_SET_HEALTH_REGEN:
        {
            for (WorldObject* target : targets)
                if (IsCreature(target))
                    target->ToCreature()->setRegeneratingHealth(p_ScriptHolder.action.setHealthRegen.regenHealth != 0);
            break;
        }

        case SMART_ACTION_SEND_QUEST_EVENT:
        {
            WorldObject* l_BaseObject = GetBaseObject();
            if (l_BaseObject)
                break;

            for (WorldObject* target : targets)
                if (Player* player = target->ToPlayer())
                    player->SendQuestEvent(p_ScriptHolder.action.getQuestEventId.questId, l_BaseObject->GetGUID());

            break;
        }

        case SMART_ACTION_MOVE_TO_POS_FIXED_ORIENTATION:
        {
            if (!me)
                break;

            WorldObject* target = NULL;
            float x, y, z;

            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_RANGE || p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_GUID ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_CREATURE_DISTANCE || p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_RANGE ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_GUID || p_ScriptHolder.GetTargetType() == SMART_TARGET_GAMEOBJECT_DISTANCE ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_CREATURE || p_ScriptHolder.GetTargetType() == SMART_TARGET_CLOSEST_GAMEOBJECT ||
                p_ScriptHolder.GetTargetType() == SMART_TARGET_OWNER_OR_SUMMONER || p_ScriptHolder.GetTargetType() == SMART_TARGET_ACTION_INVOKER)
            {
                target = targets.front();
            }

            x = target ? target->GetPositionX() : p_ScriptHolder.target.x;
            y = target ? target->GetPositionY() : p_ScriptHolder.target.y;
            z = target ? target->GetPositionZ() : p_ScriptHolder.target.z;

            me->GetMotionMaster()->MovePoint(p_ScriptHolder.action.MoveToPos.pointId, x, y, z, true, 0.0f, true);

            break;
        }

        case SMART_ACTION_ATTACK_STOP:
        {
            WorldObject* l_Front = targets.front();

            if (l_Front && l_Front->ToUnit())
                l_Front->ToUnit()->CombatStop(true);

            break;
        }

        case SMART_ACTION_IGNORE_PATHFINDING:
        {
            if (!me)
                break;

            if (p_ScriptHolder.action.setIgnorePathfinding.onOff)
                me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            else
                me->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);

            break;
        }

        case SMART_ACTION_MOVE_VERTICALLY:
        {
            if (!me)
                break;

            float x, y, z;
            x = me->GetPositionX();
            y = me->GetPositionY();
            z = me->GetPositionZ();

            if (p_ScriptHolder.action.moveVertically.upDown)
                y += p_ScriptHolder.action.moveVertically.distance;
            else
                y -= p_ScriptHolder.action.moveVertically.distance;

            me->GetClosePoint(x, y, z, me->GetObjectSize() / 2.5f);

            me->GetMotionMaster()->MovePoint(p_ScriptHolder.action.moveVertically.pointId, x, y, z, true, (float)(p_ScriptHolder.action.moveVertically.zSpeed / 10 * 7));
            break;
        }

        case SMART_ACTION_MOVE_BY_PATH:
        {
            if (!me)
                break;

            uint32 l_PointId       = p_ScriptHolder.action.moveByPath.pointId;        ///< This is the value we want to movement inform on
            uint32 l_StartWp       = p_ScriptHolder.action.moveByPath.startWp;        ///< Starting waypoint to start
            uint32 l_EndWp         = p_ScriptHolder.action.moveByPath.endWp;          ///< Ending waypoint to end at
            uint32 l_IsCatmull     = p_ScriptHolder.action.moveByPath.isCatmull;      ///< Use smooth waypoints
            uint32 l_WaypointEntry = p_ScriptHolder.action.moveByPath.waypointEntry;  ///< waypoints.entry
            uint32 l_IsCyclic      = p_ScriptHolder.action.moveByPath.isCyclic;       ///< Path will repeat

            TC_LOG_DEBUG("smartscript", "SMART_ACTION_MOVE_BY_PATH: PointID: %u, StartWP: %u, EndWP: %u, Catmull: %u, WaypointEntry: %u, Cyclic: %u", l_PointId, l_StartWp, l_EndWp, l_IsCatmull, l_WaypointEntry, l_IsCyclic);

            auto l_WaypointPath = sSmartWaypointMgr->GetPath(l_WaypointEntry);
            if (!l_WaypointPath)
                break;

            std::vector<Position> l_Waypoints;

            if ((l_EndWp && l_StartWp) && l_EndWp > l_StartWp)
            {
                for (uint32 l_I = l_StartWp; l_I <= l_EndWp; ++l_I)
                {
                    auto l_Range = l_WaypointPath->equal_range(l_I);

                    for (auto l_Itr = l_Range.first; l_Itr != l_Range.second; ++l_Itr)
                    {
                        l_Waypoints.push_back(Position(l_Itr->second->x, l_Itr->second->y, l_Itr->second->z));
                    }
                }
            }
            else
            {
                for (WPPath::const_iterator l_Itr = l_WaypointPath->begin(); l_Itr != l_WaypointPath->end(); ++l_Itr)
                {
                    l_Waypoints.push_back(Position(l_Itr->second->x, l_Itr->second->y, l_Itr->second->z));
                }
            }

            me->GetMotionMaster()->MoveSmoothPath(l_PointId, l_Waypoints, false, me->GetUnitMovementFlags() & MOVEMENTFLAG_DISABLE_GRAVITY, l_IsCyclic > 0, l_IsCatmull > 0);

            break;
        }

        case SMART_ACTION_CUSTOM_MOVE_BY_PATH:
        {

            if (!me || !IsSmartAI())
                break;

            Creature* l_Creature = me->ToCreature();

            if (!l_Creature)
                break;

            auto smartAI = static_cast<SmartAI*>(l_Creature->AI());
            auto path = sCustomWaypointMgr->GetPath(p_ScriptHolder.action.moveByPathCustom.customWaypointEntry);
            if (!path || !smartAI)
                break;

            auto passPath = const_cast<CustomWaypointPath*>(path);
            smartAI->StartCustomPath(p_ScriptHolder.action.moveByPathCustom.customWaypointEntry, p_ScriptHolder.action.moveByPathCustom.run, p_ScriptHolder.action.moveByPathCustom.repeat,
                p_ScriptHolder.action.moveByPathCustom.isCyclic, p_ScriptHolder.action.moveByPathCustom.isCatmull, passPath);
            break;
        }


        case SMART_ACTION_CUSTOM_MOVE_BY_PATHTWO:
        {
            if (!me || !IsSmartAI())
                break;

            Creature* l_Creature = me->ToCreature();

            if (!l_Creature)
                break;

            auto smartAI = static_cast<SmartAI*>(l_Creature->AI());
            auto point = sSmartWaypointMgr->GetPath(p_ScriptHolder.action.moveByPath.waypointEntry);
            if (!point || !smartAI)
                break;

            smartAI->StartPath(true, p_ScriptHolder.action.moveByPath.waypointEntry, true, nullptr, p_ScriptHolder.action.moveByPath.isCyclic, p_ScriptHolder.action.moveByPath.isCatmull, EFFECT_MOTION_TYPE);
            break;
        }

        case SMART_ACTION_SET_SPEED:
        {
            UnitMoveType l_SpeedType = static_cast<UnitMoveType>(p_ScriptHolder.action.setSpeed.speedType);
            float l_Speed = static_cast<float>(p_ScriptHolder.action.setSpeed.speed) * 0.1f;

            for (WorldObject* l_Target : targets)
            {
                if (Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->SetSpeed(l_SpeedType, l_Speed, true);
            }

            break;
        }

        case SMART_ACTION_SET_CORPSE_DELAY:
        {
            uint32 l_CorpseDelay = p_ScriptHolder.action.setCorpseDelay.corpseDelay;

            for (WorldObject* l_Target : targets)
            {
                if (Creature* l_Creature = l_Target->ToCreature())
                    l_Creature->SetCorpseDelay(l_CorpseDelay);
            }

            break;
        }

        case SMART_ACTION_SET_ANIM_KIT:
        {
            enum KitType
            {
                AI,
                Movement,
                Melee
            };

            uint32 l_AnimId = p_ScriptHolder.action.setAnimKit.animId;

            for (WorldObject* l_Target : targets)
            {
                /// @TODO FOR CATA - Anim kits is sent in movement update of SMSG_UPDATE_OBJECT
                /// It needs to not be in unit class.. (so gameobjects can use it)
                if (!l_Target->ToUnit())
                    continue;

                switch (p_ScriptHolder.action.setAnimKit.type)
                {
                    case AI:
                        l_Target->ToUnit()->SetAIAnimKitId(l_AnimId);
                        break;

                    case Movement:
                        l_Target->ToUnit()->SetMovementAnimKitId(l_AnimId);
                        break;

                    case Melee:
                        l_Target->ToUnit()->SetMeleeAnimKitId(l_AnimId);
                        break;

                    default:
                        TC_LOG_ERROR("scripts.ai", "Smart script has wrong type for setanimId[0-2]");
                        break;
                }
            }

            break;
        }

        case SMART_ACTION_MOVE_FALL:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->GetMotionMaster()->MoveFall(p_ScriptHolder.action.moveFall.id);
            break;
        }

        case SMART_ACTION_DISABLE_TURN:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->RemoveUnitMovementFlag(MOVEMENTFLAG_MASK_TURNING);
            break;
        }

        case SMART_ACTION_HANDLE_DOOR:
        {
            if (targets.empty())
                return;

            GameObject* l_GameObject = targets.front() ? targets.front()->ToGameObject() : go;

            if (!l_GameObject)
                break;

            bool openDoor = p_ScriptHolder.action.handleDoor.state;

            l_GameObject->m_events.AddEvent([l_GameObject, openDoor, this](uint64, uint32) {
                l_GameObject->SetGoState(openDoor ? GOState::GO_STATE_ACTIVE : GOState::GO_STATE_READY);

            }, l_GameObject->m_events.CalculateTime(p_ScriptHolder.action.handleDoor.seconds * IN_MILLISECONDS));

            break;
        }

        case SMART_ACTION_PLAY_MUSIC:
        {
            uint32 l_SoundId = p_ScriptHolder.action.playMusic.soundId;
            bool l_OnlySelf = p_ScriptHolder.action.playMusic.onlySelf > 0;

            for (WorldObject* l_Target : targets)
            {
                if (l_OnlySelf && l_Target->ToPlayer())
                    l_Target->PlayDirectSound(l_SoundId, l_Target->ToPlayer());
                else
                    l_Target->PlayDirectSound(l_SoundId);
            }

            break;
        }

        case SMART_ACTION_MOVE_CIRCLE:
        {
            if (!me)
                break;

            WorldObject* l_MoveAround = me ? me : unit;

            uint32 l_Radius = urand(p_ScriptHolder.action.moveCircle.minDistance, p_ScriptHolder.action.moveCircle.maxDistance);
            bool l_ClockWise = p_ScriptHolder.action.moveCircle.clockwise > 0;
            uint32 l_StepAmount = p_ScriptHolder.action.moveCircle.stepAmount;
            bool l_UseTargetAsCenter = p_ScriptHolder.action.moveCircle.UseTargetAsCenter > 0;

            if (p_ScriptHolder.GetTargetType() == SMART_TARGET_POSITION)
            {
                Position l_Pos(p_ScriptHolder.target.x, p_ScriptHolder.target.y, p_ScriptHolder.target.z, p_ScriptHolder.target.o);

                me->GetMotionMaster()->MoveAroundPoint(l_Pos, l_Radius, l_ClockWise, l_StepAmount, 0.0f, false);
            }
            else
            {
                WorldObject* l_CenterTarget = me;

                if (l_UseTargetAsCenter && !targets.empty())
                    l_CenterTarget = targets.front();

                me->GetMotionMaster()->MoveAroundPoint(*l_CenterTarget, l_Radius, l_ClockWise, l_StepAmount, 0.0f, false);
            }
            break;
        }

        case SMART_ACTION_THREAT_SINGLE:
        {
            if (!me)
                break;

            float l_Increase = static_cast<float>(p_ScriptHolder.action.threatSingle.increase);
            float l_Decrease = static_cast<float>(p_ScriptHolder.action.threatSingle.decrease);

            for (WorldObject* l_Target : targets)
            {
                Unit* l_Unit = l_Target->ToUnit();
                if (!l_Unit)
                    continue;

                if (l_Increase)
                    me->AddThreat(l_Unit, l_Increase);

                if (l_Decrease)
                    me->AddThreat(l_Unit, -l_Decrease);
            }
            break;
        }

        case SMART_ACTION_IGNORE_EVADE:
        {
            if (!me || !IsSmartAI())
                break;

            if(Creature* l_Creature = me->ToCreature())
                l_Creature->AI()->IgnoreEvade(p_ScriptHolder.action.ignoreEvade.onOff >= 1);
            break;
        }

        case SMART_ACTION_CAST_RANDOM:
        {
            if (!me)
                break;

            std::vector<uint32> l_Spells;
            std::copy_if(std::begin(p_ScriptHolder.action.castRandom.spells), std::end(p_ScriptHolder.action.castRandom.spells),
                std::back_inserter(l_Spells), [](uint32 p_SpellId) { return p_SpellId != 0; });

            uint32 l_RandomId = Trinity::Containers::SelectRandomContainerElement(l_Spells);
            
            for (WorldObject* l_Target : targets)
            {
                if (Unit* l_Unit = l_Target->ToUnit())
                    me->CastSpell(l_Unit, l_RandomId);
            }
            break;
        }

        case SMART_ACTION_PLAY_ONE_SHOT_ANIM_KIT:
        {
            for (WorldObject* l_Target : targets)
                if(Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->PlayOneShotAnimKit(p_ScriptHolder.action.playAnimKit.id);

            break;
        }

        case SMART_ACTION_PLAY_SPELL_VISUAL_KIT:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->SendPlaySpellVisualKit(p_ScriptHolder.action.playSpellVisualKit.id, p_ScriptHolder.action.playSpellVisualKit.type, p_ScriptHolder.action.playSpellVisualKit.duration);

            break;
        }

        case SMART_ACTION_PLAY_SPELL_VISUAL:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->SendPlaySpellVisual(p_ScriptHolder.action.playSpellVisual.id, unit, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());

            break;
        }

        case SMART_ACTION_PREVENT_EVENT_PHASE_RESET:
        {
            m_ShouldPreventEventPhaseReset = p_ScriptHolder.action.raw.param1 == 1;

            break;
        }

        case SMART_ACTION_SET_VEHICLE_ID:
        {
            if (!me)
                break;

            Creature* l_Creature = me->ToCreature();
            if (!l_Creature)
                break;

            uint32 vehicleId = p_ScriptHolder.action.setVehicleKit.id;

            if (l_Creature->GetVehicleKit())
                l_Creature->RemoveVehicleKit();

            if (!vehicleId)
                vehicleId = l_Creature->GetCreatureTemplate()->VehicleId;

            l_Creature->CreateVehicleKit(vehicleId, 0);
            break;
        }

        case SMART_ACTION_SET_ROOT:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->SetRooted(p_ScriptHolder.action.setRoot.onOff);

            break;
        }

        case SMART_ACTION_SET_GO_FLAG:
        {
            for (WorldObject* l_Target : targets)
                if (GameObject* l_GameObject = l_Target->ToGameObject())
                    l_GameObject->SetUInt32Value(GAMEOBJECT_FLAGS, p_ScriptHolder.action.goFlag.flag);

            break;
        }
        case SMART_ACTION_ADD_GO_FLAG:
        {
            for (WorldObject* l_Target : targets)
                if (GameObject* l_GameObject = l_Target->ToGameObject())
                    l_GameObject->SetFlag(GAMEOBJECT_FLAGS, p_ScriptHolder.action.goFlag.flag);

            break;
        }
        case SMART_ACTION_REMOVE_GO_FLAG:
        {
            for (WorldObject* l_Target : targets)
                if (GameObject* l_GameObject = l_Target->ToGameObject())
                    l_GameObject->RemoveFlag(GAMEOBJECT_FLAGS, p_ScriptHolder.action.goFlag.flag);

            break;
        }

        case SMART_ACTION_SUMMON_CREATURE_GROUP:
        {
            std::list<TempSummon*> summonList;
            GetBaseObject()->SummonCreatureGroup(p_ScriptHolder.action.creatureGroup.group, &summonList);

            for (std::list<TempSummon*>::const_iterator itr = summonList.begin(); itr != summonList.end(); ++itr)
                if (unit && p_ScriptHolder.action.creatureGroup.attackInvoker)
                    (*itr)->AI()->AttackStart(unit);

            break;
        }

        case SMART_ACTION_SET_POWER:
        {
            for (WorldObject* l_Target : targets)
                if (IsUnit(l_Target))
                    l_Target->ToUnit()->SetPower(Powers(p_ScriptHolder.action.power.powerType), p_ScriptHolder.action.power.newPower);

            break;
        }

        case SMART_ACTION_ADD_POWER:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->SetPower(Powers(p_ScriptHolder.action.power.powerType), l_Unit->GetPower(Powers(p_ScriptHolder.action.power.powerType)) + p_ScriptHolder.action.power.newPower);

            break;
        }
        case SMART_ACTION_REMOVE_POWER:
        {
            for (WorldObject* l_Target : targets)
                if (Unit* l_Unit = l_Target->ToUnit())
                    l_Unit->SetPower(Powers(p_ScriptHolder.action.power.powerType), l_Unit->GetPower(Powers(p_ScriptHolder.action.power.powerType)) - p_ScriptHolder.action.power.newPower);

            break;
        }

        case SMART_ACTION_START_CLOSEST_WAYPOINT:
        {
            std::vector<uint32> waypoints;
            std::copy_if(std::begin(p_ScriptHolder.action.closestWaypointFromList.wps), std::end(p_ScriptHolder.action.closestWaypointFromList.wps),
                std::back_inserter(waypoints), [](uint32 wp) { return wp != 0; });

            float distanceToClosest = std::numeric_limits<float>::max();
            WayPoint* closestWp = nullptr;

            for (WorldObject* target : targets)
            {
                if (Creature* creature = target->ToCreature())
                {
                    if (IsSmartAI(creature))
                    {
                        for (uint32 pathId : waypoints)
                        {
                            WPPath const* path = sSmartWaypointMgr->GetPath(pathId);
                            if (!path || path->empty())
                                continue;

                            WPPath::const_iterator itrWp = path->find(0);

                            if (itrWp != path->end())
                            {
                                if (WayPoint* wp = itrWp->second)
                                {
                                    float distToThisPath = target->GetDistance(wp->x, wp->y, wp->z);

                                    if (distToThisPath < distanceToClosest)
                                    {
                                        distanceToClosest = distToThisPath;
                                        closestWp = wp;
                                    }
                                }
                            }
                        }

                        if (closestWp)
                            CAST_AI(SmartAI, creature->AI())->StartPath(false, closestWp->id, true);
                    }
                }
            }
            break;
        }

        case SMART_ACTION_PLAY_RANDOM_SOUND:
        {
            std::vector<uint32> sounds;
            std::copy_if(std::begin(p_ScriptHolder.action.randomSound.sounds), std::end(p_ScriptHolder.action.randomSound.sounds),
                std::back_inserter(sounds), [](uint32 sound) { return sound != 0; });

            bool onlySelf = p_ScriptHolder.action.randomSound.onlySelf != 0;
            for (WorldObject* const target : targets)
            {
                if (IsUnit(target))
                {
                    uint32 sound = Trinity::Containers::SelectRandomContainerElement(sounds);

                    if (p_ScriptHolder.action.randomSound.distance == 1)
                        target->PlayDistanceSound(sound, onlySelf ? target->ToPlayer() : nullptr);
                    else
                        target->PlayDirectSound(sound, onlySelf ? target->ToPlayer() : nullptr);
                }
            }
            break;
        }

        case SMART_ACTION_DISABLE_EVADE:
        {
            if (!IsSmartAI())
                break;

            if(Creature* l_Creature = me->ToCreature())
                ENSURE_AI(SmartAI, l_Creature->AI())->SetEvadeDisabled(p_ScriptHolder.action.disableEvade.disable != 0);
            break;
        }

        case SMART_ACTION_PLAYER_TALK:
            /// @TODO: Find what this means
            break;

        case SMART_ACTION_STOP_MOVEMENT:

            for (WorldObject* target : targets)
            {
                if (Unit* l_Unit = target->ToUnit())
                {
                    switch (p_ScriptHolder.action.stopMovement.MovementType)
                    {
                        case 0:
                            unit->GetMotionMaster()->Clear();
                            break;
                        default:
                            unit->StopMoving();
                            break;
                    }

                    if (p_ScriptHolder.action.stopMovement.SetIdle)
                        unit->GetMotionMaster()->MoveIdle();
                }
            }

            break;

        case SMART_ACTION_REMOVE_AURAS_BY_TYPE:

            for (WorldObject* target : targets)
                if (Unit* l_Unit = target->ToUnit())
                    l_Unit->RemoveAurasByType(AuraType(p_ScriptHolder.action.removeAurasByType.AuraTypeId));

            break;

        case SMART_ACTION_OVERWRITE_INVOKER:

            if (unit)
                mLastInvoker = unit->GetGUID();

            break;

        //////////////////////////////////////////////
        /// AZENOR CUSTOM
        //////////////////////////////////////////////
        case SMART_ACTION_PAUSE_WAYPOINT_FOR_MS:
        {
            ///WorldObject* l_BaseObject = GetBaseObject();
            ///
            ////// Make sure we have base object
            ///if (!l_BaseObject)
            ///    break;
            ///
            ////// Only units can move
            ///Unit* l_BaseUnit = l_BaseObject->ToUnit();
            ///if (!l_BaseUnit)
            ///    break;
            ///
            ///l_BaseUnit->AddUnitState(UNIT_STATE_NOT_MOVE);
            ///
            ///if (p_ScriptHolder.action.PauseWaypintForMs.MSTime)
            ///{
            ///    l_BaseUnit->m_Events.AddEventAtOffset([&]()
            ///    {
            ///        l_BaseUnit->ClearUnitState(UNIT_STATE_NOT_MOVE);
            ///    }, std::chrono::milliseconds(p_ScriptHolder.action.PauseWaypintForMs.MSTime));
            ///}

            break;
        }
        
        case SMART_ACTION_REWARD_LFG_DUNGEON:
        {
            if (!me || !me->ToCreature())
                break;

            uint32 Flags = p_ScriptHolder.action.RewardLFGDungeon.Flags;

            enum eFlagsForLFGReward
            {
                AllModes,
                NormalOnly,
                HeroicOnly,
            };

            bool l_Return = false;

            switch (Flags)
            {
                /// Default/All no checks needed
                default:
                case AllModes:
                    break;

                case NormalOnly:
                    if (me->GetMap()->IsHeroic())
                        l_Return = true;
                    break;

                case HeroicOnly:
                    if (!me->GetMap()->IsHeroic())
                        l_Return = false;
                    break;
            }

            if (l_Return)
                break;

            if (InstanceScript* l_Instance = me->GetInstanceScript())
                l_Instance->FinishLfgDungeon(me->ToCreature());

            break;
        }

        case SMART_ACTION_DO_DIRECT_AI_TALK:
        {
            if (!me)
                break;

            for (WorldObject* l_Target : targets)
            {
                Creature* l_Creature = l_Target->ToCreature();
                if (!l_Creature)
                    continue;

                ObjectGuid l_WhisperGuid = p_ScriptHolder.action.DirectAITalk.UseSourceGUID ? me->GetGUID() : 0;

                l_Creature->AI()->Talk(p_ScriptHolder.action.DirectAITalk.GroupId, l_WhisperGuid);
            }

            break;
        }

        default:
            TC_LOG_ERROR("sql.sql", "SmartScript::ProcessAction: Entry %d SourceType %u, Event %u, Unhandled Action type %u", p_ScriptHolder.entryOrGuid, p_ScriptHolder.GetScriptType(), p_ScriptHolder.event_id, p_ScriptHolder.GetActionType());
            break;
    }

    if (p_ScriptHolder.link && p_ScriptHolder.link != p_ScriptHolder.event_id)
    {
        SmartScriptHolder linked = FindLinkedEvent(p_ScriptHolder.link);
        if (linked.GetActionType() && linked.GetEventType() == SMART_EVENT_LINK)
            ProcessEvent(linked, unit, var0, var1, bvar, p_SpellInfo, p_GameObject);
        else
            TC_LOG_ERROR("sql.sql", "SmartScript::ProcessAction: Entry %d SourceType %u, Event %u, Link Event %u not found or invalid, skipped.", p_ScriptHolder.entryOrGuid, p_ScriptHolder.GetScriptType(), p_ScriptHolder.event_id, p_ScriptHolder.link);
    }
}

void SmartScript::ProcessTimedAction(SmartScriptHolder& e, uint32 const& min, uint32 const& max, Unit* unit, uint32 var0, uint32 var1, bool bvar, const SpellInfo* spell, GameObject* p_GameObject)
{
    ConditionList const conds = sConditionMgr->GetConditionsForSmartEvent(e.entryOrGuid, e.event_id, e.source_type);
    ConditionSourceInfo info = ConditionSourceInfo(unit, GetBaseObject());

    if (sConditionMgr->IsObjectMeetToConditions(info, conds))
        ProcessAction(e, unit, var0, var1, bvar, spell, p_GameObject);

    RecalcTimer(e, min, max);
}

void SmartScript::InstallTemplate(SmartScriptHolder const& e)
{
    if (!GetBaseObject())
        return;
    if (mTemplate)
    {
        TC_LOG_ERROR("sql.sql", "SmartScript::InstallTemplate: Entry %d SourceType %u AI Template can not be set more then once, skipped.", e.entryOrGuid, e.GetScriptType());
        return;
    }
    mTemplate = (SMARTAI_TEMPLATE)e.action.installTtemplate.id;
    switch ((SMARTAI_TEMPLATE)e.action.installTtemplate.id)
    {
        case SMARTAI_TEMPLATE_CASTER:
            {
                AddEvent(SMART_EVENT_UPDATE_IC, 0, 0, 0, e.action.installTtemplate.param2, e.action.installTtemplate.param3, 0, SMART_ACTION_CAST, e.action.installTtemplate.param1, e.target.raw.param1, 0, 0, 0, 0, SMART_TARGET_VICTIM, 0, 0, 0, 1);
                AddEvent(SMART_EVENT_RANGE, 0, e.action.installTtemplate.param4, 300, 0, 0, 0, SMART_ACTION_ALLOW_COMBAT_MOVEMENT, 1, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 1);
                AddEvent(SMART_EVENT_RANGE, 0, 0, e.action.installTtemplate.param4>10?e.action.installTtemplate.param4-10:0, 0, 0, 0, SMART_ACTION_ALLOW_COMBAT_MOVEMENT, 0, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 1);
                AddEvent(SMART_EVENT_MANA_PCT, 0, e.action.installTtemplate.param5-15>100?100:e.action.installTtemplate.param5+15, 100, 1000, 1000, 0, SMART_ACTION_SET_EVENT_PHASE, 1, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 0);
                AddEvent(SMART_EVENT_MANA_PCT, 0, 0, e.action.installTtemplate.param5, 1000, 1000, 0, SMART_ACTION_SET_EVENT_PHASE, 0, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 0);
                AddEvent(SMART_EVENT_MANA_PCT, 0, 0, e.action.installTtemplate.param5, 1000, 1000, 0, SMART_ACTION_ALLOW_COMBAT_MOVEMENT, 1, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 0);
                break;
            }
        case SMARTAI_TEMPLATE_TURRET:
            {
                AddEvent(SMART_EVENT_UPDATE_IC, 0, 0, 0, e.action.installTtemplate.param2, e.action.installTtemplate.param3, 0, SMART_ACTION_CAST, e.action.installTtemplate.param1, e.target.raw.param1, 0, 0, 0, 0, SMART_TARGET_VICTIM, 0, 0, 0, 0);
                AddEvent(SMART_EVENT_JUST_CREATED, 0, 0, 0, 0, 0, 0, SMART_ACTION_ALLOW_COMBAT_MOVEMENT, 0, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 0);
                break;
            }
        case SMARTAI_TEMPLATE_CAGED_NPC_PART:
            {
                if (!me)
                    return;
                //store cage as id1
                AddEvent(SMART_EVENT_DATA_SET, 0, 0, 0, 0, 0, 0, SMART_ACTION_STORE_TARGET_LIST, 1, 0, 0, 0, 0, 0, SMART_TARGET_CLOSEST_GAMEOBJECT, e.action.installTtemplate.param1, 10, 0, 0);

                 //reset(close) cage on hostage(me) respawn
                AddEvent(SMART_EVENT_UPDATE, SMART_EVENT_FLAG_NOT_REPEATABLE, 0, 0, 0, 0, 0, SMART_ACTION_RESET_GOBJECT, 0, 0, 0, 0, 0, 0, SMART_TARGET_GAMEOBJECT_DISTANCE, e.action.installTtemplate.param1, 5, 0, 0);

                AddEvent(SMART_EVENT_DATA_SET, 0, 0, 0, 0, 0, 0, SMART_ACTION_SET_RUN, e.action.installTtemplate.param3, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 0);
                AddEvent(SMART_EVENT_DATA_SET, 0, 0, 0, 0, 0, 0, SMART_ACTION_SET_EVENT_PHASE, 1, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 0);

                AddEvent(SMART_EVENT_UPDATE, SMART_EVENT_FLAG_NOT_REPEATABLE, 1000, 1000, 0, 0, 0, SMART_ACTION_MOVE_FORWARD, e.action.installTtemplate.param4, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 1);
                 //phase 1: give quest credit on movepoint reached
                AddEvent(SMART_EVENT_MOVEMENTINFORM, 0, POINT_MOTION_TYPE, SMART_RANDOM_POINT, 0, 0, 0, SMART_ACTION_SET_DATA, 0, 0, 0, 0, 0, 0, SMART_TARGET_STORED, 1, 0, 0, 1);
                //phase 1: despawn after time on movepoint reached
                AddEvent(SMART_EVENT_MOVEMENTINFORM, 0, POINT_MOTION_TYPE, SMART_RANDOM_POINT, 0, 0, 0, SMART_ACTION_FORCE_DESPAWN, e.action.installTtemplate.param2, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 1);

                if (sCreatureTextMgr->TextExist(me->GetEntry(), (uint8)e.action.installTtemplate.param5))
                    AddEvent(SMART_EVENT_MOVEMENTINFORM, 0, POINT_MOTION_TYPE, SMART_RANDOM_POINT, 0, 0, 0, SMART_ACTION_TALK, e.action.installTtemplate.param5, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 1);
                break;
            }
        case SMARTAI_TEMPLATE_CAGED_GO_PART:
            {
                if (!go)
                    return;
                //store hostage as id1
                AddEvent(SMART_EVENT_GO_STATE_CHANGED, 0, 2, 0, 0, 0, 0, SMART_ACTION_STORE_TARGET_LIST, 1, 0, 0, 0, 0, 0, SMART_TARGET_CLOSEST_CREATURE, e.action.installTtemplate.param1, 10, 0, 0);
                //store invoker as id2
                AddEvent(SMART_EVENT_GO_STATE_CHANGED, 0, 2, 0, 0, 0, 0, SMART_ACTION_STORE_TARGET_LIST, 2, 0, 0, 0, 0, 0, SMART_TARGET_NONE, 0, 0, 0, 0);
                //signal hostage
                AddEvent(SMART_EVENT_GO_STATE_CHANGED, 0, 2, 0, 0, 0, 0, SMART_ACTION_SET_DATA, 0, 0, 0, 0, 0, 0, SMART_TARGET_STORED, 1, 0, 0, 0);
                //when hostage raeched end point, give credit to invoker
                if (e.action.installTtemplate.param2)
                    AddEvent(SMART_EVENT_DATA_SET, 0, 0, 0, 0, 0, 0, SMART_ACTION_CALL_KILLEDMONSTER, e.action.installTtemplate.param1, 0, 0, 0, 0, 0, SMART_TARGET_STORED, 2, 0, 0, 0);
                else
                    AddEvent(SMART_EVENT_GO_STATE_CHANGED, 0, 2, 0, 0, 0, 0, SMART_ACTION_CALL_KILLEDMONSTER, e.action.installTtemplate.param1, 0, 0, 0, 0, 0, SMART_TARGET_STORED, 2, 0, 0, 0);
                break;
            }
        case SMARTAI_TEMPLATE_BASIC:
        default:
            return;
    }
}

void SmartScript::AddEvent(SMART_EVENT e, uint32 event_flags, uint32 event_param1, uint32 event_param2, uint32 event_param3, uint32 event_param4, uint32 event_param5, SMART_ACTION action, uint32 action_param1, uint32 action_param2, uint32 action_param3, uint32 action_param4, uint32 action_param5, uint32 action_param6, SMARTAI_TARGETS t, uint32 target_param1, uint32 target_param2, uint32 target_param3, uint32 phaseMask)
{
    mInstallEvents.push_back(CreateSmartEvent(e, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, action, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, t, target_param1, target_param2, target_param3, phaseMask));
}

WorldObject* SmartScript::GetBaseObject() const
{
    if (me)
        return me;

    if (go)
        return go;

    if (m_Source)
        return dynamic_cast<WorldObject*>(m_Source);

    TC_LOG_ERROR("smartai", "SmartScript::GetBaseObject returned nullptr!");
    return nullptr;
}

bool SmartScript::IsUnit(WorldObject* p_Object)
{
    return p_Object && (p_Object->IsCreature() || p_Object->IsPlayer());
}

bool SmartScript::IsPlayer(WorldObject* p_Object)
{
    return p_Object && p_Object->IsPlayer();
}

bool SmartScript::IsCreature(WorldObject* p_Object)
{
    return p_Object && p_Object->IsCreature();
}

bool SmartScript::IsCharmedCreature(WorldObject* p_Object)
{
    if (!p_Object)
        return false;

    if (Creature* l_Creature = p_Object->ToCreature())
        return l_Creature->isCharmed();

    return false;
}

bool SmartScript::IsGameObject(WorldObject* p_Object)
{
    return p_Object && p_Object->GetTypeId() == TYPEID_GAMEOBJECT;
}

SmartScriptHolder SmartScript::CreateSmartEvent(SMART_EVENT e, uint32 event_flags, uint32 event_param1, uint32 event_param2, uint32 event_param3, uint32 event_param4, uint32 event_param5, SMART_ACTION action, uint32 action_param1, uint32 action_param2, uint32 action_param3, uint32 action_param4, uint32 action_param5, uint32 action_param6, SMARTAI_TARGETS t, uint32 target_param1, uint32 target_param2, uint32 target_param3, uint32 phaseMask)
{
    SmartScriptHolder script;
    script.m_Event.type = e;
    script.m_Event.raw.param1 = event_param1;
    script.m_Event.raw.param2 = event_param2;
    script.m_Event.raw.param3 = event_param3;
    script.m_Event.raw.param4 = event_param4;
    script.m_Event.raw.param5 = event_param5;
    script.m_Event.event_phase_mask = phaseMask;
    script.m_Event.event_flags = event_flags;
    script.m_Event.event_chance = 100;

    script.action.type = action;
    script.action.raw.param1 = action_param1;
    script.action.raw.param2 = action_param2;
    script.action.raw.param3 = action_param3;
    script.action.raw.param4 = action_param4;
    script.action.raw.param5 = action_param5;
    script.action.raw.param6 = action_param6;

    script.target.type = t;
    script.target.raw.param1 = target_param1;
    script.target.raw.param2 = target_param2;
    script.target.raw.param3 = target_param3;

    script.source_type = SMART_SCRIPT_TYPE_CREATURE;
    InitTimer(script);
    return script;
}

void SmartScript::GetTargets(ObjectVector& targets, SmartScriptHolder const& e, Unit* invoker /*= nullptr*/) const
{
    Unit* scriptTrigger = nullptr;
    if (invoker)
        scriptTrigger = invoker;
    else if (Unit* tempLastInvoker = GetLastInvoker())
        scriptTrigger = tempLastInvoker;

    WorldObject* baseObject = GetBaseObject();

    switch (e.GetTargetType())
    {
        case SMART_TARGET_SELF:
            if (baseObject)
                targets.push_back(baseObject);
            break;

        case SMART_TARGET_VICTIM:
            if (me && me->getVictim())
                targets.push_back(me->getVictim());
            break;

        case SMART_TARGET_HOSTILE_SECOND_AGGRO:
            if (me)
                if(Creature* l_Creature = me->ToCreature())
                    if (Unit* u = l_Creature->AI()->SelectTarget(SELECT_TARGET_TOPAGGRO, 1))
                        targets.push_back(u);
            break;

        case SMART_TARGET_HOSTILE_LAST_AGGRO:
            if (me)
                if (Creature* l_Creature = me->ToCreature())
                    if (Unit* u = l_Creature->AI()->SelectTarget(SELECT_TARGET_BOTTOMAGGRO, 0))
                        targets.push_back(u);
            break;

        case SMART_TARGET_HOSTILE_RANDOM:
            if (me)
                if (Creature* l_Creature = me->ToCreature())
                    if (Unit* u = l_Creature->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0))
                        targets.push_back(u);
            break;

        case SMART_TARGET_HOSTILE_RANDOM_NOT_TOP:
            if (me)
                if (Creature* l_Creature = me->ToCreature())
                    if (Unit* u = l_Creature->AI()->SelectTarget(SELECT_TARGET_RANDOM, 1))
                        targets.push_back(u);
            break;

        case SMART_TARGET_NONE:
        case SMART_TARGET_ACTION_INVOKER:
            if (scriptTrigger)
                targets.push_back(scriptTrigger);
            break;

        case SMART_TARGET_ACTION_INVOKER_VEHICLE:
            if (scriptTrigger && scriptTrigger->GetVehicle() && scriptTrigger->GetVehicle()->GetBase())
                targets.push_back(scriptTrigger->GetVehicle()->GetBase());
            break;

        case SMART_TARGET_INVOKER_PARTY:
        {
            if (scriptTrigger)
            {
                if (Player* player = scriptTrigger->ToPlayer())
                {
                    if (Group* group = player->GetGroup())
                    {
                        for (GroupReference* groupRef = group->GetFirstMember(); groupRef != NULL; groupRef = groupRef->next())
                            if (Player* member = groupRef->getSource())
                                targets.push_back(member);
                    }
                    // We still add the player to the list if there is no group. If we do
                    // this even if there is a group (thus the else-check), it will add the
                    // same player to the list twice. We don't want that to happen.
                    else
                        targets.push_back(scriptTrigger);
                }
            }
            break;
        }

        case SMART_TARGET_CREATURE_RANGE:
        {
            ObjectVector units;
            GetWorldObjectsInDist(units, static_cast<float>(e.target.unitRange.maxDist));

            for (WorldObject* unit : units)
            {
                if (!IsCreature(unit))
                    continue;

                if (me && me->GetGUID() == unit->GetGUID())
                    continue;

                if ((!e.target.unitRange.creature || unit->ToCreature()->GetEntry() == e.target.unitRange.creature) && baseObject->IsInRange(unit, float(e.target.unitRange.minDist), float(e.target.unitRange.maxDist)))
                    targets.push_back(unit);
            }

            if (e.target.unitRange.maxSize)
                Trinity::Containers::RandomResize(targets, e.target.unitRange.maxSize);
            break;
        }

        case SMART_TARGET_CREATURE_DISTANCE:
        {
            ObjectVector units;
            GetWorldObjectsInDist(units, static_cast<float>(e.target.unitDistance.dist));

            for (WorldObject* unit : units)
            {
                if (!IsCreature(unit))
                    continue;

                if (me && me->GetGUID() == unit->GetGUID())
                    continue;

                if (!e.target.unitDistance.creature || unit->ToCreature()->GetEntry() == e.target.unitDistance.creature)
                    targets.push_back(unit);
            }

            if (e.target.unitDistance.maxSize)
                Trinity::Containers::RandomResize(targets, e.target.unitDistance.maxSize);

            break;
        }

        case SMART_TARGET_GAMEOBJECT_DISTANCE:
        {
            ObjectVector units;
            GetWorldObjectsInDist(units, static_cast<float>(e.target.goDistance.dist));

            for (WorldObject* unit : units)
            {
                if (!IsGameObject(unit))
                    continue;

                if (go && go->GetGUID() == unit->GetGUID())
                    continue;

                if (!e.target.goDistance.entry || unit->ToGameObject()->GetEntry() == e.target.goDistance.entry)
                    targets.push_back(unit);
            }

            if (e.target.goDistance.maxSize)
                Trinity::Containers::RandomResize(targets, e.target.goDistance.maxSize);
            break;
        }

        case SMART_TARGET_GAMEOBJECT_RANGE:
        {
            ObjectVector units;
            GetWorldObjectsInDist(units, static_cast<float>(e.target.goRange.maxDist));

            for (WorldObject* unit : units)
            {
                if (!IsGameObject(unit))
                    continue;

                if (go && go->GetGUID() == unit->GetGUID())
                    continue;

                if ((!e.target.goRange.entry && unit->ToGameObject()->GetEntry() == e.target.goRange.entry) && baseObject->IsInRange(unit, float(e.target.goRange.minDist), float(e.target.goRange.maxDist)))
                    targets.push_back(unit);
            }

            if (e.target.goRange.maxSize)
                Trinity::Containers::RandomResize(targets, e.target.goRange.maxSize);
            break;
        }

        case SMART_TARGET_CREATURE_GUID:
        {
            Creature* target = NULL;
            if (!scriptTrigger && !baseObject)
            {
                TC_LOG_ERROR("sql.sql", "SMART_TARGET_CREATURE_GUID can not be used without invoker");
                break;
            }

            target = FindCreatureNear(scriptTrigger ? scriptTrigger : baseObject, e.target.unitGUID.dbGuid);

            if (target && (!e.target.unitGUID.entry || target->GetEntry() == e.target.unitGUID.entry))
                targets.push_back(target);
            break;
        }

        case SMART_TARGET_GAMEOBJECT_GUID:
        {
            GameObject* target = NULL;
            if (!scriptTrigger && !baseObject)
            {
                TC_LOG_ERROR("sql.sql", "SMART_TARGET_GAMEOBJECT_GUID can not be used without invoker");
                break;
            }

            target = FindGameObjectNear(scriptTrigger ? scriptTrigger : baseObject, e.target.goGUID.dbGuid);

            if (target && (!e.target.goGUID.entry || target->GetEntry() == e.target.goGUID.entry))
                targets.push_back(target);
            break;
        }

        case SMART_TARGET_PLAYER_RANGE:
        {
            ObjectVector units;
            GetWorldObjectsInDist(units, static_cast<float>(e.target.playerRange.maxDist));

            if (!units.empty() && baseObject)
                for (WorldObject* unit : units)
                    if (IsPlayer(unit) && baseObject->IsInRange(unit, float(e.target.playerRange.minDist), float(e.target.playerRange.maxDist)))
                        targets.push_back(unit);
            break;
        }

        case SMART_TARGET_PLAYER_DISTANCE:
        {
            ObjectVector units;
            GetWorldObjectsInDist(units, static_cast<float>(e.target.playerDistance.dist));

            for (WorldObject* unit : units)
                if (IsPlayer(unit))
                    targets.push_back(unit);
            break;
        }

        case SMART_TARGET_STORED:
        {
            WorldObject* ref = GetBaseObject();
            if (!ref)
                ref = scriptTrigger;

            if (ref)
                if (ObjectVector const* stored = GetStoredTargetVector(e.target.stored.id, *ref))
                    targets.assign(stored->begin(), stored->end());
            break;
        }

        case SMART_TARGET_CLOSEST_CREATURE:
        {
            Creature* target = GetClosestCreatureWithEntry(baseObject, e.target.closest.entry, (float)(e.target.closest.dist ? e.target.closest.dist : 100), e.target.closest.dead ? false : true);
            if (target)
                targets.push_back(target);
            break;
        }

        case SMART_TARGET_CLOSEST_GAMEOBJECT:
        {
            GameObject* target = GetClosestGameObjectWithEntry(baseObject, e.target.closest.entry, (float)(e.target.closest.dist ? e.target.closest.dist : 100));
            if (target)
                targets.push_back(target);
            break;
        }

        case SMART_TARGET_CLOSEST_PLAYER:
        {
            if (me)
            {
                Player* target = me->SelectNearestPlayer((float)e.target.playerDistance.dist);
                if (target)
                    targets.push_back(target);
            }
            break;
        }

        case SMART_TARGET_OWNER_OR_SUMMONER:
        {
            if (me)
            {
                if (Unit* owner = ObjectAccessor::GetUnit(*me, me->GetCharmerOrOwnerGUID()))
                    targets.push_back(owner);
                else if (me->isSummon() && me->ToTempSummon()->GetSummoner())
                    targets.push_back(me->ToTempSummon()->GetSummoner());
            }
            else if (go)
            {
                if (Unit* owner = ObjectAccessor::GetUnit(*go, go->GetOwnerGUID()))
                    targets.push_back(owner);
            }

            if (e.target.owner.UseCharmerOrOwner && !targets.empty())
            {
                Unit* owner = targets.front()->ToUnit();
                targets.clear();

                if (Unit* base = ObjectAccessor::GetUnit(*owner, owner->GetCharmerOrOwnerGUID()))
                    targets.push_back(base);
            }
            break;
        }

        case SMART_TARGET_THREAT_LIST:
        {
            if (me)
            {
                std::list<HostileReference*> const& threatList = me->getThreatManager().getThreatList();
                for (std::list<HostileReference*>::const_iterator i = threatList.begin(); i != threatList.end(); ++i)
                    if (Unit* temp = Unit::GetUnit(*me, (*i)->getUnitGuid()))
                        targets.push_back(temp);
            }
            break;
        }

        case SMART_TARGET_FARTHEST_PLAYER:
            {
                if (me)
                {
                    std::list<Player*> players = me->GetPlayersInRange((float)e.target.playerDistance.dist, true);
                    if (!players.empty())
                    {
                        players.sort(Trinity::ObjectDistanceOrderPred(me));
                        std::list<Player*>::reverse_iterator ritr = players.rbegin();
                        targets.push_back(*ritr);
                    }
                }
                break;
            }

        case SMART_TARGET_INVOKER_SUMMON:
        {
            if (!scriptTrigger)
            {
                TC_LOG_ERROR("sql.sql", "SMART_TARGET_INVOKER_SUMMON can not be used without invoker");
                break;
            }

            for (auto l_SummonGuid : scriptTrigger->m_TempSummons)
            {
                /// Check empty
                if (l_SummonGuid == 0)
                    continue;

                auto l_Controlled = ObjectAccessor::GetUnit(*scriptTrigger, l_SummonGuid);

                if (l_Controlled && l_Controlled->GetEntry() == e.target.invokerSummon.SummonEntry)
                    targets.push_back(l_Controlled);
            }

            break;
        }

        case SMART_TARGET_SPELL_CASTER:
        {
            targets.push_back(me);
            break;
        }

        case SMART_TARGET_SPELL_TARGET:
        {
            WorldObject* l_BaseObject = GetBaseObject();
            if (!l_BaseObject)
                break;

            for (uint64 l_Guid : m_SpellHitTargets)
                if (Unit* l_Unit = ObjectAccessor::GetUnit(*l_BaseObject, l_Guid))
                    targets.push_back(l_Unit);

            if (m_Target)
                targets.push_back((WorldObject*)m_Target);

            break;
        }

		case SMART_TARGET_ANY_IN_RANGE_WITH_CONDITIONS:
		{
            ObjectVector l_Objects;

            GetWorldObjectsInDist(l_Objects, e.target.AnyInRangeWithConditions.MaxRange);
            
            l_Objects.erase(std::remove_if(l_Objects.begin(), l_Objects.end(), [&](const WorldObject* p_Object)
            {
                /// FIXIT!
                ///if (!sConditionMgr->IsObjectMeetingSmartTargetEventConditions(e.entryOrGuid, e.event_id, e.source_type, scriptTrigger, baseObject))
                ///    return false;

                return p_Object->GetDistance(baseObject) < e.target.AnyInRangeWithConditions.MinRange;

            }), l_Objects.end());

            int32 l_Count = e.target.AnyInRangeWithConditions.TargetsCount;

            if (l_Count > l_Objects.size())
                l_Count = l_Objects.size();

            std::random_shuffle(l_Objects.begin(), l_Objects.end());

            for (int l_I = 0; l_I < l_Count; ++l_I)
                targets.emplace_back(l_Objects.at(l_I));

			break;
		}

		case SMART_TARGET_VEHICLE_PASSENGER:
		{
            if (me && me->IsVehicle())
                for (auto seatItr = me->GetVehicleKit()->Seats.begin(); seatItr != me->GetVehicleKit()->Seats.end(); ++seatItr)
                    if (!e.target.vehicle.seat || (e.target.vehicle.seat & (1 << seatItr->first)))
                        if (Unit* u = ObjectAccessor::GetUnit(*me, seatItr->second.Passenger.Guid))
                            targets.push_back(u);

			break;
		}

		case SMART_TARGET_SUMMONER_SUMMON_LIST:
		{
            if (!me)
                break;

            float l_DistanceMax = e.target.summonList.maxDist;
            float l_DistanceMin = e.target.summonList.minDist;

            for (auto l_SummonGuid : me->m_TempSummons)
            {
                /// Check empty
                if (l_SummonGuid == 0)
                    continue;

                auto l_Controlled = ObjectAccessor::GetUnit(*baseObject, l_SummonGuid);

                if (l_Controlled)
                {
                    if (l_Controlled->GetEntry() == e.target.summonList.entry)
                    {
                        float l_DistanceToMe = l_Controlled->GetDistance2d(me);

                        if (l_DistanceToMe < l_DistanceMax && l_DistanceToMe >= l_DistanceMin)
                            targets.push_back(l_Controlled);
                    }
                }
            }

			break;
		}

        case SMART_TARGET_CLOSEST_ENEMY:
        {
            if (me)
                if (Unit* target = me->SelectNearestTarget(e.target.closestAttackable.maxDist, e.target.closestAttackable.playerOnly != 0))
                    targets.push_back(target);

            break;
        }

        case SMART_TARGET_CLOSEST_FRIENDLY:
        {
            if (me)
                if (Unit* target = me->SelectNearestTarget(e.target.closestAttackable.maxDist, e.target.closestAttackable.playerOnly != 0))
                    targets.push_back(target);

            break;
        }

        case SMART_TARGET_LOOT_RECIPIENTS:
        {
            if (me)
            {
                if (Creature* l_Creature = me->ToCreature())
                {
                    if (Group* lootGroup = l_Creature->GetLootRecipientGroup())
                    {
                        for (GroupReference* it = lootGroup->GetFirstMember(); it != nullptr; it = it->next())
                            if (Player* recipient = it->getSource())
                                if (recipient->IsInMap(me))
                                    targets.push_back(recipient);
                    }
                    else
                    {
                        if (Player* recipient = l_Creature->GetLootRecipient())
                            targets.push_back(recipient);
                    }
                }
            }
        }

        case SMART_TARGET_VEHICLE_ACCESSORY:
        {
            if (me && me->IsVehicle())
            {
                if (Unit* target = me->GetVehicleKit()->GetPassenger(e.target.vehicle.seat))
                    targets.push_back(target);
            }
        }

        case SMART_TARGET_POSITION:
        default:
            break;
    }
}

void SmartScript::GetWorldObjectsInDist(ObjectVector& targets, float dist) const
{
    WorldObject* obj = GetBaseObject();
    if (!obj)
        return;

    Trinity::AllWorldObjectsInRange u_check(obj, dist);
    Trinity::WorldObjectListSearcher<Trinity::AllWorldObjectsInRange> searcher(obj, targets, u_check);
    Cell::VisitAllObjects(obj, searcher, dist);
}

void SmartScript::ProcessEvent(SmartScriptHolder& e, Unit* unit, uint32 var0, uint32 var1, bool bvar, const SpellInfo* p_SpellInfo, GameObject* p_GameObject)
{
    if (!e.active && e.GetEventType() != SMART_EVENT_LINK)
        return;

    if ((e.m_Event.event_phase_mask && !IsInPhase(e.m_Event.event_phase_mask)) || ((e.m_Event.event_flags & SMART_EVENT_FLAG_NOT_REPEATABLE) && e.runOnce))
        return;

    /// Artamedes: Bad condition
    /// Breaks like every vehicle SAI...
    ///if (!(e.m_Event.event_flags & SMART_EVENT_FLAG_WHILE_CHARMED) && IsCharmedCreature(me))
    ///{
    ///    TC_LOG_ERROR("sai", "IsCharmedCreature");
    ///    return;
    ///}

    /// Flags that will only activate on SMART_SCRIPT_TYPE_SPELL
    if (mScriptType == SMART_SCRIPT_TYPE_SPELL)
    {
        uint32 l_Flags = e.m_Event.event_flags;

        /// Check our effIndex and our flags
        /// Way it works: if flags contains SMART_EVENT_FLAG_SPELL_EFFECT_X
        /// and our effIndex is not X, don't execute this event :)

        if (l_Flags & SMART_EVENT_FLAG_SPELL_EFFECT_0)
            if (m_EffIndex != EFFECT_0)
                return;

        if (l_Flags & SMART_EVENT_FLAG_SPELL_EFFECT_1)
            if (m_EffIndex != EFFECT_1)
                return;

        if (l_Flags & SMART_EVENT_FLAG_SPELL_EFFECT_2)
            if (m_EffIndex != EFFECT_2)
                return;
    }

    switch (e.GetEventType())
    {
        case SMART_EVENT_LINK://special handling
            ProcessAction(e, unit, var0, var1, bvar, p_SpellInfo, p_GameObject);
            break;
        //called from Update tick
        case SMART_EVENT_UPDATE:
            ProcessTimedAction(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax);
            break;
        case SMART_EVENT_UPDATE_OOC:
            if (me && me->isInCombat())
                return;
            ProcessTimedAction(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax);
            break;
        case SMART_EVENT_UPDATE_IC:
            if (!me || !me->isInCombat())
                return;
            ProcessTimedAction(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax);
            break;
        case SMART_EVENT_HEALT_PCT:
        {
            if (!me || !me->isInCombat() || !me->GetMaxHealth())
                return;
            uint32 perc = (uint32)me->GetHealthPct();
            if (perc > e.m_Event.minMaxRepeat.max || perc < e.m_Event.minMaxRepeat.min)
                return;
            ProcessTimedAction(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax, unit);
            break;
        }
        case SMART_EVENT_TARGET_HEALTH_PCT:
        {
            if (!me || !me->isInCombat() || !me->getVictim() || !me->getVictim()->GetMaxHealth())
                return;
            uint32 perc = (uint32)me->getVictim()->GetHealthPct();
            if (perc > e.m_Event.minMaxRepeat.max || perc < e.m_Event.minMaxRepeat.min)
                return;
            ProcessTimedAction(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax, me->getVictim());
            break;
        }
        case SMART_EVENT_MANA_PCT:
        {
            if (!me || !me->isInCombat() || !me->GetMaxPower(POWER_MANA))
                return;
            uint32 perc = uint32(100.0f * me->GetPower(POWER_MANA) / me->GetMaxPower(POWER_MANA));
            if (perc > e.m_Event.minMaxRepeat.max || perc < e.m_Event.minMaxRepeat.min)
                return;
            ProcessTimedAction(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax);
            break;
        }
        case SMART_EVENT_TARGET_MANA_PCT:
        {
            if (!me || !me->isInCombat() || !me->getVictim() || !me->getVictim()->GetMaxPower(POWER_MANA))
                return;
            uint32 perc = uint32(100.0f * me->getVictim()->GetPower(POWER_MANA) / me->getVictim()->GetMaxPower(POWER_MANA));
            if (perc > e.m_Event.minMaxRepeat.max || perc < e.m_Event.minMaxRepeat.min)
                return;
            ProcessTimedAction(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax, me->getVictim());
            break;
        }
        case SMART_EVENT_RANGE:
        {
            if (!me || !me->isInCombat() || !me->getVictim())
                return;

            if (me->IsInRange(me->getVictim(), (float)e.m_Event.minMaxRepeat.min, (float)e.m_Event.minMaxRepeat.max))
                ProcessTimedAction(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax, me->getVictim());
            break;
        }
        case SMART_EVENT_VICTIM_CASTING:
        {
            if (!me || !me->isInCombat())
                return;

            Unit* victim = me->getVictim();

            if (!victim || !victim->IsNonMeleeSpellCasted(false, false, true))
                return;

            if (e.m_Event.targetCasting.spellId > 0)
                if (Spell* currSpell = victim->GetCurrentSpell(CURRENT_GENERIC_SPELL))
                    if (currSpell->m_spellInfo->Id != e.m_Event.targetCasting.spellId)
                        return;

            ProcessTimedAction(e, e.m_Event.targetCasting.repeatMin, e.m_Event.targetCasting.repeatMax, me->getVictim());
        }
        case SMART_EVENT_FRIENDLY_HEALTH:
        {
            if (!me || !me->isInCombat())
                return;

            Unit* target = DoSelectLowestHpFriendly((float)e.m_Event.friendlyHealth.radius, e.m_Event.friendlyHealth.hpDeficit);
            if (!target)
                return;
            ProcessTimedAction(e, e.m_Event.friendlyHealth.repeatMin, e.m_Event.friendlyHealth.repeatMax, target);
            break;
        }
        case SMART_EVENT_FRIENDLY_IS_CC:
        {
            if (!me || !me->isInCombat())
                return;

            std::vector<Creature*> creatures;
            DoFindFriendlyCC(creatures, float(e.m_Event.friendlyCC.radius));
            if (creatures.empty())
            {
                // if there are at least two same npcs, they will perform the same action immediately even if this is useless...
                RecalcTimer(e, 1000, 3000);
                return;
            }
            ProcessTimedAction(e, e.m_Event.friendlyCC.repeatMin, e.m_Event.friendlyCC.repeatMax, Trinity::Containers::SelectRandomContainerElement(creatures));
            break;
        }
        case SMART_EVENT_FRIENDLY_MISSING_BUFF:
        {
            std::vector<Creature*> l_Creatures;
            DoFindFriendlyMissingBuff(l_Creatures, (float)e.m_Event.missingBuff.radius, e.m_Event.missingBuff.spell);

            if (l_Creatures.empty())
                return;

            ProcessTimedAction(e, e.m_Event.missingBuff.repeatMin, e.m_Event.missingBuff.repeatMax, Trinity::Containers::SelectRandomContainerElement(l_Creatures));
            break;
        }
        case SMART_EVENT_HAS_AURA:
        {
            if (!me)
                return;
            uint32 count = me->GetAuraCount(e.m_Event.aura.spell);
            if ((!e.m_Event.aura.count && !count) || (e.m_Event.aura.count && count >= e.m_Event.aura.count))
                ProcessTimedAction(e, e.m_Event.aura.repeatMin, e.m_Event.aura.repeatMax);
            break;
        }
        case SMART_EVENT_TARGET_BUFFED:
        {
            if (!me || !me->getVictim())
                return;
            uint32 count = me->getVictim()->GetAuraCount(e.m_Event.aura.spell);
            if (count < e.m_Event.aura.count)
                return;
            ProcessTimedAction(e, e.m_Event.aura.repeatMin, e.m_Event.aura.repeatMax);
            break;
        }
        case SMART_EVENT_CHARMED:
        {
            if (bvar == (e.m_Event.charm.onRemove != 1))
                ProcessAction(e, unit, var0, var1, bvar, p_SpellInfo, p_GameObject);
            break;
        }
        
        /// SpellID PARAM!
        case SMART_EVENT_AURA_APPLIED:
        case SMART_EVENT_AURA_REMOVED:
            if(var0 == e.m_Event.AuraAppliedOrRemoved.SpellID)
                ProcessAction(e, unit, var0, var1, bvar, p_SpellInfo, p_GameObject);
            break;

        //no params
        case SMART_EVENT_AGGRO:
        case SMART_EVENT_DEATH:
        case SMART_EVENT_EVADE:
        case SMART_EVENT_REACHED_HOME:
        case SMART_EVENT_CHARMED_TARGET:
        case SMART_EVENT_CORPSE_REMOVED:
        case SMART_EVENT_AI_INIT:
        case SMART_EVENT_AFTER_PASSENGER_REMOVED:
        case SMART_EVENT_VEHICLE_EXIT_REQUSTED:
        case SMART_EVENT_JUST_SUMMONED:
        case SMART_EVENT_RESET:
        case SMART_EVENT_JUST_CREATED:
        case SMART_EVENT_GOSSIP_HELLO:
        case SMART_EVENT_FOLLOW_COMPLETED:
        case SMART_EVENT_ON_SPELLCLICK:
        case SMART_EVENT_QUEST_ACCEPT:
        case SMART_EVENT_QUEST_COMPLETE:
        case SMART_EVENT_QUEST_REWARD:
        case SMART_EVENT_QUEST_FAIL:
        case SMART_EVENT_QUEST_ABANDON:
            ProcessAction(e, unit, var0, var1, bvar, p_SpellInfo, p_GameObject);
            break;
        case SMART_EVENT_IS_BEHIND_TARGET:
            {
                if (!me)
                    return;

                if (Unit* victim = me->getVictim())
                {
                    if (!victim->HasInArc(static_cast<float>(M_PI), me))
                        ProcessTimedAction(e, e.m_Event.behindTarget.cooldownMin, e.m_Event.behindTarget.cooldownMax, victim);
                }
                break;
            }
        case SMART_EVENT_RECEIVE_EMOTE:
            if (e.m_Event.emote.emote == var0)
            {
                ProcessAction(e, unit);
                RecalcTimer(e, e.m_Event.emote.cooldownMin, e.m_Event.emote.cooldownMax);
            }
            break;
        case SMART_EVENT_KILL:
        {
            if (!me || !unit)
                return;
            if (e.m_Event.kill.playerOnly && unit->GetTypeId() != TYPEID_PLAYER)
                return;
            if (e.m_Event.kill.creature && unit->GetEntry() != e.m_Event.kill.creature)
                return;
            ProcessAction(e, unit);
            RecalcTimer(e, e.m_Event.kill.cooldownMin, e.m_Event.kill.cooldownMax);
            break;
        }
        case SMART_EVENT_SPELLHIT_TARGET:
        case SMART_EVENT_SPELLHIT:
        {
            if (!p_SpellInfo)
                return;
            if ((!e.m_Event.spellHit.spell || p_SpellInfo->Id == e.m_Event.spellHit.spell) &&
                (!e.m_Event.spellHit.school || (p_SpellInfo->SchoolMask & e.m_Event.spellHit.school)))
                {
                    ProcessAction(e, unit, 0, 0, bvar, p_SpellInfo, p_GameObject);
                    RecalcTimer(e, e.m_Event.spellHit.cooldownMin, e.m_Event.spellHit.cooldownMax);
                }
            break;
        }
        case SMART_EVENT_OOC_LOS:
        {
            if (!me || me->isInCombat())
                return;
            //can trigger if closer than fMaxAllowedRange
            float range = (float)e.m_Event.los.maxDist;

            //if range is ok and we are actually in LOS
            if (me->IsWithinDistInMap(unit, range) && me->IsWithinLOSInMap(unit))
            {
                //if friendly event&&who is not hostile OR hostile event&&who is hostile
                if ((e.m_Event.los.noHostile && !me->IsHostileTo(unit)) ||
                    (!e.m_Event.los.noHostile && me->IsHostileTo(unit)))
                if (!me || !unit)
                    return;
                if (e.m_Event.los.playerOnly && unit->GetTypeId() != TYPEID_PLAYER)
				    return;
                if (e.m_Event.los.playerOnly == TYPEID_PLAYER)
                    return;
                RecalcTimer(e, e.m_Event.los.cooldownMin, e.m_Event.los.cooldownMax);
                ProcessAction(e, unit);

            }
            break;
        }
        case SMART_EVENT_IC_LOS:
        {
            if (!me || !me->isInCombat())
                return;
            //can trigger if closer than fMaxAllowedRange
            float range = (float)e.m_Event.los.maxDist;

            //if range is ok and we are actually in LOS
            if (me->IsWithinDistInMap(unit, range) && me->IsWithinLOSInMap(unit))
            {
                //if friendly event&&who is not hostile OR hostile event&&who is hostile
                if ((e.m_Event.los.noHostile && !me->IsHostileTo(unit)) ||
                    (!e.m_Event.los.noHostile && me->IsHostileTo(unit)))
                if (!me || !unit)
                    return;
                if (e.m_Event.los.playerOnly && unit->GetTypeId() != TYPEID_PLAYER)
				    return;
                if (e.m_Event.los.playerOnly == TYPEID_PLAYER)
                    return;
            RecalcTimer(e, e.m_Event.los.cooldownMin, e.m_Event.los.cooldownMax);
            ProcessAction(e, unit);
            }
            break;
        }
        case SMART_EVENT_RESPAWN:
        {
            if (!GetBaseObject())
                return;
            if (e.m_Event.respawn.type == SMART_SCRIPT_RESPAWN_CONDITION_MAP && GetBaseObject()->GetMapId() != e.m_Event.respawn.map)
                return;
            if (e.m_Event.respawn.type == SMART_SCRIPT_RESPAWN_CONDITION_AREA && GetBaseObject()->GetZoneId() != e.m_Event.respawn.area)
                return;
            ProcessAction(e);
            break;
        }
        case SMART_EVENT_SUMMONED_UNIT:
        {
            if (!IsCreature(unit))
                return;
            if (e.m_Event.summoned.creature && unit->GetEntry() != e.m_Event.summoned.creature)
                return;
            ProcessAction(e, unit);
            RecalcTimer(e, e.m_Event.summoned.cooldownMin, e.m_Event.summoned.cooldownMax);
            break;
        }
        case SMART_EVENT_RECEIVE_HEAL:
        case SMART_EVENT_DAMAGED:
        case SMART_EVENT_DAMAGED_TARGET:
        {
            if (var0 > e.m_Event.minMaxRepeat.max || var0 < e.m_Event.minMaxRepeat.min)
                return;

            ProcessAction(e, unit);
            RecalcTimer(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax);
            break;
        }
        case SMART_EVENT_MOVEMENTINFORM:
        {
            if ((e.m_Event.movementInform.type && var0 != e.m_Event.movementInform.type))
                return;

            if (e.m_Event.movementInform.id && var1 != e.m_Event.movementInform.id)
                return;

            ProcessAction(e, unit, var0, var1);
            break;
        }
        case SMART_EVENT_WAYPOINT_START:
        {
            if (e.m_Event.waypoint.pathID && var0 != e.m_Event.waypoint.pathID)
                return;
            ProcessAction(e, unit, var0);
            break;
        }
        case SMART_EVENT_WAYPOINT_REACHED:
        case SMART_EVENT_WAYPOINT_RESUMED:
        case SMART_EVENT_WAYPOINT_PAUSED:
        case SMART_EVENT_WAYPOINT_STOPPED:
        case SMART_EVENT_WAYPOINT_ENDED:
        {
            if (!me || (e.m_Event.waypoint.pointID && var0 != e.m_Event.waypoint.pointID) || (e.m_Event.waypoint.pathID && GetPathId() != e.m_Event.waypoint.pathID))
                return;
            ProcessAction(e, unit);
            break;
        }
        case SMART_EVENT_SUMMON_DESPAWNED:
        case SMART_EVENT_ACCEPTED_QUEST:
        case SMART_EVENT_REWARD_QUEST:
        {
            if (e.m_Event.quest.quest && var0 != e.m_Event.quest.quest)
                return;
            ProcessAction(e, unit, var0);
            break;
        }
        case SMART_EVENT_AREATRIGGER_ONTRIGGER:
        {
            if (e.m_Event.areatrigger.id && var0 != e.m_Event.areatrigger.id)
                return;

            ProcessAction(e, unit, var0);
            break;
        }
        case SMART_EVENT_TEXT_OVER:
        {
            if (var0 != e.m_Event.textOver.textGroupID || (e.m_Event.textOver.creatureEntry && e.m_Event.textOver.creatureEntry != var1))
                return;
            ProcessAction(e, unit, var0);
            break;
        }
        case SMART_EVENT_DATA_SET:
        {
            if (e.m_Event.dataSet.id != var0 || e.m_Event.dataSet.value != var1)
                return;
            ProcessAction(e, unit, var0, var1);
            RecalcTimer(e, e.m_Event.dataSet.cooldownMin, e.m_Event.dataSet.cooldownMax);
            break;
        }
        case SMART_EVENT_PASSENGER_REMOVED:
        case SMART_EVENT_PASSENGER_BOARDED:
        {
            if (!unit)
                return;
            ProcessAction(e, unit);
            RecalcTimer(e, e.m_Event.minMax.repeatMin, e.m_Event.minMax.repeatMax);
            break;
        }
        case SMART_EVENT_TIMED_EVENT_TRIGGERED:
        {
            if (e.m_Event.timedEvent.id == var0)
                ProcessAction(e, unit);
            break;
        }
        case SMART_EVENT_GOSSIP_SELECT:
        {
            TC_LOG_DEBUG("scripts.ai", "SmartScript: Gossip Select:  menu %u action %u", var0, var1);//little help for scripters
            if (e.m_Event.gossip.sender != var0 || e.m_Event.gossip.action != var1)
                return;
            ProcessAction(e, unit, var0, var1);
            break;
        }
        case SMART_EVENT_DUMMY_EFFECT:
        {
            if (e.m_Event.dummy.spell != var0 || e.m_Event.dummy.effIndex != var1)
                return;
            ProcessAction(e, unit, var0, var1);
            break;
        }
        case SMART_EVENT_GAME_EVENT_START:
        case SMART_EVENT_GAME_EVENT_END:
        {
            if (e.m_Event.gameEvent.gameEventId != var0)
                return;
            ProcessAction(e, NULL, var0);
            break;
        }
        case SMART_EVENT_GO_STATE_CHANGED:
        {
            if (e.m_Event.goStateChanged.state != var0)
                return;
            ProcessAction(e, unit, var0, var1);
            break;
        }
        case SMART_EVENT_GO_EVENT_INFORM:
        {
            if (e.m_Event.eventInform.eventId != var0)
                return;
            ProcessAction(e, NULL, var0);
            break;
        }
        case SMART_EVENT_ACTION_DONE:
        {
            if (e.m_Event.doAction.eventId != var0)
                return;
            ProcessAction(e, unit, var0);
            break;
        }
        case SMART_EVENT_FRIENDLY_HEALTH_PCT:
        {
            if (!me || !me->isInCombat())
                return;

            ObjectVector targets;
            switch (e.GetTargetType())
            {
                case SMART_TARGET_CREATURE_RANGE:
                case SMART_TARGET_CREATURE_GUID:
                case SMART_TARGET_CREATURE_DISTANCE:
                case SMART_TARGET_CLOSEST_CREATURE:
                case SMART_TARGET_CLOSEST_PLAYER:
                case SMART_TARGET_PLAYER_RANGE:
                case SMART_TARGET_PLAYER_DISTANCE:
                    GetTargets(targets, e);
                    break;
                default:
                    return;
            }

            Unit* unitTarget = nullptr;
            for (WorldObject* target : targets)
            {
                if (IsUnit(target) && me->IsFriendlyTo(target->ToUnit()) && target->ToUnit()->isAlive() && target->ToUnit()->isInCombat())
                {
                    uint32 healthPct = uint32(target->ToUnit()->GetHealthPct());
                    if (healthPct > e.m_Event.friendlyHealthPct.maxHpPct || healthPct < e.m_Event.friendlyHealthPct.minHpPct)
                        continue;

                    unitTarget = target->ToUnit();
                    break;
                }
            }

            if (!unitTarget)
                return;

            ProcessTimedAction(e, e.m_Event.friendlyHealthPct.repeatMin, e.m_Event.friendlyHealthPct.repeatMax, unitTarget);
            break;
        }
        case SMART_EVENT_DISTANCE_CREATURE:
        {
            if (!me)
                return;

            WorldObject* creature = NULL;

            if (e.m_Event.distance.guid != 0)
            {
                creature = FindCreatureNear(me, e.m_Event.distance.guid);

                if (!creature)
                    return;

                if (!me->IsInRange(creature, 0, (float)e.m_Event.distance.dist))
                    return;
            }
            else if (e.m_Event.distance.entry != 0)
            {
                std::list<Creature*> list;
                me->GetCreatureListWithEntryInGrid(list, e.m_Event.distance.entry, (float)e.m_Event.distance.dist);

                if (!list.empty())
                    creature = list.front();
            }

            if (creature)
                ProcessTimedAction(e, e.m_Event.distance.repeat, e.m_Event.distance.repeat, creature->ToUnit());

            break;
        }
        case SMART_EVENT_DISTANCE_GAMEOBJECT:
        {
            if (!me)
                return;

            WorldObject* gameobject = NULL;

            if (e.m_Event.distance.guid != 0)
            {
                gameobject = FindGameObjectNear(me, e.m_Event.distance.guid);

                if (!gameobject)
                    return;

                if (!me->IsInRange(gameobject, 0, (float)e.m_Event.distance.dist))
                    return;
            }
            else if (e.m_Event.distance.entry != 0)
            {
                std::list<GameObject*> list;
                me->GetGameObjectListWithEntryInGrid(list, e.m_Event.distance.entry, (float)e.m_Event.distance.dist);

                if (!list.empty())
                    gameobject = list.front();
            }

            if (gameobject)
                ProcessTimedAction(e, e.m_Event.distance.repeat, e.m_Event.distance.repeat);

            break;
        }
        case SMART_EVENT_DISTANCE_PLAYER:
        {
            if (!me)
                return;

            std::list<Player*> list = me->GetPlayersInRange((float)e.m_Event.playerDistance.range, true);

            if (e.m_Event.playerDistance.aura)
                list.remove_if(Trinity::UnitAuraCheck(!e.m_Event.playerDistance.isPresent, e.m_Event.playerDistance.aura));

            if (!list.empty())
                ProcessTimedAction(e, e.m_Event.playerDistance.repeat, e.m_Event.playerDistance.repeat, list.front());

            break;
        }
        case SMART_EVENT_COUNTER_SET:
            if (e.m_Event.counter.id != var0 || GetCounterValue(e.m_Event.counter.id) != e.m_Event.counter.value)
                return;
            ProcessTimedAction(e, e.m_Event.counter.cooldownMin, e.m_Event.counter.cooldownMax);
            break;

        default:
            TC_LOG_ERROR("sql.sql", "SmartScript::ProcessEvent: Unhandled Event type %u", e.GetEventType());
            break;
    }
}

void SmartScript::InitTimer(SmartScriptHolder& e)
{
    switch (e.GetEventType())
    {
        //set only events which have initial timers
        case SMART_EVENT_UPDATE:
        case SMART_EVENT_UPDATE_IC:
        case SMART_EVENT_UPDATE_OOC:
            RecalcTimer(e, e.m_Event.minMaxRepeat.min, e.m_Event.minMaxRepeat.max);
            break;
        case SMART_EVENT_DISTANCE_CREATURE:
        case SMART_EVENT_DISTANCE_GAMEOBJECT:
            RecalcTimer(e, e.m_Event.distance.repeat, e.m_Event.distance.repeat);
            break;
        case SMART_EVENT_DISTANCE_PLAYER:
            RecalcTimer(e, e.m_Event.playerDistance.repeat, e.m_Event.playerDistance.repeat);
            break;
        default:
            e.active = true;
            break;
    }
}
void SmartScript::RecalcTimer(SmartScriptHolder& e, uint32 min, uint32 max)
{
    // min/max was checked at loading!
    e.timer = urand(uint32(min), uint32(max));
    e.active = e.timer ? false : true;
}

void SmartScript::UpdateTimer(SmartScriptHolder& e, uint32 const diff)
{
    if (e.GetEventType() == SMART_EVENT_LINK)
        return;

    if (e.m_Event.event_phase_mask && !IsInPhase(e.m_Event.event_phase_mask))
        return;

    if (e.GetEventType() == SMART_EVENT_UPDATE_IC && (!me || !me->isInCombat()))
        return;

    if (e.GetEventType() == SMART_EVENT_UPDATE_OOC && (me && me->isInCombat())) //can be used with me=nullptr (go script)
        return;

    if (e.timer < diff)
    {
        // delay spell cast event if another spell is being casted
        if (e.GetActionType() == SMART_ACTION_CAST)
        {
            if (!(e.action.cast.castFlags & SMARTCAST_INTERRUPT_PREVIOUS))
            {
                if (me && me->HasUnitState(UNIT_STATE_CASTING))
                {
                    e.timer = 1;
                    return;
                }
            }
        }

        e.active = true; // activate events with cooldown
        switch (e.GetEventType()) // process ONLY timed events
        {
            case SMART_EVENT_UPDATE:
            case SMART_EVENT_UPDATE_OOC:
            case SMART_EVENT_UPDATE_IC:
            case SMART_EVENT_HEALT_PCT:
            case SMART_EVENT_TARGET_HEALTH_PCT:
            case SMART_EVENT_MANA_PCT:
            case SMART_EVENT_TARGET_MANA_PCT:
            case SMART_EVENT_RANGE:
            case SMART_EVENT_VICTIM_CASTING:
            case SMART_EVENT_FRIENDLY_HEALTH:
            case SMART_EVENT_FRIENDLY_IS_CC:
            case SMART_EVENT_FRIENDLY_MISSING_BUFF:
            case SMART_EVENT_HAS_AURA:
            case SMART_EVENT_TARGET_BUFFED:
            case SMART_EVENT_IS_BEHIND_TARGET:
            case SMART_EVENT_FRIENDLY_HEALTH_PCT:
            case SMART_EVENT_DISTANCE_CREATURE:
            case SMART_EVENT_DISTANCE_GAMEOBJECT:
            case SMART_EVENT_DISTANCE_PLAYER:
            {
                ProcessEvent(e);

                if (e.GetScriptType() == SMART_SCRIPT_TYPE_TIMED_ACTIONLIST)
                {
                    e.enableTimed = false;//disable event if it is in an ActionList and was processed once
                    for (SmartAIEventList::iterator i = mTimedActionList.begin(); i != mTimedActionList.end(); ++i)
                    {
                        //find the first event which is not the current one and enable it
                        if (i->event_id > e.event_id)
                        {
                            i->enableTimed = true;
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    else
        e.timer -= diff;
}

bool SmartScript::CheckTimer(SmartScriptHolder const& e) const
{
    return e.active;
}

void SmartScript::InstallEvents()
{
    if (!mInstallEvents.empty())
    {
        auto l_End = mInstallEvents.end();

        for (SmartAIEventList::iterator i = mInstallEvents.begin(); i != l_End; ++i)
            mEvents.push_back(*i);//must be before UpdateTimers

        mInstallEvents.clear();
    }
}

void SmartScript::OnUpdate(uint32 const diff)
{
    InstallEvents(); // before UpdateTimers

    for (SmartAIEventList::iterator i = mEvents.begin(); i != mEvents.end(); ++i)
        UpdateTimer(*i, diff);

    if (!mStoredEvents.empty())
        for (SmartAIEventList::iterator i = mStoredEvents.begin(); i != mStoredEvents.end(); ++i)
             UpdateTimer(*i, diff);

    bool needCleanup = true;
    if (!mTimedActionList.empty())
    {
        isProcessingTimedActionList = true;
        for (SmartAIEventList::iterator i = mTimedActionList.begin(); i != mTimedActionList.end(); ++i)
        {
            if ((*i).enableTimed)
            {
                UpdateTimer(*i, diff);
                needCleanup = false;
            }
        }

        isProcessingTimedActionList = false;
    }

    if (needCleanup)
        mTimedActionList.clear();

    if (!mRemIDs.empty())
    {
        for (std::list<uint32>::iterator i = mRemIDs.begin(); i != mRemIDs.end(); ++i)
        {
             RemoveStoredEvent((*i));
        }
    }

    if (mUseTextTimer && me)
    {
        if (mTextTimer < diff)
        {
            uint32 textID = mLastTextID;
            mLastTextID = 0;
            uint32 entry = mTalkerEntry;
            mTalkerEntry = 0;
            mTextTimer = 0;
            mUseTextTimer = false;
            ProcessEventsFor(SMART_EVENT_TEXT_OVER, NULL, textID, entry);
        } else mTextTimer -= diff;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

void SmartScript::GetScript()
{
    SmartAIEventList l_SmartAIEventList;

    switch (mScriptType)
    {
        case SMART_SCRIPT_TYPE_CREATURE:
            l_SmartAIEventList = sSmartScriptMgr->GetScript(-((int32)me->ToCreature()->GetDBTableGUIDLow()), mScriptType);
            if (l_SmartAIEventList.empty())
                l_SmartAIEventList = sSmartScriptMgr->GetScript((int32)me->ToCreature()->GetEntry(), mScriptType);
            break;

        case SMART_SCRIPT_TYPE_GAMEOBJECT:
            l_SmartAIEventList = sSmartScriptMgr->GetScript(-((int32)go->GetDBTableGUIDLow()), mScriptType);
            if (l_SmartAIEventList.empty())
                l_SmartAIEventList = sSmartScriptMgr->GetScript((int32)go->GetEntry(), mScriptType);
            break;

        case SMART_SCRIPT_TYPE_AREATRIGGER:
            l_SmartAIEventList = sSmartScriptMgr->GetScript((int32)trigger->id, mScriptType);
            break;

        case SMART_SCRIPT_TYPE_QUEST:
            l_SmartAIEventList = sSmartScriptMgr->GetScript(m_QuestId, mScriptType);
            break;

        case SMART_SCRIPT_TYPE_EVENT:
        case SMART_SCRIPT_TYPE_SPELL:
        case SMART_SCRIPT_TYPE_WAYPOINT:
            l_SmartAIEventList = sSmartScriptMgr->GetScript(m_EntryId, mScriptType);
            break;
    }

    FillScript(std::move(l_SmartAIEventList));
}

void SmartScript::FillScript(SmartAIEventList p_EventList)
{
    if (p_EventList.empty())
        return;

    for (SmartAIEventList::iterator i = p_EventList.begin(); i != p_EventList.end(); ++i)
    {
        #ifndef TRINITY_DEBUG
            if ((*i).m_Event.event_flags & SMART_EVENT_FLAG_DEBUG_ONLY)
                continue;
        #endif

        if ((*i).m_Event.event_flags & SMART_EVENT_FLAG_DIFFICULTY_ALL)//if has instance flag add only if in it
        {
            if (me && me->GetMap()->IsDungeon())
            {
                switch (me->GetMap()->GetDifficulty())
                {
                    case DUNGEON_DIFFICULTY_NORMAL:
                    ///case RAID_DIFFICULTY_10MAN_NORMAL:
                        if (i->m_Event.event_flags & SMART_EVENT_FLAG_DIFFICULTY_0)
                            mEvents.emplace_back(std::move(*i));
                        break;
                    case DUNGEON_DIFFICULTY_HEROIC:
                    ///case RAID_DIFFICULTY_25MAN_NORMAL:
                        if (i->m_Event.event_flags & SMART_EVENT_FLAG_DIFFICULTY_1)
                            mEvents.emplace_back(std::move(*i));
                        break;
                    case RAID_DIFFICULTY_10MAN_HEROIC:
                        if (i->m_Event.event_flags & SMART_EVENT_FLAG_DIFFICULTY_2)
                            mEvents.emplace_back(std::move(*i));
                        break;
                    case RAID_DIFFICULTY_25MAN_HEROIC:
                        if (i->m_Event.event_flags & SMART_EVENT_FLAG_DIFFICULTY_3)
                            mEvents.emplace_back(std::move(*i));
                        break;
                    default:
                        break;
                }
            }
            continue;
        }
        mEvents.push_back(std::move(*i));//NOTE: 'world(0)' events still get processed in ANY instance mode
    }
    if (mEvents.empty() && me)
        TC_LOG_ERROR("sql.sql", "SmartScript: Entry %u has events but no events added to list because of instance flags.", me->GetEntry());
    if (mEvents.empty() && trigger)
        TC_LOG_ERROR("sql.sql", "SmartScript: AreaTrigger %u has events but no events added to list because of instance flags. NOTE: triggers can not handle any instance flags.", trigger->id);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void SmartScript::OnInitialize()
{
    GetScript(); ///< Load copy of script

    /// Calculate timers for first time use
    for (SmartAIEventList::iterator i = mEvents.begin(); i != mEvents.end(); ++i)
        InitTimer((*i));

    /// Process SMART_EVENT_AI_INIT
    ProcessEventsFor(SMART_EVENT_AI_INIT);

    /// Install all events
    InstallEvents();

    /// Process SMART_EVENT_JUST_CREATED
    ProcessEventsFor(SMART_EVENT_JUST_CREATED);

    /// Set value for m_EndTime
    switch (mScriptType)
    {
        case SMART_SCRIPT_TYPE_SPELL:
        case SMART_SCRIPT_TYPE_EVENT:
        case SMART_SCRIPT_TYPE_WAYPOINT:

            for (auto const& l_Event : mEvents)
                m_EndTime += l_Event.timer;

            break;
    }
}

void SmartScript::OnMoveInLineOfSight(Unit* who)
{
    if (!me)
        return;

    ProcessEventsFor(me->isInCombat() ? SMART_EVENT_IC_LOS : SMART_EVENT_OOC_LOS, who);
}

Unit* SmartScript::DoSelectLowestHpFriendly(float range, uint32 MinHPDiff) const
{
    if (!me)
        return nullptr;

    CellCoord p(Trinity::ComputeCellCoord(me->GetPositionX(), me->GetPositionY()));
    Cell cell(p);
    cell.SetNoCreate();

    Unit* unit = nullptr;

    Trinity::MostHPMissingInRange u_check(me, range, MinHPDiff);
    Trinity::UnitLastSearcher<Trinity::MostHPMissingInRange> searcher(me, unit, u_check);

    TypeContainerVisitor<Trinity::UnitLastSearcher<Trinity::MostHPMissingInRange>, GridTypeMapContainer >  grid_unit_searcher(searcher);

    cell.Visit(p, grid_unit_searcher, *me->GetMap(), *me, range);
    return unit;
}

void SmartScript::DoFindFriendlyCC(std::vector<Creature*>& creatures, float range) const
{
    if (!me)
        return;

    Trinity::FriendlyCCedInRange u_check(me, range);
    Trinity::CreatureListSearcher<Trinity::FriendlyCCedInRange> searcher(me, creatures, u_check);
    Cell::VisitGridObjects(me, searcher, range);
}

void SmartScript::DoFindFriendlyMissingBuff(std::vector<Creature*>& creatures, float range, uint32 spellid) const
{
    if (!me)
        return;

    Trinity::FriendlyMissingBuffInRange u_check(me, range, spellid);
    Trinity::CreatureListSearcher<Trinity::FriendlyMissingBuffInRange> searcher(me, creatures, u_check);
    Cell::VisitGridObjects(me, searcher, range);
}

Unit* SmartScript::DoFindClosestFriendlyInRange(float range, bool playerOnly) const
{
    if (!me)
        return nullptr;

    Unit* unit = nullptr;
    Trinity::AnyFriendlyUnitInObjectRangeCheck u_check(me, me, range);
    Trinity::UnitLastSearcher<Trinity::AnyFriendlyUnitInObjectRangeCheck> searcher(me, unit, u_check);
    Cell::VisitAllObjects(me, searcher, range);
    return unit;
}

void SmartScript::SetTimedActionList(SmartScriptHolder& e, uint32 entry)
{
    //do NOT clear mTimedActionList if it's being iterated because it will invalidate the iterator and delete
    // any SmartScriptHolder contained like the "e" parameter passed to this function
    if (isProcessingTimedActionList)
        return;

    mTimedActionList.clear();
    mTimedActionList = sSmartScriptMgr->GetScript(entry, SMART_SCRIPT_TYPE_TIMED_ACTIONLIST);
    if (mTimedActionList.empty())
        return;
    for (SmartAIEventList::iterator i = mTimedActionList.begin(); i != mTimedActionList.end(); ++i)
    {
        if (i == mTimedActionList.begin())
        {
            i->enableTimed = true;//enable processing only for the first action
        }
        else i->enableTimed = false;

        if (e.action.timedActionList.timerType == 1)
            i->m_Event.type = SMART_EVENT_UPDATE_IC;
        else if (e.action.timedActionList.timerType > 1)
            i->m_Event.type = SMART_EVENT_UPDATE;
        InitTimer((*i));
    }
}

Unit* SmartScript::GetLastInvoker(Unit* invoker) const
{
    // Look for invoker only on map of base object... Prevents multithreaded crashes
    if (WorldObject* baseObject = GetBaseObject())
        return ObjectAccessor::GetUnit(*baseObject, mLastInvoker);
    // used for area triggers invoker cast
    else if (invoker)
        return ObjectAccessor::GetUnit(*invoker, mLastInvoker);

    return ObjectAccessor::GetUnit(*GetBaseObject(), mLastInvoker);
}

void SmartScript::RemoveStoredEvent(uint32 id)
{
    if (!mStoredEvents.empty())
    {
        for (SmartAIEventList::iterator i = mStoredEvents.begin(); i != mStoredEvents.end(); ++i)
        {
            if (i->event_id == id)
            {
                mStoredEvents.erase(i);
                return;
            }
        }
    }
}

SmartScriptHolder SmartScript::FindLinkedEvent(uint32 link)
{
    if (!mEvents.empty())
    {
        for (SmartAIEventList::iterator i = mEvents.begin(); i != mEvents.end(); ++i)
        {
            if (i->event_id == link)
            {
                return (*i);
            }
        }
    }

    SmartScriptHolder s;
    return s;
}
