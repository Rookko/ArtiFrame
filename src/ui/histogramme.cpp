// ArtiFrame/ui/histogramme.cpp
// Classe responsable de l'histogramme

/*

#include "histogramme.h"
#include "ofMain.h"

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

        int bottom = ofxDatGuiFolder::getY() + ofxDatGuiFolder::getWidth() + ofxDatGuiFolder::getHeight();

        //Draw the histograms
        ofSetColor(255, 0, 0, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine(i, bottom, i, bottom - rHist[i] * ofxDatGuiFolder::getWidth() / redMax);
        }
        ofSetColor(0, 255, 0, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine(i, bottom, i, bottom - gHist[i] * ofxDatGuiFolder::getWidth() / greenMax);
        }
        ofSetColor(0, 0, 255, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine(i, bottom, i, bottom - bHist[i] * ofxDatGuiFolder::getWidth() / blueMax);
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

*/