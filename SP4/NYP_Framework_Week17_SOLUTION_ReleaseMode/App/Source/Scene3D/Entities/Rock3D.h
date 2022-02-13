/**
 CRock3D
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

// Include Camera
#include "../Camera.h"

#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CRock3D : public CEntity3D
{
public:
	// Default Constructor
	CRock3D(void);

	// Destructor
	virtual ~CRock3D(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Set the instancing mode
	virtual void SetInstancingMode(const bool bInstancedRendering);
	// Set the number of instances
	virtual void SetNumOfInstance(const unsigned int iNumOfInstance);
	// Set the spread distance
	virtual void SetSpreadDistance(const float fSpreadDistance);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	bool IsInstancedRendering(void) { return bInstancedRendering; }

	// Print Self
	virtual void PrintSelf(void);

protected:
	//GLuint VBO, IBO;
	unsigned int iNumOfInstance;
	float fSpreadDistance;
	unsigned int iIndicesSize;

	// Bool flag to indicate if you want to use Instanced Rendering, or use normal rendering for debugging purpose.
	// true == Instanced Rendering, false == normal rendering.
	bool bInstancedRendering;
};
