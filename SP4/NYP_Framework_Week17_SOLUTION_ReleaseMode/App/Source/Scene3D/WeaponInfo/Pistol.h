/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CPistol :	public CWeaponInfo
{
public:
	// Default Constructor
	CPistol(void);
	// Destructor
	virtual ~CPistol(void);

	// Initialise this instance to default values
	bool Init(void);
};

