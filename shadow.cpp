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

#include "shadow.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace sf;

Vector2f normalize(Vector2f vector) {
    float length = sqrt(vector.x * vector.x
                        + vector.y * vector.y);
    return Vector2f(vector.x / length
                    , vector.y / length);
}

// ShadowHandler methods
void ShadowHandler::genObstaclePoints(const char* filepath
                                      , int tileSize) {
    std::ifstream mapReader(filepath);
    char buf[10];
    mapReader >> buf;
    int width, height;
    mapReader >> width >> height;
    mapReader >> buf;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tileNum;
            mapReader >> tileNum;
            if (tileNum == 1) {
                obstaclePoints.push_back(
                    Vector2f(x * tileSize
                             , y * tileSize));
                obstaclePoints.push_back(
                    Vector2f((x+1) * tileSize
                             , y * tileSize));
                obstaclePoints.push_back(
                    Vector2f((x+1) * tileSize
                             , (y+1) * tileSize));
                obstaclePoints.push_back(
                    Vector2f(x * tileSize
                             , (y+1) * tileSize));
            }
        }
    }
}

void ShadowHandler::update(Vector2f sightCenter) {
    VertexArray vertices;
    vertices.resize(obstaclePoints.size());
    vertices.setPrimitiveType(Quads);

    float viewDistance = 1000; // TODO change dynamically
    float pi = 3.14159265359;
    for (int i = 0; i < obstaclePoints.size(); i += 4) {
        float rotation[4];
        int r = 0;
        for (int j = i; j < i + 4; j++) {
            Vector2f rayVector = obstaclePoints[j]
                - sightCenter; 
            
            rotation[r] = std::atan2
                (rayVector.x, rayVector.y) + pi;
            r++;
        }
        float maxDistance = 0;
        int minIndex, maxIndex;
        for (int x = 0; x < 4; x++) {
            for (int y = x + 1; y < 4; y++) {
                float distance = fabs(rotation[x]
                                      - rotation[y]);
                if (distance > pi) {
                    float minRot = rotation[x] < rotation[y]
                                                 ? rotation[x]
                                                 : rotation[y];
                    float maxRot = rotation[x] > rotation[y]
                                                 ? rotation[x]
                                                 : rotation[y];
                    distance = fabs(maxRot - (minRot + 2 * pi));
                }
                if (distance > maxDistance) {
                    maxDistance = distance;
                    minIndex = i + x;
                    maxIndex = i + y;
                }
            }
        }
        vertices[i+0].position = obstaclePoints[minIndex];
        vertices[i+1].position = obstaclePoints[maxIndex];
        vertices[i+2].position = normalize(
            obstaclePoints[maxIndex]
            - sightCenter)
            * viewDistance + obstaclePoints[maxIndex];
        vertices[i+3].position = normalize(
            obstaclePoints[minIndex]
            - sightCenter)
            * viewDistance + obstaclePoints[minIndex];

        for (int j = i; j < i + 4; j++) {
            vertices[j].color = Color::Black;
        }
    }
    shadows.setVertices(vertices);
}

void ShadowHandler::draw(RenderWindow* window) {
    window->draw(shadows);
    // blockRedrawer.draw(window);
}

void ShadowHandler::setBlockTexture(Texture texture
                                    , IntRect textureRect) {
    blockRedrawer.setTexture(texture, textureRect);
}

// ShadowDrawable methods
void ShadowDrawable::setVertices(VertexArray nVertices) {
    vertices = nVertices;
}

void ShadowDrawable::draw(RenderTarget& target
                         , RenderStates states) const {
    states.transform *= getTransform();
    target.draw(vertices);
}

// BlockRedrawer methods
void BlockRedrawer::resetBlocks() {
    blockPositions.clear();
}

void BlockRedrawer::addBlock(Vector2f position) {
    blockPositions.push_back(Vector2f(position));
}

void BlockRedrawer::setTexture(Texture nTexture
                               , IntRect textureRect) {
    texture = nTexture;
    blockSprite.setTexture(texture);
    blockSprite.setTextureRect(textureRect);
}

void BlockRedrawer::draw(RenderWindow* window) {
    for (int i = 0; i < blockPositions.size(); i++) {
        blockSprite.setPosition(blockPositions[i]);
        window->draw(blockSprite);
    }
}
