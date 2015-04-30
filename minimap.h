#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "shadow.h"

class Minimap {
public:
    Minimap();
    void setTileSize(int nTileSize);
    void addWall(sf::Vector2u position);
    void draw(sf::RenderWindow* window);
    void setPositionFromCenter(sf::Vector2f
                               fromCenterPosition);
    void setViewCenter(sf::Vector2f viewCenter);
    void update();
    sf::Vector2f getSize();
    void setPlayerPosition(sf::Vector2f position);
    void setScreenSize(int screenX, int screenY);
    void setWalls(std::vector<sf::Vector2f>
                  newWalls);
private:
    std::vector<sf::Vector2f> getWallsOnScreen();
    std::vector<sf::Vector2f> wallPositions;
    std::vector<bool> isWallSeen, onScreenIsWallSeen;
    std::vector<int> onScreenWallIndex;
    sf::Vector2f position, currentViewCenter;
    sf::Vector2f playerPosition;
    int tileSize, screenWidth, screenHeight;
    sf::Image minimapImage;
    sf::Texture minimapTexture;
    sf::Sprite minimapSprite;
    sf::CircleShape player;
    std::vector<sf::Vector2u> wallPoints;
    sf::Color background, walls;
};
