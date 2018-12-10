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
	this->moveFlag = DirectionNum;
}

void Enemy::setDirection(Direction direction) {
	this->moveFlag = direction;
}