#include "ofMain.h"
#include "object.h"
#include "triangulation.h"

class CustomObject : public Object {
public:

	Triangulation* triangulator = new Triangulation();

	void drawWireframeOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};