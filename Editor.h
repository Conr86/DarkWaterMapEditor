#ifndef EDITOR_H
#define EDITOR_H

#include "gamestate.h"
#include "TileMap.h"
#include "Menu.h"
#include "ClockHud.h"


class CEditorState : public CGameState
{
public:
	void Init(CGameEngine* game);
	void Cleanup();

	void Pause();
	void Resume(CGameEngine* game);

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CEditorState* Instance() {
		return &m_EditorState;
	}

protected:
	CEditorState() { }

private:
	static CEditorState m_EditorState;

	//sf::RenderWindow window;
	sf::View view;
	sf::View ui_view;
    int tileSize = 16;

    ClockHUD hud;
	sf::Font font;

    // Settings
    int brush = 2;
    const float zoomAmount{ 1.1f };
    bool gridActive = true;

    std::string activeMap;

    //sf::RectangleShape selector;
    sf::Sprite selector;

    std::pair<float,float> mouse_pos;

    TileMap map;
    std::map<std::pair<int,int>,int> tile_data;
};

#endif // EDITOR_H
