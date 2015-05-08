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
#include <vector>

class Light : sf::Drawable, sf::Transformable {
public:
    Light();
    void update();
    void setTexture(sf::Texture nTexture);
    void setObstacles(std::vector<sf::FloatRect>
                      nObstacles);
private:
    std::vector<sf::FloatRect> obstacles;
    sf::VertexArray vertices;
    sf::Texture texture;
    virtual void draw(sf::RenderTarget &target
                      , sf::RenderStates states) const;
};

sf::Texture generateLightTexture(int radius
                                 , sf::Color centerColor
                                 , int centerDistance
                                 , float pixelsToHalfIntensity // change?
                                 , int height);
