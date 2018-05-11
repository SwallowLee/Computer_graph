#include <iostream>
#include <stdio.h>
#include "mesh.h"

class scene{
public:
	float scale[10][3], angle[10], rotate[10][3], transfer[10][3];
	char name[10][100];
	int count;
	mesh *object[10];

	scene(const char* sFile){
		FILE *pfile;
		errno_t err;
		count = 0;
		err = fopen_s(&pfile, sFile, "r");
		if (err != 0)
		{
			printf("error to open scene.scene\n");
			return;
		}

		while (!feof(pfile))
		{
			fscanf(pfile, "model ");
			fscanf(pfile, "%s", &name[count]);
			fscanf(pfile, "%f %f %f ", &scale[count][0], &scale[count][1], &scale[count][2]);
			fscanf(pfile, "%f ", &angle[count]);
			fscanf(pfile, "%f %f %f ", &rotate[count][0], &rotate[count][1], &rotate[count][2]);
			fscanf(pfile, "%f %f %f\n", &transfer[count][0], &transfer[count][1], &transfer[count][2]);
			count++;
		}
		for (int i = 0; i < count; i++){
			object[i] = new mesh(name[i]);
		}
		return;
	}
};