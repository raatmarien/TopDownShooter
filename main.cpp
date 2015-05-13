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
#include "minimap.h"
#include "light.h"
#include <vector>
#include <math.h>
#include <iostream>

#define SCALE 50

using namespace sf;

void handleEvents(RenderWindow* window);
void simulatePhysics(RenderWindow* window);
void handleInput(RenderWindow* window);
void update(RenderWindow* window);
void updateDrawables(RenderWindow* window);
void draw(RenderWindow* window);

void loadSprites();

Texture spritesMap, playerSprite, mousePointerTexture;
groundTileMap tileMap;
int tileSize = 25;

b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);
Vector2f startPosition = Vector2f(150
                                 ,200);
Player player;

ShadowHandler shadowHandler;

Minimap minimap;
float minimapPadding = 50.0f;

LightManager lightManager;

MousePointer mousePointer;

View playerView;

float box2DTimeStep = 1.0f / 60.0f;
int velocityIterations = 8
    , positionIterations = 3;

Clock timer, deltaTimer;

int screenX = 50 * 20, screenY = 50 * 20;

int main() {
    RenderWindow window(VideoMode(screenX, screenY), "Top Down Shooter");
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    playerView.setSize(screenX, screenY);

    loadSprites();

    const char* mapFilePath = "maps/chambers_map3.pgm";
    
    // Set up Tilemap
    tileMap.genGroundTileMap(mapFilePath, spritesMap
                             , tileSize, tileSize
                             , 4, &world, SCALE);

    // Set up Player
    player.initialize(&world, startPosition, SCALE
                      , 40, playerSprite);

    // Set up ShadowHandler
    std::vector<Vector2f> walls = shadowHandler.genObstaclePoints(mapFilePath, tileSize);
    shadowHandler.setScreenDiagonal(screenX, screenY);

    // Set up Minimap
    minimap.setWalls(walls);
    minimap.setPositionFromCenter(
        Vector2f((0.5f * screenX) - minimapPadding
                 , (-0.5f * screenY) + minimapPadding));
    minimap.setTileSize(tileSize);
    minimap.setScreenSize(screenX, screenY);

    // Set up LightManager
    lightManager.initialize("maps/light_map3.ppm"
                            , tileSize
                            , tileSize);

    // Set up MousePointer
    mousePointer.setTexture(mousePointerTexture);
    
    float timeLeft = 0;

    std::cout << "Configuration time: "
              << timer.restart().asSeconds()
              << std::endl;
    int frames = -1;
    while(window.isOpen()) {
        if (frames == 60) {
            float time = timer.restart().asSeconds();
            std::cout <<  1.0f / (time / 60.0f) << "\n";
            frames = 0;
        }
        frames++;

        timeLeft += deltaTimer.restart().asSeconds();
        int i = 0, maxUpdatesInFrame = 2;
        do {
            handleEvents(&window);
            simulatePhysics(&window);
            handleInput(&window);
            update(&window);
            timeLeft -= box2DTimeStep;
            if (timeLeft < 0)
                timeLeft = 0;
            i++;
        } while(timeLeft > box2DTimeStep && i < maxUpdatesInFrame);
        if (i == maxUpdatesInFrame) timeLeft = 0;
        updateDrawables(&window);
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
            minimap.setPositionFromCenter(
                Vector2f((0.5f * screenX) - minimapPadding
                         - minimap.getSize().x
                         , (-0.5f * screenY) + minimapPadding));
            minimap.setScreenSize(screenX, screenY);
            lightManager.setScreenSize(screenX, screenY);
        }
    }
}

void simulatePhysics(RenderWindow* window) {
    world.Step(box2DTimeStep, velocityIterations
               , positionIterations);
    world.ClearForces();
}

void update(RenderWindow* window) {
    player.update(mousePointer.getRelativePosition());
    minimap.setPlayerPosition(player.getPosition());
    minimap.update();
    mousePointer.update(window, &playerView, player.getPosition());
}

void updateDrawables(RenderWindow* window) {
    shadowHandler.update(player.getPosition()
                         , playerView.getCenter());
    minimap.setViewCenter(playerView.getCenter());
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

    // Capture a screenshot with F1
    // For debugging purposes
    if (Keyboard::isKeyPressed(Keyboard::F1))
    {
        Image Screen = window->capture();
        Screen.saveToFile("screenshot.jpg");
    }

    // Mouse input
    if (Mouse::isButtonPressed(Mouse::Right)) {
        mousePointer.setAiming(true);
        player.setAiming(true);
    }
}

void draw(RenderWindow* window) {
    window->setView(playerView);
    window->clear(sf::Color(0,0,0));
    window->draw(tileMap);
    window->draw(player);
    lightManager.draw(window, playerView);
    shadowHandler.draw(window);
    mousePointer.draw(window, playerView);
    minimap.draw(window);
    window->display();
}

void loadSprites() {
    spritesMap.loadFromFile("sprites/spriteMap3.png");
    playerSprite.loadFromFile("sprites/player.png");
    mousePointerTexture.loadFromFile("sprites/mousePointer.png");
}
