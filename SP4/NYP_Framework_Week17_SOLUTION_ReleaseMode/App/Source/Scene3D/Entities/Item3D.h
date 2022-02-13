/**
 CItem3D
 */
#pragma once

 // Include CSolidObject
#include "SolidObject.h"
#include "System/LoadOBJ.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "../Camera.h"

// Include Player3D
//#include "Player3D.h"

// Include CTerrain
#include "../Terrain/Terrain.h"

#include <string>
using namespace std;

class CItem3D : public CSolidObject
{
public:
	
	enum class ITEMTYPE : unsigned int
	{
		BULLET = 0,
		HEALTH,
		CRYSTAL,
		NUM_ITEM
	};

	// Default Constructor
	CItem3D(void);

	// Constructor with vectors
	CItem3D(const glm::vec3 vec3Position,
		const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f),
		const float fYaw = -90.0f,
		const float fPitch = 0.0f);

	// Destructor
	virtual ~CItem3D(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Attach a camera to this class instance
	void AttachCamera(CCamera* cCamera = NULL);
	// Check if a camera ia attached to this class instance
	bool IsCameraAttached(void);

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
	ITEMTYPE GetItemType();
	void SetItemType(ITEMTYPE it);
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
	ITEMTYPE itemType;


	// The handle to the CCamera class instance
	CCamera* cCamera;


	// The handle to the CTerrain class instance
	CTerrain* cTerrain;
	// Constraint the Enemy's position
	void Constraint(void);



	// Update Front Vector and Yaw
	void UpdateFrontAndYaw(void);
};
