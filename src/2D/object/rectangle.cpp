#include "rectangle.h"

void RectangleShape::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = object2D::coordinates.x + offsetX;
    position.y = object2D::coordinates.y + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor, opacity);
    ofDrawRectangle(position, width, height);

    if (outline) {
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(outlineColor, opacity);
        ofDrawRectangle(position, width, height);
    }
}

void RectangleShape::rotate90() {
    int originalHeight = height;
    height = width;
    width = originalHeight;
}