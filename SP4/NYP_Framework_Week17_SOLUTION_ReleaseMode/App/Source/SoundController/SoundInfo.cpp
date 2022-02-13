#include "SoundInfo.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CSoundInfo::CSoundInfo(void)
	: ID(-1)
	, pSoundSource(NULL)
	, bIsLooped(false)
	, eSoundType(_2D)
	, vec3dfSoundPos(vec3df(0.0, 0.0, 0.0))
{
}

/**
 @brief Destructor
 */
CSoundInfo::~CSoundInfo(void)
{
	// Clear the memory before destroying this class instance
	if (pSoundSource != NULL)
	{
		pSoundSource->drop();
		pSoundSource = NULL;
	}
}

/**
 @brief Initialise this class instance
 @param ID A const int variable which will be the ID of the iSoundSource in the map
 @param pSoundSource A ISoundSource* variable which contains the iSoundSource
 @param bIsLooped A const bool variable which indicates if this iSoundSource will have loop playback.
 @param eSoundType A SOUNDTYPE enum variable which states the type of sound
 @param vec3dfSoundPos A vec3df variable which contains the 3D position of the sound
 @return A bool value. true is this class instance was initialised, else false
 */
bool CSoundInfo::Init(	const int ID,
						ISoundSource* pSoundSource,
						const bool bIsLooped,
						SOUNDTYPE eSoundType,
						vec3df vec3dfSoundPos)
{
	this->ID = ID;
	this->pSoundSource = pSoundSource;
	this->bIsLooped = bIsLooped;
	this->eSoundType = eSoundType;
	this->vec3dfSoundPos = vec3dfSoundPos;
	
	return true;
}

/**
 @brief Get an sound from this class
 @return The iSoundSource stored in this class instance
 */
ISoundSource* CSoundInfo::GetSound(void) const
{
	return pSoundSource;
}

/**
 @brief Get loop status
 @return bIsLooped value
 */
bool CSoundInfo::GetLoopStatus(void) const
{
	return bIsLooped;
}

/**
 @brief Get sound type
 @return CSoundInfo::SOUNDTYPE value
 */
CSoundInfo::SOUNDTYPE CSoundInfo::GetSoundType(void) const
{
	return eSoundType;
}

/**
 @brief Increase volume of a ISoundSource
 @return true if successfully decreased volume, else false
 */
bool CSoundInfo::VolumeIncrease(void)
{
	// Get the current volume
	float fCurrentVolume = pSoundSource->getDefaultVolume();

	// Increase the volume by 10%
	pSoundSource->setDefaultVolume(fCurrentVolume + 0.1f);

	// Check if the maximum volume has been reached
	if (fCurrentVolume >= 1.0f)
	{
		pSoundSource->setDefaultVolume(1.0f);
	}

	return true;
}

/**
 @brief Decrease volume of a ISoundSource
 @return true if successfully decreased volume, else false
 */
bool CSoundInfo::VolumeDecrease(void)
{
	// Get the current volume
	float fCurrentVolume = pSoundSource->getDefaultVolume();

	// Decrease the volume by 10%
	pSoundSource->setDefaultVolume(fCurrentVolume - 0.1f);

	// Check if the minimum volume has been reached
	if (fCurrentVolume <= 0.0f)
	{
		pSoundSource->setDefaultVolume(0.0f);
	}

	return true;
}

// For 3D sounds only
/**
 @brief Set Listener position
 @param x A const float variable containing the x-component of a position
 @param y A const float variable containing the y-component of a position
 @param z A const float variable containing the z-component of a position
 */
void CSoundInfo::SetPosition(const float x, const float y, const float z)
{
	vec3dfSoundPos.set(x, y, z);
}

/**
 @brief Get position of the 3D sound
 @return The position of the 3D sound
 */
vec3df CSoundInfo::GetPosition(void) const
{
	return vec3dfSoundPos;
}