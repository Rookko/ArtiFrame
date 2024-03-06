// ArtiFrame/3d/application3d.cpp
// Classe principale de l'application 3D.
#include "application3d.h"
#include "./3D/object/object.h"
#include "./3d/object/transformation.h"
#include "./3d/object/operation.h"
#include "./3d/object/cylinderPrimitive.h"
#include "./3d/object/spherePrimitive.h"
#include "./3d/object/loadedFile.h"

vector<Object*> everything;
vector<Object*> selection;
stack<Operation*> history;
stack<Operation*> historyUndone;

//ofxDatGui* objectMenu;

void Application3d::setup(int buttonSize){
	ofLog() << "<app::setup3d>";
	optionWidth = buttonSize;

    renderer.setup();
	setup3DTaskbar();

    objectScrollView = new ofxDatGuiScrollView("My scroll view", 100);
    objectScrollView->setWidth(255);
    objectScrollView->setPosition(ofGetWidth() - 255, header->getHeight() - 1);
    objectScrollView->onScrollViewEvent(this, &Application3d::onObjectSelection);

    selectionScrollView = new ofxDatGuiScrollView("My scroll view", 100);
    selectionScrollView->setPosition(300, header->getHeight() - 1);
    selectionScrollView->setOpacity(0.1);

    transformationMenu = new ofxDatGui(300, 300);
    transformationMenu->addLabel("Transformation Menu");
    transformationMenu->addHeader(":: Click here to drag ::");
    vector<string> transformationOptions = { "Translation", "Rotation", "Proportion" };
    transformationDropdown = transformationMenu->addDropdown("Transformation Type", transformationOptions);
    xAxisSlider = transformationMenu->addSlider("X", -1000, 1000, 0);
    yAxisSlider = transformationMenu->addSlider("Y", -1000, 1000, 0);
    zAxisSlider = transformationMenu->addSlider("Z", -1000, 1000, 0);
    ofxDatGuiButton* applyButton = transformationMenu->addButton("Apply");
    applyButton->onButtonEvent(this, &Application3d::onApplyTransformationEvent);
   // ofxDatGuiButton* changeCameraButton = objectMenu->addButton("Switch Camera Mode");
    //changeCameraButton->onButtonEvent(this, &Application3d::onChangeCameraMode);

 
}

void Application3d::draw() {
    renderer.draw(getRenderMode(), selection);
    /*
    if (exporting && ofGetFrameNum() % 2 == 0) {
        ofLog() << "exporting";
        ofImage img;
        img.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        string filename = "/export/" + to_string(ofGetFrameNum()) + ".png";
        img.save(filename);
    }
    */

    objectScrollView->draw();
    selectionScrollView->setHeight(selectionScrollView->getNumItems() * 26);
    selectionScrollView->draw();


    float x = static_cast<float>(ofGetMouseX());
    float y = static_cast<float>(ofGetMouseY());
/*
    if (exporting) {
        ofHideCursor();
        downloadCursor->draw(x - (downloadCursor->getWidth() / 2), y - (downloadCursor->getWidth() / 2));
    }
    else {
        ofShowCursor();
    }
    */
}

void Application3d::update() {
    renderer.update();
    objectScrollView->update();

    for (int i = 0; i < everything.size(); i++) {
        selectionScrollView->remove(0);
    }

    for (Object* object : everything) {
        object->selected = false;

        if (object->rotation_animation)
            object->rotationY = ofGetFrameNum() * 0.3;

        if (object->translation_animation) {
            int pos = ofGetFrameNum() % 360;
            if (pos >= 180) {
                object->translationX += 5;
            }
            else {
                object->translationX -= 5;
            }
        }
    }

    for (Object* object : selection) {
        selectionScrollView->add(object->name);
        object->selected = true;
    }

    if (isWPressed)
        renderer.camera->dolly(-5);
    if (isSPressed)
        renderer.camera->dolly(5);
    if (isAPressed)
        renderer.camera->truck(-5);
    if (isDPressed)
        renderer.camera->truck(5);
    if (isQPressed)
        renderer.camera->boom(5);
    if (isEPressed)
        renderer.camera->boom(-5);




}

void Application3d::exit(){}

void Application3d::showUi()
{
    fileMenu->setVisible(true);
    addMenu->setVisible(true);
    editMenu->setVisible(true);
    renderMenu->setVisible(true);
    header->setVisible(true);
    transformationMenu->setVisible(true);
    cameraMenu->setVisible(true);
}

void Application3d::hideUi()
{
    fileMenu->setVisible(false);
    addMenu->setVisible(false);
    editMenu->setVisible(false);
    renderMenu->setVisible(false);
    header->setVisible(false);
    transformationMenu->setVisible(false);
    cameraMenu->setVisible(false);
}

void Application3d::setupButton(ofxDatGuiFolder* folder, const std::string& label, void (Application3d::* eventHandler)(const ofxDatGuiButtonEvent&))
{
	ofxDatGuiButton* button = folder->addButton(label); // Ajoute un bouton au folder avec le label spécifié.
	button->onButtonEvent(this, eventHandler); // Lie le bouton à un gestionnaire d'événements.
}

// Fonction pour configurer un menu avec plusieurs boutons.
void Application3d::setupMenu(ofxDatGui*& menu, const std::string& title, int positionX, std::initializer_list<std::string> buttons)
{
    menu = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT); // Crée un nouveau menu avec ancrage en haut à gauche.
    menu->setWidth(optionWidth); // Définit la largeur du menu.
    menu->setPosition(positionX, 0); // Positionne le menu horizontalement avec positionX, verticalement à 0.
    ofxDatGuiFolder* folder = menu->addFolder(title); // Ajoute un dossier au menu avec le titre spécifié.

    // Pour chaque label dans la liste des boutons, ajoute un bouton au folder en utilisant setupButton.
    for (auto& label : buttons) {
        setupButton(folder, label, &Application3d::onAddShapeEvent); // Utilise une fonction hypothétique onAddShapeEvent comme gestionnaire pour tous les boutons.
    }
}

void Application3d::rezize3DTaskbar() {
    // Utiliser la dimension la plus petite de la fenêtre pour les calculs de proportion
    int minDimension = std::min(ofGetWidth(), ofGetHeight());

    // Setup option width pixel
    optionWidth = minDimension * 0.15f;

    fileMenu->setWidth(optionWidth);
    fileMenu->setPosition(optionWidth, 0);

    addMenu->setWidth(optionWidth);
    addMenu->setPosition(optionWidth * 2, 0);

    editMenu->setWidth(optionWidth);
    editMenu->setPosition(optionWidth * 3, 0);

    renderMenu->setWidth(optionWidth);
    renderMenu->setPosition(optionWidth*4, 0);

    headerLabel->setWidth(ofGetWidth() - optionWidth * 4);
    headerLabel->setPosition(optionWidth * 6, 0);


    cameraMenu->setWidth(optionWidth);
    cameraMenu->setPosition(optionWidth * 5, 0);

}

// Fonction utilisée lorsque bouton ou keycape sont pressés
void Application3d::onAddShapeEvent(const ofxDatGuiButtonEvent& e)
{
    // Identifier le bouton pressé en utilisant event.target->getLabel() ou toute autre propriété pertinente.
    std::string buttonLabel = e.target->getLabel();

    // Fonction pour exporter la scène lorsque bouton Export cliqué.
    if (buttonLabel == "Export")
    {
    }

    // Fonction pour importer un objet lorsque bouton Import cliqué.
    else if (buttonLabel == "Import")
    {
    }

    // Fonction pour ajouter un carré à la scène lorsque bouton Add Sphere cliqué.
    else if (buttonLabel == "Add Sphere")
    {
        addSphere();
    }

    // Fonction pour ajouter un rectangle à la scène lorsque bouton Add Cynlinder cliqué.
    else if (buttonLabel == "Add Cynlinder")
    {
        addCylinder();
    }

    // Fonction pour ajouter un Circle à la scène lorsque bouton Add Monkey cliqué.
    else if (buttonLabel == "Add Monkey")
    {

    }

    else if (buttonLabel == "Deleted")
    {

    }

    else if (buttonLabel == "Deleted All")
    {
     
    }
    else if (buttonLabel == "WireFrame")
    {
        renderMode = "Wireframe";
    }

    else if (buttonLabel == "Shader")
    {
        renderMode = "Shader";
    }
    // Ajouter des cas supplémentaires selon les besoins pour d'autres formes.


// Fonction principale pour configurer la barre d'outils 2D.
void Application3d::setup3DTaskbar()
{
    // Configure le menu 'File' avec un bouton 'Export'.
    setupMenu(fileMenu, "File", optionWidth, { "Export", "Import" });
    // Configure le menu 'Add' avec plusieurs boutons pour ajouter différentes formes.
    setupMenu(addMenu, "Add", optionWidth * 2, { "Add Sphere", "Add Cynlinder", "Add Monkey"});

    setupMenu(editMenu, "Edit", optionWidth * 3, { "Undo", "Redo", "Deleted", "Deleted All" });

    setupMenu(renderMenu, "Render Mode", optionWidth * 4, { "WireFrame", "Shader"});

    setupMenu(cameraMenu, "Camera Menu", optionWidth * 5, {"Perspective", "Orthogonale"});


    header = new ofxDatGui(ofGetWidth() - optionWidth * 6, 0);
    headerLabel = header->addLabel("ArtiFrame 3D");
    headerLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    headerLabel->setWidth(ofGetWidth() - optionWidth * 6);
    headerLabel->setPosition(optionWidth * 6, 0);
    headerLabel->setStripeVisible(false);
}

void  Application3d::keyPressed(int key){

    if (key == 119) // w
        isWPressed = true;
    if (key == 97) // a
        isAPressed = true;
    if (key == 115) // s
        isSPressed = true;
    if (key == 100) // d
        isDPressed = true;
    if (key == 113) // q
        isQPressed = true;
    if (key == 101) // e
        isEPressed = true;


}

void  Application3d::keyReleased(int key){
    if (key == 119) // w
        isWPressed = false;
    if (key == 97) // a
        isAPressed = false;
    if (key == 115) // s
        isSPressed = false;
    if (key == 100) // d
        isDPressed = false;
    if (key == 113) // q
        isQPressed = false;
    if (key == 101) // e
        isEPressed = false;
}

void Application3d::mousePressed(int x, int y, int button){}

void  Application3d::mouseReleased(int x, int y, int button){}

void Application3d::windowResized(int w, int h) {
    rezize3DTaskbar();
    objectScrollView->setPosition(ofGetWidth() - 255, header->getHeight() - 1);
}


void Application3d::onObjectSelection(ofxDatGuiScrollViewEvent e) {
    Object* obj = everything.at(e.target->getIndex());
    if (find(selection.begin(), selection.end(), obj) != selection.end()) {
        selection.erase(std::find(selection.begin(), selection.end(), obj));
    }
    else {
        selection.push_back(obj);
    }
}

void Application3d::onApplyTransformationEvent(ofxDatGuiButtonEvent e) {
    Transformation* transformation = new Transformation();

    if (transformationDropdown->getSelected()->getLabel() == "Translation") {
        transformation->translationX = xAxisSlider->getValue();
        transformation->translationY = yAxisSlider->getValue();
        transformation->translationZ = zAxisSlider->getValue();
    }
    else if (transformationDropdown->getSelected()->getLabel() == "Rotation") {
        transformation->rotationX = xAxisSlider->getValue();
        transformation->rotationY = yAxisSlider->getValue();
        transformation->rotationZ = zAxisSlider->getValue();
    }
    else if (transformationDropdown->getSelected()->getLabel() == "Proportion") {
        transformation->scaleX = xAxisSlider->getValue();
        transformation->scaleY = yAxisSlider->getValue();
        transformation->scaleZ = zAxisSlider->getValue();
    }
    Operation* operation = new Operation();
    operation->targets = selection;
    operation->transformation = transformation;
    operation->apply();
    history.push(operation);
}

void Application3d::addCylinder() {
    CylinderPrimitive* cylinderPrimitive = new CylinderPrimitive();
    cylinderPrimitive->cylinder = new ofCylinderPrimitive();
    cylinderPrimitive->cylinder->setRadius(100);
    cylinderPrimitive->cylinder->setHeight(200);
    cylinderPrimitive->cylinder->setResolutionRadius(30);
    cylinderPrimitive->cylinder->setPosition({ 0, 0, 0 });

    std::string filename = "cylinder";
    cylinderPrimitive->originalName = filename;
    filename = getElementName(filename);
    addObject(cylinderPrimitive, filename);
}

void Application3d::addSphere() {
    SpherePrimitive* spherePrimitive = new SpherePrimitive();
    spherePrimitive->sphere = new ofSpherePrimitive();
    spherePrimitive->sphere->setRadius(100);
    spherePrimitive->sphere->setPosition({ 0, 0, 0 });

    std::string filename = "sphere";
    spherePrimitive->originalName = filename;
    filename = getElementName(filename);
    addObject(spherePrimitive, filename);
}

void Application3d::addObject(Object* object, string filename) {
    if (selection.empty()) {
        object->name = filename;
        renderer.scene->objects.push_back(object);
    }
    else {
        Object* parent = selection.at(0);
        object->name = filename;
        parent->children.push_back(object);
        object->parent = parent;
    }

    objectScrollView->add(object->name);

    everything.push_back(object);

    selection.clear();
    selection.push_back(object);
}

string Application3d::getElementName(string filename) {
    int duplicate = 0;
    for (Object* element : getAllElementFromScene()) {
        if (element->originalName == filename) {
            duplicate++;
        }
    }

    if (duplicate > 0) {
        filename = filename + " (" + std::to_string(duplicate) + ")";
    }
    return filename;
}

std::vector<Object*> Application3d::getAllElementFromScene() {
    std::vector<Object*> everything;

    for (Object* children : renderer.scene->objects) {
        everything.push_back(children);
        std::vector<Object*> everythingFromChildren = getAllElementFromObject(children);
        everything.insert(everything.end(), everythingFromChildren.begin(), everythingFromChildren.end());
    }

    return everything;
}

std::vector<Object*> Application3d::getAllElementFromObject(Object* object) {
    std::vector<Object*> everything;

    for (Object* children : object->children) {
        std::vector<Object*> everythingFromChildren = getAllElementFromObject(children);
        everything.insert(everything.end(), everythingFromChildren.begin(), everythingFromChildren.end());
    }

    return everything;
}

Renderer3d::RenderMode Application3d::getRenderMode() {
    if (renderMode == "Wireframe")
        return Renderer3d::RenderMode::Wireframe;
    if (renderMode == "Shader")
        return Renderer3d::RenderMode::Shader;
}


