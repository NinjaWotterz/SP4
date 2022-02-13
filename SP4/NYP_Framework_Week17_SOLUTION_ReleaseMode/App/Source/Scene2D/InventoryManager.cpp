#include "InventoryManager.h"
#include <stdexcept>      // std::invalid_argument

/**
@brief Constructor
*/
CInventoryManager::CInventoryManager(void) 
{
}

/**
@brief Destructor
*/
CInventoryManager::~CInventoryManager(void)
{
	// Clear the memory
	Exit();
}

/**
@brief Exit by deleting the items
*/
void CInventoryManager::Exit(void)
{
	// Delete all scenes stored and empty the entire map
	//inventoryMap->Release();
	//inventoryMap = nullptr;
	std::map<std::string, CInventoryItem*>::iterator it, end;
	end = inventoryMap.end();
	for (it = inventoryMap.begin(); it != end; ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
	inventoryMap.clear();
}

/**
@brief Add a Scene to this Inventory Manager
*/
CInventoryItem* CInventoryManager::Add(	const std::string& _name,
								const char* imagePath,
								const int iItemMaxCount,
								const int iItemCount)
{
	if (Check(_name))
	{
		// Item name already exist here, unable to proceed
		throw std::exception("Duplicate item name provided");
		return NULL;
	}

	CInventoryItem* cNewItem = new CInventoryItem(imagePath);
	cNewItem->iItemMaxCount = iItemMaxCount;
	cNewItem->iItemCount = iItemCount;

	// Nothing wrong, add the scene to our map
	inventoryMap[_name] = cNewItem;

	return cNewItem;
}

/**
@brief Remove an item from this Inventory Manager
*/
bool CInventoryManager::Remove(const std::string& _name)
{
	// Does nothing if it does not exist
	if (Check(_name))
	{
		// Item is not available, unable to proceed
		throw std::exception("Unknown item name provided");
		return false;
	}

	CInventoryItem* target = inventoryMap[_name];

	// Delete and remove from our map
	delete target;
	inventoryMap.erase(_name);

	return true;
}

/**
@brief Check if a item exists in this Inventory Manager
*/
bool CInventoryManager::Check(const std::string& _name)
{
	return inventoryMap.count(_name) != 0;
}

/**
@brief Get an item by its name
*/ 
CInventoryItem* CInventoryManager::GetItem(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!Check(_name))
		return NULL;

	// Find and return the item
	return inventoryMap[_name];
}

/**
@brief Get the number of items
*/
int CInventoryManager::GetNumItems(void) const
{
	return inventoryMap.size();
}
