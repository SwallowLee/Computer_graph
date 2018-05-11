#include "glut.h"
void display();
void reshape(GLsizei w, GLsizei h);
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Drawing sample");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
	return 0;
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);//�����v,���`��,�|���h����P
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() //���d�ҵLlighting,�������C��
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);//�o����e���M���¦�
    glBegin(GL_POLYGON);//------------------------------------------BEGIN �U��@4�ճ��I
        glColor3f(1.0f, 1.0f, 1.0f);//�Ĥ@�I���C��:��
        glVertex3f(-1.0f, -1.0f, 0.0f);//�Ĥ@�I���y��:���U,z�b 0 

        glColor3f(1.0f, 0.0f, 0.0f);//��
        glVertex3f(1.0f, -1.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);//��
        glVertex3f(1.0f, 1.0f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);//��
        glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();//--------------------------------------------------------END
    glFlush();//��X
}
