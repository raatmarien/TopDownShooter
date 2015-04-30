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

/* Returns if the point is on the line segment specified
 * by the points line1 and line2, only works if the points
 * are colinear.
 */
bool onSegment(sf::Vector2f line1, sf::Vector2f line2
               , sf::Vector2f point);

/* Returns the orientation of the ordered points point1, point2
 * point3.
 * 0 means they are colinear
 * 1 means they are clockwise
 * 2 means they are counterclockwise
 */
int pointsOrientation(sf::Vector2f point1, sf::Vector2f point2
                     , sf::Vector2f point3);

/* Returns true if the line segments specified by two points
 * each intersect eachother.
 */
bool lineSegmentsIntersect(sf::Vector2f line1Point1
                           , sf::Vector2f line1Point2
                           , sf::Vector2f line2Point1
                           , sf::Vector2f line2Point2);

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
    std::vector<sf::Vector2f> genObstaclePoints(      // Every 4 Vector2f's in the
        const char* filepath     // obstaclePoints vector 
        , int tileSize);         // represents 1 square         
    void setScreenDiagonal(int screenX, int screenY);
    std::vector<sf::Vector2f> getObstaclesInRange
        (sf::Vector2f sightCenter);
    void update(sf::Vector2f sightCenter);
    void draw(sf::RenderWindow* window); // Calls the draw methods
                                         // for the shadows and the
                                         // BlockRedrawer
private:
    int screenDiagonal, screenWidth, screenHeight;
    std::vector<sf::Vector2f> obstaclePoints;
    ShadowDrawable shadows;
};
