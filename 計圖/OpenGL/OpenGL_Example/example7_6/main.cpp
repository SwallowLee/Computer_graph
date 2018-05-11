/*利用stencil buffer 作遮擋,畫兩次作出牆上的鏡子效果*/
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

//材質
GLfloat mat_diffuse[] = {0.2, 0.2, 0.2, 1.0};
GLfloat mat_specular[] = {0.2, 0.2, 0.2, 1.0};
GLfloat mat_ambient[] = {0.4, 0.4, 0.4, 1.0};
GLfloat mat_shininess[] = {100.0};

//光顏色
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
    glutInitWindowSize(700,700);//設定視窗大小
    glutInitWindowPosition(0,0);
    glutCreateWindow("牆上的鏡子");//視窗標題

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
	return 0;
}

void init() //只是設定light
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
    glEnable(GL_STENCIL_TEST);                 //會用到stencil buffer
    glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //設定清除的顏色
    glClearStencil(1);                         //設定stencil buffer 清除值 = 1
    glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//執行清除


    //第一次畫出,stencil buffer 是1的區域
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 5.0f, 1000.0f);//left right bottom top near_distance far_distance
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_xyz[0], eye_xyz[1] , eye_xyz[2] ,
              at_xyz[0] , at_xyz[1]  , at_xyz[2]  ,
              up_xyz[0] , up_xyz[1]  , up_xyz[2] );

    glFrontFace(GL_CCW);//要設定front face culling是逆

    //要先畫鏡子
    glStencilFunc(GL_ALWAYS,1,1);
    glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);//鏡子改0
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_QUADS);//鏡子的區域
        glVertex3f(25.0,75.0,-50.0);
        glVertex3f(-25.0,75.0,-50.0);
        glVertex3f(-25.0,25.0,-50.0);
        glVertex3f(25.0,25.0,-50.0);
    glEnd();

    
    pass=pass1;
    draw();

    //第二次畫,畫鏡子中所看到的,stencil buffer 是0的區域
    glClear(GL_DEPTH_BUFFER_BIT);//只清除z buffer
    
    glFrontFace(GL_CW);//要設定front face culling是順
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    
    //注意左右對調
    glFrustum(1.0f, -1.0f, -1.0f, 1.0f, 5.0f, 1000.0f);//left right bottom top near_distance far_distance
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_xyz[0], eye_xyz[1] ,(-1)*eye_xyz[2]-100 , //eye 要改
              at_xyz[0] , at_xyz[1]  ,(-1)*at_xyz[2]-100  , //look at也要改
              up_xyz[0] , up_xyz[1]  , up_xyz[2] );
    
    pass=pass2;
    draw();

    glutSwapBuffers();//輸出
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 's':   state=(state+1)%2;break;//停or走
        case 'r':   eye_xyz[0]+=20;break;//eye往右
        case 'l':   eye_xyz[0]-=20;break;//eye往左
        case 't':   eye_xyz[1]+=20;break;//eye往上
        case 'b':   eye_xyz[1]-=20;break;//eye往下
        case 'f':   eye_xyz[2]-=20;break;//eye往前
        case 'n':   eye_xyz[2]+=20;break;//eye往後
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
    glEnable(GL_COLOR_MATERIAL);//不想設定材質可以使用這個功能,可以較簡單的設定顏色 (用glColor)

    if(pass==pass1)glStencilFunc(GL_EQUAL,1,1);//是1的位置才畫
    else           glStencilFunc(GL_EQUAL,0,1);//是0的位置才畫
   
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

    //畫場景的東西
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
    glBegin(GL_QUADS);//底面地板
        glVertex3f(50.0,0.0,-50.0);
        glVertex3f(-50.0,0.0,-50.0);
        glVertex3f(-50.0,0.0,50.0);
        glVertex3f(50.0,0.0,50.0);
    glEnd();

    glColor3f(0.0,0.0,0.7);
    glBegin(GL_QUADS);//後面牆壁
        glVertex3f(50.0,100.0,-50.0);
        glVertex3f(-50.0,100.0,-50.0);
        glVertex3f(-50.0,0.0,-50.0);
        glVertex3f(50.0,0.0,-50.0);
    glEnd();

    //畫一個黃色的球表示光源位置
    if(pass==pass1)glStencilFunc(GL_ALWAYS,1,1);//一定通過
    else glStencilFunc(GL_EQUAL,0,1);//0才畫
    glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);
    glColor3f(1.0,1.0,0.0);
    glPushMatrix();
        glTranslatef(light_position[0] , light_position[1] , light_position[2]);
        glutSolidSphere(2,10,10);
    glPopMatrix();
}
