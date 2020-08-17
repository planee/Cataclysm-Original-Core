#pragma once
#include "Common.h"

struct CustomWaypoint
{
	uint32 id;
	float x;
	float y;
	float z;
};

using CustomWaypointPath = std::vector<CustomWaypoint>;

class CustomWaypointMgr
{
public:
	static CustomWaypointMgr* Instance()
	{
		static CustomWaypointMgr instance;
		return &instance;
	}

	CustomWaypointPath const* GetPath(uint32 id)
	{
		auto itr = m_waypointPaths.find(id);
		if (itr == m_waypointPaths.end())
			return nullptr;
		return &itr->second;
	}

	void Load();
private:
	std::unordered_map<uint32, CustomWaypointPath> m_waypointPaths;
};

#define sCustomWaypointMgr CustomWaypointMgr::Instance()