#include "arrow.h"

void Arrow::draw(int offsetX, int offsetY) {
    ofPoint position;
    position.x = object2D::coordinates.x + offsetX;
    position.y = object2D::coordinates.y + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor, opacity);
    drawArrow(position);

    if (outline) {
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(outlineColor, opacity);
        drawArrow(position);
    }
}

void Arrow::rotate90() {
    if (angle == 3) {
        angle = 0;
    }
    else {
        angle++;
    }
}

void Arrow::drawArrow(ofPoint position) {
    switch (angle) {
    case 0: // Right
        ofDrawRectangle({ position.x, position.y + (height / 4) }, width * 0.6, height / 2);
        ofDrawTriangle(
            { position.x + width - (width * 0.4), position.y + height, 0 },
            { position.x + width - (width * 0.4), position.y, 0 },
            { position.x + width, position.y + (height * 0.5), 0 });
        break;
    case 1: // Down
        ofDrawRectangle({ position.x + (height / 4), position.y }, height / 2, width * 0.6);
        ofDrawTriangle(
            { position.x + height , position.y + width - (width * 0.4), 0 },
            { position.x, position.y + width - (width * 0.4), 0 },
            { position.x + (height * 0.5), position.y + width, 0 });
        break;
    case 2: // Left 
        ofDrawRectangle({ position.x + (width * 0.4), position.y + (height / 4) }, width * 0.6, height / 2);
        ofDrawTriangle(
            { position.x + (width * 0.4), position.y + height, 0 },
            { position.x + (width * 0.4), position.y, 0 },
            { position.x, position.y + (height * 0.5), 0 });
        break;
    case 3: // Up
        ofDrawRectangle({ position.x + (height / 4), position.y + (width * 0.4) }, height / 2, width * 0.6);
        ofDrawTriangle(
            { position.x + height , position.y + (width * 0.4), 0 },
            { position.x, position.y + (width * 0.4), 0 },
            { position.x + (height * 0.5), position.y, 0 });
        break;
    }
}
