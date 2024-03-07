#include "ellipsis.h"

void Ellipsis::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = object2D::coordinates.x + (width/2) + offsetX;
    position.y = object2D::coordinates.y + (height/2) + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor, opacity);
    ofDrawEllipse(position, width, height);

    if (outline) {
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(outlineColor, opacity);
        ofDrawEllipse(position, width, height);
    }
}

void Ellipsis::rotate90() {
    int originalHeight = height;
    height = width;
    width = originalHeight;
}