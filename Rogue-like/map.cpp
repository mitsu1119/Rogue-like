#include "map.h"

Map::Map(int sizeX, int sizeY):sizeX(sizeX), sizeY(sizeY) {
	this->body = std::vector<Panel>(sizeX*sizeY);

	// body を自動生成
	genRndMap();
}

void Map::Draw() {
	for(int i = 0; i < this->sizeY; i++) {
		for(int j = 0; j < this->sizeX; j++) {
			// 壁なら#, 道なら を描画
			printfDx("%s", (this->body[calcIndex(j, i)].type == WALL ? "#" : " "));
		}
		printfDx("\n");
	}
}

// 再起処理でマップを分割するやつ
void Map::mapSplitter(Rect root) {
	if((root.ey - root.sy <= MINIMUM_RECT_SIZE * 2) || (root.ex - root.sx <= MINIMUM_RECT_SIZE * 2)) {
		// 充分にマップが分割できたとき再帰を終了する
		return;
	}

	Rect child(root.sx, root.sy, root.ex, root.ey);
	this->rects.push_back(child);

	// とりあえず横分割 left | right
	int splitCoord = GetRand((root.ex - MINIMUM_RECT_SIZE) - (root.sx + MINIMUM_RECT_SIZE)) + (root.sx + MINIMUM_RECT_SIZE);
	printfDx("splitCoord: %d\n", splitCoord);
	root.ex = splitCoord;
	child.sx = splitCoord;
	mapSplitter(root);
	mapSplitter(child);
	return;
}

// マップの分割を管理する関数
void Map::genRndMap() {
	for(int i = 0; i < (int)this->body.size(); i++) this->body[i].type = WALL;
	this->rects.emplace_back(0, 0, this->sizeX - 1, this->sizeY - 1);

	mapSplitter(this->rects[0]);
	int j, k;
	for(auto i : this->rects) {
		printfDx("sx: %d, ex: %d ... sy: %d, ey: %d\n", i.sx, i.ex, i.sy, i.ey);
		for(j = i.sx, k = i.sy; j <= i.ex; j++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i.sx, k = i.ey; j <= i.ex; j++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i.sx, k = i.sy; k <= i.ey; k++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i.ex, k = i.sy; k <= i.ey; k++) this->body[calcIndex(j, k)].type = ROAD;
	}
}