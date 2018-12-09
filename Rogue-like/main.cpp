#include "DxLib.h"
#include "map.h"

constexpr int MAX_MAPFOCUS_X = 14;
constexpr int MAX_MAPFOCUS_Y = 8;
constexpr int MAPCHIP_SIZE = 100;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(true);
	SetMainWindowText("Rogue-like");
	SetGraphMode(1400, 800, 32);
	if(DxLib_Init() == -1) return -1;


	/* mapchip */
	/* TODO:àÍñáÇÃâÊëúÇï™äÑÇµÇƒì«Ç›çûÇﬂÇÈÇÊÇ§Ç…Ç∑ÇÈ */
	int wall = LoadGraph("dat\\wall.png");
	int road = LoadGraph("dat\\road.png");
	int miniwall = LoadGraph("dat\\miniwall.png");
	int miniroad = LoadGraph("dat\\miniroad.png");
	int miniplayer = LoadGraph("dat\\miniplayer.png");

	std::vector<Pic> mapchips;
	mapchips.emplace_back(wall, MAPCHIP_SIZE, MAPCHIP_SIZE);
	mapchips.emplace_back(road, MAPCHIP_SIZE, MAPCHIP_SIZE);
	mapchips.emplace_back(miniwall, 6, 6);
	mapchips.emplace_back(miniroad, 6, 6);
	mapchips.emplace_back(miniplayer, 6, 6);

	/* charactor */
	Pic player(LoadGraph("dat\\player.png"), 100, 100);

	Map stage(60, 46, mapchips, MAX_MAPFOCUS_X, MAX_MAPFOCUS_Y);
	// stage.Print();  /* debug */

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
		ClearDrawScreen();

		//if(cameraX >= 0) cameraX = 0;
		//if(cameraX + stage.sizeX * MAPCHIP_SIZE <= 1400) cameraX = 1400 - stage.sizeX * MAPCHIP_SIZE;
		//if(cameraY >= 0) cameraY = 0;
		//if(cameraY + stage.sizeY * MAPCHIP_SIZE <= 800) cameraY = 800 - stage.sizeY * MAPCHIP_SIZE;

		/* ÉLÅ[ì¸óÕèàóù */
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
		DrawGraph(1400 / 2 - player.sizeX / 2, 800 / 2 - player.sizeY / 2, player.handle, true);

		DrawFormatString(0, 0, GetColor(0, 0, 0), "playerX: %d, playerY: %d\n", stage.playerX, stage.playerY);

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}