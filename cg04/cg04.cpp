//**************************************
//CG practice004
//ダイヤの3のCGを作るプログラム
//金子　卓矢
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
	glColor3d(1.0, 0.0, 0.0); /*図形の色を赤にする*/
	for (int i = 0; i < 3; i++) {
		glLoadIdentity();
		glTranslatef(-(i - 1) * 0.35, (i - 1) * 0.25, 0); /*平行移動*/
		glRotatef(45, 0, 0, 1); /*回転*/
		glBegin(GL_POLYGON);

		/*頂点の座標を設定*/
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
	glClearColor(1.0, 1.0, 1.0, 1.0); /*背景を白にする*/
}
