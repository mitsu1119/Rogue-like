#pragma once
#include "useful.h"

class Player {
public:
	Player(int speed, Pic pic):pic(pic), speed(speed) {
	}

	Pic pic;
	int speed;
};

class Enemy {
public:
	Enemy(int panelX, int panelY, int speed, Pic pic):panelX(panelX), panelY(panelY), pic(pic), speed(speed) {
	}

	Pic pic;
	int speed;
	int panelX, panelY;	// 敵のパネルにおいての座標
};