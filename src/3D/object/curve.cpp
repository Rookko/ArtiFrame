#include "curve.h"

class BezierCurve {
public:
	BezierCurve(ofPoint p0, ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4) {
		controlPoints = { p0, p1, p2, p3, p4 };
	}

	ofPoint evaluate(float t) const {
		ofPoint p0 = controlPoints[0];
		ofPoint p1 = controlPoints[1];
		ofPoint p2 = controlPoints[2];
		ofPoint p3 = controlPoints[3];
		ofPoint p4 = controlPoints[4];

		ofPoint p01 = (1 - t) * p0 + t * p1;
		ofPoint p12 = (1 - t) * p1 + t * p2;
		ofPoint p23 = (1 - t) * p2 + t * p3;
		ofPoint p34 = (1 - t) * p3 + t * p4;

		ofPoint p0112 = (1 - t) * p01 + t * p12;
		ofPoint p1223 = (1 - t) * p12 + t * p23;
		ofPoint p2334 = (1 - t) * p23 + t * p34;

		ofPoint p = (1 - t) * p0112 + t * p1223;
		ofPoint q = (1 - t) * p1223 + t * p2334;

		return (1 - t) * p + t * q;
	}

private:
	std::vector<ofPoint> controlPoints;
};

void Curve::drawWireframeOverride() {
	ofPushStyle();

	BezierCurve curve = BezierCurve(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], controlPoints[4]);

	int numSteps = 50;

	ofBeginShape();
	ofNoFill();
	for (int i = 0; i <= numSteps; i++) {
		float t = (float)i / numSteps;
		ofPoint point = curve.evaluate(t);
		ofCurveVertex(point);
	}
	ofEndShape();

	ofDrawSphere(controlPoints[0], 5);
	ofDrawSphere(controlPoints[1], 5);
	ofDrawSphere(controlPoints[2], 5);
	ofDrawSphere(controlPoints[3], 5);
	ofDrawSphere(controlPoints[4], 5);

	ofPopStyle();
}

void Curve::drawShaderOverride() {
}

void Curve::drawBoundingBoxOverride() {

}

ofVec3f Curve::getMeshBoundingBoxDimension(ofMesh* mesh) {
	return ofVec3f();
}
