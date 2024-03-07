#include "shape.h"

class RegularPolygon : public Shape
{
public:

    float angleModifier = 0;

    void draw(int offsetX, int offsetY);

    void rotate90();

    void drawPolygon(ofPoint center);
};