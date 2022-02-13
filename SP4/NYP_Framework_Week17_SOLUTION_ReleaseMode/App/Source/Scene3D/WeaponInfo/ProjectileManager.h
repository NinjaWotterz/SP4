/**
 CProjectileManager
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Projectile
#include "Projectile.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include vector
#include <vector>

class CProjectileManager : public CSingletonTemplate<CProjectileManager>
{
	friend class CSingletonTemplate<CProjectileManager>;

public:
	// Set a shader to this class instance
	void SetShader(const std::string& _name);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Activate a CProjectile* to this class instance
	virtual int Activate(	glm::vec3 vec3Position,
							glm::vec3 vec3Front,
							const double dLifetime,
							const float fSpeed,
							CSolidObject* pSource);
	// Deactivate a CProjectile* from this class instance
	virtual bool Deactivate(const unsigned int iIndex);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// Render this class instance
	virtual void PreRender(void);

	// Render this class instance
	virtual void Render(void);

	// PostRender this class instance
	virtual void PostRender(void);

	// PrintSelf
	virtual void PrintSelf(void);

	// List of CProjectiles
	std::vector<CProjectile*> vProjectile;

protected:
	// Name of Shader Program instance
	std::string sShaderName;

	// Render Settings
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// The total number of elements to reserve for the vector
	unsigned int uiTotalElements;
	// The index of the first active element in the vector to activate
	unsigned int uiIndexFirst;
	// The index of the last active element in the vector to activate
	unsigned int uiIndexLast;

	// Default Constructor
	CProjectileManager(void);
	// Destructor
	virtual ~CProjectileManager(void);
};
