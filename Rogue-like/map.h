#pragma once
#include <vector>
#include <algorithm>
#include "DxLib.h"
#include "useful.h"
#include "player.h"

// パネルの型情報のリスト
// マップチップもこの順で作る
// WALL_SINGLE_JOINTLEFT_DOWN	下側でWALL_LEFTとSINGLEがつながるやつ
enum PanelType {
	WALL_UP, WALL_RUP, WALL_RIGHT, WALL_RDOWN, WALL_DOWN, WALL_LDOWN, WALL_LEFT, WALL_LUP, WALL, WALL_INRUP, WALL_INRDOWN, WALL_INLDOWN, WALL_INLUP,
	WALL_SINGLERUP, WALL_SINGLERIGHT, WALL_SINGLERDOWN, WALL_SINGLEDOWN, WALL_SINGLELDOWN, WALL_SINGLELEFT, WALL_SINGLELUP, WALL_SINGLE,
	WALL_SINGLE_JOINTDOWN, WALL_SINGLE_JOINTRIGHT_UP, WALL_SINGLE_JOINTLEFT_UP, WALL_SINGLE_JOINTRIGHT_DOWN, WALL_SINGLE_JOINTLEFT_DOWN,
	ROAD, STAIRS, MINI_WALL, MINI_ROAD, MINI_STAIRS, MINI_PLAYER, MINI_ENEMY, TypeNum
};

class Trap {
public:
	Trap(int panelX, int panelY, PanelType type):panelX(panelX), panelY(panelY), type(type) {}

	int panelX, panelY;
	PanelType type;
};

// パネル一枚一枚
class Panel {
public:
	Panel() {
	}
	Panel(PanelType type):type(type) {
	}
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
	std::vector<Panel> body;
	std::vector<Panel> body2;	// レイヤが一つ上、プレイヤーやエネミーの上に配置される。当たり判定はなし
	std::vector<Panel> body3;	// さらに上
	std::vector<Panel> minibody;
	std::vector<bool> movable;

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

	// (screenSX, screenSY) から ([panelSX,panelEX],[panelSY,panelEY]) を計算して描画
	void DrawPart(int screenSX, int screenSY, int panelSX, int panelSY, int panelEX, int panelEY, std::vector<Panel> &map);

	// マップを描画するとき、マップをどのくらいずらすか指定する変数
	int cameraX, cameraY;
	int focusPanelX, focusPanelY;

	// (screenSX, screenSY) からマップを全部描画(つまりマップを(0, 0)から描画したものを平行移動したような感じ)
	void DrawPt(int screenSX, int screenSY);
	void DrawPt2(int screenSX, int screenSY);
	void DrawPt3(int screenSX, int screenSY);

	// プレイヤーとマップの確認のみ
	void Print();

	// (panelX, panelY)からdirectionの方向へ動けるときtrue, 動けないときfalse
	bool canMove(int panelX, int panelY, Direction direction);

	Player *player;
	std::vector<Enemy> enemys;
	bool nextMapFlag;
	std::vector<Trap> traps;

	// (panelX, panelY) にいる敵のインデックスを返す
	int searchEnemy(int panelX, int panelY);

public:
	Map(int sizeX, int sizeY, std::vector<Pic> mapchips, int focusPanelX, int focusPanelY, Player *player);
	~Map();

	// マップのサイズ(タイルの枚数)
	int sizeX, sizeY;

	int calcIndex(int x, int y) {
		return this->sizeX*y + x;
	}

	void beMovable(int panelX, int panelY) {
		this->movable[calcIndex(panelX, panelY)] = true;
	}

	void dontbeMovable(int panelX, int panelY) {
		this->movable[calcIndex(panelX, panelY)] = false;
	}

	bool canThisGetNextMap() {
		return this->nextMapFlag;
	}

	// キーが押された: true
	bool keyProcessing();
	void trapProcessing();

	// 実座標でスクロール
	void scroll(int x, int y);
	// パネルの数でスクロール
	void scrollPanel(int panelX, int panelY);

	// プレイヤーやマップを表示する位置の調整など
	void revice();

	void moveEnemys();
	bool moveAnimationEnemys();
	void dieEnemy(int index);

	void DrawMinimap(int screenSX, int screenSY);
	void DrawFocus();

	void nextMap();
}; 