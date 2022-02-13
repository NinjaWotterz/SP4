#include "MouseController.h"
#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CMouseController::CMouseController()
	: curr_posX(0.0), curr_posY(0.0)
	, prev_posX(0.0), prev_posY(0.0)
	, delta_posX(0.0), delta_posY(0.0)
	, currBtnStatus(0), prevBtnStatus(0)
	, WheelOffset_X(0), WheelOffset_Y(0)
	, bKeepMouseCentered(true)
	, bFirstUpdate(true)
{
}

/**
 @brief Destructor
 */
CMouseController::~CMouseController()
{
}

/// Update this class istance
/**
 @brief Update the mouse position
 @param _x A const double variable containing the new mouse position in the x-axis
 @param _y A const double variable containing the new mouse position in the y-axis
 */
void CMouseController::UpdateMousePosition(const double _x, const double _y)
{
	if (bFirstUpdate)
	{
		prev_posX = _x;
		prev_posY = _y;
		bFirstUpdate = false;
	}

	// Store the current positions to the previous positions
	prev_posX = curr_posX;
	prev_posY = curr_posY;

	// Update the current positions
	curr_posX = _x;
	curr_posY = _y;

	// Update the position delta
	delta_posX = curr_posX - prev_posX;
	delta_posY = prev_posY - curr_posY;
}

/**
 @brief Update a mouse button when it is pressed down
 @param _slot A const int variable which stores the button ID
 */
void CMouseController::UpdateMouseButtonPressed(const int _slot)
{
	currBtnStatus |= 1 << _slot;
}

/**
 @brief Update a mouse button when it is released
 @param _slot A const int variable which stores the button ID
 */
void CMouseController::UpdateMouseButtonReleased(const int _slot)
{
	currBtnStatus &= ~(1 << _slot);
}

/**
 @brief Update the mouse scroll status
 @param WheelOffset_X A const double variable which stores the amount of offset in the x-axis
 @param WheelOffset_Y A const double variable which stores the amount of offset in the y-axis
 */
void CMouseController::UpdateMouseScroll(const double WheelOffset_X, const double WheelOffset_Y)
{
	this->WheelOffset_X += WheelOffset_X;
	if (this->WheelOffset_X > 3.0)
		this->WheelOffset_X = 3.0;
	else if (this->WheelOffset_X < -3.0)
		this->WheelOffset_X = -3.0;
	this->WheelOffset_Y += WheelOffset_Y;
	if (this->WheelOffset_Y > 3.0)
		this->WheelOffset_Y = 3.0;
	else if (this->WheelOffset_Y < -3.0)
		this->WheelOffset_Y = -3.0;
}

/**
@brief Do a post update for this class instance
*/
void CMouseController::PostUpdate(void)
{
	// Update the position delta
	delta_posX = 0.0;
	delta_posY = 0.0;

	prevBtnStatus = currBtnStatus;

	// Update Wheel Scroll Status
	WheelOffset_X = 0.0;
	WheelOffset_Y = 0.0;
}

/**
 @brief Check if a button is pressed down
 @param _slot A const int variable which stores the button ID
 */
bool CMouseController::IsButtonDown(const unsigned char _slot)
{
	return (currBtnStatus & (1 << _slot)) != 0;
}

/**
@brief Check if a button is up, a.k.a not pressed at all
@param _slot A const int variable which stores the button ID
*/
bool CMouseController::IsButtonUp(const unsigned char _slot)
{
	return !(IsButtonDown(_slot));
}

/**
@brief Check if a button is pressed and held down
@param _slot A const int variable which stores the button ID
*/
bool CMouseController::IsButtonPressed(const unsigned char _slot)
{
	// True if currently button is down, previously is up
	return IsButtonDown(_slot) && !(prevBtnStatus & (1 << _slot));
}

/**
 @brief Check if a button is released
 @param _slot A const int variable which stores the button ID
 */
bool CMouseController::IsButtonReleased(const unsigned char _slot)
{
	// True if currently button is up, previously is down
	return IsButtonUp(_slot) && (prevBtnStatus & (1 << _slot));
}

/**
@brief Get the mouse scroll status
@param _scrolltype A SCROLL_TYPE variable containing the axis of scrolling to get for
@return A double variable containing the y-axis offset for the scroll wheel
*/
double CMouseController::GetMouseScrollStatus(const SCROLL_TYPE _scrolltype) const
{
	if (_scrolltype == SCROLL_TYPE_XOFFSET)
	{
		return WheelOffset_X;
	}
	else if (_scrolltype == SCROLL_TYPE_YOFFSET)
	{
		return WheelOffset_Y;
	}
	else
	{
		return 0.0;
	}
}

/**
 @brief Get the mouse position in the X-axis
 @return A double variable containing the x-axis for mouse position
 */
double CMouseController::GetMousePositionX(void) const
{
	return curr_posX;
}

/**
 @brief Get the mouse position in the X-axis
 @return A double variable containing the y-axis for mouse position
 */
double CMouseController::GetMousePositionY(void) const
{
	return curr_posY;
}

/**
 @brief Get the mouse position delta in the X-axis
 @return A double variable containing the x-axis for mouse delta
 */
double CMouseController::GetMouseDeltaX(void) const
{
	return delta_posX;
}

/**
 @brief Get the mouse position delta in the Y-axis
 @return A double variable containing the y-axis for mouse delta
 */
double CMouseController::GetMouseDeltaY(void) const
{
	return delta_posY;
}