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

#include "contactListener.h"
#include <Box2D/Box2D.h>
#include <iostream>

ContactListener::ContactListener() {

}

void ContactListener::BeginContact(b2Contact* contact) {
    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
    CollideData *collideDataA, *collideDataB;
    Player *player;
    bool wall;
    if (userDataA) { // has userdata
        collideDataA = static_cast<CollideData*> (userDataA);
        if (collideDataA->collideType == COLLIDE_TYPE_PLAYER) {
            player = static_cast<Player*> (collideDataA->user);
        }
        if (collideDataA->collideType == COLLIDE_TYPE_WALL) {
            wall = true;
        }
    }
    if (userDataB) { // has userdata
        collideDataB = static_cast<CollideData*> (userDataB);
        if (collideDataB->collideType == COLLIDE_TYPE_PLAYER) {
            player = static_cast<Player*> (collideDataB->user);
        }
        if (collideDataB->collideType == COLLIDE_TYPE_WALL) {
            wall = true;
        }
    }
}

void ContactListener::EndContact(b2Contact* contact) {

}
