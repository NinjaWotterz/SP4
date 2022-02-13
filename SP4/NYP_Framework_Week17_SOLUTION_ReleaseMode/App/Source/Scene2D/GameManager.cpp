#include "GameManager.h"

/**
@brief Constructor
*/
CGameManager::CGameManager(void) 
	: bPlayerWon(false)
	, bPlayerLost(false)
	, bLevelCompleted(false)
	, bLevelToReplay(false)
	, bGameToRestart(false) {
}

/**
@brief Destructor
*/
CGameManager::~CGameManager(void)
{
}

// Initialise this class instance
void CGameManager::Init(void)
{
	bPlayerWon = false;
	bPlayerLost = false;
	bLevelCompleted = false;
	bLevelToReplay = false;
	bGameToRestart = false;
}

