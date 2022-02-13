/**
 CGUI_Scene3D
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns/SingletonTemplate.h"

// Include CEntity2D
#include "Primitives/Entity2D.h"

// FPS Counter
#include "TimeControl\FPSCounter.h"

// Include CInventoryManager
#include "..\Scene2D\InventoryManager.h"

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include IMGUI
// Important: GLEW and GLFW must be included before IMGUI
#ifndef IMGUI_ACTIVE
#include "GUI\imgui.h"
#include "GUI\backends\imgui_impl_glfw.h"
#include "GUI\backends\imgui_impl_opengl3.h"
#define IMGUI_ACTIVE
#endif

#include "GameControl/Settings.h"

// Include CCameraEffectsManager
#include "CameraEffects/CameraEffectsManager.h"
// Include Minimap
#include "Minimap/Minimap.h"

// Include CPlayer3D
#include "Entities\Player3D.h"
#include "Entities\Structure3D.h"

#include <string>
using namespace std;

class CGUI_Scene3D : public CSingletonTemplate<CGUI_Scene3D>, public CEntity2D
{
	friend CSingletonTemplate<CGUI_Scene3D>;
public:
	// Init
	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// Set model
	virtual void SetModel(glm::mat4 model);

	// Set view
	virtual void SetView(glm::mat4 view);

	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// PreRender
	virtual void PreRender(void);

	// Render
	virtual void Render(void);

	// PostRender
	virtual void PostRender(void);

	virtual void StartTargetEvent(double eventTime, CStructure3D::EVENT_TRIGGER eventTypeTrigger);
	virtual bool isRunnngTargetEvent(void);
	virtual bool isRunnngMissionEndEvent(void);
	void BossDead(void);
	CStructure3D::EVENT_TRIGGER GetEventRunning();
protected:
	// Constructor
	CGUI_Scene3D(void);

	// Destructor
	virtual ~CGUI_Scene3D(void);

	// Render Settings
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// FPS Control
	CFPSCounter* cFPSCounter;
	bool bossSpawned;
	double countdown; // for target event
	double mission_display_time; // for win/lose event message display period
	string mission_message; // win/lose event message
	int mission_cleared;

	// Flags for IMGUI
	ImGuiWindowFlags window_flags;
	float m_fProgressBar;

	// Handler to the CSettings
	CSettings* cSettings;

	// Handler to the CCameraEffectsManager
	CCameraEffectsManager* cCameraEffectsManager;
	// Handler to the CMinimap
	CMinimap* cMinimap;

	// The handler containing the instance of CInventoryManager
	CInventoryManager* cInventoryManager;
	// The handler containing the instance of CInventoryItem
	CInventoryItem* cInventoryItem;

	// The handler containing the instance of CPlayer3D
	CPlayer3D* cPlayer3D;

	CStructure3D::EVENT_TRIGGER eventType;
};
