#include "player.h"

Player::Player(int speed, Pic pic, int panelSize):x(0), y(0), pic(pic), speed(speed), panelX(0), panelY(0), panelSize(panelSize), moveFlag(false), animationTime(0) {
	this->front = (Direction)randAtoB(0, DirectionNum - 1);
}

Player::Player(int panelX, int panelY, int speed, Pic pic, int panelSize): x(0), y(0), pic(pic), speed(speed), panelX(panelX), panelY(panelY), panelSize(panelSize), moveFlag(false), animationTime(0) {}

bool Player::isMoving() {
	return this->moveFlag;
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

void Player::moveAnimation(int &dx, int &dy) {
	this->x += directionDx(this->front) * this->speed;
	this->y += directionDy(this->front) * this->speed;
	this->animationTime--;
	dx = directionDx(this->front) * this->speed;
	dy = directionDy(this->front) * this->speed;
	if(animationTime == 0) this->moveFlag = false;
}

// (panelX, panelY) から (x, y) を補正する関数
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

Enemy::Enemy(int panelX, int panelY, int speed, Pic pic, int panelSize):Player(panelX, panelY, speed, pic, panelSize){
	this->front = (Direction)randAtoB(0, DirectionNum - 1);
}

void Enemy::setDirection(Direction direction) {
	this->moveFlag = direction;
}
