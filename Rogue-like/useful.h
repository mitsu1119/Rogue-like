#pragma once

// [A,B] ‚Ì—”‚ğ¶¬
int randAtoB(int A, int B);

// ‰æ‘œ‘S”ÊƒNƒ‰ƒX
class Pic {
public:
	Pic(int handle, int sizeX, int sizeY):handle(handle), sizeX(sizeX), sizeY(sizeY) {
	}

	int handle;
	int sizeX, sizeY;
};
