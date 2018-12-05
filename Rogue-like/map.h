#pragma once
#include <vector>
#include <algorithm>
#include "DxLib.h"
#include "useful.h"

// �p�l���̌^���̃��X�g
enum PanelType {
	WALL, ROAD, TypeNum
};

// �p�l���ꖇ�ꖇ
class Panel {
public:
	PanelType type;
};

// ����
class Room {
public:
	Room(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey) {
	}
	
	Room() {
	}

	int sx, sy, ex, ey;
};

// ���̃N���X
class Rect {
public:
	Rect(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey),splitvflag(true),splithflag(true) {
	}

	int sx, sy, ex, ey;
	bool splitvflag, splithflag;
	
	Room* room;
};

constexpr int MINIMUM_ROOM_SIZE = 4;
constexpr int MINIMUM_RECT_SIZE = MINIMUM_ROOM_SIZE * 2;

// �������m�̃y�A
class RoomPair {
public:
	RoomPair(bool isSide, Rect* rect1, Rect* rect2):isSide(isSide) {
		this->pair = std::make_pair(rect1, rect2);
	}

	std::pair<Rect*, Rect*> pair;
	bool isSide;	// �Ȃ���̕���(�����c��)
};

// �}�b�v�{��
class Map {
private:
//	std::vector<Panel> body;

	std::vector<Rect* > rects;
	std::vector<RoomPair* > roomPairs;

	// �}�b�v��������������
	void mapSplitter(Rect* root);
	void makePairAdd();	// ��{���ɂȂ肪���������̂ł���Ƀy�A�����
	void genRooms();
	void genRndMap();
	void makeLine(int sx, int sy, int ex, int ey);

	// �����╔�����蓖�Ă𔽉f����֐�
	void reflectRects();

	// �}�b�v�`�b�v
	std::vector<Pic> mapchips;

public:
	Map(int sizeX, int sizeY, std::vector<Pic> mapchips);
	~Map();

	int sizeX, sizeY;

	int calcIndex(int x, int y) {
		return this->sizeX*y + x;
	}

	void setMapChips(std::vector<Pic> mapchips) {
		this->mapchips = mapchips;
	}

	void Print();

	// (screenSX, screenSY) ���� ([panelSX,panelEX],[panelSY,panelEY]) ���v�Z���ĕ`��
	void DrawPart(int screenSX, int screenSY, int panelSX, int panelSY, int panelEX, int panelEY, std::vector<Pic>* mapchips);

	// (screenSX, screenSY) ����}�b�v��S���`��(�܂�}�b�v��(0, 0)����`�悵�����̂𕽍s�ړ������悤�Ȋ���)
	void DrawPt(int screenSX, int screenSY);
	void DrawPtMapchips(int screenSX, int screenSY, std::vector<Pic>* mapchips);

	std::vector<Panel> body;
}; 