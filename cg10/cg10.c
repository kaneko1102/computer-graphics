//CG practice010
//金子　卓矢

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/GLUT.h>

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

GLfloat light0_pos[] = { 2.0, 4.0, 6.0, 1.0 }; // Light position

GLfloat light0_amb[] = { 1.0, 1.0, 1.0, 0.0 }; // Ambient light
GLfloat light0_dif[] = { 1.0, 1.0, 1.0, 0.0 }; // Diffuse light

#define TEXWIDTH 256
#define TEXHEIGHT 256
static const char texName[]="./img/wall.raw";
static const char texName2[]="./img/tile.raw";

GLubyte texture[TEXHEIGHT][TEXWIDTH][3];
GLubyte texture2[TEXHEIGHT][TEXWIDTH][3];


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

//立方体の法線ベクトル
GLdouble normal[][3] = {
	{0.0,0.0,-1.0}, /*A-B-C-D面*/
	{1.0,0.0,0.0},/*B-F-G-C面*/
	{0.0,0.0,1.0}, /*F-E-H-G面*/
	{-1.0,0.0,0.0}, /*E-A-D-H面*/
	{0.0,-1.0,0.0}, /*E-F-B-A面*/
	{0.0,1.0,0.0} /*D-C-G-H面*/
};

//四角すいの法線ベクトル
GLdouble normal2[][3] = {
	{0.0,1.28,1.6},  /*I-J-K面*/
	{-1.6,1.28,0.0},/*I-K-L面*/
	{0.0,1.28,-1.6},/*I-L-M面*/
	{1.6,1.28,0.0},/*I-M-J面*/
	{0.0,-1.0,0.0} /*J-M-L-K面*/
};

GLdouble texPos[][2]={
{0.0,1.0},
{1.0,1.0},
{1.0,0.0},
{0.0,0.0}
};

GLdouble texPos2[][3]={
{0.0,1.0},
{1.0,1.0},
{0.5,0.0}
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
 FILE *fp,*fp2;
    
    // Read Texture Image
    if((fp=fopen(texName,"rb"))!=NULL){
        fread(texture,sizeof texture,1,fp);
        fclose(fp);
    }else{
        perror(texName);
    }
    
        if((fp2=fopen(texName2,"rb"))!=NULL){
        fread(texture2,sizeof texture2,1,fp2);
        fclose(fp2);
    }else{
        perror(texName2);
    }

    
    
    // Stuffing the texture as byte unit
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    // Synthesis texture and object color
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

 // Initialize lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0); /*線の色を白にする*/
	
	 //テクスチャ変換行列
    glMatrixMode(GL_TEXTURE); 
    glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//視点移動
	gluLookAt(5.0, 0.5*h, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	 // Setting the light condition
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_dif);
    
    // Start shading
    glEnable(GL_DEPTH_TEST);

//テクスチャ変換開始
    glEnable(GL_TEXTURE_2D);


	glPushMatrix(); {
    // Assignment of texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, TEXWIDTH, TEXHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, texture);
    // Designation of MAG and MIN for texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

		
		glRotated((double)r * 1.0, 0.0, 1.0, 0.0); /*回転*/
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);

		//立方体を作る
		for (int j = 0; j < 6; ++j) {
			        glNormal3dv(normal[j]);
			for (int i = 0; i < 4; ++i) {
				glTexCoord2dv(texPos[i]); //テクスチャ座標
				glVertex3dv(vertex[face[j][i]]);
			}
		}
		
		glEnd();
	}glPopMatrix();
	
	

	glPushMatrix(); {
    // Assignment of texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, TEXWIDTH, TEXHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, texture2);
    // Designation of MAG and MIN for texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		
		glRotated(-(double)r * 0.1, 0.0, 1.0, 0.0); /*回転*/
		glTranslated(-0.5, 0.0, -0.5);
		glBegin(GL_QUADS);
		//四角すいを作る
		for (int j = 0; j < 5; ++j) {
			glNormal3dv(normal2[j]); //法線設定
			if (j < 4) {
				for (int i = 0; i < 4; ++i) {
					glTexCoord2dv(texPos2[i]);//テクスチャ座標
					glVertex3dv(vertex2[face2[j][i]]);
				}
			}
			else {
				for (int i = 0; i < 5; ++i) {
					glTexCoord2dv(texPos[j]); //テクスチャ座標
					glVertex3dv(vertex2[face3[i]]);
				}
			}
		}
		glEnd();
	}glPopMatrix();
 // テクスチャ変換終了
    glDisable(GL_TEXTURE_2D);
    
    // Stop shading
    glDisable(GL_DEPTH_TEST);
	 // Across operation
    glFlush();
	glutSwapBuffers();/*ダブルバッファリングの設定*/
	if ((r+=s) >= 3600||(r+=s)<=-3600) r = 0;
	
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
		case 'Q':
		case '\033':/*'\033'はESCのASCIIコード*/
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
		s=0;
		glutPostRedisplay(); /*再描画*/
		break;
	case GLUT_KEY_DOWN:
		h--;
		s=0;
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
