// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include "OptionState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include Mesh Builder
#include "Primitives/MeshBuilder.h"
// Include ImageLoader
#include "System\ImageLoader.h"
// Include Shader Manager
#include "RenderControl\ShaderManager.h"

 // Include shader
#include "RenderControl\shader.h"

// Include CSettings
#include "GameControl/Settings.h"

// Include CKeyboardController
#include "Inputs/KeyboardController.h"

//Include Sound Controller
#include "../SoundController/SoundController.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
COptionState::COptionState(void)
	: background(NULL)
{

}

/**
 @brief Destructor
 */
COptionState::~COptionState(void)
{

}

/**
 @brief Init this class instance
 */
bool COptionState::Init(void)
{
	cout << "CMenuState::Init()\n" << endl;

	CShaderManager::GetInstance()->Use("2DShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	//Create Background Entity
	background = new CBackgroundEntity("Image/Background/optionImage.png");
	background->SetShader("2DShader");
	background->Init();

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

	// Load the images for buttons
	CImageLoader* il = CImageLoader::GetInstance();

	//Needed
	//Volume Up
	volumeUpButonData.fileName = "Image\\GUI\\buttonVolumeUp.png";
	volumeUpButonData.textureID = il->LoadTextureGetID(volumeUpButonData.fileName.c_str(), false);
	//Volume Down
	volumeDownButtonData.fileName = "Image\\GUI\\buttonVolumeDown.png";
	volumeDownButtonData.textureID = il->LoadTextureGetID(volumeDownButtonData.fileName.c_str(), false);
	//Mute button
	muteButtonData.fileName = "Image\\GUI\\buttonMute.png";
	muteButtonData.textureID = il->LoadTextureGetID(muteButtonData.fileName.c_str(), false);

	//untick image
	checkBoxUntickImageData.fileName = "Image\\GUI\\checkBoxUntick.png";
	checkBoxUntickImageData.textureID = il->LoadTextureGetID(checkBoxUntickImageData.fileName.c_str(), false);

	//tick button
	checkBoxTickImageData.fileName = "Image\\GUI\\checkBoxTick.png";
	checkBoxTickImageData.textureID = il->LoadTextureGetID(checkBoxTickImageData.fileName.c_str(), false);

	//back button
	backButtonData.fileName = "Image\\GUI\\buttonBack.png";
	backButtonData.textureID = il->LoadTextureGetID(backButtonData.fileName.c_str(), false);

	// Enable the cursor
	if (CSettings::GetInstance()->bDisableMousePointer == true)
		glfwSetInputMode(CSettings::GetInstance()->pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	return true;
}

/**
 @brief Update this class instance
 */
bool COptionState::Update(const double dElapsedTime)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoNav;

	float buttonWidth = 200;
	float buttonHeight = 100;
	float smallImageWidth = 50;
	float smallImageHeight = 50;

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Setting", NULL, window_flags);
		ImGui::SetWindowPos(ImVec2(CSettings::GetInstance()->iWindowWidth/1.3  - buttonWidth / 2.0,
			CSettings::GetInstance()->iWindowHeight / 1.9f));				// Set the top-left of the window at (10,10)
		ImGui::SetWindowSize(ImVec2(CSettings::GetInstance()->iWindowWidth, CSettings::GetInstance()->iWindowHeight));

		// Add codes for Play3D button here
		if (!CSoundController::GetInstance()->GetMute())
		{
			ImGui::Image((ImTextureID)checkBoxUntickImageData.textureID,
				ImVec2(smallImageWidth, smallImageHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0));
		}
		else
		{
			ImGui::Image((ImTextureID)checkBoxTickImageData.textureID,
				ImVec2(smallImageWidth, smallImageHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0));
		}
		ImGui::End();
		// Create a window called "Hello, world!" and append into it.
		ImGui::Begin("Main Menu", NULL, window_flags);
		ImGui::SetWindowPos(ImVec2(CSettings::GetInstance()->iWindowWidth / 2.0 - buttonWidth / 2.0,
			CSettings::GetInstance()->iWindowHeight / 9.f));				// Set the top-left of the window at (10,10)
		ImGui::SetWindowSize(ImVec2(CSettings::GetInstance()->iWindowWidth, CSettings::GetInstance()->iWindowHeight));

		//Added rounding for nicer effect
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 200.0f;

		// Add codes for Play3D button here
		if (ImGui::ImageButton((ImTextureID)volumeUpButonData.textureID,
			ImVec2(buttonWidth, buttonHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0)))
		{

			// Load the menu state
			CSoundController::GetInstance()->MasterVolumeIncrease();
		}

		if (ImGui::ImageButton((ImTextureID)volumeDownButtonData.textureID,
			ImVec2(buttonWidth, buttonHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0)))
		{

			// Load the menu state
			CSoundController::GetInstance()->MasterVolumeDecrease();
		}

		if (ImGui::ImageButton((ImTextureID)muteButtonData.textureID,
			ImVec2(buttonWidth, buttonHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0)))
		{

			// Load the menu state
			if(CSoundController::GetInstance()->GetMute())
				CSoundController::GetInstance()->SetMute(0);
			else
				CSoundController::GetInstance()->SetMute(1);

		}

		if (ImGui::ImageButton((ImTextureID)backButtonData.textureID,
			ImVec2(buttonWidth, buttonHeight), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0)))
		{
			// Load the menu state
			cout << "Loading Menu State" << endl;
			CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
		}
		ImGui::End();
	}

	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_ESCAPE))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
		cout << "Loading Menu State" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
	}

	return true;
}

/**
 @brief Render this class instance
 */
void COptionState::Render(void)
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);

	//Render Background
	background->Render();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//cout << "CMenuState::Render()\n" << endl;
}

/**
 @brief Destroy this class instance
 */
void COptionState::Destroy(void)
{
	// Disable the cursor
	if (CSettings::GetInstance()->bDisableMousePointer == true)
		glfwSetInputMode(CSettings::GetInstance()->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		// Hide the cursor
		if (CSettings::GetInstance()->bShowMousePointer == false)
			glfwSetInputMode(CSettings::GetInstance()->pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Delete the background
	if (background)
	{
		delete background;
		background = NULL;
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	cout << "CMenuState::Destroy()\n" << endl;
}
