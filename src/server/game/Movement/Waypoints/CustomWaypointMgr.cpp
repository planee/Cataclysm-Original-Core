#include "CustomWaypointMgr.h"
#include "DatabaseEnv.h"

void CustomWaypointMgr::Load()
{
	auto result = WorldDatabase.Query("SELECT entry, id, x, y, z FROM custom_waypoints ORDER BY entry, id");
	if (!result)
		return;

	m_waypointPaths.clear();

	CustomWaypointPath path;
	uint32 lastEntry = 0;
	uint32 count = 0;
	do {
		auto fields = result->Fetch();
		uint32 entry = fields[0].GetUInt32();

		if (!lastEntry)
			lastEntry = entry;

		if (lastEntry != entry)
		{
			//new path, save current path.
			m_waypointPaths[lastEntry] = std::move(path);
			path.clear();
			lastEntry = entry;
			++count;
		}
		
		path.emplace_back(CustomWaypoint{ fields[1].GetUInt32(), fields[2].GetFloat(), fields[3].GetFloat(), fields[4].GetFloat()});
	} while (result->NextRow());

	m_waypointPaths[lastEntry] = std::move(path);

	TC_LOG_INFO("server.loading", ">> Loaded %u Custom Waypoint Paths.", count);
}
