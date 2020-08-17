/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
* Copyright (C) 2014-2018 RoG_WoW Source  <http://wow.rog.snet
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <ScriptPCH.h>
#include <ScriptMgr.h>
#include "ChannelMgr.h"

class StabilityTest : public PlayerScript
{
public:
    StabilityTest() : PlayerScript("StabilityTest") {}

    enum eSpell
    {
        SPELL_TEST_HEAL = 5174
    };

    std::map<uint32, uint32> m_RandomEventTimer;

    // void OnLogin(Player* player, bool /*p_FirstLogin*/)
    void OnLogin(Player* player)
    {
        if (player->GetSession()->IsStabilityTest())
        {
            if (ChannelMgr* cMgr = ChannelMgr::forTeam(player->GetTeam()))
                if (Channel* chn = cMgr->GetJoinChannel("world", 0))
                    chn->JoinChannel(player, "");

            player->SetCheatStatusOn(CHEAT_STATE_GODMODE);

            m_RandomEventTimer[player->GetGUIDLow()] = 1000;
        }
    }

    void OnUpdate(Player* p_Player, uint32 p_Diff)
    {
        if (!p_Player->GetSession()->IsStabilityTest())
            return;

        auto l_Itr = m_RandomEventTimer.find(p_Player->GetGUIDLow());
        if (l_Itr == m_RandomEventTimer.end())
            return;

        if (p_Player->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (l_Itr->second < p_Diff)
        {
            switch (urand(0, 10))
            {
            case 0:
            {
                std::ostringstream l_Text;
                l_Text << "[SAY] StabilityTest bot " << p_Player->GetGUIDLow();
                p_Player->Say(l_Text.str(), Language::LANG_UNIVERSAL);
                break;
            }
            case 1:
            {
                std::ostringstream l_Text;
                l_Text << "[CHANNEL] StabilityTest bot " << p_Player->GetGUIDLow();

                if (ChannelMgr* l_ChannelManager = ChannelMgr::forTeam(p_Player->GetTeam()))
                {
                    if (Channel* l_Channel = l_ChannelManager->GetChannel("world", p_Player))
                        l_Channel->Say(p_Player->GetGUID(), l_Text.str().c_str(), Language::LANG_UNIVERSAL);
                }
                break;
            }
            case 2:
            {
                p_Player->CastSpell(p_Player, SPELL_TEST_HEAL, false);
                break;
            }
            default:
            {
                Position l_Pos;
                p_Player->GetRandomNearPosition(l_Pos, 150.0f);
                p_Player->GetMotionMaster()->MovePoint(0, l_Pos, true);
                break;
            }
            }

            l_Itr->second = 2000;
        }
        else
            l_Itr->second -= p_Diff;
    }
};

void AddSC_StabilityTest()
{
    new StabilityTest();
};