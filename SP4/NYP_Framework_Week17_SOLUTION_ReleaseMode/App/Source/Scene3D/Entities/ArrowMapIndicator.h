#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>
// Include CSolidObject
#include "SolidObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class ArrowMapIndicator : public CSolidObject
{
public:
	enum MAP_ICON_TYPE
	{
		PLAYER = 0,
		ENEMY,
		TOWER,
		NUM_TYPES
	};


	// Default Constructor
	ArrowMapIndicator(void);
	ArrowMapIndicator(const glm::vec3 vec3Position, const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f));

	// Destructor
	virtual ~ArrowMapIndicator(void);

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
	//// Render
	//virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Print Self
	virtual void PrintSelf(void);

	const ArrowMapIndicator::MAP_ICON_TYPE GetMapIconType() const;
	void SetMapIconType(ArrowMapIndicator::MAP_ICON_TYPE);
	//int health;
protected:
	//float fYaw;
	//float fPitch;
	//// The handle to the CTerrain class instance
	//CTerrain* cTerrain;
		// Entity Type
	MAP_ICON_TYPE iconType;
};
