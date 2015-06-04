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

#include "updatableManager.h"
#include <iostream>

using namespace sf;

void UpdatableManager::initializeChargingEnemys
(std::vector<Vector2f> chargingEnemyPositions
 , Texture texture 
 , float radius
 , float moveForce
 , float rotationTorque
 , int scale
 , int tileSize
 , Player *player
 , b2World *world
 , std::vector<Vector2f> wallPoints) {
    this->wallPoints = wallPoints;
    chargingEnemyTexture = texture;
    for (int i = 0; i < chargingEnemyPositions.size(); i++) {
        ChargingEnemy* enemy = new ChargingEnemy;
        enemy->initialize(&chargingEnemyTexture, radius
                                     , chargingEnemyPositions[i]
                                     , moveForce, rotationTorque
                                     , scale, tileSize, player
                                     , world, &(this->wallPoints));
        updatables.push_back(enemy);
    }
}

void UpdatableManager::initializeBoxes(std::vector<Vector2f> boxPositions
                                       , Texture boxTexture
                                       , Vector2f boxSize
                                       , b2World *world
                                       , int scale) {
    this->boxTexture = boxTexture;
    BoxSettings boxSettings;
    boxSettings.texture = &(this->boxTexture);
    boxSettings.size = boxSize;
    boxSettings.density = 1.0f;
    boxSettings.friction = 1.0f;
    boxSettings.scale = scale;
    boxSettings.world = world;
    for (int i = 0; i < boxPositions.size(); i++) {
        boxSettings.position = boxPositions[i];
        Box *box = new Box;
        box->initialize(boxSettings);
        updatables.push_back(box);
    }
}

void UpdatableManager::update() {
    for (int i = 0; i < updatables.size(); i++) {
        if (updatables[i]->queuedForRemoval()) {
            updatables[i]->destroy();
            delete updatables[i];
            updatables.erase(i + updatables.begin());
            i--;
        } else {
            (updatables[i])->update();
        }
    }
}

void UpdatableManager::draw(RenderTarget *target) {
    for (int i = 0; i < updatables.size(); i++) {
        target->draw(*(updatables[i]));
    }
}
