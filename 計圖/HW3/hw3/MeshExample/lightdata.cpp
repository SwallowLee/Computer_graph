// lightdata.cpp: implementation of the light class.
//
//////////////////////////////////////////////////////////////////////

#include "lightdata.h"
#include <iostream>


using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


lightdata::lightdata(const char* obj_file, int size)
{
	FILE *input;
	char token[50];
	input = fopen(obj_file, "r");
	point = new light[size];

	if (!input)
	{
		cout << string("Can not open scene File \"") << obj_file << "\" !" << endl;
		return;
	}

	cout << endl << obj_file << endl;

	for (int i = 0; i < size; ++i)
	{
		token[0] = NULL;
		fscanf(input, "%s", token);		// read token
		if (!strcmp(token, "light"))
		{
			fscanf(input, "%f %f %f", &point[i].pos[0], &point[i].pos[1], &point[i].pos[2]);
			fscanf(input, "%f %f %f", &point[i].amb[0], &point[i].amb[1], &point[i].amb[2]);
			fscanf(input, "%f %f %f", &point[i].dif[0], &point[i].dif[1], &point[i].dif[2]);
			fscanf(input, "%f %f %f", &point[i].spe[0], &point[i].spe[1], &point[i].spe[2]);
		}
	}
	fscanf(input, "%s", token);
	fscanf(input, "%f %f %f", &amb[0], &amb[1], &amb[2]);
	if (input) fclose(input);
}


lightdata::~lightdata()
{
}
