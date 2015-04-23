#include <SFML/Graphics.hpp>

using namespace sf;

/* Calculates shadow in
 * its update() method
 * and draws them in
 * its draw() method
 */
class ShadowHandler {
public:
    void update(Vector2f sightCenter
                , std::vector<Vector2f> // Every 4 Vector2f's in the
                obstaclePoints);        // obstaclePoints vector
                                        // represents 1 square
    void draw(RenderWindow* window); // Calls the draw methods
                                     // for the shadows and the
                                     // BlockRedrawer
    void setBlockTexture(Texture texture);
private:
    ShadowDrawable shadows;
    BlockRedrawer blockRedrawer;
};

/* Draws the shadows based on the
 * VertexArray given to it
 */
class ShadowDrawable : public Drawable, public Transformable {
public:
    VertexArray* getVertices();
private:
    VertexArray vertices;
    virtual void draw(RenderTarget* target
                      , RenderStates states) const;
};

/* Redraws the visible blocks
 * after the shadows have been
 * drawn
 */
class  BlockRedrawer : public Drawable, public Transformable {
public:
    void resetBlocks();
    void addBlock(Vector2f position);
    void setTexture(Texture nTexture);
private:
    Sprite blockSprite;
    Texture texture;
    std::vector<Vector2f> blockPositions;
    virtual void draw (RenderTarget* target
                       , RenderStates states) const;
}
