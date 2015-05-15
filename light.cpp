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

void LightManager::initialize(const char* lightMapFilePath
                              , int tileWidth
                              , int tileHeight) {
    // Set up shader
    lightShader.loadFromFile("shaders/light.frag", Shader::Fragment);
    lightMultiplierShader.loadFromFile("shaders/lightMultiplier.frag"
                                       , Shader::Fragment);

    // Setup rectangle
    Image onePix;
    onePix.create(1,1, Color::White);
    onePixTex.loadFromImage(onePix);
    onePixTex.setRepeated(true);
    rect.setTexture(&onePixTex);
    
    ambientColor = Color(90, 90, 90, 255);
    
    Vector3f falloff;
    falloff.x = 1.2f; // Constant falloff
    falloff.y = 0.0005f; // Linear falloff
    falloff.z = 0.0000002f; // Quadratic falloff 
    int size = 1000;
    float lightHeight = 100.0f;
    
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
                Light light;
                light.color = Color(r, g, b);
                light.center = Vector2f(x * tileWidth
                                        , y * tileHeight);
                light.rect = FloatRect(light.center.x - (size / 2)
                                       , light.center.y - (size / 2)
                                       , size, size);
                light.height = lightHeight;
                light.falloff = falloff;                    
                mapLights.push_back(&light);
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

    int lightsn = 0;
    for (int i = 0; i < lights.size(); i++) {
        Vector2f lightCenter = lights[i].center;
        FloatRect lightRect = lights[i].rect;
        if (lightRect.intersects(viewRect)) {
            lightShader.setParameter("normalTex", normal->getTexture()); 
            // Tex position of the lamp
            Vector2f texPos = Vector2f(lightRect.left, lightRect.top)
                - Vector2f(viewRect.left, viewRect.top);
            texPos.x /= (float) viewRect.width;
            texPos.y /= (float) viewRect.height;

            // Calculate intersecting rectangle
            FloatRect intersect = intersectingRectangle(lightRect, viewRect);

            // Set the parameters
            lightShader.setParameter("texPos", texPos);
            lightShader.setParameter("normalTexSize", Vector2f(viewRect.width
                                                         , viewRect.height));
            lightShader.setParameter("texSize", Vector2f(lightRect.width, lightRect.height));
            lightShader.setParameter("lightHeight", lights[i].height);
            lightShader.setParameter("falloff", lights[i].falloff);
            lightShader.setParameter("lightColor", lights[i].color);

            rect.setSize(Vector2f(lightRect.width, lightRect.height));
            rect.setPosition(lightRect.left, lightRect.top);

            RenderStates states;
            states.shader = &lightShader;
            states.blendMode = BlendAdd;
            lightTexture.draw(rect, states);
        }
    }
    lightTexture.display();

    lightSprite.setTexture(diffuse->getTexture());
    lightSprite.setTextureRect(IntRect(0,0,viewRect.width, viewRect.height));
    lightSprite.setPosition(viewRect.left, viewRect.top);

    lightMultiplierShader.setParameter("lightTex", lightTexture.getTexture());
    window->draw(lightSprite, &lightMultiplierShader);
}

void LightManager::setScreenSize(int x, int y) {
    lightTexture.create(x, y);
}

// Returns an 0 by 0 rectangle at the position (0,0)
// if the rectangles don't intersects
// Otherwise the smallest intersecting rectangle will be returned
FloatRect intersectingRectangle(FloatRect rect1
                                 , FloatRect rect2) {
    if (!rect1.intersects(rect2))
        return FloatRect(0,0,0,0);
    
    float x1 = std::max(rect1.left, rect2.left);
    float x2 = std::min(rect1.left + rect1.width
                        , rect2.left + rect2.width);
    float y1 = std::max(rect1.top, rect2.top);
    float y2 = std::min(rect1.top + rect1.height
                        , rect2.top + rect2.height);

    return FloatRect(x1, y1, x2 - x1, y2 - y1);
}
