#include "textureLoader.h"
#include "FreeImage.h"
#include "glew.h"


TextureLoader::TextureLoader()
{
	glGenTextures(1, &m_iTextureId);
}

TextureLoader::~TextureLoader()
{
	glDeleteTextures(1, &m_iTextureId);
}

void TextureLoader::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_iTextureId);
}

void TextureLoader::Load(const char* pszFileName)
{
	static bool bInitLib = false;
	if(!bInitLib)
	{
		bInitLib = true;
		FreeImage_Initialise();
	}

	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pszFileName, 0), pszFileName);
	FIBITMAP *p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);

	glBindTexture(GL_TEXTURE_2D, m_iTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void TextureLoader::GenerateMipmap()
{
	Bind();
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}