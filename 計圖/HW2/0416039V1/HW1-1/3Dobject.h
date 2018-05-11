#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>

using namespace std;
class material{
public:
	map<char *, int> mtl;
	int count = 1;
	struct feature{
		float Illum, Kd[3], Ka[3], Tf[3], Ni, Ks[3], Ns;
	};
	vector<feature>  mtl_f;
	material(char* mFile){
		FILE *pfile;
		errno_t err;
		char name[10], buf[10];
		feature temp;
		mtl_f.push_back(temp);
		err = fopen_s(&pfile, mFile, "r");
		if (err != 0)
		{
			printf("error to open %s\n", &mFile);
			return;
		}

		while (!feof(pfile))
		{
			fscanf(pfile, "%s", name);
			if (strcmp(name, "newmtl") == 0)
			{
				fscanf(pfile, "%s", buf);
				mtl[buf] = count;
				mtl_f.push_back(temp);
				count++;
			}
			else if (strcmp(name, "illum") == 0)
			{
				fscanf(pfile, "%f", &mtl_f[count-1].Illum);
			}
			else if (strcmp(name, "Kd") == 0)
			{
				fscanf(pfile, "%f %f %f", &mtl_f[count - 1].Kd[0], &mtl_f[count - 1].Kd[1], &mtl_f[count - 1].Kd[2]);
			}
			else if (strcmp(name, "Ka") == 0)
			{
				fscanf(pfile, "%f %f %f", &mtl_f[count - 1].Ka[0], &mtl_f[count - 1].Ka[1], &mtl_f[count - 1].Ka[2]);
			}
			else if (strcmp(name, "Tf") == 0)
			{
				fscanf(pfile, "%f %f %f", &mtl_f[count - 1].Tf[0], &mtl_f[count - 1].Tf[1], &mtl_f[count - 1].Tf[2]);
			}
			else if (strcmp(name, "Ni") == 0)
			{
				fscanf(pfile, "%f", &mtl_f[count - 1].Ni);
			}
			else if (strcmp(name, "Ks") == 0)
			{
				fscanf(pfile, "%f %f %f", &mtl_f[count - 1].Ks[0], &mtl_f[count - 1].Ks[1], &mtl_f[count - 1].Ks[2]);
			}
			else if (strcmp(name, "Ns") == 0)
			{
				fscanf(pfile, "%f", &mtl_f[count - 1].Ns);
			}
		}
		return;
	}
};
class object{
public:
	struct vec3{
		float x;
		float y;
		float z;
	};
	struct vec4{
		vec3 v;
		vec3 vt;
		vec3 vn;
		unsigned int mtl;
	};
	vector<vec4> face;
	int count;
	object(const char* obj){
		vector<vec3> v;
		vector<vec3> vt;
		vector<vec3> vn;
		unsigned int cur;
		char name[10], buf[100], vertice[20];
		int num, offset;
		material *now;
		vec3 vertex;
		vec4 temp;
		v.push_back(vertex);
		vt.push_back(vertex);
		vn.push_back(vertex);
		FILE *pfile;
		errno_t err;
		count = 0;
		err = fopen_s(&pfile, obj, "r");
		if (err != 0)
		{
			printf("error to open %s\n", &obj);
			return;
		}

		while (!feof(pfile))
		{
			fscanf(pfile, "%s", name);
			if (strcmp(name, "mtllib") == 0)
			{
				fscanf(pfile, "%s", buf);
				now = new material(buf);
			}
			else if (strcmp(name, "usemtl") == 0)
			{
				fscanf(pfile, "%s", buf);
				cur = now->mtl[buf];
			}
			else if (strcmp(name, "s") == 0)
			{
				//smoothy
			}
			else if (strcmp(name, "g") == 0)
			{
				fscanf(pfile, "%s", buf);
			}
			else if (strcmp(name, "v") == 0)
			{
				fscanf(pfile, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
				v.push_back(vertex);
			}
			else if (strcmp(name, "vt") == 0)
			{
				fscanf(pfile, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
				vt.push_back(vertex);
			}
			else if (strcmp(name, "vn") == 0)
			{
				fscanf(pfile, "%f %f", &vertex.x, &vertex.y);
				vn.push_back(vertex);
			}
			else if (strcmp(name, "f") == 0)
			{
				for (int i = 0; i < 2; i++)
				{
					offset = 0;
					fscanf(pfile, "%s", &vertice);
					for (int j = 0; vertice[j] != '\0'; j++)
					{
						if (vertice != "\\")
						{
							num = num * 10 + vertice[j] - '0';
						}
						else
						{
							if (offset == 0)	//v
							{
								temp.v = v[num];
							}
							else if (offset == 1)	//vt
							{
								temp.vt = vt[num];
							}
							offset++;
						}
					}
					//vn
					temp.vn = vn[num];
					temp.mtl = cur;
					face.push_back(temp);
				}
			}
			else if (strcmp(name, "#") == 0)
			{
				fgets(buf, 100, pfile);
			}

		}
	}

};