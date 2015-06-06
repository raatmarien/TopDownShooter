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
#include "updatable.h"
#include "enemy.h"
#include "item.h"
#include "player.h"
#include <vector>


class UpdatableManager {
public:
    void initialize(sf::Shader *normalRotationShader);
    void initializeChargingEnemys(std::vector<sf::Vector2f> chargingEnemyPositions
                                  , sf::Texture diffuseTexture
                                  , sf::Texture normalTexture
                                  , float radius
                                  , float moveForce
                                  , float rotationTorque
                                  , int scale
                                  , int tileSize
                                  , Player *player
                                  , b2World *world
                                  , std::vector<sf::Vector2f> wallPoints);
    void initializeBoxes(std::vector<sf::Vector2f> boxPositions
                         , sf::Texture boxTextureDiffuse
                         , sf::Texture boxTextureNormal
                         , sf::Vector2f boxSize
                         , b2World *world
                         , int scale);
    void update();
    void setNormalDraw(bool drawNormal);
    void draw(sf::RenderTarget *target);
private:
    std::vector<Updatable*> updatables;
    bool drawNormal;

    sf::Shader *normalRotationShader;
    sf::Texture chargingEnemyDiffuse, chargingEnemyNormal
        , boxTextureDiffuse, boxTextureNormal;
    std::vector<sf::Vector2f> wallPoints;
};
