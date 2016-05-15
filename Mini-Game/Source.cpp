#include<GL/glut.h>
#include<vector>
#include<cmath>
#include<set>
#include <iostream>
#include "SOIL.h"
#include<thread>
#include<fstream>
#include<string>
#include<mutex>
#include<sstream>
#include<algorithm>
#include "point.h"
#include<map>
#include<exception>
#include "world.h"
#include "Loader.h"
using namespace std;

class world;
class loader;
class sprite;
class Human;
class enemy;
class bullet;

set<sprite*> ListOfMovingObjects;
class sprite {
public:
	sprite(vector<point>& Vertices, vector<vector<int>>& faces)
		:Vertices(move(Vertices)), faces(move(faces)) {
		ListOfMovingObjects.insert(this);
	}
	virtual void update() = 0;
	~sprite()
	{
		ListOfMovingObjects.erase(this);
	}
private:
	point Center;
	point Box[4];
	vector<point> Vertices;
	vector<vector<int>> faces;
};

class Human :private sprite {
public:
	Human(vector<point>& Vertices, vector<vector<int>>& Faces)
		:sprite(Vertices, Faces) {}
};

class player :private Human {
public:
	player(vector<point>& Vertices, vector<vector<int>>& Faces)
		:Human(Vertices, Faces) {}
};

class enemy :private sprite {

};

class bullet :private sprite {

};

class app {

};

world MapA;

int WinWidth = 1280;
int WinHeight = 720;

// actual vector representing the camera's direction
float lx = 0.0, lz = -1.0, ly = 0.0;
// XZ position of the camera
float EyeX = 20.0, EyeY = 20.0, EyeZ = 80.0;
// angle for rotating triangle
float angle = 0.0f;
float upangle = 0.0f;

int keyStates[256];
float deltaAngle = 0.0f;
float deltaMove = 0;


void SpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT: 
		keyStates[200] = 1;
		deltaAngle += -0.01f; 
		break;
	case GLUT_KEY_RIGHT: 
		keyStates[201] = 1;
		deltaAngle += 0.01f; 
		break;
	case GLUT_KEY_UP: 
		keyStates[202] = 1;
		deltaMove += 0.5f; 
		break;
	case GLUT_KEY_DOWN: 
		keyStates[203] = 1;
		deltaMove += -0.5f;
		break;
	}

}

void SpecialUpKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT: 
		keyStates[200] = 0;
		deltaAngle -= -0.01f; 
		break;
	case GLUT_KEY_RIGHT: 
		keyStates[201] = 0;
		deltaAngle -= 0.01f;
		break;
	case GLUT_KEY_UP: 
		keyStates[202] = 0;
		deltaMove = 0;
		break;
	case GLUT_KEY_DOWN: 
		keyStates[203] = 0;
		deltaMove -= -0.5f;
		break;
	}
}

void computePos(float deltaMove) {

	if (keyStates['q'] && keyStates[202])
		deltaMove += 2.5f;

	EyeX += deltaMove * lx * 0.1f;
	EyeZ += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {


	if (keyStates['w'] || keyStates['s'])
	{
		upangle += deltaAngle;
		ly = upangle;
	}
	else
	{
		angle += deltaAngle;

		lx = sin(angle);
		lz = -cos(angle);
	}
	
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		deltaAngle -= -0.01f;
		keyStates['w'] = 1;
		break;
	case 's':
		deltaAngle -= 0.01f;
		keyStates['s'] = 1;
		break;
	case 'q':
		keyStates['q'] = 1; 

		break;
	}
}

void myUpKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		keyStates['w'] = 0;
		deltaAngle = 0.0f;
		break;
	case 's':
		deltaAngle = 0.0f;
		keyStates['s'] = 0; 
		break;
	case 'q':
		keyStates['q'] = 0;
		break;
	}
}

void setLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float ambientLight0[] = { 0.0f, 0.0f, 0.0f, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);

	float diffuseLight0[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);

	float specularLight0[] = { 0.0f, 0.0f, 0.0f, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);

	float position0[] = { 20.0f, 40.0f, -50.0f, 0.5 };
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	float mcolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);

}

void init()
{
	for (int i = 0; i < 256; i++)
		keyStates[i] = 0;

	glClearColor(0.5, 0.5, 0.5, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH);
	//setLight();

	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the correct perspective.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myReshape(int w, int h)
{

	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, 1.0, 0.1f, -1000.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void myMouse(int b, int s, int x, int y)
{


}

void MouseMotion(int x, int y)
{

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	if (keyStates['q'] || keyStates[202] || keyStates[203])
		computePos(deltaMove);
	if (keyStates[200] || keyStates[201] || keyStates['w'] || keyStates['s'])
		computeDir(deltaAngle);

	// Set the camera
	gluLookAt(EyeX, EyeY, EyeZ,
		EyeX + lx, EyeY + ly, EyeZ + lz,
		0.0f, 1.0f, 0.0f);

	MapA.Draw();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WinWidth, WinHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("The Game");


	init();
	MapA.Load("Farmhouse.obj");
	glutDisplayFunc(&display);
	glutKeyboardFunc(&myKeyboard);
	glutKeyboardUpFunc(&myUpKeyboard);
	glutMouseFunc(&myMouse);
	glutMotionFunc(&MouseMotion);
	glutIdleFunc(&display);
	glutSpecialFunc(&SpecialKeys);
	glutSpecialUpFunc(&SpecialUpKeys);
	glutReshapeFunc(&myReshape);

	glutIgnoreKeyRepeat(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glutMainLoop();
	return 0;
}