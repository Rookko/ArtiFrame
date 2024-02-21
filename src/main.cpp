// ArtiFrame/main.cpp
// Fichier principal de l'application ArtiFrame

#include "ofMain.h"
#include "application2d.h"

int main()
{
  ofSetupOpenGL(800, 600, OF_WINDOW);
  ofRunApp(new Application2d());
}
