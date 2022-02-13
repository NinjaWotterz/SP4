/**
 CFlyingObstacle
 */
#include "FlyingObstacle.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

// Set this to true if printing the debug info
#define _DEBUG_FSM false

/**
 @brief Default Constructor
 */
CFlyingObstacle::CFlyingObstacle(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cTarget(NULL)
	, cTerrain(NULL)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, +fHeightOffset, 0.0f);

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CFlyingObstacle::CFlyingObstacle(const glm::vec3 vec3Position,
	const glm::vec3 vec3Front,
	const float fYaw,
	const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cTarget(NULL)
	, cTerrain(NULL)
{
	this->vec3Position = vec3Position;
	//this->vec3Position.y += +fHeightOffset;
	this->vec3PreviousPosition = vec3Position;
	
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Destructor
 */
CFlyingObstacle::~CFlyingObstacle(void)
{
	if (cWaypointManager)
	{
		// We set it to NULL only since it was declared somewhere else
		cWaypointManager = NULL;
	}

	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}

	if (cTarget)
	{
		// We set it to NULL only since it was declared somewhere else
		cTarget = NULL;
	}

	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}

	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CFlyingObstacle::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::NPC);

	// Initialise the cPlayer3D
	//cPlayer3D = CPlayer3D::GetInstance();

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	mesh = CMeshBuilder::GenerateBox(glm::vec4(1, 1, 1, 1));

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Enemy_01.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Enemy_01.tga" << endl;
		return false;
	}

	// Store the handler to the terrain
	cTerrain = CTerrain::GetInstance();

	// Movement Control
	fMovementSpeed = 3.f;
	iCurrentNumMovement = 0;
	iMaxNumMovement = 100;

	health = 1 + rand() % 3;
	// Detection distance for player
	fDetectionDistance = 100.0f;

	// Init cWaypointManager
	cWaypointManager = new CWaypointManager;
	cWaypointManager->Init();

	// Add in some test Waypoints
	int m_iWayPointID = 0;
	float offset = 6.f;
	switch (patrolMv) {
	case PATROLMOVEMENT::PATROLX:
		m_iWayPointID = cWaypointManager->AddWaypoint(glm::vec3(vec3Position.x-offset, vec3Position.y, vec3Position.z));
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(vec3Position.x+offset, vec3Position.y, vec3Position.z));
		break;
	case PATROLMOVEMENT::PATROLZ:
		m_iWayPointID = cWaypointManager->AddWaypoint(glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z - offset));
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z + offset));
		break;
	case PATROLMOVEMENT::PATROLXZ:
		m_iWayPointID = cWaypointManager->AddWaypoint(glm::vec3(vec3Position.x-offset, vec3Position.y, vec3Position.z - offset));
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(vec3Position.x + offset, vec3Position.y, vec3Position.z + offset));
		break;
	case PATROLMOVEMENT::PATROLZX:
		m_iWayPointID = cWaypointManager->AddWaypoint(glm::vec3(vec3Position.x - offset, vec3Position.y, vec3Position.z + offset));
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
		m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(vec3Position.x + offset, vec3Position.y, vec3Position.z - offset));
		break;
	}
	//float fCheckHeight = cTerrain->GetHeight(0.0f, -30.0f);
	/*int m_iWayPointID = cWaypointManager->AddWaypoint(glm::vec3(0.0f, fCheckHeight, -30.0f));
	fCheckHeight = cTerrain->GetHeight(20.0f, -20.0f);
	m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(30.0f, fCheckHeight, 0.0f));
	fCheckHeight = cTerrain->GetHeight(-20.0f, -30.0f);
	m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(-30.0f, fCheckHeight, 0.0f));*/

	cWaypointManager->PrintSelf();

	// Let the NPC face the nearest waypoint
	vec3Front = glm::normalize((cWaypointManager->GetNearestWaypoint(vec3Position)->GetPosition() - vec3Position));
	UpdateFrontAndYaw();

	return true;
}

void CFlyingObstacle::SetTarget(CEntity3D* target) {
	cTarget = target;
}

void CFlyingObstacle::SetPatrolMovement(PATROLMOVEMENT pm) {
	patrolMv = pm;
}

void CFlyingObstacle::ResetHealth() {
	health = 1 + rand() % 3;
}

void CFlyingObstacle::ReduceHealth(int damage) {
	health -= damage;
}
bool CFlyingObstacle::IsDead() {
	return health <= 0;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CFlyingObstacle::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CFlyingObstacle::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CFlyingObstacle::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CFlyingObstacle::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CFlyingObstacle::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}


/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CFlyingObstacle::ProcessMovement(const ENEMYMOVEMENT direction, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime;
	if (direction == ENEMYMOVEMENT::FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == ENEMYMOVEMENT::BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == ENEMYMOVEMENT::LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == ENEMYMOVEMENT::RIGHT)
		vec3Position += vec3Right * velocity;

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
	}

}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 */
void CFlyingObstacle::ProcessRotate(const float fXOffset)
{
	// Update the yaw
	fYaw += fXOffset;// *fRotationSensitivity;

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdateEnemyVectors();
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CFlyingObstacle::Update(const double dElapsedTime)
{
	// Don't update if this entity is not active
	if (bStatus == false)
	{
		return false;
	}

	// Store the enemy's current position, if rollback is needed.
	StorePositionForRollback();
	float targetDistance = 0.f;
	switch (sCurrentFSM)
	{
	case FSM::IDLE:
		if (iFSMCounter > iMaxFSMCounter)
		{
			sCurrentFSM = FSM::PATROL;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Rested: Switching to Patrol State" << endl;
		}
		iFSMCounter++;
		break;
	case FSM::PATROL:
		// Check if the destination position has been reached
		if (cWaypointManager->HasReachedWayPoint(vec3Position))
		{
			vec3Front = glm::normalize((cWaypointManager->GetNextWaypoint()->GetPosition() - vec3Position));
			UpdateFrontAndYaw();

			if (_DEBUG_FSM == true)
				cout << "Reached waypoint: Going to next waypoint" << endl;
		}
		/*else if (iFSMCounter > iMaxFSMCounter)
		{
			sCurrentFSM = FSM::IDLE;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "FSM Counter maxed out: Switching to Idle State" << endl;
		}*/
		/*else if (glm::distance(vec3Position, cTarget->GetPosition()) < fDetectionDistance)
		{
			sCurrentFSM = FSM::ATTACK;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Target found: Switching to Attack State" << endl;
		}*/
		else
		{
			// Process the movement
			ProcessMovement(ENEMYMOVEMENT::FORWARD, (float)dElapsedTime);
			if (_DEBUG_FSM == true)
				cout << "Patrolling" << endl;
		}
		iFSMCounter++;
		break;
	default:
		break;
	}

	// Update the model
	//cout << "Flying Obstacle Position: [" << vec3Position.x << ", " << vec3Position.y << ", " << vec3Position.z << "], ";
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CFlyingObstacle::PreRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CFlyingObstacle::Render(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::Render();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CFlyingObstacle::PostRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PostRender();
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CFlyingObstacle::UpdateEnemyVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front = glm::normalize(front);

	vec3Front = front;
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Right = vec3Right;
		cCamera->vec3Up = vec3Up;
	}
}

/**
 @brief Constraint the player's position
 */
void CFlyingObstacle::Constraint(void)
{
	// Get the new height
	float fNewYValue = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	// Smooth out the change in height
	vec3Position.y += (fNewYValue - vec3Position.y) * 0.05f;
}

/**
 @brief Update Front Vector and Yaw
 */
void CFlyingObstacle::UpdateFrontAndYaw(void)
{
	fYaw = glm::degrees(glm::acos(dot(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front)));
	if (cross(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front).y < 0.0f)
		fYaw *= -1;
}
