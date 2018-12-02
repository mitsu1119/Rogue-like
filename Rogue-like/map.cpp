#include "map.h"

Map::Map(int sizeX, int sizeY):sizeX(sizeX), sizeY(sizeY) {
	this->body = std::vector<Panel>(sizeX*sizeY);

	// body ����������
	genRndMap();
}

Map::~Map() {
	for(auto i : this->rects) delete i;
}

void Map::Draw() {
	for(int i = 0; i < this->sizeY; i++) {
		for(int j = 0; j < this->sizeX; j++) {
			// �ǂȂ�#, ���Ȃ� ��`��
			printfDx("%s", (this->body[calcIndex(j, i)].type == WALL ? "#" : " "));
		}
		printfDx("\n");
	}
}

// �ċN�����Ń}�b�v�𕪊�������
void Map::mapSplitter(Rect* root) {
	if((root->ey - root->sy <= MINIMUM_RECT_SIZE * 2) || (root->ex - root->sx <= MINIMUM_RECT_SIZE * 2)) {
		// �[���Ƀ}�b�v�𕪊��ł����Ƃ��ċN���I������
		return;
	}

	Rect* child = new Rect(root->sx, root->sy, root->ex, root->ey);
	this->rects.emplace_back(child);

	// �c�����������������߂�B�c:�� = 1:1
	//int splitType = GetRand(99) + 1;

	//if(splitType <= 50) {
		// �c����
		int splitCoord = GetRand((root->ey - MINIMUM_RECT_SIZE) - (root->sy + MINIMUM_RECT_SIZE)) + (root->sy + MINIMUM_RECT_SIZE);
		printfDx("splitCoord: %d,%d\n", (root->ey - MINIMUM_RECT_SIZE) - (root->sy + MINIMUM_RECT_SIZE),splitCoord);
		root->ey = splitCoord;
		child->sy = splitCoord;
/*	}  else {
		// ������ left | right
		int splitCoord = GetRand((root.ex - MINIMUM_RECT_SIZE) - (root.sx + MINIMUM_RECT_SIZE)) + (root.sx + MINIMUM_RECT_SIZE);
		printfDx("splitCoord: %d\n", splitCoord);
		this->rects[root.num].ex = splitCoord;
		child.sx = splitCoord; 
	}*/


	mapSplitter(root);
	mapSplitter(child);
	return;
}

// �}�b�v�̕������Ǘ�����֐�
void Map::genRndMap() {
	for(int i = 0; i < (int)this->body.size(); i++) this->body[i].type = WALL;
	Rect* root = new Rect(0, 0, this->sizeX - 1, this->sizeY - 1);
	this->rects.emplace_back(root);

	mapSplitter(root);
	int j, k;
	for(auto i : this->rects) {
		for(j = i->sx, k = i->sy; j <= i->ex; j++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->sx, k = i->ey; j <= i->ex; j++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->sx, k = i->sy; k <= i->ey; k++) this->body[calcIndex(j, k)].type = ROAD;
		for(j = i->ex, k = i->sy; k <= i->ey; k++) this->body[calcIndex(j, k)].type = ROAD;
	}
}