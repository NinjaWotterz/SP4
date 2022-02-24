/**
 CEnemy3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CSolidObject
#include "../Entities/SolidObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "../Camera.h"
#include <time.h>

// Include Player3D
#include "../Entities/Player3D.h"

// Include CTerrain
#include "../Terrain/Terrain.h"

// Include FSM
#include "../AI/FSM.h"

// Include WaypointManager
#include "../Waypoint/WaypointManager.h"
// Include CKeyboardController
#include "Inputs\KeyboardController.h"

#include "System/LoadOBJ.h"
#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CZombie3D : public CSolidObject, public CFSM
{
public:
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum class ENEMYMOVEMENT : unsigned int
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		NUM_MOVEMENTS
	};

	// Default Constructor
	CZombie3D(void);

	// Constructor with vectors
	CZombie3D(	const glm::vec3 vec3Position,
				const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f),
				const float fYaw = -90.0f,
				const float fPitch = 0.0f);

	// Destructor
	virtual ~CZombie3D(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Set Weapon to this class instance
	void SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo = NULL);

	// Get current weapon of this class instance
	CWeaponInfo* GetWeapon(void) const;

	// Set current weapon
	void SetCurrentWeapon(const int iSlot);

	// Discharge current weapon
	bool DischargeWeapon(void) const;

	// Attach a camera to this class instance
	void AttachCamera(CCamera* cCamera = NULL);
	// Check if a camera ia attached to this class instance
	bool IsCameraAttached(void);

    // Processes input received from any keyboard-like input system as Enemy movements. 
	// Accepts input parameter in the form of Enemy defined ENUM (to abstract it from windowing systems)
	void ProcessMovement(const ENEMYMOVEMENT direction, const float fDeltaTime);

    // Processes input received from a mouse input system as Enemy rotation. 
	// Expects the offset value in both the x and y direction.
	void ProcessRotate(const float fXOffset);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	// Enemy Attributes
	glm::vec3 vec3Up;
	glm::vec3 vec3Right;
	glm::vec3 vec3WorldUp;
	// Euler Angles
	float fYaw;
	float fPitch;
	// Enemy options
	float fRotationSensitivity;

	// Movement Control
	int iCurrentNumMovement;
	int iMaxNumMovement;

	// Detection distance for player
	float fDetectionDistance;

	// The handle to the CCamera class instance
	CCamera* cCamera;

	// The handle to the CWeaponInfo class instance
	CWeaponInfo* cPrimaryWeapon;
	CWeaponInfo* cSecondaryWeapon;
	int iCurrentWeapon;

	// The handle to the CPlayer3D class instance
	CPlayer3D* cPlayer3D;

	// The handle to the CTerrain class instance
	CTerrain* cTerrain;

	// WaypointManager
	CWaypointManager* cWaypointManager;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

    // Calculates the front vector from the Enemy's (updated) Euler Angles
	void UpdateEnemyVectors(void);

	// Constraint the Enemy's position
	void Constraint(void);

	// Update Front Vector and Yaw
	void UpdateFrontAndYaw(void);

	float dotProduct;
	float magnitudeE;
	float magnitudeP;
	float angle;
	bool state;
	int rMovement;
};
