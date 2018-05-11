#include "../SharedCode/ShaderLoader.h"
#include "../SharedCode/mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "view.h"
#include "lights.h"
#include "scenes.h"
#include "freeImage.h"
#include <iostream>
using namespace std;

unsigned int *textures;
mesh **object;
view *view_data;
lights *light_data;
scenes *scene_data;

int windowSize[2];

float zoom_base = 0.0f;
float zoom_d = 1.0f;
float rotate_base = 0.0f;
float rotate_d = 5.0f;
int select = -1;
float originX = 0.0f;
float originY = 0.0f;
int startX;
int startY;

GLhandleARB MyShader;

int WinW,WinH;

void LoadShaders();
void light();
void display();
void reshape(GLsizei, GLsizei);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void LoadTexture(string pFilename, int tex_index);

int main(int argc, char** argv)
{
	view_data = new view("as3.view");
	light_data = new lights("as3.light");
	scene_data = new scenes("as3.scene");
	object = new mesh*[scene_data->total_obj];
	for (int i = 0; i < scene_data->total_obj; i++)
		object[i] = new mesh(scene_data->scene_list[i].obj_file_name);
	textures = new unsigned[scene_data->total_tex];

	glutInit(&argc, argv);
	glutInitWindowSize(view_data->viewport[2], view_data->viewport[3]);
	glutInitWindowPosition(view_data->viewport[0], view_data->viewport[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Mesh Example");
	glewInit();
	FreeImage_Initialise();
	glGenTextures(scene_data->total_tex, textures);
	for (int i = 0; i < scene_data->total_tex; i++)
	{
		LoadTexture(scene_data->tex_name[i], i);
	}
	FreeImage_DeInitialise();

	GLenum glew_error;
	if((glew_error = glewInit()) != GLEW_OK)return -1;

	LoadShaders();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();

	return 0;
}

void LoadShaders()
{
	MyShader = glCreateProgram();
	if(MyShader != 0)
	{
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.vert", GL_VERTEX_SHADER);
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.geom", GL_GEOMETRY_SHADER);
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.frag", GL_FRAGMENT_SHADER);
	}
}

void light()
{
	GLfloat light_specular[8][4];
	GLfloat light_diffuse[8][4];
	GLfloat light_ambient[8][4];
	GLfloat light_position[8][4];
	for (int i = 0; i < light_data->total_light; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			light_specular[i][j] = light_data->light_list[i].s[j];
			light_diffuse[i][j] = light_data->light_list[i].d[j];
			light_ambient[i][j] = light_data->light_list[i].a[j];
			light_position[i][j] = light_data->light_list[i].light_pos[j];
		}
		light_specular[i][3] = 1.0f;
		light_diffuse[i][3] = 1.0f;
		light_ambient[i][3] = 1.0f;
		light_position[i][3] = 1.0f;
	}
	GLfloat envi_ambient[4];
	for (int i = 0; i < 3; i++)
		envi_ambient[i] = light_data->amb[i];
	envi_ambient[3] = 1.0f;


	glShadeModel(GL_SMOOTH);

	// z buffer enable
	glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property

	int GL_lightN = GL_LIGHT0;
	for (int i = 0; i < light_data->total_light; i++)
	{
		glEnable(GL_lightN + i);
		glLightfv(GL_lightN + i, GL_POSITION, light_position[i]);
		glLightfv(GL_lightN + i, GL_DIFFUSE, light_diffuse[i]);
		glLightfv(GL_lightN + i, GL_SPECULAR, light_specular[i]);
		glLightfv(GL_lightN + i, GL_AMBIENT, light_ambient[i]);
	}
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, envi_ambient);

}

void display()
{
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(view_data->fovy, (GLfloat)view_data->viewport[2] / (GLfloat)view_data->viewport[3], view_data->dnear, view_data->dfar);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(view_data->eye[0], view_data->eye[1], view_data->eye[2],	// eye
		view_data->vat[0], view_data->vat[1], view_data->vat[2],	// center
		view_data->vup[0], view_data->vup[1], view_data->vup[2]);	// up

	//decide zoom-in zoom-out direction
	float unit_x = (view_data->eye[0] - view_data->vat[0]) * (view_data->eye[0] - view_data->vat[0]);
	float unit_y = (view_data->eye[1] - view_data->vat[1]) * (view_data->eye[1] - view_data->vat[1]);
	float unit_z = (view_data->eye[2] - view_data->vat[2]) * (view_data->eye[2] - view_data->vat[2]);
	float unit = unit_x + unit_y + unit_z;
	unit_x = sqrt(unit_x / unit);
	unit_y = sqrt(unit_y / unit);
	unit_z = sqrt(unit_z / unit);

	glTranslatef(unit_x * zoom_base, unit_y * zoom_base, unit_z * zoom_base);
	glTranslatef(scene_data->scene_list[0].t[0], scene_data->scene_list[0].t[1], scene_data->scene_list[0].t[2]);
	glRotatef(rotate_base, 0.0, 1.0, 0.0);
	glTranslatef(-scene_data->scene_list[0].t[0], -scene_data->scene_list[0].t[1], -scene_data->scene_list[0].t[2]);


	//注意light位置的設定，要在gluLookAt之後
	light();

	glUseProgram(MyShader);

	for (int k = 0; k < scene_data->total_obj; k++)
	{
		int lastMaterial = -1;
		glPushMatrix();
		glTranslatef(scene_data->scene_list[k].t[0], scene_data->scene_list[k].t[1], scene_data->scene_list[k].t[2]);
		glRotatef(scene_data->scene_list[k].Angle, scene_data->scene_list[k].r[0], scene_data->scene_list[k].r[1], scene_data->scene_list[k].r[2]);
		glScalef(scene_data->scene_list[k].s[0], scene_data->scene_list[k].s[1], scene_data->scene_list[k].s[2]);

		//bind textures
		if (scene_data->scene_list[k].tex_method == 1)
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[scene_data->scene_list[k].tex_ID]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
		else if (scene_data->scene_list[k].tex_method == 2)
		{
			for (int j = 0; j < 2; j++)
			{
				glActiveTexture(GL_TEXTURE0 + j);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textures[scene_data->scene_list[k].tex_ID + j]);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			}
		}
		else if (scene_data->scene_list[k].tex_method == 6)
		{
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			glEnable(GL_TEXTURE_CUBE_MAP);
			//for (int j = 0; j < 6; j++)
			//	glBindTexture(GL_TEXTURE_2D, textures[scene_data->scene_list[k].tex_ID + j]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}

		for (size_t i = 0; i < object[k]->fTotal; ++i)
		{
			// set material property if this face used different material
			if (lastMaterial != object[k]->faceList[i].m)
			{
				lastMaterial = (int)object[k]->faceList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT, object[k]->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, object[k]->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, object[k]->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &object[k]->mList[lastMaterial].Ns);

				//you can obtain the texture name by object->mList[lastMaterial].map_Kd
				//load them once in the main function before mainloop
				//bind them in display function here
			}

			glBegin(GL_TRIANGLES);
			for (size_t j = 0; j < 3; ++j)
			{
				//textex corrd. object->tList[object->faceList[i][j].t].ptr
				if (scene_data->scene_list[k].tex_method == 1)
					glTexCoord2fv(object[k]->tList[object[k]->faceList[i][j].t].ptr);
				else if (scene_data->scene_list[k].tex_method == 2)
				{
					glMultiTexCoord2fv(GL_TEXTURE0, object[k]->tList[object[k]->faceList[i][j].t].ptr);
					glMultiTexCoord2fv(GL_TEXTURE1, object[k]->tList[object[k]->faceList[i][j].t].ptr);
				}
				glNormal3fv(object[k]->nList[object[k]->faceList[i][j].n].ptr);
				glVertex3fv(object[k]->vList[object[k]->faceList[i][j].v].ptr);
			}
			glEnd();
		}

		//unbind textures
		if (scene_data->scene_list[k].tex_method == 1)
		{
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (scene_data->scene_list[k].tex_method == 2)
		{
			for (int j = 1; j >= 0; j--)
			{
				glActiveTexture(GL_TEXTURE0 + j);
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		else if (scene_data->scene_list[k].tex_method == 6)
		{
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_CUBE_MAP);
		}

		glPopMatrix();
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':	zoom_base += zoom_d;
		break;
	case 'a':	rotate_base += rotate_d;
		break;
	case 's':	zoom_base -= zoom_d;
		break;
	case 'd':	rotate_base -= rotate_d;
		break;
	default:
		break;
	}
	if (key >= '1' && key <= '9')
	{
		select = key - '1';
		if (select < scene_data->total_obj)
			cout << "selected " << select + 1 << "th object" << endl;
		else
		{
			select = -1;
			cout << "no such object" << endl;
		}

	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		cout << "ClickX: " << x << "   ClickY: " << y << endl;
		startX = x;
		startY = y;
		if (select >= 0)
		{
			originX = scene_data->scene_list[select].t[0];
			originY = scene_data->scene_list[select].t[1];
		}
	}
	else if (state == GLUT_UP)
	{
		cout << "MouseX: " << x << "   MouseY: " << y << endl;
	}
}

void motion(int x, int y)
{
	if (select >= 0)
	{
		scene_data->scene_list[select].t[0] = originX + x - startX;
		scene_data->scene_list[select].t[1] = originY - (y - startY);
		glutPostRedisplay();
	}
}

void LoadTexture(string pFilename, int tex_index)
{
	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pFilename.c_str(), 0), pFilename.c_str());
	FIBITMAP *p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);

	int bind_target, tex2D_target;
	if (scene_data->cube_index[tex_index] == -1)
	{
		bind_target = GL_TEXTURE_2D;
		tex2D_target = GL_TEXTURE_2D;
	}
	else
	{
		bind_target = GL_TEXTURE_CUBE_MAP;
		if (scene_data->cube_index[tex_index] == 0)
			tex2D_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		else if (scene_data->cube_index[tex_index] == 1)
		{
			tex2D_target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			tex_index -= 1;
		}
		else if (scene_data->cube_index[tex_index] == 2)
		{
			tex2D_target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			tex_index -= 2;
		}
		else if (scene_data->cube_index[tex_index] == 3)
		{
			tex2D_target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			tex_index -= 3;
		}
		else if (scene_data->cube_index[tex_index] == 4)
		{
			tex2D_target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			tex_index -= 4;
		}
		else if (scene_data->cube_index[tex_index] == 5)
		{
			tex2D_target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
			tex_index -= 5;
		}
	}

	glBindTexture(bind_target, textures[tex_index]);
	glTexImage2D(tex2D_target, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	glGenerateMipmap(bind_target);
	glTexParameteri(bind_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(bind_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
}