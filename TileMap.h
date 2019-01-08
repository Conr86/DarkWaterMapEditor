#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>


class TileMap : public sf::Drawable, public sf::Transformable
{
    public:
        // the map size is stored as a Vector2i, basically a pair, but accessable using .x and .y
        std::pair<int,int> map_size;
        // something used to create the map, calculated in the load function
        int tileSetConstant;

        // function to get size of map from parsed tile_data
        void calcMapSize(std::map<std::pair<int,int>,int> tiles);
        // tiles are stored as a map. with the position (a pair of ints) as the key and the type (int) as the value
        std::map<std::pair<int,int>,int> load(const std::string map_file, const std::string& tileset, int tileSize);
        // save to file
        bool save(std::map<std::pair<int,int>,int> tiles, std::string fileName);
        // creates the map drawable thing
        void create (std::map<std::pair<int,int>,int> tiles, const std::string& tileset, int tileSize);
        sf::Texture m_tileset;


    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        sf::VertexArray m_vertices;
        //sf::Texture m_tileset;
};




#endif // TILEMAP_H
