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
#include <SFML/Graphics.hpp>
#include <vector>

struct MapSettings {
    int roomPlacementAttempts;
    int corridorWidth;
    sf::Vector2i mapSize;
    sf::Vector2i baseRoomSize;
    sf::Vector2i randomAddRoomSize;
    sf::Color emptyColor; // Color to draw fill the image
    sf::Color groundColor; // Color to draw the ground
};

struct Map {
    sf::Image mapImage;
    sf::Vector2f playerStartPosition;
};

struct Room {
    sf::IntRect rect;
    bool connected;
};

Map generateSimpleMap(MapSettings settings);

// Helper functions
void generateRooms(sf::Image *map, MapSettings* settings, std::vector<Room>* rooms);
void generateCorridors(sf::Image *map, MapSettings* settings, std::vector<Room>* rooms);
sf::IntRect randomRoom(sf::Vector2i center, MapSettings* settings);
void drawRect(sf::Image* map, sf::IntRect room, sf::Color drawColor);

bool contains(sf::IntRect rectThatContains
              , sf::IntRect rectToBeContained);

sf::Image cleanWalls(sf::Image *map, sf::Color wallColor, sf::Color neutralColor); 

// Helper functions
bool hasNonWallNeighbour(int x, int y, sf::Image *map, sf::Color wallColor);
bool isWall(int x, int y, sf::Image *map, sf::Color wallColor);
bool colorsEqual(sf::Color color1, sf::Color color2);
