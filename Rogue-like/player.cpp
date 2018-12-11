#include "player.h"

void Enemy::move(Direction direction) {
	this->moveFlag = direction;
	this->panelX += directionDx(direction);
	this->panelY += directionDy(direction);
}

bool Enemy::isMoving() {
	return (this->moveFlag == DirectionNum) ? false : true;
}

void Enemy::endMoving() {
	this->movecnt = 0;
	this->moveFlag = DirectionNum;
}

void Enemy::setDirection(Direction direction) {
	this->moveFlag = direction;
}

void Enemy::autoMove() {
	Direction dir = (Direction)randAtoB(0, DirectionNum - 1);
	move(dir);
}

void Enemy::reflect() {
	this->movecnt++;
}