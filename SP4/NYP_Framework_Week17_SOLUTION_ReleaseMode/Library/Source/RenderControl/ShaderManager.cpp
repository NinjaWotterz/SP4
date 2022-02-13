#include "ShaderManager.h"
#include <stdexcept>      // std::invalid_argument

/**
@brief Constructor
*/
CShaderManager::CShaderManager(void) 
	: activeShader(nullptr)
{
}

/**
@brief Destructor
*/
CShaderManager::~CShaderManager(void)
{
	// Clear the memory
	Exit();
}

/**
@brief Exit by deleting the shader
*/
void CShaderManager::Exit(void)
{
	// Delete all scenes stored and empty the entire map
	//activeShader->Release();
	//activeShader = nullptr;
	std::map<std::string, CShader*>::iterator it, end;
	end = shaderMap.end();
	for (it = shaderMap.begin(); it != end; ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
	shaderMap.clear();
}

/**
@brief Add a Scene to this Shader Manager
*/
void CShaderManager::Add(	const std::string& _name, 
							const char* vertexPath, 
							const char* fragmentPath, 
							const char* geometryPath)
{
	if (Check(_name))
	{
		// Scene Exist, unable to proceed
		//throw std::exception("Duplicate shader name provided");
		//Change to avoid crashing
		return;
	}

	CShader* cNewShader = new CShader(vertexPath, fragmentPath);

	// Nothing wrong, add the scene to our map
	shaderMap[_name] = cNewShader;
}

/**
@brief Remove a Shader from this Shader Manager
*/
void CShaderManager::Remove(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!Check(_name))
		return;

	CShader* target = shaderMap[_name];
	try {
		if (target == activeShader)
		{
			throw std::exception("Unable to remove active Shader");
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	// Delete and remove from our map
	delete target;
	shaderMap.erase(_name);
}

/**
@brief Use a Shader
*/
void CShaderManager::Use(const std::string& _name)
{
	try {
		if (!Check(_name))
		{
			// Shader does not exist
			throw std::exception("Shader does not exist");
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	// if Shader exist, set the activeShader pointer to that Shader
	activeShader = shaderMap[_name];
	activeShader->use();
}

/**
@brief Check if a Scene exists in this Shader Manager
*/
bool CShaderManager::Check(const std::string& _name)
{
	return shaderMap.count(_name) != 0;
}