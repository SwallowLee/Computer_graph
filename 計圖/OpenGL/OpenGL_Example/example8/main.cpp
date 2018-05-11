#include <stdio.h>

#include "glew.h"
#include "glut.h"

int g_iViewport[4];

GLuint g_iFBO;
GLuint g_iRB;
GLuint g_iTexture;

void init();
void display();
void reshape(GLsizei , GLsizei );

void init()
{
	//generate texture
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &g_iTexture);
	glBindTexture(GL_TEXTURE_2D, g_iTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA_FLOAT32_ATI, 512, 512, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//generate render buffer
	glGenRenderbuffers(1, &g_iRB);
	glBindRenderbuffer(GL_RENDERBUFFER, g_iRB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 512, 512);

	//generate FBO
	glGenFramebuffers(1, &g_iFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, g_iFBO);

	//attach
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_iRB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_iTexture, 0);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	//Pass1
	{
		//render to texture
		glBindFramebuffer(GL_FRAMEBUFFER, g_iFBO);

		//clear texture and render buffer in FBO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

		glViewport(0, 0, 512, 512);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBegin(GL_QUADS);
		glColor3d(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glColor3d(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glColor3d(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glColor3d(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);
		glutSolidTeapot(0.5);
	}

	//Pass2
	{
		//render to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, 1.0, 0.5, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.5, 0.0, 50.0, 0.0, 50.0, 0.0, 1.0, 0.0);

		glViewport(g_iViewport[0], g_iViewport[1], g_iViewport[2], g_iViewport[3]);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_iTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-50.0, 0.0, 0.0);
			glTexCoord2f(100.0, 0.0);
			glVertex3f(-50.0, 0.0, 100.0);
			glTexCoord2f(100.0, 100.0);
			glVertex3f(50.0, 0.0, 100.0);
			glTexCoord2f(0.0, 100.0);
			glVertex3f(50.0, 0.0, 0.0);
		glEnd();
	}

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	g_iViewport[0] = 0;
	g_iViewport[1] = 0;
	g_iViewport[2] = w;
	g_iViewport[3] = h;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("FBO");

	glewInit();
	init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
