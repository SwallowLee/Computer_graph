#ifdef UNICODE
#undef UNICODE
#endif

#include "glaux.h"
#include "glut.h"

GLfloat fcolor[4]={1.0, 1.0, 1.0, 1.0};
AUX_RGBImageRec * img;	
GLuint texObject[1];

void LoadTexture(char* filename)
{
	img = auxDIBImageLoad(filename);
	glGenTextures(1, texObject);
	glBindTexture(GL_TEXTURE_2D, texObject[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB,GL_UNSIGNED_BYTE, img->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display()
{
	glEnable(GL_FOG);
	glFogf(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.5);
	glFogfv(GL_FOG_COLOR, fcolor);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texObject[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-50.0, 0.0, 0.0);
	glTexCoord2f(0.0, 100.0);
	glVertex3f(-50.0, 0.0, 100.0);
	glTexCoord2f(100.0, 100.0);
	glVertex3f(50.0, 0.0, 100.0);
	glTexCoord2f(100.0, 0.0);
	glVertex3f(50.0, 0.0, 0.0);
	glEnd();
	glFlush();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, 500, 500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1 / 1, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.5, 0.0, 50.0, 0.0, 50.0, 0.0, 1.0, 0.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutCreateWindow("Blending");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	LoadTexture("check.bmp");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
