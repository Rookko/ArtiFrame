#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "light.h"

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

	// Textures
	ofTexture texture;
	ofTexture filteredTexture;
	ofTexture normalMap;
	ofTexture metallicMap;
	ofTexture roughnessMap;
	ofTexture occlusionMap;
	float magFilter = GL_LINEAR;
	float exposure = 1.0f;
	float gamma = 2.2f;

	// Material
	ofColor materialAmbiant = ofColor(63, 63, 63);
	ofColor materialDiffuse = ofColor(255, 255, 255);
	ofColor materialSpecular = ofColor(255, 255, 255);
	ofColor materialIor = ofFloatColor(0.04, 0.04, 0.04);

	float materialMetallic = 0.5f;
	float materialRoughness = 0.5f;
	float materialOcclusion = 1.0f;
	float materialBrightness = 1.0f;

	void drawWireframe();
	virtual void drawWireframeOverride() = 0;

	void drawTexture(ofShader* TextureMapping);
	void drawShader(ofShader* shader, Light ambiantLight, Light pointLight, Light directionalLight, Light spotLight);
	void drawPBR(ofShader* shader, Light pointLight, Light directionalLight, Light spotLight);
	virtual void drawShaderOverride() = 0;

	void checkIfSelected();

	void drawBoundingBox();
	virtual void drawBoundingBoxOverride() = 0;

	virtual ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh) = 0;

};

#endif // !1

