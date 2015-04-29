#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Minimap {
public:
    Minimap();
    void setTileSize(int nTileSize);
    void addWall(Vector2f position);
    void draw(sf::RenderWindow* window);
private:
    int tileSize;
    sf::Image minimapImage;
    sf::Texture minimapTexture;
    sf::Sprite minimapSprite;
    std::vector<sf::Vector2u> wallPoints;
    sf::Color background, walls;
};
