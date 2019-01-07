#pragma once
#include "map.h"
#include "player.h"
#include "useful.h"
#include "DxLib.h"
#include <vector>

class Game {
private:
	// �E�B���h�E�̃T�C�Y
	int windowSizeX, windowSizeY;

	bool attackPFlag, movePFlag, moveEFlag;
	int maxFocusX, maxFocusY;
	std::vector<Pic> mapchips;

public:
	Game(int maxFocusX, int maxFocusY);
	~Game();

	Map *map;
	Player *player;

	// �����[�v���s����ׂ��֐�
	void reflect();
};