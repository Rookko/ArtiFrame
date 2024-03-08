// ArtiFrame/2d/application2d.h
// Classe principale de l'application 2D.

//TEST VINCE GIT

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDatGui.h"
#include "object2D.h";

#include "renderer2d.h"

class Application2d : public ofBaseApp
{
	struct ImageLayer {
		ofImage image;
		string filePath;
		int layer;
	};
public:
  Renderer2d renderer;
  
  ofxPanel gui;

  ofxGuiGroup group_background;
  ofxGuiGroup group_object;
  ofxGuiGroup group_trans_obj;
  ofxGuiGroup group_hsb;

  ofParameter<ofColor> color_picker_background;
  ofParameter<ofColor> color_picker_object;
  ofParameter<ofColor> color_outline_object;
  //ofParameter<ofColor> color_picker_background_hsb;
  //ofParameter<ofColor> color_picker_object_hsb;
  //ofParameter<ofColor> color_outline_object_hsb;

  ofParameter<float> slider_scale;
  ofParameter<float> slider_height;
  ofParameter<float> slider_width;
  ofParameter<int> slider_apex;
  ofParameter<bool> outline_object;

  ofParameter<float> hue, saturation, brightness;
  ofParameter<float> redValue, greenValue, blueValue, aValue;
  //ofColor color;

  ofParameter<string> textbox;

  ofParameter<bool> checkbox;

  ofxButton button;

  // Pour l'onglet HSB
  ofParameter<float> slider_hue;
  ofParameter<float> slider_saturation;
  ofParameter<float> slider_brightness;

  void setup(int buttonSize);
  void update();
  void draw();
  void button_pressed();
  void exit();

  // Déclaration de la fonction setupButton pour initialiser les boutons.
  void setupButton(ofxDatGuiFolder* folder, const std::string& label, void (Application2d::* eventHandler)(const ofxDatGuiButtonEvent&));

  // Déclaration de la fonction setupMenu pour initialiser les menus.
  void setupMenu(ofxDatGui*& menu, const std::string& title, int positionX, std::initializer_list<std::string> buttons);

  void keyReleased(int key);

  void rezize2DTaskbar();

  void windowResized(int w, int h);

  void showUi();

  void hideUi();

  void onAddShapeEvent(const ofxDatGuiButtonEvent& e);

  void saveRenderButtonEvent();
  
  void onAddSquareEvent();

  void onAddRectangleEvent();

  void onAddCircleleEvent();

  void onAddEllipsisEvent();

  void onAddRegularPolygonEvent();

  void onAddStarEvent();

  void onAddArrowEvent();

  void importButtonEvent();

  void layerUP();

  void layerDown();

  void undoButton();

  void redoButton();

  void keyPressed(int key);

  void easterEgg();

  void onDeleted();

  void onDeletedAll();

  void addBatman();

  int optionWidth;
 
  

  void import(string path);

  void onSelectImage(ofxDatGuiScrollViewEvent e);

  string getObjetName(string nomFichier);

  void dragEvent(ofDragInfo infoDrag);

  void addObjectToVector(object2D* objet);

  void removeActiveObjectFromeVector();

  void mousePressed(int x, int y, int button);

  void mouseReleased(int x, int y, int button);
  
  void mouseDragged(int x, int y, int button);

  void deleteSelected();

  // Nouvelles fonctions pour la conversion
  void RGBtoHSB(float& value);

  void HSBtoRGB(float& value);

  void updateColorPickerObject();

  bool isExporting = false;
  bool isDelete = false;
  bool isDragging = false;
  bool isRotate = false;
  bool isMoving = false;
  bool uiUpdate = false;
  bool isScaling = false;
  bool ignoreRGB = false;

  float originalHeight = 100.0f; // Valeur initiale
  float originalWidth = 150.0f;  // Valeur initiale
  float newHeight;
  float newWidth;

  ofImage* basicCursor;
  ofImage* moveCursor;
  ofImage* exportingCursor;
  ofImage* rotateCursor;
  ofImage* deleteCursor;

  private:

	// Ajouter les menus pour les différents boutons
	ofxDatGui* fileMenu;
	ofxDatGuiFolder* fileMenuFolder;

	ofxDatGui* addMenu;
	ofxDatGuiFolder* addMenuFolder;

	ofxDatGui* histoMenu;
	ofxDatGuiFolder* histoMenuFolder;

	ofxDatGui* othersMenu;
	ofxDatGuiFolder* othersMenuFolder;

	ofxDatGui* editMenu;
	ofxDatGuiFolder* editMenuFolder;

	ofxDatGui* header;
	ofxDatGuiLabel* headerLabel;

	// La fonction publique à appeler pour configurer la barre d'outils 2D.
	void setup2DTaskbar();

	int keyNumber;
	bool keyPress[255];

	string getElementName(string filename);

	void addElementToRenderer(object2D* object);

	void updateShapeFromUi();

	void updateUiFromShape();

	bool guiHit(int x, int y);

	void onUpdateShapeSliderEvent(ofxDatGuiSliderEvent e);

	void onUpdateShapeColorEvent(ofxDatGuiColorPickerEvent e);
	void onColorPickerObjectChanged(ofColor& color);
	void onHeightChanged(float& height);
	void onWidthChanged(float& width);
	void onScaleChanged(float& scale);
	void onApexChanged(int& apex);
	void onLineChanged(bool& largeur);
	void onColorLineChanged(ofColor& color);

	void deleteAll();

	void calculHistogram();


	
};
