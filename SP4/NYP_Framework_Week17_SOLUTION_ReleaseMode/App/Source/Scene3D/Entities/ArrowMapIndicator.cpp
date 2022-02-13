/**
 ArrowMapIndicator
 */
#include "ArrowMapIndicator.h"
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

//For allowing creating of Mesh 
#include "Primitives/MeshBuilder.h"

// Include CTerrain
// #include "../Terrain/Terrain.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
ArrowMapIndicator::ArrowMapIndicator(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Destructor
 */
ArrowMapIndicator::~ArrowMapIndicator(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool ArrowMapIndicator::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	mesh = CMeshBuilder::GenerateQuad(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
	/*mesh = CMeshBuilder::GenerateBox(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f, 1.0f, 1.0f);*/

	// load and create a texture 
	if (iconType == MAP_ICON_TYPE::PLAYER) {
		iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/PlayerArrow.tga", false);
	}
	else if (iconType == MAP_ICON_TYPE::ENEMY) {
		iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/PlayerArrow.tga", false);
	}
	else if (iconType == MAP_ICON_TYPE::TOWER) {
		iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/PlayerArrow.tga", false);
	}
	
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/GUI/PlayerArrow.tga" << endl;
		return false;
	}

	return true;
}

const ArrowMapIndicator::MAP_ICON_TYPE ArrowMapIndicator::GetMapIconType() const {
	return iconType;
}

void ArrowMapIndicator::SetMapIconType(ArrowMapIndicator::MAP_ICON_TYPE icon) {
	iconType = icon;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void ArrowMapIndicator::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void ArrowMapIndicator::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void ArrowMapIndicator::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}


/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool ArrowMapIndicator::Update(const double dElapsedTime)
{

	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, vec3Position);
	model = glm::rotate(model, GetRotationAngle(), GetRotationAxis());
	model = glm::rotate(model, -1.5708f, glm::vec3(1, 0, 0));
	model = glm::scale(model, vec3Scale);

	// create transformations
	//model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::translate(model, vec3Position);
	//model = glm::scale(model, vec3Scale);
	//model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	return true;
}

void ArrowMapIndicator::PreRender(void)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	//glDepthFunc(GL_LEQUAL);
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}


/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void ArrowMapIndicator::PostRender(void)
{
	//glDepthFunc(GL_LESS); // set depth function back to default

	// Disable blending
	glDisable(GL_BLEND);
}


/**
 @brief Print Self
 */
void ArrowMapIndicator::PrintSelf(void)
{
	cout << "ArrowMapIndicator::PrintSelf()" << endl;
	cout << "========================" << endl;
}
