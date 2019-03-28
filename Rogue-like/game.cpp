#include "game.h"

Game::Game(int maxFocusX, int maxFocusY):attackPFlag(false), movePFlag(false), moveEFlag(false), maxFocusX(maxFocusX), maxFocusY(maxFocusY) {

	// TODO: ウィンドウのサイズを自動所得するようにする
	this->windowSizeX = 1400;
	this->windowSizeY = 800;

	// TODO: 一枚の画像を分割してマップチップにできるようにする
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_up.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_rup.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_right.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_rdown.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_down.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_ldown.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_left.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_lup.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_inrup.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_inrdown.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_inldown.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_inlup.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_singlerup.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_singleright.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_singlerdown.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_singledown.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_singleldown.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_singleleft.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_singlelup.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_single.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_single_jointdown.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_single_jointright_up.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_single_jointleft_up.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_single_jointright_down.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\wall_single_jointleft_down.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\road.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\stairs.png"), 100, 100));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniwall.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniroad.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\ministairs.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\miniplayer.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\minienemy.png"), 6, 6));
	mapchips.emplace_back(Pic(LoadGraph("dat\\typenum.png"), 100, 100));

	std::vector<Pic> pup = {Pic(LoadGraph("dat\\player_up2.png"), 100, 140), Pic(LoadGraph("dat\\player_up1.png"), 100, 140), Pic(LoadGraph("dat\\player_up2.png"), 100, 140), Pic(LoadGraph("dat\\player_up3.png"), 100, 140)};
	std::vector<Pic> prup = {Pic(LoadGraph("dat\\player_rup2.png"), 100, 140), Pic(LoadGraph("dat\\player_rup1.png"), 100, 140), Pic(LoadGraph("dat\\player_rup2.png"), 100, 140), Pic(LoadGraph("dat\\player_rup3.png"), 100, 140)};
	std::vector<Pic> pright = {Pic(LoadGraph("dat\\player_right2.png"), 100, 140), Pic(LoadGraph("dat\\player_right1.png"), 100, 140), Pic(LoadGraph("dat\\player_right2.png"), 100, 140), Pic(LoadGraph("dat\\player_right3.png"), 100, 140)};
	std::vector<Pic> prdown = {Pic(LoadGraph("dat\\player_rdown2.png"), 100, 140), Pic(LoadGraph("dat\\player_rdown1.png"), 100, 140), Pic(LoadGraph("dat\\player_rdown2.png"), 100, 140), Pic(LoadGraph("dat\\player_rdown3.png"), 100, 140)};
	std::vector<Pic> pdown = {Pic(LoadGraph("dat\\player_down2.png"), 100, 140), Pic(LoadGraph("dat\\player_down1.png"), 100, 140), Pic(LoadGraph("dat\\player_down2.png"), 100, 140), Pic(LoadGraph("dat\\player_down3.png"), 100, 140)};
	std::vector<Pic> pldown = {Pic(LoadGraph("dat\\player_ldown2.png"), 100, 140), Pic(LoadGraph("dat\\player_ldown1.png"), 100, 140), Pic(LoadGraph("dat\\player_ldown2.png"), 100, 140), Pic(LoadGraph("dat\\player_ldown3.png"), 100, 140)};
	std::vector<Pic> pleft = {Pic(LoadGraph("dat\\player_left2.png"), 100, 140), Pic(LoadGraph("dat\\player_left1.png"), 100, 140), Pic(LoadGraph("dat\\player_left2.png"), 100, 140), Pic(LoadGraph("dat\\player_left3.png"), 100, 140)};
	std::vector<Pic> plup = {Pic(LoadGraph("dat\\player_lup2.png"), 100, 140), Pic(LoadGraph("dat\\player_lup1.png"), 100, 140), Pic(LoadGraph("dat\\player_lup2.png"), 100, 140), Pic(LoadGraph("dat\\player_lup3.png"), 100, 140)};
	std::vector<Animation> playerPic = {Animation(pup, 3), Animation(prup, 3), Animation(pright, 3), Animation(prdown, 3), Animation(pdown, 3), Animation(pldown, 3), Animation(pleft, 3), Animation(plup, 3)};
	player = new Player(7, playerPic, mapchips[ROAD].sizeX, Parameter(100, 40));

	// プレイヤーのパネル座標及び実座標はmap生成時に自動で決定される
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