#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameEngine.h"
#include "GameState.h"

#include <SFML/OpenGL.hpp>

CMenuState CMenuState::m_MenuState;

void CMenuState::Init(CGameEngine* game)
{
    // main view
    view.setSize(1280, 720);

    printf("Menu initialised\n");

    // LOGO
    if (!logo_texture.loadFromFile("graphics/logo_t.png"))
    {
        // error...
    }
    logo_sprite.setTexture(logo_texture);
    logo_sprite.setOrigin(logo_sprite.getLocalBounds().width / 2, logo_sprite.getLocalBounds().height / 2);
    logo_sprite.setPosition(view.getCenter().x, view.getCenter().y /2);

    // FONT
    if (!font.loadFromFile("fonts/PressStart2P.ttf"))
    {
        // error...
        std::cout << "error font?" << std::endl;
    }

    // SUBTITLE
    text_sub.setFont(font);
    text_sub.setString("Map Editor");
    text_sub.setPosition(view.getCenter().x - text_sub.getLocalBounds().width / 2, view.getCenter().y / 2 + logo_sprite.getLocalBounds().height / 2 + 15);
    // RESUME
    text_resume.setFont(font);
    text_resume.setString("Resume");
    text_resume.setPosition(0, text_top);
    if (game->mapLoaded == false) text_resume.setColor(sf::Color(90, 90 ,90));
    // NEW
    text_new.setFont(font);
    text_new.setString("New Map");
    text_new.setPosition(0, text_top + 50);
    // LOAD
    text_load.setFont(font);
    text_load.setString("Load Map");
    text_load.setPosition(0, text_top + 100);
    // SAVE
    text_save.setFont(font);
    text_save.setString("Save Map");
    text_save.setPosition(0, text_top + 150);
    if (game->mapLoaded == false) text_save.setColor(sf::Color(90, 90 ,90));
    // QUIT TEXT
    text_quit.setFont(font);
    text_quit.setString("Quit");
    text_quit.setPosition(0, text_top + 200);
    // SELECTION ARROW
    arrow.setFont(font);
    arrow.setString(">");
    selection = sel_resume;
    arrow.setPosition(0 - 40, text_top);

    // TEXT FIELD LABEL
    text_field_label.setFont(font);
    text_field_label.setString("File name: ");
    text_field_label.setPosition(-100, line_top);
    // TEXT FIELD
    text_field.setFont(font);
    text_field.setString("maps/");
    text_field.setPosition(text_field_label.getGlobalBounds().left + text_field_label.getGlobalBounds().width, line_top);

    // UNDERLINE FOR FIELD
    /*sf::Vertex input_line[] =
    {
        sf::Vertex(sf::Vector2f(text_field.getGlobalBounds().left, line_top + 30)),
        sf::Vertex(sf::Vector2f(text_field.getGlobalBounds().left + 700, line_top + 30))
    };*/
}

void CMenuState::UpdateArrowPosition()
{
    switch(selection) {
        case sel_resume :
            arrow.setPosition(-40, text_top);
            break;
        case sel_new :
            arrow.setPosition(-40, text_top + 50);
            break;
        case sel_load :
            arrow.setPosition(-40, text_top + 100);
            break;
        case sel_save :
            arrow.setPosition(-40, text_top + 150);
            break;
        case sel_quit :
            arrow.setPosition(-40, text_top + 200);
            break;
        default :
            std::cout << "Error in seleciton" << std::endl;
    }
}

void CMenuState::HandleEvents(CGameEngine* game)
{
    // HANDLE EVENTS
    sf::Event event;

    while (game->window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            game->Quit();

        if (waiting_for_input)
        {
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == 8){
                    if (input_str.size() > 5) // keep '/maps/' part
                        input_str.pop_back();
                }
                else if (event.text.unicode == 13){}
                // Handle ASCII characters only
                else if (event.text.unicode < 128)
                {
                    input_str += static_cast<char>(event.text.unicode);
                }
                text_field.setString(input_str);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Return) {
                    waiting_for_input = false;
                    // easy way to find out if we are saving or loading
                    if (selection == sel_load) {
                        // load input_str
                        game->action = game->action_load;
                        game->map_str = input_str;
                        game->PopState();
                    }
                    else if (selection == sel_save)
                    {
                        // we can't save the map directly from here
                        // so we will tell the editor to save it when we resume the editor
                        game->action = game->action_save;
                        // file to save it as
                        game->map_str = input_str;
                        game->PopState();
                    }
                }
            }
        }
        else {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (game->mapLoaded == true)
                        game->action = game->action_none;
                        game->PopState();
                }
                else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
                {
                    switch(selection) {
                        case sel_resume :
                            break;
                        case sel_new :
                            selection = sel_resume;
                            break;
                        case sel_load :
                            selection = sel_new;
                            break;
                        case sel_save :
                            selection = sel_load;
                            break;
                        case sel_quit :
                            selection = sel_save;
                            break;
                        default :
                            std::cout << "Error in seleciton" << std::endl;
                    }
                    UpdateArrowPosition();
                }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                {
                    switch(selection) {
                        case sel_resume :
                            selection = sel_new;
                            break;
                        case sel_new :
                            selection = sel_load;
                            break;
                        case sel_load :
                            selection = sel_save;
                            break;
                        case sel_save :
                            selection = sel_quit;
                            break;
                        case sel_quit :
                            break;
                        default :
                            std::cout << "Error in seleciton" << std::endl;
                    }
                    UpdateArrowPosition();
                }
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
                {
                    switch(selection) {
                        case sel_resume :
                            if (game->mapLoaded == true)
                            {
                                game->action = game->action_none;
                                game->PopState();
                            }
                            break;
                        case sel_new :
                            game->action = game->action_new;
                            game->map_str = "";
                            game->PopState();
                            break;
                        case sel_load :
                            waiting_for_input = true;
                            break;
                        case sel_save :
                            if (game->mapLoaded == true)
                                waiting_for_input = true;
                            break;
                        case sel_quit :
                            game->Quit();
                            break;
                        default :
                            game->Quit();
                    }
                }
            }
        }
        if (event.type == sf::Event::Resized)
        {
            //view = Util::getLetterboxView(view, event.size.width, event.size.height );
            //config.winX = event.size.width;
            //config.winY = event.size.height;
        }
    }
}
void CMenuState::Update(CGameEngine* game)
{

}

void CMenuState::Draw(CGameEngine* game)
{
    // clear the window with color
    game->window.clear(sf::Color(0,50,0));

    // set view
    game->window.setView(view);

    // draw everything here...
    game->window.draw(logo_sprite);

    if (!waiting_for_input)
    {
        game->window.draw(text_sub);
        game->window.draw(text_new);
        game->window.draw(text_load);
        game->window.draw(text_save);
        game->window.draw(text_quit);
        game->window.draw(arrow);
        game->window.draw(text_resume);
    } else {
        game->window.draw(text_field);
        game->window.draw(text_field_label);
        //game->window.draw(input_line, 2, sf::Lines);
    }

    // end the current frame
    game->window.display();
}

void CMenuState::Cleanup(){
    // nothing to do.. maybe free some stuff?
    text_sub.setString("");
    text_resume.setString("");
    text_new.setString("");
    text_load.setString("");
    text_save.setString("");
    text_quit.setString("");
    arrow.setString("");
    text_field.setString("");
    text_field_label.setString("");

    text_resume.setColor(sf::Color(255, 255, 255));
    text_save.setColor(sf::Color(255, 255, 255));

    printf("Menu closed\n");
}

void CMenuState::Pause(){}
void CMenuState::Resume(CGameEngine* game){}


