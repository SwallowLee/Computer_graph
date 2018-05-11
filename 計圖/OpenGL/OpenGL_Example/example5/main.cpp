#include "glut.h"
#include <time.h>

static GLfloat year=0.0f, day=0.0f;
int time1,time2;
int width, height;
void display();
void reshape(GLsizei , GLsizei );
void idle();
void keyboard(unsigned char , int, int);

void display()
{
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      // �]�w�M��color������
	glClearDepth(1.0f);                        // �]�wDepth Buffer (�N�Oz buffer) �M����
	glEnable(GL_DEPTH_TEST);                   // �Ұʲ`�״���
	glDepthFunc(GL_LEQUAL);                    // �i�H�]�w��less or equal�~��s�e��,�Y�O�]�w��GL_LESS�O�u��less�~��s�e��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�o����e���M������åB�M��z buffer

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 3.0, 5.0,    // eye
			  0.0, 0.0, 0.0,    // center
			  0.0, 1.0, 0.0);   // up

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width/height, 1.0, 20.0);

	// viewport transformation
	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);//�`�N�o�@��,�אּ�]�wmodel view
	glColor3f(1.0, 1.0, 1.0);

	glutWireSphere(1.0, 20, 16);// the Sun 

	glPushMatrix();//�x�s�{�b���x�} (�ثe�Omodelview)
		glRotatef(year, 0.0, 1.0, 0.0);
		glTranslatef(3.0, 0.0, 0.0);
		glRotatef(day, 0.0, 1.0, 0.0);
		glutWireSphere(0.5, 10, 8);    // the Planet, ���ɪ��x�}�O I * ModelView * R * T * R
	glPopMatrix();//��stack�����x�}����w�x�}(modelview),�|�^�� I * ModelView

	glPushMatrix();
		glRotatef(year*2.0, 0.0, 1.0, 0.0);
		glTranslatef(1.5, 0.0, 0.0);
		glRotatef(day*2.0, 0.0, 1.0, 0.0);
		glutWireSphere(0.2, 10, 8);    // the Planet 2
	glPopMatrix();

	// swap the front and back buffers
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	width = w;
	height = h;
}

// GLUT idle function
void idle()
{
	time2 = clock();
	day += 100.0*(time2-time1)/CLK_TCK; //�{�b���ήɶ�����t��
	if(day > 360.0)
		day -= 360.0;
	year += 10.0*(time2-time1)/CLK_TCK;
	if(year > 360.0)
		year -= 360.0;

	time1 = time2;

	// recall GL_display() function
	glutPostRedisplay();
}

// GLUT keyboard function
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'd':    day += 10.0;
		if(day > 360.0)
			day -= 360.0;
		glutPostRedisplay();
		break;
	case 'y':    year += 1.0;
		if(year > 360.0)
			year -= 360.0;
		glutPostRedisplay();
		break;
	case 'a':    // assign idle function
		time1 = clock();
		glutIdleFunc(idle); //���Uidle function
		break;
	case 'A':    glutIdleFunc(NULL);//����idle function
		break;
	case 27:     exit(0);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Planet");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
