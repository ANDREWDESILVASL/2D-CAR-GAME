bool keystats[256];
void idle()
{
	if (keystats['r'] && keystats['a'])
	{
		angle = angle + 0.2;
		glutPostRedisplay();
	}
	else if (keystats['d'] && keystats['r']) {
		angle = angle - 0.2;
		glutPostRedisplay();
	}
	else {
		if (keystats['a']) {
			angle = 0.0;
			angle = angle + (90.0);
			glutPostRedisplay();
			posX -= move_unit;
		}
		else if (keystats['d']) {
			angle = 0.0;
			angle = angle + (-90.0);
			glutPostRedisplay();
			posX += move_unit;
		}
		else if (keystats['w']) {
			angle = 0.0;
			glutPostRedisplay();
			posY += move_unit;
		}
		else if (keystats['s']) {
			angle = -180.0;
			glutPostRedisplay();
			posY -= move_unit;;
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
