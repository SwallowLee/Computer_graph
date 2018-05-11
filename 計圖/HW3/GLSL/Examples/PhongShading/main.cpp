#include "../SharedCode/ShaderLoader.h"
#include "../SharedCode/mesh.h"
#include <GL/glew.h>
#include "mesh.h"
#include "glut.h"
#include "view.h"
#include "light.h"
#include "scene.h"
#include "texture.h"
#include <math.h>

#define PI 3.1415926585897

GLhandleARB MyShader;
void LoadShaders();
void display();
void reshape(GLsizei, GLsizei);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void lighting();	// call in display function
void click(int, int);
void LoadTexture();

int light_distance = 0;
view *set_v;
light *set_light;
scene *set_scene;
texture *set_tex[100];
//mesh *object[10];
int width, height, select_obj = -1;
int initial_x, initial_y;
int now_x, now_y;
int move[2];

int main(int argc, char** argv)
{
	//open view.view ------------------------use in desplay function
	set_v = new view("as3.view");
	//open scene.scene && box.obj bunny.obj venus.obj && box.mtl bunny.mtl venus.mtl----------------------use in display function
	set_scene = new scene("as3.scene");
	//printf("%Iu %Iu %Iu", set_scene->object[0]->fTotal, set_scene->object[1]->fTotal, set_scene->object[2]->fTotal);
	//open light.light ----------------------use in lighting
	set_light = new light("as3.light");

	glutInit(&argc, argv);
	glutInitWindowSize(set_v->viewport[2], set_v->viewport[3]);
	glutInitWindowPosition(set_v->viewport[0], set_v->viewport[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HW1_2");
	auto result = glewInit();
	if (result != GLEW_OK){
		std::cerr << glewGetErrorString(result);
		std::exit(0);
	}
	LoadTexture();
	LoadShaders();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
}

void LoadShaders()
{
	MyShader = glCreateProgram();
	if (MyShader != 0)
	{
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.vert", GL_VERTEX_SHADER);
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.geom", GL_GEOMETRY_SHADER);
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.frag", GL_FRAGMENT_SHADER);
	}
}

void mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON)
	if (state == GLUT_DOWN){
		initial_x = x;
		initial_y = y;
	}
	if (state == GLUT_UP){
		now_x = x;
		now_y = y;
		if (select_obj != -1){
			set_scene->transfer[select_obj][0] += (now_x - initial_x) / 40.0;
			set_scene->transfer[select_obj][1] += (initial_y - now_y) / 40.0;
		}
		glutPostRedisplay();
	}
}

void keyboard(unsigned char ch, int x, int y){
	float tempx, tempz;
	tempx = set_v->eye[0] - set_v->vat[0];
	tempz = set_v->eye[2] - set_v->vat[2];
	switch (ch)
	{
	case 'w':
		printf("zoom in\n");
		set_v->eye[0] += (set_v->vat[0] - set_v->eye[0]) / 10;
		set_v->eye[1] += (set_v->vat[1] - set_v->eye[1]) / 10;
		set_v->eye[2] += (set_v->vat[2] - set_v->eye[2]) / 10;
		break;
	case 's':
		printf("zoom out\n");
		set_v->eye[0] -= (set_v->vat[0] - set_v->eye[0]) / 10;
		set_v->eye[1] -= (set_v->vat[1] - set_v->eye[1]) / 10;
		set_v->eye[2] -= (set_v->vat[2] - set_v->eye[2]) / 10;
		break;
	case 'a':
		printf("left rotate\n");
		set_v->eye[0] = tempx * cos((-10 * PI) / 180.0) - tempz * sin((-10 * PI) / 180.0);
		set_v->eye[2] = tempx * sin((-10 * PI) / 180.0) + tempz * cos((-10 * PI) / 180.0);
		set_v->eye[0] += set_v->vat[0];
		set_v->eye[2] += set_v->vat[2];
		break;
	case 'd':
		printf("right rotate\n");
		set_v->eye[0] = tempx * cos((10 * PI) / 180.0) - tempz * sin((10 * PI) / 180.0);
		set_v->eye[2] = tempx * sin((10 * PI) / 180.0) + tempz * cos((10 * PI) / 180.0);
		set_v->eye[0] += set_v->vat[0];
		set_v->eye[2] += set_v->vat[2];
		break;
	case 'f':
		printf("light down\n");
		light_distance -= 5;
		break;
	case 'r':
		printf("light up\n");
		light_distance += 5;
		break;
	}
	if (ch >= '1' && ch <= '9')
	if (ch - '1' < set_scene->count)
		select_obj = ch - '1';
	else
		select_obj = -1;
	glutPostRedisplay();
}

void display(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//viewport transformation
	glViewport(set_v->viewport[0], set_v->viewport[1], width, height);
	//projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(set_v->fovy, (float)width / height, set_v->dnear, set_v->dfar);

	//viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(set_v->eye[0], set_v->eye[1], set_v->eye[2],
		set_v->vat[0], set_v->vat[1], set_v->vat[2],
		set_v->vup[0], set_v->vup[1], set_v->vup[2]);

	lighting();
	glUseProgram(MyShader);
	GLint location = glGetUniformLocation(MyShader, "colorTexture");
	if (location == -1)
		printf("Cant find texture name: colorTexture\n");
	else
		glUniform1i(location, 0);

	for (int i = 0; i < set_scene->count; i++)
	{
		int current = -1;
		for (size_t j = 0; j < set_scene->object[i]->fTotal; j++)
		{
			glPushMatrix();
			if (current != set_scene->object[i]->faceList[j].m)
			{
				current = set_scene->object[i]->faceList[j].m;
				glMaterialfv(GL_FRONT, GL_DIFFUSE, set_scene->object[i]->mList[current].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, set_scene->object[i]->mList[current].Ks);
				glMaterialfv(GL_FRONT, GL_AMBIENT, set_scene->object[i]->mList[current].Ka);
				glMaterialfv(GL_FRONT, GL_SHININESS, &set_scene->object[i]->mList[current].Ns);
			}
			glTranslatef(set_scene->transfer[i][0], set_scene->transfer[i][1], set_scene->transfer[i][2]);
			glRotatef(set_scene->angle[i], set_scene->rotate[i][0], set_scene->rotate[i][1], set_scene->rotate[i][2]);
			glScalef(set_scene->scale[i][0], set_scene->scale[i][1], set_scene->scale[i][2]);


			//texture mapping
			if (set_scene->obj_tex_type[i] == 0){
				//no_texture
			}
			else if (set_scene->obj_tex_type[i] == 1){
				//single_texture
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, set_tex[set_scene->obj_tex_index[i]]->texObject[0]);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			}
			else if (set_scene->obj_tex_type[i] == 2){
				//multi_texture

				//texture 0
				glActiveTexture(GL_TEXTURE0);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, set_tex[set_scene->obj_tex_index[i] - 1]->texObject[0]);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				//texture 1
				glActiveTexture(GL_TEXTURE1);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, set_tex[set_scene->obj_tex_index[i]]->texObject[0]);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			}
			else if (set_scene->obj_tex_type[i] == 3){
				//cube
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
				glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glEnable(GL_TEXTURE_GEN_R);
				glEnable(GL_TEXTURE_CUBE_MAP);
				glBindTexture(GL_TEXTURE_CUBE_MAP, set_tex[set_scene->obj_tex_index[i]]->texObject[0]);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			}

			glBegin(GL_TRIANGLES);
			for (size_t k = 0; k<3; ++k)
			{
				if (set_scene->obj_tex_type[i] == 0){
					//no_texture
				}
				else if (set_scene->obj_tex_type[i] == 1){
					//single_texture
					float* point = set_scene->object[i]->tList[set_scene->object[i]->faceList[j][k].t].ptr;
					glTexCoord2f(point[0], point[1]);
				}
				else if (set_scene->obj_tex_type[i] == 2){
					//multi_texture
					float* point = set_scene->object[i]->tList[set_scene->object[i]->faceList[j][k].t].ptr;
					glMultiTexCoord2f(GL_TEXTURE0, point[0], point[1]);
					glMultiTexCoord2f(GL_TEXTURE1, point[0], point[1]);
				}
				else if (set_scene->obj_tex_type[i] == 3){
					//cube
					float* point = set_scene->object[i]->tList[set_scene->object[i]->faceList[j][k].t].ptr;
					glTexCoord2f(point[0], point[1]);
				}
				glNormal3fv(set_scene->object[i]->nList[set_scene->object[i]->faceList[j][k].n].ptr);
				glVertex3fv(set_scene->object[i]->vList[set_scene->object[i]->faceList[j][k].v].ptr);
			}
			glEnd();
			glPopMatrix();

			if (set_scene->obj_tex_type[i] == 0){
				//no_texture
			}
			else if (set_scene->obj_tex_type[i] == 1){
				//single_texture
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else if (set_scene->obj_tex_type[i] == 2){
				//multi_texture
				//texture 1
				glActiveTexture(GL_TEXTURE1);
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
				//texture 0
				glActiveTexture(GL_TEXTURE0);
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

			}
			else if (set_scene->obj_tex_type[i] == 3){
				//cube
				glDisable(GL_TEXTURE_CUBE_MAP);
				glDisable(GL_TEXTURE_GEN_R);
				glDisable(GL_TEXTURE_GEN_T);
				glDisable(GL_TEXTURE_GEN_S);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}
		}
	}
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h){
	width = w;
	height = h;
}

void lighting(){
	int i;
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	for (i = 0; i < set_light->count; i++)
	{
		GLfloat light_diffuse[] = { set_light->diffuse[i][0], set_light->diffuse[i][1], set_light->diffuse[i][2], 1.0 };
		GLfloat light_specular[] = { set_light->specular[i][0], set_light->specular[i][1], set_light->specular[i][2], 1.0 };
		GLfloat light_ambient[] = { set_light->ambient[i][0], set_light->ambient[i][1], set_light->ambient[i][2], 1.0 };
		GLfloat light_position[] = { set_light->x[i], set_light->y[i] + light_distance, set_light->z[i], 1.0 };

		//light property
		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light_ambient);
	}

	// environment ambient
	glEnable(GL_LIGHT_MODEL_AMBIENT);
	GLfloat ambient[] = { set_light->envir[0], set_light->envir[1], set_light->envir[2] };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}


void LoadTexture(){
	for (int i = 0; i < set_scene->tex_counter; i++){
		//printf("%d %d %d %s\n", set_scene->tex_counter, i, set_scene->tex_type[i], set_scene->tex_name[i][0]);
		if (set_scene->tex_type[i] == 1 || set_scene->tex_type[i] == 2){
			set_tex[i] = new texture();
			set_tex[i]->LoadTexture(set_scene->tex_name[i][0]);
			printf("Load %s\n", set_scene->tex_name[i]);
		}
		else if (set_scene->tex_type[i] == 3){ //cube load
			set_tex[i] = new texture();
			set_tex[i]->LoadEnvirTexture(set_scene->tex_name[i][0], set_scene->tex_name[i][1], set_scene->tex_name[i][2], set_scene->tex_name[i][3], set_scene->tex_name[i][4], set_scene->tex_name[i][5]);
			printf("Load CUBE %s\n", set_scene->tex_name[i][4]);
		}
	}
}