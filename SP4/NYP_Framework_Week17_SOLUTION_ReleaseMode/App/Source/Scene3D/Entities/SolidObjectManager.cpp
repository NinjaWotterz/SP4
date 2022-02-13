/**
 CSolidObjectManager
 By: Toh Da Jun
 Date: Apr 2020
 */

#include "SolidObjectManager.h"

// Include CCollisionManager
#include "Primitives/CollisionManager.h"

// Include CCameraEffectsManager
#include "../CameraEffects/CameraEffectsManager.h"
#include "../Entities/Enemy3D.h"
#include "../SceneGraph/RobotNode.h"
#include "../Entities/Item3D.h"
#include "../Entities/Structure3D.h"
#include "../GUI_Scene3D.h"
#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CSolidObjectManager::CSolidObjectManager(void)
	: model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, cProjectileManager(NULL)
{
}

/**
 @brief Destructor
 */
CSolidObjectManager::~CSolidObjectManager(void)
{
	// We won't delete this since it was created elsewhere
	cProjectileManager = NULL;

	// Remove all CSolidObject
	std::list<CSolidObject*>::iterator it = lSolidObject.begin(), end = lSolidObject.end();
	while (it != end)
	{
		// Delete if done
		delete *it;
		it = lSolidObject.erase(it);
	}
}

/**
 @brief Initialise this class instance
 @return A bool variable
 */
bool CSolidObjectManager::Init(void)
{
	lSolidObject.clear();
	targetsAvailable = 0;
	cProjectileManager = CProjectileManager::GetInstance();

	return true;
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Add a CSolidObject* to this class instance
 @param cSolidObject The CSolidObject* variable to be added to this class instance
 */
void CSolidObjectManager::Add(CSolidObject* cSolidObject)
{
	if (cSolidObject->GetType() == CEntity3D::TYPE::TARGET) {
		targetsAvailable += 1;
	}
	lSolidObject.push_back(cSolidObject);
}

/**
 @brief Remove a CSolidObject* from this class instance
 @param cSolidObject The CSolidObject* variable to be erased from this class instance
 @return A bool variable
 */
bool CSolidObjectManager::Erase(CSolidObject* cSolidObject)
{
	// Find the entity's iterator
	std::list<CSolidObject*>::iterator findIter = std::find(lSolidObject.begin(), lSolidObject.end(), cSolidObject);

	// Delete the entity if found
	if (findIter != lSolidObject.end())
	{
		// Delete the CSolidObject
		//delete *findIter;
		// Go to the next iteration after erasing from the list
		findIter = lSolidObject.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

/**
 @brief Collision Check for a CSolidObject*
 @param cSolidObject The CSolidObject* variable to be checked
 @return A bool variable
 */
bool CSolidObjectManager::CollisionCheck(CSolidObject* cSolidObject)
{
	std::list<CSolidObject*>::iterator it, end;
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// Check for collisions between the 2 entities
		if (CCollisionManager::BoxBoxCollision( cSolidObject->GetPosition() + cSolidObject->boxMin,
												cSolidObject->GetPosition() + cSolidObject->boxMax,
												(*it)->GetPosition() + (*it)->boxMin,
												(*it)->GetPosition() + (*it)->boxMax) == true)
		{
			// Rollback the cSolidObject's position
			cSolidObject->RollbackPosition();
			// Rollback the NPC's position
			(*it)->RollbackPosition();
			if ((*it)->GetType() == CSolidObject::TYPE::NPC)
				cout << "** Collision between this Entity and a NPC ***" << endl;
			else if ((*it)->GetType() == CSolidObject::TYPE::OTHERS)
				cout << "** Collision between this Entity and an OTHERS ***" << endl;
			else if ((*it)->GetType() == CSolidObject::TYPE::STRUCTURE)
				cout << "** Collision between this Entity and a STRUCTURE ***" << endl;
			else if ((*it)->GetType() == CSolidObject::TYPE::PROJECTILE)
				cout << "** Collision between this Entity and a PROJECTILE ***" << endl;
			else if ((*it)->GetType() == CSolidObject::TYPE::TOWER)
				cout << "** Collision between this Entity and a TOWER ***" << endl;
			// Quit this loop since a collision has been found
			break;
		}
	}

	return true;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CSolidObjectManager::Update(const double dElapsedTime)
{
	std::list<CSolidObject*>::iterator it, end;

	// Update all CSolidObject
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		(*it)->Update(dElapsedTime);
	}

	if (requireCleanUp) {
		CleanUp();
		requireCleanUp = false;
	}
	
	return true;
}

/**
 @brief Collision Check for all entities and projectiles
 @return A bool variable
 */
bool CSolidObjectManager::CheckForCollision(void)
{
	bool bResult = false;
	std::list<CSolidObject*>::iterator it, end;
	std::list<CSolidObject*>::iterator it_other;
	// Check for collisions among entities
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// If the entity is not active, then skip it
		if ((*it)->GetStatus() == false)
			continue;
		for (it_other = lSolidObject.begin(); it_other != end; ++it_other)
		{
			// If the entity is not active, then skip it
			if ((*it_other)->GetStatus() == false)
				continue;

			// If the 2 entities to check are the same, then skip this iteration
			if (it_other == it)
				continue;

			// Check for collisions between the 2 entities
			if (CCollisionManager::BoxBoxCollision((*it)->GetPosition() + (*it)->boxMin,
				(*it)->GetPosition() + (*it)->boxMax,
				(*it_other)->GetPosition() + (*it_other)->boxMin,
				(*it_other)->GetPosition() + (*it_other)->boxMax) == true)
			{
				// Check if a movable entity collides with another movable entity
				if (
					((*it)->GetType() == CSolidObject::TYPE::PLAYER)
					&&
					(((*it_other)->GetType() >= CSolidObject::TYPE::NPC) &&
						((*it_other)->GetType() <= CSolidObject::TYPE::OTHERS))
					)
				{
					(*it)->RollbackPosition();
					CPlayer3D* p = dynamic_cast<CPlayer3D*>(*it);
					

					if ((*it_other)->GetType() < CSolidObject::TYPE::BOSS) {
						(*it_other)->RollbackPosition();
						p->HitPlayer((*it_other)->GetPosition());
					}
					else if((*it_other)->GetType() == CSolidObject::TYPE::BOSS){
						p->HitPlayer((*it_other)->GetPosition(), 10);
					}
					
					if ((*it_other)->GetType() >= CSolidObject::TYPE::NPC) {
						bResult = true;
					}
					
					cout << "** Collision between Player and an Entity ***" << endl;
					(*it)->PrintSelf();
					(*it_other)->PrintSelf();
					break;
				}
				else if (
					((*it)->GetType() == CSolidObject::TYPE::PLAYER)
					&&
					(((*it_other)->GetType() == CSolidObject::TYPE::ITEM))
					)
				{
					(*it)->RollbackPosition();
					CPlayer3D* p = dynamic_cast<CPlayer3D*>(*it);
					CItem3D* item = dynamic_cast<CItem3D*>(*it_other);
					p->CollectItem(item);

					(*it_other)->SetStatus(false);
		
					cout << "** Collision between Player and an Item ***" << endl;
					(*it)->PrintSelf();
					(*it_other)->PrintSelf();
					break;
				}
				else if (
						(((*it)->GetType() >= CSolidObject::TYPE::NPC) &&
						((*it)->GetType() <= CSolidObject::TYPE::OTHERS))
						&&
						(((*it_other)->GetType() >= CSolidObject::TYPE::NPC) &&
						((*it_other)->GetType() <= CSolidObject::TYPE::OTHERS))
					)
				{
					if ((*it)->GetType() < CSolidObject::TYPE::BOSS) {
						(*it)->RollbackPosition();
					}
					
					if ((*it_other)->GetType() < CSolidObject::TYPE::BOSS) {
						(*it_other)->RollbackPosition();
					}
					
					cout << "** Collision between 2 Entities ***" << endl;
					(*it)->PrintSelf();
					(*it_other)->PrintSelf();
					break;
				}
				
				// Check if a movable entity collides with a non-movable entity
				if (
						(((*it)->GetType() >= CSolidObject::TYPE::PLAYER) &&
							((*it)->GetType() <= CSolidObject::TYPE::OTHERS))
						&&
						((*it_other)->GetType() == CSolidObject::TYPE::STRUCTURE || (*it_other)->GetType() == CSolidObject::TYPE::TOWER)
					)
				{
					if ((*it)->GetType() < CSolidObject::TYPE::BOSS) {
						(*it)->RollbackPosition();
					}
					//(*it)->RollbackPosition();
					/*if (((*it)->GetType() == CSolidObject::TYPE::PLAYER))
						bResult = true;*/
					cout << "** Collision between Entity and Structure ***" << endl;
					break;
				}
			}
		}
	}

	// Check for collisions between entities and projectiles
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// If the entity is not active, then skip it
		if ((*it)->GetStatus() == false)
			continue;

		for (unsigned int i = 0; i < cProjectileManager->vProjectile.size(); i++)
		{
			// If the entity is not active, then skip it
			if ((cProjectileManager->vProjectile[i])->GetStatus() == false)
				continue;

			// Use ray tracing to check for collisions between the 2 entities
			if (CCollisionManager::RayBoxCollision((*it)->GetPosition() + (*it)->boxMin,
				(*it)->GetPosition() + (*it)->boxMax,
				(cProjectileManager->vProjectile[i])->GetPreviousPosition(),
				(cProjectileManager->vProjectile[i])->GetPosition()))
			{
				if ((*it)->GetType() == CSolidObject::TYPE::PLAYER)
				{
					// If this projectile is fired by the player, then skip it
					if ((cProjectileManager->vProjectile[i])->GetSource() == (*it))
						continue;
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** RayBoxCollision between Player and Projectile ***" << endl;
					bResult = true;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::NPC)
				{
					// If this projectile is fired by the NPC, then skip it
					if ((cProjectileManager->vProjectile[i])->GetSource() == (*it))
						continue;
					CEnemy3D* enemy = dynamic_cast<CEnemy3D*>(*it);
					if (enemy) {
						enemy->ReduceHealth(1);
						if (enemy->IsDead()) {
							(*it)->SetStatus(false);
						}
					}
					
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** RayBoxCollision between NPC and Projectile ***" << endl;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::BOSS)
				{
					// If this projectile is fired by the NPC, then skip it
					if ((cProjectileManager->vProjectile[i])->GetSource() == (*it))
						continue;
					RobotNode* boss = dynamic_cast<RobotNode*>(*it);
					if (boss) {
						boss->ReduceHealth(1);
						if (boss->IsDead()) {
							boss->SetStatus(false);
							CGUI_Scene3D::GetInstance()->BossDead();
						}
					}

					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** RayBoxCollision between NPC and Projectile ***" << endl;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::TOWER)
				{
					if ((cProjectileManager->vProjectile[i])->GetSource() == (*it))
						continue;
					if ((cProjectileManager->vProjectile[i])->GetSource()->GetType() == CSolidObject::TYPE::NPC)
					{
						CStructure3D* tower = dynamic_cast<CStructure3D*>(*it);
						tower->ReduceHealth(1);
					}
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** BoxBoxCollision between NPC and Projectile ***" << endl;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::STRUCTURE)
				{
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** RayBoxCollision between Structure and Projectile ***" << endl;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::TARGET)
				{
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					(*it)->SetToDelete(true);
					targetsAvailable -= 1;
					requireCleanUp = true;
					cout << "** RayBoxCollision between Target and Projectile ***" << endl;
					break;
				}
			}

			// Check for collisions between the 2 entities
			if (CCollisionManager::BoxBoxCollision((*it)->GetPosition() + (*it)->boxMin,
				(*it)->GetPosition() + (*it)->boxMax,
				(cProjectileManager->vProjectile[i])->GetPosition() + (cProjectileManager->vProjectile[i])->boxMin,
				(cProjectileManager->vProjectile[i])->GetPosition() + (cProjectileManager->vProjectile[i])->boxMax) == true)
			{
				if ((*it)->GetType() == CSolidObject::TYPE::PLAYER)
				{
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** BoxBoxCollision between Player and Projectile ***" << endl;
					bResult = true;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::NPC)
				{
					CEnemy3D* enemy = dynamic_cast<CEnemy3D*>(*it);
					if (enemy) {
						enemy->ReduceHealth(1);
						if (enemy->IsDead()) {
							(*it)->SetStatus(false);
						}
					}
					
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** BoxBoxCollision between NPC and Projectile ***" << endl;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::TOWER)
				{
					if ((cProjectileManager->vProjectile[i])->GetSource()->GetType() == CSolidObject::TYPE::NPC)
					{
						CStructure3D* tower = dynamic_cast<CStructure3D*>(*it);
						tower->ReduceHealth(1);
					}

			
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** BoxBoxCollision between NPC and Projectile ***" << endl;
					break;
				}
				else if ((*it)->GetType() == CSolidObject::TYPE::STRUCTURE)
				{
					(cProjectileManager->vProjectile[i])->SetStatus(false);
					cout << "** BoxBoxCollision between Structure and Projectile ***" << endl;
					break;
				}
			}
		}
	}
	
	if (bResult == true)
	{
		CCameraEffectsManager::GetInstance()->Get("BloodScreen")->SetStatus(true);
	}

	return true;
}

/**
 @brief CleanUp all CEntity3Ds which are marked for deletion
 */
void CSolidObjectManager::CleanUp(void)
{
	std::list<CSolidObject*>::iterator it, end;
	it = lSolidObject.begin();
	end = lSolidObject.end();
	while (it != end)
	{
		if ((*it)->IsToDelete())
		{
			// Delete the CSolidObject
			delete *it;
			// Go to the next iteration after erasing from the list
			it = lSolidObject.erase(it);
		}
		else
		{
			// Go to the next iteration
			++it;
		}
	}
}

/**
 @brief Render this class instance
 */
void CSolidObjectManager::Render(void)
{
	// Render all entities
	std::list<CSolidObject*>::iterator it, end;
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		if (!(*it)->GetStatus()) continue;
		(*it)->SetView(view);
		(*it)->SetProjection(projection);
		(*it)->PreRender();
		(*it)->Render();
		(*it)->PostRender();
	}
}


int CSolidObjectManager::GetTargetsLeft(void) {
	return targetsAvailable;
}


// return true if all targets are hit.
bool CSolidObjectManager::CheckAndDestroyTargets(void) {
	bool hitalltargets = true;
	std::list<CSolidObject*>::iterator it, end;
	std::list<CSolidObject*>::iterator it_other;

	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == CEntity3D::TYPE::TARGET && !(*it)->IsToDelete()) {
			hitalltargets = false;
			(*it)->SetToDelete(true);
			requireCleanUp = true;
		}
	}
	targetsAvailable = 0;
	return hitalltargets;
}

// return true if all targets are hit.
bool CSolidObjectManager::CheckTowerAlive(void) {
	bool hitalltargets = true;
	std::list<CSolidObject*>::iterator it, end;
	std::list<CSolidObject*>::iterator it_other;

	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == CEntity3D::TYPE::TOWER) {
			CStructure3D* tower = dynamic_cast<CStructure3D*>(*it);
			if (tower->GetEvent() == CStructure3D::EVENT_TRIGGER::DEFEND_STRUCTURE) {
				towerHealth = tower->GetHealth();
				return !tower->IsDead();
			}
		}
	}
	
	return false;
}


int CSolidObjectManager::GetTowerHealth(void) {
	return towerHealth;
}