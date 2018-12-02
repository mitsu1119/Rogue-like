#pragma once
#include <vector>
#include "DxLib.h"
#include "useful.h"

// �p�l���̌^���̃��X�g
enum PanelType {
	WALL, ROAD
};

// �p�l���ꖇ�ꖇ
class Panel {
public:
	PanelType type;
};

// ���̃N���X
class Rect {
public:
	Rect(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey) {
	}

	Rect() {
	}

	int sx, sy, ex, ey;
};

constexpr int MINIMUM_ROOM_SIZE = 4;
constexpr int MINIMUM_RECT_SIZE = MINIMUM_ROOM_SIZE * 2;

// ����
class Room: public Rect {
public:
	Room(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey) {
	}

	int sx, sy, ex, ey;
};

// �}�b�v�{��
class Map {
private:
	int sizeX, sizeY;
	std::vector<Panel> body;

	std::vector<Rect* > rects;
	std::vector<Room* > rooms;

	// �}�b�v��������������
	void mapSplitter(Rect* root);
	void genRooms();
	void genRndMap();

	// �����╔�����蓖�Ă𔽉f����֐�
	void reflectRects();


public:
	Map(int sizeX, int sizeY);
	~Map();

	int calcIndex(int x, int y) {
		return this->sizeX*y + x;
	}

	void Draw();
}; 