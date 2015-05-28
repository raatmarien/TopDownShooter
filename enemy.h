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
#include "collidable.h"
#include "player.h"

class Enemy : public sf::Drawable
, public sf::Transformable {
public:
    virtual void update() = 0;
    virtual void hit() = 0;
private:
    virtual void draw(sf::RenderTarget &target
                      , sf::RenderStates states) const = 0;
};

class ChargingEnemy : public Enemy {
public:
    void initialize(sf::Texture texture
                    , float radius
                    , sf::Vector2f startPosition
                    , float moveForce
                    , float rotationTorque
                    , int scale
                    , Player *player
                    , b2World *world);
    void update();
    void hit();
private: 
    int scale;
    
    b2World *world;
    Player *player;

    CollideData myCollideData;
    float radius;
    b2Body* body;
    sf::Texture texture;
    sf::Sprite sprite;

    float moveForce, rotationTorque;
    float currentForce, currentTorque;
    // temp
    bool isHit;
    int hittimer;
    void move(bool forward);
    void turn(bool left);
    virtual void draw(sf::RenderTarget &target
                      , sf::RenderStates states) const;
};
