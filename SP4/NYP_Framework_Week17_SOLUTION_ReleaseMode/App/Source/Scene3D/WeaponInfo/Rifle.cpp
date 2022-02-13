#include "Rifle.h"

// Include LoadOBJ
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"
#include "ProjectileManager.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CRifle::CRifle()
{
}

/**
@brief Default Destructor
*/
CRifle::~CRifle()
{
}

/**
@brief Initialise this instance to default values
*/
bool CRifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	iMagRounds = 20;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 20;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 40;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 40;

	// The time between shots
	dTimeBetweenShots = 0.1;
	// The elapsed time (between shots)
	dElapsedTime = dTimeBetweenShots;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	// Call the parent's Init()
	CEntity3D::Init();
	SetCurrentWeapon(1);
	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/Rifle/rifleFPS.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models/Rifle/rifleFPS.obj" << endl;
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
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/Rifle/rifleFPS.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/Rifle/rifleFPS.png" << endl;
		return false;
	}

	iconTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Icons/rifle.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load rifle icon" << endl;
		return false;
	}

	return true;
}

bool CRifle::Discharge(glm::vec3 vec3Position, glm::vec3 vec3Front, CSolidObject* pSource) {
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (iMagRounds > 0)
		{
			// Create a projectile. 
			// Its position is slightly in front of the player to prevent collision
			// Its direction is same as the player.
			// It will last for 2.0 seconds and travel at 20 units per frame
			CProjectileManager::GetInstance()->Activate(vec3Position + vec3Front * 0.75f,
				vec3Front,
				1.f, 20.0f, pSource);

			// Lock the weapon after this discharge
			bFire = false;
			// Reset the dElapsedTime to dTimeBetweenShots for the next shot
			dElapsedTime = dTimeBetweenShots;
			// Reduce the rounds by 1
			iMagRounds--;

			return true;
		}
	}

	//cout << "Unable to discharge weapon." << endl;

	return false;
}