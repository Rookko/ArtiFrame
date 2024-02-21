// ArtiFrame/2d/objet/primitive.cpp
//Classe responsable de l'arrondissement du bout des lignes
//Anthony Veillet Infographie IFT-3100 H2024

#include "Primitive.h"

// Initialiser l'épaisseur de la ligne
float Primitive::l_Width = 2.0f;

Primitive::Primitive() {
    // Constructeur par défaut
}

void Primitive::setLineWidth(float lw) {
    ofSetLineWidth(lw);
    l_Width = lw;
}

// Fonctions pour dessiner chaque primitive avec des ellipses aux sommets

    //Bezier
void Primitive::drawBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{

}

void Primitive::drawBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{

}

// Bit Map String


//Circle
void Primitive::drawCircle(const glm::vec3& p, float radius)
{

}

void Primitive::drawCircle(const glm::vec2& p, float radius)
{

}

void Primitive::drawCircle(float x, float y, float radius)
{

}

void Primitive::drawCircle(float x, float y, float z, float radius)
{

}


//Curve
void Primitive::drawCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
    // Dessiner la courbe
        ofDrawCurve(x0, y0, x1, y1, x2, y2, x3, y3);

    // Vérifier si l'épaisseur de la ligne nécessite le dessin de cercle
    if (l_Width > 2) {
        // Dessiner un cercle à chaque sommet
        ofSetLineWidth(1.0f);
        ofFill();
        ofDrawCircle(x0, y0, l_Width / 2);
        ofDrawCircle(x3, y3, l_Width / 2);
        ofNoFill();
        ofSetLineWidth(l_Width);
    }
}

void Primitive::drawCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    // Dessiner la courbe
    ofDrawCurve(x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3);

    // Vérifier si l'épaisseur de la ligne nécessite le dessin de cercle
    if (l_Width > 2) {
        // Dessiner un cercle à chaque sommet
        ofSetLineWidth(1.0f);
        ofFill();
        ofDrawCircle(x0, y0, z0, l_Width / 2);
        ofDrawCircle(x3, y3, z3, l_Width / 2);
        ofNoFill();
        ofSetLineWidth(l_Width);
    }
}


//Ellipse
void Primitive::drawEllipse(const glm::vec3& p, float width, float height)
{

}

void Primitive::drawEllipse(const glm::vec2& p, float width, float height)
{

}

void Primitive::drawEllipse(float x, float y, float width, float height)
{
    // La logique pour dessiner l'ellipse avec 'h' affectant la taille/rayon
    ofDrawEllipse(x, y, width, height);
}

void Primitive::drawEllipse(float x, float y, float z, float width, float height)
{

}


//Line
void Primitive::drawLine(const glm::vec3& p1, const glm::vec3& p2)
{

}

void Primitive::drawLine(const glm::vec2& p1, const glm::vec2& p2)
{

}

void Primitive::drawLine(float x1, float y1, float x2, float y2)
{
    // Dessiner la ligne
    ofDrawLine(x1, y1, x2, y2);

    // Vérifier si l'épaisseur de la ligne nécessite le dessin de cercle
    if (l_Width > 2) {
        // Dessiner un cercle à chaque sommet
        ofSetLineWidth(1.0f);
        ofFill();
        ofDrawCircle(x1, y1, l_Width / 2);
        ofDrawCircle(x2, y2, l_Width / 2);
        ofNoFill();
        ofSetLineWidth(l_Width);
    }
}

void Primitive::drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
    // Dessiner la ligne
    ofDrawLine(x1, y1, z1, x2, y2, z2);

    // Vérifier si l'épaisseur de la ligne nécessite le dessin de cercle
    if (l_Width > 2) {
        // Dessiner un cercle à chaque sommet
        ofSetLineWidth(1.0f);
        ofFill();
        ofDrawCircle(x1, y1, z1, l_Width / 2);
        ofDrawCircle(x2, y2, z2, l_Width / 2);
        ofNoFill();
        ofSetLineWidth(l_Width);
    }
}


//Rectangle Rounded

//Rectangle
void Primitive::drawRectangle(const glm::vec3& p, float w, float h)
{

}

void Primitive::drawRectangle(const glm::vec2& p, float w, float h)
{

}

void Primitive::drawRectangle(const ofRectangle& r)
{

}

void Primitive::drawRectangle(float x, float y, float z, float w, float h)
{
    // Dessiner le rectangle
    ofDrawRectangle(x, y, z, w, h);

    // Vérifier si l'épaisseur de la ligne nécessite le dessin d'ellipses
    if (l_Width > 2) {
        // Dessiner un cercle à chaque sommet
        ofSetLineWidth(1.0f);
        ofFill();
        ofDrawCircle(x, y, z, l_Width / 2);
        ofDrawCircle(x + w, y, z, l_Width / 2);
        ofDrawCircle(x + w, y, z + h, l_Width / 2);
        ofDrawCircle(x, y, z + h, l_Width / 2);
        ofNoFill();
        ofSetLineWidth(l_Width);
    }
}

void Primitive::drawRectangle(float x1, float y1, float w, float h)
{
    // Dessiner le rectangle
    ofDrawRectangle(x1, y1, w, h);

    // Vérifier si l'épaisseur de la ligne nécessite le dessin de cercle
    if (l_Width > 2) {
        // Dessiner un cercle à chaque sommet
        ofSetLineWidth(1.0f);
        ofFill();
        ofDrawCircle(x1, y1, l_Width / 2);
        ofDrawCircle(x1 + w, y1, l_Width / 2);
        ofDrawCircle(x1 + w, y1 + h, l_Width / 2);
        ofDrawCircle(x1, y1 + h, l_Width / 2);
        ofNoFill();
        ofSetLineWidth(l_Width);
    }
}


//Triangle
void Primitive::drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{

}

void Primitive::drawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
{

}

void Primitive::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    // Dessiner le triangle
    ofDrawTriangle(x1, y1, x2, y2, x3, y3);

    // Vérifier si l'épaisseur de la ligne nécessite le dessin de cercle
    if (l_Width > 2) {
        // Dessiner un cercle à chaque sommet
        ofSetLineWidth(1.0f);
        ofFill();
        ofDrawCircle(x1, y1, l_Width / 2);
        ofDrawCircle(x2, y2, l_Width / 2);
        ofDrawCircle(x3, y3, l_Width / 2);
        ofNoFill();
        ofSetLineWidth(l_Width);
    }
}

void Primitive::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    // Dessiner le triangle
    ofDrawTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);

    // Vérifier si l'épaisseur de la ligne nécessite le dessin de cercle
    if (l_Width > 2) {
        // Dessiner un cercle à chaque sommet
        ofSetLineWidth(1.0f);
        ofFill();
        ofDrawCircle(x1, y1, z1, l_Width / 2);
        ofDrawCircle(x2, y2, z2, l_Width / 2);
        ofDrawCircle(x3, y3, z3, l_Width / 2);
        ofNoFill();
        ofSetLineWidth(l_Width);
    }
}
