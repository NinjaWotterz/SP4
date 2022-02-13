#pragma once
#include "./SceneNode.h"
//#include "../WoodenCrate3D.h"
#include "../Entities/WoodenCrate3D.h"
// Include Player3D
#include "../Entities/Player3D.h"
#include "../Camera.h"

class RobotNode : public CSceneNode {
public:
	RobotNode(void);
	~RobotNode(void) {};
	virtual bool Update(const double dElapsedTime);
	virtual void Render(void);
	//static void CreateCube() {
	//	cShader = new Shader("Shader//Scene3D.vs", "Shader//Scene3D.fs");
	//	cSimpleShader = new Shader("Shader//SimpleShader.vs", "Shader//SimpleShader.fs");
	//}

	//CWoodenCrate3D* createEntity() {
	//	CWoodenCrate3D* tempWooden = new CWoodenCrate3D();
	//	tempWooden->SetShader(cShader);
	//	tempWooden->Init();
	//	//tempWooden->ActivateCollider(cSimpleShader);
	//	return tempWooden;
	//}

	

	static void DeleteCube() {
	/*	delete cShader;
		delete cSimpleShader;*/
	}


	void ProcessMovement(const float deltaTime);

	void SetPlayerHandle(CPlayer3D* playerRef);
	int health;
	int maxHealth;
	void ReduceHealth(int damage);
	bool IsDead();
protected:
	//static Shader* cShader;
	//static Shader* cSimpleShader;
	CSceneNode* body;
	CSceneNode* head;
	CSceneNode* leftArm;
	CSceneNode* rightArm;
	CSceneNode* leftLeg;
	CSceneNode* rightLeg;
	// The handle to the CPlayer3D class instance
	CPlayer3D* cPlayer3D;
	bool changeSide;
	float timeToChange;
	// Enemy Attributes
	glm::vec3 vec3Front;
	glm::vec3 vec3Up;
	glm::vec3 vec3Right;
	glm::vec3 vec3WorldUp;
	// Euler Angles
	float fYaw;
	float fPitch;
	bool isConfused;
	float spinTime;
	float angle;
};