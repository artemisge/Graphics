
#include <GL/glut.h>
//#include <stdlib.h>
//#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#define M_PI 3.14159265358979323846
using namespace std;

int points;
float r;
float a, b, c; // color
bool randomClr;

void colorPicker() {
	a = (float)rand() / RAND_MAX;
	b = (float)rand() / RAND_MAX;
	c = (float)rand() / RAND_MAX;
	glColor3f(a, b, c);
}

void myinit()
{

	/* attributes */
	glEnable(GL_BLEND);
	glClearColor(1.0, 1.0, 1.0, 0.0); /* white background */

	/* set up viewing */
	/* 500 x 500 window with origin lower left */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	points = 10000;
	r = 1.0 / 3.0;
	// draw in random color between 0-1
	colorPicker();
	randomClr = 0; // not random
}

void display()
{
	typedef GLfloat point2[2];
	float vrt[5][2]; // tmp to initialize vertices


	// find the polygon vertices
	int n = 5;
	int radius = 250;
	float angleIncrement = 360.0f / n; // n is 5 in this case
	angleIncrement *= M_PI / 180.0f;   // convert degrees to radians
	float angle = 0.0f;
	for (int k = 0; k < n; ++k)
	{
		float a = radius * sin(angle) + 250;
		float b = radius * cos(angle) + 250;
		//p[0] = a;
		//p[1] = b;
		vrt[k][0] = a;
		vrt[k][1] = b;
		angle += angleIncrement;
	}
	point2 vertices[5] = {{vrt[0][0], vrt[0][1]}, {vrt[1][0], vrt[1][1]}, {vrt[2][0], vrt[2][1]}, {vrt[3][0], vrt[3][1]}, {vrt[4][0], vrt[4][1]}}; /* A triangle */
														

	int i, j, k;
	//long rand();       /* standard random number generator */
	float x = (vertices[0][0] + vertices[2][0]) / 2; // x of mid point of point 0 and point 2
	float y = (vertices[0][1] + vertices[2][1]) / 2; // y of mid point of point 0 and point 2

	point2 q = {x, y}; /* An arbitrary initial point inside the polygon*/

	glClear(GL_COLOR_BUFFER_BIT); /*clear the window */

	/* computes and plots x new points */

	for (k = 0; k < points; k++)
	{
		j = rand() % n; /* pick a vertex at random / n vertices*/

		/* Compute point with r distance between vertex and old point */

		q[0] = (q[0] + vertices[j][0]) * r;
		q[1] = (q[1] + vertices[j][1]) * r;

		/* plot new point */
		if (randomClr) {
			colorPicker(); // for every point random color
		}
		glBegin(GL_POINTS);
		glVertex2fv(q);
		glEnd();
	}
	glFlush(); /* clear buffers */
}

void menuF(int value)
{
	switch (value)
	{
	case 1:
		points = 8000;
		colorPicker();
		randomClr = 0;
		break;
	case 2:
		points = 10000;
		randomClr = 1;
		break;
	case 3:
		r = 3.0 / 8.0;
		break;
	case 4:
		r = 1.0 / 3.0;
		break;
	case 5:
		exit(3);
		break;

	default:
		return;
	}

	// Refresh the Window
	display();
}
 
bool leftButtonState = 0; 
int old_x = 0, old_y= 0;
int cx = 0, cy = 500;

void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		leftButtonState = 1; // clicked
		old_x = x; 
  		old_y = y;
		//cout<<"MOUSE CLICK: "<<x<<" "<<y<<endl;
	} else {
		leftButtonState = 0;
	}
}

void mouseDrag(int x, int y) {
	if (leftButtonState) {
		glMatrixMode(GL_PROJECTION); //start editing the projection matrix
		glLoadIdentity(); //remove current projection
		//gluOrtho2D(-x, 500-x, 500-y, -y); //create new one
		int dx = x - old_x;
		int dy = y - old_y;
		// camera's coord
		cx += dx;
		cy += dy;
		//cout<<"MOUSE DRAG: "<<dx<<" "<<dy<<endl;
		//gluOrtho2D(-x, 500-x, y-500.0, y);
		gluOrtho2D(-cx, 500-cx, cy-500, cy);
		old_x = x;
		old_y = y;
		glMatrixMode(GL_MODELVIEW); //back to editing the modelview matrix
	}
	//glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	/* Standard GLUT initialization */
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); /* default, not needed */
	glutInitWindowSize(500, 500);				 /* 500 x 500 pixel window */
	glutInitWindowPosition(0,0);				 /* place window top left on display */
	glutCreateWindow("Fractal Stuff");			 /* window title */
	myinit();									 /* set attributes */
	glutDisplayFunc(display);					 /* display callback invoked when window opened */

	int menu = glutCreateMenu(menuF);
	glutAddMenuEntry("8000 points", 1);
	glutAddMenuEntry("10000 points, random color", 2);
	glutAddMenuEntry("Pentagon r=3/8", 3);
	glutAddMenuEntry("Pentagon r=1/3", 4);
	glutAddMenuEntry("Exit", 5);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag);
	glutMainLoop(); /* enter event loop */
	return 0;
}
