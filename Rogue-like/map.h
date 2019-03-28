#pragma once
#include <vector>
#include <algorithm>
#include "DxLib.h"
#include "useful.h"
#include "player.h"

// �p�l���̌^���̃��X�g
// �}�b�v�`�b�v�����̏��ō��
// WALL_SINGLE_JOINTLEFT_DOWN	������WALL_LEFT��SINGLE���Ȃ�����
enum PanelType {
	WALL_UP, WALL_RUP, WALL_RIGHT, WALL_RDOWN, WALL_DOWN, WALL_LDOWN, WALL_LEFT, WALL_LUP, WALL, WALL_INRUP, WALL_INRDOWN, WALL_INLDOWN, WALL_INLUP,
	WALL_SINGLERUP, WALL_SINGLERIGHT, WALL_SINGLERDOWN, WALL_SINGLEDOWN, WALL_SINGLELDOWN, WALL_SINGLELEFT, WALL_SINGLELUP, WALL_SINGLE,
	WALL_SINGLE_JOINTDOWN, WALL_SINGLE_JOINTRIGHT_UP, WALL_SINGLE_JOINTLEFT_UP, WALL_SINGLE_JOINTRIGHT_DOWN, WALL_SINGLE_JOINTLEFT_DOWN,
	ROAD, STAIRS, MINI_WALL, MINI_ROAD, MINI_STAIRS, MINI_PLAYER, MINI_ENEMY, TypeNum
};

class Trap {
public:
	Trap(int panelX, int panelY, PanelType type):panelX(panelX), panelY(panelY), type(type) {}

	int panelX, panelY;
	PanelType type;
};

// �p�l���ꖇ�ꖇ
class Panel {
public:
	Panel() {
	}
	Panel(PanelType type):type(type) {
	}
	PanelType type;
};

// ����
class Room {
public:
	Room(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey) {
	}
	
	Room() {
	}

	int sx, sy, ex, ey;
};

// ���̃N���X
class Rect {
public:
	Rect(int sx, int sy, int ex, int ey):sx(sx), sy(sy), ex(ex), ey(ey),splitvflag(true),splithflag(true) {
	}

	int sx, sy, ex, ey;
	bool splitvflag, splithflag;
	
	Room* room;
};

constexpr int MINIMUM_ROOM_SIZE = 4;
constexpr int MINIMUM_RECT_SIZE = MINIMUM_ROOM_SIZE * 2;

// �������m�̃y�A
class RoomPair {
public:
	RoomPair(bool isSide, Rect* rect1, Rect* rect2):isSide(isSide) {
		this->pair = std::make_pair(rect1, rect2);
	}

	std::pair<Rect*, Rect*> pair;
	bool isSide;	// �Ȃ���̕���(�����c��)
};

// �}�b�v�{��
class Map {
private:
	std::vector<Panel> body;
	std::vector<Panel> body2;	// ���C�������A�v���C���[��G�l�~�[�̏�ɔz�u�����B�����蔻��͂Ȃ�
	std::vector<Panel> body3;	// ����ɏ�
	std::vector<Panel> minibody;
	std::vector<bool> movable;

	std::vector<Rect* > rects;
	std::vector<RoomPair* > roomPairs;

	// �}�b�v��������������
	void mapSplitter(Rect* root);
	void makePairAdd();	// ��{���ɂȂ肪���������̂ł���Ƀy�A�����
	void genRooms();
	void genRndMap();
	void makeLine(int sx, int sy, int ex, int ey);

	// �����╔�����蓖�Ă𔽉f����֐�
	void reflectRects();

	// �}�b�v�`�b�v
	std::vector<Pic> mapchips;

	// (screenSX, screenSY) ���� ([panelSX,panelEX],[panelSY,panelEY]) ���v�Z���ĕ`��
	void DrawPart(int screenSX, int screenSY, int panelSX, int panelSY, int panelEX, int panelEY, std::vector<Panel> &map);

	// �}�b�v��`�悷��Ƃ��A�}�b�v���ǂ̂��炢���炷���w�肷��ϐ�
	int cameraX, cameraY;
	int focusPanelX, focusPanelY;

	// (screenSX, screenSY) ����}�b�v��S���`��(�܂�}�b�v��(0, 0)����`�悵�����̂𕽍s�ړ������悤�Ȋ���)
	void DrawPt(int screenSX, int screenSY);
	void DrawPt2(int screenSX, int screenSY);
	void DrawPt3(int screenSX, int screenSY);

	// �v���C���[�ƃ}�b�v�̊m�F�̂�
	void Print();

	// (panelX, panelY)����direction�̕����֓�����Ƃ�true, �����Ȃ��Ƃ�false
	bool canMove(int panelX, int panelY, Direction direction);

	Player *player;
	std::vector<Enemy> enemys;
	bool nextMapFlag;
	std::vector<Trap> traps;

	// (panelX, panelY) �ɂ���G�̃C���f�b�N�X��Ԃ�
	int searchEnemy(int panelX, int panelY);

public:
	Map(int sizeX, int sizeY, std::vector<Pic> mapchips, int focusPanelX, int focusPanelY, Player *player);
	~Map();

	// �}�b�v�̃T�C�Y(�^�C���̖���)
	int sizeX, sizeY;

	int calcIndex(int x, int y) {
		return this->sizeX*y + x;
	}

	void beMovable(int panelX, int panelY) {
		this->movable[calcIndex(panelX, panelY)] = true;
	}

	void dontbeMovable(int panelX, int panelY) {
		this->movable[calcIndex(panelX, panelY)] = false;
	}

	bool canThisGetNextMap() {
		return this->nextMapFlag;
	}

	// �L�[�������ꂽ: true
	bool keyProcessing();
	void trapProcessing();

	// �����W�ŃX�N���[��
	void scroll(int x, int y);
	// �p�l���̐��ŃX�N���[��
	void scrollPanel(int panelX, int panelY);

	// �v���C���[��}�b�v��\������ʒu�̒����Ȃ�
	void revice();

	void moveEnemys();
	bool moveAnimationEnemys();
	void dieEnemy(int index);

	void DrawMinimap(int screenSX, int screenSY);
	void DrawFocus();

	void nextMap();
}; 