// ArtiFrame/ui/histogramme.cpp
// Classe responsable de l'histogramme



#include "histogramme.h"
#include "ofMain.h"
#include "application2d.h"

void Histogram::draw()
{
    if (ofxDatGuiFolder::mIsExpanded) {
        ofDrawRectangle(getX(), getY(), getWidth(), getHeight() + getWidth());

        // Find the maximum value in the histograms for normalization
        int redMax = 1, greenMax = 1, blueMax = 1;
        for (int i = 0; i < 256; i++) {
            redMax = max(redMax, rHist[i]);
            greenMax = max(greenMax, gHist[i]);
            blueMax = max(blueMax, bHist[i]);
        }

        int bottom = ofxDatGuiFolder::getY() + ofxDatGuiFolder::getWidth() + ofxDatGuiFolder::getHeight() * 1.45;

        int largeur = ofxDatGuiFolder::getX();

        //Draw the histograms
        ofSetColor(255, 0, 0, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine( i + largeur, bottom, i + largeur, bottom - rHist[i] * ofxDatGuiFolder::getWidth() / redMax);
        }
        ofSetColor(0, 255, 0, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine(i + largeur, bottom, i + largeur, bottom - gHist[i] * ofxDatGuiFolder::getWidth() / greenMax);
        }
        ofSetColor(0, 0, 255, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine(i + largeur, bottom, i + largeur, bottom - bHist[i] * ofxDatGuiFolder::getWidth() / blueMax);
        }
        ofSetColor(255, 255, 255, 255);
    }
    
    ofxDatGuiFolder::draw();
}

void Histogram::setRedHist(int rHistogram[])
{
    for (int i = 0; i < 256; i++) {
        rHist[i] = rHistogram[i];
    }
}

void Histogram::setGreenHist(int gHistogram[])
{
    for (int i = 0; i < 256; i++) {
        gHist[i] = gHistogram[i];
    }
}

void Histogram::setBlueHist(int bHistogram[])
{
    for (int i = 0; i < 256; i++) {
        bHist[i] = bHistogram[i];
    }
}




//DEUXIEME VERSION

/*
bool needsUpdateMax = true;
int redMax = 1, greenMax = 1, blueMax = 1;

void Histogram::updateMax() {
    for (int i = 0; i < 256; i++) {
        redMax = std::max(redMax, rHist[i]);
        greenMax = std::max(greenMax, gHist[i]);
        blueMax = std::max(blueMax, bHist[i]);
    }
    needsUpdateMax = false;
}

void Histogram::draw() {
    if (ofxDatGuiFolder::mIsExpanded) {
        if (needsUpdateMax) {
            updateMax();
        }

        ofDrawRectangle(getX(), getY(), getWidth(), getHeight() + getWidth());
        int bottom = getY() + getWidth() + getHeight();

        // Simplification des appels de fonction répétés
        float width = getWidth();
        // Dessin des histogrammes avec des boucles simplifiées
        drawHistogram(rHist, redMax, ofColor(255, 0, 0, 127), bottom, width);
        drawHistogram(gHist, greenMax, ofColor(0, 255, 0, 127), bottom, width);
        drawHistogram(bHist, blueMax, ofColor(0, 0, 255, 127), bottom, width);

        ofSetColor(255, 255, 255, 255); // Restaurer la couleur par défaut
    }
    ofxDatGuiFolder::draw();
}

void Histogram::drawHistogram(const int hist[], int maxVal, ofColor color, int bottom, float width) {
    ofSetColor(color);
    for (int i = 0; i < 256; i++) {
        if (hist[i] > 0) { // Dessiner seulement si la valeur est > 0 pour optimiser
            float height = hist[i] * width / maxVal;
            ofDrawLine(i, bottom, i, bottom - height);
        }
    }
}

void Histogram::setRedHist(int rHistogram[]) {
    std::copy(rHistogram, rHistogram + 256, rHist);
    needsUpdateMax = true;
}

void Histogram::setGreenHist(int gHistogram[]) {
    std::copy(gHistogram, gHistogram + 256, gHist);
    needsUpdateMax = true;
}

void Histogram::setBlueHist(int bHistogram[]) {
    std::copy(bHistogram, bHistogram + 256, bHist);
    needsUpdateMax = true;
}

*/