// Sample code for Übung 2
#include "stdafx.h"

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
//
/////////////////////////////////////////////////////////////


float sunPos[2] = {0, 0};
CVec2f sunVec(sunPos);

float earthPos[2] = {120, 0};
CVec2f earthVec(earthPos);
float earthAngle = 0.001;

float moonPos[2] = {170, 0};
CVec2f moonVec(moonPos);
float moonAngle = 0.01;

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

//Prototypes
void bhamLine(Point p1, Point p2, Color c);
void bhamCircle(Point p, int r, Color c);
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

CVec2f rotate(float angle, CVec2f vector)
{
	float matData[2][2] = {{ cos(angle), -sin(angle) },{ sin(angle), cos(angle) }};
	CMat2f newMat(matData);
	return newMat* vector;
}

// display callback function
void display1 (void) 
{
	glClear (GL_COLOR_BUFFER_BIT);

	///////
	// display your data here ...
	//

	earthVec = rotate(earthAngle, earthVec);
	moonVec -= earthVec;
	
	moonVec = rotate(moonAngle, moonVec);
	moonVec += earthVec;

	int rSun = 50;
	Point sun(sunVec(0), sunVec(1));
	for (int i = 0; i < rSun; i++) {		
		Color ccSun(255, 255, 0);
		bhamCircle(sun, i, ccSun);
	}

	int rEarth = 30;
	Point earth(earthVec(0), earthVec(1));
	for (int i = 0; i < rEarth; i++) {
		Color ccEarth(0, 0, 139);
		bhamCircle(earth, i, ccEarth);
	}

	int rMoon = 12;
	Point moon(moonVec(0), moonVec(1));
	for (int i = 0; i < rMoon; i++) {
		Color ccMoon(205, 179, 138);
		bhamCircle(moon, i, ccMoon);
	}
	//
	///////

	// In double buffer mode the last
	// two lines should alsways be
	glFlush ();
	glutSwapBuffers (); // swap front and back buffer
}

// display callback function
void display2 (void) 
{
	glClear (GL_COLOR_BUFFER_BIT);

	///////
	// display your data here ...
	//

	glBegin (GL_QUADS);
		glColor3f (1,0,0);
		glVertex2i (-g_vecPos(1), -g_vecPos(2));
		glColor3f (0,1,0);
		glVertex2i (g_vecPos(1), -g_vecPos(2));
		glColor3f (0,0,1);
		glVertex2i (g_vecPos(1), g_vecPos(2));
		glColor3f (1,1,0);
		glVertex2i (-g_vecPos(1), g_vecPos(2));
	glEnd ();

	//
	///////

	// In double buffer mode the last
	// two lines should alsways be
	glFlush ();
	glutSwapBuffers (); // swap front and back buffer
}

void keyboard (unsigned char key, int x, int y) 
{
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
		default:
			// do nothing ...
			break;
	};
}
///////////////////////////////////////////// bhamLine////////////////////////////////////////////////
bool a = false;
bool b = false;
bool c = false;
Point detransformPosition(Point point) {
	int tmp;
	// Fall y ist größer als x
	if (a) {
		tmp = point.y;
		point.y = point.x;
		point.x = tmp;
	}

	if (b) {
		point.x = -(point.x);
	}

	if (c) {
		point.y = -(point.y);
	}
	return point;
}
Point transformPosition(Point p) {
	int tmp;
	// Fall y ist negativ
	if (p.y < 0) {
		p.y = -(p.y);
		c = true;
	}

	// Fall x ist negativ
	if (p.x < 0) {
		p.x = -(p.x);
		b = true;
	}
	// Fall y ist größer als x
	if (p.y > p.x) {
		tmp = p.y;
		p.y = p.x;
		p.x = tmp;
		a = true;
	}
	return p;
}
void bhamLine(Point p1, Point p2, Color c) {
	int x, y, deltaX, deltaY, d, deltaNE, deltaE;

	Point nativePoint1 = p1;
	Point nativePoint2 = p2;

	p2.x = p2.x - p1.x;
	p2.y = p2.y - p1.y;

	//p1 = transformPosition(p1);
	p2 = transformPosition(p2);

	x = 0;
	y = 0;
	
	glBegin(GL_POINTS);
	glColor3f(c.r, c.g, c.b);
	
	// erster Punkt
	//setPoint(nativePoint1, c);
	glVertex2i(nativePoint1.x, nativePoint1.y);

	deltaX = p2.x - x;
	deltaY = p2.y - y;

	deltaNE = 2 * (deltaY - deltaX);
	deltaE = 2 * deltaY;

	d = 2 * (deltaY - deltaX);

	while (x <= p2.x) {
		if (d >= 0) {
			d += deltaNE;
			x++;
			y++;
		}
		else {
			d += deltaE;
			x++;
		}
		Point point(x, y);
		point = detransformPosition(point);
		point.x = point.x + nativePoint1.x;
		point.y = point.y + nativePoint1.y;
		glVertex2i(point.x, point.y);
		//setPoint(point, c);
	}

	// letzter Punkt
	glVertex2i(nativePoint2.x, nativePoint2.y);
	//setPoint(nativePoint2, c);
	glEnd();
}

/////////////////////////////////////////////////////bhamCircle/////////////////////////////////////////////
// ÜBUNG 1 AUFGABE 2:
//
// Diese Funktion soll einen Kreis mit Radius r um den Punkt p
// malen. Benutzen Sie die Funktion setPoint um die individuellen
// Punkte zu zeichnen. Vergessen Sie nicht auch den Mittelpunkt zu
// zeichnen!
void bhamCircle(Point p, int r, Color c) {
	int x, y, d, deltaSE, deltaE;
	x = 0;
	y = r;
	d = 5 - 4 * r;
	glBegin(GL_POINTS);
	glColor3f(c.r, c.g, c.b);

	Point point;
	point.x = x + p.x;
	point.y = y + p.y;
	//setPoint(point, c); //point(x,y)
	glVertex2i(point.x, point.y);

	point.x = -x + p.x;
	//setPoint(point, c); //point(-x,y)
	glVertex2i(point.x, point.y);

	point.y = -y + p.y;
	point.x = x + p.x;
	//setPoint(point, c); //point(x,-y)
	glVertex2i(point.x, point.y);

	point.x = -x + p.x;
	//setPoint(point, c); //point(-x,-y)
	glVertex2i(point.x, point.y);

	point.x = y + p.x;
	point.y = x + p.y;
	//setPoint(point, c); //point(y,x)
	glVertex2i(point.x, point.y);

	point.x = -y + p.x;
	//setPoint(point, c); //point(-y, x)
	glVertex2i(point.x, point.y);

	point.x = y + p.x;
	point.y = -x + p.y;
	//setPoint(point, c); //point(y, -x)
	glVertex2i(point.x, point.y);

	point.y = -x + p.y;
	point.x = -y + p.x;
	//setPoint(point, c); //point(-y, -x)
	glVertex2i(point.x, point.y);

	while (y > x) {
		if (d >= 0) {
			deltaSE = 4 * (2 * (x - y) + 5);
			d += deltaSE;
			x++;
			y--;
		}
		else {
			deltaE = 4 * (2 * x + 3);
			d += deltaE;
			x++;
		}
		Point point;
		point.x = x + p.x;
		point.y = y + p.y;
		//setPoint(point, c); //point(x,y)
		glVertex2i(point.x, point.y);

		point.x = -x + p.x;
		//setPoint(point, c); //point(-x,y)
		glVertex2i(point.x, point.y);

		point.y = -y + p.y;
		point.x = x + p.x;
		//setPoint(point, c); //point(x,-y)
		glVertex2i(point.x, point.y);

		point.x = -x + p.x;
		//setPoint(point, c); //point(-x,-y)
		glVertex2i(point.x, point.y);

		point.x = y + p.x;
		point.y = x + p.y;
		//setPoint(point, c); //point(y,x)
		glVertex2i(point.x, point.y);

		point.x = -y + p.x;
		//setPoint(point, c); //point(-y, x)
		glVertex2i(point.x, point.y);

		point.x = y + p.x;
		point.y = -x + p.y;
		//setPoint(point, c); //point(y, -x)
		glVertex2i(point.x, point.y);

		point.y = -x + p.y;
		point.x = -y + p.x;
		//setPoint(point, c); //point(-y, -x)
		glVertex2i(point.x, point.y);
	}

	//setPoint(p, c);
	glVertex2i(p.x, p.y);
	glEnd();
	// ...
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
