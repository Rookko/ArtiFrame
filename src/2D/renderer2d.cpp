// ArtiFrame/2D/renderer2d.cpp
// Classe responsable du rendu de l'application 2D.

#include "renderer2d.h"

void Renderer2d::setup()
{
  ofSetFrameRate(60);
  ofSetBackgroundColor(31);
}

void Renderer2d::update()
{
  // paramètres des outils de dessin
  ofSetColor(object_color);
  ofSetLineWidth(object_scale);

  // résolution du rectangle qui délimite la zone de texte
  bounding_box = font.getStringBoundingBox(text, 0, 0);
}

void Renderer2d::draw()
{
  // couleur d'arrière-plan
  ofClear(background_color);
  for (object2D* objet : Renderer2d::vecteurObjets) {
      ofSetColor(objet->color.r, objet->color.g, objet->color.b ,objet->opacity);
      objet->draw(offsetX1, offsetY1);
      ofSetColor(255, 255, 255, 255);
  }

}

bool Renderer2d::hit(int x, int y) {
    bool isHit = (x > offsetX1) && (x < offsetX2) && (y > offsetY1) && (y < offsetY2);
    ofLog() << "hit test at (" << x << ", " << y << "): " << (isHit ? "true" : "false");
    return isHit;
}


