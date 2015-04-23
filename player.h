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

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

enum Movement {
    NONE = 0
    , UP
    , UP_RIGHT
    , RIGHT
    , DOWN_RIGHT
    , DOWN
    , DOWN_LEFT
    , LEFT
    , UP_LEFT
};

class Player : public sf::Drawable
             , public sf::Transformable {
public:
    void initialize(b2World* nWorld
                    , sf::Vector2f startPosition
                    , int nScale, int nSize
                    , sf::Texture nTexture);
    void update();
    void move(bool up);
    void turn(bool left);
private:
    Movement movement;
    float moveForce, rotationTorque;
    int scale, size;
    float framesRunning;
    sf::Texture texMap;
    sf::Sprite sprite;
    sf::IntRect currentRec;
    b2Body* body;
    b2World* world;
    const static float toDegreesMultiple = 57.2957795131;

    virtual void draw(sf::RenderTarget& target
                      , sf::RenderStates states) const;
    b2Vec2 rotateVec(b2Vec2 vector, float radians);
};
