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

class Light {
public:
    Light(int radius, sf::Vector3f falloff
          , sf::Color color, sf::Vector2f position);
    int getRadius();
    sf::Color getColorAt(sf::Vector2f pixelPosition);
    sf::Vector2f getPosition();
private:
    sf::Color centerColor;
    int radius;
    sf::Vector3f falloff;
    sf::Vector2f position;
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
private:
    sf::Shader lightShader
        , lightMultiplierShader;
    std::vector<sf::Sprite> lights;
    sf::Texture standardLightTexture
        , texture;
    sf::Sprite lightSprite;
    sf::RenderTexture lightTexture;
    sf::Color ambientColor;
    // std::vector<Light> lights;
    // sf::Texture drawTexture;
    // sf::Sprite drawSprite;
};

sf::Texture generateLightTexture(int radius
                             , sf::Color centerColor
                             , sf::Vector3f falloff
                             // (constant, linear, quadratic)
    );
