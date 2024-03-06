// ArtiFrame/2d/application2d.cpp
// Classe principale de l'application 2D.

//test  45356

#include "application2d.h"
#include <algorithm> // Pour std::min


ofxDatGui* toolsGui;

ofxDatGui* shapeGui;

void Application2d::setup(int buttonSize)
{
    optionWidth = buttonSize;
  ofNoFill();
  ofLog() << "<app::setup2d>";

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

  if (imageImported) {
      imageImport.resize(ofGetWidth() / 4, ofGetHeight() / 4);
      imageImport.draw(ofGetWidth() / 2, ofGetHeight() / 2);
  }
}
//comon
void Application2d::button_pressed()
{
    // réinitialiser la zone de texte
    textbox.set("text", "ift3100");

    ofLog() << "<button pressed>";
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


// Pour ajouter une kyeSwitch, simplement ajouter la touche et la relier à ca fonction
void Application2d::keyReleased(int key)
{
    if (key == 117) // touche u
    {
        checkbox = !checkbox;
        ofLog() << "<toggle ui: " << checkbox << ">";
    }

    bool shiftPressed = ofGetKeyPressed(OF_KEY_SHIFT);

    switch (key)
    {
    case 's': // Pour "Add Square"
        if (shiftPressed)
        {
            this->onAddSquareEvent();
        }
        break;
    case 'r': // Pour "Add Rectangle"
        if (shiftPressed)
        {
            // Code pour ajouter un rectangle à la scène uniquement si Shift est pressé.
        }
        break;
    case 'c': // Pour "Add Circle"
        // Code pour ajouter un cercle à la scène. Shift n'est pas requis.
        break;
        // Ajoutez des cas supplémentaires pour d'autres raccourcis clavier.
    }

}

void Application2d::rezize2DTaskbar() {
    // Utiliser la dimension la plus petite de la fenêtre pour les calculs de proportion
    int minDimension = std::min(ofGetWidth(), ofGetHeight());

    // Setup option width pixel
    optionWidth = minDimension * 0.15f;

    fileMenu->setWidth(optionWidth);
    fileMenu->setPosition(optionWidth, 0);

    addMenu->setWidth(optionWidth);
    addMenu->setPosition(optionWidth*2, 0);

    colorMenu->setWidth(optionWidth);
    colorMenu->setPosition(optionWidth*3, 0);

    othersMenu->setWidth(optionWidth);
    othersMenu->setPosition(optionWidth*4, 0);

    editMenu->setWidth(optionWidth);
    editMenu->setPosition(optionWidth*5, 0);

    /* header stuff
    headerLabel->setWidth(ofGetWidth() - fileMenu->getWidth() * 2 - addMenu->getWidth());
    headerLabel->setPosition(fileMenu->getWidth() * 2 + addMenu->getWidth(), 0);
    /

    /
    header = new ofxDatGui(ofGetWidth() - fileMenu->getWidth()2 - addMenu->getWidth(),0);
    headerLabel = header->addLabel("ArtiFrame 2D");
    headerLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    headerLabel->setWidth(ofGetWidth() - fileMenu->getWidth() 2 - addMenu->getWidth());
    headerLabel->setPosition(fileMenu->getWidth() * 2 + addMenu->getWidth(),0);
    headerLabel->setStripeVisible(false);
    */
}

void Application2d::windowResized(int w, int h)
{
  rezize2DTaskbar();
}

void Application2d::showUi() {
    fileMenu->setVisible(true);
    addMenu->setVisible(true);
    colorMenu->setVisible(true);
    othersMenu->setVisible(true);
}

void Application2d::hideUi() {
    fileMenu->setVisible(false);
    addMenu->setVisible(false);
    colorMenu->setVisible(false);
    othersMenu->setVisible(false);

}

// Fonction utilisée lorsque bouton ou keycape sont pressés
void Application2d::onAddShapeEvent(const ofxDatGuiButtonEvent& e)
{
    // Identifier le bouton pressé en utilisant event.target->getLabel() ou toute autre propriété pertinente.
    std::string buttonLabel = e.target->getLabel();

    // Fonction pour exporter la scène lorsque bouton Export cliqué.
    if (buttonLabel == "Export")
    {
        this->saveRenderButtonEvent();
    }

    // Fonction pour importer un objet lorsque bouton Import cliqué.
    else if (buttonLabel == "Import")
    {
        this->importButtonEvent();
    }

    // Fonction pour ajouter un carré à la scène lorsque bouton Add Square cliqué.
    else if (buttonLabel == "Add Square")
    {
        this->onAddSquareEvent();
    }

    // Fonction pour ajouter un rectangle à la scène lorsque bouton Add Rectangle cliqué.
    else if (buttonLabel == "Add Rectangle")
    {
        this->onAddRectangleEvent();
    }

    // Fonction pour ajouter un Circle à la scène lorsque bouton Add Circle cliqué.
    else if (buttonLabel == "Add Circle")
    {
        this->onAddCircleleEvent();
    }

    // Fonction pour ajouter un Ellipsis à la scène lorsque bouton Add Ellipsis cliqué.
    else if (buttonLabel == "Add Ellipsis")
    {
        this->onAddEllipsisEvent();
    }

    // Fonction pour ajouter un Regular Polygon à la scène lorsque bouton Add Regular Polygon cliqué.
    else if (buttonLabel == "Add Regular Polygon")
    {
        this->onAddRegularPolygonEvent();
    }

    // Fonction pour ajouter un Regular Polygon à la scène lorsque bouton Add Regular Polygon cliqué.
    else if (buttonLabel == "Layer UP")
    {
        this->layerDown();
    }

    // Fonction pour ajouter un Regular Polygon à la scène lorsque bouton Add Regular Polygon cliqué.
    else if (buttonLabel == "Layer Down")
    {
        this->layerUP();
    }
    // Ajouter des cas supplémentaires selon les besoins pour d'autres formes.
}

void Application2d::saveRenderButtonEvent() {
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

void Application2d::onAddSquareEvent() {

}

void Application2d::onAddRectangleEvent() {

}

void Application2d::onAddCircleleEvent() {

}

void Application2d::onAddEllipsisEvent() {

}

void Application2d::onAddRegularPolygonEvent() {

}

void Application2d::onAddStarEvent() {

}

void Application2d::onAddArrowEvent() {

}

void Application2d::importButtonEvent()
{
    ofFileDialogResult importResult = ofSystemLoadDialog("Choisir l'image à importer");
    if (importResult.bSuccess)
    {
        imageImported = imageImport.load(importResult.getPath());
        // Ajoutez un log ici pour confirmer que le chemin de l'image est correct.
        ofLogNotice() << "Image loaded from path: " << importResult.getPath();
    }
}

void Application2d::layerUP()
{

}

void Application2d::layerDown()
{

}

// Fonction principale pour configurer la barre d'outils 2D.
void Application2d::setup2DTaskbar()
{
    // Configure le menu 'File' avec un bouton 'Export'.
    setupMenu(fileMenu, "File", optionWidth, { "Export", "Import"});
    // Configure le menu 'Add' avec plusieurs boutons pour ajouter différentes formes.
    setupMenu(addMenu, "Add", optionWidth*2, { "Add Square", "Add Rectangle", "Add Circle", "Add Ellipsis", "Add Regular Polygon" });
    // Ajouter optionWidth a chaque nouveau bouton pour le décaller
    setupMenu(colorMenu, "Color", optionWidth*3, { "Histogram" });

    setupMenu(othersMenu, "Others", optionWidth*4, { "Test" });

    setupMenu(editMenu, "Edit", optionWidth * 4, { "Layer UP", "Layer Down"});
}