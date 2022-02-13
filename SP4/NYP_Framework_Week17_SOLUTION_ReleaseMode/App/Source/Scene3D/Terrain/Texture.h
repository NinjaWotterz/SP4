/**
 CTexture
 By: Toh Da Jun
 Date: Sept 2021
 */

#pragma once
enum ETextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

class CTexture
{
public:
	// Constructor
	CTexture(void);

	// Destructor
	~CTexture(void);

	void CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format);
	void CreateFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps = false);

	bool LoadTexture2D(string a_sPath, bool bGenerateMipMaps = false);
	bool ReloadTexture(void);
	void BindTexture(const int iTextureUnit = 0);
	void UnBindTexture(const int iTextureUnit = 0);
	void DeleteTexture(void);

	void SetFiltering(int a_tfMagnification, int a_tfMinification);

	void SetSamplerParameter(GLenum parameter, GLenum value);

	// Get methods
	int GetMinificationFilter(void) const;
	int GetMagnificationFilter(void) const;
	int GetWidth(void) const;
	int GetHeight(void) const;
	int GetBPP(void) const;
	unsigned int GetTextureID(void) const;
	string GetPath(void) const;

protected:
	int iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
	unsigned int uiTexture; // Texture name
	unsigned int uiSampler; // Sampler name
	bool bMipMapsGenerated;

	int tfMinification, tfMagnification;

	string sPath;
};

// The number of textures for the terrain
#define NUMTEXTURES 5
// The array of textures for the terrain
extern CTexture tTextures[NUMTEXTURES];
// Load the 5 textures for the terrain
bool LoadAllTextures(void);