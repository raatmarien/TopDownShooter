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
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


struct Light {
    sf::Color color;
    sf::FloatRect rect;
    sf::Vector2f center;
    sf::Vector3f falloff;
    int lightNum;
    float height;
};

class LightManager {
public:
    void initialize(const char* lightMapFilePath
                    , int tileWidth
                    , int tileHeight);
    void draw(sf::RenderTexture* diffuse 
              , sf::RenderTexture* normal
              , sf::RenderWindow* window
              , sf::View currentView);
    void setScreenSize(int x, int y);
    int addLight(Light light);
    Light* getLight(int lightNum);
    void removeLight(int lightNum);
private:
    int currentLightNum;
    sf::Shader lightShader
        , lightMultiplierShader;
    std::vector<Light*> mapLights;
    std::vector<Light> lights;
    sf::Texture onePixTex;
    sf::RectangleShape rect;
    sf::Sprite lightSprite;
    sf::RenderTexture lightTexture;
    sf::Color ambientColor;
};

// Returns null if rectangles don't intersect
sf::FloatRect intersectingRectangle(sf::FloatRect rect1
                                     , sf::FloatRect rect2);
