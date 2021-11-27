//CG practice005
//金子　卓矢

#include <stdio.h>
#include <GL/glut.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")

void display(void);
void init(void);
void resize(int w, int h);
void drawArrow(void);

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


//立方体のベクトル情報
int edge[][2] = {
	{0,1}, /*A-B*/
	{1,2}, /*B-C*/
	{2,3}, /*C-D*/
	{3,0}, /*D-A*/
	{4,5}, /*E-F*/
	{5,6}, /*F-G*/
	{6,7}, /*G-H*/
	{7,4}, /*H-E*/
	{0,4}, /*A-E*/
	{1,5}, /*B-F*/
	{2,6}, /*C-G*/
	{3,7}, /*D-H*/
};

//四角すいのベクトル情報
int edge2[][2] = {
	{0,1}, /*I-J*/
	{0,2}, /*I-K*/
	{0,3}, /*I-L*/
	{0,4}, /*I-M*/
	{1,2}, /*J-K*/
	{1,4}, /*J-M*/
	{3,2}, /*L-K*/
	{3,4}, /*L-M*/
};

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 240);
	glutInitDisplayMode(GLUT_RGBA);
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
	glClearColor(0.5, 0.5, 0.5, 1.0); /*背景をグレーにする*/
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0); /*線の色を白にする*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//視点移動
	gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glBegin(GL_LINES);
	//立方体を作る
	for (int i = 0; i < 12; ++i) {
		glVertex3dv(vertex[edge[i][0]]);
		glVertex3dv(vertex[edge[i][1]]);
	}
	//四角すいを作る
	for (int i = 0; i < 8; ++i) {
		glVertex3dv(vertex2[edge2[i][0]]);
		glVertex3dv(vertex2[edge2[i][1]]);

	}
	glEnd();

	drawArrow();
	glFlush();
}

//矢印を描く
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
