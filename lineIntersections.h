#include <SFML/Graphics.hpp>

#pragma once

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
