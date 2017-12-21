#include "gameengine.h"
#include "gamestate.h"

void CGameEngine::Init(const char* title, int width, int height, bool fullscreen)
{

	if ( fullscreen ) {
		//flags = SDL_FULLSCREEN;
	}
	window.create(sf::VideoMode(width, height), title);
	window.setFramerateLimit(60);

	m_fullscreen = fullscreen;
	m_running = true;

    clock.setSampleDepth(100);

	printf("CGameEngine Init\n");
}

void CGameEngine::Cleanup()
{
	// cleanup the all states
	while ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// switch back to windowed mode so other
	// programs won't get accidentally resized
	if ( m_fullscreen ) {
		//screen = SDL_SetVideoMode(640, 480, 0, 0);
	}

	printf("CGameEngine Cleanup\n");

	// shutdown SDL
	//SDL_Quit();
}

void CGameEngine::ChangeState(CGameState* state)
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void CGameEngine::PushState(CGameState* state)
{
	// pause current state
	if ( !states.empty() ) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void CGameEngine::PopState()
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->Resume(this);
	}
}


void CGameEngine::HandleEvents()
{
	// let the state handle events
	states.back()->HandleEvents(this);
}

void CGameEngine::Update()
{
    clock.beginFrame();
	// let the state update the game
	states.back()->Update(this);
}

void CGameEngine::Draw()
{
	// let the state draw the screen
	states.back()->Draw(this);
	clock.endFrame();
}

