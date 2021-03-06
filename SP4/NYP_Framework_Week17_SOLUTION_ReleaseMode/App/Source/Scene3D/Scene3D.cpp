/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Scene3D.h"

 // Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include filesystem to read from a file
#include "System\filesystem.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"

// Include CCollisionManager
#include "Primitives/CollisionManager.h"

// Include CPistol
#include "WeaponInfo\Pistol.h"
#include "WeaponInfo\Rifle.h"
#include "WeaponInfo\Sniper.h"

// Include CRock3D
#include "Entities/PatrolEnemy.h"
#include "Entities/Rock3D.h"
#include "Entities/Mossrock.h"
#include "Entities/Target3D.h"
// Include CTreeKabak3D
#include "Entities/TreeKabak3D.h"
#include "Entities/Tree3D.h"

#include "Entities/Walls.h"
#include "Entities/Ceiling.h"

// Include CSpinTower
#include "SceneGraph/SpinTower.h"
#include "SceneGraph/RobotNode.h"
#include "SceneGraph/SceneGraph.h"
// Include CSpinTower
#include "Entities/Hut_Concrete.h"
#include "Entities/Platform3D.h"

// Include Enemies
#include "Entities/Zombie3D.h"
#include "Entities/Statue3D.h"
#include "Entities/Ghost3D.h"

//Include CCameraShake
#include "CameraEffects/CameraShake.h"
#include "CameraEffects/CameraRecoil.h"

#include "../Application.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cFPSCounter(NULL)
	, cGUI_Scene3D(NULL)
	, cSoundController(NULL)
	, cSolidObjectManager(NULL)
	, cPlayer3D(NULL)
	, cCamera(NULL)
	, cSkyBox(NULL)
	, cTerrain(NULL)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the cTerrain
	if (cTerrain)
	{
		cTerrain->Destroy();
		cTerrain = NULL;
	}

	// Destroy the cSkyBox
	if (cSkyBox)
	{
		cSkyBox->Destroy();
		cSkyBox = NULL;
	}

	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}

	// Destroy the cEntityManager
	if (cEntityManager)
	{
		cEntityManager->Destroy();
		cEntityManager = NULL;
	}

	// Destroy the cProjectileManager
	if (cProjectileManager)
	{
		cProjectileManager->Destroy();
		cProjectileManager = NULL;
	}

	// Destroy the cPlayer3D
	if (cPlayer3D)
	{
		cSolidObjectManager->Erase(cPlayer3D);
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}

	// Destroy the cSolidObjectManager
	if (cSolidObjectManager)
	{
		cSolidObjectManager->Destroy();
		cSolidObjectManager = NULL;
	}
	// Destroy the cSoundController
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

	// Destroy the cSoundController
	if (cGUI_Scene3D)
	{
		cGUI_Scene3D->Destroy();
		cGUI_Scene3D = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;

	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */
bool CScene3D::Init(std::string initGameLevel)
{
	gameLevel = initGameLevel;
	cSettings = CSettings::GetInstance();
	CSceneGraph::GetInstance()->Init();
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	minimapZoom = 25.f;
	// Configure the camera
	cCamera = CCamera::GetInstance();
	cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);

	//m_programID = CShaderManager::GetInstance()->Use("Shader3D_Shadow");
	// Load the Environment Entities
	//m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	//m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	//m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	//m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	//m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	//m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	// Load the SkyBox
	cSkyBox = CSkyBox::GetInstance();
	// Set a shader to this class instance of CSkyBox
	cSkyBox->SetShader("Shader3D_SkyBox");
	cSkyBox->Init();

	// Load the Ground
	cTerrain = CTerrain::GetInstance();
	cTerrain->SetShader("Shader3D_Terrain");
	cTerrain->Init(gameLevel);
	// Set the size of the Terrain
	cTerrain->SetRenderSize(100.0f, 15.0f, 100.0f);

	// Load the movable Entities
	// Initialise the CSolidObjectManager
	cSolidObjectManager = CSolidObjectManager::GetInstance();
	cSolidObjectManager->Init();

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();
	//cPlayer3D->SetPosition(glm::vec3(-28.f, cTerrain->GetHeight(-28, 16.1f), 16.1f));
	//5.81f, 28.97f
	cPlayer3D->SetPosition(glm::vec3(6.23f, cTerrain->GetHeight(6.23f, 7.8f), 7.8f));
	cPlayer3D->SetMovementSpeed(5.f);
	cPlayer3D->SetShader("Shader3D");
	cPlayer3D->Init();
	cPlayer3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	cPlayer3D->AttachCamera(cCamera);

	// Add the cPlayer3D to the cSolidObjectManager
	cSolidObjectManager->Add(cPlayer3D);

	// Initialise the projectile manager
	cProjectileManager = CProjectileManager::GetInstance();
	cProjectileManager->Init();
	cProjectileManager->SetShader("Shader3D");

	// Assign a cPistol to the cPlayer3D
	CPistol* cPistol = new CPistol();
	CRifle* cRifle = new CRifle();
	CSniper* cSniper = new CSniper();

	// Set the position, rotation and scale of this weapon
	cPistol->SetPosition(glm::vec3(0.05f, -0.09f, -0.3f));
	cPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));

	cRifle->SetPosition(glm::vec3(0.0105593, -0.11249, -0.395715));
	cRifle->SetRotation(3.24767f, glm::vec3(0.0f, 1.0f, 0.0f));
	cRifle->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));

	cSniper->SetPosition(glm::vec3(0.0105593, -0.11249, -0.395715));
	cSniper->SetRotation(3.24767f, glm::vec3(0.0f, 1.0f, 0.0f));
	cSniper->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	// Initialise the instance
	cPistol->Init();
	cRifle->Init();
	cSniper->Init();

	cPistol->SetShader("Shader3D_Model");
	cRifle->SetShader("Shader3D_Model");
	cSniper->SetShader("Shader3D_Model");

	cPlayer3D->SetWeapon(0, cPistol);
	cPlayer3D->SetWeapon(1, cRifle);
	cPlayer3D->SetWeapon(2, cSniper);

	// Initialise the cEnemy3D
	/*float fCheckHeight = cTerrain->GetHeight(0.0f, -10.0f);*/

	//for (int i = 0; i < 10;) {
	//	CPatrolEnemy3D* cEnemy3D = new CPatrolEnemy3D();
	//	float xPositive = ((rand() % 100) < 50) ? -1 : 1;
	//	float zPositive = ((rand() % 100) < 50) ? -1 : 1;
	//	float eXPos = xPositive * (rand() % 45);
	//	float eZPos = zPositive * (rand() % 45);
	//	float eYPos = cTerrain->GetHeight(eXPos, eZPos)+0.5f;
	//	glm::vec3 eSpawnPos = glm::vec3(eXPos, eYPos, eZPos);
	//	if (cTerrain->IsInWall(eSpawnPos)) {
	//		continue;
	//	}
	//	cEnemy3D->SetPosition(eSpawnPos);
	//	cEnemy3D->SetShader("Shader3D");
	//	cEnemy3D->Init();
	//	
	//	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	//	// Assign a cPistol to the cEnemy3D
	//	CPistol* cEnemyPistol = new CPistol();
	//	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	//	// Initialise the instance
	//	cEnemyPistol->Init();
	//	cEnemyPistol->SetShader("Shader3D_Model");
	//	cEnemy3D->SetWeapon(0, cEnemyPistol);
	//	cEnemy3D->SetStatus(true);

	//	// Add the cEnemy3D to the cSolidObjectManager
	//	cSolidObjectManager->Add(cEnemy3D);
	//	++i;
	//	//enemyList.push_back(cEnemy3D);
	//}

	for (int i = 0; i < 5;) {
		float xPositive = ((rand() % 100) < 50) ? -1 : 1;
		float zPositive = ((rand() % 100) < 50) ? -1 : 1;
		float eXPos = xPositive * (rand() % 45);
		float eZPos = zPositive * (rand() % 45);
		float eYPos = cTerrain->GetHeight(eXPos, eZPos)+0.5f;
		glm::vec3 eSpawnPos = glm::vec3(eXPos, eYPos, eZPos);
		if (cTerrain->IsInWall(eSpawnPos)) {
			continue;
		}
		CItem3D* itemCrystal = new CItem3D(eSpawnPos);
		itemCrystal->SetShader("Shader3D");
		itemCrystal->SetItemType(CItem3D::ITEMTYPE::CRYSTAL);
		itemCrystal->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
		itemCrystal->Init();
		itemCrystal->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		itemCrystal->SetStatus(true);
		cSolidObjectManager->Add(itemCrystal);
		itemList.push_back(itemCrystal);
		++i;
	}

	// Spawning all the obstacles required for platformer tower
	CFlyingObstacle* cFlyingObs = SpawnFlyingObstacle(glm::vec3(-52, 14.6, 26.8), CFlyingObstacle::PATROLMOVEMENT::PATROLXZ);
	cSolidObjectManager->Add(cFlyingObs);
	obstacleList.push_back(cFlyingObs);
	cFlyingObs = SpawnFlyingObstacle(glm::vec3(-59.36, 14.6, 46.5), CFlyingObstacle::PATROLMOVEMENT::PATROLX);
	cSolidObjectManager->Add(cFlyingObs);
	obstacleList.push_back(cFlyingObs);
	cFlyingObs = SpawnFlyingObstacle(glm::vec3(-49.55, 14.6, 60.2), CFlyingObstacle::PATROLMOVEMENT::PATROLZ);
	cSolidObjectManager->Add(cFlyingObs);
	obstacleList.push_back(cFlyingObs);
	cFlyingObs = SpawnFlyingObstacle(glm::vec3(-36.37, 14.6, 63.3), CFlyingObstacle::PATROLMOVEMENT::PATROLXZ);
	cSolidObjectManager->Add(cFlyingObs);
	obstacleList.push_back(cFlyingObs);
	cFlyingObs = SpawnFlyingObstacle(glm::vec3(-28.48, 14.6, 49.3), CFlyingObstacle::PATROLMOVEMENT::PATROLX);
	cSolidObjectManager->Add(cFlyingObs);
	obstacleList.push_back(cFlyingObs);
	cFlyingObs = SpawnFlyingObstacle(glm::vec3(-44.5, 14.6, 34.3), CFlyingObstacle::PATROLMOVEMENT::PATROLZX);
	cSolidObjectManager->Add(cFlyingObs);
	obstacleList.push_back(cFlyingObs);

	SpawnTower();
	// Load the GUI Entities
	// Store the cGUI_Scene3D singleton instance here
	cGUI_Scene3D = CGUI_Scene3D::GetInstance();
	cGUI_Scene3D->Init();

	// Load the non-movable Entities with no collisions
	// Initialise the CEntityManager
	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->Init();

	//Zombie 
	float fCheckHeight = cTerrain->GetHeight(-2.56f, 26.f);
	CZombie3D* cZombie3D1 = new CZombie3D(glm::vec3(-2.56f, fCheckHeight, 26.f));
	cZombie3D1->SetScale(glm::vec3(0.1f));
	cZombie3D1->SetMovementSpeed(4.0f);
	cZombie3D1->SetShader("Shader3D");
	cZombie3D1->Init();
	cZombie3D1->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cZombie3D1);

	fCheckHeight = cTerrain->GetHeight(-21.7f, 37.5f);
	CZombie3D* cZombie3D2 = new CZombie3D(glm::vec3(-21.7f, fCheckHeight, 37.5f));
	cZombie3D2->SetScale(glm::vec3(0.1f));
	cZombie3D2->SetMovementSpeed(4.0f);
	cZombie3D2->SetShader("Shader3D");
	cZombie3D2->Init();
	cZombie3D2->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cZombie3D2);

	fCheckHeight = cTerrain->GetHeight(35.9f, 28.8f);

	CZombie3D* cZombie3D3 = new CZombie3D(glm::vec3(35.9f, fCheckHeight, 28.8f));
	cZombie3D3->SetScale(glm::vec3(0.1f));
	cZombie3D3->SetMovementSpeed(4.0f);
	cZombie3D3->SetShader("Shader3D");
	cZombie3D3->Init();
	cZombie3D3->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cZombie3D3);

	//Ghost
	fCheckHeight = cTerrain->GetHeight(-15.f, 28.7f);
	CGhost3D* cGhost3D1 = new CGhost3D(glm::vec3(-15.f, fCheckHeight, 28.7f));
	cGhost3D1->SetScale(glm::vec3(0.80f));
	cGhost3D1->SetShader("Shader3D");
	cGhost3D1->Init();
	cGhost3D1->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cGhost3D1);

	fCheckHeight = cTerrain->GetHeight(3.5f, 37.7f);
	CGhost3D* cGhost3D2 = new CGhost3D(glm::vec3(3.5f, fCheckHeight, 37.7f));
	cGhost3D2->SetScale(glm::vec3(0.80f));
	cGhost3D2->SetShader("Shader3D");
	cGhost3D2->Init();
	cGhost3D2->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cGhost3D2);

	fCheckHeight = cTerrain->GetHeight(41.7f, -2.2f);
	CGhost3D* cGhost3D3 = new CGhost3D(glm::vec3(41.7f, fCheckHeight, -2.2f));
	cGhost3D3->SetScale(glm::vec3(0.80f));
	cGhost3D3->SetShader("Shader3D");
	cGhost3D3->Init();
	cGhost3D3->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cGhost3D3);


	//Statue
	fCheckHeight = cTerrain->GetHeight(9.4f, 14.4f);
	CStatue3D* cStatue3D1 = new CStatue3D(glm::vec3(9.4f, fCheckHeight, 14.4f));
	cStatue3D1->SetScale(glm::vec3(0.15f));
	cStatue3D1->SetShader("Shader3D");
	cStatue3D1->Init();
	cStatue3D1->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cStatue3D1);

	fCheckHeight = cTerrain->GetHeight(2.67f, 28.8f);
	CStatue3D* cStatue3D2 = new CStatue3D(glm::vec3(2.67f, fCheckHeight, 28.8f));
	cStatue3D2->SetScale(glm::vec3(0.15f));
	cStatue3D2->SetShader("Shader3D");
	cStatue3D2->Init();
	cStatue3D2->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cStatue3D2);

	fCheckHeight = cTerrain->GetHeight(30.9f, 40.9f);
	CStatue3D* cStatue3D3 = new CStatue3D(glm::vec3(30.9f, fCheckHeight, 40.9f));
	cStatue3D3->SetScale(glm::vec3(0.15f));
	cStatue3D3->SetShader("Shader3D");
	cStatue3D3->Init();
	cStatue3D3->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cStatue3D3);

	fCheckHeight = cTerrain->GetHeight(14.8f, -25.8f);
	CStatue3D* cStatue3D4 = new CStatue3D(glm::vec3(14.8f, fCheckHeight, -25.8f));
	cStatue3D4->SetScale(glm::vec3(0.15f));
	cStatue3D4->SetShader("Shader3D");
	cStatue3D4->Init();
	cStatue3D4->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cStatue3D4);

	fCheckHeight = cTerrain->GetHeight(-2.7f, -11.9f);
	CStatue3D* cStatue3D5 = new CStatue3D(glm::vec3(-2.7f, fCheckHeight, -11.9f));
	cStatue3D5->SetScale(glm::vec3(0.15f));
	cStatue3D5->SetShader("Shader3D");
	cStatue3D5->Init();
	cStatue3D5->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cStatue3D5);

	fCheckHeight = cTerrain->GetHeight(-38.28f, -35.6f);
	CStatue3D* cStatue3D6 = new CStatue3D(glm::vec3(-38.28f, fCheckHeight, -35.6f));
	cStatue3D6->SetScale(glm::vec3(0.15f));
	cStatue3D6->SetShader("Shader3D");
	cStatue3D6->Init();
	cStatue3D6->InitCollider("Shader3D_Line");
	// Add the cEnemy3D to the cSolidObjectManager
	cSolidObjectManager->Add(cStatue3D6);

	// Initialise the CRock3D
	//CRock3D* cRock3D = new CRock3D();
	//cRock3D->SetInstancingMode(false);

	//if (cRock3D->IsInstancedRendering() == true)
	//{
	//	cRock3D->SetScale(glm::vec3(5.0f));
	//	cRock3D->SetNumOfInstance(1000);
	//	cRock3D->SetSpreadDistance(100.0f);

	//	cRock3D->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	//}
	//else
	//{
	//	fCheckHeight = cTerrain->GetHeight(2.0f, 2.0f);
	//	cRock3D->SetPosition(glm::vec3(2.0f, fCheckHeight, 2.0f));
	//	cRock3D->SetScale(glm::vec3(0.5f));
	//	cRock3D->SetShader("Shader3DNoColour");			// FOR NORMAL RENDERING
	//}
	//if (cRock3D->Init() == true)
	//{
	//	cEntityManager->Add(cRock3D);
	//}
	//else
	//{
	//	delete cRock3D;
	//}

	// Initialise the CTreeKabak3D
	//CTreeKabak3D* cTreeKabak3D = new CTreeKabak3D(glm::vec3(0.0f, 0.0f, 0.0f));
	//cTreeKabak3D->SetInstancingMode(true);
	//if (cTreeKabak3D->IsInstancedRendering() == true)
	//{
	//	cTreeKabak3D->SetScale(glm::vec3(15.f));
	//	cTreeKabak3D->SetNumOfInstance(1000);
	//	cTreeKabak3D->SetSpreadDistance(50.0f);

	//	cTreeKabak3D->SetShader("Shader3D_3DTree");	// FOR INSTANCED RENDERING
	//}
	//if (cTreeKabak3D->Init() == true)
	//{
	//	cEntityManager->Add(cTreeKabak3D);
	//}
	//else
	//{
	//	delete cTreeKabak3D;
	//}

	//CTreeKabak3D* cTreeKabak3D2 = new CTreeKabak3D(glm::vec3(-28.0f, 0.0f, -20.0f));
	//cTreeKabak3D2->SetInstancingMode(true);
	//if (cTreeKabak3D2->IsInstancedRendering() == true)
	//{
	//	cTreeKabak3D2->SetScale(glm::vec3(15.f));
	//	cTreeKabak3D2->SetNumOfInstance(400);
	//	cTreeKabak3D2->SetSpreadDistance(50.0f);

	//	cTreeKabak3D2->SetShader("Shader3D_3DTree");	// FOR INSTANCED RENDERING
	//}
	//if (cTreeKabak3D2->Init() == true)
	//{
	//	cEntityManager->Add(cTreeKabak3D2);
	//}
	//else
	//{
	//	delete cTreeKabak3D2;
	//}

	//CTreeKabak3D* cTreeKabak3D3 = new CTreeKabak3D(glm::vec3(12.0f, 0.0f, 60.0f));
	//cTreeKabak3D3->SetInstancingMode(true);
	//if (cTreeKabak3D3->IsInstancedRendering() == true)
	//{
	//	cTreeKabak3D3->SetScale(glm::vec3(15.f));
	//	cTreeKabak3D3->SetNumOfInstance(400);
	//	cTreeKabak3D3->SetSpreadDistance(50.0f);

	//	cTreeKabak3D3->SetShader("Shader3D_3DTree");	// FOR INSTANCED RENDERING
	//}
	//if (cTreeKabak3D3->Init() == true)
	//{
	//	cEntityManager->Add(cTreeKabak3D3);
	//}
	//else
	//{
	//	delete cTreeKabak3D3;
	//}

	//CMossrock3D* cMossrock3D = new CMossrock3D(glm::vec3(0.0f, 0.0f, 0.0f));
	//cMossrock3D->SetInstancingMode(true);
	//if (cMossrock3D->IsInstancedRendering() == true)
	//{
	//	cMossrock3D->SetScale(glm::vec3(2.f));
	//	cMossrock3D->SetNumOfInstance(1000);
	//	cMossrock3D->SetSpreadDistance(100.0f);

	//	cMossrock3D->SetShader("Shader3D_3DTree");	// FOR INSTANCED RENDERING
	//}
	//if (cMossrock3D->Init() == true)
	//{
	//	cEntityManager->Add(cMossrock3D);
	//}
	//else
	//{
	//	delete cMossrock3D;
	//}

	CWalls3D* cWalls3D = new CWalls3D(glm::vec3(0.0f, 0.0f, 0.0f));
	cWalls3D->SetInstancingMode(true);
	if (cWalls3D->IsInstancedRendering() == true)
	{
		cWalls3D->SetScale(glm::vec3(1.f));
		cWalls3D->SetNumOfInstance(cTerrain->GetTotalWalls());
		cWalls3D->SetSpreadDistance(100.0f);

		cWalls3D->SetShader("Shader3D_ShadowInstance");	// FOR INSTANCED RENDERING
	}
	if (cWalls3D->Init() == true)
	{
		cEntityManager->Add(cWalls3D);
	}
	else
	{
		delete cWalls3D;
	}

	CCeiling3D* cCeiling3D = new CCeiling3D(glm::vec3(0.0f, 0.0f, 0.0f));
	cCeiling3D->SetInstancingMode(true);
	if (cCeiling3D->IsInstancedRendering() == true)
	{
		cCeiling3D->SetScale(glm::vec3(1.f));
		cCeiling3D->SetNumOfInstance(cTerrain->GetTotalWalls());
		cCeiling3D->SetSpreadDistance(100.0f);

		cCeiling3D->SetShader("Shader3D_ShadowInstance");	// FOR INSTANCED RENDERING
	}
	if (cCeiling3D->Init() == true)
	{
		cEntityManager->Add(cCeiling3D);
	}
	else
	{
		delete cCeiling3D;
	}

	// Initialise a CSpinTower
	//CSpinTower::Create();

	//// Spawn Robot Code
	//RobotNode* robot = new RobotNode();
	//robot->SetPlayerHandle(cPlayer3D);
	//cSolidObjectManager->Add(robot);

	
	// Initialise a CHut_Concrete
	CHut_Concrete* cHut_Concrete = new CHut_Concrete(glm::vec3(3.f, cTerrain->GetHeight(3.f, 12.f), 12.f));
	cHut_Concrete->SetShader("Shader3DNoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	cHut_Concrete->nextLevelState = "Play3DLevel2";
	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHut_Concrete);

	return true;
}

void CScene3D::SpawnTower() {
	float fCheckHeight = cTerrain->GetHeight(16.5f, 4.5f);
	// Position for the Target Tower
	CStructure3D* targetShootingPoint = new CStructure3D(glm::vec3(16.5f, fCheckHeight - 0.5f, 4.5f));
	targetShootingPoint->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	targetShootingPoint->SetShader("Shader3D_Model");
	targetShootingPoint->Init();
	targetShootingPoint->InitCollider("Shader3D_Line", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f) * 2.f, glm::vec3(0.5f, 1.f, 0.5f) * 2.f);

	targetShootingPoint->SetEvent(CStructure3D::EVENT_TRIGGER::TIME_ATTACK);
	cSolidObjectManager->Add(targetShootingPoint);
	cTriggerPointsList.push_back(targetShootingPoint);
	fCheckHeight = cTerrain->GetHeight(-2.58f, -7.05f);
	// Position for the Defender Tower
	/*targetShootingPoint = new CStructure3D(glm::vec3(-2.58f, fCheckHeight - 0.5f, -7.05f));*/
	targetShootingPoint = new CStructure3D(glm::vec3(6.f, fCheckHeight - 0.5f, 27.f));
	targetShootingPoint->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	targetShootingPoint->SetShader("Shader3D_Model");
	targetShootingPoint->Init();
	targetShootingPoint->InitCollider("Shader3D_Line", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f) * 2.f, glm::vec3(0.5f, 1.f, 0.5f) * 2.f);
	targetShootingPoint->SetEvent(CStructure3D::EVENT_TRIGGER::DEFEND_STRUCTURE);
	//cStructure3D->SetScale(glm::vec3(0.5f));

	// Add the cStructure3D to the cSolidObjectManager
	cSolidObjectManager->Add(targetShootingPoint);
	cTriggerPointsList.push_back(targetShootingPoint);



	// X -> -20 to 10, Z -> 20 to 50
	fCheckHeight = cTerrain->GetHeight(-30.3f, 18.2f);
	// Position for the Defender Tower
	targetShootingPoint = new CStructure3D(glm::vec3(-30.3f, fCheckHeight - 0.5f, 18.2f));
	targetShootingPoint->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	targetShootingPoint->SetShader("Shader3D_Model");
	targetShootingPoint->Init();
	targetShootingPoint->InitCollider("Shader3D_Line", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f) * 2.f, glm::vec3(0.5f, 1.f, 0.5f) * 2.f);
	targetShootingPoint->SetEvent(CStructure3D::EVENT_TRIGGER::PLATFORMER);
	//cStructure3D->SetScale(glm::vec3(0.5f));

	// Add the cStructure3D to the cSolidObjectManager
	cSolidObjectManager->Add(targetShootingPoint);
	cTriggerPointsList.push_back(targetShootingPoint);

}

CFlyingObstacle* CScene3D::SpawnFlyingObstacle(glm::vec3 pos, CFlyingObstacle::PATROLMOVEMENT pm) {
	CFlyingObstacle* cFlyingObs = new CFlyingObstacle(pos);
	cFlyingObs->SetShader("Shader3D");
	cFlyingObs->SetPatrolMovement(pm);
	cFlyingObs->ResetHealth();
	cFlyingObs->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
	cFlyingObs->Init();
	cFlyingObs->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f)*0.4f, glm::vec3(0.5f, 0.5f, 0.5f) * 0.4f);
	cFlyingObs->SetStatus(false);
	return cFlyingObs;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CScene3D::Update(const double dElapsedTime)
{
	// Store the current position, if rollback is needed.
	cPlayer3D->StorePositionForRollback();
	static bool playerMoved = false;
	static bool up = false;
	static bool down = false;
	static bool left = false;
	static bool right = false;
	static bool cameraLeft = false;
	static bool cameraRight = false;
	static bool camera180 = false;
	static bool sprint = false;
	float sprintspeed = 2.0f;
	float addonspeed = 1.0f;
	// Get keyboard updates for player3D
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
		CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Stamina");
		if (cInventoryItem->GetCount() > 0)
		{
			addonspeed = sprintspeed;
			sprint = true;
		}
	}
	else if (!CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		sprint = false;
		CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
		CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Stamina");
		cInventoryItem->Add(1);
	}
	if (!down && !left && !right && CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		playerMoved = true;
		up = true;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime * addonspeed);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		if (sprint == true)
		{
			CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
			CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Stamina");
			cInventoryItem->Remove(1);
		}
	}
	else if (!up && !left && !right && CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		playerMoved = true;
		down = true;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime * addonspeed);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		if (sprint == true)
		{
			CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
			CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Stamina");
			cInventoryItem->Remove(1);
		}
	}
	else if (!up && !down && !right && CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		playerMoved = true;
		left = true;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime * (addonspeed / 2));
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		if (sprint == true)
		{
			CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
			CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Stamina");
			cInventoryItem->Remove(1);
		}
	}
	else if (!up && !down && !left && CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		playerMoved = true;
		right = true;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime * (addonspeed / 2));
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
		if (sprint == true)
		{
			CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
			CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Stamina");
			cInventoryItem->Remove(1);
		}
	}
	if (playerMoved == false)
	{
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_Q))
		{
			playerMoved = true;
			cameraLeft = true;
		}
		else if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
		{
			playerMoved = true;
			cameraRight = true;
		}
		else if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
		{
			playerMoved = true;
			camera180 = true;
		}
	}

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_F)) {
		minimapZoom = 60.f;
	}
	else {
		minimapZoom = 25.f;
	}

	/*if (playerMoved) {
		minimapZoom += dElapsedTime * 20;
	}
	else {
		minimapZoom -= dElapsedTime * 20;
	}

	if (minimapZoom > 60.f) {
		minimapZoom = 60.f;
	}
	if (minimapZoom < 45.f) {
		minimapZoom = 45.f;
	}*/

	if (playerMoved == true)
	{
		if (up && !CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_W))
		{
			playerMoved = false;
			up = false;
		}
		else if (down && !CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_S))
		{
			playerMoved = false;
			down = false;
		}
		else if (left && !CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_A))
		{
			playerMoved = false;
			left = false;
		}
		else if (right && !CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_D))
		{
			playerMoved = false;
			right = false;
		}
		else if (cameraLeft && !CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_Q))
		{
			playerMoved = false;
			cameraLeft = false;
			cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::CAMERALEFT, (float)dElapsedTime);
		}
		else if (cameraRight && !CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
		{
			playerMoved = false;
			cameraRight = false;
			cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::CAMERARIGHT, (float)dElapsedTime);
		}
		else if (camera180 && !CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
		{
			playerMoved = false;
			camera180 = false;
			cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::CAMERA180, (float)dElapsedTime);
		}
	}



	float arrowScale = 3.f * minimapZoom / 45.f;
	cPlayer3D->GetArrow()->SetScale(glm::vec3(arrowScale));

	CSolidObject* w = adjustObj3D;
	float adjustModelSpeed = 0.3f;
	bool adjustOn = false;
	if (adjustOn && w) {
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
		{
			w->SetPosition(w->GetPosition() + (glm::vec3(0, 0, 1) * adjustModelSpeed * (float)dElapsedTime));
			/*cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
			((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;*/
		}
		else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
		{
			w->SetPosition(w->GetPosition() + (glm::vec3(0, 0, -1) * adjustModelSpeed * (float)dElapsedTime));
			/*cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
			((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;*/
		}
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
		{
			w->SetPosition(w->GetPosition() + (glm::vec3(-1, 0, 0) * adjustModelSpeed * (float)dElapsedTime));
			//cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
		}
		else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
		{
			w->SetPosition(w->GetPosition() + (glm::vec3(1, 0, 0) * adjustModelSpeed * (float)dElapsedTime));
			//cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
		}
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_U))
		{
			w->SetPosition(w->GetPosition() + (glm::vec3(0, -1, 0) * adjustModelSpeed * (float)dElapsedTime));
			//cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
		}
		else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_O))
		{
			w->SetPosition(w->GetPosition() + (glm::vec3(0, 1, 0) * adjustModelSpeed * (float)dElapsedTime));
			//cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
		}


		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_7))
		{
			w->SetRotation(w->GetRotationAngle() - adjustModelSpeed * (float)dElapsedTime, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_8))
		{
			w->SetRotation(w->GetRotationAngle() + adjustModelSpeed * (float)dElapsedTime, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		//std::cout << "weapon angle: " << w->GetRotationAngle() << std::endl;
		//std::cout << "weapon pos: " << w->GetPosition().x << "," << w->GetPosition().y << "," << w->GetPosition().z << std::endl;
	}

	/*if ((CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))) // Button 0 is the A button on the GamePad
	{
		if (cPlayer3D->GetCrouch() == false)
		{
			cPlayer3D->SetToJump();
			cSoundController->PlaySoundByID(3);
		}
	}*/

	//cTriggerPointsList
	list<CStructure3D*>::iterator it, end;
	CStructure3D* collidedEvent = nullptr;
	end = cTriggerPointsList.end();
	for (it = cTriggerPointsList.begin(); it != end; ++it)
	{
		// cout << glm::distance(cPlayer3D->GetPosition(), (*it)->GetPosition()) << endl;
		// Check for collisions between the 2 entities
		if (glm::distance(cPlayer3D->GetPosition(), (*it)->GetPosition()) < 2.5f)
		{
			collidedEvent = (*it);
		}
	}

	// start mini game
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_C))
	{
		if (!cGUI_Scene3D->isRunnngTargetEvent() && !cGUI_Scene3D->isRunnngMissionEndEvent()) {
			if (collidedEvent != nullptr && !collidedEvent->IsMissionCompleted()) {
				if (collidedEvent->GetEvent() == CStructure3D::EVENT_TRIGGER::TIME_ATTACK) {
					const glm::vec3 playerPos = cPlayer3D->GetPosition(); // position
					const glm::vec3 playerFront = cPlayer3D->GetFront();  // vector

					for (int i = 1; i < 6; ++i) {
						float xDirection = (i % 2 == 0) ? -1 : 1;
						float xPlayerDirection = (playerFront.x > 0) ? 1 : -1;
						glm::vec3 targetPos = playerPos + glm::vec3((xPlayerDirection * xDirection) * i * (10), 10 + (rand() % 30), playerFront.z * i * 20);
						float fCheckHeight = cTerrain->GetHeight(targetPos.x, targetPos.z);
						targetPos.y += fCheckHeight;
						//glm::vec3 targetPos = playerPos + glm::vec3(playerFront.x *i*(rand()%20), 1+(i*0.5f)*(rand() % 30), playerFront.z*(rand() % 100));
						// Spawn the Targets
						Target3D* targetObj = new Target3D(targetPos);
						targetObj->SetPosition(targetPos);
						targetObj->SetShader("Shader3D");
						targetObj->Init();
						targetObj->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
						cSolidObjectManager->Add(targetObj);
					}
					cGUI_Scene3D->StartTargetEvent(30, collidedEvent->GetEvent());
					tower = collidedEvent;
				}
				else if (collidedEvent->GetEvent() == CStructure3D::EVENT_TRIGGER::DEFEND_STRUCTURE) {
					tower = collidedEvent;
					collidedEvent->ResetHealth();
					cGUI_Scene3D->StartTargetEvent(60, collidedEvent->GetEvent());
					spawnTimer = 0.0;
				}
				else if (collidedEvent->GetEvent() == CStructure3D::EVENT_TRIGGER::PLATFORMER) {
					cGUI_Scene3D->StartTargetEvent(200, collidedEvent->GetEvent());
					tower = collidedEvent;
					repawnPosition = cPlayer3D->GetPosition();
					for (auto obs : obstacleList) {
						if (!obs->GetStatus()) {
							obs->SetStatus(true);
						}
					}
					for (auto item : itemList) {
						if (!item->GetStatus() && item->GetItemType() == CItem3D::ITEMTYPE::CRYSTAL) {
							item->SetStatus(true);
						}
					}


				}
			}
		}


		//countdown = 30.f;
	}

	if (cGUI_Scene3D->isRunnngTargetEvent()) {
		spawnTimer += dElapsedTime;
		if (cGUI_Scene3D->GetEventRunning() == CStructure3D::EVENT_TRIGGER::DEFEND_STRUCTURE && spawnTimer > 3.f) {
			spawnTimer = 0.f;
			glm::vec3 spawnPoint = tower->GetPosition();
			float xPositive = ((rand() % 100) < 50) ? -1 : 1;
			float zPositive = ((rand() % 100) < 50) ? -1 : 1;
			spawnPoint += glm::vec3(xPositive * (3.f + rand() % 20), 0.f, zPositive * (3.f + rand() % 20));
			float fCheckHeight = cTerrain->GetHeight(0.0f, -10.0f);
			for (auto enemy : enemyList) {
				if (!enemy->GetStatus()) {
					enemy->SetStatus(true);
					enemy->SetTarget(tower);
					enemy->ResetHealth();
					enemy->SetPosition(glm::vec3(spawnPoint.x, fCheckHeight, spawnPoint.z));
					break;
				}
			}

		}
		else if (cGUI_Scene3D->GetEventRunning() == CStructure3D::EVENT_TRIGGER::PLATFORMER) {
			if (cPlayer3D->GetPosition().y <= 5.f) {
				cPlayer3D->SetPosition(repawnPosition);
				CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
				CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Crystal");
				cInventoryItem->Remove(1);
				for (auto item : itemList) {
					if (!item->GetStatus() && item->GetItemType() == CItem3D::ITEMTYPE::CRYSTAL) {
						item->SetStatus(true);
					}
				}

			}
		}
	}

	/*if (cCamera->GetZoomState() == false)
	{
		if (cPlayer3D->IsCameraAttached() == true)
		{
			if ((CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_LEFT_SHIFT)))
			{
				cPlayer3D->ToggleCrouch();
				CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_LEFT_SHIFT);
				//return;
			}
		}
	}*/

	// Get keyboard updates for camera
	/*if (!cPlayer3D->IsCameraAttached())
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::FORWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::BACKWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::LEFT, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::RIGHT, (float)dElapsedTime);

		// Get mouse updates
		cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}
	else
	{
		// Get mouse updates
		cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}*/

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		if (cPlayer3D->IsCameraAttached())
			cPlayer3D->AttachCamera();
		else
			cPlayer3D->AttachCamera(cCamera);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
	}
	if (cCamera->GetZoomState() == false)
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
		{
			cCamera->SetZoomState(false);
			cPlayer3D->SetCurrentWeapon(0);
		}
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
		{
			cCamera->SetZoomState(false);
			cPlayer3D->SetCurrentWeapon(1);
		}
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_3))
		{
			cPlayer3D->SetCurrentWeapon(2);
		}
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_8))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CameraShake")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(!bStatus);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_8);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_9))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CrossHair")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(!bStatus);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_9);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_R))
	{
		cPlayer3D->GetWeapon()->Reload();
	}

	// Get mouse button updates
	if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::LMB))
	{
		if (cPlayer3D->GetCurrentWeapon() == 2)
		{
			if (cCamera->GetZoomState() == true)
			{
				cPlayer3D->DischargeWeapon();
				((CCameraRecoil*)CCameraEffectsManager::GetInstance()->Get("CameraRecoil"))->bToBeUpdated = true;
				cSoundController->PlaySoundByID(2);
			}
		}
		else
		{
			cPlayer3D->DischargeWeapon();
			((CCameraRecoil*)CCameraEffectsManager::GetInstance()->Get("CameraRecoil"))->bToBeUpdated = true;
			cSoundController->PlaySoundByID(2);
		}
	}

	if (cPlayer3D->GetCurrentWeapon() == 2)
	{
		static bool rightClick = false;
		if (cMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::RMB))
		{
			rightClick = true;

		}

		if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::RMB))
		{
			if (rightClick == true) {
				if (cCamera->GetZoomState() == false) {
					// Switch on Scope mode and zoom in
					cCamera->SetZoomState(true);
				}
				else {
					// Switch off Scope mode and zoom out
					cCamera->SetZoomState(false);
				}
				rightClick = false;
			}

		}
	}

	// Update the Solid Objects
	cSolidObjectManager->Update(dElapsedTime);

	// Update the projectiles
	cProjectileManager->Update(dElapsedTime);

	// Check for collisions among Entities and also with Projectiles
	cSolidObjectManager->CheckForCollision();

	// Update the entities
	cEntityManager->Update(dElapsedTime);

	// Call the cGUI_Scene3D's update method
	cGUI_Scene3D->Update(dElapsedTime);

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void CScene3D::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	CShaderManager::GetInstance()->Use("Shader3D_GPass");

	//These matrices should change when light position or direction changes
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj = glm::ortho(-1000, 1000, -1000, 1000, -1000, 2000);
	else
		m_lightDepthProj = glm::perspective(90.f, 1.f, 0.1f, 200.f);
	m_lightDepthView = glm::lookAt(lights[0].position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	RenderWorld();
}

void CScene3D::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetInstance()->GetWindowWidth(), Application::GetInstance()->GetWindowHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CShaderManager::GetInstance()->Use("Shader3D_Shadow");

	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	glm::mat4 perspective = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(cCamera->vec3Position, cCamera->vec3Front, cCamera->vec3Front);

	
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec4 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z, 1.f);
		glm::vec4 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		glm::vec4 lightPosition_cameraspace = viewStack.Top() * glm::vec4(lights[0].position, 1.f);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		glm::vec4 spotDirection_cameraspace = viewStack.Top() * glm::vec4(lights[0].spotDirection, 1.f);
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		glm::vec4 lightPosition_cameraspace = viewStack.Top() * glm::vec4(lights[0].position, 1.f);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec4 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z, 1.f);
		glm::vec4 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == Light::LIGHT_SPOT)
	{
		glm::vec4 lightPosition_cameraspace = viewStack.Top() * glm::vec4(lights[1].position, 1.f);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		glm::vec4 spotDirection_cameraspace = viewStack.Top() * glm::vec4(lights[1].spotDirection, 1.f);
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		glm::vec4 lightPosition_cameraspace = viewStack.Top() * glm::vec4(lights[1].position, 1.f);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	//RenderMesh(meshList[GEO_AXES], false);

	//RenderSkyPlane(meshList[GEO_SKYPLANE], Color(0, 1, 1), 100, 5, 5, 10, 10);

	RenderWorld();
}

void CScene3D::RenderWorld(){
	// Part 2: Render the entire scene as per normal
		// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(glm::radians(CCamera::GetInstance()->fZoom),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDepthFunc(GL_LEQUAL);

	// Render the SkyBox
	cSkyBox->SetView(view);
	cSkyBox->SetProjection(projection);
	cSkyBox->PreRender();
	cSkyBox->Render();
	cSkyBox->PostRender();

	// Render the Terrain
	cTerrain->SetView(view);
	cTerrain->SetProjection(projection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the entities
	cEntityManager->SetView(view);
	cEntityManager->SetProjection(projection);
	cEntityManager->Render();

	// Render the solid objects
	cSolidObjectManager->SetView(view);
	cSolidObjectManager->SetProjection(projection);
	cSolidObjectManager->Render();

	// Render the projectiles
	cProjectileManager->SetView(view);
	cProjectileManager->SetProjection(projection);
	cProjectileManager->PreRender();
	cProjectileManager->Render();
	cProjectileManager->PostRender();
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
	// Part 1: Render for the minimap by binding to framebuffer and render to color texture
	//         But the camera is move to top-view of the scene

	// Backup some key settings for the camera and player
	glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
	float storeCameraYaw = cCamera->fYaw;
	float storeCameraPitch = cCamera->fPitch;
	glm::vec3 storeCameraPosition = cCamera->vec3Position;
	// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
	cCamera->fYaw += 180.0f;
	cCamera->fPitch = -90.0f;
	// We store the player's position into the camera as we want the minimap to focus on the player
	cCamera->vec3Position = glm::vec3(storePlayerPosition.x, minimapZoom, storePlayerPosition.z);
	// Recalculate all the camera vectors. 
	// We disable pitch constrains for this specific case as we want the camera to look straight down
	cCamera->ProcessMouseMovement(0, 0, false);
	// Generate the view and projection
	glm::mat4 playerView = cCamera->GetViewMatrix();
	glm::mat4 playerProjection = glm::perspective(glm::radians(45.0f),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);

	// Set the camera parameters back to the previous values
	cCamera->fYaw = storeCameraYaw;
	cCamera->fPitch = storeCameraPitch;
	cCamera->vec3Position = storeCameraPosition;
	cCamera->ProcessMouseMovement(0, 0, true); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)

	// Activate the minimap system
	CMinimap::GetInstance()->Activate();
	// Setup the rendering environment
	CMinimap::GetInstance()->PreRender();
	
	//cPlayer3D->AttachCamera();
		

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	// Render the Terrain
	cTerrain->SetView(playerView);
	cTerrain->SetProjection(playerProjection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the entities
	cEntityManager->SetView(playerView);
	cEntityManager->SetProjection(playerProjection);
	cEntityManager->RenderMiniMap();


	// Render the entities for the minimap
	cSolidObjectManager->SetView(playerView);
	cSolidObjectManager->SetProjection(playerProjection);
	cSolidObjectManager->RenderMiniMap();

	// Minimap Object Renderer
	cPlayer3D->GetArrow()->SetView(playerView);
	cPlayer3D->GetArrow()->SetProjection(playerProjection);
	cPlayer3D->GetArrow()->PreRender();
	cPlayer3D->GetArrow()->Render();
	cPlayer3D->GetArrow()->PostRender();

	// Deactivate the cMinimap so that we can render as per normal
	CMinimap::GetInstance()->Deactivate();




	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(glm::radians(CCamera::GetInstance()->fZoom),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);
	
	RenderPassMain();

	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	cGUI_Scene3D->SetProjection(projection);
	// Call the cGUI_Scene3D's PreRender()
	cGUI_Scene3D->PreRender();
	// Call the cGUI_Scene3D's Render()
	cGUI_Scene3D->Render();
	// Call the cGUI_Scene3D's PostRender()
	cGUI_Scene3D->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}
void CScene3D::DestroyAllObstacles() {
	for (auto obs : obstacleList) {
		obs->SetStatus(false);
	}
}
void CScene3D::DestroyAllCrystals() {
	for (auto it : itemList) {
		if (it->GetItemType() == CItem3D::ITEMTYPE::CRYSTAL) {
			it->SetStatus(false);
		}
	}
}
bool CScene3D::CheckClearPlatformer() {
	CInventoryManager* cInventoryManager = CInventoryManager::GetInstance();
	CInventoryItem* cInventoryItem = cInventoryManager->GetItem("Crystal");
	if (tower) {
		bool withinRange = glm::distance(cPlayer3D->GetPosition(), tower->GetPosition()) < 2.5f;
		int count = cInventoryItem->GetCount();
		if (count > 0 && withinRange) {
			return true;
		}
	}
	
	
	return false;
}

void CScene3D::DestroyAllTowerAttackers() {
	for (auto enemy : enemyList) {
		enemy->SetStatus(false);
	}
}

void CScene3D::SetCurrentMissionToSuccess() {
	if (tower) {
		tower->SetMissionComplete(true);
	}
}

void CScene3D::SpawnBoss() {
	RobotNode* robot = new RobotNode();
	robot->SetPlayerHandle(cPlayer3D);
	cSolidObjectManager->Add(robot);
}