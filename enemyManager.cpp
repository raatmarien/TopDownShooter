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

#include "enemyManager.h"
#include <iostream>

using namespace sf;

void EnemyManager::initializeChargingEnemys
(std::vector<Vector2f> chargingEnemyPositions
 , Texture texture 
 , float radius
 , float moveForce
 , float rotationTorque
 , int scale
 , Player *player
 , b2World *world) {
    chargingEnemyTexture = texture;
    for (int i = 0; i < chargingEnemyPositions.size(); i++) {
        ChargingEnemy enemy;
        chargingEnemys.push_back(enemy);
        chargingEnemys[i].initialize(&chargingEnemyTexture, radius
                                     , chargingEnemyPositions[i]
                                     , moveForce, rotationTorque
                                     , scale, player, world);
    }
}

void EnemyManager::update() {
    for (int i = 0; i < chargingEnemys.size(); i++) {
        if (chargingEnemys[i].toBeRemoved) {
            chargingEnemys[i].destroy();
            chargingEnemys.erase(i + chargingEnemys.begin());
            i--;
        } else {
            chargingEnemys[i].update();
        }
    }
}

void EnemyManager::draw(RenderTarget *target) {
    for (int i = 0; i < chargingEnemys.size(); i++) {
        target->draw(chargingEnemys[i]);
    }
}
