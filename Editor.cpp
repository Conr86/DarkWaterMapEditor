#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Editor.h"

#include "GameEngine.h"
#include "GameState.h"

CEditorState CEditorState::m_EditorState;

sf::View zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom, sf::View view)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel, view) };
	//sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel, view) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	return view;
}

void CEditorState::Init(CGameEngine* game)
{
    // main view
    view.setSize(1280, 720);

    ui_view = game->window.getDefaultView();
    //ui_view.setSize(1280, 720);


    printf("Editor initialised\n");

    //selector.setSize(sf::Vector2f(16, 16));
    //selector.setFillColor(sf::Color(90, 90, 128, 200));
    //selector.setOutlineThickness(0);

    font.loadFromFile("fonts/PressStart2P.ttf");
    hud.setFont(font);
    hud.setClock(game->clock);

    game->PushState(CMenuState::Instance());

    // create the tilemap from the level definition
    //tile_data = map.load("maps/new.map", "graphics/tileset.png", tileSize);
    //map.create(tile_data, "graphics/tileset.png", tileSize);

    selector.setTexture(map.m_tileset);
    selector.setColor(sf::Color(255,255,255,128));
}

void CEditorState::HandleEvents(CGameEngine* game)
{
    // HANDLE EVENTS
    sf::Event event;
    while (game->window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            game->Quit();
        else if (event.type == sf::Event::Resized)
        {
            //view = Util::getLetterboxView(view, event.size.width, event.size.height );
            //config.winX = event.size.width;
            //config.winY = event.size.height;
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                // Pause current state and load new menu state
                game->PushState(CMenuState::Instance());
                //game->PopState();
            else if (event.key.code == sf::Keyboard::G)
                // Toggle grid
                gridActive = !gridActive;
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mouse_world = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window), view);
            mouse_pos.first = floor(mouse_world.x / 16);
            mouse_pos.second = floor(mouse_world.y / 16);
            selector.setPosition(mouse_pos.first * 16, mouse_pos.second * 16);
        }
        else if (event.type == sf::Event::MouseWheelScrolled)
        {
            if (event.mouseWheelScroll.delta > 0)
                view = zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, game->window, (1.f / zoomAmount), view);
            else if (event.mouseWheelScroll.delta < 0)
                view = zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, game->window, zoomAmount, view);
        }
    }
}

void CEditorState::Update(CGameEngine* game)
{
    // movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        view.move(0, 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        view.move(-1, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        view.move(0, -1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        view.move(1, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        brush = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        brush = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        brush = 2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        brush = 3;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
        brush = 4;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
        brush = 5;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
        brush = 6;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
        brush = 7;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
        brush = 8;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
        brush = 9;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        // Check if we are within limits
        if (mouse_pos.first >= 0 && mouse_pos.second >= 0)
        {
            if (mouse_pos.first >= map.map_size.first)
            {
                // expand map
                map.map_size.first = mouse_pos.first + 1;
            }
            if (mouse_pos.second >= map.map_size.second)
            {
                // expand map
                map.map_size.second = mouse_pos.second + 1;
            }
            // Paint tile
            tile_data[mouse_pos] = brush;
            // Redraw map
            map.create(tile_data, "graphics/tileset.png", tileSize);
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        // Check if we are within limits
        if (mouse_pos.first >= 0 && mouse_pos.second >= 0)
        {
            // Check if tile exists
            if (tile_data.count(mouse_pos))
            {
                // Remove tile
                tile_data.erase(mouse_pos);
                // Resize map
                map.calcMapSize(tile_data);
                // Rredraw map
                map.create(tile_data, "graphics/tileset.png", tileSize);
            }
        }
    }
    selector.setTextureRect(sf::IntRect(brush % map.tileSetConstant * tileSize, brush / map.tileSetConstant * tileSize, tileSize, tileSize));
}

void CEditorState::Draw(CGameEngine* game)
{
    // clear the window with black color
    game->window.clear();

    // set view
    game->window.setView(view);

    // draw everything here...
    game->window.draw(map);

    if (gridActive)
    {
        for (int i = 0; i < map.map_size.first; i++){
            for (int j = 0; j < map.map_size.second; j++){
                sf::RectangleShape rectangle;
                rectangle.setSize(sf::Vector2f(16, 16));
                rectangle.setFillColor(sf::Color::Transparent);
                rectangle.setOutlineColor(sf::Color(255, 255, 255, 128));
                rectangle.setOutlineThickness(-1);
                rectangle.setPosition(i * 16, j *16);
                game->window.draw(rectangle);
            }
        }
    }
    game->window.draw(selector);

    // draw ui elements
    game->window.setView(ui_view);
    game->window.draw(hud);

    // end the current frame
    game->window.display();
}

void CEditorState::Cleanup(){
    // nothing to do.. maybe free some stuff?
}

void CEditorState::Pause(){}

void CEditorState::Resume(CGameEngine* game){
    printf("Editor resumed\n");
    if (game->action == game->action_new){
        // new map
        game->mapLoaded = true;
        std::cout << "Creating new map" << std::endl;
        tile_data = map.load("maps/blank.map", "graphics/tileset.png", tileSize);
        map.create(tile_data, "graphics/tileset.png", tileSize);
    }
    else if (game->action == game->action_save) {
        // save map
        std::cout << "Saving map" << std::endl;
        map.save(tile_data, game->map_str);
    }
    else if (game->action == game->action_load) {
        // load map
        game->mapLoaded = true;
        std::cout << "Loading map: <" << game->map_str << ">" << std::endl;

        tile_data = map.load(game->map_str, "graphics/tileset.png", tileSize);
        map.create(tile_data, "graphics/tileset.png", tileSize);
    }
    //else if (game->action == game->aciton_resume) {}
        // otherwise map already loaded
}
