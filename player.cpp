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

#include "player.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <cmath>

using namespace sf;

void Player::initialize(
    b2World* nWorld
    , Vector2f startPosition
    , int nScale, int nSize
    , Texture nTexture) {
    world = nWorld;
    scale = nScale;
    size = nSize;
    framesRunning = 0;

    // Constants
    moveForce = 20.0f;
    rotationTorque = 2.0f;

    // Setup sprite
    texMap = nTexture;
    sprite.setTexture(texMap);
    sprite.setOrigin(size / 2
                     , size / 2);
    currentRec = IntRect(0,0,size,size);
    sprite.setTextureRect(currentRec);
    setPosition(startPosition);

    movement = NONE;

    // Setup body physics
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(startPosition.x / scale
                         , startPosition.y / scale);
    bodyDef.linearDamping = 4.0f;
    bodyDef.angularDamping = 2.0f;
    body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(((float) (size)) / (2 * scale)
                 , ((float) (size)) / (2 * scale));
    b2CircleShape circle;
    circle.m_radius = ((float) (size)) / (2 * scale);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.0f;

    body->CreateFixture(&fixtureDef);
}

void Player::update() {
    // Update position
    b2Vec2 newPlayerPosition = body->GetPosition();
    setPosition(newPlayerPosition.x * scale
                , newPlayerPosition.y * scale);

    // Update rotation
    b2Vec2 linearVelocity = body->GetLinearVelocity();
    float speed = b2Vec2(linearVelocity.x
                         , linearVelocity.y).Normalize();
    float rotation = std::atan2(linearVelocity.y
                                , linearVelocity.x)
        * toDegreesMultiple;
    setRotation(rotation + 90);

    // Apply movement force
    b2Vec2 moveForceVec = b2Vec2(0,0);
    float diagonalForce
        = sqrt(0.5 * moveForce * moveForce);
    switch (movement) {
    case UP:
        moveForceVec = b2Vec2(0, -moveForce);
        break;
    case DOWN:
        moveForceVec = b2Vec2(0, moveForce);
        break;
    case LEFT:
        moveForceVec = b2Vec2(-moveForce, 0);
        break;
    case RIGHT:
        moveForceVec = b2Vec2(moveForce, 0);
        break;
    case UP_LEFT:
        moveForceVec = b2Vec2(-diagonalForce
                           , -diagonalForce);
        break;
    case UP_RIGHT:
        moveForceVec = b2Vec2(diagonalForce
                           , -diagonalForce);
        break;
    case DOWN_LEFT:
        moveForceVec = b2Vec2(-diagonalForce
                           , diagonalForce);
        break;
    case DOWN_RIGHT:
        moveForceVec = b2Vec2(diagonalForce
                           , diagonalForce);
        break;
    default:
        moveForceVec = b2Vec2(0,0);
        break;
    }

    body->ApplyForce(moveForceVec
                     , body->GetWorldCenter()
                     , true);

    // Animation

    if (speed < 0.5) {
        currentRec = IntRect(0,0,size,size);
    } else {
        if ((int) (framesRunning)
            % 2 == 0) {
            currentRec = IntRect(size,0,size,size);
        } else {
            currentRec = IntRect(2 * size,0,size,size);
        }
        framesRunning += (speed / 80);
    }
    sprite.setTextureRect(currentRec);

    movement = NONE;
}


void Player::move(bool up) {
    if (!movement)
        movement = up ? UP : DOWN;
    else if (movement == LEFT)
        movement = up ? UP_LEFT : DOWN_LEFT;
    else if (movement == RIGHT)
        movement = up ? UP_RIGHT : DOWN_RIGHT;
    else if (movement == UP)
        movement = up ? UP : NONE;
    else if (movement == DOWN)
        movement = up ? NONE : DOWN;
    else if (movement == UP_LEFT)
        movement = up ? UP_LEFT : LEFT;
    else if (movement == DOWN_LEFT)
        movement = up ? LEFT : DOWN_LEFT;
    else if (movement == UP_RIGHT)
        movement = up ? UP_RIGHT : RIGHT;
    else if (movement == DOWN_RIGHT)
        movement = up ? RIGHT : DOWN_RIGHT;
}

void Player::turn(bool left) {
    if (!movement)
        movement = left ? LEFT : RIGHT;
    else if (movement == UP)
        movement = left ? UP_LEFT : UP_RIGHT;
    else if (movement == DOWN)
        movement = left ? DOWN_LEFT : DOWN_RIGHT;
    else if (movement == LEFT)
        movement = left ? LEFT : NONE;
    else if (movement == RIGHT)
        movement = left ? NONE : RIGHT;
    else if (movement == UP_LEFT)
        movement = left ? UP_LEFT : UP;
    else if (movement == DOWN_LEFT)
        movement = left ? DOWN_LEFT : DOWN;
    else if (movement == UP_RIGHT)
        movement = left ? UP : UP_RIGHT;
    else if (movement == DOWN_RIGHT)
        movement = left ? DOWN : DOWN_RIGHT;
}

void Player::draw(RenderTarget& target
                  , RenderStates states) const {
    states.texture = &texMap;
    states.transform *= getTransform();
    target.draw(sprite, states);
}

b2Vec2 Player::rotateVec(b2Vec2 vector, float radians) {
    float x = vector.x * cos(radians)
        - vector.y * sin(radians);
    float y = vector.x * sin(radians)
        + vector.y * cos(radians);
    return b2Vec2(x, y);
}


// MousePointer methods
void MousePointer::setTexture(Texture newPointerTexture) {
    pointerTexture = newPointerTexture;
    pointerSprite.setTexture(pointerTexture);
    pointerSprite.setOrigin(pointerTexture.getSize().x / 2
                            , pointerTexture.getSize().y / 2);
}

void MousePointer::update(RenderWindow* window, View view) {
    mousePosition = Mouse::getPosition(*window);
}

void MousePointer::draw(RenderWindow* window, View view) {
    mousePositionFromCenter = mousePosition
        - Vector2i(view.getSize().x / 2, view.getSize().y / 2);
    Vector2f currentPosFromCenter = Vector2f(mousePositionFromCenter.x
                                             , mousePositionFromCenter.y);
    bool outOfScreenX = abs(mousePositionFromCenter.x)
        > (view.getSize().x / 2)
        , outOfScreenY = abs(mousePositionFromCenter.y)
        > (view.getSize().y / 2);
    if (outOfScreenX || outOfScreenY) {
        currentPosFromCenter /= (float) sqrt(currentPosFromCenter.x
                                             * currentPosFromCenter.x
                                             + currentPosFromCenter.y
                                             * currentPosFromCenter.y);
        float xRatio = view.getSize().x / currentPosFromCenter.x
            , yRatio = view.getSize().y / currentPosFromCenter.y;
        xRatio *= (xRatio < 0) ? -1.0f : 1.0f;
        yRatio *= (yRatio < 0) ? -1.0f : 1.0f;
        currentPosFromCenter *= std::min(xRatio, yRatio) * 0.5f;
    }
    pointerSprite.setPosition( currentPosFromCenter
                              + view.getCenter());
    window->draw(pointerSprite);
}
