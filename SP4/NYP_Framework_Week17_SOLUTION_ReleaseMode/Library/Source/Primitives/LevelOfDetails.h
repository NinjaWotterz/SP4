#pragma once

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

class CLevelOfDetails
{
public:
	enum DETAIL_LEVEL
	{
		HIGH_DETAILS,
		MID_DETAILS,
		LOW_DETAILS,
		NUM_DETAIL_LEVEL
	};

	// Array containing the distances to switch LOD
	float arrLODDistance[3];

	// Constructor
	CLevelOfDetails(void);
	// Destructor
	virtual ~CLevelOfDetails(void);

	// Get the status of this LOD
	virtual bool GetLODStatus(void) const;
	// Set the status of this LOD
	virtual void SetLODStatus(const bool m_bLODStatus);

	// Set the current level of details
	virtual bool SetDetailLevel(const DETAIL_LEVEL eDetailLevel);
	// Get the current level of details
	virtual DETAIL_LEVEL GetDetailLevel(void) const;

protected:
	// Boolean flag to indicate if this LOD is active
	bool m_bLODStatus;
	// Variable storing the current level of details
	DETAIL_LEVEL eDetailLevel;

	// OpenGL objects using arrays
	GLuint arrVAO[3];

	// Arrays of indices Sizes
	GLuint arrIndicesSize[3];

	// The texture ID in OpenGL
	GLuint arriTextureID[3];
};
