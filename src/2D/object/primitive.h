// ArtiFrame/2d/objet/primitive.h
//Classe responsable de l'arrondissement du bout des lignes
//Anthony Veillet Infographie IFT-3100 H2024

#pragma once

#include "ofMain.h"

class Primitive {
public:
    // Constructeur
    Primitive();

    // Fonction pour définir l'épaisseur de la ligne
    static void setLineWidth(float l_Width);

    // Fonctions pour dessiner chaque primitive avec des ellipses aux sommets

    //Bezier
    void drawBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
    void drawBezier(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

    // Bit Map String


    //Circle
    void drawCircle(const glm::vec3& p, float radius);
    void drawCircle(const glm::vec2& p, float radius);
    void drawCircle(float x, float y, float radius);
    void drawCircle(float x, float y, float z, float radius);

    //Curve
    void drawCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
    void drawCurve(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

    //Ellipse
    void drawEllipse(const glm::vec3& p, float width, float height);
    void drawEllipse(const glm::vec2& p, float width, float height);
    void drawEllipse(float x, float y, float width, float height);
    void drawEllipse(float x, float y, float z, float width, float height);

    //Line
    void drawLine(const glm::vec3& p1, const glm::vec3& p2);
    void drawLine(const glm::vec2& p1, const glm::vec2& p2);
    void drawLine(float x1, float y1, float x2, float y2);
    void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);

    //Rectangle Rounded

    //Rectangle
    void drawRectangle(const glm::vec3& p, float w, float h);
    void drawRectangle(const glm::vec2& p, float w, float h);
    void drawRectangle(const ofRectangle& r);
    void drawRectangle(float x, float y, float z, float w, float h);
    void drawRectangle(float x1, float y1, float w, float h);

    //Triangle
    void drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
    void drawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
    void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);


private:
    // Variable statique pour stocker l'épaisseur de la ligne
    static float l_Width;

};

