//////////////////////////////////////////////////////////////////////////
///
///  PROJECT AZENOR
/// COPYRIGHT 2018-2019
/// ALL RIGHTS RESERVED.
///
//////////////////////////////////////////////////////////////////////////

#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "ObjectDefines.h"
#include "GridDefines.h"
#include "GridNotifiers.h"
#include "SpellMgr.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "GameEventMgr.h"
#include "CreatureTextMgr.h"

#include "SmartScriptMgr.h"
#include "WaypointManager.h"

void SmartWaypointMgr::LoadFromDB()
{
    uint32 oldMSTime = getMSTime();

    for (std::unordered_map<uint32, WPPath*>::iterator itr = waypoint_map.begin(); itr != waypoint_map.end(); ++itr)
    {
        for (WPPath::iterator pathItr = itr->second->begin(); pathItr != itr->second->end(); ++pathItr)
            delete pathItr->second;

        delete itr->second;
    }

    waypoint_map.clear();

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_SMARTAI_WP);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 SmartAI Waypoint Paths. DB table `waypoints` is empty.");

        return;
    }

    uint32 count = 0;
    uint32 total = 0;
    uint32 last_entry = 0;
    uint32 last_id = 1;

    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();
        uint32 id = fields[1].GetUInt32();
        float x, y, z;
        x = fields[2].GetFloat();
        y = fields[3].GetFloat();
        z = fields[4].GetFloat();

        if (last_entry != entry)
        {
            waypoint_map[entry] = new WPPath();
            last_id = 1;
            count++;
        }

        if (last_id != id)
            TC_LOG_ERROR("sql.sql", "SmartWaypointMgr::LoadFromDB: Path entry %u, unexpected point id %u, expected %u.", entry, id, last_id);

        last_id++;
        (*waypoint_map[entry])[id] = new WayPoint(id, x, y, z);

        last_entry = entry;
        total++;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u SmartAI waypoint paths (total %u waypoints) in %u ms", count, total, GetMSTimeDiffToNow(oldMSTime));

}

SmartWaypointMgr::~SmartWaypointMgr()
{
    for (std::unordered_map<uint32, WPPath*>::iterator itr = waypoint_map.begin(); itr != waypoint_map.end(); ++itr)
    {
        for (WPPath::iterator pathItr = itr->second->begin(); pathItr != itr->second->end(); ++pathItr)
            delete pathItr->second;

        delete itr->second;
    }

    waypoint_map.clear();
}

void SmartAIMgr::LoadSmartAIFromDB(int32 p_Entry)
{
    LoadHelperStores();

    uint32 oldMSTime = getMSTime();
                                                    // 0                    1     2   3          4             5              6            7
    QueryResult l_Result = WorldDatabase.Query("SELECT entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o FROM smart_scripts ORDER BY entryorguid, source_type, id, link");

    if (!l_Result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 SmartAI scripts. DB table `smart_scripts` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = l_Result->Fetch();

        SmartScriptHolder temp;

        temp.entryOrGuid                    = fields[0].GetInt32();
        SmartScriptType source_type         = (SmartScriptType)fields[1].GetUInt8();

        if (source_type >= SMART_SCRIPT_TYPE_MAX)
        {
            TC_LOG_ERROR("sql.sql", "SmartAIMgr::LoadSmartAIFromDB: invalid source_type (%u), skipped loading.", uint32(source_type));
            continue;
        }

        if (temp.entryOrGuid >= 0)
        {
            switch (source_type)
            {
                case SMART_SCRIPT_TYPE_CREATURE:
                {
                    if (!sObjectMgr->GetCreatureTemplate((uint32)temp.entryOrGuid))
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr::LoadSmartAIFromDB: Creature entry (%u) does not exist, skipped loading.", uint32(temp.entryOrGuid));
                        continue;
                    }
                    break;
                }

                case SMART_SCRIPT_TYPE_GAMEOBJECT:
                {
                    if (!sObjectMgr->GetGameObjectTemplate((uint32)temp.entryOrGuid))
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr::LoadSmartAIFromDB: GameObject entry (%u) does not exist, skipped loading.", uint32(temp.entryOrGuid));
                        continue;
                    }
                    break;
                }

                case SMART_SCRIPT_TYPE_AREATRIGGER:
                {
                    if (!sAreaTriggerStore.LookupEntry((uint32)temp.entryOrGuid))
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr::LoadSmartAIFromDB: AreaTrigger entry (%u) does not exist, skipped loading.", uint32(temp.entryOrGuid));
                        continue;
                    }
                    break;
                }

                case SMART_SCRIPT_TYPE_SPELL:
                {
                    if (!sSpellMgr->GetSpellInfo(static_cast<uint32>(temp.entryOrGuid)))
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr::LoadSmartAIFromDB: Spell entry (%u) does not exist, skipped loading.", static_cast<uint32>(temp.entryOrGuid));
                        continue;
                    }
                    break;
                }

                case SMART_SCRIPT_TYPE_TIMED_ACTIONLIST:
                    break;//nothing to check, really

                case SMART_SCRIPT_TYPE_WAYPOINT:
                    break;

                case SMART_SCRIPT_TYPE_QUEST:
                    if (!sObjectMgr->GetQuestTemplate(static_cast<uint32>(temp.entryOrGuid)))
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr::LoadSmartAIFromDB: Quest entry (%u) does not exist, skipped loading.", static_cast<uint32>(temp.entryOrGuid));
                        continue;
                    }
                    break;

                case SMART_SCRIPT_TYPE_EVENT:
                    break;

                default:
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr::LoadSmartAIFromDB: not yet implemented source_type %u", (uint32)source_type);
                    continue;
            }
        }

        temp.source_type                    = source_type;

        /// Event
        temp.event_id                       = fields[2].GetUInt16();
        temp.link                           = fields[3].GetUInt16();
        temp.m_Event.type                   = (SMART_EVENT)fields[4].GetUInt8();
        temp.m_Event.event_phase_mask       = fields[5].GetUInt8();
        temp.m_Event.event_chance           = fields[6].GetUInt8();
        temp.m_Event.event_flags            = fields[7].GetUInt16();
        temp.m_Event.raw.param1             = fields[8].GetUInt32();
        temp.m_Event.raw.param2             = fields[9].GetUInt32();
        temp.m_Event.raw.param3             = fields[10].GetUInt32();
        temp.m_Event.raw.param4             = fields[11].GetUInt32();
        temp.m_Event.raw.param5             = fields[12].GetUInt32();

        /// Action
        temp.action.type                    = (SMART_ACTION)fields[13].GetUInt32();
        temp.action.raw.param1              = fields[14].GetUInt32();
        temp.action.raw.param2              = fields[15].GetUInt32();
        temp.action.raw.param3              = fields[16].GetUInt32();
        temp.action.raw.param4              = fields[17].GetUInt32();
        temp.action.raw.param5              = fields[18].GetUInt32();
        temp.action.raw.param6              = fields[19].GetUInt32();

        /// Target
        temp.target.type                    = (SMARTAI_TARGETS)fields[20].GetUInt8();
        temp.target.raw.param1              = fields[21].GetUInt32();
        temp.target.raw.param2              = fields[22].GetUInt32();
        temp.target.raw.param3              = fields[23].GetUInt32();
        temp.target.x                       = fields[24].GetFloat();
        temp.target.y                       = fields[25].GetFloat();
        temp.target.z                       = fields[26].GetFloat();
        temp.target.o                       = fields[27].GetFloat();

        /// Check target
        if (!IsTargetValid(temp))
            continue;

        /// Check all event and action params
        if (!IsEventValid(temp))
            continue;

        /// Spellscript case for handling flags of effMask
        ///if (source_type == SmartScriptType::SMART_SCRIPT_TYPE_SPELL)
        ///{
        ///    auto FlagsToEffMask([](uint32 p_Flag) -> uint32
        ///    {
        ///        uint32 l_EffMask = 0;
        ///
        ///        if (p_Flag & SMART_EVENT_FLAG_SPELL_EFFECT_1)
        ///            l_EffMask += 1;
        ///
        ///        if (p_Flag & SMART_EVENT_FLAG_SPELL_EFFECT_2)
        ///            l_EffMask += 2;
        ///
        ///        return l_EffMask;
        ///    });
        ///
        ///    temp.entryOrGuid |= (FlagsToEffMask(temp.m_Event.event_flags) << 24);
        ///}

        /// Increase counter for log
        if (mEventMap[source_type].find(temp.entryOrGuid) == mEventMap[source_type].end())
            ++count;

        /// Store event
        mEventMap[source_type][temp.entryOrGuid].push_back(temp);

    } while (l_Result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u SmartAI scripts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

    UnLoadHelperStores();
}

bool SmartAIMgr::IsTargetValid(SmartScriptHolder const& e)
{
    if (e.GetActionType() == SMART_ACTION_INSTALL_AI_TEMPLATE)
        return true; // AI template has special handling

    switch (e.GetTargetType())
    {
        case SMART_TARGET_CREATURE_DISTANCE:
        case SMART_TARGET_CREATURE_RANGE:
        {
            if (e.target.unitDistance.creature && !sObjectMgr->GetCreatureTemplate(e.target.unitDistance.creature))
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Creature entry %u as target_param1, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.target.unitDistance.creature);
                return false;
            }
            break;
        }
        case SMART_TARGET_GAMEOBJECT_DISTANCE:
        case SMART_TARGET_GAMEOBJECT_RANGE:
        {
            if (e.target.goDistance.entry && !sObjectMgr->GetGameObjectTemplate(e.target.goDistance.entry))
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent GameObject entry %u as target_param1, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.target.goDistance.entry);
                return false;
            }
            break;
        }
        case SMART_TARGET_CREATURE_GUID:
        {
            if (e.target.unitGUID.entry && !IsCreatureValid(e, e.target.unitGUID.entry))
                return false;
            break;
        }
        case SMART_TARGET_GAMEOBJECT_GUID:
        {
            if (e.target.goGUID.entry && !IsGameObjectValid(e, e.target.goGUID.entry))
                return false;
            break;
        }
        case SMART_TARGET_PLAYER_DISTANCE:
        case SMART_TARGET_CLOSEST_PLAYER:
        {
            if (e.target.playerDistance.dist == 0)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u has maxDist 0 as target_param1, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                return false;
            }
            break;
        }
        case SMART_TARGET_PLAYER_RANGE:
        case SMART_TARGET_SELF:
        case SMART_TARGET_VICTIM:
        case SMART_TARGET_HOSTILE_SECOND_AGGRO:
        case SMART_TARGET_HOSTILE_LAST_AGGRO:
        case SMART_TARGET_HOSTILE_RANDOM:
        case SMART_TARGET_HOSTILE_RANDOM_NOT_TOP:
        case SMART_TARGET_ACTION_INVOKER:
        case SMART_TARGET_INVOKER_PARTY:
        case SMART_TARGET_POSITION:
        case SMART_TARGET_NONE:
        case SMART_TARGET_ACTION_INVOKER_VEHICLE:
        case SMART_TARGET_OWNER_OR_SUMMONER:
        case SMART_TARGET_THREAT_LIST:
        case SMART_TARGET_CLOSEST_GAMEOBJECT:
        case SMART_TARGET_CLOSEST_CREATURE:
        case SMART_TARGET_CLOSEST_ENEMY:
        case SMART_TARGET_CLOSEST_FRIENDLY:
        case SMART_TARGET_STORED:
        case SMART_TARGET_LOOT_RECIPIENTS:
        case SMART_TARGET_FARTHEST_PLAYER:
        case SMART_TARGET_VEHICLE_ACCESSORY:
        case SMART_TARGET_INVOKER_SUMMON:
        case SMART_TARGET_SPELL_CASTER:
        case SMART_TARGET_SPELL_TARGET:
        case SMART_TARGET_ANY_IN_RANGE_WITH_CONDITIONS:
        case SMART_TARGET_VEHICLE_PASSENGER:
        case SMART_TARGET_SUMMONER_SUMMON_LIST:
            break;
        default:
            TC_LOG_ERROR("sql.sql", "SmartAIMgr: Not handled target_type(%u), Entry %d SourceType %u Event %u Action %u, skipped.", e.GetTargetType(), e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
            return false;
    }
    return true;
}

bool SmartAIMgr::ScriptExist(int32 p_Entry, uint32 p_Type) const
{
    return mEventMap[p_Type].find(p_Entry) != mEventMap[p_Type].end();
}

bool SmartAIMgr::IsEventValid(SmartScriptHolder& e)
{
    if (e.m_Event.type >= SMART_EVENT_END)
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: EntryOrGuid %d using event(%u) has invalid event type (%u), skipped.", e.entryOrGuid, e.event_id, e.GetEventType());
        return false;
    }
    if (e.action.type <= 0 || e.action.type >= SMART_ACTION_END)
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: EntryOrGuid %d using event(%u) has invalid action type (%u), skipped.", e.entryOrGuid, e.event_id, e.GetActionType());
        return false;
    }
    if (e.m_Event.event_phase_mask > SMART_EVENT_PHASE_ALL)
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: EntryOrGuid %d using event(%u) has invalid phase mask (%u), skipped.", e.entryOrGuid, e.event_id, e.m_Event.event_phase_mask);
        return false;
    }
    if (e.m_Event.event_flags > SMART_EVENT_FLAGS_ALL)
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: EntryOrGuid %d using event(%u) has invalid event flags (%u), skipped.", e.entryOrGuid, e.event_id, e.m_Event.event_flags);
        return false;
    }
    if (e.GetScriptType() == SMART_SCRIPT_TYPE_TIMED_ACTIONLIST)
    {
        e.m_Event.type = SMART_EVENT_UPDATE_OOC;//force default OOC, can change when calling the script!
        if (!IsMinMaxValid(e, e.m_Event.minMaxRepeat.min, e.m_Event.minMaxRepeat.max))
            return false;

        if (!IsMinMaxValid(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax))
            return false;
    }
    else
    {
        uint32 type = e.m_Event.type;
        switch (type)
        {
            case SMART_EVENT_UPDATE:
            case SMART_EVENT_UPDATE_IC:
            case SMART_EVENT_UPDATE_OOC:
            case SMART_EVENT_HEALT_PCT:
            case SMART_EVENT_MANA_PCT:
            case SMART_EVENT_TARGET_HEALTH_PCT:
            case SMART_EVENT_TARGET_MANA_PCT:
            case SMART_EVENT_RANGE:
            case SMART_EVENT_DAMAGED:
            case SMART_EVENT_DAMAGED_TARGET:
            case SMART_EVENT_RECEIVE_HEAL:
                if (!IsMinMaxValid(e, e.m_Event.minMaxRepeat.min, e.m_Event.minMaxRepeat.max))
                    return false;

                if (!IsMinMaxValid(e, e.m_Event.minMaxRepeat.repeatMin, e.m_Event.minMaxRepeat.repeatMax))
                    return false;
                break;
            case SMART_EVENT_SPELLHIT:
            case SMART_EVENT_SPELLHIT_TARGET:
                if (e.m_Event.spellHit.spell)
                {
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(e.m_Event.spellHit.spell);
                    if (!spellInfo)
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Spell entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.m_Event.spellHit.spell);
                        return false;
                    }
                    if (e.m_Event.spellHit.school && (e.m_Event.spellHit.school & spellInfo->SchoolMask) != spellInfo->SchoolMask)
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses Spell entry %u with invalid school mask, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.m_Event.spellHit.spell);
                        return false;
                    }
                }
                if (!IsMinMaxValid(e, e.m_Event.spellHit.cooldownMin, e.m_Event.spellHit.cooldownMax))
                    return false;
                break;
            case SMART_EVENT_OOC_LOS:
            case SMART_EVENT_IC_LOS:
                if (!IsMinMaxValid(e, e.m_Event.los.cooldownMin, e.m_Event.los.cooldownMax))
                    return false;
                break;
            case SMART_EVENT_RESPAWN:
                if (e.m_Event.respawn.type == SMART_SCRIPT_RESPAWN_CONDITION_MAP && !sMapStore.LookupEntry(e.m_Event.respawn.map))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Map entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.m_Event.respawn.map);
                    return false;
                }
                if (e.m_Event.respawn.type == SMART_SCRIPT_RESPAWN_CONDITION_AREA && !sAreaStore.LookupEntry(e.m_Event.respawn.area))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Area entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.m_Event.respawn.area);
                    return false;
                }
                break;
            case SMART_EVENT_FRIENDLY_HEALTH:
                if (!NotNULL(e, e.m_Event.friendlyHealth.radius))
                    return false;

                if (!IsMinMaxValid(e, e.m_Event.friendlyHealth.repeatMin, e.m_Event.friendlyHealth.repeatMax))
                    return false;
                break;
            case SMART_EVENT_FRIENDLY_IS_CC:
                if (!IsMinMaxValid(e, e.m_Event.friendlyCC.repeatMin, e.m_Event.friendlyCC.repeatMax))
                    return false;
                break;
            case SMART_EVENT_FRIENDLY_MISSING_BUFF:
            {
                if (!IsSpellValid(e, e.m_Event.missingBuff.spell))
                    return false;

                if (!NotNULL(e, e.m_Event.missingBuff.radius))
                    return false;

                if (!IsMinMaxValid(e, e.m_Event.missingBuff.repeatMin, e.m_Event.missingBuff.repeatMax))
                    return false;
                break;
            }
            case SMART_EVENT_KILL:
                if (!IsMinMaxValid(e, e.m_Event.kill.cooldownMin, e.m_Event.kill.cooldownMax))
                    return false;

                if (e.m_Event.kill.creature && !IsCreatureValid(e, e.m_Event.kill.creature))
                    return false;
                break;
            case SMART_EVENT_VICTIM_CASTING:
                if (e.m_Event.targetCasting.spellId > 0 && !sSpellMgr->GetSpellInfo(e.m_Event.targetCasting.spellId))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Spell entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.m_Event.spellHit.spell);
                    return false;
                }

                if (!IsMinMaxValid(e, e.m_Event.targetCasting.repeatMin, e.m_Event.targetCasting.repeatMax))
                    return false;
                break;
            case SMART_EVENT_PASSENGER_BOARDED:
            case SMART_EVENT_PASSENGER_REMOVED:
                if (!IsMinMaxValid(e, e.m_Event.minMax.repeatMin, e.m_Event.minMax.repeatMax))
                    return false;
                break;
            case SMART_EVENT_SUMMON_DESPAWNED:
            case SMART_EVENT_SUMMONED_UNIT:
                if (e.m_Event.summoned.creature && !IsCreatureValid(e, e.m_Event.summoned.creature))
                    return false;

                if (!IsMinMaxValid(e, e.m_Event.summoned.cooldownMin, e.m_Event.summoned.cooldownMax))
                    return false;
                break;
            case SMART_EVENT_ACCEPTED_QUEST:
            case SMART_EVENT_REWARD_QUEST:
                if (e.m_Event.quest.quest && !IsQuestValid(e, e.m_Event.quest.quest))
                    return false;
                break;
            case SMART_EVENT_RECEIVE_EMOTE:
            {
                if (e.m_Event.emote.emote && !IsTextEmoteValid(e, e.m_Event.emote.emote))
                    return false;

                if (!IsMinMaxValid(e, e.m_Event.emote.cooldownMin, e.m_Event.emote.cooldownMax))
                    return false;
                break;
            }
            case SMART_EVENT_HAS_AURA:
            case SMART_EVENT_TARGET_BUFFED:
            {
                if (!IsSpellValid(e, e.m_Event.aura.spell))
                    return false;

                if (!IsMinMaxValid(e, e.m_Event.aura.repeatMin, e.m_Event.aura.repeatMax))
                    return false;
                break;
            }
            case SMART_EVENT_MOVEMENTINFORM:
            {
                if (e.m_Event.movementInform.type > NULL_MOTION_TYPE)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses invalid Motion type %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.m_Event.movementInform.type);
                    return false;
                }
                break;
            }
            case SMART_EVENT_DATA_SET:
            {
                if (!IsMinMaxValid(e, e.m_Event.dataSet.cooldownMin, e.m_Event.dataSet.cooldownMax))
                    return false;
                break;
            }
            case SMART_EVENT_AREATRIGGER_ONTRIGGER:
            {
                if (e.m_Event.areatrigger.id && !IsAreaTriggerValid(e, e.m_Event.areatrigger.id))
                    return false;
                break;
            }
            case SMART_EVENT_TEXT_OVER:
                //if (e.event.textOver.textGroupID && !IsTextValid(e, e.event.textOver.textGroupID)) return false;// 0 is a valid text group!
                break;
            case SMART_EVENT_COUNTER_SET:
                if (!IsMinMaxValid(e, e.m_Event.counter.cooldownMin, e.m_Event.counter.cooldownMax))
                    return false;

                if (e.m_Event.counter.id == 0)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_COUNTER_SET using invalid counter id %u, skipped.", e.m_Event.counter.id);
                    return false;
                }

                if (e.m_Event.counter.value == 0)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_COUNTER_SET using invalid value %u, skipped.", e.m_Event.counter.value);
                    return false;
                }
                break;

            case SMART_EVENT_LINK:
            {
                if (e.link && e.link == e.event_id)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u, Event %u, Link Event is linking self (infinite loop), skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id);
                    return false;
                }
                break;
            }
            case SMART_EVENT_DUMMY_EFFECT:
            {
                if (!IsSpellValid(e, e.m_Event.dummy.spell))
                    return false;

                if (e.m_Event.dummy.effIndex > EFFECT_2)
                    return false;
                break;
            }
            case SMART_EVENT_IS_BEHIND_TARGET:
            {
                if (!IsMinMaxValid(e, e.m_Event.behindTarget.cooldownMin, e.m_Event.behindTarget.cooldownMax))
                    return false;
                break;
            }
            case SMART_EVENT_GAME_EVENT_START:
            case SMART_EVENT_GAME_EVENT_END:
            {
                GameEventMgr::GameEventDataMap const& events = sGameEventMgr->GetEventMap();
                if (e.m_Event.gameEvent.gameEventId >= events.size() || !events[e.m_Event.gameEvent.gameEventId].isValid())
                    return false;
                break;
            }
            case SMART_EVENT_ACTION_DONE:
            {
                if (e.m_Event.doAction.eventId > EVENT_CHARGE)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses invalid event id %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.m_Event.doAction.eventId);
                    return false;
                }
                break;
            }
            case SMART_EVENT_FRIENDLY_HEALTH_PCT:
            {
                if (!IsMinMaxValid(e, e.m_Event.friendlyHealthPct.repeatMin, e.m_Event.friendlyHealthPct.repeatMax))
                    return false;

                if (e.m_Event.friendlyHealthPct.maxHpPct > 100 || e.m_Event.friendlyHealthPct.minHpPct > 100)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u has pct value above 100, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                    return false;
                }

                switch (e.GetTargetType())
                {
                    case SMART_TARGET_CREATURE_RANGE:
                    case SMART_TARGET_CREATURE_GUID:
                    case SMART_TARGET_CREATURE_DISTANCE:
                    case SMART_TARGET_CLOSEST_CREATURE:
                    case SMART_TARGET_CLOSEST_PLAYER:
                    case SMART_TARGET_PLAYER_RANGE:
                    case SMART_TARGET_PLAYER_DISTANCE:
                    break;
                    default:
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses invalid target_type %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.GetTargetType());
                    return false;
                }
                break;
            }
            case SMART_EVENT_DISTANCE_CREATURE:
            {
                if (e.m_Event.distance.guid == 0 && e.m_Event.distance.entry == 0)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_CREATURE did not provide creature guid or entry, skipped.");
                    return false;
                }

                if (e.m_Event.distance.guid != 0 && e.m_Event.distance.entry != 0)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_CREATURE provided both an entry and guid, skipped.");
                    return false;
                }

                if (e.m_Event.distance.guid != 0 && !sObjectMgr->GetCreatureData(e.m_Event.distance.guid))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_CREATURE using invalid creature guid %u, skipped.", e.m_Event.distance.guid);
                    return false;
                }

                if (e.m_Event.distance.entry != 0 && !sObjectMgr->GetCreatureTemplate(e.m_Event.distance.entry))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_CREATURE using invalid creature entry %u, skipped.", e.m_Event.distance.entry);
                    return false;
                }
                break;
            }
            case SMART_EVENT_DISTANCE_GAMEOBJECT:
            {
                if (e.m_Event.distance.guid == 0 && e.m_Event.distance.entry == 0)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_GAMEOBJECT did not provide gameobject guid or entry, skipped.");
                    return false;
                }

                if (e.m_Event.distance.guid != 0 && e.m_Event.distance.entry != 0)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_GAMEOBJECT provided both an entry and guid, skipped.");
                    return false;
                }

                if (e.m_Event.distance.guid != 0 && !sObjectMgr->GetGOData(e.m_Event.distance.guid))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_GAMEOBJECT using invalid gameobject guid %u, skipped.", e.m_Event.distance.guid);
                    return false;
                }

                if (e.m_Event.distance.entry != 0 && !sObjectMgr->GetGameObjectTemplate(e.m_Event.distance.entry))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_GAMEOBJECT using invalid gameobject entry %u, skipped.", e.m_Event.distance.entry);
                    return false;
                }
                break;
            }
            case SMART_EVENT_DISTANCE_PLAYER:
            {
                if (!e.m_Event.playerDistance.aura && e.m_Event.playerDistance.isPresent)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Event SMART_EVENT_DISTANCE_PLAYER no aura set but isPresent is set, skipped.");
                    return false;
                }

                if (e.m_Event.playerDistance.aura && !IsSpellValid(e, e.m_Event.playerDistance.aura))
                    return false;
                break;
            }
            case SMART_EVENT_GO_STATE_CHANGED:
            case SMART_EVENT_GO_EVENT_INFORM:
            case SMART_EVENT_TIMED_EVENT_TRIGGERED:
            case SMART_EVENT_BEFORE_DEATH:
            case SMART_EVENT_VEHICLE_EXIT_REQUSTED:
            case SMART_EVENT_CHARMED:
            case SMART_EVENT_CHARMED_TARGET:
            case SMART_EVENT_CORPSE_REMOVED:
            case SMART_EVENT_AI_INIT:
            case SMART_EVENT_AURA_APPLIED:
            case SMART_EVENT_AURA_REMOVED:
            case SMART_EVENT_AGGRO:
            case SMART_EVENT_DEATH:
            case SMART_EVENT_EVADE:
            case SMART_EVENT_REACHED_HOME:
            case SMART_EVENT_RESET:
            case SMART_EVENT_JUST_SUMMONED:
            case SMART_EVENT_WAYPOINT_START:
            case SMART_EVENT_WAYPOINT_REACHED:
            case SMART_EVENT_WAYPOINT_PAUSED:
            case SMART_EVENT_WAYPOINT_RESUMED:
            case SMART_EVENT_AFTER_PASSENGER_REMOVED:
            case SMART_EVENT_WAYPOINT_STOPPED:
            case SMART_EVENT_WAYPOINT_ENDED:
            case SMART_EVENT_GOSSIP_SELECT:
            case SMART_EVENT_GOSSIP_HELLO:
            case SMART_EVENT_JUST_CREATED:
            case SMART_EVENT_FOLLOW_COMPLETED:
            case SMART_EVENT_ON_SPELLCLICK:
            case SMART_EVENT_QUEST_ACCEPT:
            case SMART_EVENT_QUEST_COMPLETE:
            case SMART_EVENT_QUEST_REWARD:
            case SMART_EVENT_QUEST_FAIL:
            case SMART_EVENT_QUEST_ABANDON:
                break;
            default:
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Not handled event_type(%u), Entry %d SourceType %u Event %u Action %u, skipped.", e.GetEventType(), e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                return false;
        }
    }

    switch (e.GetActionType())
    {
        case SMART_ACTION_SET_FACTION:
            if (e.action.faction.factionID && !sFactionTemplateStore.LookupEntry(e.action.faction.factionID))
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Faction %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.faction.factionID);
                return false;
            }
            break;
        case SMART_ACTION_MORPH_TO_ENTRY_OR_MODEL:
        case SMART_ACTION_MOUNT_TO_ENTRY_OR_MODEL:
            if (e.action.morphOrMount.creature || e.action.morphOrMount.model)
            {
                if (e.action.morphOrMount.creature > 0 && !sObjectMgr->GetCreatureTemplate(e.action.morphOrMount.creature))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Creature entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.morphOrMount.creature);
                    return false;
                }

                if (e.action.morphOrMount.model)
                {
                    if (e.action.morphOrMount.creature)
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u has ModelID set with also set CreatureId, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                        return false;
                    }
                    else if (!sCreatureDisplayInfoStore.LookupEntry(e.action.morphOrMount.model))
                    {
                        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Model id %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.morphOrMount.model);
                        return false;
                    }
                }
            }
            break;

        case SMART_ACTION_SOUND:
            if (!IsSoundValid(e, e.action.sound.sound))
                return false;
            break;

        case SMART_ACTION_SET_EMOTE_STATE:
        case SMART_ACTION_PLAY_EMOTE:
            if (!IsEmoteValid(e, e.action.emote.emote))
                return false;
            break;

        case SMART_ACTION_FAIL_QUEST:
        case SMART_ACTION_ADD_QUEST:
            if (!e.action.quest.quest || !IsQuestValid(e, e.action.quest.quest))
                return false;
            break;

        case SMART_ACTION_ACTIVATE_TAXI:
            {
                if (!sTaxiPathStore.LookupEntry(e.action.taxi.id))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses invalid Taxi path ID %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.taxi.id);
                    return false;
                }
                break;
            }

        case SMART_ACTION_RANDOM_EMOTE:

            if (std::all_of(std::begin(e.action.randomEmote.emotes), std::end(e.action.randomEmote.emotes), [](uint32 emote) { return emote == 0; }))
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u does not have any non-zero emote",
                    e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                return false;
            }

            for (uint32 emote : e.action.randomEmote.emotes)
                if (emote && !IsEmoteValid(e, emote))
                    return false;

            break;
        case SMART_ACTION_ADD_AURA:
        case SMART_ACTION_CAST:
        case SMART_ACTION_INVOKER_CAST:
            if (!IsSpellValid(e, e.action.cast.spell))
                return false;
            break;
        case SMART_ACTION_CALL_AREAEXPLOREDOREVENTHAPPENS:
        case SMART_ACTION_CALL_GROUPEVENTHAPPENS:
            if (Quest const* qid = sObjectMgr->GetQuestTemplate(e.action.quest.quest))
            {
                if (!qid->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u SpecialFlags for Quest entry %u does not include FLAGS_EXPLORATION_OR_EVENT(2), skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.quest.quest);
                    return false;
                }
            }
            else
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Quest entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.quest.quest);
                return false;
            }
            break;
        case SMART_ACTION_SET_EVENT_PHASE:
            if (e.action.setEventPhase.phase >= SMART_EVENT_PHASE_MAX)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u attempts to set phase %u. Phase mask cannot be used past phase %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.setEventPhase.phase, SMART_EVENT_PHASE_MAX-1);
                return false;
            }
            break;
        case SMART_ACTION_INC_EVENT_PHASE:
            if (!e.action.incEventPhase.inc && !e.action.incEventPhase.dec)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u is incrementing phase by 0, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                return false;
            }
            else if (e.action.incEventPhase.inc > SMART_EVENT_PHASE_MAX || e.action.incEventPhase.dec > SMART_EVENT_PHASE_MAX)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u attempts to increment phase by too large value, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                return false;
            }
            break;
        case SMART_ACTION_REMOVE_AURAS:
            if (e.action.RemoveAuras.SpellId != 0 && !IsSpellValid(e, e.action.RemoveAuras.SpellId))
                return false;
            break;

        case SMART_ACTION_RANDOM_PHASE:
        {
            if (std::all_of(std::begin(e.action.randomPhase.phases), std::end(e.action.randomPhase.phases), [](uint32 phase) { return phase == 0; }))
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u does not have any non-zero phase",
                    e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                return false;
            }

            if (std::any_of(std::begin(e.action.randomPhase.phases), std::end(e.action.randomPhase.phases), [](uint32 phase) { return phase >= SMART_EVENT_PHASE_MAX; }))
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u attempts to set invalid phase, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                return false;
            }

            break;
        }

        case SMART_ACTION_RANDOM_PHASE_RANGE:       //PhaseMin, PhaseMax
            {
                if (e.action.randomPhaseRange.phaseMin >= SMART_EVENT_PHASE_MAX ||
                    e.action.randomPhaseRange.phaseMax >= SMART_EVENT_PHASE_MAX)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u attempts to set invalid phase, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
                    return false;
                }
                if (!IsMinMaxValid(e, e.action.randomPhaseRange.phaseMin, e.action.randomPhaseRange.phaseMax))
                    return false;
                break;
            }
        case SMART_ACTION_SUMMON_CREATURE:
        {
            if (!IsCreatureValid(e, e.action.summonCreature.Entry))
                return false;

            if (e.action.summonCreature.SummonType < TEMPSUMMON_TIMED_OR_DEAD_DESPAWN || e.action.summonCreature.SummonType > TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT_NO_RESET)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses incorrect TempSummonType %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.summonCreature.SummonType);
                return false;
            }
            break;
        }
        case SMART_ACTION_CALL_KILLEDMONSTER:
        {
            if (!IsCreatureValid(e, e.action.killedMonster.creature))
                return false;
            break;
        }
        case SMART_ACTION_UPDATE_TEMPLATE:
            if (e.action.updateTemplate.creature && !IsCreatureValid(e, e.action.updateTemplate.creature))
                return false;
            break;
        case SMART_ACTION_SET_SHEATH:
            if (e.action.setSheath.sheath && e.action.setSheath.sheath >= MAX_SHEATH_STATE)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses incorrect Sheath state %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.setSheath.sheath);
                return false;
            }
            break;
        case SMART_ACTION_SET_REACT_STATE:
            {
                if (e.action.react.state > REACT_AGGRESSIVE)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Creature %d Event %u Action %u uses invalid React State %u, skipped.", e.entryOrGuid, e.event_id, e.GetActionType(), e.action.react.state);
                    return false;
                }
                break;
            }
        case SMART_ACTION_SUMMON_GO:
        {
            if (!IsGameObjectValid(e, e.action.summonGO.entry))
                return false;
            break;
        }
        case SMART_ACTION_ADD_ITEM:
        case SMART_ACTION_REMOVE_ITEM:
            if (!IsItemValid(e, e.action.item.entry))
                return false;

            if (!NotNULL(e, e.action.item.count))
                return false;
            break;
        case SMART_ACTION_TELEPORT:
            if (!sMapStore.LookupEntry(e.action.teleport.mapID))
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Map entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.teleport.mapID);
                return false;
            }
            break;
        case SMART_ACTION_SET_COUNTER:
            if (e.action.setCounter.counterId == 0)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry " SI64FMTD " SourceType %u Event %u Action %u uses wrong counterId %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.setCounter.counterId);
                return false;
            }

            if (e.action.setCounter.value == 0)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry " SI64FMTD " SourceType %u Event %u Action %u uses wrong value %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.setCounter.value);
                return false;
            }

            break;
        case SMART_ACTION_INSTALL_AI_TEMPLATE:
            if (e.action.installTtemplate.id >= SMARTAI_TEMPLATE_END)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Creature %d Event %u Action %u uses non-existent AI template id %u, skipped.", e.entryOrGuid, e.event_id, e.GetActionType(), e.action.installTtemplate.id);
                return false;
            }
            break;
        case SMART_ACTION_WP_STOP:
            if (e.action.wpStop.quest && !IsQuestValid(e, e.action.wpStop.quest))
                return false;
            break;
        case SMART_ACTION_WP_START:
            {
                if (!sSmartWaypointMgr->GetPath(e.action.wpStart.pathID))
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Creature %d Event %u Action %u uses non-existent WaypointPath id %u, skipped.", e.entryOrGuid, e.event_id, e.GetActionType(), e.action.wpStart.pathID);
                    return false;
                }
                if (e.action.wpStart.reactState > REACT_AGGRESSIVE)
                {
                    TC_LOG_ERROR("sql.sql", "SmartAIMgr: Creature %d Event %u Action %u uses invalid React State %u, skipped.", e.entryOrGuid, e.event_id, e.GetActionType(), e.action.wpStart.reactState);
                    return false;
                }
                break;
            }
        case SMART_ACTION_CREATE_TIMED_EVENT:
        {
            if (!IsMinMaxValid(e, e.action.timeEvent.min, e.action.timeEvent.max))
                return false;

            if (!IsMinMaxValid(e, e.action.timeEvent.repeatMin, e.action.timeEvent.repeatMax))
                return false;
            break;
        }

        case SMART_ACTION_CALL_RANDOM_RANGE_TIMED_ACTIONLIST:
        {
            if (!IsMinMaxValid(e, e.action.randTimedActionList.actionLists[0], e.action.randTimedActionList.actionLists[1]))
                return false;
            break;
        }

        case SMART_ACTION_EQUIP:
        {
            if (e.GetScriptType() == SMART_SCRIPT_TYPE_CREATURE)
            {
                int32 equipId = e.action.equip.entry;
                EquipmentInfo const* einfo = sObjectMgr->GetEquipmentInfo(equipId);
                if (!einfo)
                {
                    TC_LOG_ERROR("sql.sql", "SmartScript: SMART_ACTION_EQUIP uses non-existent equipment info id %u for creature %u, skipped.", equipId, e.entryOrGuid);
                    return false;
                }
            }

            break;
        }
        case SMART_ACTION_SET_POWER:
        case SMART_ACTION_ADD_POWER:
        case SMART_ACTION_REMOVE_POWER:
            if (e.action.power.powerType > MAX_POWERS)
            {
                TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Power %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), e.action.power.powerType);
                return false;
            }
            break;
		case SMART_ACTION_START_CLOSEST_WAYPOINT:
        case SMART_ACTION_SUMMON_CREATURE_GROUP:
        case SMART_ACTION_DISABLE_EVADE:
        case SMART_ACTION_SET_ROOT:
        case SMART_ACTION_SET_GO_FLAG:
        case SMART_ACTION_ADD_GO_FLAG:
        case SMART_ACTION_REMOVE_GO_FLAG:
        case SMART_ACTION_FOLLOW:
        case SMART_ACTION_SET_ORIENTATION:
        case SMART_ACTION_STORE_TARGET_LIST:
        case SMART_ACTION_EVADE:
        case SMART_ACTION_FLEE_FOR_ASSIST:
		case SMART_ACTION_COMBAT_STOP:
        case SMART_ACTION_DIE:
        case SMART_ACTION_SET_IN_COMBAT_WITH_ZONE:
        case SMART_ACTION_SET_ACTIVE:
        case SMART_ACTION_STORE_VARIABLE_DECIMAL:
        case SMART_ACTION_WP_RESUME:
        case SMART_ACTION_KILL_UNIT:
        case SMART_ACTION_SET_INVINCIBILITY_HP_LEVEL:
        case SMART_ACTION_RESET_GOBJECT:
        case SMART_ACTION_ATTACK_START:
        case SMART_ACTION_THREAT_ALL_PCT:
        case SMART_ACTION_THREAT_SINGLE_PCT:
        case SMART_ACTION_SET_INST_DATA:
        case SMART_ACTION_SET_INST_DATA64:
        case SMART_ACTION_AUTO_ATTACK:
        case SMART_ACTION_ALLOW_COMBAT_MOVEMENT:
        case SMART_ACTION_CALL_FOR_HELP:
        case SMART_ACTION_SET_DATA:
        case SMART_ACTION_MOVE_FORWARD:
        case SMART_ACTION_SET_VISIBILITY:
        case SMART_ACTION_WP_PAUSE:
        case SMART_ACTION_SET_FLY:
        case SMART_ACTION_SET_RUN:
        case SMART_ACTION_SET_SWIM:
        case SMART_ACTION_FORCE_DESPAWN:
        case SMART_ACTION_SET_INGAME_PHASE_ID:
        case SMART_ACTION_SET_UNIT_FLAG:
        case SMART_ACTION_REMOVE_UNIT_FLAG:
        case SMART_ACTION_PLAYMOVIE:
        case SMART_ACTION_MOVE_TO_POS:
        case SMART_ACTION_RESPAWN_TARGET:
        case SMART_ACTION_CLOSE_GOSSIP:
        case SMART_ACTION_TRIGGER_TIMED_EVENT:
        case SMART_ACTION_REMOVE_TIMED_EVENT:
        case SMART_ACTION_OVERRIDE_SCRIPT_BASE_OBJECT:
        case SMART_ACTION_RESET_SCRIPT_BASE_OBJECT:
        case SMART_ACTION_ACTIVATE_GOBJECT:
        case SMART_ACTION_CALL_SCRIPT_RESET:
        case SMART_ACTION_SET_RANGED_MOVEMENT:
        case SMART_ACTION_CALL_TIMED_ACTIONLIST:
        case SMART_ACTION_SET_NPC_FLAG:
        case SMART_ACTION_ADD_NPC_FLAG:
        case SMART_ACTION_REMOVE_NPC_FLAG:
        case SMART_ACTION_TALK:
        case SMART_ACTION_SIMPLE_TALK:
        case SMART_ACTION_CROSS_CAST:
        case SMART_ACTION_CALL_RANDOM_TIMED_ACTIONLIST:
        case SMART_ACTION_RANDOM_MOVE:
        case SMART_ACTION_SET_UNIT_FIELD_ANIM_TIER:
        case SMART_ACTION_REMOVE_UNIT_FIELD_ANIM_TIER:
        case SMART_ACTION_INTERRUPT_SPELL:
        case SMART_ACTION_SEND_GO_CUSTOM_ANIM:
        case SMART_ACTION_SET_DYNAMIC_FLAG:
        case SMART_ACTION_ADD_DYNAMIC_FLAG:
        case SMART_ACTION_REMOVE_DYNAMIC_FLAG:
        case SMART_ACTION_JUMP_TO_POS:
        case SMART_ACTION_SEND_GOSSIP_MENU:
        case SMART_ACTION_GO_SET_LOOT_STATE:
        case SMART_ACTION_SEND_TARGET_TO_TARGET:
        case SMART_ACTION_SET_HOME_POS:
        case SMART_ACTION_SET_HEALTH_REGEN:
        case SMART_ACTION_MOVE_TO_POS_FIXED_ORIENTATION:
		case SMART_ACTION_ATTACK_STOP:
		case SMART_ACTION_IGNORE_PATHFINDING:
		case SMART_ACTION_MOVE_VERTICALLY:
		case SMART_ACTION_MOVE_BY_PATH:
		case SMART_ACTION_SET_SPEED:
		case SMART_ACTION_SET_CORPSE_DELAY:
		case SMART_ACTION_SET_ANIM_KIT:
		case SMART_ACTION_MOVE_FALL:
		case SMART_ACTION_DISABLE_TURN:
		case SMART_ACTION_HANDLE_DOOR:
		case SMART_ACTION_PLAY_MUSIC:
		case SMART_ACTION_MOVE_CIRCLE:
		case SMART_ACTION_THREAT_SINGLE:
		case SMART_ACTION_IGNORE_EVADE:
		case SMART_ACTION_CAST_RANDOM:
		case SMART_ACTION_PLAY_ONE_SHOT_ANIM_KIT:
		case SMART_ACTION_PLAY_SPELL_VISUAL_KIT:
        case SMART_ACTION_CUSTOM_MOVE_BY_PATH:
		case SMART_ACTION_PLAY_SPELL_VISUAL:
        case SMART_ACTION_PREVENT_EVENT_PHASE_RESET:
        case SMART_ACTION_SET_VEHICLE_ID:
        case SMART_ACTION_PLAYER_TALK:
        case SMART_ACTION_STOP_MOVEMENT:
        case SMART_ACTION_REMOVE_AURAS_BY_TYPE:
        case SMART_ACTION_OVERWRITE_INVOKER:
        case SMART_ACTION_SEND_QUEST_EVENT:
        case SMART_ACTION_CUSTOM_MOVE_BY_PATHTWO:
        case SMART_ACTION_SET_STAND_STATE:
        case SMART_ACTION_APPLY_STAND_FLAGS:
        case SMART_ACTION_APPLY_STAND_MISC_FLAGS:
        case SMART_ACTION_PLAY_RANDOM_SOUND:
            /// AZENOR CUSTOM
        case SMART_ACTION_PAUSE_WAYPOINT_FOR_MS:
        case SMART_ACTION_REWARD_LFG_DUNGEON:
        case SMART_ACTION_DO_DIRECT_AI_TALK:
            break;
        default:
            TC_LOG_ERROR("sql.sql", "SmartAIMgr: Not handled action_type(%u), event_type(%u), Entry %d SourceType %u Event %u, skipped.", e.GetActionType(), e.GetEventType(), e.entryOrGuid, e.GetScriptType(), e.event_id);
            return false;
    }

    return true;
}

void SmartAIMgr::LoadHelperStores()
{
    uint32 oldMSTime = getMSTime();

    SpellInfo const* spellInfo = NULL;
    for (uint32 i = 0; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
    {
        spellInfo = sSpellMgr->GetSpellInfo(i);
        if (!spellInfo)
            continue;

        for (uint32 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            if (spellInfo->Effects[j].IsEffect(SPELL_EFFECT_SUMMON))
                SummonCreatureSpellStore.insert(std::make_pair(uint32(spellInfo->Effects[j].MiscValue), std::make_pair(i, SpellEffIndex(j))));

            else if (spellInfo->Effects[j].IsEffect(SPELL_EFFECT_SUMMON_OBJECT_WILD))
                SummonGameObjectSpellStore.insert(std::make_pair(uint32(spellInfo->Effects[j].MiscValue), std::make_pair(i, SpellEffIndex(j))));

            else if (spellInfo->Effects[j].IsEffect(SPELL_EFFECT_KILL_CREDIT) || spellInfo->Effects[j].IsEffect(SPELL_EFFECT_KILL_CREDIT2))
                KillCreditSpellStore.insert(std::make_pair(uint32(spellInfo->Effects[j].MiscValue), std::make_pair(i, SpellEffIndex(j))));
        }
    }

    TC_LOG_INFO("server.loading", ">> Loaded SmartAIMgr Helpers in %u ms", GetMSTimeDiffToNow(oldMSTime));
}

void SmartAIMgr::UnLoadHelperStores()
{
    SummonCreatureSpellStore.clear();
    SummonGameObjectSpellStore.clear();
    KillCreditSpellStore.clear();
}

CacheSpellContainerBounds SmartAIMgr::GetSummonCreatureSpellContainerBounds(uint32 creatureEntry) const
{
    return SummonCreatureSpellStore.equal_range(creatureEntry);
}

CacheSpellContainerBounds SmartAIMgr::GetSummonGameObjectSpellContainerBounds(uint32 gameObjectEntry) const
{
    return SummonGameObjectSpellStore.equal_range(gameObjectEntry);
}

CacheSpellContainerBounds SmartAIMgr::GetKillCreditSpellContainerBounds(uint32 killCredit) const
{
    return KillCreditSpellStore.equal_range(killCredit);
}

ObjectGuidVector::ObjectGuidVector(ObjectVector const& objectVector) : _objectVector(objectVector)
{
    _guidVector.reserve(_objectVector.size());
    for (WorldObject* obj : _objectVector)
        _guidVector.push_back(obj->GetGUID());
}

void ObjectGuidVector::UpdateObjects(WorldObject const& ref) const
{
    _objectVector.clear();

    for (uint64 guid: _guidVector)
        if (WorldObject* obj = ObjectAccessor::GetWorldObject(ref, guid))
            _objectVector.push_back(obj);
}

bool SmartAIMgr::IsMinMaxValid(SmartScriptHolder const& e, uint32& min, uint32& max)
{
    max = std::max(min, max);

    return true;
}

bool SmartAIMgr::NotNULL(SmartScriptHolder const& e, uint32 data)
{
    if (!data)
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u Parameter can not be NULL, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType());
        return false;
    }
    return true;
}

bool SmartAIMgr::IsCreatureValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sObjectMgr->GetCreatureTemplate(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Creature entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}

bool SmartAIMgr::IsQuestValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sObjectMgr->GetQuestTemplate(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Quest entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}

bool SmartAIMgr::IsGameObjectValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sObjectMgr->GetGameObjectTemplate(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent GameObject entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}

bool SmartAIMgr::IsSpellValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sSpellMgr->GetSpellInfo(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Spell entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}

bool SmartAIMgr::IsItemValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sItemStore.LookupEntry(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Item entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}

bool SmartAIMgr::IsTextEmoteValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sEmotesTextStore.LookupEntry(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Text Emote entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}

bool SmartAIMgr::IsEmoteValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sEmotesStore.LookupEntry(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Emote entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}

bool SmartAIMgr::IsAreaTriggerValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sAreaTriggerStore.LookupEntry(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent AreaTrigger entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}

bool SmartAIMgr::IsSoundValid(SmartScriptHolder const& e, uint32 entry)
{
    if (!sSoundEntriesStore.LookupEntry(entry))
    {
        TC_LOG_ERROR("sql.sql", "SmartAIMgr: Entry %d SourceType %u Event %u Action %u uses non-existent Sound entry %u, skipped.", e.entryOrGuid, e.GetScriptType(), e.event_id, e.GetActionType(), entry);
        return false;
    }
    return true;
}