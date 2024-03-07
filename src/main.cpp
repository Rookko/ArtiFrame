// ArtiFrame/main.cpp
// Fichier principal de l'application ArtiFrame

#include "ofMain.h"
#include "application.h"


int main()
{
	ofGLFWWindowSettings windowSettings;
	windowSettings.resizable = true;
	windowSettings.setSize(1800, 1000);
	windowSettings.setGLVersion(4, 6);

	ofCreateWindow(windowSettings);

	Application* application = new Application();
	ofRunApp(application);
}
