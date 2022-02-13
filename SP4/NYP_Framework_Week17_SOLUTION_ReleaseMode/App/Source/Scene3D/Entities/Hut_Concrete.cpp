/**
 CHut_Concrete
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Hut_Concrete.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include CPlayer3D
#include "../Entities/Player3D.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CHut_Concrete::CHut_Concrete(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, fHeightOffset, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 */
CHut_Concrete::CHut_Concrete(	const glm::vec3 vec3Position,
								const glm::vec3 vec3Front)
{
	this->vec3Position = glm::vec3(vec3Position.x, vec3Position.y + fHeightOffset, vec3Position.z);
	this->vec3Front = vec3Front;
}

/**
 @brief Destructor
 */
CHut_Concrete::~CHut_Concrete(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CHut_Concrete::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::STRUCTURE);

	if (m_bLODStatus == false)
	{
		if (LoadModelAndTexture("Models/Hut_Concrete/Hut_Concrete_H.obj", 
								"Models/Hut_Concrete/Hut_Concrete_H.tga",
								VAO, iTextureID, iIndicesSize) == false)
		{
			cout << "Unable to load model and texture" << endl;
		}
	}
	else
	{
		if (LoadModelAndTexture("Models/Hut_Concrete/Hut_Concrete_H.obj",
								"Models/Hut_Concrete/Hut_Concrete_H.tga",
								arrVAO[0], arriTextureID[0], arrIndicesSize[0]) == false)
		{
			cout << "Unable to load high LOD model and texture" << endl;
		}
		if (LoadModelAndTexture("Models/Hut_Concrete/Hut_Concrete_M.obj",
								"Models/Hut_Concrete/Hut_Concrete_M.tga",
								arrVAO[1], arriTextureID[1], arrIndicesSize[1]) == false)
		{
			cout << "Unable to load mid LOD model and texture" << endl;
		}
		if (LoadModelAndTexture("Models/Hut_Concrete/Hut_Concrete_L.obj",
								"Models/Hut_Concrete/Hut_Concrete_L.tga",
								arrVAO[2], arriTextureID[2], arrIndicesSize[2]) == false)
		{
			cout << "Unable to load low LOD model and texture" << endl;
		}
	}

	return true;
}

/**
 @brief Load a model and texture
 @param filenameModel A const char* variable containing the filename for the model
 @param filenameTexture A const char* variable containing the filename for the texture
 @param VAO A GLuint& variable containing the VAO to be returned
 @param iTextureID A GLuint& variable containing the iTextureID to be returned
 @param iIndicesSize A GLuint& variable containing the iIndicesSize to be returned
 @return A bool variable
 */
bool CHut_Concrete::LoadModelAndTexture(const char* filenameModel,
										const char* filenameTexture,
										GLuint& VAO,
										GLuint& iTextureID,
										GLuint& iIndicesSize)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = filenameModel;
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load " << filenameModel << endl;
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
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID(filenameTexture, false);
	if (iTextureID == 0)
	{
		cout << "Unable to load " << filenameTexture << endl;
		return false;
	}

	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CHut_Concrete::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CHut_Concrete::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CHut_Concrete::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CHut_Concrete::Update(const double dElapsedTime)
{
	CSolidObject::Update(dElapsedTime);

	if (m_bLODStatus)
	{
		float fDistanceToPlayer = glm::distance(vec3Position, CPlayer3D::GetInstance()->GetPosition());
		if (fDistanceToPlayer > arrLODDistance[LOW_DETAILS])
			eDetailLevel = LOW_DETAILS;
		else if (fDistanceToPlayer > arrLODDistance[MID_DETAILS])
			eDetailLevel = MID_DETAILS;
		else
			eDetailLevel = HIGH_DETAILS;
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CHut_Concrete::PreRender(void)
{
	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CHut_Concrete::Render(void)
{
	if (m_bLODStatus == false)
	{
		// note: currently we set the projection matrix each frame, but since the projection 
		// matrix rarely changes it's often best practice to set it outside the main loop only once.
		CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
		CShaderManager::GetInstance()->activeShader->setMat4("view", view);
		CShaderManager::GetInstance()->activeShader->setMat4("model", model);

		// bind textures on corresponding texture units
		glBindTexture(GL_TEXTURE_2D, iTextureID);
		glActiveTexture(GL_TEXTURE0);
			// Render the mesh
			glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		// note: currently we set the projection matrix each frame, but since the projection 
		// matrix rarely changes it's often best practice to set it outside the main loop only once.
		CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
		CShaderManager::GetInstance()->activeShader->setMat4("view", view);
		CShaderManager::GetInstance()->activeShader->setMat4("model", model);

		// bind textures on corresponding texture units
		glBindTexture(GL_TEXTURE_2D, arriTextureID[eDetailLevel]);
		glActiveTexture(GL_TEXTURE0);
			// Render the mesh
			glBindVertexArray(arrVAO[eDetailLevel]);
				glDrawElements(GL_TRIANGLES, arrIndicesSize[eDetailLevel], GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CHut_Concrete::PostRender(void)
{
	CSolidObject::PostRender();
}

/**
 @brief Print Self
 */
void CHut_Concrete::PrintSelf(void)
{
	cout << "CHut_Concrete::PrintSelf()" << endl;
	cout << "========================" << endl;
}
