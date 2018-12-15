#include "player.h"

Player::Player(int x, int y, int speed, Pic pic):x(x), y(y), pic(pic), speed(speed), attackFlag(false), panelX(0), panelY(0), moveFlag(0), animationTime(0) {
	this->front = (Direction)randAtoB(0, DirectionNum - 1);
}

bool Player::isMoving() {
	return this->moveFlag;
}

void Player::attack() {
}

void Player::move(Direction direction, int panelSize) {
	this->moveFlag = true;
	this->front = direction;
	this->panelX += directionDx(direction);
	this->panelY += directionDy(direction);
	this->animationTime = panelSize / this->speed;
}

void Player::moveAnimation() {
	this->x += directionDx(this->front) * this->speed;
	this->y += directionDy(this->front) * this->speed;
	this->animationTime--;
	if(animationTime == 0) this->moveFlag = false;
}

void Player::Draw() {
	DrawGraph(this->x, this->y, this->pic.handle, true);
}

void Enemy::move(Direction direction) {
	this->moveFlag = direction;
	this->front = direction;
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

void Enemy::reflect() {
	this->movecnt++;
}