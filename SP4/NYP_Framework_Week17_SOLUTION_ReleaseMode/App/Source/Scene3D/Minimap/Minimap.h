/**
 CMinimap
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include <Primitives\Entity3D.h>

#include <GL/glew.h>
#include <includes\glm.hpp>

#include <RenderControl\shader.h>
#include <vector>

class CMinimap : public CEntity3D, public CSingletonTemplate<CMinimap>
{
	friend CSingletonTemplate<CMinimap>;
public:
	GLuint VBO;
	GLuint FBO;
	GLuint uiTextureColorBuffer;
	GLuint RBO;
	GLuint VAO_BORDER, VBO_BORDER;

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Activate rendering to the Minimap
	virtual void Activate(void);
	// Deactivate rendering to the Minimap
	virtual void Deactivate(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// PrintSelf
	void PrintSelf(void);

protected:
	// Constructor
	CMinimap(void);

	// Destructor
	~CMinimap(void);
};
