// view.cpp: implementation of the view class.
//
//////////////////////////////////////////////////////////////////////

#include "view.h"
#include <iostream>


using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

view::view(const char* obj_file)
{
	FILE *input;
	input = fopen( obj_file , "r" );
	char	token[100];

	if (!input)
	{
		cout << string("Can not open view File \"") << obj_file << "\" !" << endl;
		return;
	}

	cout << endl << obj_file << endl;

	while (!feof(input))
	{
		token[0] = NULL;
		fscanf(input, "%s", token);		// Ū token

		if (!strcmp(token, "eye"))
		{
			fscanf(input, "%f %f %f", &eye[ 0 ] , &eye[1] , &eye[2]);
		}

		else if (!strcmp(token, "vat"))
		{
			fscanf(input, "%f %f %f", &vat[0], &vat[1], &vat[2]);
		}

		else if (!strcmp(token, "vup"))
		{
			fscanf(input, "%f %f %f", &vup[0], &vup[1], &vup[2]);
		}

		else if (!strcmp(token, "fovy"))
		{
			fscanf(input, "%f", &fovy);
		}

		else if (!strcmp(token, "dnear"))
		{
			fscanf(input, "%f", &dnear);
		}
		else if (!strcmp(token, "dfar"))
		{
			fscanf(input, "%f", &dfar);
		}

		else if (!strcmp(token, "viewport"))
		{
			fscanf(input, "%f %f %f %f", &viewport[0] , &viewport[1] , &viewport[2] , &viewport[3]);
		}

	}

	if (input) fclose(input);

}



view::~view()
{
}

