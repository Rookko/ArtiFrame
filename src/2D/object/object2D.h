// ArtiFrame/2d/object/object2D.h
// Classe principale des objets 2D.
#ifndef TWODIMENSIONALOBJECT_H
#define TWODIMENSIONALOBJECT_H

#include "ofMain.h"

class object2D
{

public:
	string name;
	ofPoint coordinates;
	string originalName;
	ofColor color = ofColor::whiteSmoke;
	int opacity = 255;

	virtual void draw(int offsetX, int offsetY) = 0;
	virtual void rotate90() = 0;
};

#endif // !1