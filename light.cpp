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
}

void Light::setObstacles(std::vector<FloatRect> nObstacles) {
    obstacles = nObstacles;
}

void Light::draw(RenderTarget &target
          , RenderStates states) const {
    target.draw(vertices);
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
