/**
 CEntityManager
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include CEntity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include list
#include <list>

class CEntityManager : public CSingletonTemplate<CEntityManager>
{
	friend class CSingletonTemplate<CEntityManager>;

public:
	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Add a CEntity3D* to this class instance
	virtual void Add(CEntity3D* cEntity3D);
	// Remove a CEntity3D* from this class instance
	virtual bool Erase(CEntity3D* cEntity3D);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	void RenderMiniMap(void);
	// Render this class instance
	virtual void Render(void);


protected:
	// Render Settings
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// List of CEntity3D
	std::list<CEntity3D*> lEntity3D;

	
	// Default Constructor
	CEntityManager(void);
	// Destructor
	virtual ~CEntityManager(void);
};
