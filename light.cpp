#include "light.h"
#include <SFML/Graphics.hpp>

using namespace sf;

Light::Light() {
    
}

void Light::update() {

}

void Light::setTexture(Texture nTexture) {
    texture = nTexture;
}

void Light::setObstacles(std::vector<FloatRect> nObstacles) {
    obstacles = nObstacles;
}

void Light::draw(RenderTarget &target
          , RenderStates states) const {
    target.draw(vertices);
}

Texture generateLightTexture(int radius
                             , Color centerColor
                             , float pixelsToHalfIntensity) {
    return Texture();
}
