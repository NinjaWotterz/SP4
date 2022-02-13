/**
 Platform3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Platform3D.h"
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"



// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
Platform3D::Platform3D(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Platform3D::Platform3D(const glm::vec3 inVec3Position, const glm::vec3 vec3Front) {
	this->vec3Position = inVec3Position;
	//this->vec3Position.y += +fHeightOffset;
	//this->vec3Front = vec3Front;
	this->fYaw = -90;
	this->fPitch = 0;
}

/**
 @brief Destructor
 */
Platform3D::~Platform3D(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);

}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool Platform3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::STRUCTURE);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	mesh = CMeshBuilder::GenerateBox(glm::vec4(1, 1, 1, 1), 1.f, 1.f, 1.f);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Enemy_01.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Enemy_01.tga" << endl;
		return false;
	}

	// Store the handler to the terrain
	//cTerrain = CTerrain::GetInstance();


	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void Platform3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void Platform3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void Platform3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}


/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool Platform3D::Update(const double dElapsedTime)
{
	// Don't update if this entity is not active
	if (bStatus == false)
	{
		return false;
	}

	// Update the model
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void Platform3D::PreRender(void)
{
	// Change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate normal 3D shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
@brief Render Render this instance
*/
void Platform3D::Render(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::Render();
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void Platform3D::PostRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PostRender();
}

/**
 @brief Print Self
 */
void Platform3D::PrintSelf(void)
{
	cout << "Platform3D::PrintSelf()" << endl;
	cout << "========================" << endl;
	CEntity3D::PrintSelf();
}
