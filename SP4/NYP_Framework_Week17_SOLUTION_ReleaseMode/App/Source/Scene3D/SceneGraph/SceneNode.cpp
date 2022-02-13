/**
 CSceneNode
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "SceneNode.h"
#include <algorithm>

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
@brief Constructor
*/
CSceneNode::CSceneNode(void)
	: snID(-1)
	, cParentNode(NULL)
	, worldTransformMtx(glm::mat4(1.0f))
	, worldTranslateMtx(glm::mat4(1.0f))
	, worldRotateMtx(glm::mat4(1.0f))
	, worldScaleMtx(glm::mat4(1.0f))
	, localTransformMtx(glm::mat4(1.0f))
	, localTranslateMtx(glm::mat4(1.0f))
	, localRotateMtx(glm::mat4(1.0f))
	, localScaleMtx(glm::mat4(1.0f))
	, updateTranslateMtx(glm::mat4(1.0f))
	, updateRotateMtx(glm::mat4(1.0f))
	, updateScaleMtx(glm::mat4(1.0f))
	, bContinuousUpdate(false)
{
}

/**
 @brief Constructor
 @param snID A const int variable containing the ID of this CSceneNode
 */
CSceneNode::CSceneNode(const int snID)
	: snID(-1)
	, cParentNode(NULL)
	, worldTransformMtx(glm::mat4(1.0f))
	, worldTranslateMtx(glm::mat4(1.0f))
	, worldRotateMtx(glm::mat4(1.0f))
	, worldScaleMtx(glm::mat4(1.0f))
	, localTransformMtx(glm::mat4(1.0f))
	, localTranslateMtx(glm::mat4(1.0f))
	, localRotateMtx(glm::mat4(1.0f))
	, localScaleMtx(glm::mat4(1.0f))
	, updateTranslateMtx(glm::mat4(1.0f))
	, updateRotateMtx(glm::mat4(1.0f))
	, updateScaleMtx(glm::mat4(1.0f))
	, bContinuousUpdate(false)
{
	this->snID = snID;
}

/**
@brief Destructor
*/
CSceneNode::~CSceneNode(void)
{
	// Destroy the children first
	DeleteAllChildren();

	// Reset the pointers for this node
	cParentNode = NULL;
}

/**
@brief Init Initialise this instance
@return true if the initialisation is successful, else false
*/
bool CSceneNode::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	mesh = CMeshBuilder::GenerateBox(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Structure_01.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Structure_01.tga" << endl;
		return false;
	}

	vec3Position.y += fHeightOffset;
}

void CSceneNode::InitMesh(CMesh* meshObj) {
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	mesh = meshObj;

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Structure_01.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Structure_01.tga" << endl;
	}

	vec3Position.y += fHeightOffset;
}

/**
 @brief Set model
 @param model A glm::mat4 variable to be assigned to this class instance
 */
void CSceneNode::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable to be assigned to this class instance
 */
void CSceneNode::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable to be assigned to this class instance
 */
void CSceneNode::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
 */
bool CSceneNode::Update(const double dElapsedTime)
{
	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// Use local transform
	if (bContinuousUpdate == true)
	{
		localTransformMtx = glm::mat4(1.0f);
		localTranslateMtx = updateTranslateMtx * localTranslateMtx;
		localRotateMtx = updateRotateMtx * localRotateMtx;
		localScaleMtx = updateScaleMtx * localScaleMtx;

		// Update the localTransformMtx
		localTransformMtx = localTranslateMtx * localRotateMtx * localScaleMtx;

		// Calculate the model matrix
		model = localTransformMtx * model;
	}

	if (cParentNode == NULL)
	{
		// Calculate the world transform
		worldTransformMtx = glm::mat4(1.0f);
		// Update the worldTransformMtx
		/*worldTransformMtx =	worldRotateMtx *
							worldScaleMtx *
							worldTranslateMtx;*/
		worldTransformMtx = worldTranslateMtx*worldRotateMtx*worldScaleMtx;
		// Calculate the model matrix
		model = worldTransformMtx * model;
	}
	else
	{
		// Calculate the world transform
		worldTransformMtx = cParentNode->model;
		worldTransformMtx =	worldTranslateMtx * worldRotateMtx * worldScaleMtx* worldTransformMtx;
		// Calculate the model matrix
		model = worldTransformMtx * model;
	}

	// Update the children
	std::vector<CSceneNode*>::iterator it;
	for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it)
	{
		(*it)->Update(dElapsedTime);
	}

	return true;
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CSceneNode::PreRender(void)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render this instance
 */
void CSceneNode::Render(void)
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
			mesh->Render();
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Update the children
	std::vector<CSceneNode*>::iterator it;
	for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it)
	{
		(*it)->SetView(view);
		(*it)->SetProjection(projection);
		(*it)->PreRender();
		(*it)->Render();
		(*it)->PostRender();
	}
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CSceneNode::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Add a child to this node
 @param s A CSceneNode* to be added to this CSceneNode as a child
 @return A bool variable
 */
bool CSceneNode::AddChild(CSceneNode* s)
{
	if (s == NULL)
	{
		cout << "CSceneNode::AddChild - No scene node to be added." << endl;
		return false;
	}

	// Store the pointer to the parent
	s->cParentNode = this;
	// Add to vector list
	this->vChildNodes.push_back(s);

	return true;
}

/**
 @brief Delete a child from this node using the pointer to the entity
 @param s A CSceneNode* child node to be deleted from this CSceneNode
 @return A bool variable
 */
bool CSceneNode::DeleteChild(CSceneNode* s)
{
	if (s == NULL)
	{
		cout << "CSceneNode::DeleteChild - No scene node to be deleted." << endl;
		return false;
	}
	if (vChildNodes.size() == 0)
		return false;

	// Search the children for a particular CSceneNode
	std::vector<CSceneNode*>::iterator it;
	for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it)
	{
		if (s->snID == (*it)->snID)
		{
			// If s is this particular child node, then delete the child node.
			delete *it;
			// Then we should proceed to removed this child from our vector of children
			vChildNodes.erase(it);
			// Return true and Stop deleting since we have already found it
			return true;
		}
		else
		{
			// Check if s is one of this node's children using recursion
			if ((*it)->DeleteChild(s) == true)
				// Return true and Stop deleting since we have already found it in one of the child node's children
				return true;
		}
	}

	return false;
}

/**
 @brief Delete a child from this node using its snID
 @param snID The const int ID of a CSceneNode* child node to be deleted from this CSceneNode
 @return A bool variable
 */
bool CSceneNode::DeleteChild(const int snID)
{
	// If there are no children in this node, then return false
	if (vChildNodes.size() == 0)
		return false;

	// Search the children for a particular CSceneNode
	std::vector<CSceneNode*>::iterator it;
	for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it)
	{
		if ((*it)->snID == snID)
		{
			// If s is this particular child node, then delete the child node.
			delete *it;
			// Then we should proceed to removed this child from our vector of children
			vChildNodes.erase(it);
			// Return true and Stop deleting since we have already found it
			return true;
		}
		else
		{
			// Check if s is one of this node's children using recursion
			if ((*it)->DeleteChild(snID) == true)
				// Return true and Stop deleting since we have already found it in one of the child node's children
				return true;
		}
	}

	return false;
}

/**
 @brief Delete all children from this node
 @return A bool variable
 */
bool CSceneNode::DeleteAllChildren(void)
{
	bool bResult = false;

	vector <CSceneNode*>::iterator it = vChildNodes.begin();
	while (it != vChildNodes.end())
	{
		if ((*it)->DeleteAllChildren())
		{
			cout << "Deleting child nodes of CSceneNode snID=" << (*it)->snID << ". ";
		}
		// Delete the child node
		cout << "Deleting CSceneNode snID=" << (*it)->snID << endl;
		delete *it;
		// Then we should proceed to removed this child from our vector of children
		it = vChildNodes.erase(it);
		bResult = true;
	}
	return bResult;
}

/**
 @brief Detach a child from this node using its snID
 @param snID The const int ID of a CSceneNode* child node to be detached from this CSceneNode
 @return A bool variable
 */
CSceneNode* CSceneNode::DetachChild(const int snID)
{
	// Ask the children to feedback how many children they have
	std::vector<CSceneNode*>::iterator it;
	for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it)
	{
		if ((*it)->snID == snID)
		{
			cout << "*** Detaching Node #" << snID << endl;
			CSceneNode* s = (*it);
			s->cParentNode = NULL;
			// Remove this node from the children
			vChildNodes.erase(it);

			return s;
		}
		else
		{
			CSceneNode* s = (*it)->DetachChild(snID);
			if (s != NULL)
				return s;
		}
	}

	return NULL;
}

/**
 @brief Get a child from this node using its snID
 @param snID The const int ID of a CSceneNode* child node
 @return A bool variable
 */
CSceneNode* CSceneNode::GetChild(const int snID)
{
	if (this->snID == snID)
		return this;

	// Ask the children to feedback how many children they have
	std::vector<CSceneNode*>::iterator it;
	for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it)
	{
		if ((*it)->snID == snID)
		{
			return (CSceneNode*)(*it);
		}
		else
		{
			CSceneNode* s = (*it)->GetChild(snID);
			if (s != NULL)
				return s;
		}
	}

	return NULL;
}

/**
 @brief Return the number of children in this group
 @return A int variable
 */
int CSceneNode::GetNumOfChild(void)
{
	// Start with this node's children
	int NumOfChild = vChildNodes.size();

	// Ask the children to feedback how many children they have
	std::vector<CSceneNode*>::iterator it;
	for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it)
	{
		NumOfChild += (*it)->GetNumOfChild();
	}

	return NumOfChild;
}

void CSceneNode::SetWorldTranslateMtxPosition(const glm::vec3 vec3Position) {
	worldTranslateMtx = glm::translate(glm::mat4(1.0f), vec3Position);
}
/**
 @brief Set the world translate transformation matrix
 @param vec3Translate A glm::vec3 variable containing the translate vector
 */
void CSceneNode::SetWorldTranslateMtx(const glm::vec3 vec3Translate)
{
	worldTranslateMtx = glm::translate(worldTranslateMtx, vec3Translate);
}

/**
 @brief Set the world rotation transformation matrix
 @param fAngle A const float variable containing the rotation angle in degrees
 @param vec3RotateAxis A glm::vec3 variable containing the rotation axis
 */
void CSceneNode::SetWorldRotateMtxDirect(const float fAngle, const glm::vec3 vec3RotateAxis)
{
	worldRotateMtx = glm::rotate(glm::mat4(1.f), fAngle, vec3RotateAxis);
}

/**
 @brief Set the world rotation transformation matrix
 @param fAngle A const float variable containing the rotation angle in degrees
 @param vec3RotateAxis A glm::vec3 variable containing the rotation axis
 */
void CSceneNode::SetWorldRotateMtx(const float fAngle, const glm::vec3 vec3RotateAxis)
{
	worldRotateMtx = glm::rotate(worldRotateMtx, fAngle, vec3RotateAxis);
}

/**
 @brief Set the world scale transformation matrix
 @param vec3Scale A glm::vec3 variable containing the scaling values
 */
void CSceneNode::SetWorldScaleMtx(const glm::vec3 vec3Scale)
{
	worldScaleMtx = glm::scale(worldScaleMtx, vec3Scale);
}

/**
 @brief Set the local translate transformation matrix
 @param vec3Translate A glm::vec3 variable containing the translate vector
 */
void CSceneNode::SetLocalTranslateMtx(const glm::vec3 vec3Translate)
{
	localTranslateMtx = glm::translate(localTranslateMtx, vec3Translate);
}

/**
 @brief Set the local rotation transformation matrix
 @param fAngle A const float variable containing the rotation angle in degrees
 @param vec3RotateAxis A glm::vec3 variable containing the rotation axis
 */
void CSceneNode::SetLocalRotateMtx(const float fAngle, const glm::vec3 vec3RotateAxis)
{
	localRotateMtx = glm::rotate(localRotateMtx, fAngle, vec3RotateAxis);
}

/**
 @brief Set the local scale transformation matrix
 @param vec3Scale A glm::vec3 variable containing the scaling values
 */
void CSceneNode::SetLocalScaleMtx(const glm::vec3 vec3Scale)
{
	localScaleMtx = glm::scale(localScaleMtx, vec3Scale);
}

/**
 @brief Set the update transformation for translation for every update
 @param vec3Translate A glm::vec3 variable containing the translate vector
 */
void CSceneNode::SetUpdateTranslateMtx(const glm::vec3 vec3Translate)
{
	updateTranslateMtx = glm::translate(updateTranslateMtx, vec3Translate);
}

/**
 @brief Set the update transformation for rotation for every update
 @param fAngle A const float variable containing the rotation angle in degrees
 @param vec3RotateAxis A glm::vec3 variable containing the rotation axis
 */
void CSceneNode::SetUpdateRotateMtx(const float fAngle, const glm::vec3 vec3RotateAxis)
{
	updateRotateMtx = glm::rotate(updateRotateMtx, fAngle, vec3RotateAxis);
}

/**
 @brief Set the update transformation for scale for every update
 @param vec3Scale A glm::vec3 variable containing the scaling values
 */
void CSceneNode::SetUpdateScaleMtx(const glm::vec3 vec3Scale)
{
	glm::vec3 newVec3Scale = glm::vec3(1.0f + vec3Scale.x, 1.0f + vec3Scale.y, 1.0f + vec3Scale.z);
	updateScaleMtx = glm::scale(updateScaleMtx, newVec3Scale);
}

/**
 @brief Print Self
 */
void CSceneNode::PrintSelf(const int numTabs)
{
	cout << std::string(numTabs, '\t') << "================================" << endl;
	cout << std::string(numTabs, '\t') << "CSceneNode snID=" << snID << endl;

	cout << std::string(numTabs, '\t') << "worldTransformMtx:" << endl;
	cout << std::string(numTabs, '\t') << "[" << worldTransformMtx[0][0] << ", " << worldTransformMtx[1][0] << ", " << worldTransformMtx[2][0] << ", " << worldTransformMtx[3][0] << "]" << endl;
	cout << std::string(numTabs, '\t') << "[" << worldTransformMtx[0][1] << ", " << worldTransformMtx[1][1] << ", " << worldTransformMtx[2][1] << ", " << worldTransformMtx[3][1] << "]" << endl;
	cout << std::string(numTabs, '\t') << "[" << worldTransformMtx[0][2] << ", " << worldTransformMtx[1][2] << ", " << worldTransformMtx[2][2] << ", " << worldTransformMtx[3][2] << "]" << endl;
	cout << std::string(numTabs, '\t') << "[" << worldTransformMtx[0][3] << ", " << worldTransformMtx[1][3] << ", " << worldTransformMtx[2][3] << ", " << worldTransformMtx[3][3] << "]" << endl;

	cout << std::string(numTabs, '\t') << "localTransformMtx:" << endl;
	cout << std::string(numTabs, '\t') << "[" << localTransformMtx[0][0] << ", " << localTransformMtx[1][0] << ", " << localTransformMtx[2][0] << ", " << localTransformMtx[3][0] << "]" << endl;
	cout << std::string(numTabs, '\t') << "[" << localTransformMtx[0][1] << ", " << localTransformMtx[1][1] << ", " << localTransformMtx[2][1] << ", " << localTransformMtx[3][1] << "]" << endl;
	cout << std::string(numTabs, '\t') << "[" << localTransformMtx[0][2] << ", " << localTransformMtx[1][2] << ", " << localTransformMtx[2][2] << ", " << localTransformMtx[3][2] << "]" << endl;
	cout << std::string(numTabs, '\t') << "[" << localTransformMtx[0][3] << ", " << localTransformMtx[1][3] << ", " << localTransformMtx[2][3] << ", " << localTransformMtx[3][3] << "]" << endl;

	cout << std::string(numTabs, '\t') << "Number of Children=" << vChildNodes.size() << endl;
	// Print the children
	std::vector<CSceneNode*>::iterator it;
	int iCounter = 0;
	for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it, iCounter++)
	{
		cout << std::string(numTabs, '\t') << "Children #" << iCounter << endl;
		(*it)->PrintSelf((numTabs + 1));
	}
	cout << std::string(numTabs, '\t') << "================================" << endl;
}