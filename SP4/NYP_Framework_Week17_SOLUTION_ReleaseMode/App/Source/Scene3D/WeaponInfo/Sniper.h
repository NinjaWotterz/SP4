#pragma once

#include "WeaponInfo.h"

class CSniper : public CWeaponInfo
{
public:
	// Default Constructor
	CSniper(void);
	// Destructor
	virtual ~CSniper(void);

	// Initialise this instance to default values
	bool Init(void);

	virtual bool Discharge(glm::vec3 vec3Position, glm::vec3 vec3Front, CSolidObject* pSource = NULL);
};
