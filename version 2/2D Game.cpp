
#include "include\freeglut.h"	// OpenGL toolkit - in the local shared folder
#include <iostream>
#include <cstdlib> //rand and srand
#include <time.h> //time for srand seeding

//set up some constants
#define X_CENTRE 0.0      /* centre point of square */
#define Y_CENTRE 0.0
#define LENGTH   1.0      /* lengths of sides of square */

GLfloat red = 1.0, green = 1.0, blue = 1.0;
GLsizei wh = 800, ww = 800;
double positionArray[6]; //store the randomly generated positions



void reshape(GLsizei w, GLsizei h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	glMatrixMode(GL_PROJECTION);

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	// Set the aspect ratio of the clipping area to match the viewport
	if (w <= h)
		glOrtho(0.0f, ww, 0.0f, wh * h / w, 1.0, -1.0);
	else
		glOrtho(0.0f, ww * w / h, 0.0f, wh, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Generate random obstacles
void drawSquare(GLfloat length, GLfloat centerX, GLfloat centerY) {
	glBegin(GL_POLYGON);
	glVertex2f(centerX - length / 2, centerY - length / 2);
	glVertex2f(centerX - length / 2, centerY + length / 2);
	glVertex2f(centerX + length / 2, centerY + length / 2);
	glVertex2f(centerX + length / 2, centerY - length / 2);
	glEnd();
}

//Drawing the game border
void drawBorder(GLfloat length, GLfloat centerX, GLfloat centerY) {
	glBegin(GL_LINE_LOOP);
	glVertex2f(centerX - length / 2, centerY - length / 2);
	glVertex2f(centerX - length / 2, centerY + length / 2);
	glVertex2f(centerX + length / 2, centerY + length / 2);
	glVertex2f(centerX + length / 2, centerY - length / 2);
	glEnd();
}


//Drawing the Red and Green gates
void drawRectGate(GLfloat lengthX, GLfloat lengthY, GLfloat x, GLfloat y)
{
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4;

	x1 = x - lengthX / 2;
	y1 = y + lengthY / 2;
	x2 = x + lengthX / 2;
	y2 = y + lengthY / 2;
	x3 = x + lengthX / 2;
	y3 = y - lengthY / 2;
	x4 = x - lengthX / 2;
	y4 = y - lengthY / 2;

	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();

	glFlush();
}

//Generate random positions for the obstacles
void randomPositions() {
	for (int i = 0; i < 5; i=i+2) {
		positionArray[i] = 200 + (std::rand() % (560 - 140 + 1)); //generating random between 40 and 760
		positionArray[i + 1] = 200 + (std::rand() % (560 - 140 + 1));
	}
}


/* display callback function
called whenever contents of window need to be re-displayed */
//this is the all important drawing method - all drawing code goes in here
void display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);   /* clear window */
	glColor3f(red, green, blue);        /* white drawing objects */
	drawBorder(700, 400, 400);	//Creating the game border
	glColor3f(red, 0, 0);		//the red gate
	drawRectGate(110,20,130,60);//Creatig the red gate
	glColor3f(0, green, 0);		//the green gate
	drawRectGate(110, 20, 670, 740);//Creatig the green gate

	glColor3f(red, 0, 0);
	drawSquare(80, positionArray[0], positionArray[1]);
	glColor3f(0, green, 0);
	drawSquare(80, positionArray[2], positionArray[3]);
	glColor3f(0, 0, blue);
	drawSquare(80, positionArray[4], positionArray[5]);
	glColor3f(red, 0, blue);
	drawSquare(80, 200, 200);

	for (int n = 0; n <= 5; n++) {
		std::cout << positionArray[n]<<"\n";
	}
	


	glFlush();     /* execute drawing commands in buffer */
}

/* graphics initialisation */
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);   /* window will be cleared to black */
}

//rename this to main(...) and change example 2 to run this main function
int main(int argc, char** argv)
{
	/* window management code ... */
	/* initialises GLUT and processes any command line arguments */
	glutInit(&argc, argv);
	srand(time(0));
	/* use single-buffered window and RGBA colour model */
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	/* window width = 400 pixels, height = 400 pixels */
	glutInitWindowSize(800,800);
	/* window upper left corner at (100, 100) */
	glutInitWindowPosition(330,0.5);
	/* creates an OpenGL window with command argument in its title bar */
	glutCreateWindow("2D Car Game Version 1.0");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	randomPositions(); //calling the random function to generate the random positions
	glutMainLoop();
	return 0;
}

