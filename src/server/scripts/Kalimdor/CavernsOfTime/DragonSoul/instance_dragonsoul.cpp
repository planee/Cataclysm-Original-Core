#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "CreatureTextMgr.h"
#include "ScriptMgr.h"
#include "dragonsoul.h"
#include "Group.h"
#include "LFGMgr.h"
#include "Pet.h"

ObjectData const creatureData[] =
{
    { NPC_MORCHOK,                      DATA_MORCHOK                    },
    { NPC_WARLORD_ZONOZZ,               DATA_WARLORD_ZONOZZ             },
    { NPC_HAGARA,                       DATA_HAGARA                     },
    { NPC_ALEXSTRASZA_ULTRAXION,        DATA_ALEXSTRASZA_ULTRAXION      },
    { NPC_KALECGOS_ULTRAXION,           DATA_KALECGOS_ULTRAXION         },
    { NPC_YSERA_ULTRAXION,              DATA_YSERA_ULTRAXION            },
    { NPC_NOZDORMU_ULTRAXION,           DATA_NOZDORMU_ULTRAXION         },
    { NPC_THRALL_ULTRAXION,             DATA_THRALL_ULTRAXION           },
    { NPC_DEATHWING_ULTRAXION,          DATA_DEATHWING_ULTRAXION        },
    { NPC_ULTRAXION_GAUNTLET,           DATA_GAUNTLET_TRIGGER           },
    { NPC_THE_DRAGON_SOUL_ULTRAXION,    DATA_THE_DRAGON_SOUL_ULTRAXION  },
    { NPC_YORSAHJ,                      DATA_YORSAHJ                    },
    { NPC_ULTRAXION,                    DATA_ULTRAXION                  },
    { NPC_DW_SPINE,                     DATA_SPINE_OF_DEATHWING         },
    { NPC_DW_MADNESS,                   DATA_MADNESS_OF_DEATHWING       },
    { NPC_ALEXSTRASZA,                  NPC_ALEXSTRASZA                 },
    { NPC_NOZDORMU,                     NPC_NOZDORMU                    },
    { NPC_YSERA,                        NPC_YSERA                       },
    { NPC_KALECGOS,                     NPC_KALECGOS                    },
    { NPC_DW_MADNESS_THRALL,            NPC_DW_MADNESS_THRALL           },
    { 0,                                0                               } // END
};

ObjectData const gameObjectData[] =
{
    { GO_ALLIANCE_SHIP_CINEMATIC,               DATA_ALLIANCE_SHIP_CINEMATIC},
    { GO_HORDE_SHIP_CINEMATIC,                  DATA_HORDE_SHIP_CINEMATIC   },
    { GO_ALLIANCE_SHIP_BATTLE,                  DATA_ALLIANCE_BATTLE_SHIP   },
    { GO_DW_BACKPLATE_1,                        DATA_DW_SPINE_BACKPLATE_1   },
    { GO_DW_BACKPLATE_2,                        DATA_DW_SPINE_BACKPLATE_2   },
    { GO_DW_BACKPLATE_3,                        DATA_DW_SPINE_BACKPLATE_3   },
    { GO_LESSER_CACHE_OF_THE_ASPECTS_10,        DATA_ULTRAXION_CHEST        },
    { GO_LESSER_CACHE_OF_THE_ASPECTS_25,        DATA_ULTRAXION_CHEST        },
    { GO_LESSER_CACHE_OF_THE_ASPECTS_10_HC,     DATA_ULTRAXION_CHEST        },
    { GO_LESSER_CACHE_OF_THE_ASPECTS_25_HC,     DATA_ULTRAXION_CHEST        },
    { GO_LESSER_CACHE_OF_THE_ASPECTS_LFR,       DATA_ULTRAXION_CHEST_LFR    },
    { GO_GREATER_CACHE_OF_THE_ASPECTS_10,       DATA_SPINE_CHEST            },
    { GO_GREATER_CACHE_OF_THE_ASPECTS_25,       DATA_SPINE_CHEST            },
    { GO_GREATER_CACHE_OF_THE_ASPECTS_10_HC,    DATA_SPINE_CHEST            },
    { GO_GREATER_CACHE_OF_THE_ASPECTS_25_HC,    DATA_SPINE_CHEST            },
    { GO_GREATER_CACHE_OF_THE_ASPECTS_LFR,      DATA_SPINE_CHEST_LFR        },
    { GO_ELEMENTIUM_FRAGMENT_10,                DATA_MADNESS_CHEST          },
    { GO_ELEMENTIUM_FRAGMENT_25,                DATA_MADNESS_CHEST          },
    { GO_ELEMENTIUM_FRAGMENT_10_HC,             DATA_MADNESS_CHEST          },
    { GO_ELEMENTIUM_FRAGMENT_25_HC,             DATA_MADNESS_CHEST          },
    { GO_ELEMENTIUM_FRAGMENT_LFR,               DATA_MADNESS_CHEST_LFR      },
    { 0,                                0                               } // END
};

class instance_dragonsoul : public InstanceMapScript
{
    public:
        instance_dragonsoul() : InstanceMapScript(DragonsoulScriptName, 967) { }

        struct instance_dragonsoul_InstanceMapScript : public InstanceScript
        {
            instance_dragonsoul_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(MAX_ENCOUNTERS);
                LoadObjectData(creatureData, gameObjectData);
                raidMode = RAID_MODE_UNKNOWN;
                raidNerfData = RAID_NERF_ACTIVE;
                backplateCount = 0;
                mana = 0;
                activePlatform = 0;
                LfrId = 0;
                platformCount = 0;
            }

            void FillInitialWorldStates(WorldPacket& data) override
            {
                data << uint32(WS_BLACK_YELLOW) << uint32(0);
                data << uint32(WS_RED_GREEN) << uint32(0);
                data << uint32(WS_BLACK_BLUE) << uint32(0);
                data << uint32(WS_PURPLE_YELLOW) << uint32(0);
            }

            void OnPlayerEnter(Player* player) override
            {
                if (!raidMode)
                {
                    if (Group* grp = player->GetGroup())
                    {
                        raidMode = (instance->Is25ManRaid() && grp->isLFRGroup()) ? RAID_MODE_LFR : RAID_MODE_NORMAL;

                        if (raidMode == RAID_MODE_LFR && LfrId == 0)
                            LfrId = sLFGMgr->GetDungeon(grp->GetGUID());
                    }
                }
            }

            void OnPlayerLeave(Player* player) override
            {
                player->RemoveAurasDueToSpell(106005); // SPELL_DEGRADATION
                player->RemoveAurasDueToSpell(106213);

                if (Pet* pet = player->GetPet())
                    pet->RemoveAurasDueToSpell(106005);
            }

            void OnCreatureCreate(Creature* creature) override
            {
                if (!raidMode)
                {
                    Map::PlayerList const& players = instance->GetPlayers();
                    if (!players.isEmpty())
                        if (Player* player = players.begin()->getSource())
                            if (Group* grp = player->GetGroup())
                            {
                                raidMode = (instance->Is25ManRaid() && grp->isLFRGroup()) ? RAID_MODE_LFR : RAID_MODE_NORMAL;

                                if (raidMode == RAID_MODE_LFR && LfrId == 0)
                                    LfrId = sLFGMgr->GetDungeon(grp->GetGUID());
                            }
                }

                if (raidMode == RAID_MODE_LFR)
                {
                    creature->CastSpell(creature, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, true);
                    creature->SetLootMode(LOOT_MODE_LFR);
                }
                else if (raidNerfData == RAID_NERF_ACTIVE)
                {
                    dragonSoulNpcs.push_back(creature->GetGUID());
                    int32 currentNormalAndHcNerfAmount = -5;
                    creature->CastCustomSpell(creature, SPELL_PRESENCE_OF_THE_DRAGON_SOUL, &currentNormalAndHcNerfAmount, &currentNormalAndHcNerfAmount, NULL, true);
                    creature->CastSpell(creature, SPELL_POWER_OF_THE_ASPECTS, true);
                }

                switch (creature->GetEntry())
                {
                    case NPC_THRALL_ULTRAXION: // ultraxion event
                    case NPC_KALECGOS_ULTRAXION:
                    case NPC_ALEXSTRASZA_ULTRAXION:
                    case NPC_YSERA_ULTRAXION:
                    case NPC_NOZDORMU_ULTRAXION:
                    case NPC_THE_DRAGON_SOUL_ULTRAXION:
                        if (IsDone(DATA_ULTRAXION))
                            creature->SetVisible(false);
                        break;
                    case NPC_ANCIENT_WATER_LORD: // morchok trash
                    case NPC_EARTHEN_DESTROYER:
                    case NPC_TWILIGHT_SIEGE_CAPTAIN:
                    case NPC_TWILIGHT_PORTAL:
                    case NPC_EARTHEN_SOLDIER:
                        npcGroups.emplace(DATA_MORCHOK, creature->GetGUID());
                        if (IsDone(DATA_MORCHOK) || LfrId == LFR_PART_TWO)
                            creature->SetPhaseMask(0x2, true);
                        break;
                    case NPC_FLAIL_OF_GORATH_TRASH: // wing trash + bosses
                    case NPC_EYE_OF_GORATH_TRASH:
                    case NPC_GLOWING_GLOBULE:
                    case NPC_ACIDIC_GLOBULE:
                    case NPC_CRIMSON_GLOBULE:
                    case NPC_DARK_GLOBULE:
                    case NPC_SHADOWED_GLOBULE:
                    case NPC_COBALT_GLOBULE:
                    case NPC_WARLORD_ZONOZZ:
                    case NPC_YORSAHJ:
                        wingTrash.push_back(creature->GetGUID());
                        if (!IsDone(DATA_MORCHOK) || LfrId == LFR_PART_TWO)
                            creature->SetPhaseMask(0x2, true);
                        break;
                    case NPC_FACELESS_DESTROYER: // part 1 show and temple spawns
                    case NPC_FACELESS_CORRUPTOR:
                    case NPC_WYRMREST_DEFENDER:
                    case NPC_VALEERA:
                    case NPC_LETHECUS:
                    case NPC_DALORMI:
                    case NPC_DASNURIMI:
                    case NPC_CRIMSON_LIFEBINDER:
                    case NPC_TRIZORMU:
                    case NPC_MR_STOMP:
                    case NPC_GORIN:
                    case NPC_EIENDORMI:
                    case NPC_FERITHOS:
                    case NPC_LORD_AFRASASTRASZ:
                    case NPC_TYRYGOSA:
                    case NPC_DEMESTRASZ:
                    case NPC_WYRMREST_CAPTAIN:
                    case NPC_WYRMREST_DEFENDER_2:
                    case NPC_WYRMREST_PROTECTOR:
                    case NPC_WYRMREST_PROTECTOR_2:
                    case NPC_RUIN_TENTACLE:
                        wingTrash.push_back(creature->GetGUID());
                        if (!IsDone(DATA_MORCHOK) && LfrId != LFR_PART_TWO)
                            creature->SetPhaseMask(0x2, true);
                        break;
                    case NPC_THRALL_OUTRO: // madness outro
                    case NPC_AGGRA_OUTRO:
                    case NPC_ASPECT_1_OUTRO:
                    case NPC_ASPECT_2_OUTRO:
                    case NPC_ASPECT_3_OUTRO:
                    case NPC_ASPECT_4_OUTRO:
                        if (!IsDone(DATA_MADNESS_OF_DEATHWING))
                            creature->SetVisible(false);
                        npcGroups.emplace(DATA_MADNESS_OF_DEATHWING, creature->GetGUID());
                        break;
                    case NPC_TRAVEL_TO_SUMMIT: // taxi to ultraxion
                    case NPC_NETHESTRASZ:
                        if (!IsDone(DATA_YORSAHJ) && !IsDone(DATA_WARLORD_ZONOZZ) && LfrId != LFR_PART_TWO)
                            creature->SetVisible(false);
                        npcGroups.emplace(DATA_YORSAHJ, creature->GetGUID());
                        break;
                    case NPC_TRAVEL_TO_THE_MAELSTROM:
                        if (!IsDone(DATA_SPINE_OF_DEATHWING))
                            creature->SetVisible(false);
                        npcGroups.emplace(DATA_SPINE_OF_DEATHWING, creature->GetGUID());
                        break;
                    case NPC_MORCHOK:
                        if (LfrId == LFR_PART_TWO)
                            creature->SetPhaseMask(0x2, true);
                        break;
                    default:
                        break;
                }

                InstanceScript::OnCreatureCreate(creature);
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    case GO_FOCUSING_IRIS:
                        if (IsDone(DATA_HAGARA))
                            go->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_ADMINISTRATOR);
                        break;
                    case GO_ALLIANCE_SHIP_BATTLE:
                        if (IsDone(DATA_ULTRAXION))
                        {
                            instance->SummonCreature(NPC_SKY_CAPTAIN_SWAYZE, skyCaptainPos);
                            go->SetDestructibleState(GO_DESTRUCTIBLE_INTACT);
                        }
                        break;
                    case GO_ALLIANCE_SHIP_CINEMATIC:
                    case GO_HORDE_SHIP_CINEMATIC:
                        if (IsDone(DATA_ULTRAXION))
                            go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                        break;
                    default:
                        break;
                }
                InstanceScript::OnGameObjectCreate(go);
            }

            uint32 GetData(uint32 data) const override
            {
                switch (data)
                {
                    case DATA_RAID_MODE:
                        return raidMode;
                    case DATA_DW_SPINE_BACKPLATE_CNT:
                        return backplateCount;
                    case DATA_DW_SPINE_ROLL_INFO:
                        return rollInfo.empty() ? 0 : rollInfo.rbegin()->second;
                    case DATA_LFR_PART:
                        return LfrId;
                    case DATA_MADNESS_PLATFORM_COUNT:
                        return platformCount;
                    case DATA_RAID_NERF:
                        return raidNerfData;
                }
                return 0;
            }

            void SetData(uint32 type, uint32 value) override
            {
                switch (type)
                {
                    case DATA_DW_SPINE_BACKPLATE_CNT:
                        backplateCount = value;
                        break;
                    case DATA_DW_SPINE_ROLL_INFO:
                    {
                        if (!value)
                        {
                            rollInfo.clear();
                            return;
                        }
                        rollInfo[uint32(time(NULL))] = value;
                        uint32 now = time(NULL);
                        uint32 firstValue = rollInfo.cbegin()->second;
                        if (rollInfo.size() == 5)
                            if (std::all_of(rollInfo.cbegin(), rollInfo.cend(), ([firstValue](std::pair<uint32, uint32> u) { return u.second == firstValue; })))
                                if (Creature* spine = GetCreature(DATA_SPINE_OF_DEATHWING))
                                    spine->AI()->DoAction(firstValue); // ACTION_ROLL
                        break;
                    }
                    case DATA_MADNESS_PLATFORM_COUNT:
                        platformCount = value;
                        break;
                    case DATA_RAID_NERF:
                        raidNerfData = value;
                        if (value == RAID_NERF_INACTIVE && raidMode != RAID_MODE_LFR)
                        {
                            for (uint64 guid : dragonSoulNpcs)
                            {
                                if (Creature* creature = instance->GetCreature(guid))
                                {
                                    creature->RemoveAurasDueToSpell(SPELL_PRESENCE_OF_THE_DRAGON_SOUL);
                                    creature->RemoveAurasDueToSpell(SPELL_POWER_OF_THE_ASPECTS);
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }

            void SetData64(uint32 type, uint64 value) override
            {
                activePlatform = value;
            }

            uint64 GetData64(uint32 type) const override
            {
                return activePlatform;
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_MORCHOK:
                        if (state == DONE || state == DONE_HM)
                        {
                            for (auto itr : npcGroups)
                                if (itr.first == DATA_MORCHOK)
                                    if (Creature* trashMob = instance->GetCreature(itr.second))
                                        trashMob->SetPhaseMask(0x2, true);

                            for (uint64 trashGUID : wingTrash)
                                if (Creature* trashMob = instance->GetCreature(trashGUID))
                                    trashMob->SetPhaseMask(0x1, true);
                        }
                        break;
                    case DATA_MADNESS_OF_DEATHWING:
                        if (state == DONE || state == DONE_HM)
                        {
                            for (auto itr : npcGroups)
                                if (itr.first == DATA_MADNESS_OF_DEATHWING)
                                    if (Creature* trashMob = instance->GetCreature(itr.second))
                                        trashMob->SetVisible(true);
                        }
                        if (state == IN_PROGRESS || state == FAIL)
                        {
                            for (auto itr : npcGroups)
                                if (itr.first == DATA_SPINE_OF_DEATHWING)
                                    if (Creature* trashMob = instance->GetCreature(itr.second))
                                        trashMob->SetVisible(state == FAIL ? true : false);
                        }
                        break;
                    case DATA_SPINE_OF_DEATHWING:
                        if (state == DONE || state == DONE_HM)
                        {
                            for (auto itr : npcGroups)
                                if (itr.first == DATA_SPINE_OF_DEATHWING)
                                    if (Creature* trashMob = instance->GetCreature(itr.second))
                                        trashMob->SetVisible(true);
                        }
                    case DATA_YORSAHJ:
                    case DATA_WARLORD_ZONOZZ:
                        if (state == DONE || state == DONE_HM)
                        {
                            if (IsDone(DATA_YORSAHJ) && IsDone(DATA_WARLORD_ZONOZZ))
                            {
                                for (auto itr : npcGroups)
                                    if (itr.first == DATA_YORSAHJ)
                                        if (Creature* trashMob = instance->GetCreature(itr.second))
                                            trashMob->SetVisible(true);
                            }
                        }
                    default:
                        break;
                }
                return true;
            }

        private:
            uint32 raidMode;
            uint32 mana;
            uint32 backplateCount;
            uint32 LfrId;
            uint32 platformCount;
            uint32 raidNerfData;
            uint64 activePlatform;
            std::map<uint32, uint32> rollInfo;
            std::multimap<uint32, uint64> npcGroups;
            std::vector<uint64> wingTrash;
            std::vector<uint64> dragonSoulNpcs;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_dragonsoul_InstanceMapScript(map);
        }
};

void AddSC_instance_dragonsoul()
{
    new instance_dragonsoul();
}
