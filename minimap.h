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
    void setPlayerPosition(sf::Vector2f position);
private:
    sf::Vector2f position, currentViewCenter;
    sf::Vector2f playerPosition;
    int tileSize;
    sf::Image minimapImage;
    sf::Texture minimapTexture;
    sf::Sprite minimapSprite;
    sf::CircleShape player;
    std::vector<sf::Vector2u> wallPoints;
    sf::Color background, walls;
};
