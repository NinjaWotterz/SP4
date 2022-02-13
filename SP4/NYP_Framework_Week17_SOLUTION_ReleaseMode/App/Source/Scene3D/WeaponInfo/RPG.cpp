#include "RPG.h"
#include "../Projectile/Projectile.h"

CRPG::CRPG()
{
}


CRPG::~CRPG()
{
}

// Initialise this instance to default values
void CRPG::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 2;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 2;
	// The current total number of rounds currently carried by this player
	totalRounds = 2;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 2;

	// The time between shots
	timeBetweenShots = 5.000;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void CRPG::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CProjectile* aProjectile = Create::Projectile("sphere",
				position,
				(target - position).Normalized(),
				5.0f,
				50.0f,
				_source);
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}
