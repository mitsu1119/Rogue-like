#include "player.h"

Player::Player(int x, int y, int speed, Pic pic, int panelSize):x(x), y(y), pic(pic), speed(speed), attackFlag(false), panelX(0), panelY(0), panelSize(panelSize), moveFlag(0), animationTime(0) {
	this->front = (Direction)randAtoB(0, DirectionNum - 1);
}

bool Player::isMoving() {
	return this->moveFlag;
}

void Player::attack() {
}

void Player::move(Direction direction) {
	this->moveFlag = true;
	this->front = direction;
	this->panelX += directionDx(direction);
	this->panelY += directionDy(direction);
	this->animationTime = this->panelSize / this->speed;
}

void Player::moveAnimation() {
	this->x += directionDx(this->front) * this->speed;
	this->y += directionDy(this->front) * this->speed;
	this->animationTime--;
	if(animationTime == 0) this->moveFlag = false;
}

// (panelX, panelY) ‚©‚ç (x, y) ‚ð•â³‚·‚éŠÖ”
void Player::reviceCoord(bool harfX, bool harfY) {
	this->x = this->panelX * this->panelSize;
	this->y = this->panelY * this->panelSize;
	if(harfX) this->x -= this->pic.sizeX / 2;
	if(harfY) this->y -= this->pic.sizeY / 2;
}

void Player::shift(int x, int y) {
	this->x += x;
	this->y += y;
}

int Player::getx() {
	return this->x;
}

int Player::gety() {
	return this->y;
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