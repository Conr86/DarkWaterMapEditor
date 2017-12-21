#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "TileMap.h"

std::vector<std::string> splitString(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
sf::Vector2i splitStringToInt(const std::string& s, char delimiter)
{
   std::vector<int> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(std::atoi(token.c_str()));
   }
   sf::Vector2i pos;
   pos.x = tokens[0];
   pos.y = tokens[1];
   return pos;
}
std::pair<int,int> splitStringToPair(const std::string& s, char delimiter)
{
   std::vector<int> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(std::atoi(token.c_str()));
   }
   std::pair<int,int> pos;
   pos.first = tokens[0];
   pos.second = tokens[1];
   return pos;
}

void TileMap::calcMapSize(std::map<std::pair<int,int>,int> tiles)
{
    std::pair<int,int> new_size;

    int max_x = 0;
    int max_y = 0;

    std::map<std::pair<int,int>, int>::iterator it;
    for(it = tiles.begin(); it != tiles.end(); it++)
    {
        sf::Vector2i pos = sf::Vector2i(it->first.first, it->first.second);

        if (pos.x > max_x)
            max_x = pos.x;
        if (pos.y > max_y)
            max_y = pos.y;
    }
    new_size = {max_x + 1, max_y + 1};
    map_size = new_size;
}
bool TileMap::save(std::map<std::pair<int,int>,int> tiles, std::string fileName)
{
    std::string data;

    std::map<std::pair<int,int>, int>::iterator it;
    for(it = tiles.begin(); it != tiles.end(); it++)
    {
        sf::Vector2i pos = sf::Vector2i(it->first.first, it->first.second);
        int type = it->second;

        std::stringstream ss;
        ss << pos.x << "," << pos.y << ":" << type << "-" << std::endl;

        data.append(ss.str());
    }

    std::ofstream map_stream(fileName);
    if (map_stream.is_open())
    {
        map_stream << data;
        map_stream.close();
        std::cout << "Saved file" << std::endl;
        return true;
    }
    else {
        std::cout << "Unable to open file" << std::endl;
        return false;
    }
}

std::map<std::pair<int,int>,int> TileMap::load(const std::string fileName, const std::string& tileset, int tileSize)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        std::cout << "Error loading tileset" << std::endl;
    // create constant, used for setting texture location for tiles
    // this isn't used in this function, but we dont need to get the constant everything we draw stuff
    // so we do it here
    tileSetConstant = (m_tileset.getSize().x / tileSize);

    // load map
    std::string map_data;
    std::ifstream map_stream(fileName);
    if (map_stream.is_open())
    {
        while(!map_stream.eof()) // To get you all the lines.
        {
            std::string temp;
            std::getline(map_stream, temp);
            map_data.append(temp);
        }
        map_stream.close();
    }

    // split raw data into vector data
    std::vector<std::string> map_vector = splitString(map_data, '-');
    // get map size from vector data
   // map_size = splitStringToPair(map_vector[map_vector.size() - 1], ',');
    // remove map size from vector data
    //map_vector.erase(map_vector.end() - 1);

    // create list of all tiles
    std::map<std::pair<int,int>,int> tiles;

    int max_x = 0;
    int max_y = 0;

    // for each tile
    for (std::vector<std::string>::iterator it = map_vector.begin() ; it != map_vector.end(); it++)
    {
        // split into vector of pos and type
        std::vector<std::string> tile_tmp = splitString(*it, ':');

        // create pos and type individual variables
        std::pair<int,int> pos = splitStringToPair(tile_tmp[0], ',');

        if (pos.first > max_x)
            max_x = pos.first;
        if (pos.second > max_y)
            max_y = pos.second;

        int type = std::atoi(tile_tmp[1].c_str());

        // create tile object, and add to list of tiles
        tiles[pos] = type;
    }

    map_size = {max_x + 1, max_y + 1};
    std::cout << "Loaded map Size: " << map_size.first << ", " << map_size.second << std::endl;

    return tiles;
}

void TileMap::create (std::map<std::pair<int,int>,int> tiles, const std::string& tileset, int tileSize)
{

    m_vertices.clear();

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(map_size.first * map_size.second * 4);


    std::map<std::pair<int,int>, int>::iterator it;
    for(it = tiles.begin(); it != tiles.end(); it++)
    {
        sf::Vector2i pos = sf::Vector2i(it->first.first, it->first.second);
        int type = it->second;

        int tu = type % tileSetConstant;
        int tv = type / tileSetConstant;

        // get a pointer to the current tile's quad
        sf::Vertex* quad = &m_vertices[(pos.x + pos.y * map_size.first) * 4];

        // define its 4 corners
        quad[0].position = sf::Vector2f(pos.x * tileSize, pos.y * tileSize);
        quad[1].position = sf::Vector2f((pos.x + 1) * tileSize, pos.y * tileSize);
        quad[2].position = sf::Vector2f((pos.x + 1) * tileSize, (pos.y + 1) * tileSize);
        quad[3].position = sf::Vector2f(pos.x * tileSize, (pos.y + 1) * tileSize);

        // define its 4 texture coordinates
        quad[0].texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
        quad[3].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

