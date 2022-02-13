// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "Play3DGameState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include CKeyboardController
#include "Inputs/KeyboardController.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CPlay3DGameState::CPlay3DGameState(void)
	: CScene3D(NULL)
{

}

/**
 @brief Destructor
 */
CPlay3DGameState::~CPlay3DGameState(void)
{

}

/**
 @brief Init this class instance
 */
bool CPlay3DGameState::Init(void)
{
	cout << "CPlay3DGameState::Init()\n" << endl;

	// Initialise the CScene3D instance
	CScene3D = CScene3D::GetInstance();
	if (CScene3D->Init() == false)
	{
		cout << "Failed to load Scene2D" << endl;
		return false;
	}

	return true;
}

/**
 @brief Update this class instance
 */
bool CPlay3DGameState::Update(const double dElapsedTime)
{
	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_ESCAPE))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
		cout << "Loading MenuState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
		CGameStateManager::GetInstance()->OffPauseGameState();
		return true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F10))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
		cout << "Loading PauseState" << endl;
		CGameStateManager::GetInstance()->SetPauseGameState("PauseState");
	}

	// Call the CScene3D's Update method
	CScene3D->Update(dElapsedTime);

	return true;
}

/**
 @brief Render this class instance
 */
void CPlay3DGameState::Render(void)
{
	//cout << "CPlay3DGameState::Render()\n" << endl;

	// Call the CScene3D's Pre-Render method
	CScene3D->PreRender();

	// Call the CScene3D's Render method
	CScene3D->Render();

	// Call the CScene3D's PostRender method
	CScene3D->PostRender();
}

/**
 @brief Destroy this class instance
 */
void CPlay3DGameState::Destroy(void)
{
	cout << "CPlay3DGameState::Destroy()\n" << endl;

	// Destroy the CScene3D instance
	if (CScene3D)
	{
		CScene3D->Destroy();
		CScene3D = NULL;
	}
}