#include "RobotNode.h"
//#include "../SceneGraph/"
#include "../WeaponInfo/ProjectileManager.h"
// Include ShaderManager
#include "RenderControl/ShaderManager.h"
#include "../Entities/SolidObjectManager.h"
#include "../SceneGraph/SceneGraph.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

//Shader* RobotNode::cShader = NULL;
//Shader* RobotNode::cSimpleShader = NULL;

RobotNode::RobotNode(void) :
	vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, isConfused(false)
{

	//ID = CSceneGraph::GetInstance()->GenerateID();
	//cEntity3D = createEntity();
	//colour = glm::vec4(1, 0, 0, 1);
	//SetTranslation(glm::vec3(0, 0, 0));
	//SetScale(glm::vec3(1, 1, 1));
	Init();
	health = 20;
	maxHealth = 20;
	SetShader("Shader3D");
	//SetWorldScaleMtx(glm::vec3(0.2, 0.2, 0.2));
	SetWorldTranslateMtx(glm::vec3(5.81f, CTerrain::GetInstance()->GetHeight(5.81f, 28.97f) + 2.5f, 28.97f));
	//float  crateScale = 0.5 * 2.5 / 5.f;
	/*glm::vec3 scaleBox(5 * crateScale, 18 * crateScale, 5 * crateScale);*/
	glm::vec3 scaleBox(2, 2, 2);
	InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f)* scaleBox, glm::vec3(0.5f, 0.5f, 0.5f)* scaleBox);
	SetPosition(glm::vec3(5.81f, CTerrain::GetInstance()->GetHeight(5.81f, 28.97f) + 2.5f, 28.97f));
	SetPreviousPosition(GetPosition());
	SetLocalTranslateMtx(glm::vec3(0, 0, 0));
	SetType(CEntity3D::TYPE::BOSS);
	bIsDisplayed = true;
	
	//cEntity3D->cCollider->vec4Colour = glm::vec4(1, 0, 0, 1);
	//cEntity3D->cCollider->vec3TopRight = (glm::vec3(0.5f, 0.5f, 0.5f));
	//cEntity3D->cCollider->vec3BottomLeft = (glm::vec3(-0.5f, -0.5f, -0.5f));
	/*float  crateScale = 0.5 * 2.5 / 5.f;
	cEntity3D->SetColliderScale(glm::vec3(5 * crateScale, 18 * crateScale, 5 * crateScale));
	cEntity3D->SetColour(glm::vec4(1, 0, 0, 1));
	cEntity3D->cCollider->Init();
	cEntity3D->SetPosition(glm::vec3(0, 0, -7));
	isStaticObject = false;*/

	body = new CSceneNode(CSceneGraph::GetInstance()->GenerateID());
	body->SetShader("Shader3D");
	body->SetWorldTranslateMtx(glm::vec3(0, 3.5, 0));
	body->InitMesh(CMeshBuilder::GenerateBox(glm::vec4(0, 0, 1, 1)));
	body->SetLocalScaleMtx(glm::vec3(4.f, 4.5f, 2.f));
	body->bContinuousUpdate = true;
	body->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
	AddChild(body);


	head = new CSceneNode(CSceneGraph::GetInstance()->GenerateID());
	head->SetShader("Shader3D");
	head->SetWorldTranslateMtx(glm::vec3(0, 6.0, 0));
	head->InitMesh(CMeshBuilder::GenerateBox(glm::vec4(1, 1, 0, 1)));
	head->SetLocalScaleMtx(glm::vec3(2.f, 2.f, 2.f));
	head->bContinuousUpdate = true;
	head->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
	AddChild(head);

	leftArm = new CSceneNode(CSceneGraph::GetInstance()->GenerateID());
	leftArm->SetShader("Shader3D");
	leftArm->SetWorldTranslateMtx(glm::vec3(-3.f, 4.6f, 0.f));
	leftArm->InitMesh(CMeshBuilder::GenerateBoxRobot(glm::vec4(0.7, 0, 0, 1)));
	leftArm->SetLocalScaleMtx(glm::vec3(1.f, 3.f, 1.f));
	leftArm->bContinuousUpdate = true;
	leftArm->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
	AddChild(leftArm);

	rightArm = new CSceneNode(CSceneGraph::GetInstance()->GenerateID());
	rightArm->SetShader("Shader3D");
	rightArm->SetWorldTranslateMtx(glm::vec3(3.f, 4.6f, 0.f));
	rightArm->InitMesh(CMeshBuilder::GenerateBoxRobot(glm::vec4(1.0, 0, 0, 1.0)));
	rightArm->SetLocalScaleMtx(glm::vec3(1.f, 3.f, 1.f));
	rightArm->bContinuousUpdate = true;
	rightArm->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
	AddChild(rightArm);

	leftLeg = new CSceneNode(CSceneGraph::GetInstance()->GenerateID());
	leftLeg->SetShader("Shader3D");
	leftLeg->SetWorldTranslateMtx(glm::vec3(-1.5, 1.0, 0.0));
	leftLeg->InitMesh(CMeshBuilder::GenerateBoxRobot(glm::vec4(0, 1, 0, 1)));
	leftLeg->SetLocalScaleMtx(glm::vec3(1.f, 3.f, 1.f));
	leftLeg->bContinuousUpdate = true;
	leftLeg->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
	AddChild(leftLeg);

	rightLeg = new CSceneNode(CSceneGraph::GetInstance()->GenerateID());
	rightLeg->SetShader("Shader3D");
	rightLeg->SetWorldTranslateMtx(glm::vec3(1.5, 1.0, 0.0));
	rightLeg->InitMesh(CMeshBuilder::GenerateBoxRobot(glm::vec4(0, 1, 0, 1)));
	rightLeg->SetLocalScaleMtx(glm::vec3(1.f, 3.f, 1.f));
	rightLeg->bContinuousUpdate = true;
	rightLeg->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
	AddChild(rightLeg);		timeToChange = 1.f;	changeSide = false;	head->SetUpdateRotateMtx(glm::radians(-1.f), glm::vec3(0, 1, 0));	rightArm->SetUpdateRotateMtx(glm::radians(-1.f), glm::vec3(1, 0, 0));
	leftArm->SetUpdateRotateMtx(glm::radians(1.f), glm::vec3(1, 0, 0));	leftLeg->SetUpdateRotateMtx(glm::radians(-1.f), glm::vec3(1, 0, 0));	rightLeg->SetUpdateRotateMtx(glm::radians(1.f), glm::vec3(1, 0, 0));}void RobotNode::Render(void) {	CSceneNode::Render();}void RobotNode::SetPlayerHandle(CPlayer3D* playerRef) {	cPlayer3D = playerRef;}void RobotNode::ProcessMovement(const float deltaTime)
{
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front = glm::normalize(front);

	if (cPlayer3D)
	{
		// Update the direction of the enemy
		front = glm::normalize(glm::vec3(cPlayer3D->GetPosition() - GetPosition()));
		fYaw = glm::degrees(glm::atan(front.z, front.x));
		fPitch = glm::degrees(glm::asin(front.y));
	}
	front.y = 0.f;
	vec3Front = front;
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	float velocity = 0.5f * deltaTime;
	if (isConfused) {
		velocity = 0.f * deltaTime;
	}
	model = glm::mat4(1.0f);
	//localTranslationTransform = glm::translate(model, cEntity3D->GetPosition() + vec3Front * velocity);
	glm::vec3 newPos = GetPosition() + vec3Front * velocity;
	newPos.y = CTerrain::GetInstance()->GetHeight(newPos.x, newPos.z)+2.f;
	SetPosition(newPos);
	SetWorldTranslateMtxPosition(newPos);
	//SetPosition(GetPosition() + vec3Front * velocity);
	if (isConfused) {
		velocity = 0.f * deltaTime;
		angle += 30;
	}
	else {
		angle = 90;
	}

	SetWorldRotateMtxDirect(-glm::radians(fYaw) + glm::radians(angle), glm::vec3(0, 1, 0));

}void RobotNode::ReduceHealth(int damage) {
	health -= damage;
	isConfused = true;
	spinTime = 0.f;
	cout << "Boss Health:" << health << endl;
}

bool RobotNode::IsDead() {
	return health <= 0;
}
bool RobotNode::Update(const double dElapsedTime) {
	if (isConfused) {
		spinTime += dElapsedTime;		if (spinTime > 2.f) {
			isConfused = false;
			//SetRotationUpdate(glm::vec3(0, 1, 0), 0);
		}
	}

	ProcessMovement(dElapsedTime);

	//SetRotationUpdate(glm::vec3(0, 1, 0), 20*dElapsedTime);

	//head->SetRotationUpdate(glm::vec3(0, 1, 0), -dElapsedTime);
	//rightArm->SetRotationUpdate(glm::vec3(0, 1, 0), dElapsedTime);
	//leftArm->SetRotationUpdate(glm::vec3(0, 1, 0), -dElapsedTime);
	CSceneNode::Update(dElapsedTime);
	timeToChange += dElapsedTime;	if (timeToChange > 2.f) {		timeToChange = 0.f;		changeSide = !changeSide;		if (changeSide) {			leftLeg->SetUpdateRotateMtx(glm::radians(2.f), glm::vec3(1, 0, 0));			rightLeg->SetUpdateRotateMtx(glm::radians(-2.f), glm::vec3(1, 0, 0));			leftArm->SetUpdateRotateMtx(glm::radians(-2.f), glm::vec3(1, 0, 0));			rightArm->SetUpdateRotateMtx(glm::radians(2.f), glm::vec3(1, 0, 0));		}		else {			leftLeg->SetUpdateRotateMtx(glm::radians(-2.f), glm::vec3(1, 0, 0));			rightLeg->SetUpdateRotateMtx(glm::radians(2.f), glm::vec3(1, 0, 0));			leftArm->SetUpdateRotateMtx(glm::radians(2.f), glm::vec3(1, 0, 0));			rightArm->SetUpdateRotateMtx(glm::radians(-2.f), glm::vec3(1, 0, 0));		}	}	return true;}