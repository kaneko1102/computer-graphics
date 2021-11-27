//CG practice006
//���q�@���

#include <stdio.h>
#include <GL/glut.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")

void display(void);
void init(void);
void resize(int w, int h);
void drawArrow(void);

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

GLdouble color[][3] = {
	{1.0,0.0,0.0},/*��*/
{0.0,1.0,0.0}, /*��*/
{0.0,0.0,1.0},/*��*/
{1.0,1.0,0.0},/*��*/
{1.0,0.0,1.0},/*�}�[���_*/
{0.0,1.0,1.0},/*�V�A��*/
};

GLdouble color2[][3] = {
	{1.0,0.0,0.0},/*��*/
	{0.0,1.0,0.0}, /*��*/
	{0.0,0.0,1.0},/*��*/
	{1.0,1.0,0.0},/*��*/
{0.4,0.4,0.4},/*�O���[*/
};

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 240);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
	glutCreateWindow("OpenGL TEST");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	init();
	glutMainLoop();
	return 0;

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
	glClearColor(0.7, 0.7, 0.7, 1.0); /*�w�i���O���[�ɂ���*/
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0); /*���̐F�𔒂ɂ���*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//���_�ړ�
	gluLookAt(5.0, -0.5, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	

	glBegin(GL_QUADS);

	//�����̂����
	for (int j = 0; j < 6; ++j) {
		glColor3dv(color[j]);
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex[face[j][i]]);
		}
	}

	//�l�p���������
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
	glFlush();
	
}

//����`��
void drawArrow(void)
{
	glLineWidth(2.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.9, 0.1, 0);
	glVertex3f(1.0, 0, 0);
	glVertex3f(0.9, -0.1, 0);
	glVertex3f(1.0, 0, 0);
	glEnd();


	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.1, 0.9, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(-0.1, 0.9, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(0.1, 0.0, 0.9);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(-0.1, 0.0, 0.9);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}
