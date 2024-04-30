#include "ofMain.h"

#ifndef LIGHT_H
#define LIGHT_H

class Light {
public:

	ofVec3f position;
	ofVec3f direction;
	float brightness;
	float innerCutoff;
	float outerCutoff;
	ofColor color;

	void test();

};

#endif // !1