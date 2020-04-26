#include <iostream>
#include <fstream>
#include <limits>
#include "core/Particle.h"
#include "core/Spring.h"
#include "core/Simulator.h"
#include "../../lib/freeglut/freeglut.h"
#include "math.h"

using namespace std;

//----------------------------------------------------------------//
//----------------------------------------------------------------//
// Simulator vars
CSimulator ropa;
float x[25][3]; // 25 particles for cloth
ofstream File("../data/positions.txt");

// OpenGL vars
GLfloat lightPos[] = {1.0,1.0,1.0,0}; // light position
int winWidth;	// window width
int winHeight;	// window height

// Other vars
float m_rotX = 0.0f;

// Method declaration
void InitGL(void);
void InitCloth(void);
void KeyEvent(unsigned char key, int x, int y);
void Render(void);
void Reshape(int,int);
void SimularTimer(int);
void SpecialKeyEvent(int key, int x, int y);
//----------------------------------------------------------------//
//----------------------------------------------------------------//
void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glMatrixMode(GL_MODELVIEW);

	int i;
	// Draw : Planes
	glLoadIdentity();
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3i(-50, 0, 50);
	glVertex3i(50, 0, 50);
	glVertex3i(50, 0, -50);
	glVertex3i(-50, 0, -50);
	glEnd();
	// Draw : Particles
	for (i = 0; i < ropa.getParticleCount(); i++)
	{
		if (i == 0 || i == 24)
		{
			glColor3f(0.f, 1.0f, 0.f);
		}
		else
		{
			glColor3f(1.0f, 0.f, 0.f);
		}
		glLoadIdentity();
		glTranslatef(x[i][0], x[i][1], x[i][2]);
		glutSolidSphere(ropa.getParticles()[i].GetRadius(), 10, 10);
	}
	// Draw : Cloth
	for (i = 0; i < ropa.getCloth().getTriangleCount(); i++)
	{
		glLoadIdentity();
		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1.0f, 1.0f, 1.0f);
		
		glVertex3f(ropa.getCloth().getTri()[i].getA()->GetPosition()[0],
				   ropa.getCloth().getTri()[i].getA()->GetPosition()[1],
				   ropa.getCloth().getTri()[i].getA()->GetPosition()[2]);
		
		glVertex3f(ropa.getCloth().getTri()[i].getB()->GetPosition()[0],
				   ropa.getCloth().getTri()[i].getB()->GetPosition()[1],
				   ropa.getCloth().getTri()[i].getB()->GetPosition()[2]);
		
		glVertex3f(ropa.getCloth().getTri()[i].getC()->GetPosition()[0],
				   ropa.getCloth().getTri()[i].getC()->GetPosition()[1],
				   ropa.getCloth().getTri()[i].getC()->GetPosition()[2]);
		
		glEnd();
	}
	// Draw : Sphere
	glLoadIdentity();
	glColor3f(0.5f, 0.5f, 1.0f);
	glTranslatef(2.f, 30.f, 0.f);
	glutSolidSphere(10, 40, 40);

	glutSwapBuffers();
}
//----------------------------------------------------------------//
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glLoadIdentity();
	float aspect = (float)winWidth / (float)winHeight;
	gluPerspective(35, aspect, 0.01, 1000);
}
//----------------------------------------------------------------//
void InitGL(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = (float)winWidth / (float)winHeight;
	gluPerspective(35, aspect, 0.01, 1000);
	glRotatef(1.57f*10.f, 1.f, 0.f, 0.f);
	glTranslatef(0, -70, -180);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//----------------------------------------------------------------//
void InitCloth()
{
	int i;
	int n_parts = 25;

	// Particle initial info
	float **X0;		// position matrix
	float	**V0;	// velocity matrix
	float	**UV;	// uv matrix
	float	*M;		// mass array
	float	*R;		// radius array
	// Plane definition
	float	**Pts1, **Pts2, **Pts3;
	/*
	Plane H:

		  Pt1
		  |\
		  | \
		  |  \
	   Pt3|___\Pt2

	*/

	M = new float[n_parts];
	R = new float[n_parts];
	X0 = new float *[n_parts];
	V0 = new float *[n_parts];
	UV = new float *[n_parts];
	for (i = 0; i < n_parts; i++)
	{
		X0[i] = new float[3];
		V0[i] = new float[3];
		UV[i] = new float[2];
	}
	int p_planes = 1;
	Pts1 = new float*[p_planes];
	Pts2 = new float*[p_planes];
	Pts3 = new float*[p_planes];

	float h = 0.015f;	// simulation step size
	float m = 1.0f;		// Particle mass
	float r = 1.0f;		// Particle radius;

	// Init plane
	float c_A[] = { 100.f,  0.f, 100.f },
		c_B[] = { 100.f,  0.f,-100.f },
		c_C[] = { -100.f,  0.f,-100.f };

	// plane H0: ground
	Pts1[0] = c_A;	Pts2[0] = c_B;	Pts3[0] = c_C;

	float posX = -10.0f, posY = 0.f, posZ = 10.0f;
	float uvU = 0, uvV = 0;

	// System init
	// Global coordinates

	int i_col = 0;
	for (int i_p = 0; i_p < n_parts; i_p++)
	{
		if (i_col == 0 || i_col == 5)
		{
			posX = -10.0f;
			posY = 70.0f;
			posZ -= 5.0f;
			uvU = 0.0f;
			uvV += 0.25f;
			i_col = 1;
		}
		else {
			i_col++;
		}

		X0[i_p][0] = posX;
		X0[i_p][1] = posY;
		X0[i_p][2] = posZ;
		posX += 5.0f;

		UV[i_p][0] = uvU;
		UV[i_p][1] = uvV;
		uvU += 0.25f;
	}

	for (i = 0; i < n_parts; i++)
	{
		V0[i][0] = V0[i][1] = V0[i][2] = 0.0f;
		M[i] = m;
		R[i] = r;
	}
	// Init cloth simulator
	ropa.InitializeSimulator(n_parts, 0, 1);
	ropa.InitializeParticleSystem(X0, V0, M, R);
	ropa.InitializeClothSystem(UV);
	ropa.InitializePlaneSystem(Pts1, Pts2, Pts3);
	ropa.setSimStep(h);

	// Cleaning memory
	for (i = 0; i < n_parts; i++)
	{
		delete[] X0[i];	delete[] V0[i];	delete[] UV[i];
	}
	delete[] X0; delete[] V0; delete[] UV;
	delete[] M; delete[] R;

	delete[] Pts1; delete[] Pts2; delete[] Pts3;
}
//----------------------------------------------------------------//
void SimularTimer(int timerID)
{
	if (timerID == 1)
	{
		int i, j;
		ropa.SimStep();
		float **f = ropa.getPhase();

		// Spacial phase extraction
		for (i = 0; i < ropa.getParticleCount(); i++)
		{
			for (j = 0; j < 3; j++)
				x[i][j] = f[i][j];
		}
		glutTimerFunc(5, SimularTimer, 1);
		glutPostRedisplay();
	}
}
//----------------------------------------------------------------//
void KeyEvent(unsigned char key, int p_x, int p_y)
{
	glMatrixMode(GL_MODELVIEW);

	if (key == 27)
		exit(0);

	if (key == 's' || key == 'S')
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (key == 'w' || key == 'W')
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (key == 'r' || key == 'R')
		InitCloth();

	if (key == '1')
		glEnable(GL_CULL_FACE);

	if (key == '2')
		glDisable(GL_CULL_FACE);

	glutPostRedisplay();
}
//----------------------------------------------------------------//
void SpecialKeyEvent(int key, int x, int y)
{
	glMatrixMode(GL_MODELVIEW);
	if (key == GLUT_KEY_LEFT)
	{
		m_rotX += 5.0;
	}
}
//----------------------------------------------------------------//
int main(int argc, char **argv)
{
	cout << "Cloth Simulation" << endl;

	InitCloth();

	winWidth = 640;
	winHeight = 480;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Cloth Simulation Engine");

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Render);
	glutKeyboardFunc(KeyEvent);
	glutSpecialFunc(SpecialKeyEvent);

	InitGL();

	SimularTimer(1);
	glutMainLoop();

	return 0;
}
//----------------------------------------------------------------//