#include "useful.h"
#include "DxLib.h"

int randAtoB(int A, int B) {
	return GetRand(B - A) + A;
}