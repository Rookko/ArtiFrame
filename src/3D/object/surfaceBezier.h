#pragma once

#include "ofVboMesh.h"
#include "ofMain.h"

class surfaceBezier {
public:
    void setup(int largeur, int hauteur, int dimension, int resolution);
    void update();
    void draw();
    void drawWireframe();
    void drawControls();
    void modifierPointControle(int index, ofVec3f newPos);
    ofVec3f getPointControle(int index);

private:
    int largeur;
    int hauteur;
    int cx;
    int cy;
    int rx;
    int ry;
    vector<vector<ofVec3f>> input;
    vector<vector<ofVec3f>> output;
    double blendBezier(int k, double prog, int degre);
    vector< vector<ofVec3f>> calculSurfaceBezier(vector< vector<ofVec3f> > ip, vector< vector<ofVec3f> > op, int controlPointsX, int controlPointsY, int surfacePointsX, int surfacePointsY);
    void creerSurfaceBezier();
    
    ofVboMesh mesh;
};