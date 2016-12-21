// Sample code for Übung 2
#include "stdafx.h"
#include <iostream>
using namespace std;

#include "vec.h"
#include "mat.h"
#include "math.h"

// might be you have to swith to
// #include "glut.h" depending on your GLUT installation
#include "Uebung2/Uebung2/glut.h"

////////////////////////////////////////////////////////////
//
// system relevant global variables
//

// window width and height (choose an appropriate size)
const int g_iWidth  = 400;
const int g_iHeight = 400;

// global variable to tune the timer interval
int g_iTimerMSecs;

//
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// private, global variables ... replace by your own ones
//
// some global state variables used to describe ...
float g_iPos;		// ... position and ...
float g_iPosIncr;	// ... position increment (used in display1)

CVec2i g_vecPos;		// same as above but in vector form ...
CVec2i g_vecPosIncr;	// (used in display2)

bool swapXY, reflectX, reflectY = false;
float fFocus = 40.5;

float eyeP[4] = { 0, 0, 0, 1 };
CVec4f eyePoint(eyeP);

float viewD[4] = { 0, 0, -1, 0 };
CVec4f viewDir(viewD);

float viewU[4] = { 0, 1, 0, 0 };
CVec4f viewUp(viewU);

//
/////////////////////////////////////////////////////////////

// Eine überaus primitive Punktklasse
class Point {
public:

	Point(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
	}

	int x, y;
};

// Eine überaus primitive Farbklasse
class Color {
public:

	Color(float r = 1.0f, float g = 1.0f, float b = 1.0f) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	float r, g, b;
};

Color yellow(1, 1, 0);
Color red(1, 0, 0);
Color blue(0, 0, 1);

/////////////////////////////////
// Prototypes
void bhamCircle(Point p, int r, Color c);
void bhamLine(Point p1, Point p2, Color c);
CVec4f projectZ(float fFocus, CVec4f pSicht);
void drawProjektedZ(CVec3f Points[8], Color c);
void drawQuader(CVec3f Quader[8], float fFocus, Color c);
CMat4f getTransform(CVec4f viewOrigin, CVec4f viewDir, CVec4f viewUp);
CVec4f toNorm(CVec4f vec);

// function to initialize our own variables
void init () 
{

	// init timer interval
	g_iTimerMSecs = 10;

	// init variables for display1
	g_iPos     = 0;
	g_iPosIncr = 2;

	// init variables for display2
	int aiPos    [2] = {0, 0};
	int aiPosIncr[2] = {2, 2};
	g_vecPos.setData (aiPos);
	g_vecPosIncr.setData (aiPosIncr);
}

// function to initialize the view to ortho-projection
void initGL () 
{
	glViewport (0, 0, g_iWidth, g_iHeight);	// Establish viewing area to cover entire window.

	glMatrixMode (GL_PROJECTION);			// Start modifying the projection matrix.
	glLoadIdentity ();						// Reset project matrix.
	glOrtho (-g_iWidth/2, g_iWidth/2, -g_iHeight/2, g_iHeight/2, 0, 1);	// Map abstract coords directly to window coords.

	// tell GL that we draw to the back buffer and
	// swap buffers when image is ready to avoid flickering
	glDrawBuffer (GL_BACK);

	// tell which color to use to clear image
	glClearColor (0,0,0,1);
}


int min (int a, int b) { return a>b? a: b; }
// timer callback function
void timer (int value) 
{
	///////
	// update your variables here ...
	//
	//earthRotation = earthRotation + 0.01;
	//moonRotation = moonRotation + 0.05;

	int size2 = min (g_iWidth, g_iHeight) / 2;

	// variables for display1 ...
	if (g_iPos<=-size2 || g_iPos>=size2) g_iPosIncr = -g_iPosIncr;
	g_iPos += g_iPosIncr;

	// variables for display2 ...
	if (g_vecPos(1)<=-size2 || g_vecPos(1)>=size2) g_vecPosIncr = -g_vecPosIncr; 
	g_vecPos += g_vecPosIncr;

	//
	///////

	// the last two lines should always be
	glutPostRedisplay ();
	glutTimerFunc (g_iTimerMSecs, timer, 0);	// call timer for next iteration
}

// display callback function
void display1 (void) 
{
	glClear (GL_COLOR_BUFFER_BIT);

	CVec3f quader[8];
	float p0[3] = {0, 0, 30};
	float p1[3] = {30, 0, 30};
	float p2[3] = {30, 20, 30};
	float p3[3] = {0, 20, 30};
	float p4[3] = {0, 0, 0};
	float p5[3] = {30, 0, 0};
	float p6[3] = {30, 20, 0};
	float p7[3] = {0, 20, 0};
	quader[0] = CVec3f(p0);
	quader[1] = CVec3f(p1);
	quader[2] = CVec3f(p2);
	quader[3] = CVec3f(p3);
	quader[4] = CVec3f(p4);
	quader[5] = CVec3f(p5);
	quader[6] = CVec3f(p6);
	quader[7] = CVec3f(p7);

	CVec3f quader1[8];
	float p01[3] = { -40,-40,20 };
	float p11[3] = { -20,-40,20 };
	float p21[3] = { -20,-20,20 };
	float p31[3] = { -40,-20,20 };
	float p41[3] = { -40,-40, -10 };
	float p51[3] = { -20,-40, -10 };
	float p61[3] = { -20,-20, -10 };
	float p71[3] = { -40,-20, -10 };
	quader1[0] = CVec3f(p01);
	quader1[1] = CVec3f(p11);
	quader1[2] = CVec3f(p21);
	quader1[3] = CVec3f(p31);
	quader1[4] = CVec3f(p41);
	quader1[5] = CVec3f(p51);
	quader1[6] = CVec3f(p61);
	quader1[7] = CVec3f(p71);

	CVec3f quader2[8];
	float p02[3] = { 30, -150, -20 };
	float p12[3] = { 110, -150, -20 };
	float p22[3] = { 110, 130, -20 };
	float p32[3] = { 30, 130, -20 };
	float p42[3] = { 30, -150, -100 };
	float p52[3] = { 110, -150, -100 };
	float p62[3] = { 110, 130, -100 };
	float p72[3] = { 30, 130, -100 };
	quader2[0] = CVec3f(p02);
	quader2[1] = CVec3f(p12);
	quader2[2] = CVec3f(p22);
	quader2[3] = CVec3f(p32);
	quader2[4] = CVec3f(p42);
	quader2[5] = CVec3f(p52);
	quader2[6] = CVec3f(p62);
	quader2[7] = CVec3f(p72);

	drawProjektedZ(quader, blue);
	drawProjektedZ(quader1, red);
	drawProjektedZ(quader2, yellow);

	// In double buffer mode the last
	// two lines should alsways be
	glFlush ();
	glutSwapBuffers (); // swap front and back buffer
}

// display callback function
void display2 (void) 
{
	glClear (GL_COLOR_BUFFER_BIT);

	CVec3f quader[8];
	float p0[3] = { 0, 0, 30 };
	float p1[3] = { 30, 0, 30 };
	float p2[3] = { 30, 20, 30 };
	float p3[3] = { 0, 20, 30 };
	float p4[3] = { 0, 0, 0 };
	float p5[3] = { 30, 0, 0 };
	float p6[3] = { 30, 20, 0 };
	float p7[3] = { 0, 20, 0 };
	quader[0] = CVec3f(p0);
	quader[1] = CVec3f(p1);
	quader[2] = CVec3f(p2);
	quader[3] = CVec3f(p3);
	quader[4] = CVec3f(p4);
	quader[5] = CVec3f(p5);
	quader[6] = CVec3f(p6);
	quader[7] = CVec3f(p7);

	CVec3f quader1[8];
	float p01[3] = { -40,-40,20 };
	float p11[3] = { -20,-40,20 };
	float p21[3] = { -20,-20,20 };
	float p31[3] = { -40,-20,20 };
	float p41[3] = { -40,-40, -10 };
	float p51[3] = { -20,-40, -10 };
	float p61[3] = { -20,-20, -10 };
	float p71[3] = { -40,-20, -10 };
	quader1[0] = CVec3f(p01);
	quader1[1] = CVec3f(p11);
	quader1[2] = CVec3f(p21);
	quader1[3] = CVec3f(p31);
	quader1[4] = CVec3f(p41);
	quader1[5] = CVec3f(p51);
	quader1[6] = CVec3f(p61);
	quader1[7] = CVec3f(p71);

	CVec3f quader2[8];
	float p02[3] = { 30, -150, -20 };
	float p12[3] = { 110, -150, -20 };
	float p22[3] = { 110, 130, -20 };
	float p32[3] = { 30, 130, -20 };
	float p42[3] = { 30, -150, -100 };
	float p52[3] = { 110, -150, -100 };
	float p62[3] = { 110, 130, -100 };
	float p72[3] = { 30, 130, -100 };
	quader2[0] = CVec3f(p02);
	quader2[1] = CVec3f(p12);
	quader2[2] = CVec3f(p22);
	quader2[3] = CVec3f(p32);
	quader2[4] = CVec3f(p42);
	quader2[5] = CVec3f(p52);
	quader2[6] = CVec3f(p62);
	quader2[7] = CVec3f(p72);



	drawQuader(quader, fFocus, blue);
	drawQuader(quader1, fFocus, red);
	drawQuader(quader2, fFocus, yellow);

	// In double buffer mode the last
	// two lines should alsways be
	glFlush ();
	glutSwapBuffers (); // swap front and back buffer
}

CVec4f projectZ(float fFocus, CVec4f pSicht) {
	//float matrix[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, -1 / fFocus, 1} };
	//CMat4f M = matrix; 
	//pProject = M * pSicht; 

	CVec4f pProject;
	pProject(0) = pSicht(0) * (fFocus / (fFocus - pSicht(2)));
	pProject(1) = pSicht(1) * (fFocus / (fFocus - pSicht(2)));
	pProject(2) = 0;
	pProject(3) = 1;
	/*pProject(0) = pProject(0) / pProject(3);
	pProject(1) = pProject(1) / pProject(3);
	pProject(2) = pProject(2) / pProject(3);
	pProject(3) = 1;*/
	return pProject;
}

void drawLine(CVec3f point1, CVec3f point2, Color c) {
	Point p1(point1(0), point1(1));
	Point p2(point2(0), point2(1));

	bhamLine(p1, p2, c);
}

// Points[8] = A B C D E F G H
void drawProjektedZ(CVec3f Points[8], Color c) {
	drawLine(Points[0], Points[1], c);
	drawLine(Points[1], Points[2], c);
	drawLine(Points[2], Points[3], c);
	drawLine(Points[0], Points[3], c);

	drawLine(Points[4], Points[5], c);
	drawLine(Points[5], Points[6], c);
	drawLine(Points[6], Points[7], c);
	drawLine(Points[4], Points[7], c);

	drawLine(Points[0], Points[4], c);
	drawLine(Points[1], Points[5], c);
	drawLine(Points[2], Points[6], c);
	drawLine(Points[3], Points[7], c);
	
}


CVec4f toNorm(CVec4f vec) {
	float vecLength = sqrt(pow(vec(0), 2) + pow(vec(1), 2) + pow(vec(2), 2));
	vec(0) = vec(0) / vecLength;
	vec(1) = vec(1) / vecLength;
	vec(2) = vec(2) / vecLength;
	return vec;
}

void drawQuader(CVec3f Quader[8], float fFocus, Color c) {

	// normieren
	viewDir = toNorm(viewDir);
	viewUp = toNorm(viewUp);
	////////////

	CMat4f transform = getTransform(eyePoint, viewDir, viewUp);

	for (int i = 0; i < 8; i++) {		
		CVec4f tmp;
		tmp(0) = Quader[i](0);
		tmp(1) = Quader[i](1);
		tmp(2) = Quader[i](2);
		tmp(3) = 1;

		tmp = transform * tmp;
		tmp = projectZ(fFocus, tmp);


		Quader[i](0) = tmp(0);
		Quader[i](1) = tmp(1);
		Quader[i](2) = tmp(2);
	}
	drawProjektedZ(Quader, c);
}

CMat4f getTransform(CVec4f viewOrigin, CVec4f viewDir, CVec4f viewUp) {
	CVec4f viewHorizon;
	viewHorizon(0) = (viewDir(1) * viewUp(2) - viewDir(2) * viewUp(1)); //crossproduct
	viewHorizon(1) = (viewDir(2) * viewUp(0) - viewDir(0) * viewUp(2));
	viewHorizon(2) = (viewDir(0) * viewUp(1) - viewDir(1) * viewUp(0));
	viewHorizon(3) = 0;

	float transf[4][4] = {{viewHorizon(0), viewUp(0), viewDir(0), 0},
						{viewHorizon(1), viewUp(1), viewDir(1), 0 },
						{viewHorizon(2), viewUp(2), viewDir(2), 0 },
						{ 0, 0, 0, 1 }};
	
	CMat4f transform(transf);

	float transpon[3][3] = { {viewHorizon(0), viewHorizon(1), viewHorizon(2)},
							{viewUp(0) , viewUp(1),  viewUp(2)},
							{viewDir(0), viewDir(1), viewDir(2)}};
	CMat3f transponiert(transpon);

	float viewO[3] = { -viewOrigin(0), -viewOrigin(1), -viewOrigin(2)};
	CVec3f viewOriginMat3(viewO);

	CVec3f transformTransp = transponiert * viewOriginMat3 ;

	float inverse[4][4] = { { viewHorizon(0), viewHorizon(1), viewHorizon(2), transformTransp(0) },
							{ viewUp(0) , viewUp(1),  viewUp(2), transformTransp(1) },
							{ viewDir(0), viewDir(1), viewDir(2), transformTransp(2) },
							{0, 0, 0, 1} };

	CMat4f inverseMatrix(inverse);
	return inverseMatrix;
}
/////////////////bhamLine/////////////////////

Point transformPoint(Point p) {
	reflectX = false;
	reflectY = false;
	swapXY = false;
	if (p.x < 0) {
		p.x = -(p.x);
		reflectX = true;
	}

	if (p.y < 0) {
		p.y = -(p.y);
		reflectY = true;
	}

	if (p.y > p.x)
	{
		int tmp = p.y;
		p.y = p.x;
		p.x = tmp;
		swapXY = true;
	}
	return p;
}

Point retransformPoint(Point p) {
	if (swapXY)
	{
		int tmp = p.y;
		p.y = p.x;
		p.x = tmp;
	}

	if (reflectX) {
		p.x = -(p.x);
	}

	if (reflectY) {
		p.y = -(p.y);
	}
	return p;
}

void bhamLine(Point p1, Point p2, Color c) {

	int x1, y1, dx, dy, d, dNE, dE;

	x1 = p1.x;
	y1 = p1.y;

	glBegin(GL_POINTS);
	glColor3f(c.r, c.g, c.b);
	// erster Punkt
	glVertex2i((GLint)x1, (GLint)y1);

	p2.x = p2.x - p1.x;
	p2.y = p2.y - p1.y;
	x1 = 0;
	y1 = 0;

	p2 = transformPoint(p2);

	dx = p2.x - x1;
	dy = p2.y - y1;
	dNE = 2 * (dy - dx);
	dE = 2 * dy;
	d = 2 * dy - dx;

	while (x1 < p2.x) {
		if (d >= 0) {
			d += dNE;
			x1++;
			y1++;
		}
		else {
			d += dE;
			x1++;
		}

		Point p(x1, y1);
		p = retransformPoint(p);
		p.x = p.x + p1.x;
		p.y = p.y + p1.y;
		glVertex2i((GLint)p.x, (GLint)p.y);
	}

	retransformPoint(p2);
	// letzter Punkt
	//glVertex2i((GLint)p2.x, (GLint)p2.y);

	glEnd();
}


CVec3f translate3D(CVec3f p, CVec3f t) {
	float arr[4][4] = { { 1, 0, 0, t(0) },{ 0, 1, 0, t(1) },{ 0, 0, 1, t(2) },{ 0, 0, 0, 1 } };
	float pArr[4] = { p(0), p(1), p(2), 1 };
	CVec4f res = CMat4f(arr) * CVec4f(pArr);
	float resArr[3] = { res(0), res(1), res(2) };
	return CVec3f(resArr);
}

CVec4f translate3D(CVec4f p, CVec4f t) {
	float arr[4][4] = { { 1, 0, 0, t(0) },{ 0, 1, 0, t(1) },{ 0, 0, 1, t(2) },{ 0, 0, 0, 1 } };
	float pArr[4] = { p(0), p(1), p(2), 1 };
	return CMat4f(arr) * CVec4f(pArr);
}

/*CVec3f scale3D(CVec3f p, float x, float y, float z) {
	float arr[4][4] = { { x, 0, 0, 0 },{ 0, y, 0, 0 },{ 0, 0, z, 0 },{ 0, 0, 0, 1 } };
	float pArr[4] = { p(0), p(1), p(2), 1 };
	CVec4f res = CVec4f(pArr) * CMat4f(arr);
	float resArr[3] = { res(0), res(1), res(2) };
	return CVec3f(resArr);
}*/

CVec4f rotate3D_Z(CVec4f p, float angle) {
	float arr[4][4] = { { cos(angle), -sin(angle), 0, 0 },{ sin(angle), cos(angle), 0, 0 },{ 0, 0, 1, 0 },{ 0, 0, 0, 1 } };
	return p * CMat4f(arr);
}

CVec4f rotate3D_Y(CVec4f p, float angle) {
	float arr[4][4] = { { cos(angle), 0, sin(angle), 0 },{ 0, 1, 0, 0 },{ -sin(angle), 0, cos(angle), 0 } ,{ 0, 0, 0, 1 } };
	return p * CMat4f(arr);
}

CVec4f rotate3D_X(CVec4f p, float angle) {
	float arr[4][4] = { { 1, 0, 0, 0 },{ 0, cos(angle), -sin(angle), 0 },{ 0, sin(angle), cos(angle), 0 },{ 0, 0, 0, 1 } };
	return p * CMat4f(arr);
}

CVec4f rotate(CVec4f getRotate, float angle, CVec4f toRotate) {
	getRotate = toNorm(getRotate);

	float d = sqrt(pow(getRotate(0), 2) + pow(getRotate(1), 2));

	float rz[4][4] = { { getRotate(0) / d, getRotate(1) / d, 0, 0},
						{ -getRotate(1) / d, getRotate(0) / d, 0, 0},
						{0, 0, 1, 0},
						{0, 0, 0, 1} };

	float rzinv[4][4] = { {getRotate(0) / d, - getRotate(1) / d, 0, 0 },
							{ getRotate(1) / d, getRotate(0) / d, 0, 0 },
							{ 0, 0, 1, 0 },
							{ 0, 0, 0, 1 } };

	float rt[4][4] = { {1, 0, 0, -toRotate(0)},
						{0, 1, 0, -toRotate(1)},
						{0, 0, 1, -toRotate(2)},
						{0, 0, 0, 1} };

	float rtinv[4][4] = { { 1, 0, 0, toRotate(0) },
						{ 0, 1, 0, toRotate(1) },
						{ 0, 0, 1, toRotate(2) },
						{ 0, 0, 0, 1 } }; 

	float ry[4][4] = { { getRotate(2), 0, -d, 0},
						{0, 1, 0, 0},
						{d, 0, getRotate(2), 0},
						{0, 0, 0, 1} };

	float ryinv[4][4] = { { getRotate(2), 0, d, 0 },
						{ 0, 1, 0, 0 },
						{ -d, 0, getRotate(2), 0 },
						{ 0, 0, 0, 1 } };

	float rzAngle[4][4] = { { cos(angle), -sin(angle), 0, 0 },{ sin(angle), cos(angle), 0, 0 },{ 0, 0, 1, 0 },{ 0, 0, 0, 1 } };


	CVec4f result = CMat4f(rtinv) * CMat4f(rzinv) * CMat4f(ryinv) * CMat4f(rzAngle) * CMat4f(ry) * CMat4f(rz) * CMat4f(rt) * toRotate;
	return result;
}

void keyboard (unsigned char key, int x, int y) 
{
	float arr1[4] = { 1, 0, 0, 0 };
	float arr2[4] = { -1, 0, 0, 0 };
	float arr3[4] = { 0, 1, 0, 0 };
	float arr4[4] = { 0, -1, 0, 0 };
	float arr5[4] = { 0, 0, 1, 0 };
	float arr6[4] = { 0, 0, -1, 0 };
	float eyeArr[4] = { 0, 0, 0, 1 };
	float dirArr[4] = { 0, 0, -1, 0 };
	float upArr[4] = { 0, 1, 0, 0 };
	CVec4f tmp;
	CVec4f viewHorizon;

	switch (key) {
		case 'q':
		case 'Q':
			exit (0); // quit program
			break;
		case '1':
			glutDisplayFunc (display1);
			//glutPostRedisplay ();	// not needed since timer triggers redisplay
			break;
		case '2':
			glutDisplayFunc (display2);
			//glutPostRedisplay ();	// not needed since timer triggers redisplay
			break;
		case 'f':
			fFocus -= 1;
			printf("%d/n" , fFocus);
			//fokallänge verkleinern
			break;
		case 'F':
			fFocus += 1;
			printf("%d/n", fFocus);
			//fokallänge vergrößern
			break;
		case 'X':
			eyePoint = rotate3D_X(eyePoint, 0.01);
			viewDir = rotate3D_X(viewDir, 0.01);
			viewUp = rotate3D_X(viewUp, 0.01);
			//rotier positiv um die X-Achse des Weltkoordinatensystems
			break;
		case 'Y':
			eyePoint = rotate3D_Y(eyePoint, 0.01);
			viewDir = rotate3D_Y(viewDir, 0.01);
			viewUp = rotate3D_Y(viewUp, 0.01);
			break;
		case 'Z':
			eyePoint = rotate3D_Z(eyePoint, 0.01);
			viewUp = rotate3D_Z(viewUp, 0.01);
			viewDir = rotate3D_Z(viewDir, 0.01);
			break;
		case 'x':
			eyePoint = rotate3D_X(eyePoint, -0.01);
			viewDir = rotate3D_X(viewDir, -0.01);
			viewUp = rotate3D_X(viewUp, -0.01);
			//rotier negativ um die X-Achse des Weltkoordinatensystems
			break;
		case 'y':
			eyePoint = rotate3D_Y(eyePoint, -0.01);
			viewDir = rotate3D_Y(viewDir, -0.01);
			viewUp = rotate3D_Y(viewUp, -0.01);
			break;
		case 'z':
			eyePoint = rotate3D_Z(eyePoint, -0.01);
			viewUp = rotate3D_Z(viewUp, -0.01);
			viewDir = rotate3D_Z(viewDir, -0.01);
			break;
		case 'A': 
			//dreht sich um viewDir
			//viewup = rotate(viewdir,0.01, viewup)
			//die ich drehen will = rot (um die ich drehen will, winkel, die ich drehen will)
			viewUp = rotate(viewDir, 0.1, viewUp);
	
			//rotier positiv um X-Achse des Sichtkoordinatensystemns
			break;
		case 'B': //dreht sich um viewup
			viewDir = rotate(viewUp, 0.1, viewDir);
			//rotier positiv um y-Achse des Sichtkoordinatensystemns
			break;
		case 'C': //dreht sich um horizont (kreuzprodukt von viewdir und viewup)
			viewHorizon(0) = (viewDir(1) * viewUp(2) - viewDir(2) * viewUp(1)); //crossproduct
			viewHorizon(1) = (viewDir(2) * viewUp(0) - viewDir(0) * viewUp(2));
			viewHorizon(2) = (viewDir(0) * viewUp(1) - viewDir(1) * viewUp(0));
			viewHorizon(3) = 0;
			
			viewDir = rotate(viewHorizon, 0.1, viewDir);
			viewUp = rotate(viewHorizon, 0.1, viewUp);
			
			break;
		case 'a':
			viewUp = rotate(viewDir, -0.1, viewUp);
			//rotier negativ um X-Achse des Sichtkoordinatensystemns
			break;
		case 'b':
			viewDir = rotate(viewUp, -0.1, viewDir);
			//rotier negativ um y-Achse des Sichtkoordinatensystemns
			break;
		case 'c':
			
			viewHorizon(0) = (viewDir(1) * viewUp(2) - viewDir(2) * viewUp(1)); //crossproduct
			viewHorizon(1) = (viewDir(2) * viewUp(0) - viewDir(0) * viewUp(2));
			viewHorizon(2) = (viewDir(0) * viewUp(1) - viewDir(1) * viewUp(0));
			viewHorizon(3) = 0;

			viewDir = rotate(viewHorizon, -0.1, viewDir);
			viewUp = rotate(viewHorizon, -0.1, viewUp);
			//???
			break;
		case 'U':
			eyePoint = translate3D(eyePoint, CVec4f(arr1));
			//verschieb positiv auf der X-Achse des Weltkoordinatensystems
			break;
		case 'V':
			eyePoint = translate3D(eyePoint, CVec4f(arr3));
			break;
		case 'W':
			eyePoint = translate3D(eyePoint, CVec4f(arr5));
			break;
		case 'u':
			eyePoint = translate3D(eyePoint, CVec4f(arr2));
			//verschieb negativ auf der X-Achse des Weltkoordinatensystems
			break;
		case 'v':
			eyePoint = translate3D(eyePoint, CVec4f(arr4));
			break;
		case 'w':
			eyePoint = translate3D(eyePoint, CVec4f(arr6));
			break;
		case 'R':
		case 'r':
			eyePoint = CVec4f(eyeArr);
			viewDir = CVec4f(dirArr);
			viewUp = CVec4f(upArr);
			fFocus = 50.5;
			break;
		default:
			// do nothing ...
			break;
	};
}

int main (int argc, char **argv) 
{
	glutInit (&argc, argv);
	// we have to use double buffer to avoid flickering
	// TODO: lookup "double buffer", what is it for, how is it used ...
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutCreateWindow ("Übung 2");

	init ();	// init my variables first
	initGL ();	// init the GL (i.e. view settings, ...)

	// assign callbacks
	glutTimerFunc (10, timer, 0);
	glutKeyboardFunc (keyboard);
	glutDisplayFunc (display1);
	// you might want to add a resize function analog to
	// Übung1 using code similar to the initGL function ...

	// start main loop
	glutMainLoop ();

	return 0;
}