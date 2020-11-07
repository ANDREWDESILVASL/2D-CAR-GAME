void checkCollision(double centerX1, double centerY1, double centerX2, double centerY2) {
	// Boundary values for the first sqaure
	double xMin1 = centerX1 - 100;
	double yMin1 = centerY1 - 100;
	double xMax1 = centerX1 + 100;
	double yMax1 = centerY1 + 100;
	// Boundary value for the second square
	double xMin2 = centerX2 - 100;
	double yMin2 = centerY2 - 100;
	double xMax2 = centerX2 + 100;
	double yMax2 = centerY2 + 100;
  
	// Checking collision
	/*if (xMin1 <= xMax2) {
		bool xminMax = true;
		std::cout << "xminMax" << xminMax << "\n";
	}
	if (xMax1 >= xMin2) {
		bool xmaxMin = true;
		std::cout << "xmaxMin" << xmaxMin << "\n";
	}
	if (yMin1 <= yMax2) {
		bool yminMax = true;
		std::cout << "yminMax" << yminMax << "\n";
	}
	if (yMax1 >= yMin2) {
		bool ymaxMin = true;
		std::cout << "ymaxMin" << ymaxMin << "\n";
	}*/


	bool xminMax = xMin1 <= xMax2;
	bool xmaxMin = xMax1 >= xMin2;
	bool yminMax = yMin1 <= yMax2;
	bool ymaxMin = yMax1 >= yMin2;
	if (((xMin1 <= xMax2)&&(xMax1 >= xMin2)) && ((yMin1<=yMax2)&&(yMax1>=yMin2))) {
		std::cout << "collision detected!\n\n";
	}
	std::cout << "xminMax" << xminMax << "\n";
	std::cout << "xmaxMin" << xmaxMin << "\n";
	std::cout << "yminMax" << yminMax << "\n";
	std::cout << "ymaxMin" << ymaxMin << "\n";

}
