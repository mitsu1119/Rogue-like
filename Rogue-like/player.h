#pragma once;
#include "useful.h"
#include "DxLib.h"
#include <vector>

class Parameter {
protected:
	int hp, at;

public:
	Parameter(int hp, int at);

	int getHp();
	void decreaseHp(int point);

	int getAt();
};

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

	// Direction に対応
	std::vector<Animation> pics;
	
	Parameter param;

public:
	Player(int speed, std::vector<Animation> pics, int panelSize, Parameter param);
	Player(int panelX, int panelY, int speed, std::vector<Animation> pics, int panelSize, Parameter param);	// enemy用

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

	Direction attack();
	bool attackAnimation();

	// プレイヤーの実座標を (x, y) だけ平行移動
	void shift(int x, int y);

	void setFront(Direction front);

	// 実座標の所得
	int getx();
	int gety();

	void damaged(int damage);
	int calcDamage(int defence);

	void Draw();
};

class Enemy : public Player {
public:
	Enemy(int panelX, int panelY, int speed, std::vector<Animation> pics, int panelSize, Parameter param);

	bool isDead();
};