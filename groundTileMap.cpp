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

#include "groundTileMap.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <fstream>
#include <vector>
#include <iostream>

using namespace sf;

void groundTileMap::genGroundTileMap (const char* filename
                                      , int tilesW, int tilesH
                                      , int textureTileGridWidth
                                      , b2World *world, int nScale) {
    tilesWidth = tilesW;
    tilesHeight = tilesH;
    SCALE = nScale;
    std::ifstream bitmap(filename);
    char buf[10];
    bitmap >> buf;
    bitmap >> width >> height;
    bitmap >> buf; // The range of the grayscale
                   // Not needed for reading for us
    vertices.setPrimitiveType(Quads);
    vertices.resize(4 * width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int indexInVertexArray = 4 * (width * y + x), tileNum;
            bitmap >> tileNum; // The grayscale value
                               // of the corresponding pixel

            // For easier map editing in graphic software
            switch(tileNum) {
            case 255:
                tileNum = 1;
                break;
            }
            
            // std::cout << getDisplayChar(tileNum) << " ";
            
            int cornerTextureX = (tileNum % textureTileGridWidth)
                * tilesWidth
                , cornerTextureY = (tileNum / textureTileGridWidth)
                * tilesHeight;

            // If ground, randomnize textureY for random ground
            if (tileNum == 0) {
                int tilesAmmount = 4;
                cornerTextureY += tilesHeight * (rand() % tilesAmmount);
                int tilesAmmountX = 2;
                cornerTextureX += tilesWidth * (rand() % tilesAmmountX);
            }
            if (tileNum == 1) { // wall
                cornerTextureX = 2 * tilesWidth;
            }

            // Top left corner
            vertices[indexInVertexArray+0].position
                = Vector2f(x * tilesWidth
                           , y * tilesHeight);
            // Top right corner
            vertices[indexInVertexArray+1].position
                = Vector2f((x + 1) * tilesWidth
                           , y * tilesHeight);
            // Bottom right corner
            vertices[indexInVertexArray+2].position
                = Vector2f((x + 1) * tilesWidth
                           , (y + 1) * tilesHeight);
            // Bottom left corner
            vertices[indexInVertexArray+3].position
                = Vector2f(x * tilesWidth
                           , (y + 1) * tilesHeight);

            int rotation = 0; // Todo: random rotation?
                             // rand() % 4;
            // rotation = rand() % 4;
            int turnArray[4] = {0,1,2,3};
            if (rotation == 1) {
                turnArray[0] = 1;
                turnArray[1] = 2;
                turnArray[2] = 3;
                turnArray[3] = 0;
            } else if (rotation == 2) {
                turnArray[0] = 2;
                turnArray[1] = 3;
                turnArray[2] = 0;
                turnArray[3] = 1;
            } else if (rotation == 3) {
                turnArray[0] = 3;
                turnArray[1] = 0;
                turnArray[2] = 1;
                turnArray[3] = 2;
            }

            // Texture Coords
            // Top left corner
            vertices[indexInVertexArray+turnArray[0]].texCoords
                = Vector2f(cornerTextureX
                           , cornerTextureY);
            // Top right corner
            vertices[indexInVertexArray+turnArray[1]].texCoords
                = Vector2f(cornerTextureX + tilesWidth - 0.1f
                           , cornerTextureY);
            // Bottom right corner
            vertices[indexInVertexArray+turnArray[2]].texCoords
                = Vector2f(cornerTextureX + tilesWidth - 0.1f
                           , cornerTextureY + tilesHeight);
            // Bottom left corner
            vertices[indexInVertexArray+turnArray[3]].texCoords
                = Vector2f(cornerTextureX
                           , cornerTextureY + tilesHeight);

            
            // Adding a physical box if necessary
            if (isSolid(tileNum)) {
                b2BodyDef boxBodyDef;
                boxBodyDef.position.Set((float)((float)(x + 0.5) * tilesWidth)
                                     / (float) (SCALE)
                                     , (float)((float)(y + 0.5) * tilesWidth)
                                     / (float) (SCALE));
                b2Body *boxBody = world->CreateBody(&boxBodyDef);

                b2PolygonShape boxBodyShape;
                boxBodyShape.SetAsBox((float)(tilesWidth) / (2 * SCALE)
                                      , (float)(tilesHeight) / (2 * SCALE));
                boxBody->CreateFixture(&boxBodyShape, 0.0f);
            }
        }
        // std::cout << "\n"; // debug
    }
}

void groundTileMap::draw(RenderTarget& target, RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(vertices, states);
}

bool isSolid(int tileNum)
{
    return tileNum == 1;
}

char getDisplayChar(int tileNum)
{
    switch (tileNum) {
    case 0:
        return '.';
    case 1:
        return 'X';
    case 2:
        return 'E';
    default:
        return ' ';
    }
}
    
