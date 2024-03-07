#include "square.h"

void Square::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = object2D::coordinates.x + offsetX;
    position.y = object2D::coordinates.y + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor, opacity);
    ofDrawRectangle(position, width, width);

    if (outline) {
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(outlineColor, opacity);
        ofDrawRectangle(position, width, width);
    }
}

void Square::rotate90() {
    //you cant rotate a square 90 degres lol
}
