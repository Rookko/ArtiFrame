#include "ofMain.h"
#include "object.h"

#pragma once


class SpherePrimitive : public Object {
public:

	ofSpherePrimitive* sphere;

	void drawWireframeOverride();

	void drawSolidOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};