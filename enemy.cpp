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

#include "enemy.h"
#include <math.h>
#include <iostream>

#define PI 3.14159265359

using namespace sf;

void ChargingEnemy::initialize(Texture texture
                               , float radius
                               , Vector2f startPosition
                               , float moveForce
                               , float rotationTorque
                               , int scale
                               , Player *player
                               , b2World *world) {
    this->texture = texture;
    this->radius = radius;
    this->moveForce = moveForce;
    this->rotationTorque = rotationTorque;
    this->scale = scale;
    this->player = player;
    this->world = world;

    currentForce = 0.0f;
    currentTorque = 0.0f;

    myCollideData.collideType = COLLIDE_TYPE_ENEMY;
    myCollideData.user = this;

    sprite.setTexture(this->texture);
    sprite.setOrigin(radius, radius);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(startPosition.x / (float) (this->scale)
                         , startPosition.y / (float) (this->scale));
    bodyDef.linearDamping = 4.0f;
    bodyDef.angularDamping = 20.0f;
    bodyDef.userData = &myCollideData;

    body = this->world->CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = ((float) radius) / ((float) (scale));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.0f;

    body->CreateFixture(&fixtureDef);
}

void ChargingEnemy::update() {
    b2Vec2 b2CurrentPosition = body->GetPosition();
    Vector2f currentPosition = Vector2f(b2CurrentPosition.x * scale
                                        , b2CurrentPosition.y * scale);
    setPosition(currentPosition);
    float currentRotation = body->GetAngle();
    setRotation((currentRotation / PI) * 180);

    // Set move commands
    Vector2f playerPosition = player->getPosition();
    Vector2f distanceToPlayer = playerPosition - currentPosition;
    float targetRotation = atan2(distanceToPlayer.x, distanceToPlayer.y);
    targetRotation -= 1 * PI;
    targetRotation = targetRotation < 0 ? 2 * PI + targetRotation : targetRotation;
    targetRotation = 2 * PI - targetRotation;
    float rotDif = targetRotation - currentRotation;
    if (abs(rotDif) > PI) {
        rotDif = rotDif > 0 ? rotDif - (2 * PI) : rotDif + (2 * PI); 
    }
    turn(rotDif < 0);
    move(true);

    b2Vec2 force;
    force.x = -currentForce * sin(currentRotation);
    force.y = currentForce * cos(currentRotation);

    body->ApplyForce(force, body->GetWorldCenter(), true); 
    body->ApplyTorque(currentTorque, true);

    currentForce = 0.0f;
    currentTorque = 0.0f;

    if (hittimer > 0) {
        sprite.setColor(Color::Red);
        hittimer--;
    } else {
        sprite.setColor(Color::White);
    }
}

void ChargingEnemy::hit() {
    std::cout << "Enemy hit!\n";
    isHit = true;
    hittimer = 30;
}

void ChargingEnemy::move(bool forward) {
    currentForce = (forward ? -1.0f : 1.0f) * moveForce;
}

void ChargingEnemy::turn(bool left) {
    currentTorque = (left ? -1.0f : 1.0f) * rotationTorque;
}

void ChargingEnemy::draw(RenderTarget &target, RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}
