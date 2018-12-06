#include "DxLib.h"
#include "map.h"

constexpr int MAX_MAPPART_X = 14;
constexpr int MAX_MAPPART_Y = 8;
constexpr int MAPCHIP_SIZE = 100;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(true);
	SetMainWindowText("Rogue-like");
	SetGraphMode(1400, 800, 32);
	if(DxLib_Init() == -1) return -1;

	int wall = LoadGraph("dat\\wall.png");
	int road = LoadGraph("dat\\road.png");
	int miniwall = LoadGraph("dat\\miniwall.png");
	int miniroad = LoadGraph("dat\\miniroad.png");

	std::vector<Pic> mapchips;
	mapchips.emplace_back(wall, MAPCHIP_SIZE, MAPCHIP_SIZE);
	mapchips.emplace_back(road, MAPCHIP_SIZE, MAPCHIP_SIZE);

	std::vector<Pic> minimapchips;
	minimapchips.emplace_back(miniwall, 6, 6);
	minimapchips.emplace_back(miniroad, 6, 6);

	Map stage(60, 46, mapchips);
	stage.Print();

	SetDrawScreen(DX_SCREEN_BACK);
	int cameraX = 0, cameraY = 0;
	while(ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
		ClearDrawScreen();

		if(!CheckHitKey(KEY_INPUT_RIGHT)) cameraX += 10;
		if(!CheckHitKey(KEY_INPUT_LEFT)) cameraX -= 10;
		if(!CheckHitKey(KEY_INPUT_UP)) cameraY -= 10;
		if(!CheckHitKey(KEY_INPUT_DOWN)) cameraY += 10;

		if(cameraX >= 0) cameraX = 0;
		if(cameraX + stage.sizeX * MAPCHIP_SIZE <= 1400) cameraX = 1400 - stage.sizeX * MAPCHIP_SIZE;
		if(cameraY  >= 0) cameraY = 0;
		if(cameraY + stage.sizeY * MAPCHIP_SIZE <= 800) cameraY = 800 - stage.sizeY * MAPCHIP_SIZE;

		stage.DrawPt(cameraX, cameraY);
		// ミニマップの表示
		stage.DrawPtMapchips(6, 6, &minimapchips);

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}