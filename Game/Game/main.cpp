﻿#include "Game.h"
#include "TitleScene.h"

int main(int argc, char** argv)
{
	if (!GAMEINSTANCE.Initialize(1280,768,false))
	{
		return -1;
	}
	GAMEINSTANCE.setFirstScene(new TitleScene);
	GAMEINSTANCE.GetRenderer()->SetWindowTitle("Game");
	GAMEINSTANCE.Run();
	GAMEINSTANCE.Shutdown();
	return 0;
}