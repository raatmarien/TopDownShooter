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

#include "light.h"
#include <SFML/Graphics.hpp>
#include <math.h>
#include <fstream>
#include <iostream>

using namespace sf;

Light::Light() {
    
}

void Light::update() {

}

void Light::setTexture(Texture nTexture) {
    texture = nTexture;
    sprite.setTexture(texture);
    // Set origin to the center
    Vector2u size = texture.getSize();
    sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
}

void Light::setObstacles(std::vector<FloatRect> nObstacles) {
    obstacles = nObstacles;
}

void Light::setColor(Color color) {
    sprite.setColor(color);
}

void Light::draw(RenderTarget &target
          , RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &texture;
    states.blendMode = BlendAdd;
    target.draw(sprite, states);
}

void Light::draw(RenderWindow* window) {
    sprite.setPosition(getPosition());
    setTexture(texture);
    window->draw(sprite, BlendAdd);
}

Vector2u Light::getSize() {
    return texture.getSize();
}

// Light manager
void LightManager::initialize(const char* lightMapFilePath
                              , int tileWidth
                              , int tileHeight) {
    Vector3f falloff;
    falloff.x = 3.0f; // Constant falloff
    falloff.y = 0.0001f; // Linear falloff
    falloff.z = 0.00006f; // Quadratic falloff
    lightTexture
        = generateLightTexture(300, Color(255,255,255,255)
                               , 1000, 1000, falloff);
    
    // Read the positions of the lamps from the map
    std::ifstream map(lightMapFilePath);
    char buf[10];
    map >> buf; // Not needed
    int width, height;
    map >> width >> height;
    map >> buf; // Range of grayscale
                // Not needed
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r, g, b;
            map >> r >> g >> b;
            if (!(r == 0 && g == 0
                  && b == 0)) { // Lamp
                Light light;
                light.setTexture(lightTexture);
                light.setColor(Color(r,g,b));
                light.setPosition(x * tileWidth
                                  , y * tileHeight);
                lights.push_back(light);
            }
        }
    }
    
    darkShader.setPrimitiveType(Quads);
    darkShader.resize(4);
    Color shaderColor = Color(0, 0, 0
                              , 127);
    darkShader[0].color = shaderColor;
    darkShader[1].color = shaderColor;
    darkShader[2].color = shaderColor;
    darkShader[3].color = shaderColor;
}

void LightManager::draw(RenderWindow* window
                        , View currentView) {
    Vector2f viewCenter = currentView.getCenter()
        , viewSize = currentView.getSize();
    FloatRect viewRect(viewCenter.x - (viewSize.x / 2.0f)
                       , viewCenter.y - (viewSize.y / 2.0f)
                       , viewSize.x, viewSize.y);
    // Draw a transparent black shader over the
    // current view to darken it
    darkShader[0].position = Vector2f(viewRect.left
                                      , viewRect.top);
    darkShader[1].position = Vector2f(viewRect.left
                                      + viewRect.width
                                      , viewRect.top);
    darkShader[2].position = Vector2f(viewRect.left
                                      + viewRect.width
                                      , viewRect.top
                                      + viewRect.height);
    darkShader[3].position = Vector2f(viewRect.left
                                      , viewRect.top
                                      + viewRect.height);
    window->draw(darkShader);

    for (int i = 0; i < lights.size(); i++) {
        Vector2f lightCenter = lights[i].getPosition()
            , lightSize = Vector2f(lights[i].getSize().x
                                   , lights[i].getSize().y);
        FloatRect lightRect =
            FloatRect(lightCenter.x - (lightSize.x / 2.0f)
                      , lightCenter.y - (lightSize.y / 2.0f)
                      , lightSize.x, lightSize.y);
        if (lightRect.intersects(viewRect)) {
            lights[i].draw(window);
        }
    }
}

Texture generateLightTexture(int radius
                             , Color centerColor
                             , int centerDistance
                             , int height
                             , Vector3f falloff
                             // (constant, linear, quadratic)
    ) {
    Image image;
    image.create(radius * 2, radius * 2
                 , Color(0,0,0,0));
    Vector2f center = Vector2f(radius, radius);
    float radiusSquared = radius * radius; // That efficiency
    for (int y = 0; y < radius * 2; y++) {
        for (int x = 0; x < radius * 2; x++) {
            // The position used is the center of
            // the pixel
            Vector2f position = Vector2f(x + 0.5f
                                         , y + 0.5f);
            Vector2f distance = position - center;
            float lengthSquared = distance.x * distance.x
                + distance.y * distance.y;
            // Only change color if the pixel is in the
            // light circle
            if (lengthSquared <= radiusSquared) {
                float dist= sqrt(lengthSquared
                                 + height * height
                                 - centerDistance
                                 * centerDistance);
                float relativeIntensity =
                    1.0f / (falloff.x
                            + falloff.y * dist
                            + falloff.z * dist * dist);
                float blurIntensity = 1.0f - sqrt(lengthSquared)
                                                  / radius;
                                               
                Color currentColor;
                currentColor.r = centerColor.r;
                currentColor.g = centerColor.g;
                currentColor.b = centerColor.b;

                currentColor.a
                    = (Uint8) ((float) (centerColor.a) *
                               relativeIntensity * blurIntensity);

                image.setPixel(x, y, currentColor);
            }
        }
    }
    Texture lightTexture;
    lightTexture.loadFromImage(image);
    return lightTexture;
}
