// scene.cpp: implementation of the scene class.
//
//////////////////////////////////////////////////////////////////////

#include "scene.h"
#include <iostream>
#include <vector>
#include "Freeimage.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


scene::scene(const char* obj_file, int size )
{
	FILE *input;
	char token[50];
	char tmpmethod[50];
	input = fopen(obj_file, "r");
	object = new objdata[size];
	texturedata tmp_texture;
	int tex_pos = 0;
	int tmp_tex= 0;

	if (!input)
	{
		cout << string("Can not open scene File \"") << obj_file << "\" !" << endl;
		return;
	}

	cout << endl << obj_file << endl;

	for(int i = 0 ; i < size ;)
	{
		token[0] = NULL;
		fscanf(input, "%s", token);		// Ū token

		if (!strcmp(token, "model"))
		{
			strcpy(object[i].method, tmpmethod);
			object[i].textureid = tmp_tex;
			fscanf(input, "%s", object[i].name );
			fscanf(input, "%f %f %f", &object[i].scale[0], &object[i].scale[1], &object[i].scale[2]);
			fscanf(input, "%f %f %f %f", &object[i].angle[0], &object[i].angle[1], &object[i].angle[2] , &object[i].angle[3]);
			fscanf(input, "%f %f %f", &object[i].transfer[0], &object[i].transfer[1], &object[i].transfer[2]);
			++i;
		}
		else if (!strcmp(token, "single-texture")) {
			tmp_tex = tex_pos;
			++tex_pos;
			strcpy(tmpmethod, token);

			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = -1;
			texture.push_back(tmp_texture);

		}
		else if (!strcmp(token, "no-texture")) {

			strcpy(tmpmethod, token);
		}
		else if (!strcmp(token, "multi-texture")) {
			tmp_tex = tex_pos;
			tex_pos += 2;
			strcpy(tmpmethod, token);

			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = -1;
			texture.push_back(tmp_texture);

			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = -1;
			texture.push_back(tmp_texture);
		}
		else if (!strcmp(token, "cube-map")) {
			tmp_tex = tex_pos;
			tex_pos += 6;
			strcpy(tmpmethod, token);

			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = 0;
			texture.push_back(tmp_texture);

			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = 1;
			texture.push_back(tmp_texture);

			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = 2;
			texture.push_back(tmp_texture);

			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = 3;
			texture.push_back(tmp_texture);
			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = 4;
			texture.push_back(tmp_texture);

			fscanf(input, "%s", tmp_texture.filename);
			tmp_texture.meshid = i;
			tmp_texture.type = 5;
			texture.push_back(tmp_texture);
		}
		cout << "tex number: " << tex_pos << endl;
		cout << "obj scene: " << i << endl;
	}
	if (input) fclose(input);
}


scene::~scene(){
	delete[](object);
}
