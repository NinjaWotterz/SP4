/**
 CFSM
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "FSM.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CFSM::CFSM(void)
	: sCurrentFSM(FSM::IDLE)
	, iFSMCounter(0)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CFSM::~CFSM(void)
{
}

/**
 @brief Init Initialise this instance
 @return A bool variable
 */ 
bool CFSM::Init(void)
{
	return true;
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
 */
bool CFSM::Update(const double dElapsedTime)
{
	return true;
}

/**
 @brief PrintSelf
 */
void CFSM::PrintSelf(void) const
{
	cout << "CFSM::PrintSelf()" << endl;
	switch (sCurrentFSM)
	{
	case FSM::IDLE:
		cout << "sCurrentFSM = IDLE." << endl;
		break;
	case FSM::PATROL:
		cout << "sCurrentFSM = PATROL." << endl;
		break;
	case FSM::ATTACK:
		cout << "sCurrentFSM = ATTACK." << endl;
		break;
	default:
		cout << "sCurrentFSM is Undefined." << endl;
		break;
	}
	cout << "iFSMCounter = " << iFSMCounter << "/" << iMaxFSMCounter << endl;
}
