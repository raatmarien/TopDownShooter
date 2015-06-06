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
#include <Box2D/Box2D.h>
#include <player.h>
#include <enemy.h>
#include <groundTileMap.h>
#include <bulletManager.h>
#include <collidable.h>

class ContactListener : public b2ContactListener {
public:
    // Pass pointers to random objects of the specified
    // type to the object so it can check typeid against
    // them
    ContactListener();
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};
