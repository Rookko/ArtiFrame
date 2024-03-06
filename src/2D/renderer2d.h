// ArtiFrame/2D/renderer2d.h
// Classe responsable du rendu de l'application 2D.

#pragma once

#include "ofMain.h"
#include "object2D.h"

class Renderer2d
{
public:

  vector<object2D*> vecteurObjets;

  object2D* objetActif;

  int indexActif = -1;
  int offsetX1 = 0;
  int offsetY1 = 0;
  int offsetX2 = 0;
  int offsetY2 = 0;

  ofTrueTypeFont font;

  ofColor background_color;

  ofColor stroke_color;

  ofRectangle bounding_box;

  string text;

  float stroke_weight;

  float line_offset;

  int font_size;
  int minDimension;

  void setup();
  void update();
  void draw();
};
