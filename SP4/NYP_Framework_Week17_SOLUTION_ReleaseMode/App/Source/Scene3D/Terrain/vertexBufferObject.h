#pragma once

#include <gl/glew.h>

#include <windows.h>
#include <vector>
using namespace std;

/********************************

Class:		CVertexBufferObject

Purpose:	Wraps OpenGL vertex buffer
			object.

********************************/

class CVertexBufferObject
{
public:
	// Constructor
	CVertexBufferObject(void);
	// Destructor
	~CVertexBufferObject(void);

	void CreateVBO(int a_iSize = 0);
	void DeleteVBO(void);

	void* MapBufferToMemory(int iUsageHint);
	void* MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength);
	void UnmapBuffer(void);

	void BindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
	void UploadDataToGPU(int iUsageHint);
	
	void AddData(void* ptrData, unsigned int uiDataSize);

	void* GetDataPointer(void);
	unsigned int GetBufferID(void);

	int GetCurrentSize(void);

protected:
	unsigned int uiBuffer;
	int iSize;
	int iCurrentSize;
	int iBufferType;
	vector<BYTE> data;

	bool bDataUploaded;
};