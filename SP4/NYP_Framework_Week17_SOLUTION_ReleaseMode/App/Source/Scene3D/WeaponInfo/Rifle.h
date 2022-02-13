
#pragma once

#include "WeaponInfo.h"

class CRifle : public CWeaponInfo
{
public:
	// Default Constructor
	CRifle(void);
	// Destructor
	virtual ~CRifle(void);

	// Initialise this instance to default values
	bool Init(void);
	virtual bool Discharge(glm::vec3 vec3Position, glm::vec3 vec3Front, CSolidObject* pSource = NULL);
};
