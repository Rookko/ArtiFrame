// ArtiFrame/3d/application3d.h
// Classe principale de l'application 3D.
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDatGui.h"

#include "renderer3d.h"

class Application3d : public ofBaseApp
{
public:
	Renderer3d renderer;

	void setup();
	void update();
	void draw();
	void exit();
	
	void showUi();

	void hideUi();

	void setup3DTaskbar();

private:
	ofxDatGui* fileMenu;
	ofxDatGuiFolder* fileMenuFolder;
};
