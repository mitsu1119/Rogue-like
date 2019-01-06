#include "map.h"

Map::Map(int sizeX, int sizeY, std::vector<Pic> mapchips, int focusPanelX, int focusPanelY, Player *player):sizeX(sizeX), sizeY(sizeY), mapchips(mapchips), focusPanelX(focusPanelX), focusPanelY(focusPanelY), moveFlag(DirectionNum)
, movecnt(0), player(player) {
	SRand(GetNowCount());
	this->body = std::vector<Panel>(sizeX*sizeY);
	this->minibody = std::vector<Panel>(sizeX*sizeY);

	// body ����������
	genRndMap();

	// �G�̐���
	// enemy �͌�Ńf�[�^�x�[�X������Ă���
	Pic enemy = Pic(LoadGraph("dat\\enemy.png"), 100, 100);
	for(int i = 0; i < this->sizeY; i++) {
		for(int j = 0; j < this->sizeX; j++) {
			if(this->body[calcIndex(j, i)].type == ROAD && GetRand(100) == 0) {
				this->enemys.emplace_back(j, i, player->speed, enemy, this->mapchips[ROAD].sizeX);
			}
		}
	}

	// ���@�̔z�u
	int randrect = GetRand((int)this->rects.size() - 1);
	this->player->panelX = randAtoB(this->rects[randrect]->room->sx, this->rects[randrect]->room->ex);
	this->player->panelY = randAtoB(this->rects[randrect]->room->sy, this->rects[randrect]->room->ey);
	
	// �~�j�}�b�v�𐶐�
	for(int i = 0; i < (int)this->minibody.size(); i++) {
		switch(this->body[i].type) {
		case WALL:
			this->minibody[i].type = MINI_WALL;
			break;
		case ROAD:
			this->minibody[i].type = MINI_ROAD;
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
			// �ǂȂ�#, ���Ȃ� ��`��
			if(j == this->player->panelX && i == this->player->panelY) printfDx("%s", "P");
			else if(this->body[calcIndex(j, i)].type == WALL) printfDx("%s", "#");
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

	// focusPanelX,Y �������������Ƃ������������ɂ��炷����
	if(this->focusPanelX % 2 == 0) scroll(-this->mapchips[ROAD].sizeX / 2, 0);
	if(this->focusPanelY % 2 == 0) scroll(0, -this->mapchips[ROAD].sizeY / 2);

	// ���ۂ̍��W�ɕ\��������}�b�v���X�N���[�����Ă��ĉ�ʓ��̒����Ƀv���C���[�������Ă��邵���
	scrollPanel(-this->player->panelX + this->focusPanelX / 2, -this->player->panelY + this->focusPanelY / 2);
}

void Map::moveEnemys() {
	Direction dir;
	for(auto &i : this->enemys) {
		do {
			dir = (Direction)randAtoB(0, DirectionNum - 1);
		} while(!canMove(i.panelX, i.panelY, dir));
		i.move(dir);
	}
}

void Map::moveAnimationEnemys() {
	for(auto &i : this->enemys) {
		i.moveAnimation();
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

	// �G�̕\��
	for(auto i : this->enemys) {
		DrawGraph(screenSX + i.panelX * this->mapchips[MINI_ROAD].sizeX, screenSY + i.panelY * this->mapchips[MINI_ROAD].sizeY, this->mapchips[MINI_ENEMY].handle, true);
	}

	// �v���C���[�̕\��
	DrawGraph(screenSX + this->player->panelX * this->mapchips[MINI_ROAD].sizeX, screenSY + this->player->panelY * this->mapchips[MINI_ROAD].sizeY, this->mapchips[MINI_PLAYER].handle, true);
}

void Map::DrawFocus() {
	DrawPt(this->cameraX, this->cameraY);

	// �G
	for(auto i : this->enemys) i.Draw();

	// �v���C���[
	this->player->Draw();
}

// �ċN�����Ń}�b�v�𕪊�������
void Map::mapSplitter(Rect* root) {
	if(root->ey - root->sy <= MINIMUM_RECT_SIZE * 2) root->splitvflag = false;
	if(root->ex - root->sx <= MINIMUM_RECT_SIZE * 2) root->splithflag = false;
	if(!(root->splitvflag) && !(root->splithflag)) return;

	Rect* child = new Rect(root->sx, root->sy, root->ex, root->ey);
	this->rects.emplace_back(child);

	if(root->splitvflag) {
		// �c����
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
		// ������ left | right
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

	// ������E, �ォ�牺�ւȂ��̂� i < this->sizeX - 2, j < this->sizeY - 2
	for(int i = 0; i < this->sizeX - 2; i++) {
		for(j = 0; j < this->sizeY - 2; j++) {
			// ��
			if(coordRectType[calcIndex(i, j)] != coordRectType[calcIndex(i + 1, j)] && GetRand(69) == 0)
				this->roomPairs.emplace_back(new RoomPair(true, coordRectType[calcIndex(i, j)], coordRectType[calcIndex(i + 1, j)]));
			// �c
			if(coordRectType[calcIndex(i, j)] != coordRectType[calcIndex(i, j + 1)] && GetRand(69) == 0)
				this->roomPairs.emplace_back(new RoomPair(false, coordRectType[calcIndex(i, j)], coordRectType[calcIndex(i, j + 1)]));
		}
	}
}

// �����𐶐�������
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

// �}�b�v�̕������Ǘ�����֐�
void Map::genRndMap() {
	for(int i = 0; i < (int)this->body.size(); i++) this->body[i].type = WALL;
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
	
	// ��抄�蓖��
	for(auto i : this->rects) {
		// �������蓖��
		for(j = i->room->sx; j <= i->room->ex; j++) {
			for(k = i->room->sy; k <= i->room->ey; k++) {
				this->body[calcIndex(j, k)].type = ROAD;
			}
		}
	}


	// ��������ʘH��L�΂�
	int rect0x, rect0y, rect1x, rect1y;
	for(auto i : this->roomPairs) {
		// x�����֐L�΂�
		if(i->isSide) {
			// ���R���1��ex�Ƌ��2��sx�͓�����
			if(i->pair.first->ex != i->pair.second->sx) exit(1);
			
			rect0x = i->pair.first->ex;
			rect0y = randAtoB(i->pair.first->room->sy + 1, i->pair.first->room->ey - 1);
			rect1x = i->pair.second->sx;
			rect1y = randAtoB(i->pair.second->room->sy + 1, i->pair.second->room->ey - 1);

			// ���̋��E A,B �����Ƃ����Ƃ�  A | B �� | �̕���
			makeLine(rect0x, rect0y, rect1x, rect1y);

			// ���A���狫�E�ւ̑�
			makeLine(i->pair.first->room->ex, rect0y, rect0x, rect0y);

			// ���B����̋��E�ւ̑�
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
	if(this->body[calcIndex(nextX, nextY)].type == WALL) return false;

	// �΂߈ړ��̎��͏�Ɖ����l����
	// ��ŏ�]�̎��ɂ��Ĉꔭ�ł��܂�
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

void Map::enemyMove(Enemy *enemy) {
	Direction dir;

	do {
		dir = (Direction)randAtoB(0, DirectionNum - 1);
	} while(!canMove(enemy->panelX, enemy->panelY, dir));
	
	enemy->move(dir);
}

bool Map::keyProcessing() {
	/* �L�[���͏��� */
	if(CheckHitKey(KEY_INPUT_SPACE)) this->player->speed = 60;
	else this->player->speed = 7;

	/* if(CheckHitKey(KEY_INPUT_Z)) {
			stage.attackPlayer();
		} else */
	if(CheckHitKey(KEY_INPUT_UP)) {
		if(CheckHitKey(KEY_INPUT_RIGHT)) {
			if(!canMove(this->player->panelX, this->player->panelY, RUP)) return false;
			this->player->move(RUP);
		} else if(CheckHitKey(KEY_INPUT_LEFT)) {
			if(!canMove(this->player->panelX, this->player->panelY, LUP)) return false;
			this->player->move(LUP);
		} else {
			if(!canMove(this->player->panelX, this->player->panelY, UP)) return false;
			this->player->move(UP);
		}
		return true;
	} else if(CheckHitKey(KEY_INPUT_DOWN)) {
		if(CheckHitKey(KEY_INPUT_RIGHT)) {
			if(!canMove(this->player->panelX, this->player->panelY, RDOWN)) return false;
			this->player->move(RDOWN);
		} else if(CheckHitKey(KEY_INPUT_LEFT)) {
			if(!canMove(this->player->panelX, this->player->panelY, LDOWN)) return false;
			this->player->move(LDOWN);
		} else {
			if(!canMove(this->player->panelX, this->player->panelY, DOWN)) return false;
			this->player->move(DOWN);
		}
		return true;
	} else if(CheckHitKey(KEY_INPUT_RIGHT)) {
		if(!canMove(this->player->panelX, this->player->panelY, RIGHT)) return false;
		this->player->move(RIGHT);
		return true;
	} else if(CheckHitKey(KEY_INPUT_LEFT)) {
		if(!canMove(this->player->panelX, this->player->panelY, LEFT)) return false;
		this->player->move(LEFT);
		return true;
	}

	return false;
}

void Map::attackPlayer() {
	if(this->moveFlag != DirectionNum || this->player->attackFlag) return;
	this->player->attackFlag = true;

	this->player->attack();

	// �G�̓��쏈��
	for(auto &i : this->enemys) enemyMove(&i);
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

/*
void Map::reflect() {
	// �v���C���[�̈ʒu�̍X�V
	if(this->moveFlag != DirectionNum) {
		this->cameraX -= directionDx(this->moveFlag) * this->player->speed;
		this->cameraY -= directionDy(this->moveFlag) * this->player->speed;
		this->movecnt++;
		if(this->movecnt >= this->mapchips[ROAD].sizeX / this->player->speed) {
			this->movecnt = 0;
			this->moveFlag = DirectionNum;
		}
	}

	// �v���C���[�̍U�����X�V
	if(this->player->attackFlag) {
		if(this->movecnt >= 0 && this->movecnt <= 9) {
			this->cameraX -= directionDx(this->player->front) * 10;
			this->cameraY -= directionDy(this->player->front) * 10;
		} else {
			this->cameraX += directionDx(this->player->front) * 10;
			this->cameraY += directionDy(this->player->front) * 10;
		}
		this->movecnt++;
		if(this->movecnt >= 20) {
			this->movecnt = 0;
			this->player->attackFlag = false;
		}
	}

	// �G�̈ʒu�̍X�V
	if(!this->player->attackFlag) {
		for(auto &i : enemys) {
			if(i.isMoving()) {
				// �G�̃A�j���[�V��������
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
	} else {
		for(auto &i : enemys) {
			if(this->movecnt >= 0 && this->movecnt <= 9) {
				i.x -= directionDx(this->player->front) * 10;
				i.y -= directionDy(this->player->front) * 10;
			} else {
				i.x += directionDx(this->player->front) * 10;
				i.y += directionDy(this->player->front) * 10;
			}
		}
	}

	if(this->moveFlag == DirectionNum && !this->player->attackFlag) {
		this->cameraX = -(this->player->panelX - this->focusPanelX / 2) * mapchips[ROAD].sizeX - mapchips[ROAD].sizeX / 2;
		this->cameraY = -(this->player->panelY - this->focusPanelY / 2) * mapchips[ROAD].sizeY - mapchips[ROAD].sizeY / 2;
	}
}*/