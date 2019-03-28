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
	// �X�N���[�����W
	int x, y;

	// true: �����Ă���
	bool moveFlag;
	Direction front;
	int animationTime;

	bool attackFlag;

	// �v���C���[�̂���Œ���̃}�b�v�̏��
	int panelSize;

	// Direction �ɑΉ�
	std::vector<Animation> pics;
	
	Parameter param;

public:
	Player(int speed, std::vector<Animation> pics, int panelSize, Parameter param);
	Player(int panelX, int panelY, int speed, std::vector<Animation> pics, int panelSize, Parameter param);	// enemy�p

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

	void damaged(int damage);
	int calcDamage(int defence);

	void Draw();
};

class Enemy : public Player {
public:
	Enemy(int panelX, int panelY, int speed, std::vector<Animation> pics, int panelSize, Parameter param);

	bool isDead();
};