/**
 BackgroundEntity
 By: Lim Chian Song
 Date: May 2020
 */
#include "BackgroundEntity.h"

//For allowing creating of Quad Mesh 
#include "Primitives/MeshBuilder.h"

#include <iostream>
#include <vector>

using namespace std;
CBackgroundEntity::CBackgroundEntity(std::string name)
{
	mesh = NULL;

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	textureName = name;
}

CBackgroundEntity::~CBackgroundEntity()
{
	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}
}

bool CBackgroundEntity::Init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (LoadTexture(textureName.c_str()) == false)
	{
		std::cout << "Failed to load ground tile texture" << std::endl;
		return false;
	}

	//CS: Create the Quad Mesh using the mesh builder
	mesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 2, 2);

	return true;
}