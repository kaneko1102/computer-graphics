//CG practice007
//金子　卓矢

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")

void display(void);
void init(void);
void resize(int w, int h);
void drawArrow(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void idle(void);
void specialkey(int key, int x, int y);

static int r = 0; /*回転角度*/
static int s; /*回転方向*/
static int h=0; /*y方向視点*/

//立方体の頂点の座標
GLdouble vertex[][3] = {
	{0.0,0.0,0.0}, /*A*/
	{1.0,0.0,0.0}, /*B*/
	{1.0,1.0,0.0},/*C*/
	{0.0,1.0,0.0}, /*D*/
	{0.0,0.0,1.0}, /*E*/
	{1.0,0.0,1.0}, /*F*/
	{1.0,1.0,1.0}, /*G*/
	{0.0,1.0,1.0}, /*H*/
};

//四角すいの頂点の座標
GLdouble vertex2[][3] = {
	{0.5,2.0,0.5},/*I*/
	{1.3,1,1.3}, /*J*/
	{-0.3,1,1.3}, /*K*/
	{-0.3,1,-0.3}, /*L*/
	{1.3,1,-0.3}, /*M*/
};


int face[][5] = {
	{0,1,2,3,0},/*A-B-C-D面*/
	{1,5,6,2,1},/*B-F-G-C面*/
	{5,4,7,6,5},/*F-E-H-G面*/
	{4,0,3,7,4},/*E-A-D-H面*/
	{4,5,1,0,4},/*E-F-B-A面*/
	{3,2,6,7,3},/*D-C-G-H面*/
};

int face2[][4] = {
	{0,1,2,0},/*I-J-K-I面*/
{0,2,3,0},/*I-K-L-I面*/
{0,3,4,0},/*I-L-M-I面*/
{0,4,1,0},/*I-M-J-I面*/
};

int face3[5] = { 1,4,3,2,1 };/*J-M-L-K面*/

GLdouble color[][3] = {
	{1.0,0.0,0.0},/*赤*/
	{0.0,1.0,0.0}, /*緑*/
	{0.0,0.0,1.0},/*青*/
	{1.0,1.0,0.0},/*黄*/
	{1.0,0.0,1.0},/*マゼンダ*/
	{0.0,1.0,1.0},/*シアン*/
};

GLdouble color2[][3] = {
	{1.0,0.0,0.0},/*赤*/
	{0.0,1.0,0.0}, /*緑*/
	{0.0,0.0,1.0},/*青*/
	{1.0,1.0,0.0},/*黄*/
	{0.4,0.4,0.4},/*グレー*/
};

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 240);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH |GLUT_DOUBLE);
	glutCreateWindow("OpenGL TEST");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard); /*キーボード関数*/
	glutMouseFunc(mouse); /*マウス関数*/
	glutSpecialFunc(specialkey); /*特殊キー*/
	init();
	glutMainLoop();
	return 0;

}

void idle(void)
{
	glutPostRedisplay(); /*再描画*/
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glTranslated(0.0, 0.0, -5.0);
}


void init(void)
{
	glClearColor(0.7, 0.7, 0.7, 1.0); /*背景をグレーにする*/
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0); /*線の色を白にする*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//視点移動
	gluLookAt(5.0, 0.5*h, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix(); {
		glRotated((double)r * 1.0, 0.0, 1.0, 0.0); /*回転*/
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);

		//立方体を作る
		for (int j = 0; j < 6; ++j) {
			glColor3dv(color[j]);
			for (int i = 0; i < 4; ++i) {
				glVertex3dv(vertex[face[j][i]]);
			}
		}
		
		glEnd();
	}glPopMatrix();

	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*回転*/
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);
		//四角すいを作る
		for (int j = 0; j < 5; ++j) {
			glColor3dv(color2[j]);
			if (j < 4) {
				for (int i = 0; i < 4; ++i) {
					glVertex3dv(vertex2[face2[j][i]]);
				}
			}
			else {
				for (int i = 0; i < 5; ++i) {
					glVertex3dv(vertex2[face3[i]]);
				}
			}
		}
		glEnd();
	}glPopMatrix();

	drawArrow();
	
	glutSwapBuffers();/*ダブルバッファリングの設定*/
	if ((r+=s) >= 3600||(r+=s)<=-3600) r = 0;
	
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
		case 'Q':
		case '\033':/*'\033はESCのASCIIコード*/
			printf("停止が指示されました\n");
			exit(0);
		default:
			break;
	}
}

void specialkey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		h++;
		glutPostRedisplay(); /*再描画*/
		break;
	case GLUT_KEY_DOWN:
		h--;
		glutPostRedisplay();  /*再描画*/
		break;
	default :
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			s = 1;
			glutIdleFunc(idle); /*アニメーション開始*/
		}
		else {
			glutIdleFunc(0); /*アニメーション停止*/
		}

		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			s = -1;
			glutIdleFunc(idle); /*アニメーション開始*/	
		}
		else {
			glutIdleFunc(0); /*アニメーション停止*/
		}
		break;
		
	default:
		break;

	}
}

//矢印を描く
void drawArrow(void)
{
	glLineWidth(2.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(1.9, 0.1, 0);
	glVertex3f(2.0, 0, 0);
	glVertex3f(1.9, -0.1, 0);
	glVertex3f(2.0, 0, 0);
	glEnd();


	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 2.2, 0.0);
	glVertex3f(0.1, 2.1, 0.0);
	glVertex3f(0.0, 2.2, 0.0);
	glVertex3f(-0.1, 2.1, 0.0);
	glVertex3f(0.0, 2.2, 0.0);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 2.0);
	glVertex3f(0.1, 0.0, 1.9);
	glVertex3f(0.0, 0.0, 2.0);
	glVertex3f(-0.1, 0.0, 1.9);
	glVertex3f(0.0, 0.0, 2.0);
	glEnd();
}
