/**
 CWeaponInfo
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CEntity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CSolidObject
#include "../Entities/SolidObject.h"

// Include Projectile
#include "Projectile.h"

class CWeaponInfo : public CEntity3D
{
public:
	// Default Constructor
	CWeaponInfo(void);
	// Destructor
	virtual ~CWeaponInfo(void);

	enum WeaponType
	{
		PISTOL,
		RIFLE,
		SNIPER
	};

	// Set the number of ammunition in the magazine for this player
	virtual void SetMagRound(const int iMagRounds);
	// Set the maximum number of ammunition in the magazine for this weapon
	virtual void SetMaxMagRound(const int iMagRounds);
	// The current total number of rounds currently carried by this player
	virtual void SetTotalRound(const int iTotalRounds);
	// The max total number of rounds currently carried by this player
	virtual void SetMaxTotalRound(const int iMaxTotalRounds);

	// Get the number of ammunition in the magazine for this player
	virtual int GetMagRound(void) const;
	// Get the maximum number of ammunition in the magazine for this weapon
	virtual int GetMaxMagRound(void) const;
	// Get the current total number of rounds currently carried by this player
	virtual int GetTotalRound(void) const;
	// Get the max total number of rounds currently carried by this player
	virtual int GetMaxTotalRound(void) const;

	// Set the time between shots
	virtual void SetTimeBetweenShots(const double dTimeBetweenShots);
	// Set the firing rate in rounds per min
	virtual void SetFiringRate(const int iFiringRate);
	// Set the firing flag
	virtual void SetCanFire(const bool bFire);

	// Get the time between shots
	virtual double GetTimeBetweenShots(void) const;
	// Get the firing rate
	virtual int GetFiringRate(void) const;
	// Get the firing flag
	virtual bool GetCanFire(void) const;

	// Initialise this instance to default values
	virtual bool Init(void);
	// Update the elapsed time
	virtual bool Update(const double dt);
	// Discharge this weapon
	virtual bool Discharge(glm::vec3 vec3Position, glm::vec3 vec3Front, CSolidObject* pSource = NULL);
	// Reload this weapon
	virtual void Reload(void);
	// Add rounds
	virtual void AddRounds(const int iNewRounds);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Print Self
	virtual void PrintSelf(void);

	virtual string GetWeaponName(void);

	WeaponType GetCurrentWeapon(void);
	void SetCurrentWeapon(int mode);

protected:
	WeaponType equipped;

	// The number of ammunition in a magazine for this weapon
	int iMagRounds;
	// The maximum number of ammunition for this magazine for this weapon
	int iMaxMagRounds;
	// The current total number of rounds currently carried by this player
	int iTotalRounds;
	// The max total number of rounds currently carried by this player
	int iMaxTotalRounds;

	// The time between shots in milliseconds
	double dTimeBetweenShots;
	// The elapsed time (between shots) in milliseconds
	double dElapsedTime;
	// The elapsed time for reloading of a magazine in milliseconds
	double dReloadTime;
	// The maximum elapsed time for reloading of a magazine in milliseconds
	double dMaxReloadTime;
	// Boolean flag to indicate if weapon can fire now
	bool bFire;

	// For Renderings
	unsigned int iIndicesSize;
};
