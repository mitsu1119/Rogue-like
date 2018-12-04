#pragma once
#include <vector>
#include "DxLib.h"
#include "useful.h"

// パネルの型情報のリスト
enum PanelType {
	WALL, ROAD
};

// パネル一枚一枚
class Panel {
public:
	PanelType type;
};

// 区画のクラス
class Rect {
public:
	Rect(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey),splitvflag(true),splithflag(true) {
	}

	Rect() {
	}

	bool splitvflag, splithflag;
	int sx, sy, ex, ey;
};

constexpr int MINIMUM_ROOM_SIZE = 4;
constexpr int MINIMUM_RECT_SIZE = MINIMUM_ROOM_SIZE * 2;

// 部屋
class Room: public Rect {
public:
	Room(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey) {
	}

	int sx, sy, ex, ey;
};

// 部屋同士のペア
class RoomPair {
public:
	RoomPair(bool isSide, Rect* rect1, Rect* rect2):isSide(isSide) {
		this->pair = std::make_pair(rect1, rect2);
	}

	std::pair<Rect*, Rect*> pair;
	bool isSide;	// つながりの方向(横か縦か)
};

// マップ本体
class Map {
private:
	int sizeX, sizeY;
	std::vector<Panel> body;

	std::vector<Rect* > rects;
	std::vector<Room* > rooms;
	std::vector<RoomPair* > roomPairs;

	// マップ自動生成するやつ
	void mapSplitter(Rect* root);
	void genRooms();
	void genRndMap();

	// 分割や部屋割り当てを反映する関数
	void reflectRects();


public:
	Map(int sizeX, int sizeY);
	~Map();

	int calcIndex(int x, int y) {
		return this->sizeX*y + x;
	}

	void Draw();
}; 