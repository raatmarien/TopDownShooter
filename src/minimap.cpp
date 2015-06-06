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

#include <minimap.h>
#include <shadow.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace sf;

Minimap::Minimap() {
    tileSize = 1;
    background = Color(0,0,0,0);
    walls = Color(255,255,255,255);
    position = Vector2f(0,0);
    currentViewCenter = Vector2f(0,0);
    minimapImage.create(1,1,background);
    minimapTexture.loadFromImage(minimapImage);
    minimapSprite.setTexture(minimapTexture);
    minimapSprite.setPosition(0,0);
    player.setRadius(3);
    player.setOrigin(3,3);
    player.setFillColor(Color(255,0,0));
    playerPosition = Vector2f(0,0);

    currentPart = 0;
    framesToCheckEverything = 20;
    scale = 1.0f;
}

void Minimap::setTileSize(int nTileSize) {
    tileSize = nTileSize;
}


void Minimap::setScale(float scale) {
    this->scale = scale;
}

// Position should be given in tiles
void Minimap::addWall(Vector2u position) {
    unsigned int x = (unsigned int) (position.x);
    unsigned int y = (unsigned int) (position.y);
    wallPoints.push_back(Vector2u(x, y));
    Vector2u currentSize = minimapImage.getSize();
    if (x >= currentSize.x
        || y >= currentSize.y) {
        unsigned int width = (x >= currentSize.x)
            ? (x + 1) : currentSize.x;
        unsigned int height = (y >= currentSize.y)
            ? (y + 1) : currentSize.y;
        minimapImage.create(width, height, background); 
        for (int i = 0; i < wallPoints.size(); i++) {
            minimapImage.setPixel(wallPoints[i].x
                                  , wallPoints[i].y
                                  , walls);
        }
    } else {
        minimapImage.setPixel(x, y, walls);
    }
}

void Minimap::draw(RenderWindow* window) {
    minimapSprite.setPosition(currentViewCenter + position);
    player.setPosition(currentViewCenter + position
                       + ((playerPosition * scale) / (float) tileSize)
                       - Vector2f(minimapImage.getSize().x, 0));
    window->draw(minimapSprite);
    window->draw(player);
}

void Minimap::setPositionFromCenter(Vector2f
                                    positionFromCenter) {
    position = positionFromCenter;
}

void Minimap::setViewCenter(Vector2f viewCenter) {
    currentViewCenter = viewCenter;
}

Vector2f Minimap::getSize() {
    IntRect spriteRect = minimapSprite.getTextureRect();
    return Vector2f(spriteRect.width, spriteRect.height);
}

void Minimap::setPlayerPosition(Vector2f position) {
    playerPosition = position;
}

void Minimap::setScreenSize(int screenX, int screenY) {
    screenWidth = screenX;
    screenHeight = screenY;
}

void Minimap::setWalls(std::vector<Vector2f> newWalls) {
    wallPositions = newWalls;
    isWallSeen.resize(wallPositions.size(), false);
}

std::vector<Vector2f> Minimap::getWallsOnScreen() {
    std::vector<Vector2f> wallsOnScreen;
    onScreenIsWallSeen.clear();
    onScreenWallIndex.clear();
    int screenBot = currentViewCenter.y + (screenHeight / 2)
        , screenTop = currentViewCenter.y - (screenHeight / 2)
        , screenLeft = currentViewCenter.x - (screenWidth / 2)
        , screenRight = currentViewCenter.x + (screenWidth / 2);
    float halfT = ((float) (tileSize)) / 2;
    for (int i = 0; i < wallPositions.size(); i++) {
        bool rightY = wallPositions[i].y + halfT > screenTop
            && wallPositions[i].y + halfT < screenBot;
        bool rightX = wallPositions[i].x + halfT > screenLeft
            && wallPositions[i].x + halfT < screenRight;
        if(rightX && rightY) {
            wallsOnScreen.push_back(wallPositions[i]);
            onScreenIsWallSeen.push_back(isWallSeen[i]);
            onScreenWallIndex.push_back(i);
        }
    }
    return wallsOnScreen;
}

void Minimap::update() {
    std::vector<Vector2f> walls = getWallsOnScreen();
    float halfT = ((float) (tileSize)) / 2;
    int startI = (int) ((((float) currentPart)
                        / ((float) framesToCheckEverything))
                        * walls.size());
    int endBeforeI = (int) ((((float) (currentPart+1))
                            / ((float) framesToCheckEverything))
                            * walls.size());
    currentPart++;
    if (currentPart == framesToCheckEverything) {
        currentPart = 0;
    }
    for(int i = startI; i < endBeforeI; i++) {
        if (!onScreenIsWallSeen[i]) {
            Vector2f corner1 = walls[i]
                , corner2 = walls[i] + Vector2f(tileSize, 0)
                , corner3 = walls[i] + Vector2f(tileSize, tileSize)
                , corner4 = walls[i] + Vector2f(0, tileSize);
            bool visible1 = true
                , visible2 = true
                , visible3 = true
                , visible4 = true;
            for (int j = 0; j < walls.size(); j++) {
                if (i != j) {
                    Vector2f w1 = walls[j] + Vector2f(-halfT,-halfT);
                    Vector2f w2 = walls[j] + Vector2f(halfT,-halfT);
                    Vector2f w3 = walls[j] + Vector2f(halfT,halfT);
                    Vector2f w4 = walls[j] + Vector2f(-halfT,halfT);
                    if (visible1
                        && (lineSegmentsIntersect(playerPosition
                                                  , corner1
                                                  , w1
                                                  , w2)
                        || lineSegmentsIntersect(playerPosition
                                                 , corner1
                                                 , w2
                                                 , w3))) { 
                        visible1 = false;
                    }
                    if (visible2
                        && (lineSegmentsIntersect(playerPosition
                                                  , corner2
                                                  , w1
                                                  , w2)
                        || lineSegmentsIntersect(playerPosition
                                                 , corner2
                                                 , w2
                                                 , w3))) { 
                        visible2 = false;
                    }
                    if (visible3
                        && (lineSegmentsIntersect(playerPosition
                                                  , corner3
                                                  , w1
                                                  , w2)
                        || lineSegmentsIntersect(playerPosition
                                                 , corner3
                                                 , w2
                                                 , w3))) { 
                        visible3 = false;
                    }
                    if (visible4
                        && (lineSegmentsIntersect(playerPosition
                                                  , corner4
                                                  , w1
                                                  , w2)
                        || lineSegmentsIntersect(playerPosition
                                                 , corner4
                                                 , w2
                                                 , w3))) { 
                        visible4 = false;
                    }
                }
            }
            if (visible1 || visible2 || visible3 || visible4) {
                isWallSeen[onScreenWallIndex[i]] = true;
                addWall(Vector2u((walls[i].x * scale) / tileSize
                                 , (walls[i].y * scale) / tileSize));
            }
        }
    }
    minimapTexture.loadFromImage(minimapImage);
    Vector2u textureSize = minimapTexture.getSize();
    minimapSprite.setTextureRect(IntRect(0,0,textureSize.x
                                         , textureSize.y));
    minimapSprite.setOrigin(textureSize.x, 0);
}
