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

#include <enemy.h>
#include <lineIntersections.h>
#include <math.h>
#include <iostream>

#define PI 3.14159265359

using namespace sf;

void ChargingEnemy::initialize(Texture *diffuseTexture
                               , Texture *normalTexture
                               , float radius
                               , Vector2f startPosition
                               , float moveForce
                               , float rotationTorque
                               , int scale
                               , int tileSize
                               , Player *player
                               , b2World *world
                               , std::vector<Vector2f>* wallPoints) {
    this->diffuseTexture = diffuseTexture;
    this->normalTexture = normalTexture;
    this->radius = radius;
    this->moveForce = moveForce;
    this->rotationTorque = rotationTorque;
    this->scale = scale;
    this->tileSize = tileSize;
    this->player = player;
    this->world = world;
    this->visionRadius = 1600;
    this->wallPoints = wallPoints;

    currentForce = 0.0f;
    currentTorque = 0.0f;
    toBeRemoved = false;

    state = CHARGING_ENEMY_WAITING;

    myCollideData.collideType = COLLIDE_TYPE_ENEMY;
    myCollideData.user = this;

    sprite.setTexture(*(this->diffuseTexture));
    sprite.setOrigin(radius, radius);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(startPosition.x / (float) (this->scale)
                         , startPosition.y / (float) (this->scale));
    bodyDef.linearDamping = 2.0f;
    bodyDef.angularDamping = 20.0f;
    bodyDef.userData = &myCollideData;

    body = this->world->CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = ((float) radius) / ((float) (scale));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
}

void ChargingEnemy::update() {
    myCollideData.user = this;
    body->SetUserData(&myCollideData);

    b2Vec2 b2CurrentPosition = body->GetPosition();
    Vector2f currentPosition = Vector2f(b2CurrentPosition.x * scale
                                        , b2CurrentPosition.y * scale);
    setPosition(currentPosition);
    float currentRotation = body->GetAngle();
    setRotation((currentRotation / PI) * 180);

    // Update states
    Vector2f playerPosition = player->getPosition();
    Vector2f distanceToPlayer = playerPosition - currentPosition;
    if ((state != CHARGING_ENEMY_CHARGING) &&
        sqrt(distanceToPlayer.x * distanceToPlayer.x
             + distanceToPlayer.y * distanceToPlayer.y) < visionRadius
        && (rand() % 10) == 0) {
        if (isVisible(playerPosition)) {
            state = CHARGING_ENEMY_CHARGING;
        }
    }
    

    // Set move commands
    switch (state) {
    case CHARGING_ENEMY_WAITING:
        
        break;

    case CHARGING_ENEMY_CHARGING:
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
        break;
    }

    b2Vec2 force;
    force.x = -currentForce * sin(currentRotation);
    force.y = currentForce * cos(currentRotation);

    body->ApplyForce(force, body->GetWorldCenter(), true); 
    body->ApplyTorque(currentTorque, true);

    currentForce = 0.0f;
    currentTorque = 0.0f;
}

void ChargingEnemy::hit() {
    toBeRemoved = true;
}

void ChargingEnemy::destroy() {
    world->DestroyBody(body);
}

void ChargingEnemy::setDrawNormal(bool drawNormal) {
    sprite.setTexture(*(drawNormal ? normalTexture : diffuseTexture));
}

bool ChargingEnemy::isVisible(Vector2f position) {
    // Get wall points that are in the visible radius
    Vector2f myPos = getPosition();
    int squareMaxDist = visionRadius * visionRadius;
    std::vector<Vector2f*> wallPointsInVision;
    for (int i = 0; i < wallPoints->size(); i++) {
        Vector2f cornerPos = wallPoints->at(i);
        Vector2f dist = myPos - cornerPos;
        if ((dist.x * dist.x + dist.y * dist.y)
            < squareMaxDist) {
            wallPointsInVision.push_back(&(wallPoints->at(i)));
        }
    }

    for (int i = 0; i < wallPointsInVision.size(); i++) {
        // Corners of the wall block
        //   p1    p2
        //    .___.
        //    |   |
        //    !___!
        //   p4    p3
        Vector2f p1 = *(wallPointsInVision[i])
            , p2 = p1 + Vector2f(tileSize, 0)
            , p3 = p1 + Vector2f(tileSize, tileSize)
            , p4 = p1 + Vector2f(0, tileSize);
        // Check if the vision to the player is cut off
        // by one of the lines of this wall
        if (lineSegmentsIntersect(myPos, position, p1, p2)
            || lineSegmentsIntersect(myPos, position, p2, p3)
            || lineSegmentsIntersect(myPos, position, p3, p4)
            || lineSegmentsIntersect(myPos, position, p4, p1)) {
            return false;
        }
    }

    
    return true;
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
