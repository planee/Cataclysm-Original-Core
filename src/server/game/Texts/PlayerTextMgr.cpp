///< TICore 
///All rights reserved - https://github.com/Artamedes/TICore
///This file is private, you can not share without permission.
///Author: Artamedes

//Base
#include "PlayerTextMgr.h"

//Common
#include "Log.h"

//Shared
#include "ByteBuffer.h" //For objectguid

//Database
#include "Field.h"
#include "DatabaseEnv.h"

//Game
#include "Chat.h"
#include "DB2Stores.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"

PlayerTextMgr* PlayerTextMgr::instance()
{
    static PlayerTextMgr instance;
    return &instance;
}

void PlayerTextMgr::LoadTextFromDB(bool p_Reload)
{
    TC_LOG_INFO("server.loading", "Starting load of `player_texts`");
    if (p_Reload)
        m_Texts.clear();

    PreparedStatement* l_Stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_PLAYER_TEXTS);
    PreparedQueryResult l_Result = WorldDatabase.Query(l_Stmt);

    if (!l_Result)
    {
        TC_LOG_INFO("server.loading", "DB Table `player_texts` is empty.");
        return;
    }

    do
    {
        Field* l_Fields = l_Result->Fetch();

        PlayerTextTable l_Tbl;

        uint32 l_TextId = l_Fields[0].GetUInt32();

        l_Tbl.GroupId = l_Fields[1].GetUInt8();
        l_Tbl.ID = l_Fields[2].GetUInt8();
        l_Tbl.Text = l_Fields[3].GetString();
        l_Tbl.Type = l_Fields[4].GetUInt32();
        l_Tbl.Language = l_Fields[5].GetUInt32();
        l_Tbl.Probability = l_Fields[6].GetFloat();
        l_Tbl.Emote = l_Fields[7].GetUInt32();
        l_Tbl.Duration = l_Fields[8].GetUInt32();
        l_Tbl.Sound = l_Fields[9].GetUInt32();
        l_Tbl.BroadcastTextId = l_Fields[10].GetUInt32();

        if (l_Tbl.BroadcastTextId && !sObjectMgr->GetBroadcastText(l_Tbl.BroadcastTextId))
        {
            TC_LOG_ERROR("server.loading", "DB Table `player_texts` has invalid BroadcastTextId %u at TextId %u", l_Tbl.BroadcastTextId, l_TextId);
            l_Tbl.BroadcastTextId = 0;
        }

        m_Texts[l_TextId].emplace_back(l_Tbl);

    } while (l_Result->NextRow());
}

void PlayerTextMgr::SendTextToPlayer(uint32 p_TextId, uint8 p_GroupId, ObjectGuid p_Guid)
{
    PlayerTextTable l_Text = GetRandomTextFromIdAndGroup(p_TextId, p_GroupId);

    std::string l_SenderName = "";

    Player* l_Player = ObjectAccessor::FindPlayer(p_Guid);
    WorldSession *l_Sess = nullptr;

    if (l_Player)
    {
        l_SenderName = l_Player->GetName();
        l_Sess = l_Player->GetSession();
    }

    WorldPacket l_Data;
    ChatHandler::FillMessageData(&l_Data, l_Sess, uint8(l_Text.Type), l_Text.Language, "", uint64(p_Guid), l_Text.Text.c_str(), l_Player);
    if (l_Player)
        l_Player->SendDirectMessage(&l_Data);
    //else sendmessagetoset, - NOT IMPLEMENTED
}

PlayerTextTable PlayerTextMgr::GetRandomTextFromIdAndGroup(uint32 p_TextId, uint8 p_GroupId)
{
    PlayerTextGroup l_TempGroup;

    for (PlayerTextTable const& l_Text : m_Texts[p_TextId])
        if (p_GroupId == l_Text.GroupId)
            l_TempGroup.emplace_back(l_Text);

    uint8 l_Count = 0;
    float l_LastChance = -1;
    bool l_IsEqualChanced = true;
    float l_TotalChance = 0;

    for (PlayerTextTable const& l_Text : l_TempGroup)
    {
        if (l_LastChance >= 0 && l_LastChance != l_Text.Probability)
            l_IsEqualChanced = false;

        l_LastChance = l_Text.Probability;
        l_TotalChance += l_Text.Probability;
        ++l_Count;
    }

    int32 l_Offset = -1;
    if (!l_IsEqualChanced)
    {
        for (PlayerTextTable const& l_Text : l_TempGroup)
        {
            uint32 l_Chance = uint32(l_Text.Probability);
            uint32 l_R = urand(0, 100);
            ++l_Offset;
            if (l_R <= l_Chance)
                break;
        }
    }

    uint32 l_Pos = 0;
    if (l_IsEqualChanced || l_Offset < 0)
        l_Pos = urand(0, l_Count - 1);
    else if (l_Offset >= 0)
        l_Pos = l_Offset;

    return l_TempGroup[l_Pos];
}