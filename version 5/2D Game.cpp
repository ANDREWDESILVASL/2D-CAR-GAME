
#include "include\freeglut.h"	// OpenGL toolkit - in the local shared folder
#include <iostream>
#include <cstdlib> //rand and srand
#include <time.h> //time for srand seeding


//set up some constants
#define X_CENTRE 0.0      /* centre point of square */
#define Y_CENTRE 0.0
#define LENGTH   1.0      /* lengths of sides of square */
#define DEG2RAD 3.14159/180.0

GLfloat red = 1.0, green = 1.0, blue = 1.0;
GLsizei wh = 800, ww = 800;
double sortedPositionArray[10]; //this array contains the sorted positions

// Constants for rotation and movements
double angle = 0.0;
GLfloat rotation = 15.0;
float posX = 0, posY = 0, posZ = 0;


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


// Drawing ellipse for car tyres
void DrawEllipse(GLfloat x, GLfloat y,float radiusX, float radiusY)
{
	int i;

	glBegin(GL_POLYGON);

	for (i = 0; i < 360; i++)
	{
		float rad = i * DEG2RAD;
		glVertex2f(x +(cos(rad) * radiusX),
			(y+sin(rad) * radiusY));
	}

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

// Drawing the Car
//---------------
void drawCar() {
	drawRectGate(30, 50, 60, 70);
	glColor3f(0, 0, blue);
	drawSquare(10, 60, 60);
	//Drawing tyres
	glColor3f(red, 0, 0);
	DrawEllipse(40,90,5, 10);

	glColor3f(red, 0, 0);
	DrawEllipse(80, 90, 5, 10);

	glColor3f(red, 0, 0);
	DrawEllipse(40, 45, 5, 10);

	glColor3f(red, 0, 0);
	DrawEllipse(80, 45, 5, 10);
	
}

bool checkCollision(double centerX1, double centerY1, double centerX2, double centerY2) {
	// Outputing the collision status
	bool collisionStatus;
	// Boundary values for the first sqaure
	double xMin1 = centerX1 - 80;
	double yMin1 = centerY1 - 80;
	double xMax1 = centerX1 + 80;
	double yMax1 = centerY1 + 80;
	// Boundary value for the second square
	double xMin2 = centerX2 - 80;
	double yMin2 = centerY2 - 80;
	double xMax2 = centerX2 + 80;
	double yMax2 = centerY2 + 80;

	if (((xMin1 <= xMax2) && (xMax1 >= xMin2)) && ((yMin1 <= yMax2) && (yMax1 >= yMin2))) {
		collisionStatus = true;		// collision detected
	}
	else {
		collisionStatus = false;	//no collision
	}
	return collisionStatus;
}



void randomObstacles() {
	//use sortedPositionArray
	for (int nPositions = 0; nPositions < 10; nPositions = nPositions + 2) {
		sortedPositionArray[nPositions] = 160 + (std::rand() % (670 - 140 + 1));		//generating the first pair of x and y
		sortedPositionArray[nPositions + 1] = 160 + (std::rand() % (670 - 140 + 1));
		if ((nPositions == 2) && (checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1)) {
			do
			{
				double r1 = 160 + (std::rand() % (670 - 140 + 1));
				double r2 = 160 + (std::rand() % (670 - 140 + 1));

				sortedPositionArray[nPositions] = r1;
				sortedPositionArray[nPositions + 1] = r2;

			} while (checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1);
		}
		if (nPositions == 4) {
			if ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1)) {
				do
				{
					double r1 = 160 + (std::rand() % (670 - 140 + 1));
					double r2 = 160 + (std::rand() % (670 - 140 + 1));

					sortedPositionArray[nPositions] = r1;
					sortedPositionArray[nPositions + 1] = r2;

				} while ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1));
			}
		}
		if (nPositions == 6) {
			if ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[4], sortedPositionArray[5], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1)) {
				do
				{
					double r1 = 160 + (std::rand() % (670 - 140 + 1));
					double r2 = 160 + (std::rand() % (670 - 140 + 1));

					sortedPositionArray[nPositions] = r1;
					sortedPositionArray[nPositions + 1] = r2;

				} while ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[4], sortedPositionArray[5], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1));
			}
		}
		if (nPositions == 8) {
			if ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[4], sortedPositionArray[5], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[6], sortedPositionArray[7], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1)) {
				do
				{
					double r1 = 160 + (std::rand() % (670 - 140 + 1));
					double r2 = 160 + (std::rand() % (670 - 140 + 1));

					sortedPositionArray[nPositions] = r1;
					sortedPositionArray[nPositions + 1] = r2;

				} while ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[4], sortedPositionArray[5], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[6], sortedPositionArray[7], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1));
			}
		}
		if (nPositions == 10) {
			if ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[4], sortedPositionArray[5], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[6], sortedPositionArray[7], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[8], sortedPositionArray[9], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1)) {
				do
				{
					double r1 = 160 + (std::rand() % (670 - 140 + 1));
					double r2 = 160 + (std::rand() % (670 - 140 + 1));

					sortedPositionArray[nPositions] = r1;
					sortedPositionArray[nPositions + 1] = r2;

				} while ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[4], sortedPositionArray[5], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[6], sortedPositionArray[7], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
					(checkCollision(sortedPositionArray[8], sortedPositionArray[9], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1));
			}
		} std::cout << sortedPositionArray[nPositions] << " and " << sortedPositionArray[nPositions + 1] << "\n";
	}
}

void drawObstacles() {
	// Drawing the obstacles
	glColor3f(red, 0, 0);
	drawSquare(90, sortedPositionArray[0], sortedPositionArray[1]);
	glColor3f(0, green, 0);
	drawSquare(90, sortedPositionArray[2], sortedPositionArray[3]);
	glColor3f(0, 0, blue);
	drawSquare(90, sortedPositionArray[4], sortedPositionArray[5]);
	glColor3f(red, green, 0);
	drawSquare(90, sortedPositionArray[6], sortedPositionArray[7]);
	glColor3f(red, 0, blue);
	drawSquare(90, sortedPositionArray[8], sortedPositionArray[9]);
	glColor3f(red, green, blue);
}



/* display callback function
called whenever contents of window need to be re-displayed */
//this is the all important drawing method - all drawing code goes in here
void display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);   /* clear window */
	// No rotation
	glColor3f(red, green, blue);        /* white drawing objects */
	drawBorder(780, 400, 400);	//Creating the game border
	glColor3f(red, 0, 0);		//the red gate
	drawRectGate(110,20,64,20);//Creatig the red gate
	glColor3f(0, green, 0);		//the green gate
	drawRectGate(110, 20, 735, 780);//Creatig the green gate
	drawObstacles();	// Calling the obstacle drawing method

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glPushMatrix();
	glTranslatef(posX, posY, posZ);  // Using to transle keyboard movements
	glTranslatef(60, 70, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(-60, -70, 0);
	drawCar();
	glPopMatrix();
	glFlush();     /* execute drawing commands in buffer */
}

/* graphics initialisation */
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);   /* window will be cleared to black */
}


// Rotation and movement methods
double move_unit = 10;
void keyboardown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		posX += move_unit;;
		break;

	case GLUT_KEY_LEFT:
		posX -= move_unit;;
		break;

	case GLUT_KEY_UP:
		posY += move_unit;;
		break;

	case GLUT_KEY_DOWN:
		posY -= move_unit;;
		break;

	default:
		break;
	}
	glutPostRedisplay();    //This method used to refresh the realtime movement
}

void keyboard1(unsigned char key, int x, int y) {

	switch (key) {
	case 'r':
		angle = angle + 1.0;
		glutPostRedisplay();
		break;

	case 'e':
		exit(0);
		break;
	default: break;
	}
}

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
	glutCreateWindow("2D Car Game Version 5.0");
	//glutFullScreen();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	randomObstacles();	// calling the random Obstacle generator
	init();
	glutSpecialFunc(keyboardown);
	glutKeyboardFunc(keyboard1);
	glutMainLoop();
	return 0;
}

