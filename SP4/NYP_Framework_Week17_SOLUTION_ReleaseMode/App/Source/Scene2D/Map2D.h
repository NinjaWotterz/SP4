/**
 Map2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include the RapidCSV
#include "System/rapidcsv.h"
// Include map storage
#include <map>

// Include Settings
#include "GameControl\Settings.h"

// Include Entity2D
#include "Primitives/Entity2D.h"

// Include files for AStar
#include <queue>
#include <functional>

// A structure storing information about Map Sizes
struct MapSize {
	unsigned int uiRowSize;
	unsigned int uiColSize;
};

// A structure storing information about a map grid
// It includes data to be used for A* Path Finding
struct Grid {
	unsigned int value;

	Grid() 
		: value(0), pos(0, 0), parent(-1, -1), f(0), g(0), h(0) {}
	Grid(	const glm::i32vec2& pos, unsigned int f) 
		: value(0), pos(pos), parent(-1, 1), f(f), g(0), h(0) {}
	Grid(	const glm::i32vec2& pos, const glm::i32vec2& parent, 
			unsigned int f, unsigned int g, unsigned int h) 
		: value(0), pos(pos), parent(parent), f(f), g(g), h(h) {}

	glm::i32vec2 pos;
	glm::i32vec2 parent;
	unsigned int f;
	unsigned int g;
	unsigned int h;
};

using HeuristicFunction = 
	std::function<unsigned int(const glm::i32vec2&, const glm::i32vec2&, int)>;
// Reverse std::priority_queue to get the largest f value on top
inline bool operator< (const Grid& a, const Grid& b) { return b.f < a.f; }

namespace heuristic
{
	unsigned int manhattan(const glm::i32vec2& v1, const glm::i32vec2& v2, int weight);
	unsigned int euclidean(const glm::i32vec2& v1, const glm::i32vec2& v2, int weight);
}

class CMap2D : public CSingletonTemplate<CMap2D>, public CEntity2D
{
	friend CSingletonTemplate<CMap2D>;
public:
	// Init
	bool Init(	const unsigned int uiNumLevels = 1,
				const unsigned int uiNumRows = 24,
				const unsigned int uiNumCols = 32);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	// Set the specifications of the map
	void SetNumTiles(const CSettings::AXIS sAxis, const unsigned int uiValue);
	void SetNumSteps(const CSettings::AXIS sAxis, const unsigned int uiValue);

	// Set the value at certain indices in the arrMapInfo
	void SetMapInfo(const unsigned int uiRow, const unsigned int uiCol, const int iValue, const bool bInvert = true);

	// Get the value at certain indices in the arrMapInfo
	int GetMapInfo(const unsigned int uiRow, const unsigned int uiCol, const bool bInvert = true) const;

	// Load a map
	bool LoadMap(string filename, const unsigned int uiLevel = 0);

	// Save a tilemap
	bool SaveMap(string filename, const unsigned int uiLevel = 0);

	// Find the indices of a certain value in arrMapInfo
	bool FindValue(const int iValue, unsigned int& uirRow, unsigned int& uirCol, const bool bInvert = true);

	// Set current level
	void SetCurrentLevel(unsigned int uiCurLevel);
	// Get current level
	unsigned int GetCurrentLevel(void) const;

	// For AStar PathFinding
	std::vector<glm::i32vec2> PathFind(	const glm::i32vec2& startPos, 
										const glm::i32vec2& targetPos, 
										HeuristicFunction heuristicFunc, 
										const int weight = 1);
	// Set if AStar PathFinding will consider diagonal movements
	void SetDiagonalMovement(const bool bEnable);
	// Print out details about this class instance in the console window
	void PrintSelf(void) const;

protected:
	// The variable containing the rapidcsv::Document
	// We will load the CSV file's content into this Document
	rapidcsv::Document doc;

	// A 3-D array which stores the values of the tile map
	Grid*** arrMapInfo;

	// The current level
	unsigned int uiCurLevel;
	// The number of levels
	unsigned int uiNumLevels;

	// A 1-D array which stores the map sizes for each level
	MapSize* arrMapSizes;

	// Map containing texture IDs
	map<int, int> MapOfTextureIDs;

	//CS: The quadMesh for drawing the tiles
	CMesh* quadMesh;

	// Constructor
	CMap2D(void);

	// Destructor
	virtual ~CMap2D(void);

	// Load a texture
	bool LoadTexture(const char* filename, const int iTextureCode);

	// Render a tile
	void RenderTile(const unsigned int uiRow, const unsigned int uiCol);

	// For A-Star PathFinding
	// Build a path from m_cameFromList after calling PathFind()
	std::vector<glm::i32vec2> BuildPath() const;
	// Check if a grid is valid
	bool isValid(const glm::i32vec2& pos) const;
	// Check if a grid is blocked
	bool isBlocked(	const unsigned int uiRow, 
					const unsigned int uiCol, 
					const bool bInvert = true) const;
	// Convert a position to a 1D position in the array
	int ConvertTo1D(const glm::i32vec2& pos) const;

	// Delete AStar lists
	bool DeleteAStarLists(void);
	// Reset AStar lists
	bool ResetAStarLists(void);

	// Variables for A-Star PathFinding
	int m_weight;
	unsigned int m_nrOfDirections;
	glm::i32vec2 m_startPos;
	glm::i32vec2 m_targetPos;

	// The handle for heuristic functions
	HeuristicFunction m_heuristic;

	// Lists for A-Star PathFinding
	std::priority_queue<Grid> m_openList;
	std::vector<bool> m_closedList;
	std::vector<Grid> m_cameFromList;
	std::vector<glm::i32vec2> m_directions;
};

