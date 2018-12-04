#include "map.h"

Map::Map(int sizeX, int sizeY):sizeX(sizeX), sizeY(sizeY) {
	SRand(GetNowCount());
	this->body = std::vector<Panel>(sizeX*sizeY);

	// body ����������
	genRndMap();
}

Map::~Map() {
	for(auto i : this->rects) delete i;
	for(auto i : this->rooms) delete i;
	for(auto i : this->roomPairs) delete i;
}

void Map::Draw() {
	for(int i = 0; i < this->sizeY; i++) {
		for(int j = 0; j < this->sizeX; j++) {
			// �ǂȂ�#, ���Ȃ� ��`��
			printfDx("%s", (this->body[calcIndex(j, i)].type == WALL ? "#" : "."));
		}
		printfDx("\n");
	}
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

// �����𐶐�������
void Map::genRooms() {
	int x, y, w, h;
	for(auto i : this->rects) {
		w = randAtoB(MINIMUM_ROOM_SIZE, i->ex - i->sx - MINIMUM_ROOM_SIZE);
		h = randAtoB(MINIMUM_ROOM_SIZE, i->ey - i->sy - MINIMUM_ROOM_SIZE);
		x = randAtoB(i->sx + MINIMUM_ROOM_SIZE / 2, i->ex - MINIMUM_ROOM_SIZE / 2 - w);
		y = randAtoB(i->sy + MINIMUM_ROOM_SIZE / 2, i->ey - MINIMUM_ROOM_SIZE / 2 - h);
		this->rooms.emplace_back(new Room(x, y, x + w, y + h));
	}
}

// �}�b�v�̕������Ǘ�����֐�
void Map::genRndMap() {
	for(int i = 0; i < (int)this->body.size(); i++) this->body[i].type = WALL;
	Rect* root = new Rect(0, 0, this->sizeX - 1, this->sizeY - 1);
	this->rects.emplace_back(root);

	mapSplitter(root);
	genRooms();
	reflectRects();
}

void Map::reflectRects() {
	int j, k;
	
	// ��抄�蓖��
	for(auto i : this->rects) {
		for(j = i->sx, k = i->sy; j <= i->ex; j++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->sx, k = i->ey; j <= i->ex; j++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->sx, k = i->sy; k <= i->ey; k++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->ex, k = i->sy; k <= i->ey; k++) this->body[calcIndex(j, k)].type = ROAD;
	}

	// �������蓖��
	for(auto i : this->rooms) {
		for(j = i->sx; j <= i->ex; j++) {
			for(k = i->sy; k <= i->ey; k++) {
				this->body[calcIndex(j, k)].type = ROAD;
			}
		}
	}
}