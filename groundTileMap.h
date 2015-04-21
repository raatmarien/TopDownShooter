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
#include <Box2D/Box2D.h>
#include <vector>
bool isSolid(int tileNum);
char getDisplayChar(int tileNum);

using namespace sf;

class groundTileMap : public sf::Drawable, public sf::Transformable
{
public:
    void genGroundTileMap (const char* filename, sf::Texture nTexture
                           , int tilesW, int tilesH
                           , int textureTileGridWidth, b2World *world, int nScale);

private:
    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    int width, height, SCALE;
    sf::VertexArray vertices;
    sf::Texture texture;
    int tilesWidth, tilesHeight;
};
