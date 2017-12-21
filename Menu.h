#ifndef MENU_H
#define MENU_H

#include "gamestate.h"
#include "Editor.h"

class CMenuState : public CGameState
{
public:
	void Init(CGameEngine* game);
	void Cleanup();

	void Pause();
	void Resume(CGameEngine* game);

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CMenuState* Instance() {
		return &m_MenuState;
	}

protected:
	CMenuState() { }

private:
	static CMenuState m_MenuState;

	enum Selection { sel_resume, sel_new, sel_load, sel_save, sel_quit };
    void UpdateArrowPosition();

	//sf::RenderWindow window;
	sf::View view;

	bool savingEnabled = false;
	bool waiting_for_input = false;

	std::string input_str = "maps/";
	Selection selection;

	int text_top = 500;
    int line_top = 730;

    sf::Texture logo_texture;
	sf::Sprite logo_sprite;

	sf::Font font;

	sf::Text text_sub;
	sf::Text text_resume;
	sf::Text text_new;
	sf::Text text_load;
	sf::Text text_save;
	sf::Text text_quit;
	sf::Text arrow;
	sf::Text text_field_label;
	sf::Text text_field;
	//sf::Vertex input_line[];

	sf::CircleShape circle;
};

#endif // MENU_H

