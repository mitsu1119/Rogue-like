#pragma once

// [A,B] �̗����𐶐�
int randAtoB(int A, int B);

// �摜�S�ʃN���X
class Pic {
public:
	Pic(int handle, int sizeX, int sizeY):handle(handle), sizeX(sizeX), sizeY(sizeY) {
	}

	int handle;
	int sizeX, sizeY;
};
