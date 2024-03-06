// ArtiFrame/2D/renderer2d.cpp
// Classe responsable du rendu de l'application 2D.

#include "renderer2d.h"

void Renderer2d::setup()
{
  ofSetFrameRate(60);
  ofSetBackgroundColor(31);

  // Utiliser la dimension la plus petite de la fenêtre pour les calculs de proportion
  minDimension = std::min(ofGetWidth(), ofGetHeight());

  // paramètre
  font_size = 90;

  // variable
  line_offset = font_size / 2.0f;

  // importer une police de caractères
  font.load("consolas.ttf", font_size);
}

void Renderer2d::update()
{
  // paramètres des outils de dessin
  ofSetColor(stroke_color);
  ofSetLineWidth(stroke_weight);

  // résolution du rectangle qui délimite la zone de texte
  bounding_box = font.getStringBoundingBox(text, 0, 0);
}

void Renderer2d::draw()
{
  // couleur d'arrière-plan
  ofClear(background_color);

  // dessiner le texte au centre de la fenêtre d'affichage
  font.drawString(
    text,
    (ofGetWidth()  / 2.0f) - (bounding_box.getWidth()  / 2.0f),
    (ofGetHeight() / 2.0f) + (bounding_box.getHeight() / 2.0f));

  // dessiner une ligne sous le texte
  ofDrawLine(
    (ofGetWidth()  / 2.0f) - (bounding_box.getWidth()  / 2.0f),
    (ofGetHeight() / 2.0f) + (bounding_box.getHeight() / 2.0f) + line_offset,
    (ofGetWidth()  / 2.0f) + (bounding_box.getWidth()  / 2.0f),
    (ofGetHeight() / 2.0f) + (bounding_box.getHeight() / 2.0f) + line_offset);
}
