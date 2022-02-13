/**
 CBloodScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CBloodScreen : public CCameraEffects
{
public:
	// Constructor
	CBloodScreen(void);
	// Destructor
	virtual ~CBloodScreen(void);

	// Initialise this class instance
	bool Init(void);
};
