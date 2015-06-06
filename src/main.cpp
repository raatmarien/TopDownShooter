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
#include <SFML/Network.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <string>
#include <iostream>

#include <mapGen.h>
#include <groundTileMap.h>
#include <player.h>
#include <enemy.h>
#include <updatableManager.h>
#include <bulletManager.h>
#include <shadow.h>
#include <minimap.h>
#include <light.h>
#include <contactListener.h>
#include <healthBar.h>

#define SCALE 50
using namespace sf;

void handleEvents(RenderWindow* window);
void simulatePhysics(RenderWindow* window);
void handleInput(RenderWindow* window);
void update(RenderWindow* window);
void updateDrawables(RenderWindow* window);
void draw(RenderWindow* window);

void loadFiles();
void setupConnections();

Texture spritesMap, playerSprite, mousePointerTexture
                                       , normalTiles
                                       , playerNormal
                                       , enemy1Diffuse
                                       , enemy1Normal
                                       , boxTexture
                                       , boxTextureNormal;

Image tileMapImage, lightMapImage;

groundTileMap tileMap;
int tileSize = 32;

Map map;

b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);
Vector2f startPosition = Vector2f(350
                                 ,400);
Player player;

UpdatableManager updatableManager;

BulletManager bulletManager;

ShadowHandler shadowHandler;

Minimap minimap;
float minimapPadding = 50.0f;

HealthBar healthBar;

LightManager lightManager;

MousePointer mousePointer;

View playerView;

RenderTexture diffuseTarget, normalTarget;

// Shaders
Shader normalRotationShader, tileMapRotationShader;

ContactListener mainContactListener;

float box2DTimeStep = 1.0f / 60.0f;
int velocityIterations = 8
    , positionIterations = 3;

Clock timer, deltaTimer;

int screenX = 50 * 20, screenY = 50 * 20;

int mouseLightNum;

bool mouseLightOn = false;

Clock tKeyTimer;
float timeSincePressT;

int main() {
    // Test
    srand(time(NULL));
    MapSettings testMapSettings;
    testMapSettings.roomPlacementAttempts = 100;
    testMapSettings.corridorWidth = 6;
    testMapSettings.tilesPerLight = 15;
    testMapSettings.enemysPerRoom = 3;
    testMapSettings.maxBoxesPerRoom = 5;
    testMapSettings.tileSize = tileSize;
    testMapSettings.mapSize = Vector2i(270, 270);
    testMapSettings.baseRoomSize = Vector2i(7, 7);
    testMapSettings.randomAddRoomSize = Vector2i(15, 15);
    testMapSettings.emptyColor = Color::White;
    testMapSettings.groundColor = Color::Black;
    testMapSettings.roomLightColor = Color::White;
    map = generateSimpleMap(testMapSettings);
    startPosition = map.playerStartPosition * (float) (tileSize);
    Image procedurallyGeneratedLightMap = map.lightMapImage;

    setupConnections();

    Image cleanedTestMap = cleanWalls(&(map.mapImage), testMapSettings.emptyColor
                                      , testMapSettings.groundColor);
    cleanedTestMap.saveToFile("testMap.png");
    
    RenderWindow window(VideoMode(screenX, screenY), "Top Down Shooter");
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    playerView.setSize(screenX, screenY);

    // Set up RenderTargets
    diffuseTarget.create(screenX, screenY);
    normalTarget.create(screenX, screenY);

    loadFiles();

    // Set up world
    world.SetContactListener(&mainContactListener);
    
    // Set up Tilemap
    tileMap.genGroundTileMap(&cleanedTestMap//tileMapImage
                             , tileSize, tileSize
                             , 4, &world, SCALE);
    tileMap.setShader(&tileMapRotationShader);

    // Set up LightManager
    lightManager.initialize(&procedurallyGeneratedLightMap
                            , tileSize
                            , tileSize);

    // Set up BulletManager
    bulletManager.initialize(&world, SCALE, 3200.0f, 4.0f, &lightManager); 

    // Set up Player
    player.initialize(&world, startPosition, SCALE, 40, playerSprite
                      , playerNormal, &bulletManager, 0.3f, 100);

    // Set up EnemyManager
    updatableManager.initialize(&normalRotationShader);
    updatableManager.initializeChargingEnemys(map.chargingEnemyPositions, enemy1Diffuse
                                              , enemy1Normal
                                          , 12, 3.5f, 0.6f, SCALE, tileSize, &player
                                          , &world, tileMap.getObstacles());
    updatableManager.initializeBoxes(map.boxPositions, boxTexture, boxTextureNormal
                                     , Vector2f(50,30)
                                     , &world, SCALE);
    
    // Set up ShadowHandler
    shadowHandler.setObstacles(tileMap.getObstacles(), tileSize);
    shadowHandler.setScreenDiagonal(screenX, screenY);

    // Set up Minimap
    minimap.setWalls(tileMap.getObstacles());
    minimap.setPositionFromCenter(
        Vector2f((0.5f * screenX) - minimapPadding
                 , (-0.5f * screenY) + minimapPadding));
    minimap.setTileSize(tileSize);
    minimap.setScreenSize(screenX, screenY);
    minimap.setScale(0.75f);

    // Set up HealthBar
    healthBar.initialize(Vector2f(200,16), &player);

    // Set up MousePointer
    mousePointer.setTexture(mousePointerTexture);

    // test
    Vector3f falloff;
    falloff.x = 3.0f; // Constant falloff
    falloff.y = 0.003f; // Linear falloff
    falloff.z = 0.00004f; // Quadratic falloff 
    int size = 2000;
    float lightHeight = 100.0f;
    Light mouseLight;
    mouseLight.color = Color(255,255,255);
    mouseLight.center = Vector2f(100,100);
    mouseLight.rect = FloatRect(0, 0 , size, size);
    mouseLight.height = lightHeight;
    mouseLight.falloff = falloff;
    mouseLightNum = lightManager.addLight(mouseLight);

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
            handleInput(&window);
            update(&window);
            simulatePhysics(&window);
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
            lightManager.getLight(mouseLightNum)->height
                *= (1.0f + 0.1f * event.mouseWheel.delta);
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
    updatableManager.update();
    bulletManager.update();
    minimap.setPlayerPosition(player.getPosition());
    minimap.update();
    mousePointer.update(window, &playerView, player.getPosition());
}

void updateDrawables(RenderWindow* window) {
    shadowHandler.update(player.getPosition()
                         , playerView.getCenter());
    minimap.setViewCenter(playerView.getCenter());
    healthBar.update();
    healthBar.setPosition(Vector2f(10, 10) + playerView.getCenter() - (Vector2f(screenX, screenY) / 2.0f));
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
        Screen.saveToFile("screenshot.png");
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

    if (Mouse::isButtonPressed(Mouse::Left)) {
        player.shoot(mousePointer.getRelativePosition());
    }
    
    Light* pMouseLight = lightManager.getLight(mouseLightNum);
    pMouseLight->center = Vector2f(Mouse::getPosition(*window).x
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
    tileMap.setNormalDrawing(false);
    diffuseTarget.draw(tileMap, &spritesMap);
    bulletManager.drawDiffuse(&diffuseTarget);
    player.setNormal(false);
    diffuseTarget.draw(player, &playerSprite);
    updatableManager.setNormalDraw(false);
    updatableManager.draw(&diffuseTarget);
    diffuseTarget.display();

    // Normal drawing
    normalTarget.setView(playerView);
    normalTarget.clear(sf::Color(0,0,0));
    tileMap.setNormalDrawing(true);
    normalTarget.draw(tileMap, &normalTiles);
    player.setNormal(true);
    normalRotationShader.setParameter("angle"
                                      , player.getRotation());
    normalTarget.draw(player, &normalRotationShader);
    updatableManager.setNormalDraw(true);
    updatableManager.draw(&normalTarget);
    normalTarget.display();

    // Draw diffuse combined with lighting to window
    window->setView(playerView);
    window->clear(sf::Color(0,0,0));
    lightManager.draw(&diffuseTarget, &normalTarget
                      , window, playerView);

    shadowHandler.draw(window);
    mousePointer.draw(window, playerView);
    minimap.draw(window);
    window->draw(healthBar);
    window->display();
}

void loadFiles() {
    spritesMap.loadFromFile("sprites/spriteMap5.png");
    normalTiles.loadFromFile("normalmaps/tilesNormal3.png");
    playerSprite.loadFromFile("sprites/player_gray.png");
    playerNormal.loadFromFile("normalmaps/playerNormal3.png");
    enemy1Diffuse.loadFromFile("sprites/enemy1.png");
    enemy1Normal.loadFromFile("normalmaps/enemy1Normal2.png");
    boxTexture.loadFromFile("sprites/box1.png");
    boxTextureNormal.loadFromFile("normalmaps/box1Normal2.png");
    tileMapImage.loadFromFile("maps/chambers_map.png");
    lightMapImage.loadFromFile("maps/light_map.png");
    mousePointerTexture.loadFromFile("sprites/mousePointer.png");
    normalRotationShader.loadFromFile("shaders/rotateNormalBitmap.frag"
                                      , Shader::Fragment);
    tileMapRotationShader.loadFromFile("shaders/tileMapRotationShader.frag"
                                      , Shader::Fragment);
}

void setupConnections() {
    std::cout << "Type anything to play single player\n"
              << "Type 'o' to connect with another player and play on the same map\n";
    char onlineOrSingle;
    std::cin >> onlineOrSingle;
    if (onlineOrSingle != 'o')
        return;
    std::cout << "Type s to be the server.\nType c to be the client.\n"
              << "Or type anything else to continue normally\n";
    char socketType;
    std::cin >> socketType;
    if (socketType == 's') {
        std::cout << "Setting up server...\n";
        TcpListener serverListener;
        if (serverListener.listen(Socket::AnyPort) != Socket::Done) {
            std::cout << "Creating a listener failed\n";
            return;
        }
        
        std::cout << "Connect to port " << serverListener.getLocalPort()
                  << " with the client.\n";
        std::cout << "Waiting for client...\n";
        TcpSocket serverSocket;
        if (serverListener.accept(serverSocket) != Socket::Done) {
            std::cout << "Accepting a client failed\n";
            return;
        }
        std::cout << "Connected with client!\n";
        sf::Packet receivedMapPack;
        serverSocket.receive(receivedMapPack);
        
        Image *mapImage = &(map.mapImage);
        Int32 mapSizeX, mapSizeY;
        receivedMapPack >> mapSizeX >> mapSizeY;
        for (int y = 0; y < mapSizeY; y++) {
            for (int x = 0; x < mapSizeX; x++) {
                Int8 red;
                receivedMapPack >> red;
                mapImage->setPixel(x, y, Color(red, red, red));
            }
        }
        Int32 startPosX, startPosY;
        receivedMapPack >> startPosX >> startPosY;
        map.playerStartPosition = Vector2f(startPosX, startPosY);
    } else if (socketType == 'c') {
        std::cout << "Input the servers IP adress:\n";
        std::string serverIP;
        std::cin >> serverIP;
        std::cout << "Input the port to connect to:\n";
        int port;
        std::cin >> port;

        TcpSocket clientSocket;
        if (clientSocket.connect(serverIP, port) != Socket::Done) {
            std::cout << "Connecting to the server failed\n";
            return;
        }
        std::cout << "Succesfully connected to the server.\n";

        Packet mapPack;
        Image *mapImage = &(map.mapImage);
        Vector2u mapSize = mapImage->getSize();
        Int32 mapSizeX = mapSize.x, mapSizeY = mapSize.y;
        mapPack << mapSizeX << mapSizeY;
        for (int y = 0; y < mapSize.y; y++) {
            for (int x = 0; x < mapSize.x; x++) {
                Int8 red = mapImage->getPixel(x, y).r;
                mapPack << red;
            }
        }
        Int32 startPosX = map.playerStartPosition.x
            , startPosY = map.playerStartPosition.y;
        mapPack << startPosX << startPosY;
        clientSocket.send(mapPack);
    }
}
