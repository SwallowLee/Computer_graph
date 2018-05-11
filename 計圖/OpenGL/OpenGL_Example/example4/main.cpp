#include "glut.h"
void display();
void reshape(int, int);
void lighting();
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Planet");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

void lighting()
{
    GLfloat mat_diffuse[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat mat_specular[] = {0.0f, 1.0f, 0.0f, 1.0f};
    GLfloat mat_ambient[] = {0.0f, 0.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {10.0f};

    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat light_position[] = {5.0f, 5.0f, 10.0f, 0.0f};

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//設定底色
    glShadeModel(GL_SMOOTH);

    // z buffer enable
    glEnable(GL_DEPTH_TEST);

    // enable lighting
    glEnable(GL_LIGHTING);
    // set light property
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);

    // set material property
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void display()
{
	lighting();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除底色和 z buffer
    glutSolidSphere(0.8, 128, 128);
    glFlush();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   glOrtho (-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
