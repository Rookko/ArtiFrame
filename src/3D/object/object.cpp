#include "object.h"

void Object::drawWireframe()
{
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawWireframeOverride();

	for (Object* child : children) {
		child->drawWireframe();
	}

	ofPopMatrix();
}

void Object::drawSolid() {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawSolidOverride();

	for (Object* child : children) {
		child->drawSolid();
	}

	ofPopMatrix();
}

void Object::drawShader() {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawShaderOverride();

	for (Object* child : children) {
		child->drawShader();
	}

	ofPopMatrix();
}

void Object::checkIfSelected() {
	if (selected) {
		drawBoundingBox();
	}
	else {
		ofPushMatrix();

		ofTranslate(translationX, translationY, translationZ);

		ofRotateXDeg(rotationX);
		ofRotateYDeg(rotationY);
		ofRotateZDeg(rotationZ);

		ofScale(scaleX, scaleY, scaleZ);

		for (Object* child : children) {
			child->checkIfSelected();
		}

		ofPopMatrix();
	}
}

void Object::drawBoundingBox() {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawBoundingBoxOverride();

	for (Object* child : children) {
		child->drawBoundingBox();
	}

	ofPopMatrix();
}