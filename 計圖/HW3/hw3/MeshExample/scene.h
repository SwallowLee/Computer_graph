// scene.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_scene_H_
#define AFX_scene_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class scene
{
	class texturedata {
	public:
		char filename[50];
		int meshid;
		int type;
		~texturedata() {};
		texturedata() {};
	};

	class objdata		// store a scene data of one single object
	{
	public:
		int textureid;
		char name[50];			// the file name of the mesh
		float scale[3];			// the scale value of each direction
		float angle[4];			// [ 0 ] is the degree of rotation. the rest are the rotation axis vector
		float transfer[3];		// the transfer vector. It means that the origin of the model should move to (x, y, z)
		char method[50];		// the texture mapping method this object adopt

		~objdata() {};
		objdata() {};
	};

public:
	objdata *object;
	std::vector<texturedata> texture;
	scene(const char* obj_file , int size );
	~scene();

};


#endif
#pragma once
