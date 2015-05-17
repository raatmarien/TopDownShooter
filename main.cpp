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

void loadFiles();

Texture spritesMap, playerSprite, mousePointerTexture
                                       , normalTiles
                                       , playerNormal;
groundTileMap tileMap;
int tileSize = 32;

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

RenderTexture diffuseTarget, normalTarget;

// Shaders
Shader normalRotationShader;

float box2DTimeStep = 1.0f / 60.0f;
int velocityIterations = 8
    , positionIterations = 3;

Clock timer, deltaTimer;

int screenX = 50 * 20, screenY = 50 * 20;

// test
Light mouseLight, *pMouseLight;

bool mouseLightOn = true;

Clock tKeyTimer;
float timeSincePressT;

int main() {
    RenderWindow window(VideoMode(screenX, screenY), "Top Down Shooter");
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    playerView.setSize(screenX, screenY);

    // Set up RenderTargets
    diffuseTarget.create(screenX, screenY);
    normalTarget.create(screenX, screenY);

    loadFiles();

    const char* mapFilePath = "maps/chambers_map3.pgm";
    
    // Set up Tilemap
    tileMap.genGroundTileMap(mapFilePath
                             , tileSize, tileSize
                             , 4, &world, SCALE);

    // Set up Player
    player.initialize(&world, startPosition, SCALE
                      , 40, playerSprite, playerNormal);

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

    // test
    Vector3f falloff;
    falloff.x = 3.0f; // Constant falloff
    falloff.y = 0.003f; // Linear falloff
    falloff.z = 0.00004f; // Quadratic falloff 
    int size = 2000;
    float lightHeight = 100.0f;
    mouseLight.color = Color(255,255,255);
    mouseLight.center = Vector2f(100,100);
    mouseLight.rect = FloatRect(0
                                , 0 
                                , size, size);
    mouseLight.height = lightHeight;
    mouseLight.falloff = falloff;
    pMouseLight = lightManager.addLight(mouseLight);

    float timeLeft = 0;

    std::cout << "Configuration time: "
              << timer.restart().asSeconds()
              << std::endl;
    int frames = -1;
    while(window.isOpen()) {
        if (frames == 60) {
            float time = timer.restart().asSeconds();
            std::cout << "FPS: " << 1.0f / (time / 60.0f) << "\n";
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
            std::cout << "New screen size: "
                << screenX << " " << screenY << "\n";
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

            // RenderTextures
            diffuseTarget.create(screenX, screenY);
            normalTarget.create(screenX, screenY);
        }
        if (event.type == Event::MouseWheelMoved) {
            pMouseLight->height *= (1.0f + 0.1f * event.mouseWheel.delta);
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

    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window->close();
    }
    if (Keyboard::isKeyPressed(Keyboard::T)
        && (timeSincePressT += tKeyTimer.restart().asSeconds()) > 0.4f) {
        mouseLightOn = !mouseLightOn;
        timeSincePressT = 0;
    }

    // Mouse input
    if (Mouse::isButtonPressed(Mouse::Right)) {
        mousePointer.setAiming(true);
        player.setAiming(true);
    }
    (*pMouseLight).center = Vector2f(Mouse::getPosition(*window).x
                                 , Mouse::getPosition(*window).y)
        + playerView.getCenter() - Vector2f(screenX / 2, screenY / 2);

    if (mouseLightOn) {
        pMouseLight->color = Color::White;
    } else {
        pMouseLight->color = Color::Black;
    }
}

void draw(RenderWindow* window) {
    // Diffuse drawing
    diffuseTarget.setView(playerView);
    diffuseTarget.clear(sf::Color(0,0,0));
    diffuseTarget.draw(tileMap, &spritesMap);
    player.setNormal(false);
    diffuseTarget.draw(player, &playerSprite);
    diffuseTarget.display();

    // Normal drawing
    normalTarget.setView(playerView);
    normalTarget.clear(sf::Color(0,0,0));
    normalTarget.draw(tileMap, &normalTiles);
    player.setNormal(true);
    normalRotationShader.setParameter("angle"
                                      , player.getRotation());
    normalTarget.draw(player, &normalRotationShader);
    normalTarget.display();

    // Draw diffuse combined with lighting to window
    window->setView(playerView);
    window->clear(sf::Color(0,0,0));
    lightManager.draw(&diffuseTarget, &normalTarget
                      , window, playerView);

    shadowHandler.draw(window);
    mousePointer.draw(window, playerView);
    minimap.draw(window);
    window->display();
}

void loadFiles() {
    spritesMap.loadFromFile("sprites/spriteMap5.png");
    normalTiles.loadFromFile("normalmaps/tilesNormal3.png");
    playerSprite.loadFromFile("sprites/player_gray.png");
    playerNormal.loadFromFile("normalmaps/playerNormal2.png");
    mousePointerTexture.loadFromFile("sprites/mousePointer.png");
    normalRotationShader.loadFromFile("shaders/rotateNormalBitmap.frag"
                                      , Shader::Fragment);
}
