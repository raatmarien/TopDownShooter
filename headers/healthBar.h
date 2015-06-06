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
#include <player.h>

class HealthBar : public sf::Drawable, public sf::Transformable {
public:
    void initialize(sf::Vector2f size, Player *player);
    void setSize(sf::Vector2f size);
    void update();
private:
    sf::Vector2f size;
    Player *player;

    float healthOffset; // Offset of the actual health from the outline
    sf::VertexArray outline, health;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
