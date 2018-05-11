#include "mesh.h"
#include "glut.h"
#include "view.h"
#include "light.h"
#include "scene.h"
#include <math.h>

#define PI 3.1415926585897
void display();
void reshape(GLsizei, GLsizei);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void lighting();	// call in display function
void click(int, int);

view *set_v;
light *set_light;
scene *set_scene;
//mesh *object[10];
int width, height, select_obj = -1;
float angle = 0, distance = 1;
int initial_x, initial_y;
int now_x, now_y;
int move[2];

int main(int argc, char** argv)
{
	//open view.view ------------------------use in desplay function
	set_v = new view("scene2.view");
	//open scene.scene && box.obj bunny.obj venus.obj && box.mtl bunny.mtl venus.mtl----------------------use in display function
	set_scene = new scene("scene2.scene");
	//printf("%Iu %Iu %Iu", set_scene->object[0]->fTotal, set_scene->object[1]->fTotal, set_scene->object[2]->fTotal);
	//open light.light ----------------------use in lighting
	set_light = new light("scene2.light");

	glutInit(&argc, argv);
	glutInitWindowSize(set_v->viewport[2], set_v->viewport[3]);
	glutInitWindowPosition(set_v->viewport[0], set_v->viewport[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HW1_2");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
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
	tempx = set_v->eye[0];
	tempz = set_v->eye[2];
	switch (ch)
	{
	case 'w':
		printf("zoom in\n");
		set_v->eye[0] += (set_v->vat[0] - set_v->eye[0]) / 10;
		set_v->eye[1] += (set_v->vat[1] - set_v->eye[1]) / 10;
		set_v->eye[2] += (set_v->vat[2] - set_v->eye[2]) / 10;
		//distance += 5.0;
		break;
	case 's':
		printf("zoom out\n");
		set_v->eye[0] -= (set_v->vat[0] - set_v->eye[0]) / 10;
		set_v->eye[1] -= (set_v->vat[1] - set_v->eye[1]) / 10;
		set_v->eye[2] -= (set_v->vat[2] - set_v->eye[2]) / 10;
		//distance -= 5.0;
		break;
	case 'a':
		printf("left rotate\n");
		set_v->eye[0] = tempx * cos((10 * PI) / 180.0) - tempz * sin((10 * PI) / 180.0);
		set_v->eye[2] = tempx * sin((10 * PI) / 180.0) + tempz * cos((10 * PI) / 180.0);
		//angle -= 30;
		//printf("%f\n", angle);
		break;
	case 'd':
		printf("right rotate\n");
		set_v->eye[0] = tempx * cos((-10 * PI) / 180.0) - tempz * sin((-10 * PI) / 180.0);
		set_v->eye[2] = tempx * sin((-10 * PI) / 180.0) + tempz * cos((-10 * PI) / 180.0);
		//angle += 30;
		//printf("%f\n", angle);
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
			/*//normalize
			float A = set_v->eye[0] * set_v->eye[0];
			float B = set_v->eye[1] * set_v->eye[1];
			float C = set_v->eye[2] * set_v->eye[2];
			float D = A + B + C;
			A = A / D;
			B = B / D;
			C = C / D;
			glTranslatef(A * distance, B * distance, C * distance);*/
			//glRotatef(angle, set_scene->rotate[i][0], set_scene->rotate[i][1], set_scene->rotate[i][2]);
			glTranslatef(set_scene->transfer[i][0], set_scene->transfer[i][1], set_scene->transfer[i][2]);
			glRotatef(set_scene->angle[i], set_scene->rotate[i][0], set_scene->rotate[i][1], set_scene->rotate[i][2]);
			glScalef(set_scene->scale[i][0], set_scene->scale[i][1], set_scene->scale[i][2]);

			glBegin(GL_TRIANGLES);
			for (size_t k = 0; k<3; ++k)
			{
				//textex corrd. object->tList[object->faceList[i][j].t].ptr
				glNormal3fv(set_scene->object[i]->nList[set_scene->object[i]->faceList[j][k].n].ptr);
				glVertex3fv(set_scene->object[i]->vList[set_scene->object[i]->faceList[j][k].v].ptr);
			}
			glEnd();
			glPopMatrix();
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
		GLfloat light_position[] = { set_light->x[i], set_light->y[i], set_light->z[i], 1.0 };

		//light property
		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light_ambient);
	}

	// environment ambient
	GLfloat ambient[] = { set_light->envir[0], set_light->envir[1], set_light->envir[2] };

}
