/**
 CCameraEffects
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "CameraEffects.h"

 // Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CCameraEffects::CCameraEffects(void)
	: transform(glm::mat4(1.0f))
	, fCountdownTimer(2.0f)
{
}

/**
 @brief Destructor
 */
CCameraEffects::~CCameraEffects(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCameraEffects::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate the mesh
	mesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 2.0f, 2.0f);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/CameraEffects_Blood.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/GUI/CameraEffects_Blood.tga" << endl;
		return false;
	}

	bStatus = false;

	return true;
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CCameraEffects::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CCameraEffects::SetView(const glm::mat4 view)
{
	this->view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CCameraEffects::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/** 
 @brief Set Countdown Timer
 @param fCountdownTimer A const float variable containing the new countdown timer
 @return A bool variable
 */
bool CCameraEffects::SetTimer(const float fCountdownTimer)
{
	this->fCountdownTimer = fCountdownTimer;
	return true;
}

/** 
 @brief Get Countdown Timer
 @return A float variable
 */
float CCameraEffects::GetTimer(void) const
{
	return fCountdownTimer;
}

/**
 @brief Update
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
 */
bool CCameraEffects::Update(const double dElapsedTime)
{
	if (bStatus == false)
		return false;

	fCountdownTimer -= (float)dElapsedTime;
	if (fCountdownTimer <= 0.0f)
	{
		// Set this Camera Effects' status to false
		bStatus = false;
		// Reset the countdown timer to the default value
		fCountdownTimer = 2.0f;
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCameraEffects::PreRender(void)
{
	if (!bStatus)
		return;

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render this instance
 */
void CCameraEffects::Render(void)
{
	if (!bStatus)
		return;

	unsigned int transformLoc;
	// get matrix's uniform location and set matrix
	transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");

	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	unsigned int colorLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "runtime_color");
	glUniform4fv(colorLoc, 1, glm::value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));

	// Get the texture to be rendered
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// Render the quad
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	mesh->Render();

	// Reset to default
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CCameraEffects::PostRender(void)
{
	if (!bStatus)
		return;

	// Disable blending
	glDisable(GL_BLEND);
}

/**
 @brief Print Self
 */
void CCameraEffects::PrintSelf(void)
{
	cout << "CCameraEffects::PrintSelf()" << endl;
	cout << "========================" << endl;
}