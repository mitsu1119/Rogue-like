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
	void setDirection(Direction direction);

	void move(Direction direction);

public:
	Enemy(int panelX, int panelY, int speed, Pic pic):panelX(panelX), panelY(panelY), pic(pic), speed(speed), moveFlag(DirectionNum), movecnt(0), x(0), y(0) {
	}

	Pic pic;
	int speed;
	int panelX, panelY;	// 敵のパネルにおいての座標
	int x, y;	// スクリーン座標
	int movecnt;
	Direction moveFlag;

	bool isMoving();
	void endMoving();
	void autoMove();
	void reflect();
};