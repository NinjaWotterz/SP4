#include "KeyboardController.h"

/**
 @brief This Constructor is a protected access modified as this class instance will be a Singleton.
 */
CKeyboardController::CKeyboardController(void)
{
}

/**
 @brief This Destructor is a protected access modified as this class instance will be a Singleton.
 */
CKeyboardController::~CKeyboardController(void)
{
}

/**
 @brief Perform pre-update operations
 */
void CKeyboardController::PreUpdate(void)
{
}


/**
@brief Perform post-update operations
*/
void CKeyboardController::PostUpdate(void)
{
	// Copy the entire currStatus to prevStatus. Note that this method will not compare 
	// the current and previous status correctly for IsKeyPressed and IsKeyReleased
	prevStatus = currStatus;
}

/**
 @brief Perform update operation for a key
 */
void CKeyboardController::Update(const int key, const int action)
{
	if ((key >= 0) && (key < MAX_KEYS) && (action != 2))
	{
		// Backup the curStatus to prevStatus
		prevStatus.set(key, currStatus[key]);
		// Set the new status to curStatus
		currStatus.set(key, action);
	}
}

/**
 @brief Check if a key is pressed down
 @param int A const int variable to check in the currStatus array
 */
bool CKeyboardController::IsKeyDown(const int key)
{
	return currStatus.test(key);
}

/**
 @brief Check if a key is up a.k.a. not pressed down
 @param int A const int variable to check in the currStatus array
 */
bool CKeyboardController::IsKeyUp(const int key)
{
	return !currStatus.test(key);
}

/**
 @brief Check if a key is pressed down and held down
 @param int A const int variable to check in the currStatus and prevStatus array
 */
bool CKeyboardController::IsKeyPressed(const int key)
{
	return IsKeyDown(key) && !prevStatus.test(key);
}

/**
 @brief Check if a key is released after being held down
 @param int A const int variable to check in the currStatus array
 */
bool CKeyboardController::IsKeyReleased(const int key)
{
	return IsKeyUp(key) && prevStatus.test(key);
}

/**
 @brief Reset a key
 @param int A const int variable to reset a key in the currStatus and prevStatus array
 */
void CKeyboardController::ResetKey(const int key)
{
	currStatus[key] = false;
	prevStatus[key] = false;
}

/**
 @brief Reset all keys
 @param int A const int variable to reset a key in the currStatus and prevStatus array
 */
void CKeyboardController::Reset(void)
{
	for (int key = 0; key < MAX_KEYS; key++)
	{
		currStatus[key] = false;
		prevStatus[key] = false;
	}
}
