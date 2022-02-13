/**
 CPhysics3D
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Physics3D.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CPhysics3D::CPhysics3D(void)
	: v3InitialVelocity(glm::vec3(0.0f))
	, v3FinalVelocity(glm::vec3(0.0f))
	, v3Acceleration(glm::vec3(0.0f))
	, v3PrevDisplacement(glm::vec3(0.0f))
	, v3Displacement(glm::vec3(0.0f))
	, fTotalTime(0.0f)
	, fElapsedTime(0.0f)
	, sCurrentStatus(STATUS::IDLE)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CPhysics3D::~CPhysics3D(void)
{
}

/**
@brief Init Initialise this instance
@return true if the initialisation is successful, else false
*/
bool CPhysics3D::Init(void)
{
	// Reset these variables
	v3InitialVelocity = glm::vec3(0.0f);
	v3FinalVelocity = glm::vec3(0.0f);
	v3Acceleration = glm::vec3(0.0f);		// Acceleration does not need to be reset here.
	v3PrevDisplacement = glm::vec3(0.0f);
	v3Displacement = glm::vec3(0.0f);
	fTotalTime = 0.0f;
	fElapsedTime = 0.0f;
	return true;
}

// Set methods
/**
 @brief Set Initial velocity
 @param v3InitialVelocity A const glm::vec3 variable describing the initial velocity
 */
void CPhysics3D::SetInitialVelocity(const glm::vec3 v3InitialVelocity)
{
	this->v3InitialVelocity = v3InitialVelocity;	// Initial velocity
}

/**
 @brief Set Final velocity
 @param v3FinalVelocity A const glm::vec3 variable describing the final velocity
 */
void CPhysics3D::SetFinalVelocity(const glm::vec3 v3FinalVelocity)
{
	this->v3FinalVelocity = v3FinalVelocity;		// Final velocity
}

/**
 @brief Set Acceleration
 @param v3Acceleration A const glm::vec3 variable describing the acceleration
 */
void CPhysics3D::SetAcceleration(const glm::vec3 v3Acceleration)
{
	this->v3Acceleration = v3Acceleration;		// Acceleration
}

/**
 @brief Set Displacement
 @param v3Displacement A const glm::vec3 variable describing the displacement
 */
void CPhysics3D::SetDisplacement(const glm::vec3 v3Displacement)
{
	this->v3Displacement = v3Displacement;		// Displacement
}

/**
 @brief Set Total Elapsed Time
 @param fTotalTime A const float variable describing the total time since this physics calculation started
 */
void CPhysics3D::SetTotalElapsedTime(const float fTotalTime)
{
	this->fTotalTime = fTotalTime;	// Total Elapsed Time
}

/**
 @brief Set Elapsed Time
 @param fElapsedTime A const float variable containing the elapsed time since the last frame
 */
void CPhysics3D::SetElapsedTime(const float fElapsedTime)
{
	this->fElapsedTime = fElapsedTime;	// Total Elapsed Time
}

/**
 @brief Set Status
 @param sStatus A const STATUS variable setting the new status of this class instance
 */
void CPhysics3D::SetStatus(const STATUS sStatus)
{
	// If there is a change in status, then reset to default values
	if (sCurrentStatus != sStatus)
	{
		// Reset to default values
		Init();

		// Store the new status
		sCurrentStatus = sStatus;
	}
}

// Get methods
/**
 @brief Get Initial velocity
 @return a glm::vec3 variable
*/
glm::vec3 CPhysics3D::GetInitialVelocity(void) const
{
	return v3InitialVelocity;	// Initial velocity
}

/**
 @brief Get Final velocity
 @return a glm::vec3 variable
 */
glm::vec3 CPhysics3D::GetFinalVelocity(void) const
{
	return v3FinalVelocity;		// Final velocity
}

/**
 @brief Get Acceleration
 @return a glm::vec3 variable
 */
glm::vec3 CPhysics3D::GetAcceleration(void) const
{
	return v3Acceleration;		// Acceleration
}

/**
 @brief Get Displacement
 @return a glm::vec3 variable
 */
glm::vec3 CPhysics3D::GetDisplacement(void) const
{
	return v3Displacement;		// Displacement
}

/**
 @brief Get Delta Displacement
 @return a glm::vec3 variable
 */
glm::vec3 CPhysics3D::GetDeltaDisplacement(void) const
{
	return v3Displacement - v3PrevDisplacement;		// Delta Displacement
}

/**
 @brief Get Time
 @return a float variable
 */
float CPhysics3D::GetTotalElapsedTime(void) const
{
	return fTotalTime;	// Total Elapsed Time
}

/**
 @brief Get Status
 @return a CPhysics3D::STATUS variable
 */
CPhysics3D::STATUS CPhysics3D::GetStatus(void) const
{
	return sCurrentStatus;
}

/**
 @brief Update
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
 */
bool CPhysics3D::Update(const double dElapsedTime)
{
	// If the player is in IDLE mode, 
	// then we don't calculate further
	if (sCurrentStatus == STATUS::IDLE)
		return false;

	// Store the previous displacement values
	v3PrevDisplacement = v3Displacement;
	// Calculate the displacement
	v3Displacement = v3InitialVelocity * fTotalTime + 0.5f * v3Gravity * fTotalTime * fTotalTime;

	return true;
}

/**
 @brief Add elapsed time
 @param fElapsedTime A const float variable containing the elapsed time since the last frame
 */
void CPhysics3D::AddElapsedTime(const float fElapsedTime)
{
	this->fElapsedTime = fElapsedTime;
	fTotalTime += fElapsedTime;
}

/**
 @brief Calculate the distance between two vec2 varables
 @param source A const glm::vec3 variable containing the source position
 @param destination A const glm::vec3 variable containing the destination position
 @return a float variable
*/
float CPhysics3D::CalculateDistance(glm::vec3 source, glm::vec3 destination)
{
	return glm::length(destination - source);
}

/**
 @brief PrintSelf
 */
void CPhysics3D::PrintSelf(void)
{
	cout << "CPhysics3D::PrintSelf()" << endl;
	cout << "v3InitialVelocity\t=\t[" << v3InitialVelocity.x << ", " << v3InitialVelocity.y << "]" << endl;
	cout << "v3FinalVelocity\t=\t[" << v3FinalVelocity.x << ", " << v3FinalVelocity.y << "]" << endl;
	cout << "v3Acceleration\t=\t[" << v3Acceleration.x << ", " << v3Acceleration.y << "]" << endl;
	cout << "v3Displacement\t=\t[" << v3Displacement.x << ", " << v3Displacement.y << "]" << endl;
	cout << "fTotalTime\t=\t" << fTotalTime << endl;
	cout << "fElapsedTime\t=\t" << fElapsedTime << endl;
}
