#pragma once

#include "ofMain.h"
#include "object.h"

class Curve : public Object {
public:

	ofPolyline* polyline;

	ofPoint controlPoints[5] = { ofPoint(-200, 0, 0), ofPoint(-200, 100, 0), ofPoint(0, 200, 0), ofPoint(200, 100, 0), ofPoint(200, 0, 0) };

	void drawWireframeOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);

};