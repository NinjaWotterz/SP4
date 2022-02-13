/**
 CSoundInfo
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include IrrKlang
#include <includes/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class CSoundInfo
{
public:
	enum SOUNDTYPE
	{
		_2D = 0,
		_3D,
		NUM_SOUNDTYPE
	};

	// Constructor
	CSoundInfo(void);

	// Destructor
	virtual ~CSoundInfo(void);

	// Initialise this class instance
	bool Init(	const int ID, 
				ISoundSource* pSoundSource, 
				const bool bIsLooped = false, 
				SOUNDTYPE eSoundType = _2D,
				vec3df vec3dfSoundPos = vec3df(0.0f, 0.0f, 0.0f));

	// Get an sound from this map
	ISoundSource* GetSound(void) const;

	// Get loop status
	bool GetLoopStatus(void) const;

	// Get sound type
	SOUNDTYPE GetSoundType(void) const;

	// Increase volume of a ISoundSource
	bool VolumeIncrease(void);
	// Decrease volume of a ISoundSource
	bool VolumeDecrease(void);

	// For 3D sounds only
	// Set position
	void SetPosition(const float x, const float y, const float z);

	// Get position
	vec3df GetPosition(void) const;

protected:
	// ID for this sound
	int ID;

	// The handler to the irrklang Sound Engine
	ISoundSource* pSoundSource;

	// Indicate if this sound will be looped during playback
	bool bIsLooped;

	// Sound type
	SOUNDTYPE eSoundType;

	// For 3D sound only: Sound position
	vec3df vec3dfSoundPos;
};

