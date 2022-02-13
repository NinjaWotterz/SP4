/**
 LoadOBJ
 By: Lim Chian Song. Modified by Toh Da Jun
 Date: Sep 2021
 */
#ifndef LOAD_OBJ_H
#define LOAD_OBJ_H

#include <includes/glm.hpp>
#include <vector>

class ModelVertex
{
public:
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;
	ModelVertex(void)
		: pos(glm::vec3(0.0f))
		, normal(glm::vec3(0.0f))
		, texCoord(glm::vec2(0.0f))
	{
	}
};

class CLoadOBJ
{
public:
	// Constructor
	CLoadOBJ(void);
	// Destructor
	~CLoadOBJ(void);

	// Load an OBJ from a file
	static bool LoadOBJ(	const char* file_path,
							std::vector<glm::vec3>& out_vertices,
							std::vector<glm::vec2>& out_uvs,
							std::vector<glm::vec3>& out_normals,
							const bool bFlipTextureForZ = false);
	// Create the IndexVBOs
	static void IndexVBO(	std::vector<glm::vec3>& in_vertices,
							std::vector<glm::vec2>& in_uvs,
							std::vector<glm::vec3>& in_normals,

							std::vector<unsigned>& out_indices,
							std::vector<ModelVertex>& out_vertices);
};



#endif