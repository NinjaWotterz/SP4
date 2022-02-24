/**
 CCameraShake2
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "CameraShake2.h"

 // Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include math.h
#include <math.h>

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CCameraShake2::CCameraShake2(void)
	: cCamera(NULL)
	, fAngle(0.0f)
	, vec2Magnitude(glm::vec2(0.0f))
	, bToBeUpdated(false)
	, bPrepareToStop(false)
{
}

/**
 @brief Destructor
 */
CCameraShake2::~CCameraShake2(void)
{
	cCamera = NULL;
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCameraShake2::Init(void)
{
	// Call the parent's Init()
	CCameraEffects::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Store the camera's instance to this handle
	cCamera = CCamera::GetInstance();

	// Magnitude of the camera shake
	//vec2Magnitude = glm::vec2(0.10f, 0.05f);
	vec2Magnitude = glm::vec2(0.10f * 2, 0.05f * 2);
	fAngle = 1.570797f;

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	bStatus = true;
	return true;
}

/**
 @brief Override the SetStatus() from CEntity3D
 @param bStatus A const bool variable containing the new status
 */
void CCameraShake2::SetStatus(const bool bStatus)
{
	if (bStatus == true)
	{
		// If bStatus is true, then just assign to this->bStatus
		this->bStatus = bStatus;
	}
	else
	{
		// If bStatus is true, then just assign to this->bStatus
		bPrepareToStop = true;
	}
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CCameraShake2::Update(const double dElapsedTime)
{
	if ((!bStatus) || (!bToBeUpdated))
		return false;

	// Update fAngle
	fAngle += 0.0174533f * 150.0f;	// 0.0.0174533f radians == 1 degree
	if (fAngle > 6.28319f)			// If fAngle is > 360 degrees
		fAngle = fAngle - 6.28319f;	// Then deduct the value by 360 degrees

	// Update the camera's position on the x-z axes
	cCamera->vec3Offset = cCamera->vec3Right * (vec2Magnitude.x * cos(fAngle));
	// Update the camera's position on the y axis
	//cCamera->vec3Offset.y = vec2Magnitude.y * sin(2 * fAngle) * 0.5f;

	// Set to false since we have already updated camera shake
	bToBeUpdated = false;

	cout << "Camera is shaking" << endl;
	// If bPrepareToStop == true, then stop the camera shake when fAngle 
	// is around 1.570797f. This will prevent sudden camera reset
	if ((bPrepareToStop) && ((fAngle > 15.70f) && (fAngle < 15.71f)))
	{
		bStatus = false;
		bPrepareToStop = false;
	}

	return true;
}


/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCameraShake2::PreRender(void)
{
	if (!bStatus)
		return;
}

/**
 @brief Render this instance
 */
void CCameraShake2::Render(void)
{
	if (!bStatus)
		return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CCameraShake2::PostRender(void)
{
	if (!bStatus)
		return;
}
