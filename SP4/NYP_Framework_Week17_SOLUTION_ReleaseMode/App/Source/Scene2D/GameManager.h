/**
CGameManager: This class manages the gameplay and determines 
			  if it should go to next level or end.
By: Toh Da Jun
Date: May 2021
*/
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include <map>
#include <string>
#include "InventoryItem.h"

class CGameManager : public CSingletonTemplate<CGameManager>
{
	friend CSingletonTemplate<CGameManager>;
public:
	// Initialise this class instance
	void Init(void);

	// Boolean flag to indicate if the player won the game
	bool bPlayerWon;
	// Boolean flag to indicate if the player lost the game
	bool bPlayerLost;
	// Boolean flag to indicate if the level is completed
	bool bLevelCompleted;

	// Boolean flag to indicate if the level is be replayed
	bool bLevelToReplay;
	// Boolean flag to indicate if the game is be restarted
	bool bGameToRestart;


protected:
	// Constructor
	CGameManager(void);

	// Destructor
	virtual ~CGameManager(void);
};
