#include "DxLib.h"
#include "map.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(true);
	SetMainWindowText("Rogue-like");
	SetGraphMode(1200, 800, 32);
	if(DxLib_Init() == -1) return -1;

	Map stage(60, 46);
	stage.Draw();

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
		ClearDrawScreen();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}