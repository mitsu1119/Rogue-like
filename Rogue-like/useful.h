#pragma once
#include <vector>

// [A,B] の乱数を生成
int randAtoB(int A, int B);

// 画像全般クラス
class Pic {
public:
	Pic(int handle, int sizeX, int sizeY):handle(handle), sizeX(sizeX), sizeY(sizeY) {
	}

	Pic() {
	}

	int handle;
	int sizeX, sizeY;
};

// アニメーションクラス
class Animation {
private:
	int cycle;
	int cyclePt;
	int buf;
	std::vector<int> time;
public:
	std::vector<Pic> pics;

	Animation(std::vector<Pic> pics, int time):pics(pics),cycle(pics.size()),time(std::vector<int> (cycle, time)) {
		cyclePt = 0;
		buf = 0;
	}

	Animation(std::vector<Pic> pics, std::vector<int> times):pics(pics), cycle(pics.size()), time(times) {
		cyclePt = 0;
		buf = 0;
	}

	Animation() {
		cyclePt = 0;
		cycle = 0;
	}

	int handle() {
		return this->pics[cyclePt].handle;
	}

	void nextImg() {
		buf++;
		if(buf == time[cyclePt]) {
			buf = 0;
			this->cyclePt = (this->cyclePt + 1) % cycle;
		}
	}

	void reset() {
		buf = 0;
		cyclePt = 0;
	}
};

enum Direction {
	UP = 0, RUP, RIGHT, RDOWN, DOWN, LDOWN, LEFT, LUP, DirectionNum
};
int directionDx(Direction direction);
int directionDy(Direction direction);
