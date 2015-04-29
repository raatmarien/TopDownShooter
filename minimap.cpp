#include "minimap.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

Minimap::Minimap() {
    tileSize = 1;
    background = Color(0,0,0,0);
    walls = Color(256,256,256,256);
    minimapImage.create(1,1,background);
    minimapTexture.loadFromImage(minimapImage);
    minimapSprite.setTexture(minimapTexture);
    minimapSprite.setPosition(500,500);
}

void Minimap::setTileSize(int nTileSize) {
    tileSize = nTileSize;
}

void Minimap::addWall(Vector2f position) {
    unsigned int x = (unsigned int) (position.x / tileSize);
    unsigned int y = (unsigned int) (position.y / tileSize);
    wallPoints.push_back(Vector2u(x, y));
    Vector2u currentSize = minimapImage.getSize();
    if (x >= currentSize.x
        || y >= currentSize.y) {
        minimapImage.create(x+1, y+1, background); 
        for (int i = 0; i < wallPoints.size(); i++) {
            minimapImage.setPixel(wallPoints[i].x
                                  , wallPoints[i].y
                                  , walls);
        }
    } else {
        minimapImage.setPixel(x, y, walls);
    }
    minimapTexture.loadFromImage(minimapImage);
    minimapSprite.setTexture(minimapTexture);
}

void Minimap::draw(RenderWindow* window) {
    window->draw(minimapSprite);
}
