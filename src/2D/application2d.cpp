// ArtiFrame/2d/application2d.cpp
// Classe principale de l'application 2D.

#include "application2d.h"
#include <algorithm> // Pour std::min


ofxDatGui* toolsGui;

ofxDatGui* shapeGui;
bool draggingShapeGui;

ofxDatGuiColorPicker* shapeColorPicker;
ofxDatGuiSlider* widthSlider;
ofxDatGuiSlider* heightSlider;

ofPoint position;
ofPoint positionImageOrigine = { 0,0,0 };

ofPoint dragOrigine = { 0, 0, 0 };
bool draggingObject;

ofxDatGuiScrollView* imageScroller;

Histogram* histogramme;
bool afficherHistogram;
ofxDatGuiFolder* histoFolder;

void Application2d::setup(int buttonSize)
{
  optionWidth = buttonSize;
  ofNoFill();
  ofLog() << "<app::setup2d>";

  setup2DTaskbar();

  renderer.setup();

  gui.setup("Unlock Interface");
  gui.setPosition(ofGetWidth() - ofGetWidth()*0.24f, ofGetHeight() - ofGetHeight()*0.975f);

  group_object.setup("Object Package");
  group_background.setup("Background Package");
  group_trans_obj.setup("Object Tranformations");
  group_hsb.setup("Object HSB Color");

  color_picker_background.set("Background RGB Color", ofColor(31), ofColor(0, 0), ofColor(255, 255));
  color_picker_object.set("Object RGB Color", ofColor(255), ofColor(0, 0), ofColor(255, 255));
  color_outline_object.set("Outline RGB Color", ofColor(255), ofColor(0, 0), ofColor(255, 255));

  slider_scale.set("Scale", 1.0f, 0.0f, 100.0f);
  slider_height.set("Height", 100.0f, 0.0f, 1000.0f);
  slider_width.set("Width", 100.0f, 0.0f, 1000.0f);
  slider_apex.set("Apex", 5, 3, 40);
  outline_object.setName("Object Outline");

  // HSB Sliders
  group_hsb.add(hue.set("Hue", 127, 0, 255));
  group_hsb.add(saturation.set("Saturation", 127, 0, 255));
  group_hsb.add(brightness.set("Brightness", 127, 0, 255));
  group_object.add(&group_hsb);

  // Listeners pour les changements
  hue.addListener(this, &Application2d::HSBtoRGB);
  saturation.addListener(this, &Application2d::HSBtoRGB);
  brightness.addListener(this, &Application2d::HSBtoRGB);
  redValue.addListener(this, &Application2d::RGBtoHSB);
  greenValue.addListener(this, &Application2d::RGBtoHSB);
  blueValue.addListener(this, &Application2d::RGBtoHSB);
  aValue.addListener(this, &Application2d::RGBtoHSB);

  group_background.add(color_picker_background);
  group_object.add(color_picker_object);
  group_object.add(color_outline_object);
  group_trans_obj.add(slider_scale);
  group_trans_obj.add(slider_height);
  group_trans_obj.add(slider_width);
  group_trans_obj.add(slider_apex);
  group_trans_obj.add(outline_object);

  gui.add(&group_background);
  gui.add(&group_object);
  gui.add(&group_trans_obj);


  textbox.set("", "");

  button.setup("Scapiiishh Button");
  button.addListener(this, &Application2d::button_pressed);
  color_picker_object.addListener(this, &Application2d::onColorPickerObjectChanged); // Ajouter listener color_picker_object
  slider_height.addListener(this, &Application2d::onHeightChanged); // Ajouter listener slider_height
  slider_width.addListener(this, &Application2d::onWidthChanged); // Ajouter listener slider_width
  slider_scale.addListener(this, &Application2d::onScaleChanged); // Ajouter listener slider_scale
  slider_apex.addListener(this, &Application2d::onApexChanged); // Ajouter listener slider_apex
  outline_object.addListener(this, &Application2d::onLineChanged); // Ajouter listener outline_object
  color_outline_object.addListener(this, &Application2d::onColorLineChanged); // Ajouter listener color_outline_object

  gui.add(&button);

  //menu pour les shape
  shapeGui = new ofxDatGui(300, 300);
  // (Test#2)shapeGui->addLabel("Menu des primitives");
  // (Test#2)shapeGui->addHeader("Slider");
  widthSlider = shapeGui->addSlider("Width", 0, 1000, 100);
  // Test#1 widthSlider->onSliderEvent(this, &Application2d::onUpdateShapeSliderEvent);
  // Test#1 heightSlider = shapeGui->addSlider("Height", 0, 1000, 100);
  // Test#1 heightSlider->onSliderEvent(this, &Application2d::onUpdateShapeSliderEvent);
  shapeColorPicker = shapeGui->addColorPicker("Shape color", ofColor::aliceBlue);
  // Test#1 shapeColorPicker->onColorPickerEvent(this, &Application2d::onUpdateShapeColorEvent);

  imageScroller = new ofxDatGuiScrollView("Scroll view", 100);
  imageScroller->setWidth(255);
  imageScroller->setPosition(ofGetWidth() - 255, header->getHeight() - 1);
  imageScroller->onScrollViewEvent(this, &Application2d::onSelectImage);

  histogramme = new Histogram("Open/Close", ofColor::black);
  toolsGui = new ofxDatGui(255, 255);
  toolsGui->addLabel("Histogram");
  toolsGui->setPosition(ofGetWidth() - imageScroller->getWidth(), ofGetHeight() - (ofGetHeight() / 3));
  toolsGui->addFolder(histogramme);

  renderer.offsetX1 = 0;
  renderer.offsetX2 = ofGetWidth() - imageScroller->getWidth();
  renderer.offsetY1 = 0;
  renderer.offsetY2 = ofGetHeight();

  basicCursor = new ofImage();
  basicCursor->load("cursors/basic_icon.png");
  moveCursor = new ofImage();
  moveCursor->load("cursors/move_icon.png");
  exportingCursor = new ofImage();
  exportingCursor->load("cursors/export_icon.png");
  rotateCursor = new ofImage();
  rotateCursor->load("cursors/rotate_icon.png");
  deleteCursor = new ofImage();
  deleteCursor->load("cursors/deleted_icon.png");
}

void Application2d::update()
{

  imageScroller->update();
  // assigner les états courants de l'interface
  renderer.background_color = color_picker_background;
  // (Test#3)renderer.object_color = color_picker_object;
  // (Test#3)renderer.object_scale = slider_scale;
  renderer.text = textbox;

  // Touche multiple ici avec space bar
  if (keyPress[32] && keyPress['i']) {
      importButtonEvent(); // Pour "Import"
  }
  else if (keyPress[32] && keyPress['u']) {
      layerDown(); // Pour "Layer UP rapide"
  }
  else if (keyPress[32] && keyPress['d']) {
      layerUP(); // Pour "Layer Down rapide"
  }
  else if (keyPress['e'] && keyPress['s'] && keyPress['t'] && keyPress['r']) {
      easterEgg(); // Pour "EasterEgg"
  }
  else if (keyPress[127] && keyPress['a']) {
      deleteAll(); // Pour "Deleted All" DEL + a
  }
  
  //Ajouter les autres AU DESSUS D'ICI

  if (draggingObject && renderer.objetActif != nullptr) {
      renderer.objetActif->coordinates.x = ofGetMouseX() - dragOrigine.x + positionImageOrigine.x;
      renderer.objetActif->coordinates.y = ofGetMouseY() - dragOrigine.y + positionImageOrigine.y;
  }

  renderer.update();
}

void Application2d::draw()
{
  renderer.draw();

  if (checkbox)
    gui.draw();

  imageScroller->draw();
  if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
      shapeGui->setVisible(true);
  }
  else {
      shapeGui->setVisible(false);
  }
  float x = static_cast<float>(ofGetMouseX());
  float y = static_cast<float>(ofGetMouseY());
  
  
  if (isDelete && renderer.hit(x, y) && renderer.objetActif != nullptr) {
      ofHideCursor();
      deleteCursor->draw(x - (deleteCursor->getWidth() / 2), y - (deleteCursor->getWidth() / 2));
  }
  else if (draggingObject && renderer.objetActif != nullptr) {
      ofHideCursor();
      moveCursor->draw(x - (moveCursor->getWidth() / 2), y - (moveCursor->getWidth() / 2));

  }
  else if (isRotate && renderer.hit(x, y) && renderer.objetActif != nullptr) {
      ofHideCursor();
      rotateCursor->draw(x - (rotateCursor->getWidth() / 2), y - (moveCursor->getWidth() / 2));
  }
  else if (isDragging && renderer.objetActif != nullptr) {
      ofHideCursor();
      moveCursor->draw(x - (moveCursor->getWidth() / 2), y - (moveCursor->getWidth() / 2));
  }
  else if (isExporting) {
      ofHideCursor();
      exportingCursor->draw(x, y);
  } 
  
  else if (!guiHit(x, y)) {
  ofHideCursor();
  basicCursor->draw(x, y);
  }
  else {
      ofShowCursor();
  }


}

void Application2d::button_pressed()
{
    // réinitialiser la zone de texte
    textbox.set("", "");

    ofLog() << "<button pressed>";
}

void Application2d::exit()
{
    button.removeListener(this, &Application2d::button_pressed);
    color_picker_object.removeListener(this, &Application2d::onColorPickerObjectChanged); // Retrait listener color_picker_object
    slider_height.removeListener(this, &Application2d::onHeightChanged); // Retirer listener slider_height
    slider_width.removeListener(this, &Application2d::onWidthChanged); // Retirer listener slider_width
    slider_scale.removeListener(this, &Application2d::onScaleChanged); // Retirer listener slider_scale
    slider_apex.removeListener(this, &Application2d::onApexChanged); // Ajouter listener slider_apex
    outline_object.removeListener(this, &Application2d::onLineChanged); // Ajouter listener outline_object
    color_outline_object.removeListener(this, &Application2d::onColorLineChanged); // Ajouter listener color_outline_object
    // Listeners pour les changements
    hue.removeListener(this, &Application2d::HSBtoRGB);
    saturation.removeListener(this, &Application2d::HSBtoRGB);
    brightness.removeListener(this, &Application2d::HSBtoRGB);
    redValue.removeListener(this, &Application2d::RGBtoHSB);
    greenValue.removeListener(this, &Application2d::RGBtoHSB);
    blueValue.removeListener(this, &Application2d::RGBtoHSB);
    aValue.removeListener(this, &Application2d::RGBtoHSB);

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
    if (key == 'o') // touche o fermer Unlock Interface
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
    case 'w': // Pour "Add Arrow"
        this->onAddArrowEvent();
        break;
    case 'q': // Pour "Add Star"
        this->onAddStarEvent();
        break;
    case 127: // Pour "Deleted"
        if (isDelete == true) {
            isDelete = false;
        }

        else {
            isDelete = true;
        }
        break;

    case 'u': // Pour "Layer UP 1x"
        this->layerDown();
        break;
    case 'd': // Pour "Layer Down 1x"
        this->layerUP();
        break;

    case 'm' : //Export
        if (isExporting == true) {
            isExporting = false;
        }

        else {
            isExporting = true;
        }
        break;

    case 't': //rotate
        if (isRotate == true) {
            isRotate = false;
        }

        else {
            isRotate = true;
        }
        break;
    }

        //Ajouter les autres AU DESSUS D'ICI
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

    histoMenu->setWidth(optionWidth);
    histoMenu->setPosition(optionWidth*4, 0);

    othersMenu->setWidth(optionWidth);
    othersMenu->setPosition(optionWidth*5, 0);

    editMenu->setWidth(optionWidth);
    editMenu->setPosition(optionWidth * 3, 0);

    headerLabel->setWidth(ofGetWidth() - optionWidth * 5);
    headerLabel->setPosition(optionWidth * 6, 0);
}

void Application2d::windowResized(int w, int h)
{
  rezize2DTaskbar();
  ofLog() << "<app::windowResized to: (" << w << ", " << h << ")>";
  imageScroller->setPosition(ofGetWidth() - 255, header->getHeight() - 1);
  toolsGui->setPosition(ofGetWidth() - imageScroller->getWidth(), ofGetHeight() - (ofGetHeight() / 3));
}

void Application2d::showUi() {
    fileMenu->setVisible(true);
    addMenu->setVisible(true);
    histoMenu->setVisible(true);
    editMenu->setVisible(true);
    othersMenu->setVisible(true);
    header->setVisible(true);
    shapeGui->setVisible(true);
    histogramme->setVisible(true);
    toolsGui->setVisible(true);
}

void Application2d::hideUi() {
    fileMenu->setVisible(false);
    addMenu->setVisible(false);
    histoMenu->setVisible(false);
    editMenu->setVisible(false);
    othersMenu->setVisible(false);
    header->setVisible(false);
    shapeGui->setVisible(false);
    histogramme->setVisible(false);
    toolsGui->setVisible(false);
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
    else if (buttonLabel == "Add Star")
    {
        this->onAddStarEvent();
    }
    else if (buttonLabel == "Add Arrow")
    {
        this->onAddArrowEvent();
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
        this->deleteSelected();
    }
    else if (buttonLabel == "Deleted All")
    {
        this->deleteAll();
    }
    else if (buttonLabel == "Start")
    {
        startMusic();
    }
    else if (buttonLabel == "Play")
    {
        resumeMusic();
    }
    else if (buttonLabel == "Pause")
    {
        pauseMusic();
    }
    else if (buttonLabel == "Restart")
    {
        restartMusic();
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
    Square* square = new Square();
    string filename = "square";
    square->originalName = filename;
    filename = getElementName(filename);
    square->name = filename;

    imageScroller->add(filename);

    addElementToRenderer(square);
    updateUiFromShape();
}

void Application2d::onAddRectangleEvent() {
    // Appel la fonction rectangle de la classe Primitive
    RectangleShape* rectangle = new RectangleShape();
    string filename = "rectangle";
    rectangle->originalName = filename;
    filename = getElementName(filename);
    rectangle->name = filename;

    imageScroller->add(filename);

    addElementToRenderer(rectangle);
    updateUiFromShape();

}

void Application2d::onAddCircleleEvent() {
    // Appel la fonction circle de la classe Primitive
    Circle* circle = new Circle();
    string filename = "circle";
    circle->originalName = filename;
    filename = getElementName(filename);
    circle->name = filename;

    imageScroller->add(filename);

    addElementToRenderer(circle);
    updateUiFromShape();
    
}

void Application2d::onAddEllipsisEvent() {
    // Appel la fonction ellipsis de la classe Primitive
    Ellipsis* ellipse = new Ellipsis();
    string filename = "ellipse";
    ellipse->originalName = filename;
    filename = getElementName(filename);
    ellipse->name = filename;

    imageScroller->add(filename);

    addElementToRenderer(ellipse);
    updateUiFromShape();
}

void Application2d::onAddRegularPolygonEvent() {
    // Appel la fonction polygone de la classe Primitive
    RegularPolygon* regularPolygon = new RegularPolygon();
    string filename = "polygone régulier";
    regularPolygon->originalName = filename;
    filename = getElementName(filename);
    regularPolygon->name = filename;

    imageScroller->add(filename);

    addElementToRenderer(regularPolygon);
    updateUiFromShape();
}

void Application2d::onAddStarEvent() {
    // Appel la fonction star de la classe Primitive
    Star* star = new Star();
    string filename = "star";
    star->originalName = filename;
    filename = getElementName(filename);
    star->name = filename;

    imageScroller->add(filename);

    addElementToRenderer(star);
    updateUiFromShape();
}

void Application2d::onAddArrowEvent() {
    // Appel la fonction arrow de la classe Primitive
    Arrow* arrow = new Arrow();
    string filename = "arrow";
    arrow->originalName = filename;
    filename = getElementName(filename);
    arrow->name = filename;

    imageScroller->add(filename);

    addElementToRenderer(arrow);
    updateUiFromShape();
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
{
    if (renderer.objetActif != nullptr) {
        // Si l'image est trouvée et n'est pas déjà en haut
        if (renderer.indexActif < renderer.vecteurObjets.size() - 1) {
            // Échangez les positions de couche avec l'image suivante
            imageScroller->swap(renderer.indexActif, renderer.indexActif + 1);
            swap(renderer.vecteurObjets[renderer.indexActif], renderer.vecteurObjets[renderer.indexActif + 1]);
            renderer.indexActif += 1;
            renderer.objetActif = renderer.vecteurObjets.at(renderer.indexActif);
        }
    }
}

void Application2d::layerDown()
{
    if (renderer.objetActif != nullptr) {
        // Si l'image est trouvée et n'est pas déjà en haut
        if (renderer.indexActif > 0) {
            // Échangez les positions de couche avec l'image suivante
            imageScroller->swap(renderer.indexActif, renderer.indexActif - 1);
            swap(renderer.vecteurObjets[renderer.indexActif], renderer.vecteurObjets[renderer.indexActif - 1]);
            renderer.indexActif -= 1;
            renderer.objetActif = renderer.vecteurObjets.at(renderer.indexActif);
        }
    }
}


// Fonction principale pour configurer la barre d'outils 2D.
void Application2d::setup2DTaskbar()
{
    // Configure le menu 'File' avec un bouton 'Export'.
    setupMenu(fileMenu, "File", optionWidth, { "Export", "Import"});
    // Configure le menu 'Add' avec plusieurs boutons pour ajouter différentes formes.
    setupMenu(addMenu, "Add", optionWidth*2, { "Add Square", "Add Rectangle", "Add Circle", "Add Ellipsis", "Add Regular Polygon", "Add Star", "Add Arrow"});
    // Ajouter optionWidth a chaque nouveau bouton pour le décaller
    setupMenu(histoMenu, "Music", optionWidth*4, { "Start", "Pause", "Play", "Restart" });

    setupMenu(othersMenu, "Others", optionWidth*5, { "Test" });
    setupMenu(editMenu, "Edit", optionWidth * 3, { "Undo", "Redo", "Layer UP", "Layer Down", "Deleted", "Deleted All" });

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

    if (key == OF_KEY_ALT) {
        isDelete = true;
    }
    else if (key == OF_KEY_SHIFT) {
        isRotate = true;
    }
    else if (key == 48) {
        isExporting = true;
    }
}


void Application2d::easterEgg() {
    textbox.set("Text", "SQUAPISSSSHHHH");
    string url = "https://www.youtube.com/watch?v=iik25wqIuFo";
    ofLaunchBrowser(url);
}


void Application2d::onDeleted() {
    isDelete = false;
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

        calculHistogram();

    }
}

void Application2d::onSelectImage(ofxDatGuiScrollViewEvent e) {
    ofLog() << "<app::selecting image at index : " << e.target->getIndex() << ">";

    renderer.objetActif = renderer.vecteurObjets.at(e.target->getIndex());
    renderer.indexActif = e.target->getIndex();
    calculHistogram();

    if (renderer.objetActif != nullptr) {
        renderer.objetActif->setColor(color_picker_object);
    }
    updateUiFromShape(); // <3 <3 <3 <3 Charles <3 <3 <3 UwU <3 <3
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

void Application2d::addObjectToVector(object2D* objet) {
    renderer.vecteurObjets.push_back(objet);
    renderer.objetActif = objet;
    renderer.indexActif++;
}

void Application2d::removeActiveObjectFromeVector() {
    if (renderer.objetActif != nullptr) {
        renderer.vecteurObjets.erase(renderer.vecteurObjets.begin() + renderer.indexActif);
        
        imageScroller->remove(renderer.indexActif);
        if (renderer.vecteurObjets.size() > 0) {
            renderer.objetActif = renderer.vecteurObjets.at(0);
            renderer.indexActif = 0;
        }
        else {
            renderer.objetActif = nullptr;
            renderer.indexActif = -1;
        }
    }
}

void Application2d::mousePressed(int x, int y, int button) {
    ofLog() << "<app::mousePressed at:(" << x << ", " << y << ")>";
    ofLog() << "hitGUi test at (" << x << ", " << y << "): " << (guiHit(x,y) ? "true" : "false");
    // (Test#2)ofLog() << "shapeGui Position: (" << shapeGui->getPosition().x << ", " << shapeGui->getPosition().y << "), Taille: " << shapeGui->getWidth() << "x" << shapeGui->getHeight() << std::endl;
    if (isDelete) {
        deleteSelected();
        isDelete = false;
    }
    else if (isRotate)
    {
        renderer.objetActif->rotate90();
    }
    else if (isExporting)
    {
        saveRenderButtonEvent();
        isExporting = false;
    }
    else if (guiHit(x,y)) {
        draggingShapeGui = true;
        draggingObject = false;
    }
    else {
        if (renderer.hit(x, y) && renderer.objetActif != nullptr) {
            draggingObject = true;
            dragOrigine = { static_cast<float>(x),static_cast<float>(y), 0 };
            positionImageOrigine = { renderer.objetActif->coordinates.x, renderer.objetActif->coordinates.y, 0 };
        }
    }
   }

void Application2d::mouseReleased(int x, int y, int button) {
    ofLog() << "<app::mouseReleased at:(" << x << ", " << y << ")>";
    draggingObject = false;
}

void Application2d::mouseDragged(int x, int y, int button) {
    if (draggingObject && renderer.objetActif != nullptr) {
        renderer.objetActif->coordinates.x = x - dragOrigine.x + positionImageOrigine.x;
        renderer.objetActif->coordinates.y = y - dragOrigine.y + positionImageOrigine.y;
    }

}

bool Application2d::guiHit(int x, int y) {
    
    bool guiHit;


    if (shapeGui->getVisible()) {
            guiHit = y > header->getPosition().y && y < header->getPosition().y + header->getHeight()
            || (x > shapeGui->getPosition().x && x < shapeGui->getPosition().x + shapeGui->getWidth()
                && y > shapeGui->getPosition().y && y < shapeGui->getPosition().y + shapeGui->getHeight() + 80);
    }
    else {
         guiHit = y > header->getPosition().y&& y < header->getPosition().y + header->getHeight();
    }
        return guiHit;
    
}






void Application2d::calculHistogram() {
    ofLog() << "<app::calculHistogram>";

    int redHist[256] = { 0 };
    int blueHist[256] = { 0 };
    int greenHist[256] = { 0 };

    if (dynamic_cast<Image*>(renderer.objetActif) != nullptr) {
        Image image = *dynamic_cast<Image*>(renderer.objetActif);
        ofPixels pixels = image.dataImg.getPixels();
        int largeurImage = image.dataImg.getWidth();
        int hauteurImage = image.dataImg.getHeight();

        for (int i = 0; i < hauteurImage; i++) {
            for (int j = 0; j < largeurImage; j++) {
                ofColor couleurPixel = pixels.getColor(j, i);
                redHist[couleurPixel.r]++;
                blueHist[couleurPixel.b]++;
                greenHist[couleurPixel.g]++;
            }
        }
    }
    histogramme->setRedHist(redHist);
    histogramme->setBlueHist(blueHist);
    histogramme->setGreenHist(greenHist);

}


void Application2d::RGBtoHSB(float& value) {
    if (ignoreRGB) return;

    ignoreRGB = true;

    // Utilisez "value" si nécessaire pour la logique spécifique ou ignorez-le
    ofColor color(redValue.get(), greenValue.get(), blueValue.get());
    hue.set(color.getHue());
    saturation.set(color.getSaturation());
    brightness.set(color.getBrightness());

    ofLogNotice("RGBtoHSB") << "Mise à jour des valeurs HSB basée sur RGB";

    ignoreRGB = false;
    updateColorPickerObject();
}
void Application2d::HSBtoRGB(float& value) {
    if (ignoreRGB) return;

    ignoreRGB = true;

    // Utilisez "value" si nécessaire pour la logique spécifique ou ignorez-le
    ofColor color = ofColor::fromHsb(hue.get(), saturation.get(), brightness.get());
    redValue.set(color.r);
    greenValue.set(color.g);
    blueValue.set(color.b);
    aValue.set(color.a); // Selon votre logique d'application

    

    ofLogNotice("HSBtoRGB") << "Mise à jour des valeurs RGB et color_picker_object basée sur HSB";

    ignoreRGB = false;
    updateColorPickerObject();
}

void Application2d::updateColorPickerObject() {
    if (ignoreRGB) {
        //std::cout << "updateColorPickerObject skipped due to ignoreRGB" << std::endl;
        return;
    }
    std::cout << "Updating color_picker_object" << std::endl;

    ofColor newColor(redValue, greenValue, blueValue, aValue);
    color_picker_object = newColor;
}

// Démarrer ou reprendre la musique
void Application2d::startMusic(const std::string& filePath) {
    if (!mySound.isPlaying()) {
        mySound.load(filePath); // Charge le fichier
        mySound.play(); // Joue le fichier
        isMusicPaused = false; // Réinitialise l'état de pause
    }
    else if (isMusicPaused) {
        // Si la musique est en pause, la reprendre
        mySound.setPaused(false);
        isMusicPaused = false;
    }
}

// Mettre la musique en pause
void Application2d::pauseMusic() {
    if (mySound.isPlaying() && !isMusicPaused) {
        mySound.setPaused(true);
        isMusicPaused = true; // Met à jour l'état de pause
    }
}

// Reprendre la musique après une pause
void Application2d::resumeMusic() {
    if (mySound.isPlaying() && isMusicPaused) {
        mySound.setPaused(false);
        isMusicPaused = false; // Réinitialise l'état de pause
    }
}

void Application2d::restartMusic() {
    if (mySound.isPlaying() || isMusicPaused) {
        // Arrête la musique en cours pour s'assurer qu'elle peut être redémarrée proprement
        mySound.stop();

        // Remet la lecture audio au début et joue le son
        mySound.play();

        // Réinitialise l'état de pause
        isMusicPaused = false;
    }
}

void Application2d::onHeightChanged(float& height) {
    if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.objetActif);
        shape->height = height;
    }
}

void Application2d::onWidthChanged(float& width) {
    if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.objetActif);
        shape->width = width;
    }
}

void Application2d::onScaleChanged(float& scale) {
    if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
        if (!isScaling) {
            isScaling = true;

            slider_height.set(originalHeight * scale / 10.0);
            slider_width.set(originalWidth * scale / 10.0);

            isScaling = false;
        }

        Shape* shape = dynamic_cast<Shape*>(renderer.objetActif);
        shape->height = (slider_height.get()) * scale/10;
        shape->width = (slider_width.get()) * scale/10;

    }
}

void Application2d::onApexChanged(int& apex) {
    if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.objetActif);
        shape->nbApex = apex;
    }
}

void Application2d::onLineChanged(bool& largeur) {
    if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.objetActif);
        shape->outline = largeur;
    }
}

void Application2d::onColorLineChanged(ofColor& color) {
    if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.objetActif);
        shape->outlineColor = color;
    }
}

/* Test deleted #1
void Application2d::onUpdateShapeSliderEvent(ofxDatGuiSliderEvent e) {
    updateShapeFromUi();
}

void Application2d::onUpdateShapeColorEvent(ofxDatGuiColorPickerEvent e) {
    updateShapeFromUi();
}
*/

void Application2d::onColorPickerObjectChanged(ofColor& color) {
    // Vérifie si la mise à jour provient d'une interaction avec color_picker_object pour éviter les boucles
    if (ignoreRGB) return;
    ignoreRGB = true;

    std::cout << "Color Picker Changed: " << color << std::endl;

    redValue.set(color.r);
    greenValue.set(color.g);
    blueValue.set(color.b);
    aValue.set(color.a);
    float fictif = 0; // Paramètre fictif, puisque la valeur n'est pas utilisée dans la fonction
    

    // Logique spécifique à votre application pour mettre à jour la couleur de remplissage d'une forme
    if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.objetActif);
        shape->fillColor = color; // Utilisez directement la couleur passée en paramètre
    }

    ignoreRGB = false; // Réinitialise le drapeau pour permettre d'autres mises à jour
    RGBtoHSB(fictif);
}

string Application2d::getElementName(string filename) {
    int duplicate = 0;
    for (object2D* element : renderer.vecteurObjets) {
        if (element->originalName == filename) {
            duplicate++;
        }
    }

    if (duplicate > 0) {
        filename = filename + " (" + to_string(duplicate) + ")";
    }
    return filename;
}

void Application2d::addElementToRenderer(object2D* object) {
    renderer.vecteurObjets.push_back(object);
    renderer.objetActif = object;
    renderer.indexActif++;
    calculHistogram();
}

void Application2d::updateShapeFromUi() {
    
}

void Application2d::updateUiFromShape() {
    if (dynamic_cast<Shape*>(renderer.objetActif) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.objetActif);
        slider_height.removeListener(this, &Application2d::onHeightChanged);
        slider_width.removeListener(this, &Application2d::onWidthChanged);
        slider_scale.removeListener(this, &Application2d::onScaleChanged);
        slider_apex.removeListener(this, &Application2d::onApexChanged);
        outline_object.removeListener(this, &Application2d::onLineChanged);
        color_outline_object.removeListener(this, &Application2d::onColorLineChanged);
        color_picker_object.set(shape->fillColor);
        updateColorPickerObject();
        slider_height.set(shape->height);
        slider_width.set(shape->width);
        slider_scale.set(10);
        slider_apex.set(shape->nbApex);
        outline_object.set(shape->outline);
        color_outline_object.set(shape->outlineColor);
        slider_height.addListener(this, &Application2d::onHeightChanged);
        slider_width.addListener(this, &Application2d::onWidthChanged);
        slider_scale.addListener(this, &Application2d::onScaleChanged);
        slider_apex.addListener(this, &Application2d::onApexChanged);
        outline_object.addListener(this, &Application2d::onLineChanged);
        color_outline_object.addListener(this, &Application2d::onColorLineChanged);

        uiUpdate = true;
    }
}


void Application2d::deleteSelected() {
    if (renderer.objetActif != nullptr) {
        renderer.vecteurObjets.erase(find(renderer.vecteurObjets.begin(), renderer.vecteurObjets.end(), renderer.objetActif));

        imageScroller->remove(renderer.indexActif);
        renderer.indexActif = -1;
        renderer.objetActif = nullptr;
    }
}

void Application2d::deleteAll() {

    int taille = renderer.vecteurObjets.size() - 1;
    
        

    renderer.vecteurObjets.clear();

    imageScroller->clear();
        renderer.indexActif = -1;
        renderer.objetActif = nullptr; 
    
}


