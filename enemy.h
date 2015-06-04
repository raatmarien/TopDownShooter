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
#include "collidable.h"
#include "updatable.h"
#include "player.h"

class Enemy : public Updatable {
public:
    virtual void update() = 0;
    virtual void hit() = 0;
    virtual void destroy() = 0;
    virtual void setDrawNormal(bool drawNormal) = 0;
    bool queuedForRemoval() { return toBeRemoved; }
    bool toBeRemoved;
private:
    virtual void draw(sf::RenderTarget &target
                      , sf::RenderStates states) const = 0;
};

enum ChargingEnemyState {
    CHARGING_ENEMY_WAITING = 0,
    CHARGING_ENEMY_WANDERING,
    CHARGING_ENEMY_CHARGING
};

class ChargingEnemy : public Enemy {
public:
    void initialize(sf::Texture *diffuseTexture
                    , sf::Texture *normalTexture
                    , float radius
                    , sf::Vector2f startPosition
                    , float moveForce
                    , float rotationTorque
                    , int scale
                    , int tileSize
                    , Player *player
                    , b2World *world
                    , std::vector<sf::Vector2f>* wallPoints);
    void update();
    void hit();
    void destroy();
    void setDrawNormal(bool drawNormal);
private: 
    int scale;
    int tileSize;
    int visionRadius;
    
    b2World *world;
    Player *player;
    std::vector<sf::Vector2f>* wallPoints;

    CollideData myCollideData;
    float radius;
    b2Body* body;
    sf::Texture *diffuseTexture, *normalTexture;
    sf::Sprite sprite;

    float moveForce, rotationTorque;
    float currentForce, currentTorque;
    ChargingEnemyState state;

    bool isVisible(sf::Vector2f position);
    
    void move(bool forward);
    void turn(bool left);
    virtual void draw(sf::RenderTarget &target
                      , sf::RenderStates states) const;
};
