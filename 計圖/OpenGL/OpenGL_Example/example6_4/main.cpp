#include "textureLoader.h"

#include "glew.h"
#include "glut.h"

TextureLoader* pTexture;

void GL_display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	pTexture->Bind();
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
}

void GL_reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.5, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.5, 0.0, 50.0, 0.0, 50.0, 0.0, 1.0, 0.0);
}

void GL_keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:
		delete pTexture;
		exit(0);
	case 'm': //事後建立mipmap
		pTexture->GenerateMipmap();
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Texture");
	
	glewInit();

	pTexture = new TextureLoader();
	pTexture->Load("check.bmp");

	glutDisplayFunc(GL_display);
	glutReshapeFunc(GL_reshape);
	glutKeyboardFunc(GL_keyboard);
	glutMainLoop();
    return 0;
}


