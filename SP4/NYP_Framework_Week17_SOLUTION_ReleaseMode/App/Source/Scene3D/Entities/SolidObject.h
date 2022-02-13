/**
 CSolidObject
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include Collider
#include <Primitives/Collider.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CSolidObject : public CEntity3D, public CCollider
{
public:
	// Default Constructor
	CSolidObject(void);

	// Constructor with vectors
	CSolidObject(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f));

	// Destructor
	virtual ~CSolidObject(void);

	// Initialise this class instance
	bool Init(void);
	// Initialise the collider
	bool InitCollider(	const std::string& _name,
						glm::vec4 boxColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
						glm::vec3 boxMin = glm::vec3(-0.5f, -0.5f, -0.5f),
						glm::vec3 boxMax = glm::vec3(0.5f, 0.5f, 0.5f));

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

	// Print Self
	virtual void PrintSelf(void);

protected:
	// The number of indices in the model loaded in
	unsigned int iIndicesSize;
};
