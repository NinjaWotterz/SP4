/**
 Map2D
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

class CImageLoader : public CSingletonTemplate<CImageLoader>
{
	friend CSingletonTemplate<CImageLoader>;
public:
	// Init
	bool Init(void);

	// Load an image and return as unsigned char*
	unsigned char * Load(	const char* filename, 
							int& width, int& height, 
							int& nrChannels, const bool bInvert = false);

	// Load an image and return as a Texture ID
	unsigned int LoadTextureGetID(const char* filename, const bool bInvert);
protected:
	// Constructor
	CImageLoader(void);

	// Destructor
	virtual ~CImageLoader(void);
};

