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
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);//平行投影,不常用,會失去遠近感
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() //此範例無lighting,直接給顏色
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);//這兩行把畫面清成黑色
    glBegin(GL_POLYGON);//------------------------------------------BEGIN 下方共4組頂點
        glColor3f(1.0f, 1.0f, 1.0f);//第一點的顏色:白
        glVertex3f(-1.0f, -1.0f, 0.0f);//第一點的座標:左下,z軸 0 

        glColor3f(1.0f, 0.0f, 0.0f);//紅
        glVertex3f(1.0f, -1.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);//綠
        glVertex3f(1.0f, 1.0f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);//藍
        glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();//--------------------------------------------------------END
    glFlush();//輸出
}
