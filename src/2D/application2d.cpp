// ArtiFrame/2d/application2d.cpp
// Classe principale de l'application 2D.

//test  45356

#include "application2d.h"
#include <algorithm> // Pour std::min
#include "image.h";


ofxDatGui* toolsGui;

ofxDatGui* shapeGui;

ofPoint position;
ofPoint positionImageOrigine = { 0,0,0 };

ofPoint dragOrigine = { 0, 0, 0 };

ofxDatGuiScrollView* imageScroller;

void Application2d::setup(int buttonSize)
{
  optionWidth = buttonSize;
  ofNoFill();
  ofLog() << "<app::setup2d>";

  setup2DTaskbar();

  renderer.setup();

  gui.setup("Unlock Interface");
  gui.setPosition(ofGetWidth() - ofGetWidth()*0.25f, ofGetHeight() - ofGetHeight()*0.95f);

  group_draw.setup("Color Package");

  color_picker_background.set("Background Color", ofColor(31), ofColor(0, 0), ofColor(255, 255));
  color_picker_object.set("Selected Object Color", ofColor(255), ofColor(0, 0), ofColor(255, 255));

  slider_stroke_weight.set("Selected Object Scale", 4.0f, 0.0f, 20.0f);

  group_draw.add(color_picker_background);
  group_draw.add(color_picker_object);
  group_draw.add(slider_stroke_weight);

  gui.add(&group_draw);

  textbox.set("Text", "[{-_-}] ZZZzz zz z...");
  gui.add(textbox);

  button.setup("Scapiiishh Button");
  button.addListener(this, &Application2d::button_pressed);
  gui.add(&button);

  checkbox.setName("UwU");
  gui.add(checkbox);

  imageScroller = new ofxDatGuiScrollView("Scroll view", 100);
  imageScroller->setWidth(255);
  imageScroller->setPosition(ofGetWidth() - 255, header->getHeight() - 1);

  checkbox = true;
}

void Application2d::update()
{

  imageScroller->update();
  // assigner les états courants de l'interface
  renderer.background_color = color_picker_background;
  renderer.object_color = color_picker_object;
  renderer.stroke_weight = slider_stroke_weight;
  renderer.text = textbox;

  // Touche multiple ici avec space bar
  if (keyPress[32] && keyPress['i']) {
      importButtonEvent(); // Pour "Import"
  }
  else if (keyPress[32] && keyPress['e']) {
      saveRenderButtonEvent(); // Pour "Export"
  }
  else if (keyPress[32] && keyPress['u']) {
      layerUP(); // Pour "Layer UP"
  }
  else if (keyPress[32] && keyPress['d']) {
      layerDown(); // Pour "Layer Down"
  }
  else if (keyPress[32] && keyPress['z']) {
      undoButton(); // Pour "Undo"
  }
  else if (keyPress[32] && keyPress['x']) {
      redoButton(); // Pour "Redo"
  }
  else if (keyPress['e'] && keyPress['s'] && keyPress['t'] && keyPress['r']) {
      esterEgg(); // Pour "EsterEgg"
  }
  else if (keyPress[127] && keyPress['a']) {
      onDeletedAll(); // Pour "Deleted All" DEL + a
  }
  
  //Ajouter les autres AU DESSUS D'ICI

  renderer.update();
}

void Application2d::draw()
{
  renderer.draw();

  if (checkbox)
    gui.draw();

  imageScroller->draw();

}
//comon
void Application2d::button_pressed()
{
    // réinitialiser la zone de texte
    textbox.set("Text", "(-.-)Zzz...");

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
    keyPress[key] = false;
    if (key == 117) // touche u
    {
        checkbox = !checkbox;
        ofLog() << "<toggle ui: " << checkbox << ">";
    }

    // Ici, vous pouvez gérer les cas où seulement une touche est pressé.
    switch (key)
    {
    case 's': // Pour "Add Square"
        this->onAddSquareEvent();
        break;
    case 'r': // Pour "Add Rectangle"
        this->onAddRectangleEvent();
        break;
    case 'c': // Pour "Add Circle"
        this->onAddCircleleEvent();
        break;
    case 'e': // Pour "Add Ellipse"
        this->onAddEllipsisEvent();
        break;
    case 'p': // Pour "Add Polygone"
        this->onAddRegularPolygonEvent();
        break;
    case 'b': // Pour "Add Batman"
        this->addBatman();
        break;
    case 127: // Pour "Deleted"
        this->onDeleted();
        break;

        //Ajouter les autres AU DESSUS D'ICI
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
    othersMenu->setPosition(optionWidth*5, 0);

    editMenu->setWidth(optionWidth);
    editMenu->setPosition(optionWidth * 4, 0);

    headerLabel->setWidth(ofGetWidth() - optionWidth * 5);
    headerLabel->setPosition(optionWidth * 6, 0);
    
}

void Application2d::windowResized(int w, int h)
{
  rezize2DTaskbar();
  ofLog() << "<app::windowResized to: (" << w << ", " << h << ")>";
}

void Application2d::showUi() {
    fileMenu->setVisible(true);
    addMenu->setVisible(true);
    colorMenu->setVisible(true);
    editMenu->setVisible(true);
    othersMenu->setVisible(true);
    header->setVisible(true);
}

void Application2d::hideUi() {
    fileMenu->setVisible(false);
    addMenu->setVisible(false);
    colorMenu->setVisible(false);
    editMenu->setVisible(false);
    othersMenu->setVisible(false);
    header->setVisible(false);

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

    else if (buttonLabel == "Deleted")
    {
        this->onDeleted();
    }

    else if (buttonLabel == "Deleted All")
    {
        this->onDeletedAll();
    }

    else if (buttonLabel == "Add Batman")
    {
        this->addBatman();
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
    // Appel la fonction rectangle de la classe Primitive
    // Création d'une instance de Primitive2D en tant que carré
    /*
    Primitive2D* square = new Primitive2D(Primitive2D::SQUARE, taille); // Remplacer 'taille' par la valeur appropriée
    string filename = "square";
    square->originalName = filename; // Assurez-vous que Primitive2D gère ces propriétés
    filename = getElementName(filename);
    square->name = filename;

    imgScrollView->add(filename);

    addElementToRenderer(square); // Assurez-vous que cette méthode accepte Primitive2D*
    updateUiFromShape();
    */
}

void Application2d::onAddRectangleEvent() {
    // Appel la fonction rectangle de la classe Primitive
}

void Application2d::onAddCircleleEvent() {
    // Appel la fonction circle de la classe Primitive
}

void Application2d::onAddEllipsisEvent() {
    // Appel la fonction ellipsis de la classe Primitive
}

void Application2d::onAddRegularPolygonEvent() {
    // Appel la fonction polygone de la classe Primitive
}

void Application2d::onAddStarEvent() {
    // Appel la fonction star de la classe Primitive
}

void Application2d::onAddArrowEvent() {
    // Appel la fonction arrow de la classe Primitive
}

void Application2d::importButtonEvent()
{
    ofFileDialogResult importResult = ofSystemLoadDialog("Choisir l'image à importer");
    if (importResult.bSuccess)
    {
        import(importResult.getPath());
        ofLog() << "<app::import success>";
    }
    else {
        ofLog() << "app::import failed>";
    }
}



void Application2d::layerUP()
{/*
    int selectedLayer = -1;
    for (int i = 0; i < images.size(); ++i) {
        if (images[i].filePath == imageImport.getPath()) {
            selectedLayer = i;
            break;
        }
    }

    // Si l'image est trouvée et n'est pas déjà en haut
    if (selectedLayer != -1 && selectedLayer < images.size() - 1) {
        // Échangez les positions de couche avec l'image suivante
        swap(images[selectedLayer], images[selectedLayer + 1]);
    } */
}

void Application2d::layerDown()
{ /*
    if (imageImported && imageLayer < images.size() - 1) {
        // Incrémentez l'indice de la couche de l'image importée
        imageLayer++;

        // Déplacez l'image importée vers le bas dans la liste en intervertissant avec l'image suivante
        std::swap(images[imageLayer], images[imageLayer - 1]);

        ofLogNotice() << "Layer Down: Image moved down";
    } */
}


// Fonction principale pour configurer la barre d'outils 2D.
void Application2d::setup2DTaskbar()
{
    // Configure le menu 'File' avec un bouton 'Export'.
    setupMenu(fileMenu, "File", optionWidth, { "Export", "Import"});
    // Configure le menu 'Add' avec plusieurs boutons pour ajouter différentes formes.
    setupMenu(addMenu, "Add", optionWidth*2, { "Add Square", "Add Rectangle", "Add Circle", "Add Ellipsis", "Add Regular Polygon", "Add Batman" });
    // Ajouter optionWidth a chaque nouveau bouton pour le décaller
    setupMenu(colorMenu, "Color", optionWidth*3, { "Histogram" });

    setupMenu(othersMenu, "Others", optionWidth*5, { "Test" });
    setupMenu(editMenu, "Edit", optionWidth * 4, { "Undo", "Redo", "Layer UP", "Layer Down", "Deleted", "Deleted All" });

    header = new ofxDatGui(ofGetWidth() - optionWidth * 6, 0);
    headerLabel = header->addLabel("ArtiFrame 2D");
    headerLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    headerLabel->setWidth(ofGetWidth() - optionWidth * 6);
    headerLabel->setPosition(optionWidth * 6, 0);
    headerLabel->setStripeVisible(false);
}



void Application2d::undoButton()
{

}


void Application2d::redoButton()
{

}


void Application2d::keyPressed(int key)
{
    //keyNumber = key;
    keyPress[key] = true;
    ofLog() << "<boolkeyPressed: " << keyPress << ">";
    ofLog() << "<keyNumberPressed: " << keyNumber << ">";
}


void Application2d::esterEgg() {
    textbox.set("Text", "PipiCaca hihi");
}


void Application2d::onDeleted() {

}


void Application2d::onDeletedAll() {

}


void Application2d::addBatman() {
    // Appel la fonction batman de la classe Primitive
}



void Application2d::import(string path) {
    ofImage dataImg;
    dataImg.load(path);
    if (dataImg.isAllocated()) {
        string nomFichier;
        string::size_type index = path.rfind('\\');
        if (index != string::npos) {
            nomFichier = path.substr(index + 1);
        }
        
        Image* image = new Image();
        image->path = path;
        image->dataImg = dataImg;
        image->originalName = nomFichier;
        nomFichier = getObjetName(nomFichier);
        image->name = nomFichier;

        imageScroller->add(nomFichier);

        renderer.vecteurObjets.push_back(image);
        renderer.objetActif = image;
        renderer.indexActif++;

    }
}

void Application2d::onSelectImage(ofxDatGuiScrollViewEvent e) {

}

string Application2d::getObjetName(string nomFichier) {
    int copie = 0;
    for (object2D* objet : renderer.vecteurObjets) {
        if (objet->originalName == nomFichier) {
            copie++;
        }
    }

    if (copie > 0) {
        nomFichier += " (" + to_string(copie) + ") ";
    }
    return nomFichier;
}

void Application2d::dragEvent(ofDragInfo infoDrag) {
    ofLog() << "<app::ofDragInfo file[0]: " << infoDrag.files.at(0)
        << " at: " << infoDrag.position << ">";
    import(infoDrag.files.at(0));
}