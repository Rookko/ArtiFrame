#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#ifndef OBJECT_H
#define OBJECT_H

class Object {
public:

	string name;
	string originalName;

	Object* parent;
	vector<Object*> children;

	bool selected = true;
	bool rotation_animation = false;
	bool translation_animation = false;

	float rotationX = 0;
	float rotationY = 0;
	float rotationZ = 0;

	float translationX = 0;
	float translationY = 0;
	float translationZ = 0;

	float scaleX = 1;
	float scaleY = 1;
	float scaleZ = 1;

	void drawWireframe();
	virtual void drawWireframeOverride() = 0;

	void drawSolid();
	virtual void drawSolidOverride() = 0;

	void drawShader();
	virtual void drawShaderOverride() = 0;

	void checkIfSelected();

	void drawBoundingBox();
	virtual void drawBoundingBoxOverride() = 0;

	virtual ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh) = 0;

};

#endif // !1

