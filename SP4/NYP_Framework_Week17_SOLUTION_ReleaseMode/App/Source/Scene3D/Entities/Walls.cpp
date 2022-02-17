/**
CWalls3D
 */
#include "Walls.h"
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
CWalls3D::CWalls3D(void)
	: bInstancedRendering(false)
	, iNumOfInstance(10)
	, fSpreadDistance(5.0f)
	, iIndicesSize(0)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CWalls3D::CWalls3D(const glm::vec3 vec3Position,
	const glm::vec3 vec3Front,
	const float fYaw,
	const float fPitch)
	: bInstancedRendering(false)
	, iNumOfInstance(10)
	, fSpreadDistance(5.0f)
	, iIndicesSize(0)
{
	// Set the default position to the origin
	this->vec3Position = vec3Position;
}

/**
 @brief Destructor
 */
CWalls3D::~CWalls3D(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CWalls3D::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Prepare to load the OBJ
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::string file_path = "Models/Dungeon/wall2.obj";
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
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/dungeon/dungeon.jpg", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/dungeon.jpg" << endl;
		return false;
	}

	if (bInstancedRendering)
	{
		// Generate the list of transformation matrices which 
		// indicates where each instance will be at, and its orientation
		CTerrain* cTerrainInstance = CTerrain::GetInstance();
		iNumOfInstance = cTerrainInstance->GetTotalWalls();
		glm::mat4* modelMatrices = new glm::mat4[iNumOfInstance];

		// Initialize random seed
		srand((unsigned int)glfwGetTime());
		int index = 0;
		for (unsigned int i = 0; i < 100; i+=1)
		{
			for (unsigned int j = 0; j < 100; j+=1)
			{
				// x will go -49.5 to 49.5
				float x = -49.5f + i;
				float z = -49.5f + j;
				float y = 4.82352972+1.5f; 
				/*try {
					y = CTerrain::GetInstance()->GetHeight(x, z);
				}
				catch (exception e) { y = 4.82352972; }*/

				if (!cTerrainInstance->IsInWall(glm::vec3(x, y, z))) {
					//y = 5+4.82352972;
					continue;
				}
				glm::mat4 model = glm::mat4(1.0f);
				// 1. Use Translation to randomly disperse the trees in a rectangular area
				
				model = glm::translate(model, glm::vec3(x, y, z));

				// 2. Use scaling to create trees of various sizes
				float scale = 1.f; // (rand() % 100) * 0.0005f + 0.01f;
				model = glm::scale(model, glm::vec3(vec3Scale.x * scale,
					vec3Scale.y * scale,
					vec3Scale.z * scale));

				// 3. Add this transformation matrix to list of matrices
				modelMatrices[index] = model;
				index += 1;
			}
		}

		// configure instanced array
		// -------------------------
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
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
void CWalls3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CWalls3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CWalls3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Set the instancing mode
@param bInstancedRendering A const bool containing the status of the instancing mode
*/
void CWalls3D::SetInstancingMode(const bool bInstancedRendering)
{
	this->bInstancedRendering = bInstancedRendering;
}

/**
 @brief Set the number of instances
 @param iNumOfInstance A const unsigned int containing the number of instances to render for this entity
 */
void CWalls3D::SetNumOfInstance(const unsigned int iNumOfInstance)
{
	this->iNumOfInstance = iNumOfInstance;
}

/**
 @brief Set the spread distance
 @param fSpreadDistance A const float containing the spread distance when randomly generating positions for the instances
 */
void CWalls3D::SetSpreadDistance(const float fSpreadDistance)
{
	this->fSpreadDistance = fSpreadDistance;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CWalls3D::Update(const double dElapsedTime)
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
void CWalls3D::PreRender(void)
{
	// Change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate Instanced Rendering shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
@brief Render Render this instance
*/
void CWalls3D::Render(void)
{
	if (bInstancedRendering)
	{
		// configure transformation matrices
		CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
		CShaderManager::GetInstance()->activeShader->setMat4("view", view);

		// draw a tree
		CShaderManager::GetInstance()->activeShader->setInt("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, iTextureID); // note: we also made the textures_loaded vector public (instead of private) from the model class.

		// We instanced render the trees. if you have multiple VAOs, then you can render them using this loop
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

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, iTextureID);
		// Render the OBJ
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CWalls3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Print Self
 */
void CWalls3D::PrintSelf(void)
{
	cout << "CWalls3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}
