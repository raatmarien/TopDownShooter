/*
A top down 2d shooter
Copyright (C) 2015  Marien Raat

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    int framesToCheckEverything, currentPart;
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
