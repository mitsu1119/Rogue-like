#pragma once;
#include "useful.h"
#include "DxLib.h"

class Player {
private:
	// true: 動いている
	bool moveFlag;
	Direction front;
	int animationTime;

	// スクリーン座標
	int x, y;

public:
	Player(int x, int y, int speed, Pic pic, int panelSize);

	Pic pic;
	int speed;

	// パネルの座標. パネル一枚のサイズ
	int panelX, panelY, panelSize;

	// true:している false:してない
	bool attackFlag;
	bool isMoving();

	void attack();
	void move(Direction direction);
	void moveAnimation();
	void reviceCoord(bool harfX, bool harfY);

	// プレイヤーの実座標を (x, y) だけ平行移動
	void shift(int x, int y);

	// 実座標の所得
	int getx();
	int gety();

	void Draw();
};

class Enemy {
private:
	void setDirection(Direction direction);

public:
	Enemy(int panelX, int panelY, int speed, Pic pic):panelX(panelX), panelY(panelY), pic(pic), speed(speed), moveFlag(DirectionNum), movecnt(0), x(0), y(0) {
		this->front = (Direction)randAtoB(0, DirectionNum - 1);
	}

	Pic pic;
	int speed;
	int panelX, panelY;	// 敵のパネルにおいての座標
	int x, y;	// スクリーン座標
	int movecnt;
	Direction moveFlag;

	Direction front;

	void move(Direction direction);
	bool isMoving();
	void endMoving();
	void reflect();
};