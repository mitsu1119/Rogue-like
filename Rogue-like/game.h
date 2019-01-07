#pragma once
#include "map.h"
#include "player.h"
#include "useful.h"
#include "DxLib.h"
#include <vector>

class Game {
private:
	// ウィンドウのサイズ
	int windowSizeX, windowSizeY;

	bool attackPFlag, movePFlag, moveEFlag;
	int maxFocusX, maxFocusY;
	std::vector<Pic> mapchips;

public:
	Game(int maxFocusX, int maxFocusY);
	~Game();

	Map *map;
	Player *player;

	// 毎ループ実行するべき関数
	void reflect();
};