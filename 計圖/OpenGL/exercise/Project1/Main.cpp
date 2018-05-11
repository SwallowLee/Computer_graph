#include "GL/glut.h"
#include <time.h>

static GLfloat year = 0.0f, day = 0.0f;
int time1, time2;
int width, height;
void display();
void reshape(GLsizei, GLsizei);
void idle();
void keyboard(unsigned char, int, int);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Planet");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void display()
{
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 3.0, 5.0,	// eye
			  0.0, 0.0, 0.0,	// center
			  0.0, 1.0, 0.0);	// up vector

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width / height, 1.0, 20.0);

	// viewport transformation
	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0, 1.0, 1.0);

	glutWireSphere(1.0, 20, 16);		// the Sun

	glPushMatrix();
		glRotatef(year, 0.0, 1.0, 0.0);
		glTranslatef(3.0, 0.0, 0.0);
		glRotatef(day, 0.0, 1.0, 0.0);
		glutWireSphere(0.5, 10, 8);
	glPopMatrix();

	glPushMatrix();
		glRotatef(year*2.0, 0.0, 1.0, 0.0);
		glTranslatef(1.5, 0.0, 0.0);
		glRotatef(day*2.0, 0.0, 1.0, 0.0);
		glutWireSphere(0.2, 10, 8);
	glPopMatrix();

	// swap the front and back buffers
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	width = w;
	height = h;
}

void idle()
{
	time2 = clock();
	day += 100.0*(time2 - time1) / CLK_TCK;
	if (day > 360.0)
		day -= 360.0;
	year += 10.0*(time2 - time1) / CLK_TCK;
	if (year > 360.0)
		year -= 360.0;

	time1 = time2;

	// recall GL_display() function
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'd': day += 10.0;
		if (day > 360.0)
			day -= 360.0;
		glutPostRedisplay();
		break;
	case 'y': year += 1.0;
		if (year > 360.0)
			year -= 360.0;
		glutPostRedisplay();
		break;
	case 'a': 
		time1 = clock();
		glutIdleFunc(idle);
		break;
	case 'A': 
		glutIdleFunc(NULL);
		break;
	case 27: exit(0);
	}
}
