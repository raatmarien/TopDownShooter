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
void LightManager::initialize(int ammountLights) {
    lightTexture
        = generateLightTexture(300, Color(255,245,150,255)
                               , 998, 1000, 1000);
    for (int i = 0; i < ammountLights; i++) {
        Light light;
        light.setTexture(lightTexture);
        light.setPosition(rand() % 5000
                          , rand() % 5000);
        lights.push_back(light);
    }
}

void LightManager::draw(RenderWindow* window
                        , View currentView) {
    Vector2f viewCenter = currentView.getCenter()
        , viewSize = currentView.getSize();
    FloatRect viewRect(viewCenter.x - (viewSize.x / 2.0f)
                       , viewCenter.y - (viewSize.y / 2.0f)
                       , viewSize.x, viewSize.y);
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
                             , float pixelsToHalfIntensity
                             , int height) {
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
                float distSquared = lengthSquared + height * height
                    - centerDistance * centerDistance;
                float relativeIntensity
                    = 1.0f / ((distSquared + pixelsToHalfIntensity)
                        / pixelsToHalfIntensity);
                Color currentColor;
                currentColor.r = centerColor.r;
                currentColor.g = centerColor.g;
                currentColor.b = centerColor.b;

                currentColor.a
                    = (Uint8) ((float) (centerColor.a) *
                               relativeIntensity);

                image.setPixel(x, y, currentColor);
            }
        }
    }
    Texture lightTexture;
    lightTexture.loadFromImage(image);
    return lightTexture;
}
