/**
 CKeyboardController
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

#include "../DesignPatterns/SingletonTemplate.h"
#include <bitset>
#include <windows.h>

class CKeyboardController : public CSingletonTemplate<CKeyboardController>
{
	friend CSingletonTemplate<CKeyboardController>;

public:
	// Constant int to indicate how many keys will be processed
	const static int MAX_KEYS = 348;

	// System Interface
	// Perform pre-update operations
	void PreUpdate(void);
	// Perform update operation for a key
	void Update(const int key, const int action);
	// Perform post-update operations
	void PostUpdate(void);

	// User Interface
	bool IsKeyDown(const int key);
	bool IsKeyUp(const int key);
	bool IsKeyPressed(const int key);
	bool IsKeyReleased(const int key);

	// Reset a key
	void ResetKey(const int key);
	// Reset all keys
	void Reset(void);

protected:
	// Constructor
	CKeyboardController(void);

	// Destructor
	virtual ~CKeyboardController(void);

	// Bitset to store information about current and previous keypress statuses
	std::bitset<MAX_KEYS> currStatus, prevStatus;
};
