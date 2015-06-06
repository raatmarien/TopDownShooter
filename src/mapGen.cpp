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

#include <mapGen.h>
#include <vector>
#include <iostream>
#include <math.h>

using namespace sf;

Map generateSimpleMap(MapSettings settings) {
    Map map;
    Image mapImage, lightMapImage;
    map.mapImage = mapImage;
    map.lightMapImage = lightMapImage;

    (map.mapImage).create(settings.mapSize.x, settings.mapSize.y
                          , settings.emptyColor);
    (map.lightMapImage).create(settings.mapSize.x, settings.mapSize.y
                               , Color::Black);

    std::vector<Room> rooms;

    generateRooms(&(map.mapImage), &settings, &rooms);

    populateRooms(&rooms, &map, &settings);
    
    lightRooms(&(map.lightMapImage), &rooms, &settings);

    generateCorridors(&(map.mapImage), &settings, &rooms);

    IntRect startRoom= rooms[0].rect;
    Vector2f startPosition =
        Vector2f(startRoom.left
                 + ((float) (startRoom.width) / 2.0f)
                 , startRoom.top
                 + ((float) (startRoom.height) / 2.0f));
    map.playerStartPosition = startPosition;

    return map;
}

void generateRooms(Image *map, MapSettings *settings
                   , std::vector<Room>* rooms) {
    int attempts = 0;
    IntRect canvas = IntRect(0, 0, settings->mapSize.x
                             , settings->mapSize.y);
    while(attempts <= settings->roomPlacementAttempts) {
        Vector2i position;
        position.x = rand() % (settings->mapSize.x + 1);
        position.y = rand() % (settings->mapSize.y + 1);
        IntRect room = randomRoom(position, settings);
        if (contains(canvas, room)) {
            bool doesOverLap = false;
            for (int i = 0; i < rooms->size(); i++) {
                if (room.intersects(rooms->at(i).rect)) {
                    doesOverLap = true;
                    break;
                }
            }
            if (doesOverLap) {
                attempts++;
            } else {
                drawRect(map, room, settings->groundColor);
                Room roomStruct;
                roomStruct.rect = room;
                roomStruct.connected = false;
                rooms->push_back(roomStruct);
            }
        }
    }
}

void generateCorridors(Image *map, MapSettings *settings
                       , std::vector<Room> *rooms) {
    for (int i = 0; i < rooms->size() - 1; i++) {
        rooms->at(i).connected = true;
        IntRect startRoomRect = rooms->at(i).rect;
        Vector2f startRoomCenter =
            Vector2f(startRoomRect.left
                     + ((float) (startRoomRect.width) / 2.0f)
                     , startRoomRect.top
                     + ((float) (startRoomRect.height) / 2.0f));
        float minDist = map->getSize().x + map->getSize().y;
        int minIndex;
        // Find the closests room that isn't already connected
        for (int j = 0; j < rooms->size(); j++) {
            if (!rooms->at(j).connected) {
                IntRect endRoomRect = rooms->at(j).rect;
                Vector2f endRoomCenter =
                    Vector2f(endRoomRect.left
                             + ((float) (endRoomRect.width) / 2.0f)
                             , endRoomRect.top
                             + ((float) (endRoomRect.height) / 2.0f));
                Vector2f dif = endRoomCenter - startRoomCenter;
                float distance = sqrt(dif.x * dif.x + dif.y * dif.y);
                if (distance < minDist) {
                    minDist = distance;
                    minIndex = j;
                }
            }
        }
        IntRect endRoomRect = rooms->at(minIndex).rect;
        Vector2f endRoomCenter =
            Vector2f(endRoomRect.left
                     + ((float) (endRoomRect.width) / 2.0f)
                     , endRoomRect.top
                     + ((float) (endRoomRect.height) / 2.0f));
        Vector2f distance = endRoomCenter - startRoomCenter;
        bool fromStartDown = distance.y > 0.0f;
        bool fromEndLeft = distance.x > 0.0f;

        int cWidth = settings->corridorWidth;

        // Calculate start point
        // Top left coordinate of the corridor is calculated
        int startPosY = fromStartDown ? startRoomRect.top + startRoomRect.height 
            : startRoomRect.top - 1;
        int startPosX = startRoomRect.left
            + (rand() % (startRoomRect.width - cWidth));

        // Calculate end point
        int endPosX = fromEndLeft ? endRoomRect.left - 1
            : endRoomRect.left + endRoomRect.width;
        int endPosY = endRoomRect.top
            + (rand() % (endRoomRect.height - cWidth));

        IntRect verticalCorridor;
        if (fromStartDown) {
            verticalCorridor = IntRect(startPosX, startPosY - 1, cWidth, endPosY - startPosY + cWidth);
        } else {
            verticalCorridor = IntRect(startPosX, endPosY - 1, cWidth, startPosY - endPosY + cWidth);
        }
        drawRect(map, verticalCorridor, settings->groundColor);
        IntRect horizontalCorridor;
        if (fromEndLeft) {
            horizontalCorridor = IntRect(startPosX - 1, endPosY, endPosX - startPosX + cWidth, cWidth);
        } else {
            horizontalCorridor = IntRect(endPosX - 1, endPosY, startPosX - endPosX + cWidth, cWidth);
        }
        drawRect(map, horizontalCorridor, settings->groundColor);
    }
}

IntRect randomRoom(Vector2i position, MapSettings* settings) {
    int leftWidth = ((2 * settings->baseRoomSize.x) +
                     (rand() % (2 * settings->randomAddRoomSize.x))) / 2;
    int rightWidth = ((2 * settings->baseRoomSize.x) +
                      (rand() % (2 * settings->randomAddRoomSize.x))) / 2;
    int topHeight = ((2 * settings->baseRoomSize.y) +
                     (rand() % (2 * settings->randomAddRoomSize.y))) / 2;
    int bottomHeight = ((2 * settings->baseRoomSize.y) +
                        (rand() % (2 * settings->randomAddRoomSize.y))) / 2;
    return IntRect(position.x - leftWidth
                   , position.y - topHeight
                   , leftWidth + rightWidth
                   , topHeight + bottomHeight);
}

void drawRect(Image* map, IntRect room, Color drawColor) {
    for (int x = room.left; x < (room.left + room.width); x++) {
        for (int y = room.top; y < (room.top + room.height); y++) {
            map->setPixel(x, y, drawColor);
        }
    }
}

bool contains(IntRect rectThatContains, IntRect rectToBeContained) {
    return rectToBeContained.left > rectThatContains.left
        && rectToBeContained.top > rectThatContains.top
        && ((rectToBeContained.left + rectToBeContained.width)
            < (rectThatContains.left + rectThatContains.width))
        && ((rectToBeContained.top + rectToBeContained.height)
            < (rectThatContains.top + rectThatContains.height));
}

// Populating
void populateRooms(std::vector<Room> *rooms, Map *map, MapSettings *settings) {
    for (int i = 0; i < rooms->size(); i++) {
        for (int j = 0; j < settings->enemysPerRoom; j++) {
            Vector2f enemyPosition(rooms->at(i).rect.left
                                   + (rand() % (rooms->at(i).rect.width - 1))
                                   , rooms->at(i).rect.top
                                   + (rand() % (rooms->at(i).rect.height - 1)));
            enemyPosition *= (float) (settings->tileSize);
            map->chargingEnemyPositions.push_back(enemyPosition);
        }
        int boxes = rand() % (settings->maxBoxesPerRoom + 1);
        for (int j = 0; j < boxes; j++) {
            Vector2f boxPosition(rooms->at(i).rect.left
                                   + (rand() % (rooms->at(i).rect.width - 1))
                                   , rooms->at(i).rect.top
                                   + (rand() % (rooms->at(i).rect.height - 1)));
            boxPosition *= (float) (settings->tileSize);
            map->boxPositions.push_back(boxPosition);
        }
    }
}

// Lighting
void lightRooms(sf::Image *lightMap, std::vector<Room>* rooms, MapSettings* settings) {
    for (int i = 0; i < rooms->size(); i++) {
        lightRoom(lightMap, &(rooms->at(i)), settings);
    }
}

void lightRoom(sf::Image *lightMap, Room *room, MapSettings* settings) {
    IntRect roomRect = room->rect;
    float lightsInWidth = (float) (roomRect.width) / (float) (settings->tilesPerLight)
        , lightsInHeight = (float) (roomRect.height) / (float) (settings->tilesPerLight);
    for (int x = 0; x < (lightsInWidth + 1.0f); x++) {
        for (int y = 0; y < (lightsInHeight + 1.0f); y++) {
            int xCoor = roomRect.left + ((float) (roomRect.width) * ((float) (x) / lightsInWidth));
            int yCoor = roomRect.top + ((float) (roomRect.height) * ((float) (y) / lightsInHeight));
            xCoor += (rand() % 3) - 1;
            yCoor += (rand() % 3) - 1;
            int lightVariance = rand() % 50;
            Color testColor = Color(255 - (lightVariance / 2), 120 + (lightVariance * 2), 130 - lightVariance);
            lightMap->setPixel(xCoor, yCoor, testColor * settings->roomLightColor);
        }
    }
}

Image cleanWalls(Image *map, Color wallColor, Color neutralColor) {
    Image newMap;
    newMap.create(map->getSize().x, map->getSize().y, neutralColor);
    newMap.copy(*map, 0, 0);
    for (int x = 0; x < map->getSize().x; x++) {
        for (int y = 0; y < map->getSize().y; y++) {
            if (colorsEqual(map->getPixel(x, y), wallColor)) {
                if (!hasNonWallNeighbour(x, y, map, wallColor)) {
                    newMap.setPixel(x, y, neutralColor);
                }
            }
        }
    }
    return newMap;
}

bool hasNonWallNeighbour(int x, int y, Image *map, Color wallColor) {
    return !(isWall(x - 1, y - 1, map, wallColor)
             && isWall(x, y - 1, map, wallColor)
             && isWall(x + 1, y - 1, map, wallColor)
             && isWall(x + 1, y, map, wallColor)
             && isWall(x + 1, y + 1, map, wallColor)
             && isWall(x, y + 1, map, wallColor)
             && isWall(x - 1, y + 1, map, wallColor)
             && isWall(x - 1, y, map, wallColor));
}

bool isWall(int x, int y, Image *map, Color wallColor) {
    if (x > 0 && x < map->getSize().x
        && y > 0 && y < map->getSize().y) {
        return colorsEqual(map->getPixel(x, y), wallColor);
    } else {
        return true;
    }
}

bool colorsEqual(Color color1, Color color2) {
    return color1.r == color2.r
        && color1.g == color2.g
        && color1.b == color2.b
        && color1.a == color2.a;
}
