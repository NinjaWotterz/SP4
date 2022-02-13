/**
 CGUI_Scene3D
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "GUI_Scene3D.h"

// Include CBloodScreen
#include "CameraEffects/BloodScreen.h"
// Include CScopeScreen
#include "CameraEffects/ScopeScreen.h"
// Include CCrossHair
#include "CameraEffects/CrossHair.h"
// Include CCameraShake
#include "CameraEffects/CameraShake.h"
#include "CameraEffects/CameraRecoil.h"
#include "Entities\EntityManager.h"
#include "Entities\SolidObjectManager.h"
#include "Scene3D.h"
#include "../GameStateManagement/GameStateManager.h"
#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene3D::CGUI_Scene3D(void)
	: cSettings(NULL)
	, model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, m_fProgressBar(0.0f)
	, cCameraEffectsManager(NULL)
	, cMinimap(NULL)
	, cInventoryManager(NULL)
	, cInventoryItem(NULL)
	, cPlayer3D(NULL)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene3D::~CGUI_Scene3D(void)
{
	// Set the cPlayer3D to NULL since it was initialised elsewhere
	cPlayer3D = NULL;
	
	// Destroy the cInventoryManager and set it to NULL
	if (cInventoryManager)
	{
		cInventoryManager->Destroy();
		cInventoryManager = NULL;
	}

	// Cleanup IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Destroy the cMinimap and set it to NULL
	if (cMinimap)
	{
		cMinimap->Destroy();
		cMinimap = NULL;
	}

	// Destroy the cCameraEffects and set it to NULL
	if (cCameraEffectsManager)
	{
		cCameraEffectsManager->Destroy();
		cCameraEffectsManager = NULL;
	}

	// Set the cSettings to NULL since it was initialised elsewhere
	cSettings = NULL;
}

/**
  @brief Initialise this instance
  */
bool CGUI_Scene3D::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the CCameraEffectsManager
	cCameraEffectsManager = CCameraEffectsManager::GetInstance();
	cCameraEffectsManager->Init();
	bossSpawned = false;
	// Add the CameraEffects
	// Add BloodScreen
	CBloodScreen* cBloodScreen = new CBloodScreen();
	cBloodScreen->Init();
	cBloodScreen->SetShader("Shader_GUI");
	cCameraEffectsManager->Add("BloodScreen", cBloodScreen);
	// Add ScopeScreen
	CScopeScreen* cScopeScreen = new CScopeScreen();
	cScopeScreen->Init();
	cScopeScreen->SetShader("Shader_GUI");
	cCameraEffectsManager->Add("ScopeScreen", cScopeScreen);
	// Add CrossHair
	CCrossHair* cCrossHair = new CCrossHair();
	cCrossHair->Init();
	cCrossHair->SetShader("Shader_GUI");
	cCameraEffectsManager->Add("CrossHair", cCrossHair);

	CCameraRecoil* cCameraRecoil = new CCameraRecoil();
	cCameraRecoil->Init();
	cCameraEffectsManager->Add("CameraRecoil", cCameraRecoil);
	// Add CameraShake
	CCameraShake* cCameraShake = new CCameraShake();
	cCameraShake->Init();
	cCameraEffectsManager->Add("CameraShake", cCameraShake);

	// Load the Minimap
	cMinimap = CMinimap::GetInstance();
	// Set a shader to this class instance of Minimap
	cMinimap->SetShader("cRenderToTextureShader");
	cMinimap->Init();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(CSettings::GetInstance()->pWindow, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Define the window flags
	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	// Get the handler to the CInventoryManager instance
	cInventoryManager = CInventoryManager::GetInstance();
	// Add a Lives icon as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Lives", "Image/Scene3D_Complete.tga", 4, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	cInventoryItem = cInventoryManager->Add("Crystal", "Image/Scene2D_Lives.tga", 1, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a Health icon as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Health", "Image/Scene2D_Health.tga", 100, 100);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a pistol as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Pistol", "Image/Scene2D_TreeTile.tga", 5, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Get the handler to the CPlayer3D instance
	cPlayer3D = CPlayer3D::GetInstance();
	countdown = 0.f;
	mission_display_time = 0.f;
	mission_message = "";
	mission_cleared = 0;
	return true;
}


/**
 @brief Set model
 @param model A glm::mat4 variable to be assigned to this class instance
 */ 
void CGUI_Scene3D::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable to be assigned to this class instance
 */
void CGUI_Scene3D::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable to be assigned to this class instance
 */
void CGUI_Scene3D::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

void CGUI_Scene3D::StartTargetEvent(double eventTime, CStructure3D::EVENT_TRIGGER eventTypeTrigger) {
	countdown = eventTime;
	eventType = eventTypeTrigger;
}

CStructure3D::EVENT_TRIGGER CGUI_Scene3D::GetEventRunning() {
	return eventType;
}

bool CGUI_Scene3D::isRunnngMissionEndEvent(void) {
	return mission_display_time > 0;
}

bool CGUI_Scene3D::isRunnngTargetEvent(void) {
	return countdown > 0;
}

void CGUI_Scene3D::BossDead() {
	mission_display_time = 4.f;
	mission_message = "Mission Cleared!";
	mission_cleared += 1;
}


/**
 @brief Update this instance
 @param dElapsedTime A const double containing the elapsed time since the last frame
 */
void CGUI_Scene3D::Update(const double dElapsedTime)
{
	CSolidObjectManager* cSolidObjManager = CSolidObjectManager::GetInstance();
	
	// Update camera effects
	cCameraEffectsManager->Update(dElapsedTime);

	if (countdown > 0 && dElapsedTime < 1)
	{
		countdown -= (float)dElapsedTime;
		if (eventType == CStructure3D::EVENT_TRIGGER::TIME_ATTACK) {
			if (countdown <= 0 || cSolidObjManager->GetTargetsLeft() == 0) {
				countdown = 0;
				bool all_hit = cSolidObjManager->CheckAndDestroyTargets();
				mission_display_time = 4.f;
				if (all_hit) {
					// Show Mission Complete
					CScene3D::GetInstance()->SetCurrentMissionToSuccess();
					mission_message = "Mission Cleared!";
					mission_cleared += 1;
				}
				else {
					// Show Mission Fail
					mission_message = "Mission Failed!";
				}
			}
		}
		else if(eventType == CStructure3D::EVENT_TRIGGER::DEFEND_STRUCTURE) {
			if (countdown <= 0 || !cSolidObjManager->CheckTowerAlive()) {
				countdown = 0;
				mission_display_time = 4.f;
				CScene3D::GetInstance()->DestroyAllTowerAttackers();
				if (cSolidObjManager->CheckTowerAlive()) {
					// Show Mission Complete
					CScene3D::GetInstance()->SetCurrentMissionToSuccess();
					mission_message = "Mission Cleared!";
					mission_cleared += 1;
				}
				else {
					// Show Mission Fail
					
					mission_message = "Mission Failed!";
				}
			}
		}
		else if (eventType == CStructure3D::EVENT_TRIGGER::PLATFORMER) {
			CScene3D* cScene3D = CScene3D::GetInstance();
			if (countdown <= 0 || cScene3D->CheckClearPlatformer()) {
				countdown = 0;
				mission_display_time = 4.f;
				CScene3D::GetInstance()->DestroyAllCrystals();
				CScene3D::GetInstance()->DestroyAllObstacles();
				//CScene3D::GetInstance()->DestroyAllTowerAttackers();
				if (cScene3D->CheckClearPlatformer()) {
					// Show Mission Complete
					CScene3D::GetInstance()->SetCurrentMissionToSuccess();
					mission_message = "Mission Cleared!";
					mission_cleared += 1;
				}
				else {
					// Show Mission Fail
					CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
					CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Crystal");
					cInventoryItem->Remove(1);
					mission_message = "Mission Failed!";
				}
			}
		}
	}
	//}
	if (mission_display_time > 0 && dElapsedTime < 1)
	{
		mission_display_time -= (float)dElapsedTime;
		if (mission_display_time <= 0.f && mission_cleared == 3 && !bossSpawned) {
			mission_message = "BOSS ARRIVES!";
			mission_display_time = 4.f;
			CScene3D::GetInstance()->SpawnBoss();
			bossSpawned = true;
		}
		if (mission_display_time<= 0.f && mission_cleared == 4) {
			mission_message = "Game Completed!";
			mission_display_time = 4.f;
			mission_cleared += 1;
		}
		else if (mission_display_time <= 0.f && mission_cleared > 4) {
			// Reset the CKeyboardController
			CKeyboardController::GetInstance()->Reset();

			// Load the menu state
			cout << "Loading MenuState" << endl;
			CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
			CGameStateManager::GetInstance()->OffPauseGameState();
		}
	}

	// Calculate the relative scale to our default windows width
	const float relativeScale_x = cSettings->iWindowWidth / 800.0f;
	const float relativeScale_y = cSettings->iWindowHeight / 600.0f;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create an invisible window which covers the entire OpenGL window
	ImGui::Begin("Invisible window", NULL, window_flags);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2((float)cSettings->iWindowWidth, (float)cSettings->iWindowHeight));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);

	// Display the FPS
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "FPS: %d", cFPSCounter->GetFrameRate());

	// Render the Health
	ImGuiWindowFlags healthWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Health", NULL, healthWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.03f, cSettings->iWindowHeight * 0.03f));
	ImGui::SetWindowSize(ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	cInventoryItem = cInventoryManager->GetItem("Health");
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(cInventoryItem->vec2Size.x * relativeScale_x, cInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::ProgressBar(cInventoryItem->GetCount() /
		(float)cInventoryItem->GetMaxCount(), ImVec2(100.0f * relativeScale_x, 20.0f * relativeScale_y));
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::End();

	// Render the Lives
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));  // Set a background color
	ImGuiWindowFlags livesWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Lives", NULL, livesWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.03f, cSettings->iWindowHeight * 0.09f));
	ImGui::SetWindowSize(ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	cInventoryItem = cInventoryManager->GetItem("Lives");
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(cInventoryItem->vec2Size.x * relativeScale_x, cInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d / %d",
		mission_cleared, cInventoryItem->GetMaxCount());
	ImGui::End();
	ImGui::PopStyleColor();

	// Render Count Down UI

	if (countdown > 0) {
		ImGuiWindowFlags countdownFlag = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		ImGui::Begin("Time", NULL, countdownFlag);
		ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.3f, cSettings->iWindowHeight * 0.09f));
		ImGui::SetWindowSize(ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	
		// 1.34567 * 100 -> 134.567 -> 134 -> 1.34
		ImGui::SetWindowFontScale(2.f * relativeScale_y);
		if (eventType == CStructure3D::EVENT_TRIGGER::TIME_ATTACK) {
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Targets left: %s", std::to_string(cSolidObjManager->GetTargetsLeft()).c_str());
		}
		else if (eventType == CStructure3D::EVENT_TRIGGER::DEFEND_STRUCTURE) {
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Tower Health: %s", std::to_string(cSolidObjManager->GetTowerHealth()).c_str());
		}
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Time Left: %s", std::to_string(int(countdown * 100) / 100.f).substr(0, 5).c_str());
	

		ImGui::End();
		ImGui::PopStyleColor();
	}

	if (mission_display_time > 0) {
		ImGuiWindowFlags missionFlag = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		ImGui::Begin("GameStatus", NULL, missionFlag);
		ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.45f, cSettings->iWindowHeight * 0.45f));
		ImGui::SetWindowSize(ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));

		ImGui::SetWindowFontScale(2.f * relativeScale_y);
		if (mission_message == "Mission Cleared!" || mission_message == "Game Completed!") {
			ImGui::TextColored(ImVec4(0, 1, 0, 1), mission_message.c_str());
		}
		else {
			ImGui::TextColored(ImVec4(1, 0, 0, 1), mission_message.c_str());
		}

		ImGui::End();
		ImGui::PopStyleColor();
	}

	// Render the inventory items
	cInventoryItem = cInventoryManager->GetItem("Pistol");
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));  // Set a background color
	ImGuiWindowFlags inventoryWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Image", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.01f, cSettings->iWindowHeight * 0.8f));
	ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 40.0f * relativeScale_y));
	ImGui::Image((void*)(intptr_t)cPlayer3D->GetWeapon()->GetIconTextureID(), //get weapon texture
		ImVec2(cInventoryItem->vec2Size.x * relativeScale_x, cInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 0), ImVec2(1, 1));
	ImGui::SameLine();
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Weapon : %s", cPlayer3D->GetWeapon()->GetWeaponName().c_str());
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Ammo : %d / %d",
		cPlayer3D->GetWeapon()->GetMagRound(), cPlayer3D->GetWeapon()->GetMaxMagRound());
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Total Ammo : %d / %d",
		cPlayer3D->GetWeapon()->GetTotalRound(), cPlayer3D->GetWeapon()->GetMaxTotalRound());
	ImGui::End();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));  // Set a background color
	ImGui::Begin("PlayerPos", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.72f, cSettings->iWindowHeight * 0.9f));
	ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Position : ");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%3.2f, %3.2f, %3.2f",
		CCamera::GetInstance()->vec3Position.x,
		CCamera::GetInstance()->vec3Position.y,
		CCamera::GetInstance()->vec3Position.z);
	ImGui::End();
	ImGui::PopStyleColor();

	// for invisible window
	ImGui::End();


}


/**
 @brief Set up the OpenGL display environment before rendering
 */
void CGUI_Scene3D::PreRender(void)
{
}

/**
 @brief Render this instance
 */
void CGUI_Scene3D::Render(void)
{
	// Render the camera effects
	cCameraEffectsManager->Render();

	// Render the minimap
	cMinimap->PreRender();
	cMinimap->Render();
	cMinimap->PostRender();

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		cPlayer3D->GetWeapon()->SetProjection(projection);
		cPlayer3D->GetWeapon()->PreRender();
		cPlayer3D->GetWeapon()->Render();
		cPlayer3D->GetWeapon()->PostRender();
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGUI_Scene3D::PostRender(void)
{
}
