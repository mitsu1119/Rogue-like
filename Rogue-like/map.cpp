#include "map.h"

Map::Map(int sizeX, int sizeY, std::vector<Pic> mapchips):sizeX(sizeX), sizeY(sizeY), mapchips(mapchips) {
	SRand(GetNowCount());
	this->body = std::vector<Panel>(sizeX*sizeY);

	// body Çé©ìÆê∂ê¨
	genRndMap();
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
			// ï«Ç»ÇÁ#, ìπÇ»ÇÁ Çï`âÊ
			printfDx("%s", (this->body[calcIndex(j, i)].type == WALL ? "#" : "."));
		}
		printfDx("\n");
	}
}

void Map::DrawPart(int screenSX, int screenSY, int panelSX, int panelSY, int panelEX, int panelEY) {
	int xsum = 0, ysum = 0;

	for(int i = panelSY; i <= panelEY; i++) {
		for(int j = panelSX; j <= panelEX; j++) {
			DrawGraph(screenSX + xsum, screenSY + ysum, this->mapchips[this->body[calcIndex(j, i)].type].handle, true);
			xsum += this->mapchips[this->body[calcIndex(j, i)].type].sizeX;
		}
		xsum = 0;
		ysum += this->mapchips[this->body[calcIndex(panelEX, i)].type].sizeY;
	}
}

void Map::DrawPt(int screenSX, int screenSY) {
	DrawPart(screenSX, screenSY, 0, 0, this->sizeX - 1, this->sizeY - 1);
}

// çƒãNèàóùÇ≈É}ÉbÉvÇï™äÑÇ∑ÇÈÇ‚Ç¬
void Map::mapSplitter(Rect* root) {
	if(root->ey - root->sy <= MINIMUM_RECT_SIZE * 2) root->splitvflag = false;
	if(root->ex - root->sx <= MINIMUM_RECT_SIZE * 2) root->splithflag = false;
	if(!(root->splitvflag) && !(root->splithflag)) return;

	Rect* child = new Rect(root->sx, root->sy, root->ex, root->ey);
	this->rects.emplace_back(child);

	if(root->splitvflag) {
		// ècï™äÑ
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
		// â°ï™äÑ left | right
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

	// ç∂Ç©ÇÁâE, è„Ç©ÇÁâ∫Ç÷Ç¬Ç»ÇÆÇÃÇ≈ i < this->sizeX - 2, j < this->sizeY - 2
	for(int i = 0; i < this->sizeX - 2; i++) {
		for(j = 0; j < this->sizeY - 2; j++) {
			// â°
			if(coordRectType[calcIndex(i, j)] != coordRectType[calcIndex(i + 1, j)] && GetRand(53) == 0)
				this->roomPairs.emplace_back(new RoomPair(true, coordRectType[calcIndex(i, j)], coordRectType[calcIndex(i + 1, j)]));
			// èc
			if(coordRectType[calcIndex(i, j)] != coordRectType[calcIndex(i, j + 1)] && GetRand(53) == 0)
				this->roomPairs.emplace_back(new RoomPair(false, coordRectType[calcIndex(i, j)], coordRectType[calcIndex(i, j + 1)]));
		}
	}
}

// ïîâÆÇê∂ê¨Ç∑ÇÈÇ‚Ç¬
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

// É}ÉbÉvÇÃï™äÑÇä«óùÇ∑ÇÈä÷êî
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
	
	// ãÊâÊäÑÇËìñÇƒ
	for(auto i : this->rects) {
		/*for(j = i->sx, k = i->sy; j <= i->ex; j++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->sx, k = i->ey; j <= i->ex; j++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->sx, k = i->sy; k <= i->ey; k++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->ex, k = i->sy; k <= i->ey; k++) this->body[calcIndex(j, k)].type = ROAD;*/

		// ïîâÆäÑÇËìñÇƒ
		for(j = i->room->sx; j <= i->room->ex; j++) {
			for(k = i->room->sy; k <= i->room->ey; k++) {
				this->body[calcIndex(j, k)].type = ROAD;
			}
		}
	}


	// ïîâÆÇ©ÇÁí òHÇêLÇŒÇ∑
	int rect0x, rect0y, rect1x, rect1y;
	for(auto i : this->roomPairs) {
		// xï˚å¸Ç÷êLÇŒÇ∑
		if(i->isSide) {
			// ìñëRãÊâÊ1ÇÃexÇ∆ãÊâÊ2ÇÃsxÇÕìôÇµÇ¢
			if(i->pair.first->ex != i->pair.second->sx) exit(1);
			
			rect0x = i->pair.first->ex;
			rect0y = randAtoB(i->pair.first->room->sy + 1, i->pair.first->room->ey - 1);
			rect1x = i->pair.second->sx;
			rect1y = randAtoB(i->pair.second->room->sy + 1, i->pair.second->room->ey - 1);

			// ãÊâÊÇÃã´äE A,B ÇãÊâÊÇ∆ÇµÇΩÇ∆Ç´  A | B ÇÃ | ÇÃïîï™
			makeLine(rect0x, rect0y, rect1x, rect1y);

			// ãÊâÊAÇ©ÇÁã´äEÇ÷ÇÃë´
			makeLine(i->pair.first->room->ex, rect0y, rect0x, rect0y);

			// ãÊâÊBÇ©ÇÁÇÃã´äEÇ÷ÇÃë´
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