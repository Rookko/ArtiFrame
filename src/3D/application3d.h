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

	void setup(int buttonSize);
	void update();
	void draw();
	void exit();
	
	void showUi();

	void hideUi();

	void setup3DTaskbar();

	void setupButton(ofxDatGuiFolder* folder, const std::string& label, void (Application3d::* eventHandler)(const ofxDatGuiButtonEvent&));

	void setupMenu(ofxDatGui*& menu, const std::string& title, int positionX, std::initializer_list<std::string> buttons);

	void rezize3DTaskbar();

	void onAddShapeEvent(const ofxDatGuiButtonEvent& e);

	void keyPressed(int key);

	void keyReleased(int key);

	void mousePressed(int x, int y, int button);

	void mouseReleased(int x, int y, int button);

	void windowResized(int w, int h);

	void onObjectSelection(ofxDatGuiScrollViewEvent e);

	void onApplyTransformationEvent(ofxDatGuiButtonEvent e);

	void addSphere();

	void addCylinder();

	void addObject(Object* object, string filename);

	string getElementName(string filename);

	std::vector<Object*> getAllElementFromScene();

	std::vector<Object*> getAllElementFromObject(Object* object);

	Renderer3d::RenderMode getRenderMode();



	int optionWidth;
	string renderMode = "Wireframe";


	bool isWPressed = false;
	bool isAPressed = false;
	bool isSPressed = false;
	bool isDPressed = false;

	bool isQPressed = false;
	bool isEPressed = false;

	void onChangeCameraMode(ofxDatGuiButtonEvent e);



private:

	// Ajouter les menus pour les différents boutons
	ofxDatGui* fileMenu;
	ofxDatGuiFolder* fileMenuFolder;

	ofxDatGui* addMenu;
	ofxDatGuiFolder* addMenuFolder;

	ofxDatGui* colorMenu;
	ofxDatGuiFolder* colorMenuFolder;

	ofxDatGui* editMenu;
	ofxDatGuiFolder* editMenuFolder;

	ofxDatGui* renderMenu;
	ofxDatGuiFolder* renderMenuFolder;

	ofxDatGui* header;
	ofxDatGuiLabel* headerLabel;

	//Menu objet et transformation 3d
	ofxDatGuiScrollView* objectScrollView;
	ofxDatGuiScrollView* selectionScrollView;

	ofxDatGui* transformationMenu;
	ofxDatGuiDropdown* transformationDropdown;
	ofxDatGuiSlider* xAxisSlider;
	ofxDatGuiSlider* yAxisSlider;
	ofxDatGuiSlider* zAxisSlider;

};
