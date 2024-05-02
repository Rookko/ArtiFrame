#pragma once

#include "ofMain.h"
#include "object.h"
#include "surfaceBezier.h"

class Surface : public Object {
public:

	surfaceBezier* surfaceBezier = new surfaceBezier();

	void drawWireframeOverride();

	void drawSolidOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);

};