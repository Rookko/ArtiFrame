// ArtiFrame/2d/application2d.cpp
// Classe principale de l'application 2D.

#include "application2d.h"
#include <algorithm> // Pour std::min


ofxDatGui* toolsGui;

ofxDatGui* shapeGui;

void Application2d::setup()
{

  ofNoFill();

  ofLog() << "<app::setup2d>";

  // Utiliser la dimension la plus petite de la fenêtre pour les calculs de proportion
  minDimension = std::min(ofGetWidth(), ofGetHeight());

  // Setup option width pixel
  optionWidth = minDimension * 0.01f;
  ofLog() << optionWidth;
  setup2DTaskbar();

  renderer.setup();

  gui.setup("interface");
  gui.setPosition(0, 300);

  group_draw.setup("outils de dessin");

  color_picker_background.set("couleur du canevas", ofColor(31), ofColor(0, 0), ofColor(255, 255));
  color_picker_stroke.set("couleur du trait", ofColor(255), ofColor(0, 0), ofColor(255, 255));

  slider_stroke_weight.set("largeur de la ligne", 4.0f, 0.0f, 10.0f);

  group_draw.add(color_picker_background);
  group_draw.add(color_picker_stroke);
  group_draw.add(slider_stroke_weight);

  gui.add(&group_draw);

  textbox.set("text", "ift3100");
  gui.add(textbox);

  button.setup("button");
  button.addListener(this, &Application2d::button_pressed);
  gui.add(&button);

  checkbox.setName("visible");
  gui.add(checkbox);

  checkbox = true;
}

void Application2d::update()
{
  // assigner les états courants de l'interface
  renderer.background_color = color_picker_background;
  renderer.stroke_color = color_picker_stroke;
  renderer.stroke_weight = slider_stroke_weight;
  renderer.text = textbox;

  renderer.update();
}

void Application2d::draw()
{
  renderer.draw();

  if (checkbox)
    gui.draw();
}

void Application2d::exit()
{
    button.removeListener(this, &Application2d::button_pressed);

    ofLog() << "<app::exit>";
}

// Fonction pour ajouter un bouton à un dossier (folder) avec gestion d'événement.
void Application2d::setupButton(ofxDatGuiFolder* folder, const std::string& label, void (Application2d::* eventHandler)(const ofxDatGuiButtonEvent&))
{
    ofxDatGuiButton* button = folder->addButton(label); // Ajoute un bouton au folder avec le label spécifié.
    button->onButtonEvent(this, eventHandler); // Lie le bouton à un gestionnaire d'événements.
}

// Fonction pour configurer un menu avec plusieurs boutons.
void Application2d::setupMenu(ofxDatGui*& menu, const std::string& title, int positionX, std::initializer_list<std::string> buttons)
{
    menu = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT); // Crée un nouveau menu avec ancrage en haut à gauche.
    menu->setWidth(optionWidth); // Définit la largeur du menu.
    menu->setPosition(positionX, 0); // Positionne le menu horizontalement avec positionX, verticalement à 0.
    ofxDatGuiFolder* folder = menu->addFolder(title); // Ajoute un dossier au menu avec le titre spécifié.

    // Pour chaque label dans la liste des boutons, ajoute un bouton au folder en utilisant setupButton.
    for (auto& label : buttons) {
        setupButton(folder, label, &Application2d::onAddShapeEvent); // Utilise une fonction hypothétique onAddShapeEvent comme gestionnaire pour tous les boutons.
    }
}

void Application2d::keyReleased(int key)
{
    if (key == 117) // touche u
    {
        checkbox = !checkbox;
        ofLog() << "<toggle ui: " << checkbox << ">";
    }
}

void Application2d::windowResized(int w, int h)
{
    ofLog() << "<app::windowResized: (" << w << ", " << h << ")>";
}

void Application2d::button_pressed()
{
    // réinitialiser la zone de texte
    textbox.set("text", "ift3100");

    ofLog() << "<button pressed>";
}

void Application2d::showUi() {
    fileMenu->setVisible(true);
    addMenu->setVisible(true);

}

void Application2d::hideUi() {
    fileMenu->setVisible(false);
    addMenu->setVisible(false);
}

void Application2d::onAddShapeEvent(ofxDatGuiButtonEvent e)
{
    // Identifier le bouton pressé en utilisant event.target->getLabel() ou toute autre propriété pertinente.
    std::string buttonLabel = event.target->getLabel();

    // Effectuer une action en fonction du bouton pressé.
    if (buttonLabel == "Add Square") {
        // Code pour ajouter un carré à la scène.
    }
    else if (buttonLabel == "Add Rectangle") {
        // Code pour ajouter un rectangle à la scène.
    }
    else if (buttonLabel == "Add Circle") {
        // Code pour ajouter un cercle à la scène.
    }
    else if (buttonLabel == "Add Ellipsis") {
        // Code pour ajouter une ellipse à la scène.
    }
    else if (buttonLabel == "Add Regular Polygon") {
        // Code pour ajouter un polygone régulier à la scène.
    }
    // Ajouter des cas supplémentaires selon les besoins pour d'autres formes.
}


void Application2d::saveRenderButtonEvent(ofxDatGuiButtonEvent e) {
    // Open a file dialog to get the desired file path and name

    ofFileDialogResult result = ofSystemLoadDialog("Select a folder", true);
    if (result.bSuccess) {
        string folderPath = result.filePath;
        ofLogNotice("Folder Selected") << folderPath;

        // Create an ofImage and grab the screen
        ofImage image;
        image.grabScreen(0, 60, ofGetWidth(), ofGetHeight() - 60);
        // Save the image to the specified folder with a default name 
        string defaultFileName = "render.png";
        folderPath = ofFilePath::addTrailingSlash(folderPath);

        string filePath = folderPath + defaultFileName;
        image.save(filePath);

        ofLogNotice("Image Saved") << "Render saved to: " << filePath;
    }
    else {
        ofLogNotice("File selection canceled");
    }

}

void Application2d::onAddSquareEvent(ofxDatGuiButtonEvent e) {

}

void Application2d::onAddRectangleEvent(ofxDatGuiButtonEvent e) {

}

void Application2d::onAddCircleleEvent(ofxDatGuiButtonEvent e) {

}

void Application2d::onAddEllipsisEvent(ofxDatGuiButtonEvent e) {

}

void Application2d::onAddRegularPolygonEvent(ofxDatGuiButtonEvent e) {

}

void Application2d::onAddStarEvent(ofxDatGuiButtonEvent e) {

}

void Application2d::onAddArrowEvent(ofxDatGuiButtonEvent e) {

}

// Fonction principale pour configurer la barre d'outils 2D.
void Application2d::setup2DTaskbar()
{
    // Configure le menu 'File' avec un bouton 'Export'.
    setupMenu(fileMenu, "File", 0, { "Export" });
    // Configure le menu 'Add' avec plusieurs boutons pour ajouter différentes formes.
    setupMenu(addMenu, "Add", optionWidth, { "Add Square", "Add Rectangle", "Add Circle", "Add Ellipsis", "Add Regular Polygon" });

    addSquareBtn->onButtonEvent(this, &Application2d::onAddShapeEvent);
    addRectangleBtn->onButtonEvent(this, &Application2d::onAddShapeEvent);
    addCircleBtn->onButtonEvent(this, &Application2d::onAddShapeEvent);
    addEllipsisBtn->onButtonEvent(this, &Application2d::onAddShapeEvent);
    addRegularPolygonBtn->onButtonEvent(this, &Application2d::onAddShapeEvent);
}








/*
void Application2d::setup2DTaskbar() {

    fileMenu = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    fileMenu->setWidth(optionWidth);
    fileMenuFolder = fileMenu->addFolder("File");
    ofxDatGuiButton* fileRenderButton = fileMenuFolder->addButton("Export");
    fileRenderButton->onButtonEvent(this, &Application2d::saveRenderButtonEvent);

    addMenu = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    addMenu->setWidth(120);
    addMenu->setPosition(optionWidth, 0);
    addMenuFolder = addMenu->addFolder("Add");
    ofxDatGuiButton* addSquareBtn = addMenuFolder->addButton("Add Square");
    addSquareBtn->onButtonEvent(this, &Application2d::onAddSquareEvent);
    ofxDatGuiButton* addRectangleBtn = addMenuFolder->addButton("Add Rectangle");
    addRectangleBtn->onButtonEvent(this, &Application2d::onAddRectangleEvent);
    ofxDatGuiButton* addCircleBtn = addMenuFolder->addButton("Add Circle");
    addCircleBtn->onButtonEvent(this, &Application2d::onAddCircleleEvent);
    ofxDatGuiButton* addEllipsisBtn = addMenuFolder->addButton("Add Ellipsis");
    addEllipsisBtn->onButtonEvent(this, &Application2d::onAddEllipsisEvent);
    ofxDatGuiButton* addRegularPolygonBtn = addMenuFolder->addButton("Add Regular Polygon");
    addRegularPolygonBtn->onButtonEvent(this, &Application2d::onAddRegularPolygonEvent);
}
*/


