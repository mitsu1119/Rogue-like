#pragma once;
#include "useful.h"

class Player {
public:
	Player(int speed, Pic pic):pic(pic), speed(speed) {
	}

	Pic pic;
	int speed;
};

class Enemy {
private:
	Direction moveFlag;

public:
	Enemy(int panelX, int panelY, int speed, Pic pic):panelX(panelX), panelY(panelY), pic(pic), speed(speed), moveFlag(DirectionNum), movecnt(0) {
	}

	Pic pic;
	int speed;
	int panelX, panelY;	// “G‚Ìƒpƒlƒ‹‚É‚¨‚¢‚Ä‚ÌÀ•W
	int movecnt;

	void move(Direction direction);
	bool isMoving();
	void endMoving();
	void setDirection(Direction direction);
};