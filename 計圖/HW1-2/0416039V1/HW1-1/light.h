#include <iostream>
#include <stdio.h>

class light{
public:
	float x[10], y[10], z[10];
	float specular[8][3], diffuse[8][3], ambient[8][3];
	float envir[3];
	int count;

	light(const char* ligFile)
	{
		FILE *pfile;
		errno_t err;
		char name[10];
		count = 0;
		err = fopen_s(&pfile, ligFile, "r");
		if (err != 0)
		{
			printf("error to open light.light\n");
			return;
		}

		while (!feof(pfile))
		{
			fscanf(pfile, "%s", name);
			if (strcmp(name, "light") == 0)
			{
				fscanf(pfile, "%f %f %f ", &x[count], &y[count], &z[count]);
				fscanf(pfile, "%f %f %f ", &ambient[count][0], &ambient[count][1], &ambient[count][2]);
				fscanf(pfile, "%f %f %f ", &diffuse[count][0], &diffuse[count][1], &diffuse[count][2]);
				fscanf(pfile, "%f %f %f ", &specular[count][0], &specular[count][1], &specular[count][2]);
				count++;
			}
			else if (strcmp(name, "ambient") == 0)
			{
				fscanf(pfile, "%f %f %f", &envir[0], &envir[1], &envir[2]);
			}
		}
		return;
	}
};
