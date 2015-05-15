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

Light::Light(int radius, Vector3f falloff
             , Color color, Vector2f position) {
    this->radius = radius;
    this->falloff = falloff;
    this->centerColor = color;
    this->position = position;
}

int Light::getRadius() {
    return radius;
}

Color Light::getColorAt(Vector2f pixelPosition) {
    Vector2f dif = position - pixelPosition;
    float dist = sqrt(dif.x * dif.x
                      + dif.y * dif.y);

    if (dist > radius) {
        return Color(0,0,0); // Neutral color
    }
    // Intensity determined by falloff
    float relativeIntensity =
        1.0f / (falloff.x
                + falloff.y * dist
                + falloff.z * dist * dist);
    // Intensity determined by linear distance from the
    // center. Is this needed?
    float circleIntensity = 1.0f - (dist / radius);
    float intensity = relativeIntensity * circleIntensity;

    Color colorAtPos;
    colorAtPos.r = (Uint8) ((float) (centerColor.r) * intensity);
    colorAtPos.g = (Uint8) ((float) (centerColor.g) * intensity);
    colorAtPos.b = (Uint8) ((float) (centerColor.b) * intensity);

    colorAtPos.a = centerColor.a;

    return colorAtPos;
}

Vector2f Light::getPosition() {
    return position;
}

void LightManager::initialize(const char* lightMapFilePath
                              , int tileWidth
                              , int tileHeight) {
    // Set up shader
    lightShader.loadFromFile("shaders/light.frag", Shader::Fragment);
    lightMultiplierShader.loadFromFile("shaders/lightMultiplier.frag"
                                       , Shader::Fragment);
    
    ambientColor = Color(80, 80, 80, 255);
    lightTexture.create(1000, 1000);
    
    Vector3f falloff;
    falloff.x = 1.2f; // Constant falloff
    falloff.y = 0.0005f; // Linear falloff
    falloff.z = 0.0000002f; // Quadratic falloff

    standardLightTexture
        = generateLightTexture(500, Color(255,255,255,255)
                               , falloff);
    
    // Read the positions of the lamps from the map
    std::ifstream map(lightMapFilePath);
    char buf[10];
    map >> buf; // Not needed
    int width, height;
    map >> width >> height;
    map >> buf; // Range of bitmap
                // Not needed
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r, g, b;
            map >> r >> g >> b;
            if (!(r == 0 && g == 0
                  && b == 0)) { // Lamp
                Sprite light;
                light.setTexture(standardLightTexture);
                light.setColor(Color(r,g,b));
                light.setPosition(x * tileWidth
                                  , y * tileHeight);
                light.setOrigin(500, 500);
                lights.push_back(light);
            }
        }
    }
}

void LightManager::draw(RenderTexture* diffuse
                        , RenderTexture* normal
                        , RenderWindow* window
                        , View currentView) {
    Vector2f viewCenter = currentView.getCenter()
        , viewSize = currentView.getSize();
    FloatRect viewRect(viewCenter.x - (viewSize.x / 2.0f)
                       , viewCenter.y - (viewSize.y / 2.0f)
                       , viewSize.x, viewSize.y);

    lightTexture.setView(currentView);
    lightTexture.clear(ambientColor);

    for (int i = 0; i < lights.size(); i++) {
        Vector2f lightCenter = lights[i].getPosition()
            , lightSize = Vector2f(standardLightTexture.getSize().x
                                   , standardLightTexture.getSize().y);
        FloatRect lightRect =
            FloatRect(lightCenter.x - (lightSize.x / 2.0f)
                      , lightCenter.y - (lightSize.y / 2.0f)
                      , lightSize.x, lightSize.y);
        if (lightRect.intersects(viewRect)) {
            lightShader.setParameter("normalTex", normal->getTexture()); 
            // Tex position of the lamp
            Vector2f texPos = Vector2f(lightRect.left, lightRect.top)
                - Vector2f(viewRect.left, viewRect.top);
            texPos.x /= (float) viewRect.width;
            texPos.y /= (float) viewRect.height;

            // Set the parameters
            lightShader.setParameter("texPos", texPos);
            lightShader.setParameter("normalTexSize", Vector2f(viewRect.width
                                                         , viewRect.height));
            lightShader.setParameter("texSize", lightSize);
            Vector2f dLightPos = lightCenter - Vector2f(viewRect.left, viewRect.top);
            dLightPos.x /= (float) viewRect.width;
            dLightPos.y /= (float) viewRect.height;
            lightShader.setParameter("lightPos", Vector3f(dLightPos.x, dLightPos.y
                                                          , 0.3f));
            RenderStates states;
            states.shader = &lightShader;
            states.blendMode = BlendAdd;
            lightTexture.draw(lights[i], states);
        }
    }
    lightTexture.display();

    lightSprite.setTexture(diffuse->getTexture());
    lightSprite.setTextureRect(IntRect(0,0,viewRect.width, viewRect.height));
    lightSprite.setPosition(viewRect.left, viewRect.top);

    lightMultiplierShader.setParameter("lightTex", lightTexture.getTexture());
    window->draw(lightSprite, &lightMultiplierShader);


    // Vector2f viewTopRight = Vector2f(viewRect.left
    //                                  , viewRect.top);
    // Color ambientLight = Color(30,30,30);
    // Image canvas = window->capture();
    // for (int y = 0; y < viewSize.y; y++) {
    //     for (int x = 0; x < viewSize.x; x++) {
    //         Color pixelColor = canvas.getPixel(x, y);
    //         if (!(pixelColor.r == 0
    //         swsddddddddddddddddddd      && pixelColor.g == 0
    //               && pixelColor.b == 0)) {
    //             Vector2f pixelPosition = Vector2f((float) x + 0.5f
    //                                               , (float) y + 0.5f);
    //             Color lightColor = ambientLight;
    //             for (int i = 0; i < lightsInView.size(); i++) {
    //                 Color extraLight = lightsInView[i]
    //                     .getColorAt(pixelPosition + viewTopRight);
    //                 lightColor.r += extraLight.r;
    //                 lightColor.g += extraLight.g;
    //                 lightColor.b += extraLight.b;
    //             }
    //             pixelColor.r *= ((float) (lightColor.r)
    //                              / 255.0f);
    //             pixelColor.g *= ((float) (lightColor.g)
    //                              / 255.0f);
    //             pixelColor.b *= ((float) (lightColor.b)
    //                              / 255.0f);
    //             canvas.setPixel(x, y, pixelColor);
    //         }
    //     }
    // }
    // drawTexture.loadFromImage(canvas);
    // drawSprite.setTexture(drawTexture);
    // drawSprite.setPosition(viewRect.left, viewRect.top);
    // drawSprite.setTextureRect(
    //     IntRect(0, 0
    //             , drawTexture.getSize().x
    //             , drawTexture.getSize().y));
    // window->draw(drawSprite);
}

void LightManager::setScreenSize(int x, int y) {
    lightTexture.create(x, y);
}

Texture generateLightTexture(int radius
                             , Color centerColor
                             , Vector3f falloff
                             // (constant, linear, quadratic)
    ) {
    Image image;
    image.create(radius * 2, radius * 2
                 , Color(0,0,0,1));
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
                float dist= sqrt(lengthSquared);
                float relativeIntensity =
                    1.0f / (falloff.x
                            + falloff.y * dist
                            + falloff.z * dist * dist);
                float blurIntensity = 1.0f - sqrt(sqrt(lengthSquared)
                                                  / radius);
                                               
                Color currentColor;

                currentColor.r
                    = (Uint8) ((float) (centerColor.r) *
                               relativeIntensity * blurIntensity);
                currentColor.g
                    = (Uint8) ((float) (centerColor.g) *
                               relativeIntensity * blurIntensity);
                currentColor.b
                    = (Uint8) ((float) (centerColor.b) *
                               relativeIntensity * blurIntensity);

                image.setPixel(x, y, currentColor);
            }
        }
    }
    Texture newLightTexture;
    newLightTexture.loadFromImage(image);
    return newLightTexture;
}
