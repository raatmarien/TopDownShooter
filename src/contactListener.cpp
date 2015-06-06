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

#include <contactListener.h>
#include <Box2D/Box2D.h>
#include <iostream>

// #define DEBUG_CONTACTS

ContactListener::ContactListener() {

}

void ContactListener::BeginContact(b2Contact* contact) {
    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
    CollideData *collideDataA, *collideDataB;
    Player *player = NULL;
    Enemy *enemy = NULL;
    Bullet *bullet = NULL;
    bool wall = false;
    if (userDataA) { // has userdata
        collideDataA = static_cast<CollideData*> (userDataA);
        if (collideDataA->collideType == COLLIDE_TYPE_PLAYER) {
            player = static_cast<Player*> (collideDataA->user);
        } else if (collideDataA->collideType == COLLIDE_TYPE_WALL) {
            wall = true;
        } else if (collideDataA->collideType == COLLIDE_TYPE_BULLET) {
            bullet = static_cast<Bullet*> (collideDataA->user);
        } else if (collideDataA->collideType == COLLIDE_TYPE_ENEMY) {
            enemy = static_cast<Enemy*> (collideDataA->user);
        } else {
#ifdef DEBUG_CONTACTS
            std::cout << collideDataB->collideType << "\n";
#endif
        }
    }
    if (userDataB) { // has userdata
        collideDataB = static_cast<CollideData*> (userDataB);
        if (collideDataB->collideType == COLLIDE_TYPE_PLAYER) {
            player = static_cast<Player*> (collideDataB->user);
        } else if (collideDataB->collideType == COLLIDE_TYPE_WALL) {
            wall = true;
        } else if (collideDataB->collideType == COLLIDE_TYPE_BULLET) {
            bullet = static_cast<Bullet*> (collideDataB->user);
        } else if (collideDataB->collideType == COLLIDE_TYPE_ENEMY) {
            enemy = static_cast<Enemy*> (collideDataB->user);
        } else {
#ifdef DEBUG_CONTACTS
            std::cout << collideDataB->collideType << "\n";
#endif
        }
    }
#ifdef DEBUG_CONTACTS
    if (player) {
        std::cout << "player ";
    }
    if (enemy) {
        std::cout << "enemy ";
    }
    if (bullet) {
        std::cout << "bullet ";
    }
    if (wall) {
        std::cout << "wall ";
    }
    std::cout << "\n";
#endif

    if (bullet && !bullet->isQueuedForRemoval()) {
        if (enemy) {
            enemy->hit();
            bullet->queueRemoval();
        } else if (player) {
        } else {
            bullet->queueRemoval();
        }
    } else if (enemy) {
        if (player) {
            player->hit(5);
        }
    }
}

void ContactListener::EndContact(b2Contact* contact) {

}
