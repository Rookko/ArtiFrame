// ArtiFrame/2d/application2d.h
// Classe principale de l'application 2D.

//TEST VINCE GIT

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDatGui.h"

#include "renderer2d.h"

class Application2d : public ofBaseApp
{
public:
  Renderer2d renderer;

  ofxPanel gui;

  ofxGuiGroup group_draw;

  ofParameter<ofColor> color_picker_background;
  ofParameter<ofColor> color_picker_stroke;

  ofParameter<float> slider_stroke_weight;

  ofParameter<string> textbox;

  ofParameter<bool> checkbox;

  ofxButton button;

  void setup();
  void update();
  void draw();
  void exit();

  // Déclaration de la fonction setupButton pour initialiser les boutons.
  void setupButton(ofxDatGuiFolder* folder, const std::string& label, void (Application2d::* eventHandler)(const ofxDatGuiButtonEvent&));

  // Déclaration de la fonction setupMenu pour initialiser les menus.
  void setupMenu(ofxDatGui*& menu, const std::string& title, int positionX, std::initializer_list<std::string> buttons);

  void keyReleased(int key);

  void windowResized(int w, int h);

  void button_pressed();

  void showUi();

  void hideUi();

  //void setup2DTaskbar();

  void onAddShapeEvent(const ofxDatGuiButtonEvent& e);

  void saveRenderButtonEvent(ofxDatGuiButtonEvent e);

  void onAddSquareEvent(ofxDatGuiButtonEvent e);

  void onAddRectangleEvent(ofxDatGuiButtonEvent e);

  void onAddCircleleEvent(ofxDatGuiButtonEvent e);

  void onAddEllipsisEvent(ofxDatGuiButtonEvent e);

  void onAddRegularPolygonEvent(ofxDatGuiButtonEvent e);

  void onAddStarEvent(ofxDatGuiButtonEvent e);

  void onAddArrowEvent(ofxDatGuiButtonEvent e);

  
  int minDimension;
  int optionWidth;

  private:

	// Ajouter les menus pour les différents boutons
	ofxDatGui* fileMenu;
	ofxDatGuiFolder* fileMenuFolder;

	ofxDatGui* addMenu;
	ofxDatGuiFolder* addMenuFolder;

	ofxDatGui* colorMenu;
	ofxDatGuiFolder* colorMenuFolder;

	// La fonction publique à appeler pour configurer la barre d'outils 2D.
	void setup2DTaskbar();
};
