// lightdata.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_light_H_
#define AFX_light_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>

class lightdata
{
	class light		// store a scene data of one single object
	{
	public:
		float pos[3];		// the position of the light 
		float amb[4];		// define the ambient term
		float dif[3];		// define the diffuse term
		float spe[3];		// define the specular term

		~light() {};
		light() {};
	};
public:
	light *point;
	float amb[3];			// define the ambient light from environment.
	lightdata(const char* obj_file, int size);
	~lightdata();

};

#endif
#pragma once

