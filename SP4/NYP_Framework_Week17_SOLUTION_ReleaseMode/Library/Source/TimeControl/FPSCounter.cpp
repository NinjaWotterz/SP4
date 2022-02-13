#include "FPSCounter.h"
#include <iostream>
using namespace std;

CFPSCounter::CFPSCounter()
	: dElapsedTime(0.0)
	, nFrames(0)
	, iFrameRate(0)
	, dFrameTime(0.0)
{
	Init();
}


CFPSCounter::~CFPSCounter()
{
}


// Initialise the class instance
void CFPSCounter::Init(void)
{
	dElapsedTime = 0.0;
	nFrames = 0;
	iFrameRate = 0;
	dFrameTime = 60;
}

// Update the class instance
void CFPSCounter::Update(const double deltaTime)
{
	// Update elapsed time
	dElapsedTime += deltaTime;

	// Update the frame count
	nFrames++;

	if (dElapsedTime >= 1.0){ // If last update was more than 1 sec ago...
		// Calculate the current frame rate
		dFrameTime = 1000.0 / double(nFrames);

		// Update the frame count for the last 1 second
		iFrameRate = nFrames;

		// Reset timer and update the lastTime
		nFrames = 0;
		dElapsedTime = 0.0;
	}
}

// Get the current frame rate
int CFPSCounter::GetFrameRate(void) const
{
	return iFrameRate;
}

// Get a string containing the current frame rate
std::string CFPSCounter::GetFrameRateString(void) const
{
	return to_string(iFrameRate);
}

// Get the current frame time
double CFPSCounter::GetFrameTime(void) const
{
	return dFrameTime;
}
