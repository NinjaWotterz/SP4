#include "Terrain.h"

#include <vector>

using namespace std;

// Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include FreeImage library
#include <FreeImage.h>
// Include ShaderManager
#include "RenderControl/ShaderManager.h"
// Include CTexture
#include "Texture.h"

/**
 @brief Constructor
 */
CTerrain::CTerrain(void)
	: vRenderScale(glm::vec3(1.0f, 1.0f, 1.0f))
	, vMinPos(glm::vec3(-1.0f))
	, vMaxPos(glm::vec3(1.0f))
{
	// Set the default position of the Terrain
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Destructor
 */
CTerrain::~CTerrain(void)
{
	ReleaseHeightmap();
}

/**
 @brief Initialise this class instance
 @return A bool variable
 */
bool CTerrain::Init(std::string gameFileName)
{
	gameLevel = gameFileName;
	// Load all the 5 textures
	if (LoadAllTextures() == false)
		return false;
	
	// Load greyscale image for the terrain
	if (LoadHeightMapFromImage("Image/Terrain/World/terrain.bmp") == false)
		return false;

	if (LoadGridMapFromImage("Image/Terrain/World/"+ gameLevel +".bmp") == false) {
		return false;
	}

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable to be assigned to this class instance
 */
void CTerrain::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable to be assigned to this class instance
 */
void CTerrain::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable to be assigned to this class instance
 */
void CTerrain::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
 */
bool CTerrain::Update(const double dElapsedTime)
{
	return true;
}

/**
 @brief PreRender.
 */
void CTerrain::PreRender(void)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render.
 */
void CTerrain::Render(void)
{
	unsigned int m_programID = CShaderManager::GetInstance()->GetProgramID("Shader3D_Terrain");
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	glm::vec3 fogColor(0.8f, 0.8f, 0.8f);

	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_START], 10.f);
	glUniform1f(m_parameters[U_FOG_END], 1000.f);
	glUniform1f(m_parameters[U_FOG_DENSITY], FOG_DENSITY);
	glUniform1i(m_parameters[U_FOG_TYPE], 1);
	glUniform1i(m_parameters[U_FOG_ENABLED], 1);

	CShaderManager::GetInstance()->activeShader->setMat4("matrices.projMatrix", projection);
	CShaderManager::GetInstance()->activeShader->setMat4("matrices.viewMatrix", view);

	// We bind all 5 textures - 3 of them are textures for layers, 1 texture is a "path" texture, and last one is
	// the places in heightmap where path should be and how intense should it be
	for (int i = 0; i < 5; i++)
	{
		char sSamplerName[256];
		sprintf(sSamplerName, "gSampler[%d]", i);
		tTextures[i].BindTexture(i);
		CShaderManager::GetInstance()->activeShader->setInt(sSamplerName, i);
	}

	// Create model transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);

	CShaderManager::GetInstance()->activeShader->setMat4("matrices.modelMatrix", model);
	CShaderManager::GetInstance()->activeShader->setMat4("matrices.normalMatrix", glm::mat4(1.0f));
	CShaderManager::GetInstance()->activeShader->setVec4("vColor", glm::vec4(1.0f));

	CShaderManager::GetInstance()->activeShader->setFloat("fRenderHeight", vRenderScale.y);
	CShaderManager::GetInstance()->activeShader->setFloat("fMaxTextureU", float(iCols) * 0.1f);
	CShaderManager::GetInstance()->activeShader->setFloat("fMaxTextureV", float(iRows) * 0.1f);

	CShaderManager::GetInstance()->activeShader->setMat4("HeightmapScaleMatrix", glm::scale(glm::mat4(1.0), glm::vec3(vRenderScale)));

	// Now we're ready to render - we are drawing set of triangle strips using one call, but we g otta enable primitive restart
	glBindVertexArray(uiVAO);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(iRows * iCols);

	int iNumIndices = (iRows - 1) * iCols * 2 + iRows - 1;
	glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);
	glBindVertexArray(0);

	// Unbind the texture and samplers
	for (int i = 0; i < 5; i++)
	{
		tTextures[i].UnBindTexture(i);
	}
}

/**
 @brief PostRender
 */
void CTerrain::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

bool CTerrain::IsInWall(glm::vec3 pPos) {
	int row = int((pPos.z + 50.f)); // player X for map is -50 to 50;
	int col = int((pPos.x + 50.f)); // player X for map is -50 to 50;
	if (row > 99 || row < 0 || col > 99 || col < 0) {
		return true;
	}

	if (vGridVertexData[row][col].y >= 1.0f) {
		return true;
	}

	return false;
}

bool CTerrain::LoadGridMapFromImage(const string sImagePath) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);
	totalWalls = 0;
	fif = FreeImage_GetFileType(sImagePath.c_str(), 0); // Check the file signature and deduce its format

	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(sImagePath.c_str());

	if (fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, sImagePath.c_str());
	if (!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
	iRows = FreeImage_GetHeight(dib);
	iCols = FreeImage_GetWidth(dib);
	// We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
	if (bDataPointer == NULL || iRows == 0 || iCols == 0 || (FreeImage_GetBPP(dib) != 24 && FreeImage_GetBPP(dib) != 8))
		return false;

	// How much to increase data pointer to get to next pixel data
	unsigned int ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;
	// Length of one row in data
	unsigned int row_step = ptr_inc * iCols;

	vGridVertexData = vector< vector< glm::vec3> >(iRows, vector<glm::vec3>(iCols));
	vGridCoordsData = vector< vector< glm::vec2> >(iRows, vector<glm::vec2>(iCols));
	vGridVertexData.resize(iRows, vector<glm::vec3>(iCols, glm::vec3(0.0f)));
	vGridCoordsData.resize(iRows, vector<glm::vec2>(iCols, glm::vec2(0.0f)));

	float fTextureU = float(iCols) * 0.1f;
	float fTextureV = float(iRows) * 0.1f;
	for (int i = 0; i < iRows; i++)
	{
		for (int j = 0; j < iCols; j++)
		{
			float fScaleC = float(j) / float(iCols - 1);
			float fScaleR = float(i) / float(iRows - 1);
			// if height  == 1 <- this is a wall, the rest we treat as  walkable now
			// height go from 0-255, 128,128,128
			// per grid cell is 0...1, total 100 grid cell
			float fVertexHeight = float(*(bDataPointer + row_step * i + j * ptr_inc)) / 255.0f;
			
			vGridVertexData[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
			vGridCoordsData[i][j] = glm::vec2(fTextureU * fScaleC, fTextureV * fScaleR);
		}
	}
	cout << "Print map data" << endl;
	for (vector< glm::vec3>& row : vGridVertexData) {
		
		for (glm::vec3& col : row) {
			if (col.y >= 1.f) {
				cout << "*";
				totalWalls += 1;
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}
	cout << "End map data, total walls: " << totalWalls << endl;
}

int CTerrain::GetTotalWalls() {
	return totalWalls;
}
/**
 @brief Loads a heightmap and builds up all OpenGL structures for rendering.
 @param sImagePath - path to the (optimally) grayscale
		image containing heightmap data.
 @return A bool variable
*/
bool CTerrain::LoadHeightMapFromImage(const string sImagePath)
{
	if (bLoaded)
	{
		bLoaded = false;
		ReleaseHeightmap();
	}
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sImagePath.c_str(), 0); // Check the file signature and deduce its format

	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(sImagePath.c_str());

	if (fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, sImagePath.c_str());
	if (!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data
	iRows = FreeImage_GetHeight(dib);
	iCols = FreeImage_GetWidth(dib);

	// We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
	if (bDataPointer == NULL || iRows == 0 || iCols == 0 || (FreeImage_GetBPP(dib) != 24 && FreeImage_GetBPP(dib) != 8))
		return false;

	// How much to increase data pointer to get to next pixel data
	unsigned int ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;
	// Length of one row in data
	unsigned int row_step = ptr_inc * iCols;

	vboHeightmapData.CreateVBO();
	// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
	//vector< vector< glm::vec3> > vVertexData(iRows, vector<glm::vec3>(iCols));
	//vector< vector< glm::vec2> > vCoordsData(iRows, vector<glm::vec2>(iCols));

	vVertexData.resize(iRows, vector<glm::vec3>(iCols, glm::vec3(0.0f)));
	vCoordsData.resize(iRows, vector<glm::vec2>(iCols, glm::vec2(0.0f)));

	float fTextureU = float(iCols) * 0.1f;
	float fTextureV = float(iRows) * 0.1f;

	for (int i = 0; i < iRows; i++)
	{
		for (int j = 0; j < iCols; j++)
		{
			float fScaleC = float(j) / float(iCols - 1);
			float fScaleR = float(i) / float(iRows - 1);
			float fVertexHeight = float(*(bDataPointer + row_step * i + j * ptr_inc)) / 255.0f;
			vVertexData[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
			vCoordsData[i][j] = glm::vec2(fTextureU * fScaleC, fTextureV * fScaleR);
		}
	}

	// Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
	vector< vector<glm::vec3> > vNormals[2];
	for (int i = 0; i < 2; i++)
		vNormals[i] = vector< vector<glm::vec3> >(iRows - 1, vector<glm::vec3>(iCols - 1));

	for (int i = 0; i < iRows - 1; i++)
	{
		for (int j = 0; j < iCols - 1; j++)
		{
			glm::vec3 vTriangle0[] =
			{
				vVertexData[i][j],
				vVertexData[i + 1][j],
				vVertexData[i + 1][j + 1]
			};
			glm::vec3 vTriangle1[] =
			{
				vVertexData[i + 1][j + 1],
				vVertexData[i][j + 1],
				vVertexData[i][j]
			};

			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

			vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
			vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
		}
	}

	vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));

	for (int i = 0; i < iRows; i++)
		for (int j = 0; j < iCols; j++)
		{
			// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
			// of all adjacent triangles' normals
			glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for upper-left triangles
			if (j != 0 && i != 0)
				for (int k = 0; k < 2; k++)
					vFinalNormal += vNormals[k][i - 1][j - 1];
			// Look for upper-right triangles
			if (i != 0 && j != iCols - 1)
				vFinalNormal += vNormals[0][i - 1][j];
			// Look for bottom-right triangles
			if (i != iRows - 1 && j != iCols - 1)
				for (int k = 0; k < 2; k++)
					vFinalNormal += vNormals[k][i][j];
			// Look for bottom-left triangles
			if (i != iRows - 1 && j != 0)
				vFinalNormal += vNormals[1][i][j - 1];
			vFinalNormal = glm::normalize(vFinalNormal);

			vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
		}

	// First, create a VBO with only vertex data
	int counter = 0;
	vboHeightmapData.CreateVBO(iRows * iCols * (2 * sizeof(glm::vec3) + sizeof(glm::vec2))); // Preallocate memory
	for (int i = 0; i < iRows; i++)
	{
		for (int j = 0; j < iCols; j++)
		{
			vboHeightmapData.AddData(&vVertexData[i][j], sizeof(glm::vec3)); // Add vertex
			vboHeightmapData.AddData(&vCoordsData[i][j], sizeof(glm::vec2)); // Add tex. coord
			vboHeightmapData.AddData(&vFinalNormals[i][j], sizeof(glm::vec3)); // Add normal
			counter++;
		}
	}

	// Now create a VBO with heightmap indices
	/*
		0-1-2
		|/|/|
		3-4-5
		and
		3-4-5
		|/|/|
		6-7-8

		0, 3, 1, 4, 2, 5 for first strip and
		3, 6, 4, 7, 5, 8 for second strip
	*/
	vboHeightmapIndices.CreateVBO();
	int iPrimitiveRestartIndex = iRows * iCols;
	for (int i = 0; i < iRows - 1; i++)
	{
		for (int j = 0; j < iCols - 1; j++)
			for (int k = 1; k >= 0; k--)
			{
				int iRow = i + (1 - k);
				int iIndex = iRow * iCols + j;
				vboHeightmapIndices.AddData(&iIndex, sizeof(int));
			}
		// Restart triangle strips
		vboHeightmapIndices.AddData(&iPrimitiveRestartIndex, sizeof(int));
	}

	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);
	// Attach vertex data to this VAO
	vboHeightmapData.BindVBO();
	vboHeightmapData.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	// And now attach index data to this VAO
	// Here don't forget to bind another type of VBO - the element array buffer, or simplier indices to vertices
	vboHeightmapIndices.BindVBO(GL_ELEMENT_ARRAY_BUFFER);
	vboHeightmapIndices.UploadDataToGPU(GL_STATIC_DRAW);
	vboHeightmapData.BindVBO();

	glBindVertexArray(0);

	bLoaded = true; // If get here, we succeeded with generating heightmap
	return true;
}

/**
 @brief Sets rendering size (scaling) of heightmap.
 @param fRenderX Size in X-axis
 @param fHeight Height in Y-axis
 @param fRenderZ Size in Z-axis
 */
void CTerrain::SetRenderSize(const float fRenderX, const float fHeight, const float fRenderZ)
{
	vRenderScale = glm::vec3(fRenderX, fHeight, fRenderZ);

	// Calculate the min and max positions
	vMinPos = glm::vec3(fRenderX * -0.5f, 0.0f, fRenderZ * -0.5f);
	vMaxPos = glm::vec3(fRenderX * 0.5f, fHeight, fRenderZ * 0.5f);
}

/**
 @brief Sets rendering size (scaling) of heightmap.
 @param fQuadSize How big should be one quad
 @param fHeight - Height of heightmap
 */
void CTerrain::SetRenderSize(const float fQuadSize, const float fHeight)
{
	vRenderScale = glm::vec3(float(iCols)*fQuadSize, fHeight, float(iRows)*fQuadSize);
}

/**
 @brief Releases all data of one heightmap instance.
 */
void CTerrain::ReleaseHeightmap(void)
{
	if(!bLoaded)
		return; // Heightmap must be loaded

	vboHeightmapData.DeleteVBO();
	vboHeightmapIndices.DeleteVBO();

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glDeleteVertexArrays(1, &uiVAO);
	bLoaded = false;
}

/**
 @brief Get Height Map rows
 @return A int variable
 */
int CTerrain::GetNumHeightmapRows(void) const
{
	return iRows;
}

/**
 @brief Get Height Map columns
 @return A int variable
 */
int CTerrain::GetNumHeightmapCols(void) const
{
	return iCols;
}

/**
 @brief Get the height at a X- and Z-coordinate
 @param fX A const float variable containing the x coordinate to check for the height
 @param fZ A const float variable containing the z coordinate to check for the height
 @return A float variable
 */
float CTerrain::GetHeight(const float fX, const float fZ)
{
	// If it is out of the boundary, then return 0.0f
	if (((fX < vMinPos.x) || (fX >= vMaxPos.x)) || 
		((fZ < vMinPos.z) || (fZ >= vMaxPos.z)))
		return 0.0f;

	// Calculate the raw indices for these coordinates
	float fCol = ((fX - (vRenderScale.x * -0.5f)) / vRenderScale.x) * iCols;
	float fRow = ((fZ - (vRenderScale.z * -0.5f)) / vRenderScale.z) * iRows;

	// Calculate the indices for these coordinates
	int iCol = floor(fCol);
	int iRow = floor(fRow);

	// Use Bilinear interpolation of the surrounding 4 indices to smooth out the height
	float fTX = fRow - iRow;
	float fTZ = fCol - iCol;
	float fSampleH1 = vVertexData[iRow][iCol].y;
	float fSampleH2 = vVertexData[iRow + 1][iCol].y;
	float fSampleH3 = vVertexData[iRow][iCol + 1].y;
	float fSampleH4 = vVertexData[iRow + 1][iCol + 1].y;
	float fFinalHeight = (fSampleH1 * (1.0f - fTX) + fSampleH2 * fTX) * (1.0f - fTZ) +
						(fSampleH3 * (1.0f - fTX) + fSampleH4 * fTX) * (fTZ);

	return vRenderScale.y * fFinalHeight;
}

glm::vec3 CTerrain::GetNormal(const float fX, const float fZ)
{
	// If it is out of the boundary, then return 0.0f
	if (((fX < vMinPos.x) || (fX >= vMaxPos.x)) ||
		((fZ < vMinPos.z) || (fZ >= vMaxPos.z)))
		return glm::vec3(0,1,0);

	// Calculate the raw indices for these coordinates
	float fCol = ((fX - (vRenderScale.x * -0.5f)) / vRenderScale.x) * iCols;
	float fRow = ((fZ - (vRenderScale.z * -0.5f)) / vRenderScale.z) * iRows;

	// Calculate the indices for these coordinates
	int iCol = floor(fCol);
	int iRow = floor(fRow);
	return vFinalNormals[iRow][iCol];
	// Use Bilinear interpolation of the surrounding 4 indices to smooth out the height
	//float fTX = fRow - iRow;
	//float fTZ = fCol - iCol;
	
	/*float fSampleH1 = vVertexData[iRow][iCol].y;
	float fSampleH2 = vVertexData[iRow + 1][iCol].y;
	float fSampleH3 = vVertexData[iRow][iCol + 1].y;
	float fSampleH4 = vVertexData[iRow + 1][iCol + 1].y;
	float fFinalHeight = (fSampleH1 * (1.0f - fTX) + fSampleH2 * fTX) * (1.0f - fTZ) +
		(fSampleH3 * (1.0f - fTX) + fSampleH4 * fTX) * (fTZ);*/

	//return vRenderScale.y * fFinalHeight;
	//return 0;
}

glm::vec3 CTerrain::GetDownwardSlope(const float fX, const float fZ)
{
	// If it is out of the boundary, then return 0.0f
	if (((fX < vMinPos.x) || (fX >= vMaxPos.x)) ||
		((fZ < vMinPos.z) || (fZ >= vMaxPos.z)))
		return glm::vec3(0, 0, 0);

	// Calculate the raw indices for these coordinates
	float fCol = ((fX - (vRenderScale.x * -0.5f)) / vRenderScale.x) * iCols;
	float fRow = ((fZ - (vRenderScale.z * -0.5f)) / vRenderScale.z) * iRows;

	// Calculate the indices for these coordinates
	int iCol = floor(fCol);
	int iRow = floor(fRow);
	glm::vec3 p1 = vVertexData[iRow][iCol];
	glm::vec3 p2 = vVertexData[iRow+1][iCol];
	if (p1.y < p2.y) {
		return glm::normalize(p1 - p2);
	}
	else {
		return glm::normalize(p2 - p1);
	}
	
}

/**
 @brief Get the vMinPos
 @return A glm::vec3 variable
 */
glm::vec3 CTerrain::GetMinPos(void) const
{
	return vMinPos;
}

/**
 @brief Get the vMaxPos
 @return A glm::vec3 variable
 */
glm::vec3 CTerrain::GetMaxPos(void) const
{
	return vMaxPos;
}

/**
 @brief Print Self
 */
void CTerrain::PrintSelf(void)
{
	cout << "CTerrain::PrintSelf()" << endl;
	cout << "========================" << endl;
}