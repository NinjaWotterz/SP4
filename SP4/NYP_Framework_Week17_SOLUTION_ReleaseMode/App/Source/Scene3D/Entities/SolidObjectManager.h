#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include CSolidObject
#include "SolidObject.h"

// Include CProjectileManager
#include "../WeaponInfo/ProjectileManager.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include list
#include <list>

class CSolidObjectManager : public CSingletonTemplate<CSolidObjectManager>
{
	friend class CSingletonTemplate<CSolidObjectManager>;

public:
	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Add a CSolidObject* to this class instance
	virtual void Add(CSolidObject* cSolidObject);
	// Remove a CSolidObject* from this class instance
	virtual bool Erase(CSolidObject* cSolidObject);

	// Collision Check for a CSolidObject*
	virtual bool CollisionCheck(CSolidObject* cSolidObject);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// Collision Check for all entities and projectiles
	virtual bool CheckForCollision(void);

	// CleanUp all CSolidObjects which are marked for deletion
	virtual void CleanUp(void);

	// Render this class instance
	virtual void Render(void);
	virtual void RenderMiniMap(void);
	virtual bool CheckAndDestroyTargets(void);
	virtual bool CheckTowerAlive(void);
	virtual int GetTargetsLeft(void);
	virtual int GetTowerHealth(void);
protected:
	// Render Settings
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// List of CSolidObject
	std::list<CSolidObject*> lSolidObject;

	// Handler to the CProjectileManager
	CProjectileManager* cProjectileManager;
	int targetsAvailable;
	int towerHealth;
	// Default Constructor
	CSolidObjectManager(void);
	// Destructor
	virtual ~CSolidObjectManager(void);
	bool requireCleanUp;
};
