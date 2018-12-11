#include "map.h"

Map::Map(int sizeX, int sizeY, std::vector<Pic> mapchips, int focusPanelX, int focusPanelY, Player *player, Enemy *enemy):sizeX(sizeX), sizeY(sizeY), mapchips(mapchips), focusPanelX(focusPanelX), focusPanelY(focusPanelY), moveFlag(DirectionNum)
, movecnt(0), player(player) {
	SRand(GetNowCount());
	this->body = std::vector<Panel>(sizeX*sizeY);
	this->minibody = std::vector<Panel>(sizeX*sizeY);

	// body を自動生成
	genRndMap();

	// 敵の生成
	// *enemy をもとに同じ型の敵が作られていく
	for(int i = 0; i < this->sizeY; i++) {
		for(int j = 0; j < this->sizeX; j++) {
			if(this->body[calcIndex(j, i)].type == ROAD && GetRand(100) == 0) {
				this->enemys.emplace_back(j, i, player->speed, enemy->pic);
			}
		}
	}

	// 自機の配置
	int randrect = GetRand((int)this->rects.size() - 1);
	this->playerX = randAtoB(this->rects[randrect]->room->sx, this->rects[randrect]->room->ex);
	this->playerY = randAtoB(this->rects[randrect]->room->sy, this->rects[randrect]->room->ey);

	this->cameraX = -(this->playerX - focusPanelX / 2) * mapchips[ROAD].sizeX - mapchips[ROAD].sizeX / 2;
	this->cameraY = -(this->playerY - focusPanelY / 2) * mapchips[ROAD].sizeY - mapchips[ROAD].sizeY / 2;

	// ミニマップを生成
	for(int i = 0; i < (int)this->minibody.size(); i++) {
		switch(this->body[i].type) {
		case WALL:
			this->minibody[i].type = MINI_WALL;
			break;
		case ROAD:
			this->minibody[i].type = MINI_ROAD;
		};
	}
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
			if(j == this->playerX && i == this->playerY) printfDx("%s", "P");
			else if(this->body[calcIndex(j, i)].type == WALL) printfDx("%s", "#");
			else if(this->body[calcIndex(j, i)].type == ROAD) printfDx("%s", " ");
		}
		printfDx("\n");
	}
}

void Map::DrawPart(int screenSX, int screenSY, int panelSX, int panelSY, int panelEX, int panelEY) {
	int xsum = 0, ysum = 0;

	for(int i = panelSY; i <= panelEY; i++) {
		for(int j = panelSX; j <= panelEX; j++) {
			DrawGraph(screenSX + xsum, screenSY + ysum, this->mapchips.at(this->body[calcIndex(j, i)].type).handle, true);
			xsum +=this->mapchips.at(this->body[calcIndex(j, i)].type).sizeX;
		}
		xsum = 0;
		ysum +=this->mapchips.at(this->body[calcIndex(panelEX, i)].type).sizeY;
	}
}

void Map::DrawPt(int screenSX, int screenSY) {
	DrawPart(screenSX, screenSY, 0, 0, this->sizeX - 1, this->sizeY - 1);
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
	DrawGraph(screenSX + this->playerX * this->mapchips[MINI_ROAD].sizeX, screenSY + this->playerY * this->mapchips[MINI_ROAD].sizeY, this->mapchips[MINI_PLAYER].handle, true);
}

void Map::DrawFocus() {
	DrawPt(this->cameraX, this->cameraY);

	// 敵
	for(auto i : this->enemys) {
		// DrawGraph(this->cameraX + i.panelX * this->mapchips[ROAD].sizeX, this->cameraY + i.panelY * this->mapchips[ROAD].sizeY, i.pic.handle, true);
		DrawGraph(i.x, i.y, i.pic.handle, true);
	}

	// プレイヤー
	DrawGraph(1400 / 2 - this->player->pic.sizeX / 2, 800 / 2 - this->player->pic.sizeY / 2, this->player->pic.handle, true);
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
	for(int i = 0; i < (int)this->body.size(); i++) this->body[i].type = WALL;
	Rect* root = new Rect(0, 0, this->sizeX - 1, this->sizeY - 1);
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

bool Map::canMove(Direction direction) {
	if(this->moveFlag != DirectionNum) return false;

	int nextX = this->playerX + directionDx(direction);
	int nextY = this->playerY + directionDy(direction);

	if(nextX < 0 || nextX >= this->sizeX) return false;
	if(nextY < 0 || nextY >= this->sizeY) return false;
	if(this->body[calcIndex(nextX, nextY)].type == WALL) return false;

	// 斜め移動の時は上と横も考える
	// 後で剰余の式にして一発ですます
	if(direction == RUP) {
		if(this->body[calcIndex(nextX, nextY + 1)].type == WALL) return false;
		if(this->body[calcIndex(nextX - 1, nextY)].type == WALL) return false;
	}
	if(direction == RDOWN) {
		if(this->body[calcIndex(nextX, nextY - 1)].type == WALL) return false;
		if(this->body[calcIndex(nextX - 1, nextY)].type == WALL) return false;
	}
	if(direction == LUP) {
		if(this->body[calcIndex(nextX, nextY + 1)].type == WALL) return false;
		if(this->body[calcIndex(nextX + 1, nextY)].type == WALL) return false;
	}
	if(direction == LDOWN) {
		if(this->body[calcIndex(nextX, nextY - 1)].type == WALL) return false;
		if(this->body[calcIndex(nextX + 1, nextY)].type == WALL) return false;
	}

	return true;
}

void Map::movePlayer(Direction direction) {
	if(!canMove(direction)) return;
	this->moveFlag = direction;
	
	// 敵の動作処理
	for(auto &i : this->enemys) {	
		i.autoMove();
	}

	this->playerX += directionDx(direction);
	this->playerY += directionDy(direction);
}

void Map::reflect() {
	// プレイヤーの位置の更新
	if(this->moveFlag != DirectionNum) {
		this->cameraX -= directionDx(this->moveFlag) * this->player->speed;
		this->cameraY -= directionDy(this->moveFlag) * this->player->speed;
		this->movecnt++;
		if(this->movecnt >= this->mapchips[ROAD].sizeX / this->player->speed) {
			this->movecnt = 0;
			this->moveFlag = DirectionNum;
		}
	}

	// 敵の位置の更新
	for(auto &i : enemys) {
		if(i.isMoving()) {
			// 敵のアニメーション処理
			i.x = this->cameraX + (i.panelX - directionDx(i.moveFlag)) * this->mapchips[ROAD].sizeX + i.movecnt * i.speed * directionDx(i.moveFlag);
			i.y = this->cameraY + (i.panelY - directionDy(i.moveFlag)) * this->mapchips[ROAD].sizeY + i.movecnt * i.speed * directionDy(i.moveFlag);
			i.reflect();
		} else {
			i.x = this->cameraX + i.panelX * this->mapchips[ROAD].sizeX;
			i.y = this->cameraY + i.panelY * this->mapchips[ROAD].sizeY;
		}

		if(i.movecnt >= this->mapchips[ROAD].sizeX / i.speed) {
			i.endMoving();
		}
	}

	if(this->moveFlag == DirectionNum) {
		this->cameraX = -(this->playerX - this->focusPanelX / 2) * mapchips[ROAD].sizeX - mapchips[ROAD].sizeX / 2;
		this->cameraY = -(this->playerY - this->focusPanelY / 2) * mapchips[ROAD].sizeY - mapchips[ROAD].sizeY / 2;
	}
}