//CG practice009
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
//void drawArrow(void);
void DRAW_FLOOR(); //床描画関数
void CalcShadowMatrixf(); //影生成関数

void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void idle(void);
void specialkey(int key, int x, int y);

static int r = 0; /*回転角度*/
static int s; /*回転方向*/
static int h = 0; /*y方向視点*/

//平面方程式ax+by+cz+d=0
float plane[] = { 0.0,1.0,0.0,1.0 };

float ShadowMatrix[4][4];//影行列

//ポリゴンオフセット
float OffsetFactor = 1.0f;
float OffsetUnit = 0.0f;

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

//立方体の法線ベクトル
GLdouble normal[][3] = {
	{0.0,0.0,-1.0}, /*A-B-C-D面*/
	{1.0,0.0,0.0},/*B-F-G-C面*/
	{0.0,0.0,1.0}, /*F-E-H-G面*/
	{-1.0,0.0,0.0}, /*E-A-D-H面*/
	{0.0,-1.0,0.0}, /*E-F-B-A面*/
	{0.0,1.0,0.0}, /*D-C-G-H面*/
};

//四角すいの法線ベクトル
GLdouble normal2[][3] = {
	{0.0,1.28,1.6},  /*I-J-K面*/
	{-1.6,1.28,0.0},/*I-K-L面*/
	{0.0,1.28,-1.6},/*I-L-M面*/
	{1.6,1.28,0.0},/*I-M-J面*/
	{0.0,-1.0,0.0}, /*J-M-L-K面*/
};

GLfloat light0_pos[] = { 7.0,5.0,5.0,1.0 }; /*光源の位置*/

//立方体の材質
GLfloat material_amb[] = { 0.2,0.2,0.2,0.0 }; /*環境光に対する反射*/
GLfloat material_dif[] = { 0.6,0.6,0.6,0.0 }; /*拡散光に対する反射*/
GLfloat material_spc[] = { 0.6,0.6,0.6,0.0 }; /*鏡面反射*/
GLfloat material_shi[] = { 40.0 }; /*光沢*/

//四角すいの材質
GLfloat material_amb2[] = { 0.2,0.2,0.2,0.0 }; /*環境光に対する反射*/
GLfloat material_dif2[] = { 0.4,0.4,0.0,0.0 }; /*拡散光に対する反射*/
GLfloat material_spc2[] = { 0.6,0.6,0.0,0.0 }; /*鏡面反射*/
GLfloat material_shi2[] = { 50.0 }; /*光沢*/

//球の材質
GLfloat material_amb3[] = { 0.2,0.2,0.2,0.0 }; /*環境光に対する反射*/
GLfloat material_dif3[] = { 0.0,0.4,0.0,0.0 }; /*拡散光に対する反射*/
GLfloat material_spc3[] = { 0.0,0.6,0.0,0.0 }; /*鏡面反射*/
GLfloat material_shi3[] = { 60.0 }; /*光沢*/

GLfloat floor_dif[] = { 0.8,0.8,0.8,1.0 }; //床材質

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 240);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
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
	glClearColor(0.0, 0.0, 0.0, 1.0); /*背景を黒にする*/
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_LIGHTING); //光源有効
	//glEnable(GL_LIGHT0); //光源0有効
	CalcShadowMatrixf();
}

void display(void)
{
	//光源を点灯
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0); /*線の色を白にする*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//視点移動
	gluLookAt(5.0, 0.5 * h, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos); /*光源位置設定*/


	glPushMatrix(); { //階層1

		glTranslatef(0, -1.0, 0); //一致されるために必要

	//材質ON
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spc);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shi);

	glPushMatrix(); {
		glRotated((double)r * 1.0, 0.0, 1.0, 0.0); /*回転*/
		
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);

		//立方体を作る
		for (int j = 0; j < 6; ++j) {
			glNormal3dv(normal[j]); /*法線設定*/
			for (int i = 0; i < 4; ++i) {
				glVertex3dv(vertex[face[j][i]]);
			}
		}
		glEnd();
	}glPopMatrix();

	//材質ON
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_amb2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spc2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shi2);
	
	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*回転*/
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);
		//四角すいを作る
		for (int j = 0; j < 5; ++j) {
			glNormal3dv(normal2[j]); /*法線設定*/
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

	//材質ON
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_amb3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spc3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shi3);

	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*回転*/
		glTranslatef(0.8, 1.0, 0.8);
		for (int i = 0; i < 4; i++) {
			if (i == 1) 
				glTranslatef(0.0, 0.0, -1.6);

			if (i == 2) 
				glTranslatef(-1.6, 0.0, 0.0);

			if (i == 3) 
				glTranslatef(0.0, 0.0, 1.6);

			glutSolidSphere(0.1, 36, 36);
		}
		
	}glPopMatrix();
	
		glTranslatef(0, 2.0, 0);
		glutSolidSphere(0.1, 36, 36);

	}glPopMatrix();

	//床を描画(ポリゴンオフセット付き)
	glEnable(GL_POLYGON_OFFSET_FILL); //ポリゴンオフセットON

	glPushMatrix(); { //階層2
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floor_dif);
		glPolygonOffset(OffsetFactor, OffsetUnit);
		DRAW_FLOOR();
	} glPopMatrix();
	glDisable(GL_POLYGON_OFFSET_FILL); //ポリゴンオフセットOFF
	//光源を消灯
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	//影の描画
	glPushMatrix(); { //階層3
		glMultMatrixf(&ShadowMatrix[0][0]); //影行列を乗算
		glColor3d(0, 0, 0); //影は黒とする

		glTranslated(0, -1.0, 0); //一致させるために必要

		//立方体の描画
		glPushMatrix(); {
		glRotated((double)r * 1.0, 0.0, 1.0, 0.0); /*回転*/

		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);

		//立方体を作る
		for (int j = 0; j < 6; ++j) {
			glNormal3dv(normal[j]); /*法線設定*/
			for (int i = 0; i < 4; ++i) {
				glVertex3dv(vertex[face[j][i]]);
			}
		}

		glEnd();
	}glPopMatrix();
		
	//四角すいの描画
	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*回転*/
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);
		//四角すいを作る
		for (int j = 0; j < 5; ++j) {
			glNormal3dv(normal2[j]); /*法線設定*/
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

	//球の描画
	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*回転*/
		glTranslatef(0.8, 1.0, 0.8);
		for (int i = 0; i < 4; i++) {
			if (i == 1)
				glTranslatef(0.0, 0.0, -1.6);

			if (i == 2)
				glTranslatef(-1.6, 0.0, 0.0);

			if (i == 3)
				glTranslatef(0.0, 0.0, 1.6);

			glutSolidSphere(0.1, 36, 36);
		}
	}glPopMatrix();
		glTranslatef(0, 2.0, 0);
		glutSolidSphere(0.1, 36, 36);

	}glPopMatrix();


	glutSwapBuffers();/*ダブルバッファリングの設定*/
	if ((r += s) >= 3600 || (r += s) <= -3600) r = 0;

}

//床の描画関数
void DRAW_FLOOR() {
	glTranslatef(0, -1, 0);
	glRotatef(90, -1, 0, 0);
	glNormal3d(0, 0, 1);
	glRectf(-5, 5, 5, -5);
}

//影行列の計算
void CalcShadowMatrixf() {
	static float dot = 0.0;

	//平面と光源ベクトルの内積
	for (int i = 0; i < 4; ++i) {
		dot = dot + plane[i] * light0_pos[i];
	}

	//シャドウマッピングの計算
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			ShadowMatrix[i][j] = -plane[i] * light0_pos[j];
			if (i == j)
				ShadowMatrix[i][j] += dot;
		}
	}
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
		s = 0;
		glutPostRedisplay(); /*再描画*/
		break;
	case GLUT_KEY_DOWN:
		h--;
		s = 0;
		glutPostRedisplay();  /*再描画*/
		break;
	default:
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
