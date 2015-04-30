#include "minimap.h"
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
    minimapImage.create(4,4,background);
    minimapTexture.loadFromImage(minimapImage);
    minimapSprite.setTexture(minimapTexture);
    minimapSprite.setPosition(0,0);
}

void Minimap::setTileSize(int nTileSize) {
    tileSize = nTileSize;
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
        minimapTexture.loadFromImage(minimapImage);
        minimapSprite.setTextureRect(IntRect(0,0,width,height));
    } else {
        minimapImage.setPixel(x, y, walls);
        minimapTexture.loadFromImage(minimapImage);
    }
}

void Minimap::draw(RenderWindow* window) {
    minimapSprite.setPosition(currentViewCenter + position);
    window->draw(minimapSprite);
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
