#include <iostream>
#include <stdio.h>
#include "mesh.h"
#include <string.h>

class scene{
public:
	float scale[100][3], angle[100], rotate[100][3], transfer[100][3];
	char name[100][100];
	int count;
	mesh *object[100];

	//texture
	char tex_name[100][6][100];	//[type_counter][最多六個(cube)][file_name];
	int tex_type[100];	//1: single 2: multi 3: cube
	int obj_tex_index[100];	//對應到哪個texture
	int obj_tex_type[100];	//0: no 1: single 2: multi 3: cube 
	int tex_counter;

	scene(const char* sFile){
		int type;
		char ch[100];
		FILE *pfile;
		errno_t err;
		count = 0;
		tex_counter = 0;
		err = fopen_s(&pfile, sFile, "r");
		if (err != 0)
		{
			printf("error to open scene.scene\n");
			return;
		}

		while (!feof(pfile))
		{
			fscanf(pfile, "%s", ch);
			if (strcmp(ch, "model") == 0)
			{
				//printf("%d\n", count);
				fscanf(pfile, "%s", name[count]);
				fscanf(pfile, "%f %f %f ", &scale[count][0], &scale[count][1], &scale[count][2]);
				fscanf(pfile, "%f ", &angle[count]);
				fscanf(pfile, "%f %f %f ", &rotate[count][0], &rotate[count][1], &rotate[count][2]);
				fscanf(pfile, "%f %f %f\n", &transfer[count][0], &transfer[count][1], &transfer[count][2]);
				obj_tex_index[count] = tex_counter - 1;
				obj_tex_type[count] = type;
				count++;
			}
			else if (strcmp(ch, "no_texture") == 0)
			{
				type = 0;
			}
			else if (strcmp(ch, "single-texture") == 0)
			{
				type = 1;
				fscanf(pfile, "%s", tex_name[tex_counter][0]);
				tex_type[tex_counter] = type;
				tex_counter++;
			}
			else if (strcmp(ch, "multi-texture") == 0)
			{
				type = 2;
				for (int i = 0; i < 2; i++){
					fscanf(pfile, "%s", tex_name[tex_counter][0]);
					tex_type[tex_counter] = 2;
					tex_counter++;
				}
			}
			else if (strcmp(ch, "cube-map") == 0)
			{
				type = 3;
				fscanf(pfile, "%s", tex_name[tex_counter][0]);
				fscanf(pfile, "%s", tex_name[tex_counter][1]);
				fscanf(pfile, "%s", tex_name[tex_counter][2]);
				fscanf(pfile, "%s", tex_name[tex_counter][3]);
				fscanf(pfile, "%s", tex_name[tex_counter][4]);
				fscanf(pfile, "%s", tex_name[tex_counter][5]);
				tex_type[tex_counter] = type;
				tex_counter++;
			}
		}
		for (int i = 0; i < count; i++){
			object[i] = new mesh(name[i]);
			//printf("%s: %d %d\n", name[i], obj_tex_type[i], obj_tex_index[i]);
		}
		return;
	}
};