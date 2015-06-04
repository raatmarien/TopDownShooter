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

#include "item.h"

using namespace sf;

void Box::initialize(BoxSettings settings) {
    world = settings.world;
    scale = settings.scale;
    
    sprite.setTexture(*(settings.texture));
    sprite.setOrigin(settings.size / 2.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(settings.position.x / (float) (settings.scale)
                         , settings.position.y / (float) (settings.scale));
    bodyDef.linearDamping = 1.0f;
    bodyDef.angularDamping = 1.0f;
    
    body = world->CreateBody(&bodyDef);

    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(settings.size.x / (float) (settings.scale * 2)
                       , settings.size.y / (float) (settings.scale * 2));

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &bodyShape;
    fixtureDef.density = settings.density;
    fixtureDef.friction = settings.friction;

    body->CreateFixture(&fixtureDef);
}

void Box::update() {
    b2Vec2 boxPosition = body->GetPosition();
    float boxAngle = body->GetAngle();
    setPosition(boxPosition.x * scale
                , boxPosition.y * scale);
    setRotation(boxAngle * 57.2957795);
}

void Box::destroy() {
    world->DestroyBody(body);
}

void Box::draw(RenderTarget &target, RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}
