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
#include <bulletManager.h>
#include <collidable.h>

enum Movement {
    NONE = 0
    , UP
    , UP_RIGHT
    , RIGHT
    , DOWN_RIGHT
    , DOWN
    , DOWN_LEFT
    , LEFT
    , UP_LEFT
};

class Player : public sf::Drawable
, public sf::Transformable {
public:
    void initialize(b2World* nWorld
                    , sf::Vector2f startPosition
                    , int nScale, int nSize
                    , sf::Texture nTexture
                    , sf::Texture normalTexture
                    , BulletManager* nBulletManager
                    , float nReloadTime
                    , int maxHealth);
    void update(sf::Vector2f relativeMousePointerPos);
    void move(bool up);
    void turn(bool left);
    void setAiming(bool nAiming);
    void shoot(sf::Vector2f relativePointerPosition);
    void setNormal(bool drawNormalMap);
    void hit(int damage);
    int getHealth();
    int getMaxHealth();
private:
    bool shooting;
    sf::Vector2f relativeMousePointerPos;

    int maxHealth, currentHealth;
    
    float reloadTime, timeSinceLastShot;
    sf::Clock shooterTimer;
    CollideData myCollideData;
    bool aiming, aimingLastFrame;
    Movement movement;
    float movementForce;
    int scale, size;
    float framesRunning;
    sf::Texture texMap, normalTexMap;
    sf::Sprite sprite;
    sf::IntRect currentRec;
    BulletManager* bulletManager;
    b2Body* body;
    b2World* world;
    const static float toDegreesMultiple = 57.2957795131;

    virtual void draw(sf::RenderTarget& target
                      , sf::RenderStates states) const;
    b2Vec2 rotateVec(b2Vec2 vector, float radians);
    sf::Vector2f rotateVec(sf::Vector2f vector, float radians);
    void shootBullet(sf::Vector2f relativePointerPosition);
};

class MousePointer {
public:
    MousePointer();
    void setTexture(sf::Texture newPointerTexture);
    void update(sf::RenderWindow* window
                , sf::View* view
                , sf::Vector2f playerPos);
    void draw(sf::RenderWindow* window
              , sf::View view);
    void setAiming(bool nAiming);
    sf::Vector2f getRelativePosition();
private:
    bool aiming;
    int viewMovingDistance;
    sf::Texture pointerTexture;
    sf::Sprite pointerSprite;
    sf::Vector2i mousePosition
        , mousePositionFromCenter;
    sf::Vector2f viewMoved
        , targetViewMoved;
    float currentRotation
        , targetRotation;
};
