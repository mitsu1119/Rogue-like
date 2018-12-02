#pragma once
#include <vector>
#include "DxLib.h"

// パネルの型情報のリスト
enum PanelType {
	WALL, ROAD
};

class Panel {
public:
	PanelType type;
};

// 区画のクラス
class Rect {
public:
	int sx, sy, ex, ey;

	Rect(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey) {
	}
};
constexpr int MINIMUM_RECT_SIZE = 4;

class Map {
private:
	int sizeX, sizeY;
	std::vector<Panel> body;

	// マップ自動生成するやつ
	std::vector<Rect> rects;
	void mapSplitter(Rect root);
	void genRndMap();

public:
	Map(int sizeX, int sizeY);

	int calcIndex(int x, int y) {
		return this->sizeX*y + x;
	}

	void Draw();
}; 