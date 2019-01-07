#include "player.h"

Player::Player(int speed, Pic pic, int panelSize):x(0), y(0), pic(pic), speed(speed), panelX(0), panelY(0), panelSize(panelSize), moveFlag(false), attackFlag(false), animationTime(0) {
	this->front = (Direction)randAtoB(0, DirectionNum - 1);
}

Player::Player(int panelX, int panelY, int speed, Pic pic, int panelSize): x(0), y(0), pic(pic), speed(speed), panelX(panelX), panelY(panelY), panelSize(panelSize), moveFlag(false), attackFlag(false), animationTime(0) {}

bool Player::isMoving() {
	return this->moveFlag;
}

bool Player::isAttacking() {
	return this->attackFlag;
}

void Player::move(Direction direction) {
	this->moveFlag = true;
	this->front = direction;
	this->panelX += directionDx(direction);
	this->panelY += directionDy(direction);
	this->animationTime = this->panelSize / this->speed;
}

bool Player::moveAnimation() {
	this->x += directionDx(this->front) * this->speed;
	this->y += directionDy(this->front) * this->speed;
	this->animationTime--;
	if(animationTime == 0) {
		this->moveFlag = false;
		return false;
	}
	return true;
}

bool Player::moveAnimation(int &dx, int &dy) {
	this->x += directionDx(this->front) * this->speed;
	this->y += directionDy(this->front) * this->speed;
	this->animationTime--;
	dx = directionDx(this->front) * this->speed;
	dy = directionDy(this->front) * this->speed;
	if(animationTime == 0) {
		this->moveFlag = false;
		return false;
	}
	return true;
}

// (panelX, panelY) ‚©‚ç (x, y) ‚ð•â³‚·‚éŠÖ”
void Player::reviceCoord(bool harfX, bool harfY) {
	this->x = this->panelX * this->panelSize;
	this->y = this->panelY * this->panelSize;
	if(harfX) this->x -= this->pic.sizeX / 2;
	if(harfY) this->y -= this->pic.sizeY / 2;
}

void Player::attack() {
	this->attackFlag = true;
	this->animationTime = 12;
}

bool Player::attackAnimation() {
	this->animationTime--;
	if(animationTime>=6) {
		this->x += directionDx(this->front)*13;
		this->y += directionDy(this->front)*13;
	} else {
		this->x -= directionDx(this->front)*13;
		this->y -= directionDy(this->front)*13;
	}
	if(animationTime==0) {
		this->attackFlag = false;
		return false;
	}
	return true;
}

void Player::shift(int x, int y) {
	this->x += x;
	this->y += y;
}

void Player::setFront(Direction front) {
	this->front = front;
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
