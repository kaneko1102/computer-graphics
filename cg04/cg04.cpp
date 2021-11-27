//**************************************
//CG practice004
//�_�C����3��CG�����v���O����
//���q�@���
//**************************************

#include <stdio.h>
#include <GL/glut.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")

void display(void);
void init(void);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow("Opengl TEST");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1.0, 0.0, 0.0); /*�}�`�̐F��Ԃɂ���*/
	for (int i = 0; i < 3; i++) {
		glLoadIdentity();
		glTranslatef(-(i - 1) * 0.35, (i - 1) * 0.25, 0); /*���s�ړ�*/
		glRotatef(45, 0, 0, 1); /*��]*/
		glBegin(GL_POLYGON);

		/*���_�̍��W��ݒ�*/
		glVertex2f(-0.15 + 0.5 * (i - 1), 0.15);
		glVertex2f(0.15 + 0.5 * (i - 1), 0.15);
		glVertex2f(0.15 + 0.5 * (i - 1), -0.15);
		glVertex2f(-0.15 + 0.5 * (i - 1), -0.15);

		glEnd();
	}
	glFlush();

}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0); /*�w�i�𔒂ɂ���*/
}
