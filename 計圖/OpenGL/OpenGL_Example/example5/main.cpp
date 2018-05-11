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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      // 設定清除color的底色
	glClearDepth(1.0f);                        // 設定Depth Buffer (就是z buffer) 清除值
	glEnable(GL_DEPTH_TEST);                   // 啟動深度測試
	glDepthFunc(GL_LEQUAL);                    // 可以設定成less or equal才更新畫面,若是設定成GL_LESS是只有less才更新畫面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這兩行把畫面清成底色並且清除z buffer

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

	glMatrixMode(GL_MODELVIEW);//注意這一項,改為設定model view
	glColor3f(1.0, 1.0, 1.0);

	glutWireSphere(1.0, 20, 16);// the Sun 

	glPushMatrix();//儲存現在的矩陣 (目前是modelview)
		glRotatef(year, 0.0, 1.0, 0.0);
		glTranslatef(3.0, 0.0, 0.0);
		glRotatef(day, 0.0, 1.0, 0.0);
		glutWireSphere(0.5, 10, 8);    // the Planet, 此時的矩陣是 I * ModelView * R * T * R
	glPopMatrix();//拿stack中的矩陣到指定矩陣(modelview),會回到 I * ModelView

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
	day += 100.0*(time2-time1)/CLK_TCK; //現在有用時間控制速度
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
		glutIdleFunc(idle); //註冊idle function
		break;
	case 'A':    glutIdleFunc(NULL);//取消idle function
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
