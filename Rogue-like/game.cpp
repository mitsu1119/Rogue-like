#include "game.h"

Game::Game(int maxFocusX, int maxFocusY): turn(0), turnOld(-1), maxFocusX(maxFocusX), maxFocusY(maxFocusY) {

	// TODO: �E�B���h�E�̃T�C�Y��������������悤�ɂ���
	this->windowSizeX = 1400;
	this->windowSizeY = 800;

	// TODO: �ꖇ�̉摜�𕪊����ă}�b�v�`�b�v�ɂł���悤�ɂ���
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\road.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniwall.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniroad.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniplayer.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\minienemy.png"), 6, 6));

	Pic playerPic(LoadGraph("dat\\player.png"), 100, 100);
	player = new Player(7, playerPic, mapchips[ROAD].sizeX);

	// �v���C���[�̃p�l�����W�y�ю����W��map�������Ɏ����Ō��肳���
	map = new Map(60, 46, this->mapchips, this->maxFocusX, this->maxFocusY, this->player);
} 

Game::~Game() {
	delete this->player;
	delete this->map;
} 

void Game::reflect() {
	if(!this->player->isMoving()) {
		this->turn = this->turnOld + 1;
		this->map->revice();
	}

	if(this->turn > this->turnOld) {
		if(map->keyProcessing()) {
			this->turnOld = this->turn;
			this->map->moveEnemys();
		}
	}

	if(this->player->isMoving()) {
		int dx, dy;
		this->map->moveAnimationEnemys();
		this->player->moveAnimation(dx, dy);
		this->map->scroll(-dx, -dy);
	}

	map->DrawFocus();
	map->DrawMinimap(6, 6);
}