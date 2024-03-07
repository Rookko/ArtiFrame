// ArtiFrame/2D/primitive2D.h
// Classe responsable des Primitive 2D de l'application 2D.

#pragma once

#include "shape.h"

class Primitive2D : public Shape {
public:
    enum ShapeType { SQUARE, RECTANGLE, CIRCLE, ELLIPSIS, POLYGONE, STAR, ARROW, BATMAN };

    // Constructeur qui définit le type de forme
    Primitive2D(ShapeType type, float width, float height = 0); // type(type), width(width), height(height == 0 ? width : height) {}

    // Méthode pour dessiner la forme
    void draw(int offsetX, int offsetY);

    void rotate90();

    void drawPolygon(ofPoint center);

    void drawStar(ofPoint center);

    void drawArrow(ofPoint position);

    void drawBatman(ofPoint position);



    int angle = 0;
    float angleModifier = 0;

private:

    // Propriétés communes aux formes ici
    ShapeType type;
    float width, height;
    ofPoint position;
    ofColor fillColor, outlineColor;
    float outlineThickness;
};