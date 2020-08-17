#pragma once
///< TICore 
///All rights reserved - https://github.com/Artamedes/TICore
///This file is private, you can not share without permission.
///Author: Artamedes

#pragma once

#include "Define.h"

#include <unordered_map>
#include <vector>

struct ObjectGuid;

struct PlayerTextTable
{
    uint8 GroupId;
    uint8 ID;
    std::string Text;
    uint32 Type;
    uint32 Language;
    float Probability;
    uint32 Emote;
    uint32 Duration;
    uint32 Sound;
    uint32 BroadcastTextId;
};

typedef std::vector<PlayerTextTable> PlayerTextGroup;
typedef std::unordered_map<uint32, PlayerTextGroup> PlayerTextContainer;

class PlayerTextMgr
{
public:
    static PlayerTextMgr* instance();

    void LoadTextFromDB(bool p_Reload = false);

    void SendTextToPlayer(uint32 p_TextId, uint8 p_GroupId, ObjectGuid p_Guid);

    PlayerTextTable GetRandomTextFromIdAndGroup(uint32 p_TextId, uint8 p_GroupId);

private:
    PlayerTextContainer m_Texts;
};

#define sPlayerTextMgr PlayerTextMgr::instance()