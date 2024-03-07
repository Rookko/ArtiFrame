#include "shape.h"

class Arrow : public Shape
{
public:

    int angle = 0;

    void draw(int offsetX, int offsetY);

    void rotate90();

    void Arrow::drawArrow(ofPoint position);

};