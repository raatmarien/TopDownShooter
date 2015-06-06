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

sf::Vector2f normalize(sf::Vector2f vector);

/* Draws the shadows based on the
 * VertexArray given to it
 */
class ShadowDrawable : public sf::Drawable
                     , public sf::Transformable {
public:
    void setVertices(sf::VertexArray nVertices);
private:
    sf::VertexArray vertices;
    virtual void draw(sf::RenderTarget& target
                      , sf::RenderStates states) const;
};

/* Calculates shadow in
 * its update() method
 * and draws them in
 * its draw() method
 */
class ShadowHandler {
public:
    ShadowHandler();
    // Every 4 Vector2f's in the
    // obstaclePoints vector 
    // represents 1 square         
    void setObstacles(std::vector<sf::Vector2f> obstacles, int tileSize);
    void setScreenDiagonal(int screenX, int screenY);
    std::vector<sf::Vector2f> getObstaclesInRange
        (sf::Vector2f sightCenter);
    void update(sf::Vector2f sightCenter
                , sf::Vector2f viewCenter);
    void draw(sf::RenderWindow* window); // Calls the draw methods
                                         // for the shadows and the
                                         // BlockRedrawer
private:
    int screenDiagonal, screenWidth, screenHeight;
    std::vector<sf::Vector2f> obstaclePoints;
    ShadowDrawable shadows;
};
