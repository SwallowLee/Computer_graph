#include "mesh.h"
#include "view.h"
#include "scene.h"
#include "lightdata.h"
#include "Freeimage.h"
#include "glew.h"
#include "glut.h"

mesh **object;
view *viewpoint;
scene *sceneobj;
lightdata *envlight;

int level = 3;
float radius = 1.0;

const int light_count = 1;
const int obj_count = 1;

GLhandleARB MyShader;

float rot_x = 0.0f;
float rot_degree = 5.0f;
float zoom_distance = 0.0f;
float zoom_unit = 1.0f;
float light_distance = 0.0f;
float light_unit = 10.0f;
float diff_x;
float diff_y;

int selected = -1;
unsigned int *tex_objs;
int windowSize[2];
void drawobj();
void light();
void display();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void reshape(GLsizei , GLsizei );
void LoadTexture(char* pFilename, unsigned texID);
bool ShaderLoad(GLhandleARB programId, char* shaderSrc, GLenum shaderType);
void LoadShaders();




int main(int argc, char** argv)
{
	envlight = new lightdata("as3.light", light_count);
	viewpoint = new view("as3.view");
	sceneobj = new scene("as3.scene", obj_count);
	object = new mesh*[obj_count];
	for (int i = 0; i < obj_count; i++) {
		object[i] = new mesh(sceneobj->object[i].name);
	}
	tex_objs = new unsigned int[ sceneobj->texture.size() ];



	glutInit(&argc, argv);
	glutInitWindowSize(viewpoint->viewport[0], viewpoint->viewport[1]);
	glutInitWindowPosition(viewpoint->viewport[2], viewpoint->viewport[3]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Homework3");

	GLenum glew_error;
	if ((glew_error = glewInit()) != GLEW_OK)return -1;

	glewInit();
	FreeImage_Initialise();
	glGenTextures(sceneobj->texture.size() , tex_objs);
	for (int i = 0; i < sceneobj->texture.size(); i++) {
		LoadTexture(sceneobj->texture.at( i ).filename, i);
	}
	FreeImage_DeInitialise();

	LoadShaders();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();

	return 0;
}

void LoadShaders()
{
	MyShader = glCreateProgram();
	if (MyShader != 0)
	{
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.vert", GL_VERTEX_SHADER);
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.geom", GL_GEOMETRY_SHADER);
		ShaderLoad(MyShader, "../Examples/PhongShading/PhongShading.frag", GL_FRAGMENT_SHADER);
	}
}



void LoadTexture(char* pFilename, unsigned texID) {
	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pFilename, 0), pFilename);
	FIBITMAP *p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);

	int tex_type, tex_mode;
	switch (sceneobj->texture.at(texID).type) {
	case -1:
		tex_type = GL_TEXTURE_2D;
		tex_mode = GL_TEXTURE_2D;
		break;
	case 0:
		tex_type = GL_TEXTURE_CUBE_MAP;
		tex_mode = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		break;
	case 1:
		tex_type = GL_TEXTURE_CUBE_MAP;
		tex_mode = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		texID -= 1;
		break;
	case 2:
		tex_type = GL_TEXTURE_CUBE_MAP;
		tex_mode = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		texID -= 2;
		break;
	case 3:
		tex_type = GL_TEXTURE_CUBE_MAP;
		tex_mode = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		texID -= 3;
		break;
	case 4:
		tex_type = GL_TEXTURE_CUBE_MAP;
		tex_mode = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		texID -= 4;
		break;
	case 5:
		tex_type = GL_TEXTURE_CUBE_MAP;
		tex_mode = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		texID -= 5;
		break;
	default:
		break;
	}

	glBindTexture(tex_type, tex_objs[texID]);
	glTexImage2D(tex_mode, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	glGenerateMipmap(tex_type);
	glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	float fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(tex_type, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
}

bool ShaderLoad(GLhandleARB programId, char* shaderSrc, GLenum shaderType)
{
	FILE *fp;
	GLhandleARB h_shader;
	GLcharARB *shader_string;
	GLint str_length, maxLength;
	GLint isCompiled = GL_FALSE, isLinked = GL_FALSE;
	GLcharARB *pInfoLog;

	// open the file of shader source code
	if ((fp = fopen(shaderSrc, "r")) == NULL)
	{
		fprintf(stderr, "Error : Failed to read the OpenGL shader source \"%s\".\n", shaderSrc);
		return false;
	}

	// allocate memory for program string and load it.
	shader_string = (GLcharARB*)malloc(sizeof(GLcharARB) * 65536);
	str_length = (GLint)fread(shader_string, 1, 65536, fp);
	fclose(fp);

	// Create and load shader string.
	h_shader = glCreateShader(shaderType);
	if (h_shader == 0)
	{
		fprintf(stderr, "Error : Failed to create OpenGL shader object \"%s\".\n", shaderSrc);
		return false;
	}
	glShaderSource(h_shader, 1, (const GLcharARB**)&shader_string, &str_length);
	free(shader_string);

	// Compile the vertex shader, print out the compiler log message.
	glCompileShader(h_shader);

	// get compile state information
	glGetObjectParameterivARB(h_shader, GL_OBJECT_COMPILE_STATUS_ARB, &isCompiled);

	if (!isCompiled)
	{
		fprintf(stderr, "Error : Failed to compile OpenGL shader source \"%s\".\n", shaderSrc);
		glGetObjectParameterivARB(h_shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		pInfoLog = (GLcharARB *)malloc(maxLength * sizeof(GLcharARB));
		glGetInfoLogARB(h_shader, maxLength, &str_length, pInfoLog);
		fprintf(stderr, "%s\n", pInfoLog);
		free(pInfoLog);
		return false;
	}
	glAttachShader(programId, h_shader);

	// delete the shader object, since we have attached it with the program object.
	glDeleteShader(h_shader);

	// Link the program and print out the linker log message
	glLinkProgram(programId);
	glGetObjectParameterivARB(programId, GL_OBJECT_LINK_STATUS_ARB, &isLinked);

	if (!isLinked)
	{
		fprintf(stderr, "Error : Failed to link OpenGL shader \"%s\".\n", shaderSrc);
		glGetObjectParameterivARB(programId, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		pInfoLog = (GLcharARB *)malloc(maxLength * sizeof(GLcharARB));
		glGetInfoLogARB(programId, maxLength, &str_length, pInfoLog);
		fprintf(stderr, "%s\n", pInfoLog);
		free(pInfoLog);
		return false;
	}

	return true;
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case 'w':	zoom_distance += zoom_unit;
		printf("Zoom in\n");
		break;
	case 'a':	rot_x += rot_degree;
		printf("Rotate left\n");
		break;
	case 's':	zoom_distance -= zoom_unit;
		printf("Zoom out\n");
		break;
	case 'd':	rot_x -= rot_degree;
		printf("Rotate right\n");
		break;
	
	case 'f':	light_distance -= light_unit;
		printf("move light position(Decrease y)\n");
		break;
	
	case 'r':	light_distance += light_unit;
		printf("move light position(Increase y)\n");
		break;
	}
	if ('1' <= key && key <= '9') {
		int i = key - '1';
		if (i < obj_count) {
			selected = i;
			printf("Select object %d\n", i + 1);

		}
		else {
			selected = -1;
		}
	}
	printf("key = %c\n", key);
	printf("mouse = %d %d\n", x, y);
	glutPostRedisplay();
}

void motion(int x, int y) {
	
	if (selected >= 0) {
		sceneobj->object[selected].transfer[0] = x - diff_x;
		sceneobj->object[selected].transfer[1] = -1 * (y - diff_y);	// Y differs in window coordinate and object coordinate
		glutPostRedisplay();
	}
	printf("mouse = %d %d\n", x, y);
}

void mouse(int button, int state, int x, int y) {
	int startX = 0, startY = 0;
	if (state == GLUT_DOWN) {
		startX = x;
		startY = y;
		if (selected >= 0) {
			diff_x = x - sceneobj->object[selected].transfer[0];
			diff_y = y - sceneobj->object[selected].transfer[1];
		}
	}
	else if (state == GLUT_UP) {
		printf("mouse = %d %d \n", x - startX, y - startY);
	}
}

void light()
{
	GLfloat light_specular[light_count][4];
	GLfloat light_diffuse[light_count][4];
	GLfloat light_ambient[light_count][4];
	GLfloat light_position[light_count][4];
	
	for (int i = 0; i < light_count; i++) {
		for (int j = 0; j < 3; j++) {
			light_specular[i][j] = envlight->point[i].spe[j];
			light_diffuse[i][j] = envlight->point[i].dif[j];
			light_ambient[i][j] = envlight->point[i].amb[j];
			light_position[i][j] = envlight->point[i].pos[j];
			if (j == 1) {
				light_position[i][j] += light_distance;
			}
		}
		light_specular[i][3] = 1.0f;
		light_diffuse[i][3] = 1.0f;
		light_ambient[i][3] = 1.0f;
		light_position[i][3] = 1.0f;
	}
	GLfloat ambient[3];
	for (int i = 0; i < 3; i++) {
		ambient[i] = envlight->amb[i];
	}

	glShadeModel(GL_SMOOTH);

	// z buffer enable
	glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	int lightN = GL_LIGHT0;
	for (int i = 0; i < light_count; i++) {
		glEnable(lightN + i);
		glLightfv(lightN + i, GL_POSITION, light_position[i]);
		glLightfv(lightN + i, GL_DIFFUSE, light_diffuse[i]);
		glLightfv(lightN + i, GL_SPECULAR, light_specular[i]);
		glLightfv(lightN + i, GL_AMBIENT, light_ambient[i]);
	}
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}


void display()
{
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	glEnable(GL_ALPHA_TEST);                   // Enables Alpha Testing
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glAlphaFunc(GL_GREATER, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewpoint->fovy, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], viewpoint->dnear, viewpoint->dfar);
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		viewpoint->eye[0], viewpoint->eye[1], viewpoint->eye[2],		// eye
		viewpoint->vat[0], viewpoint->vat[1], viewpoint->vat[2],		// center
		viewpoint->vup[0], viewpoint->vup[1], viewpoint->vup[2]);		// up
	// normalize
	float X = viewpoint->vat[0] - viewpoint->eye[0];
	float Y = viewpoint->vat[1] - viewpoint->eye[1];
	float Z = viewpoint->vat[2] - viewpoint->eye[2];
	float sum = X * X + Y * Y + Z * Z;
	sqrt(sum);

	X /= sum;
	Y /= sum;
	Z /= sum;
	glTranslatef(X * zoom_distance, Y * zoom_distance, Z * zoom_distance);

	glTranslatef(sceneobj->object[0].transfer[0], sceneobj->object[0].transfer[1], sceneobj->object[0].transfer[2]);
	glRotatef(rot_x, 0.0, 1.0, 0.0);
	glTranslatef(-sceneobj->object[0].transfer[0], -sceneobj->object[0].transfer[1], -sceneobj->object[0].transfer[2]);
	//注意light位置的設定，要在gluLookAt之後
	light();

	glUseProgram(MyShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_objs[0]);
	GLint location = glGetUniformLocation(MyShader, "colorTexture");
	if (location == -1)
		printf("Cant find texture name: colorTexture\n");
	else
		glUniform1i(location, 0);

	drawobj();

	glutSwapBuffers();
}

void drawobj() {
	mesh *current;
	int lastMaterial;
	for (int i = 0; i < obj_count; i++) {
		current = object[i];
		lastMaterial = -1;
		glPushMatrix();
		glTranslatef(sceneobj->object[i].transfer[0], sceneobj->object[i].transfer[1], sceneobj->object[i].transfer[2]);
		glRotatef(sceneobj->object[i].angle[0], sceneobj->object[i].angle[1], sceneobj->object[i].angle[2], sceneobj->object[i].angle[3]);
		glScalef(sceneobj->object[i].scale[0], sceneobj->object[i].scale[1], sceneobj->object[i].scale[2]);
		
		int tex_n;

		if (!strcmp(sceneobj->object[i].method, "single-texture")) {
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex_objs[sceneobj->object[i].textureid]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else if (!strcmp(sceneobj->object[i].method, "no-texture")) {
		}
		else if (!strcmp(sceneobj->object[i].method, "multi-texture")) {

			tex_n = GL_TEXTURE0;
			for (int tex_offset = 0; tex_offset < 2; tex_offset++) {
				glActiveTexture(tex_n + tex_offset);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tex_objs[sceneobj->object[i].textureid + tex_offset]);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			}
		}
		else if (!strcmp(sceneobj->object[i].method, "cube-map")) {

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
			glEnable(GL_TEXTURE_CUBE_MAP);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		}
	
		
		for (size_t i2 = 0; i2 < current->fTotal; ++i2)
		{
			// set material property if this face used different material
			if (lastMaterial != current->faceList[i2].m)
			{
				lastMaterial = (int)current->faceList[i2].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT, current->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, current->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, current->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &current->mList[lastMaterial].Ns);
			}

			glBegin(GL_TRIANGLES);
			for (size_t j = 0; j < 3; ++j)
			{
				//textex corrd. object->tList[object->faceList[i][j].t].ptr
				if (!strcmp(sceneobj->object[i].method, "single-texture")){
					glTexCoord2fv(current->tList[current->faceList[i2][j].t].ptr);
				}
				else if (!strcmp(sceneobj->object[i].method, "no-texture")){
				}
				else if (!strcmp(sceneobj->object[i].method, "multi-texture")){
					glMultiTexCoord2fv(GL_TEXTURE0, current->tList[current->faceList[i2][j].t].ptr);
					glMultiTexCoord2fv(GL_TEXTURE1, current->tList[current->faceList[i2][j].t].ptr);
				}
				else if (!strcmp(sceneobj->object[i].method, "cube-map")){
				}
				glNormal3fv(current->nList[current->faceList[i2][j].n].ptr);
				glVertex3fv(current->vList[current->faceList[i2][j].v].ptr);
			}
			glEnd();
		}

		// unbind textures
		if (!strcmp(sceneobj->object[i].method, "single-texture")) {
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (!strcmp(sceneobj->object[i].method, "no-texture")) {
		}
		else if (!strcmp(sceneobj->object[i].method, "multi-texture")) {

			tex_n = GL_TEXTURE0;
			for (int tex_offset = 2 - 1; tex_offset >= 0; tex_offset--) {
				glActiveTexture(tex_n + tex_offset);
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		else if (!strcmp(sceneobj->object[i].method, "cube-map")) {
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_CUBE_MAP);
		}

		glPopMatrix();
	}
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}


