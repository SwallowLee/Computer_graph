#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

/**
 * texture loader
 *
 * This class needs glew library for generate mipmap
 * Please add glew32.lib and glew32s.lib in the linker input!
 */
class TextureLoader
{
public:
	unsigned int m_iTextureId;				///< Texture id generated by openGL

	TextureLoader();						///< Constructor
	~TextureLoader();						///< destructor

	void Bind();							///< bind this texture
	void Load(const char* pszFileName);		///< load a bmp file by glaux
	void GenerateMipmap();					///< generate mipmap
};

#endif
