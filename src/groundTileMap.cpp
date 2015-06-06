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

#include <groundTileMap.h>
#include <collidable.h>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <fstream>
#include <vector>
#include <iostream>

using namespace sf;

void groundTileMap::genGroundTileMap (Image* map
                                      , int tilesW, int tilesH
                                      , int textureTileGridWidth
                                      , b2World *world, int nScale) {
    // Set up collide data
    myCollideData.collideType = COLLIDE_TYPE_WALL;
    
    tilesWidth = tilesW;
    tilesHeight = tilesH;
    SCALE = nScale;
    width = map->getSize().x;
    height = map->getSize().y;
    vertices.setPrimitiveType(Quads);
    vertices.resize(4 * width * height);

    normalVertices.setPrimitiveType(Quads);
    normalVertices.resize(4 * width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int indexInVertexArray = 4 * (width * y + x);
            int tileNum = map->getPixel(x, y).r; // The red value
                                             // of the corresponding pixel
                                             // used for information

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
                cornerTextureX = tilesWidth * (rand() % tilesAmmountX);
                cornerTextureY = (0 + rand() % 3) * tilesHeight;
                // cornerTextureX = 0;
                // cornerTextureY = 0;
            }
            if (tileNum == 1) { // wall
                cornerTextureX = 2 * tilesWidth;
            }

            int rotation = rand() % 4;
            
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

            
            // Top left corner
            normalVertices[indexInVertexArray+0].position
                = Vector2f(x * tilesWidth
                           , y * tilesHeight);
            // Top right corner
            normalVertices[indexInVertexArray+1].position
                = Vector2f((x + 1) * tilesWidth
                           , y * tilesHeight);
            // Bottom right corner
            normalVertices[indexInVertexArray+2].position
                = Vector2f((x + 1) * tilesWidth
                           , (y + 1) * tilesHeight);
            // Bottom left corner
            normalVertices[indexInVertexArray+3].position
                = Vector2f(x * tilesWidth
                           , (y + 1) * tilesHeight);


            // Texture Coords
            // Top left corner
            normalVertices[indexInVertexArray+turnArray[0]].texCoords
                = Vector2f(cornerTextureX
                           , cornerTextureY);
            // Top right corner
            normalVertices[indexInVertexArray+turnArray[1]].texCoords
                = Vector2f(cornerTextureX + tilesWidth - 0.1f
                           , cornerTextureY);
            // Bottom right corner
            normalVertices[indexInVertexArray+turnArray[2]].texCoords
                = Vector2f(cornerTextureX + tilesWidth - 0.1f
                           , cornerTextureY + tilesHeight);
            // Bottom left corner
            normalVertices[indexInVertexArray+turnArray[3]].texCoords
                = Vector2f(cornerTextureX
                           , cornerTextureY + tilesHeight);

            // Rotation is saved in the alpha value of the vertex color
            // with the alpha being the rotation where 0.0f is upright and every
            // increase of 0.25f is a turn of 90 degrees clockwise
            Color rotationInfoColor = Color(255, 255, 255, ((float) rotation) * (255.0f / 4.0f));
            normalVertices[indexInVertexArray+0].color = rotationInfoColor;
            normalVertices[indexInVertexArray+1].color = rotationInfoColor;
            normalVertices[indexInVertexArray+2].color = rotationInfoColor;
            normalVertices[indexInVertexArray+3].color = rotationInfoColor;
            
            // Adding a physical box if necessary
            if (isSolid(tileNum)) {
                b2BodyDef boxBodyDef;
                boxBodyDef.position.Set((float)((float)(x + 0.5) * tilesWidth)
                                     / (float) (SCALE)
                                     , (float)((float)(y + 0.5) * tilesWidth)
                                     / (float) (SCALE));
                boxBodyDef.userData = &myCollideData;
                b2Body *boxBody = world->CreateBody(&boxBodyDef);

                b2PolygonShape boxBodyShape;
                boxBodyShape.SetAsBox((float)(tilesWidth) / (2 * SCALE)
                                      , (float)(tilesHeight) / (2 * SCALE));
                boxBody->CreateFixture(&boxBodyShape, 0.0f);

                // Add solid blocks to obstacles
                obstacles.push_back(Vector2f(x * tilesWidth, y * tilesHeight));
            }
        }
        // std::cout << "\n"; // debug
    }
}

std::vector<Vector2f> groundTileMap::getObstacles() {
    return obstacles;
}

void groundTileMap::setShader(Shader *tileMapRotationShader) {
    this->tileMapRotationShader = tileMapRotationShader;
}

void groundTileMap::setNormalDrawing(bool normalDrawingOn) {
    this->normalDrawingOn = normalDrawingOn;
}

void groundTileMap::draw(RenderTarget& target, RenderStates states) const
{
    states.transform *= getTransform();
    if (normalDrawingOn) {
        states.shader = tileMapRotationShader;
        target.draw(normalVertices, states);
    } else {
        target.draw(vertices, states);
    }
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
    
