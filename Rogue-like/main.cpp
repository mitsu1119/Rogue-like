#include "DxLib.h"
#include "map.h"
#include "player.h"

constexpr int MAX_MAPFOCUS_X = 14;
constexpr int MAX_MAPFOCUS_Y = 8;
constexpr int MAPCHIP_SIZE = 100;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(true);
	SetMainWindowText("Rogue-like");
	SetGraphMode(1400, 800, 32);
	if(DxLib_Init() == -1) return -1;


	/* mapchip */
	/* TODO:一枚の画像を分割して読み込めるようにする */
	int wall = LoadGraph("dat\\wall.png");
	int road = LoadGraph("dat\\road.png");
	int miniwall = LoadGraph("dat\\miniwall.png");
	int miniroad = LoadGraph("dat\\miniroad.png");
	int miniplayer = LoadGraph("dat\\miniplayer.png");
	int minienemy = LoadGraph("dat\\minienemy.png");

	std::vector<Pic> mapchips;
	mapchips.emplace_back(wall, MAPCHIP_SIZE, MAPCHIP_SIZE);
	mapchips.emplace_back(road, MAPCHIP_SIZE, MAPCHIP_SIZE);
	mapchips.emplace_back(miniwall, 6, 6);
	mapchips.emplace_back(miniroad, 6, 6);
	mapchips.emplace_back(miniplayer, 6, 6);
	mapchips.emplace_back(minienemy, 6, 6);

	/* charactor */
	Pic player(LoadGraph("dat\\player.png"), 100, 100);
	Pic enemy(LoadGraph("dat\\enemy.png"), 100, 100);
	Player pp(7, player);
	Enemy ee(0, 0, 7, enemy);

	Map stage(60, 46, mapchips, MAX_MAPFOCUS_X, MAX_MAPFOCUS_Y, &pp, &ee);

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
		ClearDrawScreen();

		/* キー入力処理 */
		if(CheckHitKey(KEY_INPUT_SPACE)) pp.speed = 60;
		else pp.speed = 7;

		if(CheckHitKey(KEY_INPUT_UP)) {
			if(CheckHitKey(KEY_INPUT_RIGHT)) stage.movePlayer(RUP);
			else if(CheckHitKey(KEY_INPUT_LEFT)) stage.movePlayer(LUP);
			else stage.movePlayer(UP);
		} else if(CheckHitKey(KEY_INPUT_DOWN)) {
			if(CheckHitKey(KEY_INPUT_RIGHT)) stage.movePlayer(RDOWN);
			else if(CheckHitKey(KEY_INPUT_LEFT)) stage.movePlayer(LDOWN);
			else stage.movePlayer(DOWN);
		} else if(CheckHitKey(KEY_INPUT_RIGHT)) stage.movePlayer(RIGHT);
		else if(CheckHitKey(KEY_INPUT_LEFT)) stage.movePlayer(LEFT);

		stage.reflect();

		stage.DrawFocus();
		stage.DrawMinimap(6, 6);

		DrawFormatString(0, 0, GetColor(0, 0, 0), "playerX: %d, playerY: %d\n", stage.playerX, stage.playerY);

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}