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
ShadowHandler::ShadowHandler() {
    setScreenDiagonal(1000, 1000);
}

void ShadowHandler::setObstacles(std::vector<Vector2f> obstacles, int tileSize) {
    for (int i = 0; i < obstacles.size(); i++) {
        float x = obstacles[i].x
            , y = obstacles[i].y;
        obstaclePoints.push_back(Vector2f(x, y));
        obstaclePoints.push_back(Vector2f(x + tileSize, y));
        obstaclePoints.push_back(Vector2f(x + tileSize, y + tileSize));
        obstaclePoints.push_back(Vector2f(x, y + tileSize));
    }
}

void ShadowHandler::setScreenDiagonal(int screenX
                                      , int screenY) {
    screenDiagonal = (int) (ceil(sqrt(screenX * screenX
                                      + screenY * screenY)));
    screenWidth = screenX;
    screenHeight = screenY;
}

std::vector<Vector2f> ShadowHandler::getObstaclesInRange(Vector2f sightCenter) {
    std::vector<Vector2f> obstaclesInRange;
    float range = (float) (screenDiagonal) / 2.0f;
    for (int i = 0; i < obstaclePoints.size(); i += 4) {
        float halfX = (float) (screenWidth) / 2
            , halfY = (float) (screenHeight) / 2;
        bool rightY = (obstaclePoints[i+2].y > (sightCenter.y - halfY))
            && (obstaclePoints[i].y < (sightCenter.y + halfY))
            , rightX = (obstaclePoints[i+2].x > (sightCenter.x - halfX))
            && (obstaclePoints[i].x < (sightCenter.x + halfX));
        if (rightX && rightY) {
            obstaclesInRange.push_back(obstaclePoints[i+0]);
            obstaclesInRange.push_back(obstaclePoints[i+1]);
            obstaclesInRange.push_back(obstaclePoints[i+2]);
            obstaclesInRange.push_back(obstaclePoints[i+3]);
        }
    }
    return obstaclesInRange;
}

void ShadowHandler::update(Vector2f sightCenter
                           , Vector2f viewCenter) {
    std::vector<Vector2f> obstacles = getObstaclesInRange(viewCenter);
    VertexArray vertices;
    vertices.resize(obstacles.size() * 2);
    vertices.setPrimitiveType(Quads);

    float viewDistance = screenDiagonal; // TODO change dynamically
    float pi = 3.14159265359;
    for (int i = 0; i < obstacles.size(); i += 4) {
        float rotation[4];
        int r = 0;
        for (int j = i; j < i + 4; j++) {
            Vector2f rayVector = obstacles[j]
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
        Vector2f direction1 = normalize
            (obstacles[minIndex] - sightCenter)
            , direction2 = normalize
            (obstacles[maxIndex] - sightCenter);
        float move = 10;
        Vector2f startPoint1 = obstacles[minIndex]
            + (move * direction1)
            , startPoint2 = obstacles[maxIndex]
                  + (move * direction2);
        
        vertices[2*i+0].position = startPoint1;
        vertices[2*i+1].position = startPoint2;
        vertices[2*i+2].position = direction2
            * viewDistance + startPoint2;
        vertices[2*i+3].position = direction1
            * viewDistance + startPoint1;

        // Draw black rectangle in shape of the original
        // obstacle
        vertices[2*i+4].position = obstacles[i+0] + (move * normalize(obstacles[i+0] - sightCenter));
        vertices[2*i+5].position = obstacles[i+1] + (move * normalize(obstacles[i+1] - sightCenter));
        vertices[2*i+6].position = obstacles[i+2] + (move * normalize(obstacles[i+2] - sightCenter));
        vertices[2*i+7].position = obstacles[i+3] + (move * normalize(obstacles[i+3] - sightCenter));

    }
    for (int i = 0; i < vertices.getVertexCount(); i++) {
        vertices[i].color = Color(0,0,0);
    }
    shadows.setVertices(vertices);
}

void ShadowHandler::draw(RenderWindow* window) {
    window->draw(shadows);
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
