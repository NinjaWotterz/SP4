/**
 CProjectileManager
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "ProjectileManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CProjectileManager::CProjectileManager(void)
	: model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, uiTotalElements(2)
	, uiIndexLast(0)
{
}

/**
 @brief Destructor
 */
CProjectileManager::~CProjectileManager(void)
{
	// Remove all CProjectile*
	for (int i = 0; i < vProjectile.size(); i++)
	{
		delete vProjectile[i];
		vProjectile[i] = NULL;
	}
	vProjectile.clear();
}

/**
 @brief Set a shader to this class instance
 @param _name The name of the Shader instance in the CShaderManager
 */
void CProjectileManager::SetShader(const std::string& _name)
{
	this->sShaderName = _name;
}

/**
 @brief Initialise this class instance
 @return A bool variable
 */
bool CProjectileManager::Init(void)
{
	// Clear the vector
	vProjectile.clear();

	// Reserve the size of the vector
	vProjectile.reserve(uiTotalElements);

	// Create the instances of CProjectile* and store them in the vector
	for (int i = 0; i < uiTotalElements; i++)
	{
		CProjectile* aProjectile = new CProjectile();
		aProjectile->Init(glm::vec3(0.0f), glm::vec3(0.0f), 0.0, 0.0f);
		vProjectile.push_back(new CProjectile());
	}
	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CProjectileManager::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CProjectileManager::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CProjectileManager::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Activate a CProjectile* to this class instance
 @param vec3Position A const glm::vec3 variable containing the source position of the projectile
 @param vec3Front A const glm::vec3 variable containing the direction of the projectile
 @param dLifeTime A const double variable containing the life time of this projectile in milliseconds
 @param fSpeed A const float variable containing the speed of this projectile
 @param pSource A CSolidObject* variable containing the source of this projectile
 @return A int variable
 */
int CProjectileManager::Activate(	glm::vec3 vec3Position, 
									glm::vec3 vec3Front, 
									const double dLifetime,
									const float fSpeed,
									CSolidObject* pSource)
{
	// Since a projectile has been added, we activate the next element in the vector
	vProjectile[uiIndexLast]->Init(vec3Position, vec3Front, dLifetime, fSpeed);
	vProjectile[uiIndexLast]->SetPreviousPosition(vec3Position);
	vProjectile[uiIndexLast]->InitCollider(	"Shader3D_Line", 
											glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
											glm::vec3(-0.05f, -0.05f, -0.05f), 
											glm::vec3(0.05f, 0.05f, 0.05f));
	vProjectile[uiIndexLast]->SetStatus(true);
	vProjectile[uiIndexLast]->SetSource(pSource);
	vProjectile[uiIndexLast]->SetShader(sShaderName);

	// Increase the uiIndexLast by 1 since a projectile is going to be added
	if (uiIndexLast + 1 == vProjectile.size())
		uiIndexLast = 0;
	else
		uiIndexLast++;

	return uiIndexLast;
}

/**
 @brief Deactivate a CProjectile* from this class instance
 @param iIndex A const unsigned int variable containing the index of the projectile to deactivate
 @return A bool variable
 */
bool CProjectileManager::Deactivate(const unsigned int iIndex)
{
	if (iIndex<vProjectile.size())
	{
		// Set the projectile to inactive
		vProjectile[iIndex]->SetStatus(false);
		// Return true
		return true;
	}

	// Return false if not found
	return false;
}

/**
 @brief Update this instance
 @param dElapsedTime A const double containing the elapsed time since the last frame
 */
void CProjectileManager::Update(const double dElapsedTime)
{
	uiIndexFirst = uiIndexLast + 1;
	if (uiIndexFirst >= vProjectile.size())
	{
		uiIndexFirst = 0;

		// CollisionCheck all projectiles against CSolidObject
		// Check the oldest index until the end of 
		for (int i = uiIndexFirst; i <= uiIndexLast; i++)
		{
			// If this projectile is not active, then skip it
			if (vProjectile[i]->GetStatus() == false)
				continue;

			// Update a projectile
			vProjectile[i]->Update(dElapsedTime);
		}
	}
	else
	{
		// CollisionCheck all projectiles against CSolidObject
		// Check the oldest index until the end of 
		for (int i = uiIndexFirst; i < vProjectile.size(); i++)
		{
			// If this projectile is not active, then skip it
			if (vProjectile[i]->GetStatus() == false)
				continue;

			// Update a projectile
			vProjectile[i]->Update(dElapsedTime);
		}

		// CollisionCheck all projectiles against CSolidObject
		for (int i = 0; i <= uiIndexLast; i++)
		{
			// If this projectile is not active, then skip it
			if (vProjectile[i]->GetStatus() == false)
				continue;

			// Update a projectile
			vProjectile[i]->Update(dElapsedTime);
		}
	}
}

/**
 @brief PreRender this class instance
 */
void CProjectileManager::PreRender(void)
{
}

/**
 @brief Render this class instance
 */
void CProjectileManager::Render(void)
{
	// Render all entities
	for (int i = 0; i < vProjectile.size(); i++)
	{
		// If this projectile is not active, then skip it
		if (vProjectile[i]->GetStatus() == false)
			continue;

		vProjectile[i]->SetView(view);
		vProjectile[i]->SetProjection(projection);
		vProjectile[i]->PreRender();
		vProjectile[i]->Render();
		vProjectile[i]->PostRender();
	}
}

/**
 @brief PostRender this class instance
 */
void CProjectileManager::PostRender(void)
{
}

/**
 @brief PrintSelf
 */
void CProjectileManager::PrintSelf(void)
{
	cout << "===============================" << endl;
	cout << "CProjectileManager::PrintSelf()" << endl;
	cout << "-------------------------------" << endl;
	// Render all entities
	for (int i = 0; i < vProjectile.size(); i++)
	{
		cout << i << "\t: ";
		vProjectile[i]->PrintSelf();
	}
	cout << "===============================" << endl;
}
