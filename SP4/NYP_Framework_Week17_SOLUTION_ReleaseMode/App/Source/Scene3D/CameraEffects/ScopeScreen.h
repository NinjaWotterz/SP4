/**
 CScopeScreen
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CScopeScreen : public CCameraEffects
{
public:
	// Constructor
	CScopeScreen(void);
	// Destructor
	virtual ~CScopeScreen(void);

	// Initialise this class instance
	virtual bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
