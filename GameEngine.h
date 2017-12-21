#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "FrameClock.h"

class CGameState;

class CGameEngine
{
public:

	void Init(const char* title, int width=640, int height=480, bool fullscreen=false);
	void Cleanup();

	void ChangeState(CGameState* state);
	void PushState(CGameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }

	sf::RenderWindow window;

	sfx::FrameClock clock;

	// the current map (can be set by the menu and then loaded by editor)
	//std::string currentMap;
	// if a map is loaded
	bool mapLoaded = false;
	// create a new map on resuming editor?
	//bool loadNewMap = false;
	// save map on resuming editor?
	//bool saveMap = false;
	//std::string saveAs;

	enum Action { action_new, action_load, action_save, action_none };
	Action action = action_none;

	std::string map_str;

private:
	// the stack of states
	std::vector<CGameState*> states;

	bool m_running;
	bool m_fullscreen;
};

#endif

