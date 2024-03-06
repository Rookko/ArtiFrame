// ArtiFrame/main.cpp
// Fichier principal de l'application ArtiFrame

#include "ofMain.h"
#include "application.h"


int main()
{
  ofSetupOpenGL(1800, 1000, OF_WINDOW);
  ofRunApp(new Application());
}
