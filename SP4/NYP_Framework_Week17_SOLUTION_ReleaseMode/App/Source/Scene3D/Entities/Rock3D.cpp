/**
 CRock3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Rock3D.h"
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include CTerrain
#include "../Terrain/Terrain.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CRock3D::CRock3D(void)
	: bInstancedRendering(false)
	, iNumOfInstance(10)
	, fSpreadDistance(5.0f)
	, iIndicesSize(0)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Destructor
 */
CRock3D::~CRock3D(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CRock3D::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Prepare to load the OBJ
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	
	std::string file_path = "Models/rock/rock.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
		return false;
	
	// Load the vertices and indices
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	iIndicesSize = index_buffer_data.size();

	// Load the data to the graphics card
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(ModelVertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	iIndicesSize = index_buffer_data.size();

	// Load the details to the shader program in the graphics card
	// We can customised for instanced rendering or normal rendering here.
	// The index for glVertexAttribPointer must match the .vs shader file
	if (bInstancedRendering)
	{
		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
		// texture coord attribute
		// The location in the instancing .vs shader file is 2, so we must indicate 2 for glVertexAttribPointer(2,...
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
		glBindVertexArray(0);
	}
	else
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/rock/rock.png", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/rock/rock.png" << endl;
		return false;
	}

	if (bInstancedRendering)
	{
		// Generate the list of transformation matrices which 
		// indicates where each instance will be at, and its orientation
		glm::mat4* modelMatrices = new glm::mat4[iNumOfInstance];

		// Initialize random seed
		srand((unsigned int)glfwGetTime());

		for (unsigned int i = 0; i < iNumOfInstance; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// 1. Use Translation to randomly disperse the rocks in a rectangular area
			float x = vec3Position.x + (rand() % 100) * fSpreadDistance * 0.01f - fSpreadDistance * 0.5f;
			float z = vec3Position.z + (rand() % 100) * fSpreadDistance * 0.01f - fSpreadDistance * 0.5f;
			float y = CTerrain::GetInstance()->GetHeight(x, z);
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. Use scaling to create rocks of various sizes
			float scale = (rand() % 100) * 0.0005f + 0.01f;
			model = glm::scale(model, glm::vec3(vec3Scale.x * scale,
				vec3Scale.y * scale,
				vec3Scale.z * scale));

			// 3. Randomly rotate the rock around a (semi)randomly picked rotation axis vector
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. Add this transformation matrix to list of matrices
			modelMatrices[i] = model;
		}

		// configure instanced array
		// -------------------------
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, iNumOfInstance * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
		// Delete the array of modelMatrices since we had already uploaded it to the graphics card
		delete[] modelMatrices;

		// Set transformation matrices as an instance vertex attribute (with divisor 1)
		// We simplify our codes by loading the VAO as well
		// Use the loop for multiple VAOs for this rendering pass
		for (unsigned int i = 0; i < 1; i++)
		{
			glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}

	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CRock3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CRock3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CRock3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Set the instancing mode
@param bInstancedRendering A const bool containing the status of the instancing mode
*/
void CRock3D::SetInstancingMode(const bool bInstancedRendering)
{
	this->bInstancedRendering = bInstancedRendering;
}

/**
 @brief Set the number of instances
 @param iNumOfInstance A const unsigned int containing the number of instances to render for this entity
 */
void CRock3D::SetNumOfInstance(const unsigned int iNumOfInstance)
{
	this->iNumOfInstance = iNumOfInstance;
}

/**
 @brief Set the spread distance
 @param fSpreadDistance A const float containing the spread distance when randomly generating positions for the instances
 */
void CRock3D::SetSpreadDistance(const float fSpreadDistance)
{
	this->fSpreadDistance = fSpreadDistance;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CRock3D::Update(const double dElapsedTime)
{
	if (!bInstancedRendering)
	{
		// create transformations
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, vec3Position);
		model = glm::rotate(model, -1.5708f, glm::vec3(1, 0, 0));
		model = glm::scale(model, vec3Scale);
	}

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CRock3D::PreRender(void)
{
	// Change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate normal 3D shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
@brief Render Render this instance
*/
void CRock3D::Render(void)
{
	if (bInstancedRendering)
	{
		// configure transformation matrices
		CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
		CShaderManager::GetInstance()->activeShader->setMat4("view", view);

		// draw rocks
		CShaderManager::GetInstance()->activeShader->setInt("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, iTextureID); // note: we also made the textures_loaded vector public (instead of private) from the model class.
		
		// We instanced render the rock. if you have multiple VAOs, then you can render them using this loop
		for (unsigned int i = 0; i < 1; i++)
		{
			glBindVertexArray(VAO);
			glDrawElementsInstanced(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0, iNumOfInstance);
			glBindVertexArray(0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		// note: currently we set the projection matrix each frame, but since the projection 
		// matrix rarely changes it's often best practice to set it outside the main loop only once.
		CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
		CShaderManager::GetInstance()->activeShader->setMat4("view", view);
		CShaderManager::GetInstance()->activeShader->setMat4("model", model);

		// Render the OBJ
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, iTextureID);
			glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CRock3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Print Self
 */
void CRock3D::PrintSelf(void)
{
	cout << "CRock3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}
