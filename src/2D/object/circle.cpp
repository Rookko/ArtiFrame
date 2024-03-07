#include "circle.h"

void Circle::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = object2D::coordinates.x + (width / 2) + offsetX;
    position.y = object2D::coordinates.y + (width / 2) + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor, opacity);
    ofDrawCircle(position, width / 2);

    if (outline) {
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(outlineColor, opacity);
        ofDrawCircle(position, width/2);
    }
}

void Circle::rotate90() {
    //you cant rotate a circle 90 degres lol
}