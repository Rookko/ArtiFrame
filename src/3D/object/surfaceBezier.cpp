#include "surfaceBezier.h"

void surfaceBezier::setup(int largeur, int hauteur, int dimension, int resolution) {
    largeur = largeur;
    hauteur = hauteur;
    cx = dimension - 1;
    cy = dimension - 1;
    rx = resolution;
    ry = resolution;

    int i, j;
    for (i = 0; i <= cx; i++) {
        input.push_back(vector<ofVec3f>());
        for (j = 0; j <= cy; j++) {
            input[i].push_back(ofVec3f());
        }
    }
    for (i = 0; i < rx; i++) {
        output.push_back(vector<ofVec3f>());
        for (j = 0; j < ry; j++) {
            output[i].push_back(ofVec3f());
        }
    }

    for (i = 0; i <= cx; i++) {
        for (j = 0; j <= cy; j++) {
            input[i][j].x = ofMap(i, 0, cx, 0, largeur);
            input[i][j].y = ofMap(j, 0, cy, 0, hauteur);
            input[i][j].z = ofRandom(-50, 50);
        }
    }

    mesh = ofMesh::plane(largeur, hauteur, rx, ry, OF_PRIMITIVE_TRIANGLES);
    creerSurfaceBezier();

    vector<glm::vec3> verts = mesh.getVertices();
    for (i = 0; i < verts.size(); i++) {
        mesh.setTexCoord(i, ofVec2f(verts[i].x, verts[i].y));
    }
}

void surfaceBezier::modifierPointControle(int index, ofVec3f newPos) {
    int offsetX = index / input.size();
    int offsetY = index % input.size();
    input[offsetX][offsetY] = newPos;
}

void surfaceBezier::draw() {
    ofSetColor(ofColor::white);
    mesh.draw();
}

void surfaceBezier::drawWireframe() {
    ofSetColor(ofColor::white);
    mesh.drawWireframe();
}

void surfaceBezier::update() {
    creerSurfaceBezier();
}

void surfaceBezier::drawControls() {
    ofSetColor(ofColor::white);
    int index = 0;
    for (vector<ofVec3f> column : input) {
        for (ofVec3f controlPoints : column) {
            ofSetColor(ofColor::red);
            ofDrawSphere(controlPoints, 3);
            ofSetColor(ofColor::red);
            ofDrawBitmapString(to_string(index), controlPoints);
            index++;
        }
    }
}

ofVec3f surfaceBezier::getPointControle(int index) {
    int offsetX = index / input.size();
    int offsetY = index % input.size();
    return input[offsetX][offsetY];
}

//----------------------------------------------------- bezier.
void surfaceBezier::creerSurfaceBezier() {
    output = calculSurfaceBezier(input, output, cx, cy, rx, ry);

    int i, j;
    int cnt;
    vector<ofVec3f> verts;
    for (i = 0; i < rx; i++) {
        for (j = 0; j < ry; j++) {
            verts.push_back(output[i][j]);
        }
    }
    for (i = 0; i < verts.size(); i++) {
        mesh.setVertex(i, verts[i]);
    }
}

vector< vector<ofVec3f> > surfaceBezier::calculSurfaceBezier(vector< vector<ofVec3f> > ip, vector< vector<ofVec3f> > op,
    int controlPointsX, int controlPointsY, int surfacePointsX, int surfacePointsY) {
    double mui, muj;
    double pi, pj;
    float x, y, z;

    for (int i = 0; i < surfacePointsX; i++) {
        mui = i / (double)(surfacePointsX - 1);
        for (int j = 0; j < surfacePointsY; j++) {
            muj = j / (double)(surfacePointsY - 1);

            op[i][j].x = 0;
            op[i][j].y = 0;
            op[i][j].z = 0;

            for (int ki = 0; ki <= controlPointsX; ki++) {
                pi = blendBezier(ki, mui, controlPointsX);
                for (int kj = 0; kj <= controlPointsY; kj++) {
                    pj = blendBezier(kj, muj, controlPointsY);
                    op[i][j].x += (ip[ki][kj].x * pi * pj);
                    op[i][j].y += (ip[ki][kj].y * pi * pj);
                    op[i][j].z += (ip[ki][kj].z * pi * pj);
                }
            }
        }
    }

    return op;
}

double surfaceBezier::blendBezier(int k, double prog, int degre) {
    int n, kn, nkn;
    double blend = 1;

    n = degre;
    kn = k;
    nkn = degre - k;

    while (n >= 1) {
        blend *= n;
        n--;
        if (kn > 1) {
            blend = blend / (double)kn;
            kn--;
        }
        if (nkn > 1) {
            blend = blend / (double)nkn;
            nkn--;
        }
    }
    if (k > 0)
        blend = blend * pow(prog, (double)k);
    if (n - k > 0)
        blend = blend * pow(1 - prog, (double)(degre - k));

    return(blend);
}
