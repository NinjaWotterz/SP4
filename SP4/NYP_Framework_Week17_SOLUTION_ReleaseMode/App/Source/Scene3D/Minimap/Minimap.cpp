/**
 CMinimap
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Minimap.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CMinimap::CMinimap(void)
{
}

/**
 @brief Destructor
 */
CMinimap::~CMinimap(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteBuffers(1, &FBO);
	glDeleteBuffers(1, &uiTextureColorBuffer);
	glDeleteBuffers(1, &RBO);
	glDeleteVertexArrays(1, &VAO_BORDER);
	glDeleteBuffers(1, &VBO_BORDER);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CMinimap::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Set screenTexture to 0 in the shader program
	CShaderManager::GetInstance()->activeShader->setInt("screenTexture", 0);

	float vertices[] = 
	{
		// positions	// texCoords
		0.5f, 1.0f,		0.0f, 1.0f,
		0.5f, 0.5f,		0.0f, 0.0f,
		1.0f, 0.5f,		1.0f, 0.0f,

		0.5f, 1.0f,		0.0f, 1.0f,
		1.0f, 0.5f,		1.0f, 0.0f,
		1.0f, 1.0f,		1.0f, 1.0f
	};

	// Set up the rendering environment
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Framebuffer configuration
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// create a color attachment texture
	glGenTextures(1, &uiTextureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, uiTextureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cSettings->iWindowWidth, cSettings->iWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, uiTextureColorBuffer, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, cSettings->iWindowWidth, cSettings->iWindowHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // now actually attach it
	// now that we actually created the FBO and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Setup the border
	float vertices_border[] =
	{
		// positions	// texCoords
		0.5f, 1.0f,		0.0f, 1.0f,
		0.5f, 0.5f,		0.0f, 0.0f,
		1.0f, 0.5f,		1.0f, 0.0f,
		1.0f, 1.0f,		1.0f, 1.0f
	};

	// Set up the rendering environment
	glGenVertexArrays(1, &VAO_BORDER);
	glGenBuffers(1, &VBO_BORDER);
	glBindVertexArray(VAO_BORDER);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_BORDER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_border), &vertices_border, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CMinimap::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CMinimap::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CMinimap::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Activate rendering to the Minimap
 */
void CMinimap::Activate(void)
{
	// Start rendering the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Clear the framebuffer's content and display a pale green background
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Deactivate rendering to the Minimap
 */
void CMinimap::Deactivate(void)
{
	// Stop rendering the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
 */
bool CMinimap::Update(const double dElapsedTime)
{
	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CMinimap::PreRender(void)
{
	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render Render this instance
 @param cShader A Shader* variable which contains the Shader to use in this class instance
 */
void CMinimap::Render(void)
{
	// Render the texture for the minimap
	glBindVertexArray(VAO);
		// Use the color attachment texture as the texture of the quad plane
		glBindTexture(GL_TEXTURE_2D, uiTextureColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Reset to default
		glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	// Render the border
	glBindVertexArray(VAO_BORDER);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	// Reset to default
	glBindVertexArray(0);

}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CMinimap::PostRender(void)
{
}

/**
 @brief PrintSelf
 */
void CMinimap::PrintSelf(void)
{
	cout << "CMinimap::PrintSelf()" << endl;
}
