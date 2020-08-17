/*
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

#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "CreatureTextMgr.h"
#include "ScriptMgr.h"
#include "firelands.h"

DoorData const doorData[] =
{
    { GO_DOOR_BETHILAC,         DATA_BETHTILAC,             DOOR_TYPE_ROOM,         BOUNDARY_SE   },
    { GO_VOLCANO_ALYS,          DATA_LORD_RHYOLITH,         DOOR_TYPE_ROOM,         BOUNDARY_NONE },
    { GO_RHYOLITH_BRIDGE,       DATA_LORD_RHYOLITH,         DOOR_TYPE_SPAWN_HOLE,   BOUNDARY_S    },
    { GO_DOOR_BALOROC_AFTER,    DATA_BALOROC,               DOOR_TYPE_PASSAGE,      BOUNDARY_S    },
    { GO_MAJ_DOOR,              DATA_MAJORDOMO_STAGHELM,    DOOR_TYPE_PASSAGE,      BOUNDARY_N    },
    { GO_MAJA_DOOR,             DATA_MAJORDOMO_STAGHELM,    DOOR_TYPE_ROOM,         BOUNDARY_NONE },
    { GO_RAGNAROS_DOOR,         DATA_RAGNAROS,              DOOR_TYPE_ROOM,         BOUNDARY_S    },
    { 0,                        0,                          DOOR_TYPE_ROOM,         BOUNDARY_NONE } // END
};

Position const Feather = { 0.760f, -305.456f, 53.5113f, 5.597f };

class instance_firelands : public InstanceMapScript
{
    public:
        instance_firelands() : InstanceMapScript("instance_firelands", 720) { }

        struct instance_firelands_InstanceMapScript : public InstanceScript
        {
            instance_firelands_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);
            }

            void Initialize() override
            {
                shannoxGUID           = 0;
                rhyolithGUID          = 0;
                bethtilacGUID         = 0;
                alysrazorGUID         = 0;
                balorocGUID           = 0;
                majordomoStaghelmGUID = 0;
                ragnarosGUID          = 0;
                bethtilacLiftGUID     = 0;
                magmaOrbGUID          = 0;
                slaintrash            = 0;
                shannoxTeleporterGUID = 0;
                majordomoTeleporterGUID = 0;
                shannoxControllerGUID = 0;
                dataKneelToFlame = 1;
                fandrallActivationCount = 0;
                fandralLeftActivationCount = 0;
                fandralRightActivationCount = 0;
                dataAlysGround        = BUILDING_STATE_INTACT;
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    case GO_RAGNAROS_PLATFORM:
                        HandleGameObject(0, false, go);
                        break;
                    case GO_MAJ_DOOR:
                    case GO_MAJA_DOOR:
                    case GO_RHYOLITH_BRIDGE:
                    case GO_DOOR_BETHILAC:
                    case GO_DOOR_BALOROC_AFTER:
                    case GO_RAGNAROS_DOOR:
                        AddDoor(go, true);
                        break;
                    case GO_SULFURON_BRIDGE:
                        sulfuronBridgeGUID = go->GetGUID();
                        if (IsDone(DATA_BALOROC) && IsDone(DATA_SHANNOX) && IsDone(DATA_LORD_RHYOLITH) && IsDone(DATA_BETHTILAC) && IsDone(DATA_ALYSRAZOR))
                            go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                        break;
                    case GO_VOLCANO_ALYS:
                        alysrazorVolcanoGUID = go->GetGUID();
                        AddDoor(go, true);
                        break;
                    case GO_CACHE_OF_THE_FIRELORD_10:
                    case GO_CACHE_OF_THE_FIRELORD_25:
                        cacheOfTheFirelordGUID = go->GetGUID();
                        go->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_GAMEMASTER);
                        break;
                    case GO_DOOR_BALOROC:
                        balorocDoorGUID = go->GetGUID();
                        HandleGameObject(0, GetBossState(DATA_BALOROC) != IN_PROGRESS && (shannoxGUID != 0 || IsDone(DATA_SHANNOX)), go);
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    case GO_MAJ_DOOR:
                    case GO_MAJA_DOOR:
                    case GO_RHYOLITH_BRIDGE:
                    case GO_DOOR_BETHILAC:
                    case GO_DOOR_BALOROC_AFTER:
                    case GO_VOLCANO_ALYS:
                    case GO_RAGNAROS_DOOR:
                        AddDoor(go, false);
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                    case BOSS_SHANNOX:
                        shannoxGUID = creature->GetGUID();
                        break;
                    case BOSS_RHYOLITH:
                        rhyolithGUID = creature->GetGUID();
                        break;
                    case BOSS_BETHTILAC:
                        bethtilacGUID = creature->GetGUID();
                        break;
                    case BOSS_ALYSRAZOR:
                        alysrazorGUID = creature->GetGUID();
                        break;
                    case BOSS_BALEROC:
                        balorocGUID = creature->GetGUID();
                        break;
                    case BOSS_MAJORDOMO_STAGHELM:
                        majordomoStaghelmGUID = creature->GetGUID();
                        break;
                    case BOSS_RAGNAROS:
                        ragnarosGUID = creature->GetGUID();
                        break;
                    case NPC_RAGEFACE:
                        ragefaceGUID = creature->GetGUID();
                        break;
                    case NPC_RIPLIMB:
                        riplimbGUID = creature->GetGUID();
                        break;
                    case NPC_SHANNOX_SPEAR:
                        shannoxSpearGUID = creature->GetGUID();
                        break;
                    case NPC_SMOULDERING_HATCHLING:
                        // Cannot directly start attacking here as the creature is not yet on map
                        creature->m_Events.AddEvent(new DelayedAttackStartEvent(creature), creature->m_Events.CalculateTime(500));
                        break;
                    case NPC_BETHTILAC_VEHICLE:
                        bethtilacLiftGUID = creature->GetGUID();
                        break;
                    case NPC_MAGMA_ORB:
                        magmaOrbGUID = creature->GetGUID();
                        break;
                    case NPC_SHANNOX_CONTROLLER:
                        shannoxControllerGUID = creature->GetGUID();
                        CheckShannoxState(creature);
                        break;
                    case NPC_MAJORDOMO_EVENT_INTRO:
                        if (!IsDone(DATA_ALYSRAZOR) && !creature->isAlive())
                            instance->SummonCreature(53089, Feather);
                        break;
                    case NPC_FIRELANDS_TELEPORT_MAJOR:
                        if (IsDone(DATA_BALOROC) && IsDone(DATA_SHANNOX) && IsDone(DATA_LORD_RHYOLITH) && IsDone(DATA_BETHTILAC) && IsDone(DATA_ALYSRAZOR))
                            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        majordomoTeleporterGUID = creature->GetGUID();
                        break;
                    case NPC_FIRELANDS_TELEPORT_SHANNOX:
                        if (IsDone(DATA_BALOROC) && IsDone(DATA_SHANNOX) && IsDone(DATA_LORD_RHYOLITH) && IsDone(DATA_BETHTILAC) && IsDone(DATA_ALYSRAZOR))
                            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        shannoxTeleporterGUID = creature->GetGUID();
                        break;
                }
            }

            void OnCreatureRemove(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                    case BOSS_SHANNOX:
                        shannoxGUID = 0;
                        break;
                    default:
                        break;
                }
            }

            uint64 GetData64(uint32 identifier) const override
            {
                switch (identifier)
                {
                    //BOSSES
                    case DATA_SHANNOX:                      return shannoxGUID;
                    case DATA_LORD_RHYOLITH:                return rhyolithGUID;
                    case DATA_BETHTILAC:                    return bethtilacGUID;
                    case DATA_ALYSRAZOR:                    return alysrazorGUID;
                    case DATA_BALOROC:                      return balorocGUID;
                    case DATA_MAJORDOMO_STAGHELM:           return majordomoStaghelmGUID;
                    case DATA_RAGNAROS:                     return ragnarosGUID;
                    //NPCs
                    case NPC_RAGEFACE:                      return ragefaceGUID;
                    case NPC_RIPLIMB:                       return riplimbGUID;
                    case NPC_SHANNOX_SPEAR:                 return shannoxSpearGUID;
                    case NPC_MAGMA_ORB:                     return magmaOrbGUID;
                    //GOs
                    case GO_SULFURON_BRIDGE:                return sulfuronBridgeGUID;
                    case GO_VOLCANO_ALYS:                   return alysrazorVolcanoGUID;
                    case GO_CACHE_OF_THE_FIRELORD_10:       return cacheOfTheFirelordGUID;
                    case NPC_BETHTILAC_VEHICLE:             return bethtilacLiftGUID;
                }

                return 0;
            }

            void OnUnitDeath(Unit* unit) override
            {
                if (Player *player = unit->ToPlayer())
                    if (player->GetAreaId() == 5764)
                        if (player->GetPositionZ() >= 100)
                        {
                            player->NearTeleportTo(player->GetPositionX(), player->GetPositionY(), 80.0f, player->GetOrientation());
                            player->GetMotionMaster()->MoveFall();
                            return;
                        }

                Creature* creature = unit->ToCreature();
                if (!creature)
                    return;

                switch (creature->GetEntry())
                {
                    case 53115:
                    case 53119:
                    case 53130:
                    case 53134:
                    case 53141:
                    case 53167:
                    case 53185:
                    case 53187:
                    case 53188:
                    case 53206:
                    case 53220:
                    case 53221:
                    case 53222:
                    case 53223:
                    case 53639:
                    case 53640:
                    case 53732:
                    case 53901:
                    case 54073:
                    case 53128:
                    case 53127:
                    case 53094:
                    case 53095:
                    case 53096:
                    case 53121:
                    case 53120:
                        slaintrash++;
                        CheckShannoxState();
                        SaveToDB();
                        break;
                    default:
                        break;
                }
            }

            void CheckShannoxState(Creature* controller = nullptr)
            {
                // Shannox already spawned
                if (shannoxGUID)
                    return;

                if (IsDone(DATA_SHANNOX))
                    return;

                if (!controller)
                    controller = instance->GetCreature(shannoxControllerGUID);

                if (controller)
                {
                    if (slaintrash == 25)
                    {
                        sCreatureTextMgr->SendChat(controller, 0, 0, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_MAP); // we dont a AI at this moment (if called from oncreaturecreate)
                    }
                    else if (slaintrash == 45)
                    {
                        sCreatureTextMgr->SendChat(controller, 1, 0, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_MAP);
                    }
                    else if (slaintrash >= 60)
                    {
                        if (Creature* shannox = controller->SummonCreature(BOSS_SHANNOX, shannoxspawn, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3600000))
                            sCreatureTextMgr->SendChat(shannox, 0, 0, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_MAP);

                        HandleGameObject(balorocDoorGUID, true);
                    }
                }
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_SHANNOX:
                    case DATA_LORD_RHYOLITH:
                    case DATA_BETHTILAC:
                    case DATA_ALYSRAZOR:
                        if (IsDone(DATA_SHANNOX) && IsDone(DATA_LORD_RHYOLITH) && IsDone(DATA_BETHTILAC) && IsDone(DATA_ALYSRAZOR) && IsDone(DATA_BALOROC))
                        {
                            if (Creature* orb = instance->GetCreature(magmaOrbGUID))
                            {
                                orb->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                                orb->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            }

                            if (Creature* teleportShannox = instance->GetCreature(shannoxTeleporterGUID))
                                teleportShannox->AI()->DoAction(ACTION_ENABLE_TELEPORT);

                            if (Creature* teleportMajor = instance->GetCreature(majordomoTeleporterGUID))
                                teleportMajor->AI()->DoAction(ACTION_ENABLE_TELEPORT);
                        }
                        break;
                    case DATA_BALOROC:
                        HandleGameObject(balorocDoorGUID, state != IN_PROGRESS && (shannoxGUID != 0 || IsDone(DATA_SHANNOX)));
                        break;
                    case DATA_MAJORDOMO_STAGHELM:
                        break;
                    case DATA_RAGNAROS:
                        if (state == DONE)
                        {
                            DoRespawnGameObject(cacheOfTheFirelordGUID, 7 * DAY);
                            if (GameObject* cache = instance->GetGameObject(cacheOfTheFirelordGUID))
                                cache->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                        }
                        break;
                }

                return true;
            }

            void SetData(uint32 id, uint32 value) override
            {
                switch (id)
                {
                    case DATA_KNEEL_TO_FLAME:
                        dataKneelToFlame = value;
                        break;
                    case DATA_GROUND_STATE:
                        dataAlysGround = value;
                        break;
                }
            }

            uint32 GetData(uint32 id) const override
            {
                switch (id)
                {
                    case DATA_KNEEL_TO_FLAME:
                        return dataKneelToFlame;
                        break;
                    case DATA_GROUND_STATE:
                        return dataAlysGround;
                        break;
                }

                return 0;
            }

            void WriteSaveDataMore(std::ostringstream& data) override
            {
                data << slaintrash;
            }

            void ReadSaveDataMore(std::istringstream& data) override
            {
                data >> slaintrash;
            }

            void IncrementFandrallEvent()
            {
                if (!dataKneelToFlame)
                    return;

                fandrallActivationCount++;
                if (!Events.GetNextEventTime(EVENT_DESACTIVATE_FANDRALL))
                    Events.ScheduleEvent(EVENT_DESACTIVATE_FANDRALL, 2500);

                if (fandrallActivationCount == FANDRALL_ACTIVATION_COUNT)
                {
                    DoCastSpellOnPlayers(SPELL_FANDRALL_ACHIEVEMENT_CHECK);
                    dataKneelToFlame = 0;
                }
            }
            
            void ProcessEvent(WorldObject* source, uint32 eventId)
            {
                switch (eventId)
                {
                    case EVENT_FANDRALL_LEFT_ACTIVATION:
                        if (++fandralLeftActivationCount <= (FANDRALL_ACTIVATION_COUNT / 2))
                            IncrementFandrallEvent();
                        break;
                    case EVENT_FANDRALL_RIGHT_ACTIVATION:
                        if (++fandralRightActivationCount <= (FANDRALL_ACTIVATION_COUNT / 2))
                            IncrementFandrallEvent();
                        break;
                }
            }

            void Update(uint32 diff)
            {
                Events.Update(diff);

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DESACTIVATE_FANDRALL:
                            dataKneelToFlame = 0;
                            break;
                        default:
                            break;
                    }
                }
            }
            
        private:
            EventMap Events;
            uint32 slaintrash;
            uint32 dataAlysGround;
            uint32 dataKneelToFlame;
            uint64 shannoxGUID;
            uint64 rhyolithGUID;
            uint64 bethtilacGUID;
            uint64 alysrazorGUID;
            uint64 balorocGUID;
            uint64 majordomoStaghelmGUID;
            uint64 ragnarosGUID;
            uint64 riplimbGUID;
            uint64 ragefaceGUID;
            uint64 shannoxSpearGUID;
            uint64 sulfuronBridgeGUID;
            uint64 alysrazorVolcanoGUID;
            uint64 cacheOfTheFirelordGUID;
            uint64 bethtilacLiftGUID;
            uint64 magmaOrbGUID;
            uint64 balorocDoorGUID;
            uint64 shannoxTeleporterGUID;
            uint64 majordomoTeleporterGUID;
            uint64 shannoxControllerGUID;
            uint8 fandrallActivationCount;
            uint8 fandralLeftActivationCount;
            uint8 fandralRightActivationCount;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_firelands_InstanceMapScript(map);
        }
class npc_firelands_molten_lord: public CreatureScript
{
    public:
        npc_firelands_molten_lord() :
                CreatureScript("npc_firelands_molten_lord")
        {
        }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_firelands_molten_lordAI(pCreature);
        }

        struct npc_firelands_molten_lordAI: public ScriptedAI
        {
                npc_firelands_molten_lordAI(Creature* pCreature) :
                        ScriptedAI(pCreature)
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
                    pInstance = pCreature->GetInstanceScript();
                }

                InstanceScript* pInstance;
                EventMap events;

                void Reset()
                {
                    events.Reset();
                }

                void JustDied(Unit* killer)
                {
                    if (pInstance)
                    {
                        Map::PlayerList const& PlayerList = pInstance->instance->GetPlayers();
                        if (!PlayerList.isEmpty())
                        {
                            for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end();
                                    ++itr)
                            {
                                if (Player* pPlayer = itr->getSource())
                                {
                                    if (!pPlayer->IsAtGroupRewardDistance(me))
                                        continue;

                                    // Only mages, warlocks priests, shamans and druids
                                    // in caster specialization can accept quest
                                    if (!(pPlayer->getClass() == CLASS_MAGE || pPlayer->getClass() == CLASS_WARLOCK
                                            || pPlayer->getClass() == CLASS_PRIEST
                                            || (pPlayer->getClass() == CLASS_SHAMAN
                                                    && (pPlayer->GetPrimaryTalentTree(pPlayer->GetActiveSpec())
                                                            == TALENT_TREE_SHAMAN_ELEMENTAL
                                                            || pPlayer->GetPrimaryTalentTree(pPlayer->GetActiveSpec())
                                                                    == TALENT_TREE_SHAMAN_RESTORATION))
                                            || (pPlayer->getClass() == CLASS_DRUID
                                                    && (pPlayer->GetPrimaryTalentTree(pPlayer->GetActiveSpec())
                                                            == TALENT_TREE_DRUID_BALANCE
                                                            || pPlayer->GetPrimaryTalentTree(pPlayer->GetActiveSpec())
                                                                    == TALENT_TREE_DRUID_RESTORATION))))
                                        continue;

                                    if (pPlayer->GetTeam() == ALLIANCE
                                            && pPlayer->GetQuestStatus(29453) == QUEST_STATUS_NONE)
                                        pPlayer->AddQuest(sObjectMgr->GetQuestTemplate(29453), NULL);
                                        else if (pPlayer->GetTeam() == HORDE && pPlayer->GetQuestStatus(29452) ==QUEST_STATUS_NONE)
                                    pPlayer->AddQuest(sObjectMgr->GetQuestTemplate(29452), NULL);
                                    }
                                }
                            }
                        }
                    }
				};
			};	
};

void AddSC_instance_firelands()
{
	new instance_firelands();
	
}
