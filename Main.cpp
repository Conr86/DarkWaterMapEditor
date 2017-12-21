#include <iostream>
#include <SFML/Graphics.hpp>

#include "GameEngine.h"
#include "Menu.h"

int main(int argc, char** argv)
{
    CGameEngine game;

    game.Init("DarkWater Map Editor", 1280, 720, false);
    game.ChangeState(CEditorState::Instance() );

	// main loop
	while ( game.Running() )
	{
		game.HandleEvents();
		game.Update();
		game.Draw();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}
