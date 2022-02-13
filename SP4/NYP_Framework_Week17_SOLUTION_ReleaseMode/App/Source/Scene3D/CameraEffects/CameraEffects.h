/**
 CCameraEffects
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

class CCameraEffects : public CEntity3D
{
public:
	// Constructor
	CCameraEffects(void);
	// Destructor
	virtual ~CCameraEffects(void);

	// Initialise this class instance
	virtual bool Init(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Set Countdown Timer
	virtual bool SetTimer(const float fCountdownTimer = 2.0f);
	// Get Countdown Timer
	virtual float GetTimer(void) const;

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Print Self
	void PrintSelf(void);

protected:
	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// Countdown Timer
	float fCountdownTimer;
};
