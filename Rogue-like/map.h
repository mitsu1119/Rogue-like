#pragma once
#include <vector>
#include <algorithm>
#include "DxLib.h"
#include "useful.h"

// パネルの型情報のリスト
enum PanelType {
	WALL, ROAD, TypeNum
};

// パネル一枚一枚
class Panel {
public:
	PanelType type;
};

// 部屋
class Room {
public:
	Room(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey) {
	}
	
	Room() {
	}

	int sx, sy, ex, ey;
};

// 区画のクラス
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
//	std::vector<Panel> body;

	std::vector<Rect* > rects;
	std::vector<RoomPair* > roomPairs;

	// マップ自動生成するやつ
	void mapSplitter(Rect* root);
	void makePairAdd();	// 一本道になりがちだったのでさらにペアを作る
	void genRooms();
	void genRndMap();
	void makeLine(int sx, int sy, int ex, int ey);

	// 分割や部屋割り当てを反映する関数
	void reflectRects();

	// マップチップ
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

	// (screenSX, screenSY) から ([panelSX,panelEX],[panelSY,panelEY]) を計算して描画
	void DrawPart(int screenSX, int screenSY, int panelSX, int panelSY, int panelEX, int panelEY, std::vector<Pic>* mapchips);

	// (screenSX, screenSY) からマップを全部描画(つまりマップを(0, 0)から描画したものを平行移動したような感じ)
	void DrawPt(int screenSX, int screenSY);
	void DrawPtMapchips(int screenSX, int screenSY, std::vector<Pic>* mapchips);

	std::vector<Panel> body;
}; 