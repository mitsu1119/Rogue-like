#pragma once

// [A,B] の乱数を生成
int randAtoB(int A, int B);

// 画像全般クラス
class Pic {
public:
	Pic(int handle, int sizeX, int sizeY):handle(handle), sizeX(sizeX), sizeY(sizeY) {
	}

	int handle;
	int sizeX, sizeY;
};

enum Direction {
	UP, RUP, RIGHT, RDOWN, DOWN, LDOWN, LEFT, LUP, DirectionNum
};
int directionDx(Direction direction);
int directionDy(Direction direction);
