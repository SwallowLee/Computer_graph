#include "mesh.h"
#include "FreeImage.h"
#include <GL/glew.h>

class texture
{
public:
	int tex_Width;
	int tex_Height;
	unsigned int texObject[1];

	texture(){
		glGenTextures(1, texObject);
	}

	~texture(){
		glDeleteTextures(1, texObject);
	}

	void LoadTexture(char* fname){
		FreeImage_Initialise();
		FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(fname, 0), fname);
		FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
		tex_Width = FreeImage_GetWidth(p32BitsImage);
		tex_Height = FreeImage_GetHeight(p32BitsImage);

		glBindTexture(GL_TEXTURE_2D, texObject[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_Width, tex_Height,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));

		//printf("FIN %d %d", tex_Width, tex_Height);
		glGenerateMipmap(GL_TEXTURE_2D);
		//printf("FIN %d %d", tex_Width, tex_Height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		FreeImage_Unload(p32BitsImage);
		FreeImage_Unload(pImage);
	}
	
	void LoadEnvirTexture(char *fname1, char *fname2, char *fname3, 
							char *fname4, char *fname5, char *fname6){
		FIBITMAP* pImage = NULL;
		FIBITMAP* p32BitsImage = NULL;

		glBindTexture(GL_TEXTURE_CUBE_MAP, texObject[0]);
		for (int i = 0; i < 6; i++)
		{
			if (i == 0)
			{
				FreeImage_Initialise();
				pImage = FreeImage_Load(FreeImage_GetFileType(fname1, 0), fname1);
				p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
				tex_Width = FreeImage_GetWidth(p32BitsImage);
				tex_Height = FreeImage_GetHeight(p32BitsImage);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, 
							tex_Width, tex_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, 
							(void*)FreeImage_GetBits(pImage));
			}
			else if (i == 1)
			{
				FreeImage_Initialise();
				pImage = FreeImage_Load(FreeImage_GetFileType(fname2, 0), fname2);
				p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
				tex_Width = FreeImage_GetWidth(p32BitsImage);
				tex_Height = FreeImage_GetHeight(p32BitsImage);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA,
					tex_Width, tex_Height, 0, GL_BGR, GL_UNSIGNED_BYTE,
					(void*)FreeImage_GetBits(pImage));
			}
			else if (i == 2)
			{
				FreeImage_Initialise();
				pImage = FreeImage_Load(FreeImage_GetFileType(fname3, 0), fname3);
				p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
				tex_Width = FreeImage_GetWidth(p32BitsImage);
				tex_Height = FreeImage_GetHeight(p32BitsImage);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA,
					tex_Width, tex_Height, 0, GL_BGR, GL_UNSIGNED_BYTE,
					(void*)FreeImage_GetBits(pImage));
			}
			else if (i == 3)
			{
				FreeImage_Initialise();
				pImage = FreeImage_Load(FreeImage_GetFileType(fname4, 0), fname4);
				p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
				tex_Width = FreeImage_GetWidth(p32BitsImage);
				tex_Height = FreeImage_GetHeight(p32BitsImage);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA,
					tex_Width, tex_Height, 0, GL_BGR, GL_UNSIGNED_BYTE,
					(void*)FreeImage_GetBits(pImage));
			}
			else if (i == 4)
			{
				FreeImage_Initialise();
				pImage = FreeImage_Load(FreeImage_GetFileType(fname5, 0), fname5);
				p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
				tex_Width = FreeImage_GetWidth(p32BitsImage);
				tex_Height = FreeImage_GetHeight(p32BitsImage);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA,
					tex_Width, tex_Height, 0, GL_BGR, GL_UNSIGNED_BYTE,
					(void*)FreeImage_GetBits(pImage));
			}
			else if (i == 5)
			{
				FreeImage_Initialise();
				pImage = FreeImage_Load(FreeImage_GetFileType(fname6, 0), fname6);
				p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
				tex_Width = FreeImage_GetWidth(p32BitsImage);
				tex_Height = FreeImage_GetHeight(p32BitsImage);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA,
					tex_Width, tex_Height, 0, GL_BGR, GL_UNSIGNED_BYTE,
					(void*)FreeImage_GetBits(pImage));
			}
		}

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexEnvf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		FreeImage_Unload(p32BitsImage);
		FreeImage_Unload(pImage);
	}

	void Bind(){
		glBindTexture(GL_TEXTURE_2D, texObject[0]);
	}
};