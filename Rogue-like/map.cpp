#include "map.h"

Map::Map(int sizeX, int sizeY, std::vector<Pic> mapchips, int focusPanelX, int focusPanelY, Player *player):sizeX(sizeX), sizeY(sizeY), mapchips(mapchips), focusPanelX(focusPanelX), focusPanelY(focusPanelY), player(player),
nextMapFlag(false) {
	SRand(GetNowCount());
	this->body = std::vector<Panel>(sizeX*sizeY);
	this->body2 = std::vector<Panel>(sizeX * sizeY, Panel(TypeNum));
	this->body3 = std::vector<Panel>(sizeX * sizeY, Panel(TypeNum));
	this->minibody = std::vector<Panel>(sizeX*sizeY);

	// body を自動生成
	genRndMap();
	for(size_t i = 0; i < body.size(); i++) {
		// 壁の整形(右側などの割り当て)
		// 四隅
		if(i == 0 || i == sizeX - 1 || i == this->body.size() - sizeX || i == this->body.size() - 1) {
			this->body[i].type = WALL;
			continue;
		}
		if(this->body[i].type == WALL_DOWN) {
			// 四辺
			// 上
			if(i > 0 && i < sizeX && this->body[i + sizeX].type != ROAD) {
				this->body[i].type = WALL;
				continue;
			}
			// 右
			if(i % sizeX == sizeX - 1 && this->body[i - 1].type != ROAD) {
				this->body[i].type = WALL;
				continue;
			}
			// 下
			if(i >= this->body.size() - sizeX && this->body[i - sizeX].type != ROAD) {
				this->body[i].type = WALL;
				continue;
			}
			// 左
			if(i % sizeX == 0 && this->body[i + 1].type != ROAD) {
				this->body[i].type = WALL;
				continue;
			}

			// 四隅と辺 以外のところ(真ん中の部分)
			if(i % sizeX != 0 && i % sizeX != sizeX - 1 && this->body[i + 1].type == ROAD && this->body[i - 1].type == ROAD) {
				if(i < this->body.size() - sizeX && this->body[i + sizeX].type == ROAD) this->body[i].type = WALL_SINGLEDOWN;
				else this->body[i].type = WALL_SINGLE;
				continue;
			}
			if(i <= this->body.size() - 1 - sizeX && i >= sizeX && i % sizeX != 0 && i % sizeX != sizeX - 1 &&
				this->body[i - 1].type != ROAD && this->body[i + 1].type != ROAD && this->body[i + sizeX].type != ROAD) {
				this->body[i].type = WALL;
				continue;
			}
			if(i < this->body.size() - 1 - sizeX && this->body[i + 1].type == ROAD && this->body[i + sizeX].type  == ROAD) {
				this->body[i].type = WALL_RDOWN;
				continue;
			}
			if(i > 0 && i < this->body.size() - 1 - sizeX && this->body[i - 1].type == ROAD && this->body[i + sizeX].type == ROAD) {
				this->body[i].type = WALL_LDOWN;
				continue;
			}
			if(i % sizeX != sizeX - 1 && this->body[i + 1].type == ROAD) {
				this->body[i].type = WALL_RIGHT;
				continue;
			}
			if(i % sizeX != 0 && this->body[i - 1].type == ROAD) {
				this->body[i].type = WALL_LEFT;
				continue;
			}
		}
	}
	for(size_t i = 0; i < body.size(); i++) {
		// 内側の整形
		// inldown など
		if(this->body[i].type != ROAD) {
			if(i < this->body.size() - sizeX && i % sizeX != 0 && (this->body[i + sizeX].type == WALL_LDOWN || this->body[i + sizeX].type ==WALL_LEFT) && 
				(this->body[i - 1].type == WALL || this->body[i - 1].type == WALL_DOWN || this->body[i - 1].type == WALL_LDOWN)) {
				this->body[i].type = WALL_INRUP;
			}
			if(i % sizeX != 0 && i > sizeX && i < this->body.size() - sizeX && (this->body[i  - sizeX].type == WALL_LEFT) &&
				(this->body[i - 1].type == WALL || this->body[i - 1].type == WALL_LEFT || this->body[i - 1].type == WALL_LEFT || this->body[i - 1].type == WALL_DOWN)) {
				this->body[i - sizeX].type = WALL_INRDOWN;
			}
			if(i % sizeX != sizeX - 1 && i > sizeX && i < this->body.size() - sizeX && (this->body[i - sizeX].type == WALL_RIGHT) &&
				(this->body[i + 1].type == WALL || this->body[i + 1].type == WALL_RIGHT || this->body[i + 1].type == WALL_RIGHT || this->body[i + 1].type == WALL_DOWN)) {
				this->body[i - sizeX].type = WALL_INLDOWN;
			}
			if(i % sizeX != sizeX - 1 && i < this->body.size() - sizeX && (this->body[i + 1].type == WALL_DOWN || this->body[i + 1].type == WALL_RDOWN) && (this->body[i + sizeX].type == WALL_RIGHT || this->body[i + sizeX].type == WALL_RDOWN)) {
				this->body[i].type = WALL_INLUP;
			}
			if(i > sizeX && (this->body[i].type == WALL_SINGLE || this->body[i].type == WALL_SINGLEDOWN) && this->body[i - sizeX].type == WALL /* 実質 WALLDOWN */) {
				this->body[i - sizeX].type = WALL_SINGLE_JOINTDOWN;
			}
			if(i > sizeX && (this->body[i].type == WALL_SINGLE || this->body[i].type == WALL_SINGLEDOWN) && this->body[i - sizeX].type == WALL_RIGHT) {
				this->body[i - sizeX].type = WALL_SINGLE_JOINTRIGHT_UP;
			}
			if(i > sizeX && (this->body[i].type == WALL_SINGLE || this->body[i].type == WALL_SINGLEDOWN) && this->body[i - sizeX].type == WALL_LEFT) {
				this->body[i - sizeX].type = WALL_SINGLE_JOINTLEFT_UP;
			}
			if(i < this->body.size() - sizeX && this->body[i].type == WALL_SINGLE && this->body[i + sizeX].type == WALL_RIGHT) {
				this->body[i].type = WALL_SINGLE_JOINTRIGHT_DOWN;
			}
			if(i < this->body.size() - sizeX && this->body[i].type == WALL_SINGLE && this->body[i + sizeX].type == WALL_LEFT) {
				this->body[i].type = WALL_SINGLE_JOINTLEFT_DOWN;
			}
		}
	}

	for(auto &i : this->body) {
		switch(i.type) {
		case WALL_RIGHT:
		case WALL_RDOWN:
		case WALL_DOWN:
		case WALL_LDOWN:
		case WALL_LEFT:
		case WALL:
		case WALL_INRUP:
		case WALL_INRDOWN:
		case WALL_INLUP:
		case WALL_INLDOWN:
		case WALL_SINGLEDOWN:
		case WALL_SINGLE:
		case WALL_SINGLE_JOINTDOWN:
		case WALL_SINGLE_JOINTRIGHT_UP:
		case WALL_SINGLE_JOINTLEFT_UP:
		case WALL_SINGLE_JOINTRIGHT_DOWN:
		case WALL_SINGLE_JOINTLEFT_DOWN:
			this->movable.emplace_back(false);
			break;
		default:
			this->movable.emplace_back(true);
		};
	}

	// body2,3 を生成
	for(size_t i = sizeX; i < body.size(); i++) {
		if((body[i].type == WALL_DOWN || body[i].type == WALL || body[i].type == WALL_INLUP || body[i].type == WALL_INRUP) && body[i - sizeX].type == ROAD) {
			this->body2[i - sizeX] = WALL_UP;
			continue;
		}
		if((body[i].type == WALL_RIGHT || body[i].type == WALL_RDOWN || body[i].type == WALL_INLDOWN) && body[i - sizeX].type == ROAD) {
			this->body2[i - sizeX] = WALL_RUP;
			continue;
		}
		if((body[i].type == WALL_LEFT || body[i].type == WALL_LDOWN || body[i].type == WALL_INRDOWN) && body[i - sizeX].type == ROAD) {
			this->body2[i - sizeX] = WALL_LUP;
			continue;
		}
		if(body[i].type == WALL_SINGLE) {
			this->body2[i + 1] = WALL_SINGLERIGHT;
			this->body3[i - 1] = WALL_SINGLELEFT;
			if(body[i - sizeX].type != WALL_SINGLE) {
				this->body2[i + 1 - sizeX] = WALL_SINGLERUP;
				this->body2[i - 1 - sizeX] = WALL_SINGLELUP;
				this->body2[i  - sizeX] = WALL_UP;
			}
			continue;
		}
		if(body[i].type == WALL_SINGLEDOWN && body[i - sizeX].type != WALL_SINGLE) {
			this->body3[i + 1] = WALL_SINGLERDOWN;
			this->body3[i - 1] = WALL_SINGLELDOWN;
			this->body2[i - sizeX] = WALL_UP;
			continue;
		}
	}

	// 敵の生成
	// TODO: Enemyのデータベースを作る
	std::vector<Pic> pup = {Pic(LoadGraph("dat\\enemy_up1.png"), 100, 200), Pic(LoadGraph("dat\\enemy_up2.png"), 100, 200), Pic(LoadGraph("dat\\enemy_up3.png"), 100, 200), Pic(LoadGraph("dat\\enemy_up4.png"), 100, 200)};
	std::vector<Pic> prup = {Pic(LoadGraph("dat\\enemy_rup1.png"), 100, 200), Pic(LoadGraph("dat\\enemy_rup2.png"), 100, 200), Pic(LoadGraph("dat\\enemy_rup3.png"), 100, 200), Pic(LoadGraph("dat\\enemy_rup4.png"), 100, 200)};
	std::vector<Pic> pright = {Pic(LoadGraph("dat\\enemy_right1.png"), 100, 200), Pic(LoadGraph("dat\\enemy_right2.png"), 100, 200), Pic(LoadGraph("dat\\enemy_right3.png"), 100, 200), Pic(LoadGraph("dat\\enemy_right4.png"), 100, 200)};
	std::vector<Pic> prdown = {Pic(LoadGraph("dat\\enemy_rdown1.png"), 100, 200), Pic(LoadGraph("dat\\enemy_rdown2.png"), 100, 200), Pic(LoadGraph("dat\\enemy_rdown3.png"), 100, 200), Pic(LoadGraph("dat\\enemy_rdown4.png"), 100, 200)};
	std::vector<Pic> pdown = {Pic(LoadGraph("dat\\enemy_down1.png"), 100, 200), Pic(LoadGraph("dat\\enemy_down2.png"), 100, 200), Pic(LoadGraph("dat\\enemy_down3.png"), 100, 200), Pic(LoadGraph("dat\\enemy_down4.png"), 100, 200)};
	std::vector<Pic> pldown = {Pic(LoadGraph("dat\\enemy_ldown1.png"), 100, 200), Pic(LoadGraph("dat\\enemy_ldown2.png"), 100, 200), Pic(LoadGraph("dat\\enemy_ldown3.png"), 100, 200), Pic(LoadGraph("dat\\enemy_ldown4.png"), 100, 200)};
	std::vector<Pic> pleft = {Pic(LoadGraph("dat\\enemy_left1.png"), 100, 200), Pic(LoadGraph("dat\\enemy_left2.png"), 100, 200), Pic(LoadGraph("dat\\enemy_left3.png"), 100, 200), Pic(LoadGraph("dat\\enemy_left4.png"), 100, 200)};
	std::vector<Pic> plup = {Pic(LoadGraph("dat\\enemy_lup1.png"), 100, 200), Pic(LoadGraph("dat\\enemy_lup2.png"), 100, 200), Pic(LoadGraph("dat\\enemy_lup3.png"), 100, 200), Pic(LoadGraph("dat\\enemy_lup4.png"), 100, 200)};
	std::vector<Animation> enemy = {Animation(pup, {3, 5, 4, 5}), Animation(prup, {3, 5, 4, 5}), Animation(pright, {3, 5, 4, 5}), Animation(prdown, {3, 5, 4, 5}), Animation(pdown, {3, 5, 4, 5}), Animation(pldown, {3, 5, 4, 5}), Animation(pleft, {3, 5, 4, 5}), Animation(plup, {3, 5, 4, 5})};

	for(int i = 0; i < this->sizeY; i++) {
		for(int j = 0; j < this->sizeX; j++) {
			if(this->body[calcIndex(j, i)].type == ROAD && GetRand(200) == 0) {
				this->enemys.emplace_back(j, i, 7, enemy, this->mapchips[ROAD].sizeX, Parameter(50, 0));
				this->movable[calcIndex(j, i)] = false;
			}
		}
	}

	// 自機の配置
	int randrect = GetRand((int)this->rects.size() - 1);
	this->player->panelX = randAtoB(this->rects[randrect]->room->sx, this->rects[randrect]->room->ex);
	this->player->panelY = randAtoB(this->rects[randrect]->room->sy, this->rects[randrect]->room->ey);
	this->movable[calcIndex(this->player->panelX, this->player->panelY)] = false;

	// 階段の配置
	randrect = GetRand((int)this->rects.size() - 1);
	int stairsPanelX = randAtoB(this->rects[randrect]->room->sx, this->rects[randrect]->room->ex);
	int stairsPanelY = randAtoB(this->rects[randrect]->room->sy, this->rects[randrect]->room->ey);
	this->body[calcIndex(stairsPanelX, stairsPanelY)].type = STAIRS;
	this->movable[calcIndex(stairsPanelX, stairsPanelY)] = true;
	this->traps.emplace_back(stairsPanelX, stairsPanelY, STAIRS);
	
	// ミニマップを生成
	for(int i = 0; i < (int)this->minibody.size(); i++) {
		switch(this->body[i].type) {
		case WALL_RIGHT:
		case WALL_RDOWN:
		case WALL_DOWN:
		case WALL_LDOWN:
		case WALL_LEFT:
		case WALL:
		case WALL_INRUP:
		case WALL_INRDOWN:
		case WALL_INLUP:
		case WALL_INLDOWN:
		case WALL_SINGLEDOWN:
		case WALL_SINGLE:
		case WALL_SINGLE_JOINTDOWN:
		case WALL_SINGLE_JOINTRIGHT_UP:
		case WALL_SINGLE_JOINTLEFT_UP:
		case WALL_SINGLE_JOINTRIGHT_DOWN:
		case WALL_SINGLE_JOINTLEFT_DOWN:
			this->minibody[i].type = MINI_WALL;
			break;
		case ROAD:
		case TypeNum:
			this->minibody[i].type = MINI_ROAD;
			break;
		case STAIRS:
			this->minibody[i].type = MINI_STAIRS;
			break;
		};
	}
	revice();
}

Map::~Map() {
	for(auto i : this->rects) {
		delete i->room;
		delete i;
	}

	for(auto i : this->roomPairs) delete i;
}

void Map::Print() {
	for(int i = 0; i < this->sizeY; i++) {
		for(int j = 0; j < this->sizeX; j++) {
			// 壁なら#, 道なら を描画
			if(j == this->player->panelX && i == this->player->panelY) printfDx("%s", "P");
			else if(this->body[calcIndex(j, i)].type == WALL_DOWN) printfDx("%s", "#");
			else if(this->body[calcIndex(j, i)].type == ROAD) printfDx("%s", " ");
		}
		printfDx("\n");
	}
}

void Map::revice() {
	this->cameraX = 0;
	this->cameraY = 0;
	this->player->reviceCoord(false, false);
	for(auto &i : this->enemys) i.reviceCoord(false, false);

	// focusPanelX,Y が偶数だったとき半分だけ横にずらす処理
	if(this->focusPanelX % 2 == 0) scroll(-this->mapchips[ROAD].sizeX / 2, 0);
	if(this->focusPanelY % 2 == 0) scroll(0, -this->mapchips[ROAD].sizeY / 2);

	// 実際の座標に表示した後マップをスクロールしてきて画面内の中央にプレイヤーを持ってくるしょり
	scrollPanel(-this->player->panelX + this->focusPanelX / 2, -this->player->panelY + this->focusPanelY / 2);
}

void Map::moveEnemys() {
	Direction dir;
	for(auto &i : this->enemys) {
		do {
			dir = (Direction)randAtoB(0, DirectionNum - 1);
		} while(!canMove(i.panelX, i.panelY, dir));
		beMovable(i.panelX, i.panelY);
		i.move(dir);
		dontbeMovable(i.panelX, i.panelY);
	}
}

bool Map::moveAnimationEnemys() {
	bool ret = false;
	for(auto &i : this->enemys) {
		if(i.moveAnimation()) ret = true;
	}

	// 一体でも動けたとき true
	return ret;
}

int Map::searchEnemy(int panelX, int panelY) {
	for(int i = 0; i<(int)this->enemys.size(); i++) {
		if(this->enemys[i].panelX == panelX && this->enemys[i].panelY == panelY) {
			return i;
		}
	}

	return -1;
}

void Map::dieEnemy(int index) {
	if(index >= 0 && index < this->enemys.size() && this->enemys[index].isDead()) {
		beMovable(this->enemys[index].panelX, this->enemys[index].panelY);
		this->enemys.erase(this->enemys.begin() + index);
	}
}

void Map::DrawPart(int screenSX, int screenSY, int panelSX, int panelSY, int panelEX, int panelEY, std::vector<Panel> &map) {
	int xsum, ysum;
	int initxsum = this->mapchips.at(map[0].type).sizeX * (std::max)(0, panelSX);
	int initysum = this->mapchips.at(map[0].type).sizeY * (std::max)(0, panelSY);

	xsum = initxsum;
	ysum = initysum;

	for(int i = (std::max)(0, panelSY); i <= (std::min)(panelEY, this->sizeY - 1); i++) {
		for(int j = (std::max)(0, panelSX); j <= (std::min)(panelEX, this->sizeX - 1); j++) {
			DrawGraph(screenSX + xsum, screenSY + ysum, this->mapchips.at(map[calcIndex(j, i)].type).handle, true);
			xsum +=this->mapchips.at(map[calcIndex(j, i)].type).sizeX;
		}
		xsum = initxsum;
		ysum +=this->mapchips.at(map[0].type).sizeY;
	}
}

void Map::DrawPt(int screenSX, int screenSY) {
	DrawPart(screenSX, screenSY, this->player->panelX - this->focusPanelX/2-1, this->player->panelY - this->focusPanelY/2-1, this->player->panelX + this->focusPanelX/2+1, this->player->panelY + this->focusPanelY/2+1, this->body);
}

void Map::DrawPt2(int screenSX, int screenSY) {
	DrawPart(screenSX, screenSY, this->player->panelX - this->focusPanelX/2-1, this->player->panelY - this->focusPanelY/2-1, this->player->panelX + this->focusPanelX/2+1, this->player->panelY + this->focusPanelY/2+1, this->body2);
}

void Map::DrawPt3(int screenSX, int screenSY) {
	DrawPart(screenSX, screenSY, this->player->panelX - this->focusPanelX/2-1, this->player->panelY - this->focusPanelY/2-1, this->player->panelX + this->focusPanelX/2+1, this->player->panelY + this->focusPanelY/2+1, this->body3);
}

void Map::DrawMinimap(int screenSX, int screenSY) {
	int xsum = 0, ysum = 0;

	for(int i = 0; i < this->sizeY; i++) {
		for(int j = 0; j < this->sizeX; j++) {
			DrawGraph(screenSX + xsum, screenSY + ysum, this->mapchips.at(this->minibody[calcIndex(j, i)].type).handle, true);
			xsum += this->mapchips.at(this->minibody[calcIndex(j, i)].type).sizeX;
		}
		xsum = 0;
		ysum += this->mapchips.at(this->minibody[calcIndex(this->sizeX - 1, i)].type).sizeY;
	}

	// 敵の表示
	for(auto i : this->enemys) {
		DrawGraph(screenSX + i.panelX * this->mapchips[MINI_ROAD].sizeX, screenSY + i.panelY * this->mapchips[MINI_ROAD].sizeY, this->mapchips[MINI_ENEMY].handle, true);
	}

	// プレイヤーの表示
	DrawGraph(screenSX + this->player->panelX * this->mapchips[MINI_ROAD].sizeX, screenSY + this->player->panelY * this->mapchips[MINI_ROAD].sizeY, this->mapchips[MINI_PLAYER].handle, true);
}

void Map::DrawFocus() {
	DrawPt(this->cameraX, this->cameraY);

	// 敵
	for(auto i : this->enemys) i.Draw();

	// プレイヤー
	this->player->Draw();

	// body2
	DrawPt2(this->cameraX, this->cameraY);
	DrawPt3(this->cameraX, this->cameraY);
}

// 再起処理でマップを分割するやつ
void Map::mapSplitter(Rect* root) {
	if(root->ey - root->sy <= MINIMUM_RECT_SIZE * 2) root->splitvflag = false;
	if(root->ex - root->sx <= MINIMUM_RECT_SIZE * 2) root->splithflag = false;
	if(!(root->splitvflag) && !(root->splithflag)) return;

	Rect* child = new Rect(root->sx, root->sy, root->ex, root->ey);
	this->rects.emplace_back(child);

	if(root->splitvflag) {
		// 縦分割
		int splitCoord = randAtoB(root->sy + MINIMUM_RECT_SIZE, root->ey - MINIMUM_RECT_SIZE - 1);
		root->ey = splitCoord;
		child->sy = splitCoord;
		root->splitvflag = false;
		child->splitvflag = false;
		this->roomPairs.emplace_back(new RoomPair(false, root, child));
		mapSplitter(root);
		mapSplitter(child);
		return;
	}

	if(root->splithflag) {
		// 横分割 left | right
		int splitCoord = randAtoB(root->sx + MINIMUM_RECT_SIZE, root->ex - MINIMUM_RECT_SIZE - 1);
		root->ex = splitCoord;
		child->sx = splitCoord; 
		root->splithflag = false;
		child->splithflag = false;
		this->roomPairs.emplace_back(new RoomPair(true, root, child));
		mapSplitter(root);
		mapSplitter(child);
		return;
	}
}

void Map::makePairAdd() {
	std::vector<Rect* > coordRectType(this->sizeX*this->sizeY);

	int j, k;
	for(auto i : this->rects) {
		for(j = i->sx; j < i->ex; j++)
			for(k = i->sy; k < i->ey; k++) coordRectType[calcIndex(j, k)] = i;
	}

	// 左から右, 上から下へつなぐので i < this->sizeX - 2, j < this->sizeY - 2
	for(int i = 0; i < this->sizeX - 2; i++) {
		for(j = 0; j < this->sizeY - 2; j++) {
			// 横
			if(coordRectType[calcIndex(i, j)] != coordRectType[calcIndex(i + 1, j)] && GetRand(69) == 0)
				this->roomPairs.emplace_back(new RoomPair(true, coordRectType[calcIndex(i, j)], coordRectType[calcIndex(i + 1, j)]));
			// 縦
			if(coordRectType[calcIndex(i, j)] != coordRectType[calcIndex(i, j + 1)] && GetRand(69) == 0)
				this->roomPairs.emplace_back(new RoomPair(false, coordRectType[calcIndex(i, j)], coordRectType[calcIndex(i, j + 1)]));
		}
	}
}

// 部屋を生成するやつ
void Map::genRooms() {
	int x, y, w, h;
	for(auto i : this->rects) {
		w = randAtoB(MINIMUM_ROOM_SIZE, i->ex - i->sx - MINIMUM_ROOM_SIZE);
		h = randAtoB(MINIMUM_ROOM_SIZE, i->ey - i->sy - MINIMUM_ROOM_SIZE);
		x = randAtoB(i->sx + MINIMUM_ROOM_SIZE / 2, i->ex - MINIMUM_ROOM_SIZE / 2 - w);
		y = randAtoB(i->sy + MINIMUM_ROOM_SIZE / 2, i->ey - MINIMUM_ROOM_SIZE / 2 - h);
		i->room = new Room(x, y, x + w, y + h);
	}
}

// マップの分割を管理する関数
void Map::genRndMap() {
	for(int i = 0; i < (int)this->body.size(); i++) this->body[i].type = WALL_DOWN;
	Rect *root = new Rect(0, 0, this->sizeX - 1, this->sizeY - 1);
	this->rects.emplace_back(root);

	mapSplitter(root);
	makePairAdd();
	genRooms();
	reflectRects();
}

void Map::makeLine(int sx, int sy, int ex, int ey) {
	int minx = (std::min)(sx, ex);
	int miny = (std::min)(sy, ey);
	int maxx = (std::max)(sx, ex);
	int maxy = (std::max)(sy, ey);

	if(minx < 0 || miny < 0 || maxx >= this->sizeX || maxy >= this->sizeY) exit(1);

	// [0, pi/2]
	if(sx <= ex && sy >= ey) {
		for(int i = minx; i <= maxx; i++) this->body[calcIndex(i, maxy)].type = ROAD;
		for(int i = miny; i <= maxy; i++) this->body[calcIndex(maxx, i)].type = ROAD;
		return;
	}

	// (pi/2, pi)
	if(sx > ex && sy > ey) {
		for(int i = minx; i <= maxx; i++) this->body[calcIndex(i, miny)].type = ROAD;
		for(int i = miny; i <= maxy; i++) this->body[calcIndex(maxx, i)].type = ROAD;
		return;
	}

	// [pi, 3pi/2)
	if(sx > ex && sy <= ey) {
		for(int i = minx; i <= maxx; i++) this->body[calcIndex(i, miny)].type = ROAD;
		for(int i = miny; i <= maxy; i++) this->body[calcIndex(minx, i)].type = ROAD;
		return;
	}

	// [3pi/2, 2pi)
	if(sx <= ex && sy < ey) {
		for(int i = minx; i <= maxx; i++) this->body[calcIndex(i, maxy)].type = ROAD;
		for(int i = miny; i <= maxy; i++) this->body[calcIndex(minx, i)].type = ROAD;
		return;
	}
}

void Map::reflectRects() {
	int j, k;
	
	// 区画割り当て
	for(auto i : this->rects) {
		// 部屋割り当て
		for(j = i->room->sx; j <= i->room->ex; j++) {
			for(k = i->room->sy; k <= i->room->ey; k++) {
				this->body[calcIndex(j, k)].type = ROAD;
			}
		}
	}

	// 部屋から通路を伸ばす
	int rect0x, rect0y, rect1x, rect1y;
	for(auto i : this->roomPairs) {
		// x方向へ伸ばす
		if(i->isSide) {
			// 当然区画1のexと区画2のsxは等しい
			if(i->pair.first->ex != i->pair.second->sx) exit(1);
			
			rect0x = i->pair.first->ex;
			rect0y = randAtoB(i->pair.first->room->sy + 1, i->pair.first->room->ey - 1);
			rect1x = i->pair.second->sx;
			rect1y = randAtoB(i->pair.second->room->sy + 1, i->pair.second->room->ey - 1);

			// 区画の境界 A,B を区画としたとき  A | B の | の部分
			makeLine(rect0x, rect0y, rect1x, rect1y);

			// 区画Aから境界への足
			makeLine(i->pair.first->room->ex, rect0y, rect0x, rect0y);

			// 区画Bからの境界への足
			makeLine(i->pair.second->room->sx, rect1y, rect1x, rect1y);
		} else {
			if(i->pair.first->ey != i->pair.second->sy) exit(1);

			rect0x = randAtoB(i->pair.first->room->sx + 1, i->pair.first->room->ex - 1);
			rect0y = i->pair.first->ey;
			rect1x = randAtoB(i->pair.second->room->sx + 1, i->pair.second->room->ex - 1);
			rect1y = i->pair.second->sy;

			makeLine(rect0x, rect0y, rect1x, rect1y);
			makeLine(rect0x, i->pair.first->room->ey, rect0x, rect0y);
			makeLine(rect1x, i->pair.second->room->sy, rect1x, rect1y);
		}
	}
}

bool Map::canMove(int panelX, int panelY, Direction direction) {
	int nextX = panelX + directionDx(direction);
	int nextY = panelY + directionDy(direction);

	if(nextX < 0 || nextX >= this->sizeX) return false;
	if(nextY < 0 || nextY >= this->sizeY) return false;
	if(!this->movable[calcIndex(nextX, nextY)]) return false;

	// 斜め移動の時は上と横も考える
	// 後で剰余の式にして一発ですます
	if(direction == RUP) {
		if(!this->movable[calcIndex(nextX, nextY + 1)]) return false;
		if(!this->movable[calcIndex(nextX - 1, nextY)]) return false;
	}
	if(direction == RDOWN) {
		if(!this->movable[calcIndex(nextX, nextY - 1)]) return false;
		if(!this->movable[calcIndex(nextX - 1, nextY)]) return false;
	}
	if(direction == LUP) {
		if(!this->movable[calcIndex(nextX, nextY + 1)]) return false;
		if(!this->movable[calcIndex(nextX + 1, nextY)]) return false;
	}
	if(direction == LDOWN) {
		if(!this->movable[calcIndex(nextX, nextY - 1)]) return false;
		if(!this->movable[calcIndex(nextX + 1, nextY)]) return false;
	}

	return true;
}

bool Map::keyProcessing() {
	/* キー入力処理 */
	if(CheckHitKey(KEY_INPUT_SPACE)) this->player->speed = 60;
	else this->player->speed = 7;

	if(CheckHitKey(KEY_INPUT_Z)) {
		Direction dir = this->player->attack();
		int e = searchEnemy(this->player->panelX + directionDx(dir), this->player->panelY + directionDy(dir));
		if(e != -1){
			this->enemys[e].damaged(this->player->calcDamage(0));
			if(this->enemys[e].isDead()) dieEnemy(e);
		}
		return true;
	} else if(CheckHitKey(KEY_INPUT_UP)) {
		if(CheckHitKey(KEY_INPUT_RIGHT)) {
			this->player->setFront(RUP);
			if(!canMove(this->player->panelX, this->player->panelY, RUP)) return false;
			beMovable(this->player->panelX, this->player->panelY);
			this->player->move(RUP);
		} else if(CheckHitKey(KEY_INPUT_LEFT)) {
			this->player->setFront(LUP);
			if(!canMove(this->player->panelX, this->player->panelY, LUP)) return false;
			beMovable(this->player->panelX, this->player->panelY);
			this->player->move(LUP);
		} else {
			this->player->setFront(UP);
			if(!canMove(this->player->panelX, this->player->panelY, UP)) return false;
			beMovable(this->player->panelX, this->player->panelY);
			this->player->move(UP);
		}
		dontbeMovable(this->player->panelX, this->player->panelY);
		return true;
	} else if(CheckHitKey(KEY_INPUT_DOWN)) {
		if(CheckHitKey(KEY_INPUT_RIGHT)) {
			this->player->setFront(RDOWN);
			if(!canMove(this->player->panelX, this->player->panelY, RDOWN)) return false;
			beMovable(this->player->panelX, this->player->panelY);
			this->player->move(RDOWN);
		} else if(CheckHitKey(KEY_INPUT_LEFT)) {
			this->player->setFront(LDOWN);
			if(!canMove(this->player->panelX, this->player->panelY, LDOWN)) return false;
			beMovable(this->player->panelX, this->player->panelY);
			this->player->move(LDOWN);
		} else {
			this->player->setFront(DOWN);
			if(!canMove(this->player->panelX, this->player->panelY, DOWN)) return false;
			beMovable(this->player->panelX, this->player->panelY);
			this->player->move(DOWN);
		}
		dontbeMovable(this->player->panelX, this->player->panelY);
		return true;
	} else if(CheckHitKey(KEY_INPUT_RIGHT)) {
		this->player->setFront(RIGHT);
		if(!canMove(this->player->panelX, this->player->panelY, RIGHT)) return false;
		beMovable(this->player->panelX, this->player->panelY);
		this->player->move(RIGHT);
		dontbeMovable(this->player->panelX, this->player->panelY);
		return true;
	} else if(CheckHitKey(KEY_INPUT_LEFT)) {
		this->player->setFront(LEFT);
		if(!canMove(this->player->panelX, this->player->panelY, LEFT)) return false;
		beMovable(this->player->panelX, this->player->panelY);
		this->player->move(LEFT);
		dontbeMovable(this->player->panelX, this->player->panelY);
		return true;
	}

	return false;
}

void Map::trapProcessing() {
	for(auto i: this->traps) {
		if(i.panelX == this->player->panelX && i.panelY == this->player->panelY) {
			switch(i.type) {
			case STAIRS:
				nextMap();
				return;
			default:
				break;
			};
		}
	}
}

void Map::scroll(int x, int y) {
	this->cameraX += x;
	this->cameraY += y;
	this->player->shift(x, y);
	for(auto &i : this->enemys) i.shift(x, y);
}

void Map::scrollPanel(int panelX, int panelY) {
	scroll(this->mapchips[ROAD].sizeX * panelX, this->mapchips[ROAD].sizeY * panelY);
}

void Map::nextMap() {
	this->nextMapFlag = true;
}