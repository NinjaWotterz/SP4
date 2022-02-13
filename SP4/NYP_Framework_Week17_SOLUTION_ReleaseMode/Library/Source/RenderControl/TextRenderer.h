/**
 CTextRenderer
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CEntity2D
#include "../Primitives/Entity2D.h"

#include <string>
#include <map>

class CTextRenderer : public CSingletonTemplate<CTextRenderer>, public CEntity2D
{
	friend CSingletonTemplate<CTextRenderer>;

	// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

public:
	// Init
	bool Init(void);

	// Update
	void Update(void);

	// PreRender
	void PreRender(void);

	// Render
	void Render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 colour);

	// PostRender
	void PostRender(void);

protected:
	// Map of characters
	std::map<GLchar, Character> Characters;

	// Constructor
	CTextRenderer(void);

	// Destructor
	virtual ~CTextRenderer(void);
};

