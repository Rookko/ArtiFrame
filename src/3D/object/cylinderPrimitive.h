#include "ofMain.h"
#include "object.h"

class CylinderPrimitive : public Object {
public:

	ofCylinderPrimitive* cylinder;

	void drawWireframeOverride();

	void drawSolidOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};