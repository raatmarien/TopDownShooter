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

#include <healthBar.h>
#include <iostream>

using namespace sf;

void HealthBar::initialize(Vector2f size, Player *player) {
    this->size = size;
    this->player = player;
    this->healthOffset = 4;

    // Set up an pixel thick outline of the bar
    outline.resize(5);
    outline.setPrimitiveType(LinesStrip);

    outline[0].position = Vector2f(0, 0);
    outline[1].position = Vector2f(size.x, 0);
    outline[2].position = Vector2f(size.x, size.y);
    outline[3].position = Vector2f(0, size.y);
    outline[4].position = Vector2f(0, 0);

    Color outlineColor = Color::White;
    for (int i = 0; i < 5; i++) outline[i].color = outlineColor;

    // Set up inside of health bar
    health.resize(4);
    health.setPrimitiveType(Quads);

}

void HealthBar::setSize(Vector2f size) {
    this->size = size;

    outline[1].position = Vector2f(size.x, 0);
    outline[2].position = Vector2f(size.x, size.y);
    outline[3].position = Vector2f(0, size.y);
}

void HealthBar::update() {
    float healthRatio = (float) (player->getHealth())
        / (float) (player->getMaxHealth());
    // Don't show health bar when health is negative
    Color healthColor = Color::Red;
    for (int i = 0; i < 4; i++)
        health[i].color = healthRatio > 0 ? healthColor : Color(0,0,0,0);
    int maxSize = size.x - (healthOffset * 2);
    Vector2f currentSize;
    currentSize.x = (int) ((float) (maxSize) * healthRatio);
    currentSize.y = size.y - (2 * healthOffset);

    Vector2f position = Vector2f(healthOffset, healthOffset);                              
    health[0].position = position;
    health[1].position = position + Vector2f(currentSize.x, 0);
    health[2].position = position + Vector2f(currentSize.x, currentSize.y);
    health[3].position = position + Vector2f(0, currentSize.y);
    
}

void HealthBar::draw(RenderTarget &target, RenderStates states) const {
    states.transform *= getTransform();
    target.draw(outline, states);
    target.draw(health, states);
}
