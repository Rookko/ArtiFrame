// ArtiFrame/2d/object/object2D.h
// Classe principale des objets 2D.

#include "ofMain.h"

class object2D
{

public:
	string name;
	ofPoint coordinates;
	int offsetX1 = 0;
	int offsetY1 = 0;
	int offsetX2 = 0;
	int offsetY2 = 0;

	int active = -1;

	void setup();

	void draw();

};

