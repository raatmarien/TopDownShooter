#include "lineIntersections.h"

using namespace sf;

// Intersection functions
bool onSegment(Vector2f line1, Vector2f line2
               , Vector2f point) {
    return point.x < std::max(line1.x, line2.x)
        && point.x > std::min(line1.x, line2.x)
        && point.y < std::max(line1.y, line2.y)
        && point.y > std::min(line1.y, line2.y);
}

int pointsOrientation(Vector2f point1
                      , Vector2f point2
                      , Vector2f point3) {
// Magic value :). More info:
// http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
    int value = ((point2.y - point1.y)
                 * (point3.x - point2.x))
        - ((point2.x - point1.x)
           * (point3.y - point2.y));
    if (value == 0)
        return 0; // The points are colinear

    return (value > 0) ? 1 : 2;
}

bool lineSegmentsIntersect(Vector2f line1Point1
                           , Vector2f line1Point2
                           , Vector2f line2Point1
                           , Vector2f line2Point2) {
    int orientation1 = pointsOrientation(line1Point1
                                         , line1Point2
                                         , line2Point1);
    int orientation2 = pointsOrientation(line1Point1
                                         , line1Point2
                                         , line2Point2);
    int orientation3 = pointsOrientation(line2Point1
                                         , line2Point2
                                         , line1Point1);
    int orientation4 = pointsOrientation(line2Point1
                                         , line2Point2
                                         , line1Point2);

    if (orientation1 != orientation2
        && orientation3 != orientation4)
        return true;

    if (orientation1 == 0
        && onSegment(line1Point1, line1Point2
                     , line2Point1))
        return true;

    if (orientation2 == 0
        && onSegment(line1Point1, line1Point2
                     , line2Point2))
        return true;

    if (orientation3 == 0
        && onSegment(line2Point1, line2Point2
                     , line1Point1))
        return true;

    if (orientation4 == 0
        && onSegment(line2Point1, line2Point2
                     , line1Point2))
        return true;

    return false;
}
