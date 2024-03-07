#include "shape.h"

class Circle : public Shape
{
public:

    void draw(int offsetX, int offsetY);

    void rotate90();
};