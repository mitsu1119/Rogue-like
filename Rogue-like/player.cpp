#include "player.h"

Parameter::Parameter(int hp, int at):hp(hp), at(at) {}

int Parameter::getHp() {
	return this->hp;
}

void Parameter::decreaseHp(int point) {
	this->hp -= point;
}

int Parameter::getAt() {
	return this->at;
}

Player::Player(int speed, std::vector<Animation> pics, int panelSize, Parameter param):x(0), y(0), pics(pics), speed(speed), panelX(0), panelY(0), panelSize(panelSize), moveFlag(false), attackFlag(false), animationTime(0), param(param) {
	this->front = (Direction)randAtoB(0, DirectionNum - 1);
}

Player::Player(int panelX, int panelY, int speed, std::vector<Animation> pics, int panelSize, Parameter param): x(0), y(0), pics(pics), speed(speed), panelX(panelX), panelY(panelY), panelSize(panelSize), moveFlag(false), attackFlag(false), animationTime(0), param(param) {}

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
	this->pics[this->front].nextImg();
	this->x += directionDx(this->front) * this->speed;
	this->y += directionDy(this->front) * this->speed;
	this->animationTime--;
	if(animationTime == 0) {
		this->pics[this->front].reset();
		this->moveFlag = false;
		return false;
	}
	return true;
}

bool Player::moveAnimation(int &dx, int &dy) {
	this->pics[this->front].nextImg();
	this->x += directionDx(this->front) * this->speed;
	this->y += directionDy(this->front) * this->speed;
	this->animationTime--;
	dx = directionDx(this->front) * this->speed;
	dy = directionDy(this->front) * this->speed;
	if(animationTime == 0) {
		this->pics[this->front].reset();
		this->moveFlag = false;
		return false;
	}
	return true;
}

// (panelX, panelY) ‚©‚ç (x, y) ‚ð•â³‚·‚éŠÖ”
void Player::reviceCoord(bool harfX, bool harfY) {
	this->x = this->panelX * this->panelSize;
	this->y = this->panelY * this->panelSize;
	if(harfX) this->x -= this->pics[this->front].pics[0].sizeX / 2;
	if(harfY) this->y -= this->pics[this->front].pics[0].sizeY / 2;
}

Direction Player::attack() {
	this->attackFlag = true;
	this->animationTime = 12;
	return this->front;
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

void Player::damaged(int damage) {
	this->param.decreaseHp(damage);
}

int Player::calcDamage(int defence) {
	return this->param.getAt() - defence;
}

void Player::Draw() {
	DrawGraph(this->x - (this->pics[this->front].pics[0].sizeX - 100), this->y - (this->pics[this->front].pics[0].sizeY - 100), this->pics[this->front].handle(), true);
}

Enemy::Enemy(int panelX, int panelY, int speed, std::vector<Animation> pics, int panelSize, Parameter param):Player(panelX, panelY, speed, pics, panelSize, param){
	this->front = (Direction)randAtoB(0, DirectionNum - 1);
}

bool Enemy::isDead() {
	if(this->param.getHp() <= 0) return true;
	return false;
}