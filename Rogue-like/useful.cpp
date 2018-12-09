#include "useful.h"
#include "DxLib.h"

int randAtoB(int A, int B) {
	return GetRand(B - A) + A;
}

int directionDx(Direction direction) {
	switch(direction) {
	case UP:
	case DOWN:
		return 0;
	case RIGHT:
	case RUP:
	case RDOWN:
		return 1;
	case LEFT:
	case LUP:
	case LDOWN:
		return -1;
	default:
		return 0;
	}
}

int directionDy(Direction direction) {
	switch(direction) {
	case RIGHT:
	case LEFT:
		return 0;
	case UP:
	case RUP:
	case LUP:
		return -1;
	case DOWN:
	case RDOWN:
	case LDOWN:
		return 1;
	default:
		return 0;
	}
}