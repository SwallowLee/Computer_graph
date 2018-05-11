#include <iostream>
#include <stdio.h>

class view{
public:
	float eye[3], vat[3], vup[3], fovy, dnear, dfar, viewport[4];
	view(const char* vFile){
		FILE *pfile;
		errno_t err;
		char name[10];
		err = fopen_s(&pfile, vFile, "r");
		if (err != 0)
		{
			printf("error to open view.view\n");
			return;
		}
		
		while (!feof(pfile))
		{
			fscanf(pfile, "%s", name);
			if (strcmp(name, "eye") == 0)
			{
				fscanf(pfile, "%f %f %f\n", &eye[0], &eye[1], &eye[2]);
			}
			else if (strcmp(name, "vat") == 0)
			{
				fscanf(pfile, "%f %f %f\n", &vat[0], &vat[1], &vat[2]);
			}
			else if (strcmp(name, "vup") == 0)
			{
				fscanf(pfile, "%f %f %f\n", &vup[0], &vup[1], &vup[2]);
			}
			else if (strcmp(name, "fovy") == 0)
			{
				fscanf(pfile, "%f\n", &fovy);
			}
			else if (strcmp(name, "dnear") == 0)
			{
				fscanf(pfile, "%f\n", &dnear);
			}
			else if (strcmp(name, "dfar") == 0)
			{
				fscanf(pfile, "%f\n", &dfar);
			}
			else if (strcmp(name, "viewport") == 0)
			{
				fscanf(pfile, "%f %f %f %f\n", &viewport[0], &viewport[1], &viewport[2], &viewport[3]);
			}
		}
		return;
	}
};