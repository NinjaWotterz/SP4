/**
CShaderManager: This class manages all the shaders used in this project.
By: Toh Da Jun
Date: Mar 2020
*/
#pragma once

// Include SingletonTemplate
#include "..\DesignPatterns\SingletonTemplate.h"

#include <map>
#include <string>
#include "Shader.h"

class CShaderManager : public CSingletonTemplate<CShaderManager>
{
	friend CSingletonTemplate<CShaderManager>;
public:
	void Exit(void);

	// User Interface
	void Add(const std::string& _name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Remove(const std::string& _name);
	void Use(const std::string& _name);
	bool Check(const std::string& _name);

	// The current active shader
	CShader* activeShader;

protected:
	// Constructor
	CShaderManager(void);

	// Destructor
	virtual ~CShaderManager(void);

	// The map containing all the shaders loaded
	std::map<std::string, CShader*> shaderMap;
};
