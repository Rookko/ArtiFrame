// ArtiFrame/2d/application2d.h
// Classe principale de l'application 2D.

//TEST VINCE GIT

#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "renderer2d.h"

class Application2d : public ofBaseApp
{
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

  void keyReleased(int key);

  void windowResized(int w, int h);

  void button_pressed();
};
