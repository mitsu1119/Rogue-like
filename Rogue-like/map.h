#pragma once
#include <vector>
#include "DxLib.h"

// �p�l���̌^���̃��X�g
enum PanelType {
	WALL, ROAD
};

class Panel {
public:
	PanelType type;
};

// ���̃N���X
class Rect {
public:
	int sx, sy, ex, ey;

	Rect(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey) {
	}
};
constexpr int MINIMUM_RECT_SIZE = 8;

class Map {
private:
	int sizeX, sizeY;
	std::vector<Panel> body;

	// �}�b�v��������������
	std::vector<Rect* > rects;
	void mapSplitter(Rect* root);
	void genRndMap();

public:
	Map(int sizeX, int sizeY);
	~Map();

	int calcIndex(int x, int y) {
		return this->sizeX*y + x;
	}

	void reflectRects() {
		int j, k;
		for(auto i : this->rects) {
			for(j = i->sx, k = i->sy; j <= i->ex; j++) this->body[calcIndex(j, k)].type = ROAD;
			for(j = i->sx, k = i->ey; j <= i->ex; j++) this->body[calcIndex(j, k)].type = ROAD;
			for(j = i->sx, k = i->sy; k <= i->ey; k++) this->body[calcIndex(j, k)].type = ROAD;
			for(j = i->ex, k = i->sy; k <= i->ey; k++) this->body[calcIndex(j, k)].type = ROAD;
		}
	}

	void Draw();
}; 