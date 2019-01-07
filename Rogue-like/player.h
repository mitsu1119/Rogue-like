#pragma once;
#include "useful.h"
#include "DxLib.h"

class Player {
protected:
	// スクリーン座標
	int x, y;

	// true: 動いている
	bool moveFlag;
	Direction front;
	int animationTime;

	bool attackFlag;

	// プレイヤーのいる最低限のマップの情報
	int panelSize;

	Pic pic;

public:
	Player(int speed, Pic pic, int panelSize);
	Player(int panelX, int panelY, int speed, Pic pic, int panelSize);	// enemy用

	// パネルの座標
	int panelX, panelY;
	int speed;

	// true:している false:してない
	bool isMoving();
	bool isAttacking();

	void move(Direction direction);
	bool moveAnimation();
	bool moveAnimation(int &dx, int &dy);	// どのくらい動いたか知りたいとき
	void reviceCoord(bool harfX, bool harfY);

	void attack();
	bool attackAnimation();

	// プレイヤーの実座標を (x, y) だけ平行移動
	void shift(int x, int y);

	void setFront(Direction front);

	// 実座標の所得
	int getx();
	int gety();

	void Draw();
};

class Enemy : public Player {
public:
	Enemy(int panelX, int panelY, int speed, Pic pic, int panelSize);
};