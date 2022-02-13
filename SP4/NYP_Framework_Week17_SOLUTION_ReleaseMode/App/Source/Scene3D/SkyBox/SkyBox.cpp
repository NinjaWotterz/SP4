/**
 CSkyBox
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "SkyBox.h"

#include <includes\gtc\matrix_transform.hpp>

// Include ImageLoader
#include "System\ImageLoader.h"

#include "RenderControl/ShaderManager.h"

#include <iostream>
using namespace std;

// Load SkyBox textures
unsigned int LoadSkyBoxTextures(void);

/**
 @brief Default Constructor
 */
CSkyBox::CSkyBox(void)
{
}

/**
 @brief Destructor
 */
CSkyBox::~CSkyBox(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CSkyBox::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	float vertices[] = {
		// positions     
		// Rear
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		// Left
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		// Right
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		// Front
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		// Top
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		// Bottom
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// skybox VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Create the vector of skybox textures
	skyboxTexture.push_back("Image/SkyBox/skybox_right.jpg");
	skyboxTexture.push_back("Image/SkyBox/skybox_left.jpg");
	skyboxTexture.push_back("Image/SkyBox/skybox_top.jpg");
	skyboxTexture.push_back("Image/SkyBox/skybox_bottom.jpg");
	skyboxTexture.push_back("Image/SkyBox/skybox_front.jpg");
	skyboxTexture.push_back("Image/SkyBox/skybox_back.jpg");

	// Load the skybox textures into the Graphics Card
	iTextureID = LoadSkyBoxTextures();

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CSkyBox::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CSkyBox::SetView(glm::mat4 view)
{
	this->view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CSkyBox::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
 */
bool CSkyBox::Update(const double dElapsedTime)
{
	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CSkyBox::PreRender(void)
{
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render Render this instance
 @param cShader A Shader* variable which contains the Shader to use in this class instance
 */
void CSkyBox::Render(void)
{
	CShaderManager::GetInstance()->activeShader->setMat4("view", view);
	CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);

	// Render skybox cube
	glBindTexture(GL_TEXTURE_CUBE_MAP, iTextureID);
	glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CSkyBox::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

// Load SkyBox textures
// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
GLuint CSkyBox::LoadSkyBoxTextures(void)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);

	// Get the image loader
	CImageLoader* cImageLoader = CImageLoader::GetInstance();

	// Load each of the textures as a single texture ID
	int width, height, nrChannels;
	for (unsigned int i = 0; i < skyboxTexture.size(); i++)
	{
		unsigned char *data = cImageLoader->Load(skyboxTexture[i].c_str(), width, height, nrChannels, false);
		if (data)
		{
			std::cout << skyboxTexture[i] << " successfully loaded. " << std::endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << skyboxTexture[i] << std::endl;
		}
		// Free up the memory of the file data read in
		free(data);
	}
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//// Generate mipmaps
	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return textureID;
}

/**
 @brief Print Self
 */
void CSkyBox::PrintSelf(void)
{
	cout << "CSkyBox::PrintSelf()" << endl;
	cout << "========================" << endl;
}