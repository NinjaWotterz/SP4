#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include <string>
#include "SceneNode.h"

class CSceneGraph : public CSingletonTemplate<CSceneGraph>, public CSceneNode
{
	friend class CSingletonTemplate<CSceneGraph>;

public:
	// Initialise this class instance
	bool Init(void);

	// Generate an ID for a Scene Node
	int GenerateID(void);

	// PrintSelf for debug purposes
	virtual void PrintSelf(const int numTabs = 0);

protected:
	// Constructor
	CSceneGraph(void);
	// Destructor
	virtual ~CSceneGraph(void);

	// The next ID to be assigned to a scene node.
	int iNextID;
};
