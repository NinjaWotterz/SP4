/**
 Mesh.h
 By: Lim Chian Song
 Date: Apr 2021
 */
#ifndef MESH_H
#define MESH_H

#include <includes/glm.hpp>

/**
	Vertex Class
	Used for 2D texture.
	Create a version with normals for 3D normal
*/
struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	Vertex()
	{
		position = glm::vec3();
		color = glm::vec4(1.0);
		texCoord = glm::vec2();
	}
};

/**
	Mesh 
	Used for generating mesh for rendering
	Contains the vertex buffer and index buffer
*/
class CMesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};

	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	DRAW_MODE mode;

	// Constructor
	CMesh(void);
	// Destructor
	~CMesh(void);
	virtual void Render();
};

#endif