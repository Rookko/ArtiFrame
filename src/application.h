// ArtiFrame/application.h
// Classe principale de l'application.
#pragma once

#include "ofxDatGui.h"

class Application : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void selectViewType();
	void changeViewTo2dEvent(ofxDatGuiButtonEvent e);
	void changeViewTo3dEvent(ofxDatGuiButtonEvent e);

	enum ViewType { View2D, View3D };

private:
	ofxDatGui* viewMenu;
	ofxDatGuiFolder* viewMenuFolder;

};