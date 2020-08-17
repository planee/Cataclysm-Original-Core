#pragma once
#include "MovementGenerator.h"
#include "Movement/Waypoints/CustomWaypointMgr.h"
#include "Creature.h"

template<class T>
class CustomWaypointMovementGenerator;

template<>
class CustomWaypointMovementGenerator<Creature> : public MovementGeneratorMedium< Creature, CustomWaypointMovementGenerator<Creature>>
{
public:
	CustomWaypointMovementGenerator(uint32 pathId, CustomWaypointPath* path, bool repeat, bool isCyclic, bool isCatmullRom)
		: m_pathId(pathId), m_path(path), m_isArrivalDone(false), doneMovement(false), m_repeating(repeat), m_isCyclic(isCyclic), m_isCatmullRom(isCatmullRom), m_nodeId(0) {}
	void DoInitialize(Creature*);
	void DoFinalize(Creature*);
	void DoReset(Creature*);
	bool DoUpdate(Creature*, uint32 diff);

	void MovementInform(Creature*);

	MovementGeneratorType GetMovementGeneratorType() { return CUSTOM_WAYPOINT_MOTION_TYPE; }

	bool GetResetPos(Creature*, float& x, float& y, float& z);

private:
	void OnArrived(Creature*);
	bool StartMove(Creature*);

	bool m_isArrivalDone;
	bool m_repeating;
	bool m_isCyclic;
    bool doneMovement;
	bool m_isCatmullRom;
	CustomWaypointPath* m_path;
	uint32 m_pathId;
	uint32 m_nodeId;
};