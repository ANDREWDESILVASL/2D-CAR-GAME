// Pass the car X and Y posititons
// Car is considered as square with size of 60
// Block x=400 y=400
// Block is considered as square with size of 80

void checkCollisionCar(double carX, double carY) {
	double carLeft = carX - 30;
	double carYtop = carY + 30;
	double carRight = carX + 30;
	double carYbottom = carY - 30;

	bool car_is_right_to_the_block = false;
	bool car_is_left_to_the_block = false;
	bool car_is_above_block = false;
	bool car_is_below_block = false;
	
	if (carLeft > 440) {
		car_is_right_to_the_block = true;
	}
	else if (carRight < 360) {
		car_is_left_to_the_block = true;
	}
	else if (carYbottom > 440) {
		car_is_above_block = true;
	}
	else if (carYtop < 360) {
		car_is_below_block = true;
	}

	if (car_is_right_to_the_block == 0 && car_is_left_to_the_block == 0 && car_is_above_block == 0 && car_is_below_block == 0) {
		std::cout << "collision detected\n\n";
	}

}
