#include "minimap.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace sf;

Minimap::Minimap() {
    tileSize = 1;
    background = Color(0,0,0,0);
    walls = Color(255,255,255,255);
    minimapImage.create(4,4,background);
    minimapTexture.loadFromImage(minimapImage);
    minimapSprite.setTexture(minimapTexture);
    minimapSprite.setPosition(100,100);
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
    window->draw(minimapSprite);
}
