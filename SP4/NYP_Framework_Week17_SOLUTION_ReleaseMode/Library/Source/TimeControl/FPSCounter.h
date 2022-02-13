/**
CKeyboardController
By: Toh Da Jun
Date: Mar 2020
*/
#pragma once

// Include SingtonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

#include <string>

class CFPSCounter : public CSingletonTemplate<CFPSCounter>
{
	friend CSingletonTemplate<CFPSCounter>;

public:
	// Destructor
	virtual ~CFPSCounter(void);

	// Initialise the class instance
	void Init(void);

	// Update the class instance
	void Update(const double deltaTime);

	// Get the current frame rate
	int GetFrameRate(void) const;

	// Get a string containing the current frame rate
	std::string GetFrameRateString(void) const;

	// Get the current frame time
	double GetFrameTime(void) const;

protected:
	// Count the elapsed time since the last reset
	double dElapsedTime;
	// Count the number of frames for the current second
	int nFrames;
	// The frame count for the last 1 second
	int iFrameRate;
	// Count the elapsed time since the last reset
	double dFrameTime;

	// Constructor
	CFPSCounter(void);
};
