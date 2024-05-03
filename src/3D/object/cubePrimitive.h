#include "ofMain.h"
#include "object.h"

#pragma once

class CubePrimitive : public Object {
public:

	ofBoxPrimitive* cube;

	void drawWireframeOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};