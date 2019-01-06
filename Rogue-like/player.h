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
	Player(int speed, Pic pic, int panelSize);

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
	void moveAnimation(int &dx, int &dy);	// どのくらい動いたか知りたいとき
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
	Enemy(int panelX, int panelY, int speed, Pic pic, int panelSize);

	Pic pic;
	int speed;
	int panelX, panelY, panelSize;	// 敵のパネルにおいての座標
	int x, y;	// スクリーン座標
	int movecnt;
	bool moveFlag;
	int animationTime;

	Direction front;

	void move(Direction direction);
	void moveAnimation();
	bool isMoving();
	void endMoving();
	void shift(int x, int y);
	void reviceCoord(bool harfX, bool harfY);
	void reflect();
	void Draw();
};