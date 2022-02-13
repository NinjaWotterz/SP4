#include "Collider.h"

// Include Shader Manager
#include "..\RenderControl\ShaderManager.h"

#include <GLFW/glfw3.h>

/**
 @brief Default Constructor
 */
CCollider::CCollider(void)
	: boxMin(glm::vec3(-0.5f, -0.5f, -0.5f))
	, boxMax(glm::vec3(0.5f, 0.5f, 0.5f))
	, boxColour(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f))
	, bIsDisplayed(true)
	, fLineWidth(3.0f)
{
}

/**
 @brief Default Destructor
 */
CCollider::~CCollider(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

/**
 @brief Initialise this class instance
 @param _name A const std::string& variable which stores the name of the shader
*/
bool CCollider::InitCollider(	const std::string& _name,
								glm::vec4 boxColour,
								glm::vec3 boxMin,
								glm::vec3 boxMax)
{
	this->SetColliderShader(_name);
	this->boxMin = boxMin;
	this->boxMax = boxMax;
	this->boxColour = boxColour;

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		boxMin.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMax.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,

		boxMin.x, boxMin.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMin.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMin.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,

		boxMin.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMax.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMin.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,

		boxMax.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMin.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,

		boxMin.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMin.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMin.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMin.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMin.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,

		boxMin.x, boxMax.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMax.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMax.y, boxMax.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a,
		boxMin.x, boxMax.y, boxMin.z, boxColour.x, boxColour.y, boxColour.z, boxColour.a
	};

	glGenVertexArrays(1, &colliderVAO);
	glGenBuffers(1, &colliderVBO);

	glBindVertexArray(colliderVAO);

	glBindBuffer(GL_ARRAY_BUFFER, colliderVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glLineWidth(fLineWidth);

	return true;
}

/**
 @brief Set a shader to this class instance
 @param _name The name of the Shader instance in the CShaderManager
 */
void CCollider::SetColliderShader(const std::string& _name)
{
	sColliderShaderName = _name;
}

/**
 @brief PreRender
 */
void CCollider::PreRender(void)
{
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use the shader defined for this class
	CShaderManager::GetInstance()->Use(sColliderShaderName);
}

/**
 @brief Render
 */
void CCollider::Render(void)
{
	if (!bIsDisplayed)
		return;

	CShaderManager::GetInstance()->activeShader->setMat4("model", colliderModel);
	CShaderManager::GetInstance()->activeShader->setMat4("view", colliderView);
	CShaderManager::GetInstance()->activeShader->setMat4("projection", colliderProjection);

	// render box
	glBindVertexArray(colliderVAO);
	glDrawArrays(GL_LINE_LOOP, 0, 36);
	glBindVertexArray(0);
}

/**
 @brief PostRender
 */
void CCollider::PostRender(void)
{
	// Deactivate blending mode
	glDisable(GL_BLEND);
}
