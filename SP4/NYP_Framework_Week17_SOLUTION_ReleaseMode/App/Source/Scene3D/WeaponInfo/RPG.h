#pragma once

#include "WeaponInfo.h"

class CRPG :
	public CWeaponInfo
{
public:
	CRPG();
	virtual ~CRPG();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source);
};

