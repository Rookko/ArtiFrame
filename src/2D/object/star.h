#include "shape.h"

class Star : public Shape
{
public:

    float angleModifier = 0;

    void draw(int offsetX, int offsetY);

    void rotate90();

    void drawStar(ofPoint center);

};