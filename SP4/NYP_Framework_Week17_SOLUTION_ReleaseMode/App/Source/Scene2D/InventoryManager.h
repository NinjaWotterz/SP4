/**
CInventoryManager: This class manages all the shaders used in this project.
By: Toh Da Jun
Date: Mar 2020
*/
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include <map>
#include <string>
#include "InventoryItem.h"

class CInventoryManager : public CSingletonTemplate<CInventoryManager>
{
	friend CSingletonTemplate<CInventoryManager>;
public:
	void Exit(void);

	// Add a new item
	CInventoryItem* Add(	const std::string& _name,
							const char* imagePath, 
							const int iItemMaxCount, 
							const int iItemCount = 0);
	// Remove an item
	bool Remove(const std::string& _name);
	// Check if an item exists in inventoryMap
	bool Check(const std::string& _name);

	// Get an item by its name
	CInventoryItem* GetItem(const std::string& _name);
	// Get the number of items
	int GetNumItems(void) const;

protected:
	// Constructor
	CInventoryManager(void);

	// Destructor
	virtual ~CInventoryManager(void);

	// The map containing all the items
	std::map<std::string, CInventoryItem*> inventoryMap;
};
