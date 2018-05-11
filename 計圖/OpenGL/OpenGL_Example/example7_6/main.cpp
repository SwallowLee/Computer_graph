/*�Q��stencil buffer �@�B��,�e�⦸�@�X��W����l�ĪG*/
#include <math.h>
#include <time.h>
#include "glut.h"

#define pass1 1
#define pass2 2

//Global
int pass;

GLfloat eye_xyz[3]={100.0,200.0,500.0};
GLfloat at_xyz[3]={0.0,0.0,0.0};
GLfloat up_xyz[3]={0.0,1.0,0.0};

int state=1;
int time1,time2;

//����
GLfloat mat_diffuse[] = {0.2, 0.2, 0.2, 1.0};
GLfloat mat_specular[] = {0.2, 0.2, 0.2, 1.0};
GLfloat mat_ambient[] = {0.4, 0.4, 0.4, 1.0};
GLfloat mat_shininess[] = {100.0};

//���C��
GLfloat light_specular[] = {0.2, 0.2, 0.2, 1.0};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[] = {0.7, 0.7, 0.7, 1.0};
GLfloat light_position[] = {0.0, 40.0, 40.0, 1.0};

//function
void init();
void display();
void reshape(GLsizei w, GLsizei h);
void keyboard(unsigned char key, int x, int y);
void idle();
void draw();//call by display

//main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL); //with STENCIL buffer
    glutInitWindowSize(700,700);//�]�w�����j�p
    glutInitWindowPosition(0,0);
    glutCreateWindow("��W����l");//�������D

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
	return 0;
}

void init() //�u�O�]�wlight
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void display()
{
    glEnable(GL_CULL_FACE);                    // enable back face cutting
    glEnable(GL_STENCIL_TEST);                 //�|�Ψ�stencil buffer
    glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //�]�w�M�����C��
    glClearStencil(1);                         //�]�wstencil buffer �M���� = 1
    glClearDepth(1.0f);                        // Depth Buffer (�N�Oz buffer) Setup
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//����M��


    //�Ĥ@���e�X,stencil buffer �O1���ϰ�
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 5.0f, 1000.0f);//left right bottom top near_distance far_distance
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_xyz[0], eye_xyz[1] , eye_xyz[2] ,
              at_xyz[0] , at_xyz[1]  , at_xyz[2]  ,
              up_xyz[0] , up_xyz[1]  , up_xyz[2] );

    glFrontFace(GL_CCW);//�n�]�wfront face culling�O�f

    //�n���e��l
    glStencilFunc(GL_ALWAYS,1,1);
    glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);//��l��0
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_QUADS);//��l���ϰ�
        glVertex3f(25.0,75.0,-50.0);
        glVertex3f(-25.0,75.0,-50.0);
        glVertex3f(-25.0,25.0,-50.0);
        glVertex3f(25.0,25.0,-50.0);
    glEnd();

    
    pass=pass1;
    draw();

    //�ĤG���e,�e��l���Ҭݨ쪺,stencil buffer �O0���ϰ�
    glClear(GL_DEPTH_BUFFER_BIT);//�u�M��z buffer
    
    glFrontFace(GL_CW);//�n�]�wfront face culling�O��
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    
    //�`�N���k���
    glFrustum(1.0f, -1.0f, -1.0f, 1.0f, 5.0f, 1000.0f);//left right bottom top near_distance far_distance
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_xyz[0], eye_xyz[1] ,(-1)*eye_xyz[2]-100 , //eye �n��
              at_xyz[0] , at_xyz[1]  ,(-1)*at_xyz[2]-100  , //look at�]�n��
              up_xyz[0] , up_xyz[1]  , up_xyz[2] );
    
    pass=pass2;
    draw();

    glutSwapBuffers();//��X
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 's':   state=(state+1)%2;break;//��or��
        case 'r':   eye_xyz[0]+=20;break;//eye���k
        case 'l':   eye_xyz[0]-=20;break;//eye����
        case 't':   eye_xyz[1]+=20;break;//eye���W
        case 'b':   eye_xyz[1]-=20;break;//eye���U
        case 'f':   eye_xyz[2]-=20;break;//eye���e
        case 'n':   eye_xyz[2]+=20;break;//eye����
        case 27 :   exit(0);break;
    }
}

void idle()
{
	time2 = clock();
    static float angle=0;
    if(state)angle+=3.14*(time2-time1)/CLK_TCK;
    light_position[0]=40*cos(angle);
    light_position[2]=40*sin(angle);
	time1 = time2;
    glutPostRedisplay();
}

void draw()
{
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_COLOR_MATERIAL);//���Q�]�w����i�H�ϥγo�ӥ\��,�i�H��²�檺�]�w�C�� (��glColor)

    if(pass==pass1)glStencilFunc(GL_EQUAL,1,1);//�O1����m�~�e
    else           glStencilFunc(GL_EQUAL,0,1);//�O0����m�~�e
   
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

    //�e�������F��
    glColor3f(0.7,0.0,0.0);
    glPushMatrix();
        glTranslatef(20.0,10.0,0.0);
        glutSolidCube(20);
    glPopMatrix();

    glColor3f(0.0,0.0,0.7);
    glPushMatrix();
        glTranslatef(-20.0,10.0,0.0);
        glutSolidCube(20);
    glPopMatrix();
    
    glColor3f(0.0,0.7,0.0);
    glBegin(GL_QUADS);//�����a�O
        glVertex3f(50.0,0.0,-50.0);
        glVertex3f(-50.0,0.0,-50.0);
        glVertex3f(-50.0,0.0,50.0);
        glVertex3f(50.0,0.0,50.0);
    glEnd();

    glColor3f(0.0,0.0,0.7);
    glBegin(GL_QUADS);//�᭱���
        glVertex3f(50.0,100.0,-50.0);
        glVertex3f(-50.0,100.0,-50.0);
        glVertex3f(-50.0,0.0,-50.0);
        glVertex3f(50.0,0.0,-50.0);
    glEnd();

    //�e�@�Ӷ��⪺�y��ܥ�����m
    if(pass==pass1)glStencilFunc(GL_ALWAYS,1,1);//�@�w�q�L
    else glStencilFunc(GL_EQUAL,0,1);//0�~�e
    glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);
    glColor3f(1.0,1.0,0.0);
    glPushMatrix();
        glTranslatef(light_position[0] , light_position[1] , light_position[2]);
        glutSolidSphere(2,10,10);
    glPopMatrix();
}
