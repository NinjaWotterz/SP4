/**
 CEnemy3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Ghost3D.h"

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
CGhost3D::CGhost3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, cPlayer3D(NULL)
	, cTerrain(NULL)
	,cKeyboardController(NULL)
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
CGhost3D::CGhost3D(	const glm::vec3 vec3Position,
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
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, cPlayer3D(NULL)
	, cTerrain(NULL)
{
	this->vec3Position = vec3Position;
	this->vec3Position.y += +fHeightOffset;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Destructor
 */
CGhost3D::~CGhost3D(void)
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

	if (cPlayer3D)
	{
		// We set it to NULL only since it was declared somewhere else
		cPlayer3D = NULL;
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
bool CGhost3D::Init(void)
{
	srand(time(NULL));
	rMovement = rand() % 4;
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::GHOST);

	//// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/Enemy/ghost.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models/Enemy/enemy.obj" << endl;
		return false;
	}

	CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(ModelVertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	iIndicesSize = index_buffer_data.size();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/ghost.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Enemy_01.tga" << endl;
		return false;
	}
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();
	// Store the handler to the terrain
	cTerrain = CTerrain::GetInstance();

	// Movement Control
	fMovementSpeed = 0.5f;
	iCurrentNumMovement = 0;
	iMaxNumMovement = 100;

	// Detection distance for player
	fDetectionDistance = 6.f;

	// Init cWaypointManager
	cWaypointManager = new CWaypointManager;
	cWaypointManager->Init();

	// Add in some test Waypoints
	float fCheckHeight = cTerrain->GetHeight(0.0f, -30.0f);
	int m_iWayPointID = cWaypointManager->AddWaypoint(glm::vec3(0.0f, fCheckHeight, -30.0f));
	fCheckHeight = cTerrain->GetHeight(20.0f, -20.0f);
	m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(30.0f, fCheckHeight, 0.0f));
	fCheckHeight = cTerrain->GetHeight(-20.0f, -30.0f);
	m_iWayPointID = cWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(-30.0f, fCheckHeight, 0.0f));

	cWaypointManager->PrintSelf();

	// Let the NPC face the nearest waypoint
	vec3Front = glm::normalize((cWaypointManager->GetNearestWaypoint(vec3Position)->GetPosition() - vec3Position));
	UpdateFrontAndYaw();

	return true;
}


/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CGhost3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CGhost3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CGhost3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CGhost3D::AttachCamera(CCamera* cCamera)
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
bool CGhost3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 */
void CGhost3D::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		cPrimaryWeapon = cWeaponInfo;
	else if (iSlot == 1)
		cSecondaryWeapon = cWeaponInfo;
}

/**
 @brief Get Weapon of this class instance
 @return The CWeaponInfo* value
 */
CWeaponInfo* CGhost3D::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
		return cPrimaryWeapon;
	else if (iCurrentWeapon == 1)
		return cSecondaryWeapon;

	return NULL;
}

/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
void CGhost3D::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

/**
 @brief Discharge weapon
 @return A bool value
 */
bool CGhost3D::DischargeWeapon(void) const
{
	if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
	{
		return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
	{
		return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	//return NULL;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CGhost3D::ProcessMovement(const ENEMYMOVEMENT direction, const float deltaTime)
{
	float velocity = fMovementSpeed* deltaTime;
	if (direction == ENEMYMOVEMENT::FORWARD)
		vec3Position += vec3Front * velocity * 2.f;
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
	if (cTerrain->IsInWall(vec3Position)) {
		RollbackPositionXZ();
		cout << "Enemy is in wall" << endl;
	}
	// Constraint the player's position
	Constraint();
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 */
void CGhost3D::ProcessRotate(const float fXOffset)
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
bool CGhost3D::Update(const double dElapsedTime)
{
	// Don't update if this entity is not active
	if (bStatus == false)
	{
		return false;
	}

	// Store the enemy's current position, if rollback is needed.
	StorePositionForRollback();
	//UpdateJumpFall(dElapsedTime * fMovementSpeed);
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
		if (glm::distance(vec3Position, cPlayer3D->GetPosition()) < fDetectionDistance )
		{
			sCurrentFSM = FSM::ATTACK;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Target found: Switching to Attack State" << endl;
		}
		iFSMCounter++;
		break;
	case FSM::PATROL:
		// Check if the destination position has been reached
		fMovementSpeed = 0.5f;
		if (iFSMCounter > iMaxFSMCounter)
		{
			rMovement = rand() % 4;
			sCurrentFSM = FSM::IDLE;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "FSM Counter maxed out: Switching to Idle State" << endl;
		}
		else if (glm::distance(vec3Position, cPlayer3D->GetPosition()) < fDetectionDistance)
		{
			rMovement = rand() % 4;
			sCurrentFSM = FSM::ATTACK;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Target found: Switching to Attack State" << endl;
		}
		else
		{
			UpdateFrontAndYaw();
			// Process the movement
			switch (rMovement)
			{
			case 0:
				vec3Front = glm::vec3(1, 0, 0);
				break;
			case 1:
				vec3Front = glm::vec3(-1, 0, 0);
				break;
			case 2:
				vec3Front = glm::vec3(0, 0, 1);
				break;
			case 3:
				vec3Front = glm::vec3(0, 0, -1);
				break;
			default:
				break;
			}

			if (_DEBUG_FSM == true)
				cout << "Patrolling" << endl;
		}
		ProcessMovement(ENEMYMOVEMENT::FORWARD, (float)dElapsedTime);
		//SetToJump();
		iFSMCounter++;
		break;
	case FSM::ATTACK:
		fMovementSpeed = 3.5f;
		if (glm::distance(vec3Position, cPlayer3D->GetPosition()) < fDetectionDistance)
		{
			vec3Front = glm::normalize((cPlayer3D->GetPosition() - vec3Position));
			UpdateFrontAndYaw();

			// Process the movement
			ProcessMovement(ENEMYMOVEMENT::FORWARD, (float)dElapsedTime);
			//SetToJump();
			if (_DEBUG_FSM == true)
				cout << "Attacking now" << endl;
		}
		else
		{

			// Swtich to patrol mode
			sCurrentFSM = FSM::PATROL;
			//iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Switching to Patrol State" << endl;
		}
		iFSMCounter++;
		break;
	default:
		break;
	}

	// Update the model
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	// Update the weapon's position
	if (cPrimaryWeapon)
	{
		//cPrimaryWeapon->SetPosition(vec3Position + glm::vec3(0.05f, -0.075f, 0.5f));
		cPrimaryWeapon->Update(dElapsedTime);
		glm::mat4 gunModel = model;
		gunModel = glm::rotate(gunModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		gunModel = glm::translate(gunModel, glm::vec3(0.05f, -0.075f, 0.5f));
		cPrimaryWeapon->SetModel(gunModel);
	}
	if (cSecondaryWeapon)
	{
		cSecondaryWeapon->SetPosition(vec3Position + glm::vec3(0.05f, -0.075f, 0.5f));
		cSecondaryWeapon->SetRotation(fYaw, glm::vec3(0.0f, 1.0f, 0.0f));
		cSecondaryWeapon->Update(dElapsedTime);
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CGhost3D::PreRender(void)
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
void CGhost3D::Render(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::Render();

	//cPrimaryWeapon->SetView(view);
	//cPrimaryWeapon->SetProjection(projection);
	//cPrimaryWeapon->PreRender();
	//cPrimaryWeapon->Render();
	//cPrimaryWeapon->PostRender();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGhost3D::PostRender(void)
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
void CGhost3D::UpdateEnemyVectors(void)
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
void CGhost3D::Constraint(void)
{
	// Get the new height
	float fNewYValue = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	// Smooth out the change in height
	vec3Position.y += (fNewYValue - vec3Position.y) * 0.05f;
}

/**
 @brief Update Front Vector and Yaw
 */
void CGhost3D::UpdateFrontAndYaw(void)
{
	fYaw = glm::degrees(glm::acos(dot(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front)));
	if (cross(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front).y < 0.0f)
		fYaw *= -1;
}

void CGhost3D::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update(dElapsedTime);
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// If the player is still jumping and the initial velocity has reached zero or below zero, 
		// then it has reach the peak of its jump
		if ((cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP) && (cPhysics3D.GetDeltaDisplacement().y <= 0.0f))
		{
			// Set status to fall
			cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
		}
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update(dElapsedTime);
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// Constaint the player's position to the terrain
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

		// Set the Physics to idle status
		if (fCheckHeight > vec3Position.y)
		{
			vec3Position.y = fCheckHeight;
			cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
		}
	}
}
void CGhost3D::SetToJump(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::JUMP);
		cPhysics3D.SetInitialVelocity(glm::vec3(0.0f, 1.2f, 0.0f));
	}
}
