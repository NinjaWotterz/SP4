/**
 CCameraShake
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

// Include CCamera
#include "../Camera.h"

class CCameraShake : public CCameraEffects
{
public:
	// Constructor
	CCameraShake(void);
	// Destructor
	virtual ~CCameraShake(void);

	// Initialise this class instance
	bool Init(void);

	// Override the SetStatus() from CEntity3D
	virtual void SetStatus(const bool bStatus);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Boolean flag to indicate if this class is to be updated
	bool bToBeUpdated;

protected:
	// The handle to the camera instance
	CCamera* cCamera;

	// Angle in radians
	float fAngle;
	// The magnitude of the camera shake
	glm::vec2 vec2Magnitude;
	// Boolean flag to indicate this camera shake should prepare to stop
	bool bPrepareToStop;

};
