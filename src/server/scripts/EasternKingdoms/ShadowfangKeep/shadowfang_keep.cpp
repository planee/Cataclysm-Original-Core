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

#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "shadowfang_keep.h"

class npc_haunted_stable_hand : public CreatureScript
{
public:
    npc_haunted_stable_hand() : CreatureScript("npc_haunted_stable_hand") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 Sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (Sender != GOSSIP_SENDER_MAIN)
            return true;
        if (!player->getAttackers().empty())
            return true;

        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->TeleportTo(33, -225.70f, 2269.67f, 74.999f, 2.76f);
                player->CLOSE_GOSSIP_MENU();
            break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->TeleportTo(33, -260.66f, 2246.97f, 100.89f, 2.43f);
                player->CLOSE_GOSSIP_MENU();
            break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->TeleportTo(33, -171.28f, 2182.020f, 129.255f, 5.92f);
                player->CLOSE_GOSSIP_MENU();
            break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        InstanceScript* instance = creature->GetInstanceScript();

        if (instance && instance->GetData(DATA_BARON_SILVERLAINE_EVENT)==DONE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport me to Baron Silberlein", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        if (instance && instance->GetData(DATA_COMMANDER_SPRINGVALE_EVENT)==DONE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport me to Commander Springvale", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        if (instance && instance->GetData(DATA_LORD_WALDEN_EVENT)==DONE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport me to Lord Walden", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

        player->SEND_GOSSIP_MENU(2475, creature->GetGUID());
        return true;
    }
};

void AddSC_shadowfang_keep()
{
   new npc_haunted_stable_hand();
}