// ArtiFrame/2D/primitive2D.cpp
// Classe responsable des Primitive 2D de l'application 2D.

#include "primitive2D.h"

Primitive2D::Primitive2D(ShapeType type, float width, float height)
	: type(type), width(width), height(height), position(ofPoint(0, 0)), fillColor(ofColor::black), outlineColor(ofColor::black), outlineThickness(0) {}


void Primitive2D::draw(int offsetX, int offsetY)
{
}

void Primitive2D::rotate90()
{
}

void Primitive2D::drawPolygon(ofPoint center)
{
}

void Primitive2D::drawStar(ofPoint center)
{
}

void Primitive2D::drawArrow(ofPoint position)
{
}

void Primitive2D::drawBatman(ofPoint position)
{
}
