/**
 CGUI
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "GUI.h"

 // Include Shader Manager
#include "..\RenderControl\ShaderManager.h"

// Include ImageLoader
#include "..\System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CGUI::CGUI(void)
	: cTextRenderer(NULL)
	, cSettings(NULL)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CGUI::~CGUI(void)
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Show the mouse pointer
	if (cSettings->bDisableMousePointer == true)
		glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		// Hide the cursor
		if (cSettings->bShowMousePointer == false)
			glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// We won't delete this since it was created elsewhere
	cSettings = NULL;

	if (cTextRenderer)
	{
		cTextRenderer->Destroy();
		cTextRenderer = NULL;
	}
}

/**
  @brief Initialise this instance
  */
bool CGUI::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Create and initialise the Map 2D
	cTextRenderer = CTextRenderer::GetInstance();
	// Set a shader to this class
	cTextRenderer->SetShader("textShader");
	// Initialise the instance
	if (cTextRenderer->Init() == false)
	{
		cout << "Failed to load CTextRenderer" << endl;
		return false;
	}

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

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

	show_demo_window = true;
	show_another_window = false;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Show the mouse pointer
	glfwSetInputMode(CSettings::GetInstance()->pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	return true;
}

/**
 @brief Update this instance
 */
void CGUI::Update(const double dElapsedTime)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		// Create a window called "Hello, world!" and append into it.
		ImGui::Begin("Hello, world!", NULL, window_flags);
		//ImGui::SetWindowPos(ImVec2(100.0f, 100.0f));				// Set the top-left of the window at (10,10)
		ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetWindowSize(ImVec2((float)cSettings->iWindowWidth, (float)cSettings->iWindowHeight));
		
		ImVec4 col = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		//ImGui::TextUnformatted("FPS: %d", cFPSCounter->GetFrameRate());
		ImGui::Text("FPS: %d", cFPSCounter->GetFrameRate());
		ImGui::PopStyleColor();


		col = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col);
			col = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, col);
				ImGui::ProgressBar(0.9f, ImVec2(100.0f, 20.0f));
			ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::Begin("OpenGL Texture Text");
		ImGui::Image((void*)(intptr_t)3, ImVec2(50.0f, 50.0f));
		ImGui::End();

		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CGUI::PreRender(void)
{
	//// Reset the OpenGL rendering environment
	//glLoadIdentity();

	//// Clear the screen and buffer
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);

	// Call the Map2D's PreRender()
	cTextRenderer->PreRender();
}

/**
 @brief Render this instance
 */
void CGUI::Render(void)
{
	//// Call the CTextRenderer's Render()
	//cTextRenderer->Render("DM2213 2D Game Creation", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	//// Render FPS info
	//cTextRenderer->Render(	cFPSCounter->GetFrameRateString(),
	//						10.0f, 580.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGUI::PostRender(void)
{
	// Call the Map2D's PostRender()
	cTextRenderer->PostRender();
}
