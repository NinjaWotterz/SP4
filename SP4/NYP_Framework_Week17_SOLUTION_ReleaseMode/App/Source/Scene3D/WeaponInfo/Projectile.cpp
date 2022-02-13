/**
 CProjectile
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "Projectile.h"

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
CProjectile::CProjectile(void)
	: dLifetime(0.0f)
	, fSpeed(1.0f)
	, pSource(NULL)
{
}

/**
@brief Destructor
*/
CProjectile::~CProjectile(void)
{
	// We set it to NULL only since it was declared somewhere else
	pSource = NULL;
}

/** 
 @brief Set the position and direction of the projectile
 @param vec3Position A const glm::vec3 variable which contains the position of the projectile
 @param vec3Front A const glm::vec3 variable which contains the up direction of the projectile
 @param dLifetime A const float variable which contains the lifetime for this projectile
 @param fSpeed A const float variable which contains the speed of the projectile
 */
bool CProjectile::Init(	glm::vec3 vec3Position,
						glm::vec3 vec3Front,
						const double dLifetime,
						const float fSpeed)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::PROJECTILE);

	// Set the attributes for this projectile
	this->vec3Position = vec3Position;
	this->vec3Front = vec3Front;
	this->dLifetime = dLifetime;
	this->fSpeed = fSpeed;
	vec4Colour = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate the mesh
	mesh = CMeshBuilder::GenerateBox(vec4Colour, 0.1f, 0.1f, 0.1f);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Player.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Player.tga" << endl;
		return false;
	}

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CProjectile::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CProjectile::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CProjectile::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/** 
 @brief Activate the projectile. true == active, false == inactive
 @param bStatus A const bool variable which is true if this projectile is active, otherwise false.
 */
void CProjectile::SetStatus(const bool bStatus)
{
	if (bStatus == false)
		dLifetime = 0.0f;
	this->bStatus = bStatus;
}

/** 
 @brief Set the direction of the projectile
 @param vec3Front A const glm::vec3 variable which contains the up direction of the projectile
 */
void CProjectile::SetDirection(glm::vec3 vec3Front)
{
	this->vec3Front = vec3Front;
}

/** 
 @brief Set the lifetime of the projectile
 @param dLifetime A const float variable which contains the lifetime for this projectile
 */
void CProjectile::SetLifetime(const double dLifetime)
{
	this->dLifetime = dLifetime;
}

/** 
 @brief Set the speed of the projectile
 @param fSpeed A const float variable which contains the speed of the projectile
 */
void CProjectile::SetSpeed(const float fSpeed)
{
	this->fSpeed = fSpeed;
}

/** 
 @brief Set the source of the projectile
 @param pSource A CSolidObject* variable which is the CSolidObject who created this projectile
 */
void CProjectile::SetSource(CSolidObject* pSource)
{
	this->pSource = pSource;
}

/** 
 @brief Get the direction of the projectile
 @return A const glm::vec3 variable which contains the up direction of the projectile
 */
glm::vec3 CProjectile::GetDirection(void)
{
	return vec3Front;
}

/** 
 @brief Get the lifetime of the projectile
 @return A const float variable which contains the lifetime for this projectile
 */
double CProjectile::GetLifetime(void) const
{
	return dLifetime;
}

/** 
 @brief Get the speed of the projectile
 @return A const float variable which contains the speed of the projectile
 */
float CProjectile::GetSpeed(void) const
{
	return fSpeed;
}

/** 
 @brief Get the source of the projectile
 @return A CSolidObject* variable which is the CSolidObject who created this projectile
 */
CSolidObject* CProjectile::GetSource(void) const
{
	return pSource;
}

/** 
 @brief Update this projectile
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CProjectile::Update(const double dElapsedTime)
{
	if (bStatus == false)
		return false;

	// Update TimeLife of projectile. Set to inactive if too long
	dLifetime -= dElapsedTime;
	if (dLifetime < 0.0f)
	{
		SetStatus(false);
	}

	// Store current position
	vec3PreviousPosition = vec3Position;
	// Update Position
	vec3Position = vec3Position + vec3Front * (float)dElapsedTime * fSpeed;

	// Render the projectile
	// Create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CProjectile::PreRender(void)
{
	// Change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/** 
 @brief Render this projectile
 */
void CProjectile::Render(void)
{
	if (bStatus == false)
		return;

	if (dLifetime < 0.0f)
		return;

	CSolidObject::PreRender();
	CSolidObject::Render();
	CSolidObject::PostRender();
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CProjectile::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
@brief PrintSelf
*/
void CProjectile::PrintSelf(void)
{
	// Display the specifications of this projectile
	cout << "Position: [" << vec3Position.x << ", " << vec3Position.y << ", " << vec3Position.z << "], ";
	cout << "Direction: [" << vec3Front.x << ", " << vec3Front.y << ", " << vec3Front.z << "], ";
	cout << "LifeTime: " << dLifetime << ", Speed: " << fSpeed << ", Status: ";
	if (bStatus == true)
		cout << "Active" << endl;
	else
		cout << "Inactive" << endl;
}
