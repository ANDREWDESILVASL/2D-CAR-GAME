void randomPositions() {
	for (int i = 0; i < 5; i = i + 2) {
		positionArray[i] = 200 + (std::rand() % (560 - 140 + 1)); //generating random between 40 and 760
		positionArray[i + 1] = 200 + (std::rand() % (560 - 140 + 1));
	}
	std::cout << "==============\n\n";
	for (int n = 0; n <= 5; n++) {
		std::cout << positionArray[n] << "\n";
	}

	do
	{
		double r1 = 200 + (std::rand() % (560 - 140 + 1)); //generating random between 40 and 760
		double r2 = 200 + (std::rand() % (560 - 140 + 1));
		std::cout << "random changed " << r1 << " + " << r2 << "\n";
		positionArray[2] = r1;
		positionArray[3] = r2;
		if (checkCollision(positionArray[0], positionArray[1], positionArray[2], positionArray[3]) == 0) {
			break;
		}

	} while (checkCollision(positionArray[0], positionArray[1], positionArray[2], positionArray[3]) == 1);
}
