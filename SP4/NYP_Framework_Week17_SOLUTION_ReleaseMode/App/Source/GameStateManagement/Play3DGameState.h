#pragma once

/**
 CPlay3DGameState
 @brief This class is derived from CGameState. It will introduce the game to the player.
 By: Toh Da Jun
 Date: July 2021
 */

#include "GameStateBase.h"

#include "..\Scene3D\Scene3D.h"

class CPlay3DGameState : public CGameStateBase
{
public:
	// Constructor
	CPlay3DGameState(void);
	// Destructor
	~CPlay3DGameState(void);

	// Init this class instance
	virtual bool Init(void);
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	// Render this class instance
	virtual void Render(void);
	// Destroy this class instance
	virtual void Destroy(void);

protected:
	// The handler to the CScene3D instance
	CScene3D* CScene3D;
};
