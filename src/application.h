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

	void keyPressed(int key);

	void keyReleased(int key);

	void mousePressed(int x, int y, int button);

	void mouseReleased(int x, int y, int button);

	void windowResized(int w, int h);

	void dragEvent(ofDragInfo dragInfo);

	enum ViewType { View2D, View3D };
	int optionWidth;

private:
	ofxDatGui* viewMenu;
	ofxDatGuiFolder* viewMenuFolder;

};