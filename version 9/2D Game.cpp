
#include "include\freeglut.h"	// OpenGL toolkit - in the local shared folder
#include <iostream>
#include <cstdlib> //rand and srand
#include <time.h> //time for srand seeding
#include <stdbool.h>
#include <sstream>
#include <string>

//set up some constants
#define X_CENTRE 0.0      /* centre point of square */
#define Y_CENTRE 0.0
#define LENGTH   1.0      /* lengths of sides of square */
#define DEG2RAD 3.14159/180.0	
GLsizei wh = 800, ww = 800;	// Window width and height
GLint gameScoreMax = 50; // Maximum score value
GLint gameScoreMinus = -10; // Score reduce for a collision
GLint gameScoreMinimum = 0; // Minimum score can achived
GLfloat yStep = 0.1f; // Changing y coordinate for exhaust effect
GLfloat red = 1.0, green = 1.0, blue = 1.0;
double sortedPositionArray[10]; // Save the randomly generated positions for blocks
double move_unit = 0.3; // Move the car
double back_unit = 20.0; // Reset the car x or y coordinates back to a non-collision area
double angle = 0.0; // Rotate the car
double posX = 0, posY = 0, posZ = 0; // Car coordinates for transformation
bool keystats[256]; // Store the curunt keyboard status
bool gameComplete = false; // Check whether the car has enter the wining area
GLint grid = 0.0; // value to check whether the grid should draw or not


/* reshape callback function
executed when window is moved or resized */
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
//----------------------This is game design section------------------------------

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
void DrawEllipse(GLfloat x, GLfloat y, float radiusX, float radiusY)
{
	int i;

	glBegin(GL_POLYGON);

	for (i = 0; i < 360; i++)
	{
		float rad = i * DEG2RAD;
		glVertex2f(x + (cos(rad) * radiusX),
			(y + sin(rad) * radiusY));
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

// Grid menu for glutMenu option
void drawGrid() {
	GLint verticleLeftX = 40;
	GLint horizontaldownY = 40;
	GLint verticleRightX = 760;
	GLint horizontalupY = 760;
	for (int i = 160; i < 720; i+=80) {
		glBegin(GL_LINE_LOOP);
		glColor3f(0.4, 0.4, 0.4);
		glVertex2f(i, horizontaldownY);
		glVertex2f(i, horizontalupY);
		glEnd();
	}
	for (int i = 160; i < 720; i += 80) {
		glBegin(GL_LINE_LOOP);
		glColor3f(0.4, 0.4, 0.4);
		glVertex2f(verticleLeftX,i);
		glVertex2f(verticleRightX,i);
		glEnd();
	}
	

}

// Score Text convert
void writeBitmapString(void* font, char const* string)
{
	char const* c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Score display text
void scoreText(GLint scoreX, GLint scoreY, GLint score) {
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(scoreX, scoreY);
	std::string s = std::to_string(score);
	char const* scoreChar = s.c_str();
	writeBitmapString(GLUT_BITMAP_HELVETICA_18, "Score ");
	writeBitmapString(GLUT_BITMAP_HELVETICA_18, scoreChar);
}

// Draw red and green gates
void drawGates(GLint redLX, GLint redLY, GLint redX, GLint redY,GLint greenLX, GLint greenLY, GLint greenX, GLint greenY) {
	glColor3f(red, 0, 0);		//the red gate
	drawRectGate(redLX, redLY, redX, redY);//Creatig the red gate
	glColor3f(0, green, 0);		//the green gate
	drawRectGate(greenLX, greenLY, greenX, greenY);//Creatig the green gate
}

// Drawing the car
void drawCar() {
	// Car body
	glColor3f(0, green, 0);
	drawRectGate(35, 45, 95, 100);
	drawRectGate(5, 10, 95, 78);
	// Car exhaust square
	glColor3f(red, green, blue);
	drawSquare(3, 95, 65+yStep);
	glColor3f(0, 0, 0);
	drawSquare(30, 95, 105);
	//Drawing tyres
	glColor3f(red, 0, 0);
	DrawEllipse(72, 110, 5, 10);

	glColor3f(red, 0, 0);
	DrawEllipse(118, 110, 5, 10);

	glColor3f(red, 0, 0);
	DrawEllipse(72, 80, 5, 10);

	glColor3f(red, 0, 0);
	DrawEllipse(118, 80, 5, 10);

	//Border for the car - del
	glColor3f(red, green, blue);
	//drawCarBorder(60, 60, 95, 96);
}

//------------------------------This is Game Logic Section------------------------------

// Check Collision betweem two sqaures
bool checkCollision(double centerX1, double centerY1, double centerX2, double centerY2) {
	// Outputing the collision status
	bool collisionStatus;
	// Boundary values for the first sqaure
	double xMin1 = centerX1 - 60;
	double yMin1 = centerY1 - 60;
	double xMax1 = centerX1 + 60;
	double yMax1 = centerY1 + 60;
	// Boundary value for the second square
	double xMin2 = centerX2 - 60;
	double yMin2 = centerY2 - 60;
	double xMax2 = centerX2 + 60;
	double yMax2 = centerY2 + 60;

	if (((xMin1 <= xMax2) && (xMax1 >= xMin2)) && ((yMin1 <= yMax2) && (yMax1 >= yMin2))) {
		collisionStatus = true;		// collision detected
	}
	else {
		collisionStatus = false;	//no collision
	}
	return collisionStatus;
}

// Generate random obstacles with collision detection and saved to array
void randomObstacles() {
	//use sortedPositionArray
	for (int nPositions = 0; nPositions < 10; nPositions = nPositions + 2) {
		sortedPositionArray[nPositions] = 160 + (std::rand() % (600 - 140 + 1));		//generating the first pair of x and y
		sortedPositionArray[nPositions + 1] = 160 + (std::rand() % (600 - 140 + 1));
		if ((nPositions == 2) && (checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1)) {
			do
			{
				double r1 = 160 + (std::rand() % (600 - 140 + 1));
				double r2 = 160 + (std::rand() % (600 - 140 + 1));

				sortedPositionArray[nPositions] = r1;
				sortedPositionArray[nPositions + 1] = r2;

			} while (checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1);
		}
		if (nPositions == 4) {
			if ((checkCollision(sortedPositionArray[0], sortedPositionArray[1], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1) ||
				(checkCollision(sortedPositionArray[2], sortedPositionArray[3], sortedPositionArray[nPositions], sortedPositionArray[nPositions + 1]) == 1)) {
				do
				{
					double r1 = 160 + (std::rand() % (600 - 140 + 1));
					double r2 = 160 + (std::rand() % (600 - 140 + 1));

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
					double r1 = 160 + (std::rand() % (600 - 140 + 1));
					double r2 = 160 + (std::rand() % (600 - 140 + 1));

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
					double r1 = 160 + (std::rand() % (600 - 140 + 1));
					double r2 = 160 + (std::rand() % (600 - 140 + 1));

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
					double r1 = 160 + (std::rand() % (600 - 140 + 1));
					double r2 = 160 + (std::rand() % (600 - 140 + 1));

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

// Draw generated random obstacles
void drawObstacles() {
	// Drawing the obstacles
	glColor3f(red, 0, 0);
	drawSquare(80, sortedPositionArray[0], sortedPositionArray[1]);
	glColor3f(0, green, 0);
	drawSquare(80, sortedPositionArray[2], sortedPositionArray[3]);
	glColor3f(0, 0, blue);
	drawSquare(80, sortedPositionArray[4], sortedPositionArray[5]);
	glColor3f(red, green, 0);
	drawSquare(80, sortedPositionArray[6], sortedPositionArray[7]);
	glColor3f(red, 0, blue);
	drawSquare(80, sortedPositionArray[8], sortedPositionArray[9]);
	glColor3f(red, green, blue);
}

// Check car and block collision
bool collisionAABB(double carX, double carY, GLint index) {
	double carLeft = carX - 28;
	double carYtop = carY + 28;
	double carRight = carX + 28;
	double carYbottom = carY - 28;

	bool car_is_right_to_the_block = false;
	bool car_is_left_to_the_block = false;
	bool car_is_above_block = false;
	bool car_is_below_block = false;

	bool collisionStatus = false;

	if (carLeft > sortedPositionArray[index] + 35) {
		car_is_right_to_the_block = true;
	}
	else if (carRight < sortedPositionArray[index] - 35) {
		car_is_left_to_the_block = true;
	}
	else if (carYbottom > sortedPositionArray[index + 1] + 35) {
		car_is_above_block = true;
	}
	else if (carYtop < sortedPositionArray[index + 1] - 35) {
		car_is_below_block = true;
	}
	if (car_is_right_to_the_block == 0 && car_is_left_to_the_block == 0 && car_is_above_block == 0 && car_is_below_block == 0) {
		//std::cout << "collision detected\n" << index << " " << index + 1 << "\n";
		collisionStatus = true;
	}
	return collisionStatus;
}

// Check the game win
bool gameCheckWin(double carX, double carY) {
	if (((carX + posX) > 630) && (carY + posY) > 700) {
		gameComplete = true;
	}
	return gameComplete;
}

// Timer function for animation
void timerFunction(int value) {
	if(yStep > 13.0f) {
		yStep = 0.1f;
	}
	yStep += 1;
	glutPostRedisplay();
	glutTimerFunc(40, timerFunction, 0);
}

// GlutMenu function to turn on the grid
void glutMenu(GLint id) {
	if (id == 1)
	{
		grid = 1.0;
	}
	else {
		grid = 0.0;
	}
	glutPostRedisplay();
}



/* display callback function
called whenever contents of window need to be re-displayed */
//this is the all important drawing method - all drawing code goes in here
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glColor3f(red, green, blue);
	//-------------------Game UI-----------------------------------
	// Glut menu option for Grid
	if (grid)
	{
		drawGrid();
	}
	drawBorder(720, 400, 400);	//Creating the game border
	scoreText(60, 770, gameScoreMax); // Score text
	drawGates(110, 20, 95, 50, 110, 20, 705, 750); // Drawingt the red and green gates
	drawObstacles();	// Calling the obstacle drawing method

	//-------------------Car----------------------------------------
	glPushMatrix();
	glTranslatef(posX, posY, posZ);  // Using to transle keyboard movements
	glTranslatef(95, 100, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(-95, -100, 0);
	drawCar();						// Drawing the car

	// Check the game completed
	gameCheckWin(95, 96);
	if ((gameComplete == true) || (gameScoreMax == gameScoreMinimum)) {
		posX = 0, posY = 0, posZ = 0; //setting the coordinates to origin
		gameComplete = false;		// setting the boolean value as game not compelted
		gameScoreMax = 50;			// reseting the score
		randomObstacles();			// Generating the random obstacles for new game
	}
	glPopMatrix();
	glutSwapBuffers();

}


// Keyboard functions
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);   /* window will be cleared to black */
	//randomObstacles();	// calling the random Obstacle generator
	glClear(GL_COLOR_BUFFER_BIT);
}

void idle()
{
	// If r and a pressed rotation
	if (keystats['r'] && keystats['a'])
	{
		angle = angle + 0.1;
		glutPostRedisplay();
	}
	// if d and r pressed rotation
	else if (keystats['d'] && keystats['r']) {
		angle = angle - 0.1;
		glutPostRedisplay();
	}
	else {
		if (keystats['a']) { // car move left
			angle = 0.0;
			angle = angle + (90.0);
			glutPostRedisplay();
			posX -= move_unit;
			for (int i = 0; i < 10; i += 2) { // Checking the car collision with a block
				if (collisionAABB(95 + posX, 96 + posY, i) == true) { // checking the collision with car coordinates and square coordinates
					posX += back_unit; // if a collision happens setting the car x coordinate to a non-collision area
					gameScoreMax += gameScoreMinus; // reducing a score value
				}
			}
		}
		else if (keystats['d']) { // car move right
			angle = 0.0;
			angle = angle + (-90.0);
			glutPostRedisplay();
			posX += move_unit;
			for (int i = 0; i < 10; i += 2) {		// Checking the car collision with a block
				if (collisionAABB(95 + posX, 96 + posY, i) == true) { // checking the collision with car coordinates and square coordinates
					posX -= back_unit;  // if a collision happens setting the car x coordinate to a non-collision area
					gameScoreMax += gameScoreMinus; // reducing a score value
				}
			}
		}
		else if (keystats['w']) { // car move forward
			angle = 0.0;
			glutPostRedisplay();
			posY += move_unit;
			for (int i = 0; i < 10; i += 2) {		// Checking the car collision with a block
				if (collisionAABB(95 + posX, 96 + posY, i) == true) { // checking the collision with car coordinates and square coordinates
					posY -= back_unit;  // if a collision happens setting the car y coordinate to a non-collision area
					gameScoreMax += gameScoreMinus; // reducing a score value
				}
			}
		}
		else if (keystats['s']) { // car move down
			angle = -180.0;
			glutPostRedisplay();
			posY -= move_unit;;
			for (int i = 0; i < 10; i += 2) {			// Checking the car collision with a block
				if (collisionAABB(95 + posX, 96 + posY, i) == true) { // checking the collision with car coordinates and square coordinates
					posY += back_unit;  // if a collision happens setting the car y coordinate to a non-collision area
					gameScoreMax += gameScoreMinus; // reducing a score value
				}
			}
		}
	}
}

void keyPressed(unsigned char key, int x, int y)
{
	keystats[key] = true;         // Set the state of the current key to pressed

}

void keyUp(unsigned char key, int x, int y)
{
	keystats[key] = false;     // Set the state of the current key to not pressed  

}




int main(int argc, char** argv)
{
	srand(time(0));
	/* window management code ... */
	/* initialises GLUT and processes any command line arguments */
	glutInit(&argc, argv);
	/* use single-buffered window and RGBA colour model */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	/* window width = 700 pixels, height = 700 pixels */
	glutInitWindowSize(700, 700);
	/* window upper left corner at (100, 100) */
	glutInitWindowPosition(380, 60);
	/* creates an OpenGL window with command argument in its title bar */
	glutCreateWindow("2D Car Game Version 9.0");
	//init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	randomObstacles();	// calling the random Obstacle generator
	//glutKeyboardFunc(keyboardCar);
	glutIdleFunc(idle);
	glutKeyboardUpFunc(keyUp);
	glutKeyboardFunc(keyPressed);
	glutCreateMenu(glutMenu);
	glutAddMenuEntry("Grid on", 1);
	glutAddMenuEntry("Grid off", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutTimerFunc(40, timerFunction, 0);
	glutMainLoop();

	return 0;
}


