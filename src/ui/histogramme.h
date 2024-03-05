// ArtiFrame/ui/histogramme.h
// Classe responsable de l'histogramme

#pragma once


#include "ofxDatGui.h"

class HistogramComponent : public ofxDatGuiFolder
{
public:

    int redHist[256] = { 0 };
    int greenHist[256] = { 0 };
    int blueHist[256] = { 0 };

    HistogramComponent(string label, ofColor color) : ofxDatGuiFolder(label, color) {}
    void draw();
    void setRedHist(int redHist[]);
    void setGreenHist(int greenHist[]);
    void setBlueHist(int blueHist[]);
};