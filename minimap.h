#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Minimap {
public:
    Minimap();
    void setTileSize(int nTileSize);
    void addWall(sf::Vector2u position);
    void draw(sf::RenderWindow* window);
    void setPositionFromCenter(sf::Vector2f
                               fromCenterPosition);
    void setViewCenter(sf::Vector2f viewCenter);
    sf::Vector2f getSize();
private:
    sf::Vector2f position, currentViewCenter;
    int tileSize;
    sf::Image minimapImage;
    sf::Texture minimapTexture;
    sf::Sprite minimapSprite;
    std::vector<sf::Vector2u> wallPoints;
    sf::Color background, walls;
};
