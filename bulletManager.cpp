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
#include "bulletManager.h"
#include <iostream>

using namespace sf;


// Bullet methods

Bullet::Bullet(Vector2f position, float radius, b2Vec2 nVelocity
               , b2World* nWorld, int nScale) {
    world = nWorld;
    scale = nScale;
    velocity = nVelocity;

    needsToBeRemoved = false;

    // Setup basic drawing circle
    bulletCircle.setRadius(radius);
    bulletCircle.setOrigin(radius, radius);
    bulletCircle.setPosition(position);
    bulletCircle.setFillColor(Color::Red);

    // Set up collide data
    myCollideData.collideType = COLLIDE_TYPE_BULLET;
    myCollideData.user = this;

    // Set up body
    b2BodyDef bulletBodyDef;
    bulletBodyDef.type = b2_dynamicBody;
    bulletBodyDef.position.Set(position.x / (float) (scale)
                               , position.y / (float) (scale));
    bulletBodyDef.linearDamping = 0.0f;
    bulletBodyDef.angularDamping = 0.0f;
    // bulletBodyDef.isBullet = true;
    bulletBodyDef.userData = &myCollideData;
    
    bulletBody = world->CreateBody(&bulletBodyDef);

    b2CircleShape bulletCircleShape;
    bulletCircleShape.m_radius = radius / (float) (scale);

    b2FixtureDef bulletFixtureDef;
    bulletFixtureDef.shape = &bulletCircleShape;
    bulletFixtureDef.density = 1.0f;
    bulletFixtureDef.friction = 1.0f;
    // bulletFixtureDef.isSensor = true; // Should this be true?

    bulletBody->CreateFixture(&bulletFixtureDef);
    bulletBody->SetLinearVelocity(velocity);
    bulletBody->SetUserData(&myCollideData);
}

void Bullet::update() {
    myCollideData.user = this;
    bulletBody->SetUserData(&myCollideData);
    b2Vec2 position = bulletBody->GetPosition();
    bulletCircle.setPosition(position.x * scale
                             , position.y * scale);
}

void Bullet::draw(RenderTarget& target
                  , RenderStates states) const {
    target.draw(bulletCircle);//, states);
}

    void Bullet::queueRemoval() {
    needsToBeRemoved = true;
}

bool Bullet::isQueuedForRemoval() {
    return needsToBeRemoved;
}

void Bullet::destroyBody() {
    world->DestroyBody(bulletBody);
}

// BulletManager methods

BulletManager::BulletManager() {

}

void BulletManager::initialize(b2World *nWorld, int nScale
                               , float nBulletSpeed
                               , float nBulletRadius) {
    world = nWorld;
    scale = nScale;
    bulletSpeed = nBulletSpeed;
    bulletRadius = nBulletRadius;
}

void BulletManager::update() {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].update();
        if (bullets[i].isQueuedForRemoval()) {
            bullets[i].destroyBody();
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

void BulletManager::drawDiffuse(RenderTarget* diffuseTarget) {
    for (int i = 0; i < bullets.size(); i++) {
        diffuseTarget->draw(bullets[i]);
    }
}


void BulletManager::addBullet(Vector2f startPosition
               , Vector2f normalizedDirection) {
    b2Vec2 velocity = b2Vec2((normalizedDirection.x * bulletSpeed)
                             / (float) (scale)
                             , (normalizedDirection.y * bulletSpeed)
                             / (float) (scale));
    Bullet newBullet(startPosition, bulletRadius, velocity
                     , world, scale);
    bullets.push_back(newBullet);
}
