/**
 CStructure3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CSolidObject
#include "SolidObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "../Camera.h"

#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CStructure3D : public CSolidObject
{

public:
	enum EVENT_TRIGGER
	{
		TIME_ATTACK = 0,
		DEFEND_STRUCTURE,
		PLATFORMER,
		BOSS,
		NUM_TYPES
	};



	// Default Constructor
	CStructure3D(void);

	// Constructor with vectors
	CStructure3D(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f));

	// Destructor
	virtual ~CStructure3D(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	void SetEvent(EVENT_TRIGGER);
	EVENT_TRIGGER GetEvent();

	void ResetHealth();
	void ReduceHealth(int damage);
	int GetHealth();
	bool IsDead();
	bool IsMissionCompleted();
	void SetMissionComplete(bool isComplete);
private:
	EVENT_TRIGGER eventType;
	int health;
	bool missionCompleted;
};
