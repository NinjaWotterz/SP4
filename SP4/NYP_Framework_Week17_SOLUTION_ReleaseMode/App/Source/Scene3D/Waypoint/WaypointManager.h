/**
 CWaypointManager
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

#include "Waypoint.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <vector>
using namespace std;

class CWaypointManager
{
public:
	// Constructor
	CWaypointManager(void);
	// Destructor
	virtual ~CWaypointManager(void);

	// Init
	void Init(void);
	// Destroy the instance
	void Destroy(void);

	// Add waypoint
	int AddWaypoint(glm::vec3 position);
	// Add the position of a waypoint by searching for its m_iID
	int AddWaypoint(const int m_iID, const glm::vec3 position);
	// Remove waypoint based on its m_iID
	bool RemoveWaypoint(const int m_iID);

	// Get next ID when adding a waypoint
	int GetNextIDToAdd(void);
	// Get Current ID
	int GetCurrentID(void);
	// Get Next WayPoint ID
	int GetNextWayPointID(void);

	// HasReachedWayPoint
	bool HasReachedWayPoint(const glm::vec3 aPosition);
	// Get a Waypoint based on its ID
	CWaypoint* GetWaypoint(const int m_iID);
	// Get nearest waypoint amongst related Waypoints
	// This method is used when your NPC has deviated from the WayPoints 
	// and it needs to find the nearest WayPoint to go to
	CWaypoint* GetNearestWaypoint(const glm::vec3 aPosition);
	// Get next waypoint
	CWaypoint* GetNextWaypoint(void);
	// Get next waypoint position
	glm::vec3 GetNextWaypointPosition(void);
	// Get the number of related Waypoints
	int GetNumberOfWaypoints(void) const;

	// PrintSelf
	void PrintSelf(void);

protected:
	// The current Waypoint's ID
	int m_iCurrentID;
	// Distance tolerance for checking proximity to a WayPoint
	float m_fDistanceTolerance;

	vector<CWaypoint*> listOfWaypoints;

	// Remove related waypoint
	void RemoveRelatedWaypoint(const int m_iID);
};