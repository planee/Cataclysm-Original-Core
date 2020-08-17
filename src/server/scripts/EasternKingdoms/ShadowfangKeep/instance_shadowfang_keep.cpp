/*
 * Copyright (C) 2010-2017 OMFG.GG <http://www.omfg.gg/>
 * Copyright (C) 2017-2017 WoWZealot <http://www.wowzealot.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "InstanceScript.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "shadowfang_keep.h"

TeleportationData const teleportationData[] =
{
    {DATA_TELEPORT_HIGH_HALL,         DATA_BARON_ASHBURY_EVENT         },
    {DATA_TELEPORT_CHAPELLE,          DATA_BARON_SILVERLAINE_EVENT     },
    {DATA_TELEPORT_REMPARTS,          DATA_COMMANDER_SPRINGVALE_EVENT  },
    {DATA_TELEPORT_LABO,              DATA_LORD_WALDEN_EVENT           },
    {0,                               0,                               }, // END
};

class instance_shadowfang_keep : public InstanceMapScript
{
public:
    instance_shadowfang_keep() : InstanceMapScript("instance_shadowfang_keep", 33) { }

    struct instance_shadowfang_keep_InstanceMapScript: public InstanceScript
    {
        instance_shadowfang_keep_InstanceMapScript(InstanceMap* map): InstanceScript(map)  { }

        void Initialize()
        {
            SetHeaders(DataHeader);
            SetBossNumber(MAX_ENCOUNTER);
            LoadTeleportationData(teleportationData);

            BaronAshburyGUID        = 0;
            BaronSilverlaineGUID    = 0;
            CommanderSpringvaleGUID = 0;
            LordGodfreyGUID         = 0;
            LordWaldenGUID          = 0;
            ApothecaryHummelGUID    = 0;
            ApothecaryBaxterGUID    = 0;
            ApothecaryFryeGUID      = 0;
            TeamInInstance          = 0;
            GodfreyPreEvent         = NOT_STARTED;
        }

        void OnCreatureCreate(Creature* creature)
        {
            Map::PlayerList const &players = instance->GetPlayers();
            if (TeamInInstance == 0 && !players.isEmpty())
            {
                if (Player* player = players.begin()->getSource())
                    TeamInInstance = player->GetTeam();
            }

            switch (creature->GetEntry())
            {
                case BOSS_BARON_ASHBURY:
                    BaronAshburyGUID        = creature->GetGUID();
                    break;
                case BOSS_BARON_SILVERLAINE:
                    BaronSilverlaineGUID    = creature->GetGUID();
                    break;
                case BOSS_COMMANDER_SPRINGVALE:
                    CommanderSpringvaleGUID = creature->GetGUID();
                    break;
                case BOSS_LORD_GODFREY:
                    LordGodfreyGUID         = creature->GetGUID();
                    break;
                case BOSS_LORD_WALDEN:
                    LordWaldenGUID          = creature->GetGUID();
                    break;
                case NPC_HUMMEL:
                    ApothecaryHummelGUID    = creature->GetGUID();
                    break;
                case NPC_BAXTER:
                    ApothecaryBaxterGUID    = creature->GetGUID();
                    break;
                case NPC_FRYE:
                    ApothecaryFryeGUID      = creature->GetGUID();
                    break;
                case NPC_HAUNTED_TELEPORTER:
                    AddTeleporter(creature->GetGUID());
                    break;
                case NPC_DEATHSTALKER_COMMANDER_BELMOT:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_PACKLEADER_IVAR_BLOODFANG, ALLIANCE);
                    break;
                case NPC_VETERAN_FORSAKEN_TROOPER:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_BLOODFANG_BERSERKER, ALLIANCE);
                    break;
                case NPC_POISON_STALKER:
                    creature->CastSpell(creature, SPELL_DISEASE_CLOUD, true);
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_BARON_ASHBURY_DOOR:
                    go->SetGoState(IsDone(DATA_BARON_ASHBURY_EVENT) ? GO_STATE_ACTIVE : GO_STATE_READY);
                    break;
                case GO_SORCERS_GATE:
                    go->SetGoState(IsDone(DATA_LORD_WALDEN_EVENT) ? GO_STATE_ACTIVE : GO_STATE_READY);
                    break;
                case GO_LORD_GODFREY_DOOR:
                    go->SetGoState(GodfreyPreEvent == DONE ? GO_STATE_ACTIVE : GO_STATE_READY);
                    break;
            }
        }

        void OnPlayerEnter(Player* player)
        {
            if (!TeamInInstance)
                TeamInInstance = player->GetTeam();
        }

        uint32 GetData(uint32 identifier) const
        {
            if (identifier == TEAM_IN_INSTANCE)
                return TeamInInstance;
            else if (identifier == DATA_GODFREY_PRE_EVENT)
                return GodfreyPreEvent;

            return 0;
        }

        void SetData(uint32 type, uint32 data)
        {
            if (type == DATA_GODFREY_PRE_EVENT)
            {
                GodfreyPreEvent = data;
                SaveToDB();
            }
        }

        uint64 GetData64(uint32 identifier) const
        {
            switch (identifier)
            {
                case DATA_BARON_ASHBURY:
                    return BaronAshburyGUID;
                case DATA_BARON_SILVERLAINE:
                    return BaronSilverlaineGUID;
                case DATA_COMMANDER_SPRINGVALE:
                    return CommanderSpringvaleGUID;
                case DATA_LORD_GODFREY:
                    return LordGodfreyGUID;
                case DATA_LORD_WALDEN:
                    return LordWaldenGUID;
                case NPC_HUMMEL:
                    return ApothecaryHummelGUID;
                case NPC_BAXTER:
                    return ApothecaryBaxterGUID;
                case NPC_FRYE:
                    return ApothecaryFryeGUID;
            }
            return 0;
        }

        void WriteSaveDataMore(std::ostringstream& data) override
        {
            data << GodfreyPreEvent;
        }

        void ReadSaveDataMore(std::istringstream& data) override
        {
            uint32 temp = 0;
            data >> temp;
            GodfreyPreEvent = temp ? DONE : NOT_STARTED;
        }

        protected:
            uint32 TeamInInstance;
            uint32 GodfreyPreEvent;
            uint64 BaronAshburyGUID;
            uint64 BaronSilverlaineGUID;
            uint64 CommanderSpringvaleGUID;
            uint64 LordGodfreyGUID;
            uint64 LordWaldenGUID;
            uint64 ApothecaryHummelGUID;
            uint64 ApothecaryBaxterGUID;
            uint64 ApothecaryFryeGUID;
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_shadowfang_keep_InstanceMapScript(map);
    }
};

void AddSC_instance_shadowfang_keep()
{
    new instance_shadowfang_keep();
}
