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
#include <Box2D/Box2D.h>
#include "groundTileMap.h"
#include "player.h"
#include "shadow.h"
#include <vector>
#include <math.h>
#include <iostream>

#define SCALE 50

using namespace sf;

void handleEvents(RenderWindow* window);
void simulatePhysics(RenderWindow* window);
void handleInput(RenderWindow* window);
void update(RenderWindow* window);
void draw(RenderWindow* window);

void loadSprites();

Texture spritesMap, playerSprite;
groundTileMap tileMap;

b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);
Vector2f startPosition = Vector2f(150
                                 ,200);
Player player;

ShadowHandler shadowHandler;

View playerView;

Clock deltaTimer;

float box2DTimeStep = 1.0f / 60.0f;
int velocityIterations = 8
    , positionIterations = 3;

int screenX = 50 * 20, screenY = 50 * 20;

int main() {
    RenderWindow window(VideoMode(screenX, screenY), "Float");
    window.setVerticalSyncEnabled(true);
    playerView.setSize(screenX, screenY);

    loadSprites();

    const char* mapFilePath = "maps/chambers_map.pgm";
    
    tileMap.genGroundTileMap(mapFilePath, spritesMap
                             , 25, 25, 4, &world, SCALE);
    player.initialize(&world, startPosition, SCALE
                      , 40, playerSprite);
    shadowHandler.genObstaclePoints(mapFilePath, 25);
    shadowHandler.setScreenDiagonal(screenX, screenY);

    while(window.isOpen()) {
        std::cout << (1.0f / deltaTimer.restart().asSeconds())
            << std::endl;
        handleEvents(&window);
        simulatePhysics(&window);
        handleInput(&window);
        update(&window);
        draw(&window);
    }
}

void handleEvents(RenderWindow* window) {
    Event event;
    while(window->pollEvent(event)) {
        if (event.type == Event::Closed) {
            window->close();
        }
        if (event.type == Event::Resized) {
            screenX = event.size.width;
            screenY = event.size.height;
            playerView.setSize(screenX
                               , screenY);
            shadowHandler.setScreenDiagonal(screenX
                                            , screenY);
        }
    }
}

void simulatePhysics(RenderWindow* window) {
    world.Step(box2DTimeStep, velocityIterations
               , positionIterations);
    world.ClearForces();
}

void update(RenderWindow* window) {
    player.update();
    playerView.setCenter(player.getPosition());
    shadowHandler.update(player.getPosition());
}

void handleInput(RenderWindow* window) {
    if (Keyboard::isKeyPressed(Keyboard::W)
        || Keyboard::isKeyPressed(Keyboard::Up)) {
        player.move(true);
    }
    if (Keyboard::isKeyPressed(Keyboard::S)
        || Keyboard::isKeyPressed(Keyboard::Down)) {
        player.move(false);
    }
    if (Keyboard::isKeyPressed(Keyboard::A)
        || Keyboard::isKeyPressed(Keyboard::Left)) {
        player.turn(true);
    }
    if (Keyboard::isKeyPressed(Keyboard::D)
        || Keyboard::isKeyPressed(Keyboard::Right)) {
        player.turn(false);
    }
}

void draw(RenderWindow* window) {
    window->setView(playerView);
    window->clear(sf::Color(0,0,0));
    window->draw(tileMap);
    window->draw(player);
    shadowHandler.draw(window);
    window->display();
}

void loadSprites() {
    spritesMap.loadFromFile("sprites/spriteMap.png");
    playerSprite.loadFromFile("sprites/player.png");
}
