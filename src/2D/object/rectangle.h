#include "shape.h"

//Cant call it rectangle another class seem to have that name
class RectangleShape : public Shape
{
public:

    void draw(int offsetX, int offsetY);

    void rotate90();
};