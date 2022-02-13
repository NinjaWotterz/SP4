#include "SceneGraph.h"
#include <iostream>

/**
 @brief Constructor
 */
CSceneGraph::CSceneGraph(void)
	: iNextID(0)
{
}

/**
 @brief Destructor
 */
CSceneGraph::~CSceneGraph(void)
{
	//vector <CSceneNode*>::iterator it = vChildNodes.begin();
	//while (it != vChildNodes.end())
	//{
	//	if ((*it)->DeleteAllChildren())
	//	{
	//		std::cout << "Deleted child nodes of CSceneNode ID=" << (*it)->GetID() << ". ";
	//	}
	//	else
	//	{
	//		std::cout << "Unable to delete child nodes of CSceneNode ID=" << (*it)->GetID() << ". ";
	//	}

	//	// Delete the child node
	//	std::cout << "Deleting CSceneNode ID=" << (*it)->GetID() << endl;
	//	delete* it;
	//	// Then we should proceed to removed this child from our vector of children
	//	it = vChildNodes.erase(it);
	//}

	//// Clear the vector of child nodes
	//vChildNodes.clear();
}

/**
 @brief Initialise this class instance
 */
bool CSceneGraph::Init(void)
{
	iNextID = 0;
	return true;
}

/**
 @brief Generate an ID for a Scene Node
 */
int CSceneGraph::GenerateID(void)
{
	std::cout << "Generating ID = " << iNextID << endl;
	// Return the ID and increment the value by 1
	return iNextID++;
}

/**
 @brief PrintSelf for debug purposes
 */
void CSceneGraph::PrintSelf(const int numTabs)
{
	//std::cout << std::string(numTabs, '\t') << "================================" << endl;
	//std::cout << std::string(numTabs, '\t') << "CSceneGraph" << endl;

	//std::cout << std::string(numTabs, '\t') << "localTransform:" << endl;
	//std::cout << std::string(numTabs, '\t') << "[" << localTransform[0][0] << ", " << localTransform[1][0] << ", " << localTransform[2][0] << ", " << localTransform[3][0] << "]" << endl;
	//std::cout << std::string(numTabs, '\t') << "[" << localTransform[0][1] << ", " << localTransform[1][1] << ", " << localTransform[2][1] << ", " << localTransform[3][1] << "]" << endl;
	//std::cout << std::string(numTabs, '\t') << "[" << localTransform[0][2] << ", " << localTransform[1][2] << ", " << localTransform[2][2] << ", " << localTransform[3][2] << "]" << endl;
	//std::cout << std::string(numTabs, '\t') << "[" << localTransform[0][3] << ", " << localTransform[1][3] << ", " << localTransform[2][3] << ", " << localTransform[3][3] << "]" << endl;

	//std::cout << std::string(numTabs, '\t') << "worldTransform:" << endl;
	//std::cout << std::string(numTabs, '\t') << "[" << worldTransform[0][0] << ", " << worldTransform[1][0] << ", " << worldTransform[2][0] << ", " << worldTransform[3][0] << "]" << endl;
	//std::cout << std::string(numTabs, '\t') << "[" << worldTransform[0][1] << ", " << worldTransform[1][1] << ", " << worldTransform[2][1] << ", " << worldTransform[3][1] << "]" << endl;
	//std::cout << std::string(numTabs, '\t') << "[" << worldTransform[0][2] << ", " << worldTransform[1][2] << ", " << worldTransform[2][2] << ", " << worldTransform[3][2] << "]" << endl;
	//std::cout << std::string(numTabs, '\t') << "[" << worldTransform[0][3] << ", " << worldTransform[1][3] << ", " << worldTransform[2][3] << ", " << worldTransform[3][3] << "]" << endl;

	//std::cout << std::string(numTabs, '\t') << "Number of Children=" << vChildNodes.size() << endl;
	//// Print the children
	//std::vector<CSceneNode*>::iterator it;
	//int iCounter = 0;
	//for (it = vChildNodes.begin(); it != vChildNodes.end(); ++it, iCounter++)
	//{
	//	std::cout << std::string(numTabs, '\t') << "Children #" << iCounter << endl;
	//	(*it)->PrintSelf((numTabs + 1));
	//}
	//std::cout << std::string(numTabs, '\t') << "================================" << endl;
}