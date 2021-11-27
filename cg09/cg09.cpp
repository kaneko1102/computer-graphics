//CG practice009
//���q�@���

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
void DRAW_FLOOR(); //���`��֐�
void CalcShadowMatrixf(); //�e�����֐�

void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void idle(void);
void specialkey(int key, int x, int y);

static int r = 0; /*��]�p�x*/
static int s; /*��]����*/
static int h = 0; /*y�������_*/

//���ʕ�����ax+by+cz+d=0
float plane[] = { 0.0,1.0,0.0,1.0 };

float ShadowMatrix[4][4];//�e�s��

//�|���S���I�t�Z�b�g
float OffsetFactor = 1.0f;
float OffsetUnit = 0.0f;

//�����̂̒��_�̍��W
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

//�l�p�����̒��_�̍��W
GLdouble vertex2[][3] = {
	{0.5,2.0,0.5},/*I*/
	{1.3,1,1.3}, /*J*/
	{-0.3,1,1.3}, /*K*/
	{-0.3,1,-0.3}, /*L*/
	{1.3,1,-0.3}, /*M*/
};


int face[][5] = {
	{0,1,2,3,0},/*A-B-C-D��*/
	{1,5,6,2,1},/*B-F-G-C��*/
	{5,4,7,6,5},/*F-E-H-G��*/
	{4,0,3,7,4},/*E-A-D-H��*/
	{4,5,1,0,4},/*E-F-B-A��*/
	{3,2,6,7,3},/*D-C-G-H��*/
};

int face2[][4] = {
	{0,1,2,0},/*I-J-K-I��*/
{0,2,3,0},/*I-K-L-I��*/
{0,3,4,0},/*I-L-M-I��*/
{0,4,1,0},/*I-M-J-I��*/
};

int face3[5] = { 1,4,3,2,1 };/*J-M-L-K��*/

//�����̖̂@���x�N�g��
GLdouble normal[][3] = {
	{0.0,0.0,-1.0}, /*A-B-C-D��*/
	{1.0,0.0,0.0},/*B-F-G-C��*/
	{0.0,0.0,1.0}, /*F-E-H-G��*/
	{-1.0,0.0,0.0}, /*E-A-D-H��*/
	{0.0,-1.0,0.0}, /*E-F-B-A��*/
	{0.0,1.0,0.0}, /*D-C-G-H��*/
};

//�l�p�����̖@���x�N�g��
GLdouble normal2[][3] = {
	{0.0,1.28,1.6},  /*I-J-K��*/
	{-1.6,1.28,0.0},/*I-K-L��*/
	{0.0,1.28,-1.6},/*I-L-M��*/
	{1.6,1.28,0.0},/*I-M-J��*/
	{0.0,-1.0,0.0}, /*J-M-L-K��*/
};

GLfloat light0_pos[] = { 7.0,5.0,5.0,1.0 }; /*�����̈ʒu*/

//�����̂̍ގ�
GLfloat material_amb[] = { 0.2,0.2,0.2,0.0 }; /*�����ɑ΂��锽��*/
GLfloat material_dif[] = { 0.6,0.6,0.6,0.0 }; /*�g�U���ɑ΂��锽��*/
GLfloat material_spc[] = { 0.6,0.6,0.6,0.0 }; /*���ʔ���*/
GLfloat material_shi[] = { 40.0 }; /*����*/

//�l�p�����̍ގ�
GLfloat material_amb2[] = { 0.2,0.2,0.2,0.0 }; /*�����ɑ΂��锽��*/
GLfloat material_dif2[] = { 0.4,0.4,0.0,0.0 }; /*�g�U���ɑ΂��锽��*/
GLfloat material_spc2[] = { 0.6,0.6,0.0,0.0 }; /*���ʔ���*/
GLfloat material_shi2[] = { 50.0 }; /*����*/

//���̍ގ�
GLfloat material_amb3[] = { 0.2,0.2,0.2,0.0 }; /*�����ɑ΂��锽��*/
GLfloat material_dif3[] = { 0.0,0.4,0.0,0.0 }; /*�g�U���ɑ΂��锽��*/
GLfloat material_spc3[] = { 0.0,0.6,0.0,0.0 }; /*���ʔ���*/
GLfloat material_shi3[] = { 60.0 }; /*����*/

GLfloat floor_dif[] = { 0.8,0.8,0.8,1.0 }; //���ގ�

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 240);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("OpenGL TEST");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard); /*�L�[�{�[�h�֐�*/
	glutMouseFunc(mouse); /*�}�E�X�֐�*/
	glutSpecialFunc(specialkey); /*����L�[*/
	init();
	glutMainLoop();
	return 0;

}

void idle(void)
{
	glutPostRedisplay(); /*�ĕ`��*/
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
	glClearColor(0.0, 0.0, 0.0, 1.0); /*�w�i�����ɂ���*/
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_LIGHTING); //�����L��
	//glEnable(GL_LIGHT0); //����0�L��
	CalcShadowMatrixf();
}

void display(void)
{
	//������_��
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0); /*���̐F�𔒂ɂ���*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//���_�ړ�
	gluLookAt(5.0, 0.5 * h, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos); /*�����ʒu�ݒ�*/


	glPushMatrix(); { //�K�w1

		glTranslatef(0, -1.0, 0); //��v����邽�߂ɕK�v

	//�ގ�ON
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spc);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shi);

	glPushMatrix(); {
		glRotated((double)r * 1.0, 0.0, 1.0, 0.0); /*��]*/
		
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);

		//�����̂����
		for (int j = 0; j < 6; ++j) {
			glNormal3dv(normal[j]); /*�@���ݒ�*/
			for (int i = 0; i < 4; ++i) {
				glVertex3dv(vertex[face[j][i]]);
			}
		}
		glEnd();
	}glPopMatrix();

	//�ގ�ON
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_amb2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spc2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shi2);
	
	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*��]*/
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);
		//�l�p���������
		for (int j = 0; j < 5; ++j) {
			glNormal3dv(normal2[j]); /*�@���ݒ�*/
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

	//�ގ�ON
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_amb3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spc3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shi3);

	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*��]*/
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

	//����`��(�|���S���I�t�Z�b�g�t��)
	glEnable(GL_POLYGON_OFFSET_FILL); //�|���S���I�t�Z�b�gON

	glPushMatrix(); { //�K�w2
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floor_dif);
		glPolygonOffset(OffsetFactor, OffsetUnit);
		DRAW_FLOOR();
	} glPopMatrix();
	glDisable(GL_POLYGON_OFFSET_FILL); //�|���S���I�t�Z�b�gOFF
	//����������
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	//�e�̕`��
	glPushMatrix(); { //�K�w3
		glMultMatrixf(&ShadowMatrix[0][0]); //�e�s�����Z
		glColor3d(0, 0, 0); //�e�͍��Ƃ���

		glTranslated(0, -1.0, 0); //��v�����邽�߂ɕK�v

		//�����̂̕`��
		glPushMatrix(); {
		glRotated((double)r * 1.0, 0.0, 1.0, 0.0); /*��]*/

		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);

		//�����̂����
		for (int j = 0; j < 6; ++j) {
			glNormal3dv(normal[j]); /*�@���ݒ�*/
			for (int i = 0; i < 4; ++i) {
				glVertex3dv(vertex[face[j][i]]);
			}
		}

		glEnd();
	}glPopMatrix();
		
	//�l�p�����̕`��
	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*��]*/
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);
		//�l�p���������
		for (int j = 0; j < 5; ++j) {
			glNormal3dv(normal2[j]); /*�@���ݒ�*/
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

	//���̕`��
	glPushMatrix(); {
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*��]*/
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


	glutSwapBuffers();/*�_�u���o�b�t�@�����O�̐ݒ�*/
	if ((r += s) >= 3600 || (r += s) <= -3600) r = 0;

}

//���̕`��֐�
void DRAW_FLOOR() {
	glTranslatef(0, -1, 0);
	glRotatef(90, -1, 0, 0);
	glNormal3d(0, 0, 1);
	glRectf(-5, 5, 5, -5);
}

//�e�s��̌v�Z
void CalcShadowMatrixf() {
	static float dot = 0.0;

	//���ʂƌ����x�N�g���̓���
	for (int i = 0; i < 4; ++i) {
		dot = dot + plane[i] * light0_pos[i];
	}

	//�V���h�E�}�b�s���O�̌v�Z
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
	case '\033':/*'\033��ESC��ASCII�R�[�h*/
		printf("��~���w������܂���\n");
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
		glutPostRedisplay(); /*�ĕ`��*/
		break;
	case GLUT_KEY_DOWN:
		h--;
		s = 0;
		glutPostRedisplay();  /*�ĕ`��*/
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
			glutIdleFunc(idle); /*�A�j���[�V�����J�n*/
		}
		else {
			glutIdleFunc(0); /*�A�j���[�V������~*/
		}

		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			s = -1;
			glutIdleFunc(idle); /*�A�j���[�V�����J�n*/
		}
		else {
			glutIdleFunc(0); /*�A�j���[�V������~*/
		}
		break;

	default:
		break;

	}
}

//����`��
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
