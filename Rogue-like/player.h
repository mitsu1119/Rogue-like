#pragma once;
#include "useful.h"
#include "DxLib.h"

class Player {
private:
	// true: �����Ă���
	bool moveFlag;
	Direction front;
	int animationTime;

	// �X�N���[�����W
	int x, y;

public:
	Player(int x, int y, int speed, Pic pic, int panelSize);

	Pic pic;
	int speed;

	// �p�l���̍��W. �p�l���ꖇ�̃T�C�Y
	int panelX, panelY, panelSize;

	// true:���Ă��� false:���ĂȂ�
	bool attackFlag;
	bool isMoving();

	void attack();
	void move(Direction direction);
	void moveAnimation();
	void moveAnimation(int &dx, int &dy);	// �ǂ̂��炢���������m�肽���Ƃ�
	void reviceCoord(bool harfX, bool harfY);

	// �v���C���[�̎����W�� (x, y) �������s�ړ�
	void shift(int x, int y);

	// �����W�̏���
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
	int panelX, panelY;	// �G�̃p�l���ɂ����Ă̍��W
	int x, y;	// �X�N���[�����W
	int movecnt;
	Direction moveFlag;

	Direction front;

	void move(Direction direction);
	bool isMoving();
	void endMoving();
	void reflect();
};