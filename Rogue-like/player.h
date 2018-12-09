#pragma once
#include "useful.h"

class Player {
public:
	Player(int speed, Pic pic):pic(pic), speed(speed) {
	}

	Pic pic;
	int speed;
};