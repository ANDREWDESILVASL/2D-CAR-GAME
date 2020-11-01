
#include "include\freeglut.h"	// OpenGL toolkit - in the local shared folder
#include <iostream>
#include <cstdlib> //rand and srand
#include <time.h> //time for srand seeding

//set up some constants
#define X_CENTRE 0.0      /* centre point of square */
#define Y_CENTRE 0.0
#define LENGTH   1.0      /* lengths of sides of square */

GLfloat red = 1.0, green = 1.0, blue = 1.0;
GLsizei wh = 500, ww = 500;


/*void reshape(GLsizei w, GLsizei h)
{
	//Using gluOrtho2D
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w, h);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)w, 0, (GLdouble)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}*/

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
/*void drawSquareRandom() {
	/*glColor3f(red, 0, 0);
	drawSquare(80, 100, 200);
	glColor3f(0, green, 0);
	drawSquare(80, 300, 400);
	glColor3f(0, 0, blue);
	drawSquare(80, 350, 250);
	for (int i = 0; i < 5; i++) {
		//int randomNumber = rand();//generating a random number
		double randomCenterX = (rand() % 300) + 1; //Generating a random number between 300 and 1
		double randomCenterY = (rand() % 400) + 300; //Generating a random number between 400 and 300
		drawSquare(80,randomCenterX,randomCenterY);
	}
}*/

double positionArray[6];

void randomPositions() {
	for (int i = 0; i < 5; i=i+2) {
		positionArray[i] = rand() % 300;
		positionArray[i+1] = rand() % 500;
	}
}


/* display callback function
called whenever contents of window need to be re-displayed */
//this is the all important drawing method - all drawing code goes in here
void display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);   /* clear window */
	glColor3f(red, green, blue);        /* white drawing objects */
	//drawSquareRandom();
	glColor3f(red, 0, 0);
	drawSquare(80, positionArray[0], positionArray[1]);
	glColor3f(0, green, 0);
	drawSquare(80, positionArray[2], positionArray[3]);
	glColor3f(0, 0, blue);
	drawSquare(80, positionArray[4], positionArray[5]);

	for (int n = 0; n < 5; n++) {
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
	glutInitWindowSize(500, 500);
	/* window upper left corner at (100, 100) */
	glutInitWindowPosition(568,232);
	/* creates an OpenGL window with command argument in its title bar */
	glutCreateWindow("Example 1");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	randomPositions();

	glutMainLoop();
	return 0;
}

