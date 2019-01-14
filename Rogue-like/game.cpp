#include "game.h"

Game::Game(int maxFocusX, int maxFocusY):attackPFlag(false), movePFlag(false), moveEFlag(false), maxFocusX(maxFocusX), maxFocusY(maxFocusY) {

	// TODO: �E�B���h�E�̃T�C�Y��������������悤�ɂ���
	this->windowSizeX = 1400;
	this->windowSizeY = 800;

	// TODO: �ꖇ�̉摜�𕪊����ă}�b�v�`�b�v�ɂł���悤�ɂ���
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\road.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\stairs.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniwall.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniroad.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\ministairs.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniplayer.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\minienemy.png"), 6, 6));

	Pic playerPic(LoadGraph("dat\\player.png"), 100, 100);
	player = new Player(7, playerPic, mapchips[ROAD].sizeX, Parameter(100, 40));

	// �v���C���[�̃p�l�����W�y�ю����W��map�������Ɏ����Ō��肳���
	map = new Map(100, 50, this->mapchips, this->maxFocusX, this->maxFocusY, this->player);
} 

Game::~Game() {
	delete this->player;
	delete this->map;
} 

void Game::reflect() {
	if(this->map->canThisGetNextMap()) {
		delete this->map;
		map = new Map(100, 50, this->mapchips, this->maxFocusX, this->maxFocusY, this->player);
	}

	if(!this->attackPFlag && !this->movePFlag && !this->moveEFlag) {
		this->map->revice();
		if(map->keyProcessing()) {
			if(this->player->isMoving()) {
				this->movePFlag = true;
			} else {
				this->attackPFlag = true;
			}
			if(movePFlag) this->map->moveEnemys();
		}
	}

	if(this->movePFlag) {
		int dx, dy;
		this->map->moveAnimationEnemys();
		if(!this->player->moveAnimation(dx, dy)) {
			this->movePFlag = false;
			this->map->trapProcessing();
		}
		this->map->scroll(-dx, -dy);
	}

	if(this->attackPFlag) {
		if(!this->player->attackAnimation()) {
			this->attackPFlag = false;
			this->moveEFlag = true;
			this->map->moveEnemys();
		}
	}

	if(this->moveEFlag) {
		if(!this->map->moveAnimationEnemys()) moveEFlag = false;
	}

	map->DrawFocus();
	map->DrawMinimap(6, 6);
}