/**
 CPlayer3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Player3D.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System/ImageLoader.h"

//For allowing creating of Mesh 
#include "Primitives/MeshBuilder.h"
// Include CSoundController
#include "..\..\SoundController\SoundController.h"
#include "..\..\Scene2D\InventoryManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CPlayer3D::CPlayer3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cTerrain(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, cPistol(NULL)
	, cRifle(NULL)
	, cSniper(NULL)
	, cArrow(NULL)
	, iCurrentWeapon(0)
	, fCameraSwayAngle(0.0f)
	, fCameraSwayDeltaAngle(0.25f)
	, bCameraSwayDirection(false)	// false = left, true = right
	, bCameraSwayActive(true)
	, Crouchcheck(false)
{
	// Set the default position so it is above the ground
	vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Constructor with vectors
 @param position A const glm::vec3 variable which contains the position of the camera
 @param up A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CPlayer3D::CPlayer3D(	const glm::vec3 vec3Position,
						const glm::vec3 vec3Front,
						const float fYaw,
						const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cTerrain(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, cPistol(NULL)
	, cRifle(NULL)
	, cSniper(NULL)
	, cArrow(NULL)
	, iCurrentWeapon(0)
	, fCameraSwayAngle(0.0f)
	, fCameraSwayDeltaAngle(0.5f)
	, bCameraSwayDirection(false)	// false = left, true = right
	, bCameraSwayActive(true)
{
	mesh = NULL;

	this->vec3Position = vec3Position;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Destructor
 */
CPlayer3D::~CPlayer3D(void)
{
	if (cSecondaryWeapon)
	{
		delete cSecondaryWeapon;
		cSecondaryWeapon = NULL;
	}

	if (cPrimaryWeapon)
	{
		delete cPrimaryWeapon;
		cPrimaryWeapon = NULL;
	}

	if (cRifle)
	{
		delete cRifle;
		cRifle = NULL;
	}

	if (cPistol)
	{
		delete cPistol;
		cPistol = NULL;
	}

	if (cSniper)
	{
		delete cSniper;
		cSniper = NULL;
	}

	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}
	
	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}

	if (cArrow) {
		delete cArrow;
		cArrow = NULL;
	}
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CPlayer3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::PLAYER);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	mesh = CMeshBuilder::GenerateBox(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f, 1.0f, 1.0f);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Player.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Player.tga" << endl;
		return false;
	}

	// Store the handler to the CTerrain
	cTerrain = CTerrain::GetInstance();
	// Update the y-axis position of the player
	vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z);

	// Set the Physics to fall status by default
	cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);

	if(cArrow == nullptr)
		cArrow = new ArrowMapIndicator();

	// Set the position, rotation and scale of this weapon
	cArrow->SetPosition(glm::vec3(vec3Position.x, vec3Position.y+5.f, vec3Position.z));
	cArrow->SetRotation(0.f, glm::vec3(0.0f, 1.0f, 0.0f));
	cArrow->SetScale(glm::vec3(3.f, 3.f, 3.f));
	cArrow->SetMapIconType(ArrowMapIndicator::MAP_ICON_TYPE::PLAYER);
	cArrow->bIsDisplayed = false;
	cArrow->Init();
	cArrow->SetShader("Shader3D");
	
	health = 3;
	active = true;
	
	return true;
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
@return A glm::mat4 variable which contains the view matrix
*/
glm::mat4 CPlayer3D::GetViewMatrix(void) const
{
	return glm::lookAt(vec3Position, vec3Position + vec3Front, vec3Up);
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CPlayer3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CPlayer3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

void CPlayer3D::HitPlayer(glm::vec3 enemyPos, int damage) {
	if (invulnerableTime <= 0.f) {
		enemyPos.y = vec3Position.y;
		glm::vec3 pushBack = glm::normalize(vec3Position - enemyPos);
		pushBack.y = 3.f;
		pushBack *= 1;
		cPhysics3D.SetStatus(CPhysics3D::STATUS::JUMP);
		cPhysics3D.SetInitialVelocity(pushBack);
		invulnerableTime = 2.f;
		CSoundController* cSoundController = CSoundController::GetInstance();
		cSoundController->PlaySoundByID(2);
		CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
		CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Remove(damage);
		//cInventoryManager
		//cout << "ROBOT HIT PLAYER" << endl;
		//CSoundController::GetInstance()->PlaySoundByID(2);
	}
}

void CPlayer3D::CollectItem(CItem3D* item) {
	if (item->GetItemType() == CItem3D::ITEMTYPE::CRYSTAL) {
		CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
		CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Crystal");
		cInventoryItem->Add(1);
	}
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 */
void CPlayer3D::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		cPistol = cWeaponInfo;
	else if (iSlot == 1)
		cRifle = cWeaponInfo;
	else if (iSlot == 2)
		cSniper = cWeaponInfo;
}

/**
 @brief Get Weapon of this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
CWeaponInfo* CPlayer3D::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
		return cPistol;
	else if (iCurrentWeapon == 1)
		return cRifle;
	else if (iCurrentWeapon == 2)
		return cSniper;

	return NULL;
}

ArrowMapIndicator* CPlayer3D::GetArrow(void) const {
	return cArrow;
}

/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
void CPlayer3D::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

int CPlayer3D::GetCurrentWeapon(void)
{
	return iCurrentWeapon;
}

/**
 @brief Discharge weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @return A bool variable
 */
bool CPlayer3D::DischargeWeapon(void) const
{
	glm::vec3 temp = vec3Position;
	if (Crouchcheck == true)
	{
		temp.y -= 0.2f;
	}

	if ((iCurrentWeapon == 0) && (cPistol))
	{
		return cPistol->Discharge(temp, vec3Front, (CSolidObject*)this);
	}
	else if ((iCurrentWeapon == 1) && (cRifle))
	{
		//vec3Position + vec3Front;
		cRifle->SetCurrentWeapon(1);
		return cRifle->Discharge(temp, vec3Front, (CSolidObject*)this);
	}
	else if ((iCurrentWeapon == 2) && (cSniper))
	{
		//vec3Position + vec3Front;
		cSniper->SetCurrentWeapon(2);
		return cSniper->Discharge(temp, vec3Front, (CSolidObject*)this);
	}
	//return NULL;
}

/**
 @brief Set to Jump
 */
void CPlayer3D::SetToJump(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::JUMP);

		float normalJump = 2.4f;
		if (terrainGradient < 0.1f) {
			normalJump = 0.5f;
		}
		//glm::vec2 jumpDirection = glm::normalize(glm::vec2(cCamera->vec3Front.x, cCamera->vec3Front.z))*2.4f;
		//cPhysics3D.SetInitialVelocity(glm::vec3(jumpDirection.x, 2.4f, jumpDirection.y));
		cPhysics3D.SetInitialVelocity(glm::vec3(0.f, normalJump, 0.f));
	}
}

void CPlayer3D::ToggleCrouch()
{
	if (Crouchcheck == false)
	{
		Crouchcheck = true;
	}
	else
	{
		Crouchcheck = false;
	}
}

bool CPlayer3D::GetCrouch(void)
{
	return Crouchcheck;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::ProcessMovement(const PLAYERMOVEMENT direction, const float deltaTime)
{
	// Indicate that camera sway is to be updated
	if (bCameraSwayActive)
		bUpdateCameraSway = true;

	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		if (terrainGradient <= 0.1) {
			return;
		}
	}

	float velocity = fMovementSpeed * deltaTime;
	glm::vec3 forward = glm::normalize(vec3Front - glm::dot(vec3Front, glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0));
	glm::vec3 right = glm::normalize(vec3Right - glm::dot(vec3Right, glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0));



	if (direction == PLAYERMOVEMENT::FORWARD)
		vec3Position += forward * velocity;
	if (direction == PLAYERMOVEMENT::BACKWARD)
		vec3Position -= forward * velocity;
	if (direction == PLAYERMOVEMENT::LEFT)
		vec3Position -= right * velocity;
	if (direction == PLAYERMOVEMENT::RIGHT)
		vec3Position += right * velocity;

	//float newHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	//if (newHeight >= 11.f) {
	//	RollbackPositionXZ();
	//}
	//cout << "Player is moving" << endl;
	if (cTerrain->IsInWall(vec3Position)) {
		RollbackPositionXZ();
		cout << "Player is in wall" << endl;
	}
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CPlayer3D::ProcessRotate(float fXOffset, float fYOffset, const bool constrainPitch)
{
	fXOffset *= fMouseSensitivity;
	fYOffset *= fMouseSensitivity;

	fYaw += fXOffset;
	fPitch += fYOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (fPitch > 89.0f)
			fPitch = 89.0f;
		if (fPitch < -89.0f)
			fPitch = -89.0f;
	}

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdatePlayerVectors();
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CPlayer3D::Update(const double dElapsedTime)
{
	if (cPistol)
		cPistol->Update(dElapsedTime);
	if (cRifle)
		cRifle->Update(dElapsedTime);
	if (cSniper)
		cSniper->Update(dElapsedTime);

	if (invulnerableTime > 0) {
		invulnerableTime -= dElapsedTime;
	}

	if (Crouchcheck == true)
	{
		vec3Position += glm::vec3(0.f, -0.2f, 0.f);
		vec3Front += glm::vec3(0.f, -0.2f, 0.f);
	}

	UpdatePlayerVectors();

	// Update the Jump/Fall
	UpdateJumpFall(dElapsedTime);

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}

	cArrow->SetPosition(vec3Position);
	cArrow->SetRotation(glm::radians(fYaw-90.f), glm::vec3(0,-1,0));

	// Constraint the player's position
	//Constraint();

	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		terrainGradient = glm::dot(glm::vec3(0, 1, 0), cTerrain->GetNormal(vec3Position.x, vec3Position.z));
		/*cout << "Terrain Normal: " << cTerrain->GetNormal(vec3Position.x, vec3Position.z).x;
		cout << ", " << cTerrain->GetNormal(vec3Position.x, vec3Position.z).y;
		cout << ", " << cTerrain->GetNormal(vec3Position.x, vec3Position.z).z << endl;
		cout << "Terrain Gradient: " << terrainGradient << endl;*/
		if (terrainGradient <= 0.1) {
			glm::vec3 downSlope = cTerrain->GetDownwardSlope(vec3Position.x, vec3Position.z);
			vec3Position = vec3Position + (downSlope * 100.f * (float)dElapsedTime);
			cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
		}
	}

	Constraint();
	// Implement player/camera sway
	if ((bUpdateCameraSway) && (bCameraSwayActive))
	{
		glm::mat4 rotationMat(1); // Creates a identity matrix
		rotationMat = glm::rotate(rotationMat, glm::radians(fCameraSwayAngle), vec3Front);
		vec3Up = glm::vec3(rotationMat * glm::vec4(vec3WorldUp, 1.0f));

		if (bCameraSwayDirection == false)	// Sway to left
		{
			fCameraSwayAngle -= fCameraSwayDeltaAngle;
			if (fCameraSwayAngle < fCameraSwayAngle_LeftLimit)
			{
				fCameraSwayAngle = fCameraSwayAngle_LeftLimit;
				bCameraSwayDirection = !bCameraSwayDirection;
			}
		}
		else if (bCameraSwayDirection == true)	// Sway to right
		{
			fCameraSwayAngle += fCameraSwayDeltaAngle;
			if (fCameraSwayAngle > fCameraSwayAngle_RightLimit)
			{
				fCameraSwayAngle = fCameraSwayAngle_RightLimit;
				bCameraSwayDirection = !bCameraSwayDirection;
			}
		}
		bUpdateCameraSway = false;
	}

	CSolidObject::Update(dElapsedTime);
	cArrow->Update(dElapsedTime);
	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CPlayer3D::PreRender(void)
{
	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CPlayer3D::Render(void)
{
	// Don't render the player if the camera is attached to it
	if (cCamera)
		return;

	CSolidObject::Render();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer3D::PostRender(void)
{
	CSolidObject::PostRender();
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CPlayer3D::UpdatePlayerVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	vec3Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  
	if (!bCameraSwayActive)
		vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Constraint the player's position
 */
void CPlayer3D::Constraint(void)
{
	// If the player is not jumping nor falling, then we snap his position to the terrain
	//cout << "terrain height:" << cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset << endl;
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	}
	else
	{
		// If the player is jumping nor falling, then we only snap his position 
		// if the player's y-coordinate is below the cTerrain 
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
		
		// If the player is below the terrain, then snap to the terrain height
		if (fCheckHeight > vec3Position.y)
			vec3Position.y = fCheckHeight;
	}

	
}

/**
 @brief Update Jump or Fall
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::UpdateJumpFall(const double dElapsedTime)
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

/**
 @brief Print Self
 */
void CPlayer3D::PrintSelf(void)
{
	cout << "CPlayer3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}

int CPlayer3D::GetHP()
{
	return health;
}
void CPlayer3D::SetHP(int dif)
{
	health += dif;
}
bool CPlayer3D::GetActive()
{
	return active;
}
void CPlayer3D::SetActive(bool at)
{
	active = at;
}
