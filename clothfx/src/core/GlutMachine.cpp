//
//  GlutMachine.cpp
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#include "GlutMachine.h"

using namespace Core;

Core::IListener* tkGlutMachine::listener = NULL;
Core::tkWindowInfo* tkGlutMachine::window = NULL;
unsigned int tkGlutMachine::fixedT = 10;

//----------------------------------------------------------------------
tkGlutMachine::tkGlutMachine(int argc, char **argv)
{
	window = new tkWindowInfo("Rigid Body - paper from Baraff & Witkin");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(window->GetTitle());
	glutReshapeWindow(window->GetWidth(), window->GetHeight());
}
//----------------------------------------------------------------------
tkGlutMachine::~tkGlutMachine()
{
	delete window;
	window = NULL;

	delete listener;
	listener = NULL;
}
//----------------------------------------------------------------------
void tkGlutMachine::InitGLUT(unsigned int t)
{
	fixedT = t;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(Display);
	glutKeyboardFunc(KeyPressed);
	glutReshapeFunc(ReshapeWindow);
	glutTimerFunc(t, TimerGL, 1);

	SetCameraGL();
}
//----------------------------------------------------------------------
 void tkGlutMachine::SetCameraGL()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, window->GetAspect(), 0.01f, 1000.0f);
	glRotatef(1.57f*10.f, 1.f, 0.f, 0.f);
	glTranslatef(0, -70, -180);
	glViewport(0, 0, window->GetWidth(), window->GetHeight());
}
//----------------------------------------------------------------------
void tkGlutMachine::Display()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);

	listener->Render();

	glutSwapBuffers();
}
//----------------------------------------------------------------------
void tkGlutMachine::RunGLUT()
{
	std::cout << "GLUT:\t Start running." << std::endl;
	glutMainLoop();
}
//----------------------------------------------------------------------
void tkGlutMachine::SetListener(Core::IListener *& iListener)
{
	listener = iListener;
}
//----------------------------------------------------------------------
void tkGlutMachine::KeyPressed(unsigned char key, int x, int y)
{
	listener->KeyPressed(key, x, y);
}
//----------------------------------------------------------------------
void tkGlutMachine::ReshapeWindow(int width, int height)
{
	window->SetDimensions(width, height);

	SetCameraGL();
}
//----------------------------------------------------------------------
void tkGlutMachine::TimerGL(int id)
{
	if (id == 1)
	{
		listener->Update();
		glutPostRedisplay();
		glutTimerFunc(fixedT, TimerGL, 1);
	}
}
//----------------------------------------------------------------------