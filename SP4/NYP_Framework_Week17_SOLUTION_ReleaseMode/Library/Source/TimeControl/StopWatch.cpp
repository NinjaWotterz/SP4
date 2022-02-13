#include "StopWatch.h"

#define TARGET_RESOLUTION 1         // 1-millisecond target resolution

/**
@brief Constructor
*/
CStopWatch::CStopWatch(void)
{
	QueryPerformanceFrequency(&frequency);

	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
	{
		// Error; application can't continue.
	}

	wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(wTimerRes);
}

/**
@brief Destructor
*/ 
CStopWatch::~CStopWatch(void)
{
	StopTimer();
}

/**
@brief Initialise this class instance
*/
void Init(void)
{

}

/**
@brief Start Timer
*/ 
void CStopWatch::StartTimer(void)
{
	QueryPerformanceCounter(&prevTime);
}


/**
 @brief Stop timer
 */
void CStopWatch::StopTimer(void)
{
	timeEndPeriod(wTimerRes);
}

/**
 @brief Get elapsed time in seconds since the last call to this function
 */ 
double CStopWatch::GetElapsedTime(void)
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&currTime);
	time.QuadPart = currTime.QuadPart - prevTime.QuadPart;
	prevTime = currTime;
	return LIToSecs(time);
}

/**
 @brief Wait until this time in milliseconds has passed
 */
void CStopWatch::WaitUntil(const long long llTime)
{
	LARGE_INTEGER nowTime;
	LONGLONG timeElapsed;
	while (true)
	{
		QueryPerformanceCounter(&nowTime);
		timeElapsed = ((nowTime.QuadPart - prevTime.QuadPart) * 1000 / frequency.QuadPart);
		if (timeElapsed > llTime)
			return;
		else if (llTime - timeElapsed > 1)
			Sleep(1);
	}
}

/**
 @brief Convert from Large Integer to seconds in double
 @param L A LARGE_INTEGER & containing the time value
 @return The time in seconds unit and in double data type
 */
double CStopWatch::LIToSecs(LARGE_INTEGER & L)
{
	return ((double)L.QuadPart / (double)frequency.QuadPart);
}
