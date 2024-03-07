#include "shape.h"

class Square : public Shape
{
public:

    void draw(int offsetX, int offsetY);

    void rotate90();
};