/**
 Target3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Target3D.h"
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
Target3D::Target3D(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
	health = 1;
}

Target3D::Target3D(const glm::vec3 inVec3Position, const glm::vec3 vec3Front) {
	this->vec3Position = inVec3Position;
	//this->vec3Position.y += +fHeightOffset;
	//this->vec3Front = vec3Front;
	this->fYaw = -90;
	this->fPitch = 0;
	health = 1;
}

/**
 @brief Destructor
 */
Target3D::~Target3D(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);

}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool Target3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::TARGET);
	health = 1;

	//// Generate and bind the VAO
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);

	//mesh = CMeshBuilder::GenerateBox(glm::vec4(1, 1, 1, 1));

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/Target/target.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models/Target/target.obj" << endl;
		return false;
	}

	CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(ModelVertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	iIndicesSize = index_buffer_data.size();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Target/target.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Target/target.tga" << endl;
		return false;
	}

	// Store the handler to the terrain
	cTerrain = CTerrain::GetInstance();


	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void Target3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void Target3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void Target3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}


/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool Target3D::Update(const double dElapsedTime)
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
void Target3D::PreRender(void)
{
	// Change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate normal 3D shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
@brief Render Render this instance
*/
void Target3D::Render(void)
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
void Target3D::PostRender(void)
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
void Target3D::PrintSelf(void)
{
	cout << "Target3D::PrintSelf()" << endl;
	cout << "========================" << endl;
	CEntity3D::PrintSelf();
}
