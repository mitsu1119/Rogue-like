#include "DxLib.h"
#include "map.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(true);
	SetMainWindowText("Rogue-like");
	SetGraphMode(1200, 800, 32);
	if(DxLib_Init() == -1) return -1;

	Map stage(40, 40);
	stage.Draw();

	SetDrawScreen(DX_SCREEN_BACK);
	while(ProcessMessage() == 0) {
		ClearDrawScreen();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}