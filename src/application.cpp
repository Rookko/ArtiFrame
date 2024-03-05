// ArtiFrame/application.cpp
// Classe principale de l'application.
#include <windows.h>
#include "application.h"
#include "application2d.h"
#include "application3d.h"
#define MAIN_ICON 102

Application::ViewType viewType = Application::ViewType::View2D;

Application2d* application2D;
Application3d* application3D;

//--------------------------------------------------------------
void Application::setup() {
    ofLog() << "<app::setup>";

    ofSetWindowTitle("Artiframe");

    // Définir l'icône de l'application
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAIN_ICON));
    if (hIcon) {
        // Associer l'icône à la fenêtre de l'application
        HWND hwnd = ofGetWin32Window();
        SetClassLongPtr(hwnd, GCLP_HICON, reinterpret_cast<LONG_PTR>(hIcon));
    }



    viewMenu = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    viewMenu->setPosition(60, 0);
    viewMenu->setWidth(60);
    viewMenuFolder = viewMenu->addFolder("View");
    ofxDatGuiButton* view2DButton = viewMenuFolder->addButton("2D Editor");
    view2DButton->onButtonEvent(this, &Application::changeViewTo2dEvent);
    ofxDatGuiButton* view3DButton = viewMenuFolder->addButton("3D Editor");
    view3DButton->onButtonEvent(this, &Application::changeViewTo3dEvent);

    application2D = new Application2d();
    application3D = new Application3d();

    application2D->setup();
    application3D->setup();

    selectViewType();

}

//--------------------------------------------------------------
void Application::update() {
    if (viewType == Application::ViewType::View2D) {
        application2D->update();
    }
    else if(viewType == Application::ViewType::View3D){
        application3D->update();
    }
}

//--------------------------------------------------------------
void Application::draw() {
    if (viewType == Application::ViewType::View2D) {
        application2D->draw();
    }
    else if (viewType == Application::ViewType::View3D) {
        application3D->draw();
    }
}
//--------------------------------------------------------------
void Application::selectViewType() {
    if (viewType == Application::ViewType::View2D) {
        application2D->showUi();
        application3D->hideUi();
    }
    else if (viewType == Application::ViewType::View3D) {
        application3D->showUi();
        application2D->hideUi();
    }
}
//--------------------------------------------------------------
void Application::changeViewTo2dEvent(ofxDatGuiButtonEvent e) {
    viewType = Application::ViewType::View2D;
    selectViewType();
    viewMenuFolder->collapse();

}
//--------------------------------------------------------------
void Application::changeViewTo3dEvent(ofxDatGuiButtonEvent e) {
    viewType = Application::ViewType::View3D;
    selectViewType();
    viewMenuFolder->collapse();
}