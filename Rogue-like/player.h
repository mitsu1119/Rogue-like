#pragma once;
#include "useful.h"
#include "DxLib.h"

class Parameter {
protected:
	int hp, at;

public:
	Parameter(int hp, int at);
};

class Player {
protected:
	// �X�N���[�����W
	int x, y;

	// true: �����Ă���
	bool moveFlag;
	Direction front;
	int animationTime;

	bool attackFlag;

	// �v���C���[�̂���Œ���̃}�b�v�̏��
	int panelSize;

	Pic pic;
	
	Parameter param;

public:
	Player(int speed, Pic pic, int panelSize, Parameter param);
	Player(int panelX, int panelY, int speed, Pic pic, int panelSize, Parameter param);	// enemy�p

	// �p�l���̍��W
	int panelX, panelY;
	int speed;

	// true:���Ă��� false:���ĂȂ�
	bool isMoving();
	bool isAttacking();

	void move(Direction direction);
	bool moveAnimation();
	bool moveAnimation(int &dx, int &dy);	// �ǂ̂��炢���������m�肽���Ƃ�
	void reviceCoord(bool harfX, bool harfY);

	Direction attack();
	bool attackAnimation();

	// �v���C���[�̎����W�� (x, y) �������s�ړ�
	void shift(int x, int y);

	void setFront(Direction front);

	// �����W�̏���
	int getx();
	int gety();

	void Draw();
};

class Enemy : public Player {
public:
	Enemy(int panelX, int panelY, int speed, Pic pic, int panelSize, Parameter param);
};