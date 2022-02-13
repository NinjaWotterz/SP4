#include "LevelOfDetails.h"

/**
 @brief Constructor
*/
CLevelOfDetails::CLevelOfDetails(void)
	: m_bLODStatus(false)
	, eDetailLevel(HIGH_DETAILS)
{
	arrLODDistance[HIGH_DETAILS]	= 0.0f;
	arrLODDistance[MID_DETAILS]		= 15.0f;
	arrLODDistance[LOW_DETAILS]		= 30.0f;
}

/** 
@brief Destructor
*/
CLevelOfDetails::~CLevelOfDetails(void)
{
}

/** 
@brief Get the LOD Status
*/
bool CLevelOfDetails::GetLODStatus(void) const
{
	return m_bLODStatus;
}

/** 
@brief Set the status of the LOD
*/
void CLevelOfDetails::SetLODStatus(const bool m_bLODStatus)
{
	this->m_bLODStatus = m_bLODStatus;

	// If we disable LOD, then let it always return HIGH_DETAILS when you call GetDetailLevel()
	if (this->m_bLODStatus == false)
	{
		eDetailLevel = HIGH_DETAILS;
	}
}

/** 
@brief Get the current detail level
*/
CLevelOfDetails::DETAIL_LEVEL CLevelOfDetails::GetDetailLevel(void) const
{
	return eDetailLevel;
}

/** 
@brief Set the current detail level
*/
bool CLevelOfDetails::SetDetailLevel(const DETAIL_LEVEL eDetailLevel)
{
	if ((eDetailLevel >= HIGH_DETAILS) && (eDetailLevel < NUM_DETAIL_LEVEL))
	{
		this->eDetailLevel = eDetailLevel;
		return true;
	}
	return false;
}
