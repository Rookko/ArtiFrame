// ArtiFrame/2d/object/shape.h
// Classe principale des formes 2D.

#include "object2D.h"

#pragma once

#ifndef SHAPE_H
#define SHAPE_H

class Shape : public object2D
{
public:

    int height = 100;
    int width = 150;
    int nbApex = 5;
    ofColor fillColor = ofColor::white;
    ofColor outlineColor = ofColor::black;
    bool outline = false;

};

#endif // SHAPE_H