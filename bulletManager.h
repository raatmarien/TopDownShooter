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
#include <vector>

class Bullet : public sf::Drawable
, public sf::Transformable {
public:
    Bullet(sf::Vector2f position
           , float radius
           , b2Vec2 nVelocity
           , b2World* nWorld
           , int nScale); 
    void update();
    void queueRemoval();
    void destroyBody();
    bool isQueuedForRemoval();
private:
    bool needsToBeRemoved;
    CollideData myCollideData;
    int scale;
    b2Vec2 velocity;
    sf::CircleShape bulletCircle;
    b2World* world;
    b2Body* bulletBody;

    virtual void draw(sf::RenderTarget &target
                      , sf::RenderStates states) const;
};


class BulletManager {
public:
    BulletManager();
    void initialize(b2World *nWorld, int nScale
                    , float nBulletSpeed
                    , float nBulletRadius);
    void update();
    void drawDiffuse(sf::RenderTarget* diffuseTarget);
    void addBullet(sf::Vector2f startPosition
                   , sf::Vector2f normalizedDirection);
private:
    b2World *world;
    int scale, bulletRadius;
    float bulletSpeed;
    std::vector<Bullet> bullets;
};
