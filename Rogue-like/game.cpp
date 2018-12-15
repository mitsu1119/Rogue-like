#include "game.h"

Game::Game(int maxFocusX, int maxFocusY): turn(0), turnOld(-1), maxFocusX(maxFocusX), maxFocusY(maxFocusY) {

	// TODO: ウィンドウのサイズを自動所得するようにする
	this->windowSizeX = 1400;
	this->windowSizeY = 800;

	// TODO: 一枚の画像を分割してマップチップにできるようにする
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\road.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniwall.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniroad.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniplayer.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\minienemy.png"), 6, 6));

	Pic playerPic(LoadGraph("dat\\player.png"), 100, 100);
	player = new Player(this->windowSizeX / 2 - playerPic.sizeX / 2, this->windowSizeY / 2 - playerPic.sizeY / 2, 7, playerPic);

	map = new Map(60, 46, this->mapchips, this->maxFocusX, this->maxFocusY, this->player);
} 

Game::~Game() {
	delete this->player;
	delete this->map;
} 

void Game::reflect() {
	if(!this->player->isMoving()) this->turn = this->turnOld + 1;

	if(this->turn > this->turnOld) {
		if(map->keyProcessing()) this->turnOld = this->turn;
	}

	if(this->player->isMoving()) {
		this->player->moveAnimation();
	}

	map->DrawFocus();
	map->DrawMinimap(6, 6);
	DrawFormatString(0, 0, GetColor(0, 0, 0) ,"panelX: %d, panelY: %d\n", this->player->panelX, this->player->panelY);
}