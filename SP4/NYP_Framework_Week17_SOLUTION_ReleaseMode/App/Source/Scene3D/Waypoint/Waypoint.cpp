/**
 CWaypoint
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "Waypoint.h"
#include <limits>
#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CWaypoint::CWaypoint(void)
	: position(glm::vec3(0.0f, 0.0f, 0.0f))
	, m_iID(-1)
{
	// Init this class instance
	Init();
}

/**
 @brief Default Destructor
 */
CWaypoint::~CWaypoint(void)
{
	// Destroy this class instance
	Destroy();
}

/**
 @brief Initialise this instance to default values
 @return A bool variable
 */
bool CWaypoint::Init(void)
{
	// Clear the relatedWaypoints
	relatedWaypoints.clear();

	return true;
}

/**
 @brief Destroy this class instance
 */
void CWaypoint::Destroy(void)
{
	// Clear the relatedWaypoints
	relatedWaypoints.clear();
}

/**
 @brief Set position of this waypoint
 @param position A const glm::vec3 variable containing the new position
 */
void CWaypoint::SetPosition(glm::vec3 position)
{
	this->position = position;
}
/**
 @brief Get position of this waypoint
 @return A const glm::vec3 variable
 */
glm::vec3 CWaypoint::GetPosition(void) const
{
	return position;
}

/**
 @brief Set an ID to this WayPoint
 @param m_iID A const int variable containing the ID of a new WayPoint
 */
void CWaypoint::SetID(const int m_iID)
{
	this->m_iID = m_iID;
}
/**
 @brief Get an ID of this WayPoint
 @return A const int variable containing the ID of this WayPoint
 */
int CWaypoint::GetID(void) const
{
	return m_iID;
}

/**
 @brief Add waypoint related to this WayPoint
 @param relatedWaypoint A CWayPoint variable of the related WayPoint
 */
void CWaypoint::AddRelatedWaypoint(CWaypoint* relatedWaypoint)
{
	relatedWaypoints.push_back(relatedWaypoint);
}

/**
 @brief Use a waypoint's ID to find and remove a WayPoint related to this WayPoint
 @param A const int variable containing the ID of this WayPoint
 @return A bool variable
 */
bool CWaypoint::RemoveRelatedWaypoint(const int m_iID)
{
	// If Waypoints has related Waypoints, then we proceed to search.
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			if ((*it)->GetID() == m_iID)
			{
				// We erase from the vector, relatedWaypoints, only. DO NOT DELETE!
				// Let CWayPointManager delete it!
				it = relatedWaypoints.erase(it);
				return true;
			}
			it++;
		}
	}
	return false;
}

/**
 @brief Get nearest Waypoint amongst Waypoints related to this WayPoint
 @return A CWaypoint variable
 */
CWaypoint* CWaypoint::GetNearestWaypoint(void)
{
	CWaypoint* theNearestWaypoint = NULL;
	float m_fDistance = numeric_limits<float>::max();

	// If Waypoints has related Waypoints, then we proceed to search.
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			// Get position of a WayPoint
			glm::vec3 aRelatedWaypoint = (*it)->GetPosition();

			// Get the x- and z-component distance between position and aRelatedWaypoint
			float xDistance = position.x - aRelatedWaypoint.x;
			float zDistance = position.x - aRelatedWaypoint.z;
			// Calculate the distance squared between position and aRelatedWaypoint
			float distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
			if (m_fDistance > distanceSquared)
			{
				// Set distanceSquared to m_fDistance
				m_fDistance = distanceSquared;
				// Set theNearestWaypoint to this WayPoint
				theNearestWaypoint = (CWaypoint*)(*it);
			}

			// Increment the iterator
			it++;
		}
	}
	// Return the nearest WayPoint
	return theNearestWaypoint;
}

/**
 @brief Get the number of related Waypoints
 @return An int variable
 */
int CWaypoint::GetNumberOfWaypoints(void) const
{
	return relatedWaypoints.size();
}

/**
 @brief PrintSelf
 */
void CWaypoint::PrintSelf(void)
{
	cout << "Waypoint ID	: " << m_iID << endl;
	cout << " Position		: " << position.x << ", " << position.y << ", " << position.z << endl;
	cout << " Related waypoints : ";
	// If Waypoints has related Waypoints, then we proceed to search.
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			cout << (*it)->GetID();
			it++;

			if (it != relatedWaypoints.end())
				cout << ", ";
		}
		cout << endl;
	}
	else
		cout << "Nil" << endl;
}
