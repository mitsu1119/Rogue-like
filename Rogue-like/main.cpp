#include "DxLib.h"
#include "game.h"

constexpr int MAX_MAPFOCUS_X = 14;
constexpr int MAX_MAPFOCUS_Y = 8;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(true);
	SetMainWindowText("Rogue-like");
	SetGraphMode(1400, 800, 32);
	if(DxLib_Init() == -1) return -1;

	Game game(MAX_MAPFOCUS_X, MAX_MAPFOCUS_Y);

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
		ClearDrawScreen();

		game.reflect();
		ScreenFlip();
	}

	DxLib_End();
	return 0;
}