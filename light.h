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
    virtual void draw(RenderTarget &target
                      , RenderStates states) const;
};

sf::Texture generateLightTexture(int radius
                                 , Color centerColor
                                 , float pixelsToHalfIntensity); // change?

