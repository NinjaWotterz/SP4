/**
 CEntity2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Settings
#include "..\GameControl\Settings.h"
#include <string>

//CS: Include Mesh.h to use to draw (include vertex and index buffers)
#include "Mesh.h"
using namespace std;

class CEntity2D
{
public:
	// Constructor
	CEntity2D(void);

	// Destructor
	virtual ~CEntity2D(void);

	// Init
	virtual bool Init(void);

	// Set the name of the shader to be used in this class instance
	virtual void SetShader(const std::string& _name);

	// Update
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);

	// Render
	virtual void Render(void);

	// PostRender
	virtual void PostRender(void);

	// The i32vec2 which stores the indices of an Entity2D in the Map2D
	glm::i32vec2 i32vec2Index;

	// The i32vec2 variable which stores The number of microsteps from the tile indices for the Entity2D. 
	// A tile's width or height is in multiples of these microsteps
	glm::i32vec2 i32vec2NumMicroSteps;

	// The vec2 variable which stores the UV coordinates to render the Entity2D
	glm::vec2 vec2UVCoordinate;

protected:
	// Name of Shader Program instance
	std::string sShaderName;

	//CS: The mesh that is used to draw objects
	CMesh* mesh;

	// OpenGL objects
	unsigned int VBO, VAO, EBO;

	// The texture ID in OpenGL
	unsigned int iTextureID;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// Load a texture
	virtual bool LoadTexture(const char* filename);

	// Settings
	CSettings* cSettings;
};
