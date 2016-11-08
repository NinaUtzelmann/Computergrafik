///////////////////////////////////////////////////////////////////////
//										                             //
// Codegerüst für Vorlesung Computergraphik WS 2011/12 Übung 1       //
//										                             //
///////////////////////////////////////////////////////////////////////


// Include-File für die Text Ein-/Ausgabe
#include "stdafx.h"
#include <iostream>
using namespace std;

// Include-File für die GLUT-Library
#include "glut.h"



/////////////////////////////////////////////////////////////////////////////////////////
//
// Hier wird einiges initialisiert. Sie dürfen sich gerne den Code anschauen und
// versuchen ihn zu verstehen. Das sollte Ihnen nicht allzu schwer fallen. Es werden
// jedoch einige Techniken benutzt (Texturen, ...) die wir in der Vorlesung noch
// nicht besprochen haben, die wir aber aus techn. Gründen hier benötigen.
//
// Weiter unten finden Sie einen Vermerk, ab dem Sie den Code jedoch vollständig
// verstehen sollten!!!


// Auflösungen der gesamten Textur
// !!!ACHTUNG!!! nicht alle Texturauflösungen funktionieren!
// Stichwort ungefähr: POT2 Problematik
#define TEX_RES_X 60
#define TEX_RES_Y 60
// Anzahl der Pixel der Textur
#define TEX_RES TEX_RES_X*TEX_RES_Y
// Achsenlänge der Textur (Achsen sind asymmetrisch von -TexRes#/2 bis TesRes#/2-1)
#define TEX_HALF_X TEX_RES_X/2
#define TEX_HALF_Y TEX_RES_Y/2
// Konvertiert Indices von (x,y) Koordinaten in ein lineares Array
#define TO_LINEAR(x, y) (((x)) + TEX_RES_X*((y)))

// globaler Speicher für Texturdaten
char g_Buffer[3*TEX_RES];
// Textur ID Name
GLuint g_TexID = 0;

// Auflösung des Hauptfensters (kann sich durch User ändern)
int g_WinWidth = 800;
int g_WinHeight = 800;

// Funktion organisiert die Textur.
// Kümmern Sie sich nicht weiter um diese Funktion, da
// sie momentan nur ein notwendiges Übel darstellt!
void manageTexture () {

	glEnable (GL_TEXTURE_2D);

	if (g_TexID==0)	glGenTextures (1, &g_TexID);

	glBindTexture (GL_TEXTURE_2D, g_TexID);

	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, TEX_RES_X, TEX_RES_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, g_Buffer);
	
	glBindTexture (GL_TEXTURE_2D, 0);

	glDisable (GL_TEXTURE_2D);
}

// Callback Funktion um die Fenstergrößen anzupassen.
// Auch diese Funktion ist ein notwendiges Übel! Kümmern
// Sie sich im Moment nicht weiter darum.
void reshape(int w, int h) {

	g_WinWidth = w;
	g_WinHeight = h;

	glViewport(0, 0, w, h);					// Establish viewing area to cover entire window.

	glMatrixMode(GL_PROJECTION);			// Start modifying the projection matrix.
	glLoadIdentity();						// Reset project matrix.
	glOrtho(-w/2, w/2, -h/2, h/2, 0, 1);	// Map abstract coords directly to window coords.

	glutPostRedisplay ();
}




//
//
/////////////////////////////////////////////////////////////////////////////////////////
//
// Hier fängt der für Sie wirklich relevante Teil des Programms an.
//




// Eine überaus primitive Punktklasse
class Point {
public:

	Point (int x=0, int y=0) {
		this->x = x;
		this->y = y;
	}

	int x, y;
};

// Eine überaus primitive Farbklasse
class Color {
public:

	Color (float r=1.0f, float g=1.0f, float b=1.0f) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	float r, g, b;
};

// Funktion löscht den Bildschirm mit der angegebenen Farbe
// Usage z.B.: clearImage (Color (1,1,1))
// löscht den Bildschirm in Weiß.
// Ohne Farbangabe ist der Standard Weiß
void clearImage (Color c=Color()) {
	for (int i=0; i<TEX_RES; i++) {
		g_Buffer[3*i  ] = 255.0*c.r;
		g_Buffer[3*i+1] = 255.0*c.g;
		g_Buffer[3*i+2] = 255.0*c.b;
	}
}

// Funktion malt einen Punkt an die angegebenen Koordinaten
// Usage z.B.: setPoint (Point(10, 5), Color (1,0,0))
// malt einen Punkt an die Koordinate (10, 5)
// Ohne Farbangabe ist die standard Malfarbe Schwarz
//
// Nutzen Sie diese Funktion ...
void setPoint (Point p, Color c=Color(0,0,0)) {
	int x = p.x + TEX_HALF_X;
	int y = p.y + TEX_HALF_Y;
	if (x < 0 || y < 0 || x >= TEX_RES_X || y >= TEX_RES_Y) {
		cerr << "Illegal point co-ordinates (" << p.x << ", " << p.y << ")\n" << flush;
		return;
	}

	g_Buffer[3*TO_LINEAR (x, y)  ] = 255.0*c.r;
	g_Buffer[3*TO_LINEAR (x, y)+1] = 255.0*c.g;
	g_Buffer[3*TO_LINEAR (x, y)+2] = 255.0*c.b;
}

//
// ÜBUNG 1 AUFGABE 1:
//
// Diese Funktion soll eine Gerade zwischen den Punkten
// p1 und p2 in der Farbe c malen. Benutzen Sie die Funktion
// setPoint um die individuellen Punkte zu zeichnen.
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
void bhamLine (Point p1, Point p2, Color c) {
	int x, y, deltaX, deltaY, d, deltaNE, deltaE;
	
	Point nativePoint1 = p1;
	Point nativePoint2 = p2;

	p2.x = p2.x - p1.x;
	p2.y = p2.y - p1.y;

	//p1 = transformPosition(p1);
	p2 = transformPosition(p2);

	x = 0;
	y = 0;

	// erster Punkt
	setPoint(nativePoint1, c);

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
		setPoint(point, c);
	}

	// letzter Punkt
	setPoint (nativePoint2, c);
}

//
// ÜBUNG 1 AUFGABE 2:
//
// Diese Funktion soll einen Kreis mit Radius r um den Punkt p
// malen. Benutzen Sie die Funktion setPoint um die individuellen
// Punkte zu zeichnen. Vergessen Sie nicht auch den Mittelpunkt zu
// zeichnen!
void bhamCircle (Point p, int r, Color c) {
	int x, y, d, deltaSE, deltaE;
	x = 0;
	y = r;
	d = 5 - 4 * r;

	Point point;
	point.x = x + p.x;
	point.y = y + p.y;
	setPoint(point, c); //point(x,y)

	point.x = -x + p.x;
	setPoint(point, c); //point(-x,y)

	point.y = -y + p.y;
	point.x = x + p.x;
	setPoint(point, c); //point(x,-y)

	point.x = -x + p.x;
	setPoint(point, c); //point(-x,-y)

	point.x = y + p.x;
	point.y = x + p.y;
	setPoint(point, c); //point(y,x)

	point.x = -y + p.x;
	setPoint(point, c); //point(-y, x)

	point.x = y + p.x;
	point.y = -x + p.y;
	setPoint(point, c); //point(y, -x)

	point.y = -x + p.y;
	point.x = -y + p.x;
	setPoint(point, c); //point(-y, -x)

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
		setPoint(point, c); //point(x,y)

		point.x = -x + p.x;
		setPoint(point, c); //point(-x,y)

		point.y = -y + p.y;
		point.x = x + p.x;
		setPoint(point, c); //point(x,-y)

		point.x = -x + p.x;
		setPoint(point, c); //point(-x,-y)

		point.x = y + p.x;
		point.y = x + p.y;
		setPoint(point, c); //point(y,x)

		point.x = -y + p.x;
		setPoint(point, c); //point(-y, x)

		point.x = y + p.x;
		point.y = -x + p.y;
		setPoint(point, c); //point(y, -x)

		point.y = -x + p.y;
		point.x = -y + p.x;
		setPoint(point, c); //point(-y, -x)
	}

	setPoint (p, c);

	// ...
}

// Die Callback Funktion die für das eigentliche Malen
// zuständig ist. Im Wesentlichen brauchen Sie sich nur
// um den Bereich zwischen den Kommentaren zu kümmern,
// alles andere ist wiederum ein notwendiges Übel!
void display (void) {

	//////////////////////////////////////////////////////////////////
	//
	// Hier sollen Ihre Bresenham-Funktionen
	// eine Gerade und einen Kreis zeichnen.
	//
	// Im Prinzip brauchen Sie hier keine
	// großartigen Änderungen vorzunehmen, es
	// sei denn Sie wollen "spielen" :-D
	//

	clearImage ();			// altes Bild löschen

	Point p1(-10, 20);		// ersten Punkt für Gerade definieren
	Point p2(20, -15);		// ebenso den zweiten Punkt
	Point p3(0, 0);
	Point p4(12, 12);
	
	Color cl(1,0,0);		// es soll eine rote Gerade sein ...
	bhamLine(p3, p4, cl);
	bhamLine (p1, p2, cl);	// Gerade zeichnen ...

	Point p(-3, -5);		// Mittelpunkt für Kreis definieren
	int r = 17;				// Radius festlegen
	Color cc(0,0,1);		// es soll ein blauer Kreis sein ...

	bhamCircle (p, r, cc);	// Kreis zeichnen ...

	//
	// Ab hier sollten Sie nichts mehr ändern!
	//
	//////////////////////////////////////////////////////////////////

	manageTexture ();

	glClear (GL_COLOR_BUFFER_BIT);
	glBindTexture (GL_TEXTURE_2D, g_TexID);
	
	glEnable (GL_TEXTURE_2D);
	glBegin (GL_QUADS);
		glColor3f (1, 0, 0);
		glTexCoord2f (0, 0);
		glVertex2f (-g_WinWidth/2, -g_WinHeight/2);
		glTexCoord2f (1, 0);
		glVertex2f (g_WinWidth/2, -g_WinHeight/2);
		glTexCoord2f (1, 1);
		glVertex2f (g_WinWidth/2, g_WinHeight/2);
		glTexCoord2f (0, 1);
		glVertex2f (-g_WinWidth/2, g_WinHeight/2);
	glEnd ();

	glBindTexture (GL_TEXTURE_2D, 0);
	glDisable (GL_TEXTURE_2D);

	glFlush ();
}

// Die Main-Funktion
int main (int argc, char **argv) {

	glutInit (&argc, argv);
	glutInitWindowSize (g_WinWidth, g_WinHeight);
	glutCreateWindow ("Übung 1: Bresenham");

	glutReshapeFunc (reshape);	// zuständig für Größenänderungen des Fensters
	glutDisplayFunc (display);	// zuständig für das wiederholte Neuzeichnen des Bildschirms

	glutMainLoop ();

	glDeleteTextures (1, &g_TexID); // löscht die oben angelegte Textur

	return 0;
}
