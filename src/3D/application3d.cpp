// ArtiFrame/3d/application3d.cpp
// Classe principale de l'application 3D.
#include "application3d.h"
#include "./3D/object/object.h"
#include "./3d/object/transformation.h"
#include "./3d/object/operation.h"
#include "./3d/object/cylinderPrimitive.h"
#include "./3d/object/spherePrimitive.h"
#include "./3d/object/cubePrimitive.h"
#include "./3d/object/loadedFile.h"
#include "./3D/object/curve.h"
#include "./3D/object/surface.h"
#include "./3D/object/customObject.h"


vector<Object*> allObject;
vector<Object*> selectionObjet;
stack<Operation*> history;
stack<Operation*> historyUndone;


void Application3d::setup(int buttonSize) {
    ofLog() << "<app::setup3d>";
    optionWidth = buttonSize;

    renderer.setup();
    setup3DTaskbar();

    objectScrollView = new ofxDatGuiScrollView("Graph of scene", 100);
    objectScrollView->setWidth(255);
    objectScrollView->setPosition(ofGetWidth() - 255, header->getHeight() - 1);
    objectScrollView->onScrollViewEvent(this, &Application3d::onObjectSelection);

    selectionScrollView = new ofxDatGuiScrollView("Selected object", 100);
    selectionScrollView->setPosition(optionWidth * 6, header->getHeight() - 1);
    selectionScrollView->setOpacity(0.1);

    objectMenu = new ofxDatGui(300, 300);
    objectMenu->addLabel("|-----------------------Object Menu-----------------------|");
    objectMenu->addLabel("|~Transformation Menu~|");
    objectMenu->addHeader(":: Click here to drag ::");
    vector<string> transformationOptions = { "Translation", "Rotation", "Proportion" };
    transformationDropdown = objectMenu->addDropdown("Transformation Type", transformationOptions);
    xAxisSlider = objectMenu->addSlider("X", -1000, 1000, 0);
    yAxisSlider = objectMenu->addSlider("Y", -1000, 1000, 0);
    zAxisSlider = objectMenu->addSlider("Z", -1000, 1000, 0);
    ofxDatGuiButton* applyButton = objectMenu->addButton("Apply");
    applyButton->onButtonEvent(this, &Application3d::onApplyTransformationEvent);

    transformationDropdown->onDropdownEvent([&](ofxDatGuiDropdownEvent e) {});   //Remove  [WARNING] :: Event Handler Not Set form console
    xAxisSlider->onSliderEvent([&](ofxDatGuiSliderEvent e) { });
    yAxisSlider->onSliderEvent([&](ofxDatGuiSliderEvent e) { });
    zAxisSlider->onSliderEvent([&](ofxDatGuiSliderEvent e) {});

    objectMenu->addLabel("|~Animation Menu~|");
    ofxDatGuiButton* enableTurntableBtn = objectMenu->addButton("Enable Turntable");
    enableTurntableBtn->onButtonEvent(this, &Application3d::onEnableTurntable);
    ofxDatGuiButton* enableTranslationAnimBtn = objectMenu->addButton("Enable Translation Animation");
    enableTranslationAnimBtn->onButtonEvent(this, &Application3d::onEnableTranslationAnimation);
   
    objectMenu->addLabel("|~Texture Menu~|");
    vector<string> magFilterOptions = { "Nearest", "Linear" };
    textureMagFilterDropdown = objectMenu->addDropdown("Image Scaling", magFilterOptions);
    textureMagFilterDropdown->onDropdownEvent(this, &Application3d::onTextureMagFilterDropdownSelection);
    vector<string> filterOptions = { "None", "Gaussian Blur", "Sharpen", "Edge Detect" };
    textureFilterDropdown = objectMenu->addDropdown("Texture Filter", filterOptions);
    textureFilterDropdown->onDropdownEvent(this, &Application3d::onTextureFilterDropdownSelection);
    objectMenu->addLabel("Tone Mapping");
    exposureSlider = objectMenu->addSlider("Exposure", 0, 5, 1);
    exposureSlider->onSliderEvent(this, &Application3d::onToneMappingEvent);
    gammaSlider = objectMenu->addSlider("Gamma", 0, 5, 2.2);
    gammaSlider->onSliderEvent(this, &Application3d::onToneMappingEvent);

    objectMenu->addLabel("|~Material Menu~|");
    ofxDatGuiFolder* colorFolder = objectMenu->addFolder("Color");
    materialAmbiantCP = colorFolder->addColorPicker("Ambiant", ofFloatColor(0.1, 0.1, 0.1));
    materialAmbiantCP->onColorPickerEvent(this, &Application3d::onMaterialColorChangeEvent);
    materialDiffuseCP = colorFolder->addColorPicker("Diffuse", ofFloatColor(0.0, 0.6, 0.6));
    materialDiffuseCP->onColorPickerEvent(this, &Application3d::onMaterialColorChangeEvent);
    materialSpecularCP = colorFolder->addColorPicker("Specular", ofFloatColor(1.0, 0.0, 1.0));
    materialSpecularCP->onColorPickerEvent(this, &Application3d::onMaterialColorChangeEvent);
    ofxDatGuiFolder* factorFolder = objectMenu->addFolder("Factor");
    materialMetallicSlider = factorFolder->addSlider("Metallic", 0, 1, 0.5);
    materialMetallicSlider->onSliderEvent(this, &Application3d::onMaterialFactorChangeEvent);
    materialRoughnessSlider = factorFolder->addSlider("Roughness", 0, 1, 0.5);
    materialRoughnessSlider->onSliderEvent(this, &Application3d::onMaterialFactorChangeEvent);
    materialOcclusionSlider = factorFolder->addSlider("Occlusion", 0, 5, 1);
    materialOcclusionSlider->onSliderEvent(this, &Application3d::onMaterialFactorChangeEvent);
    materialBrightnessSlider = factorFolder->addSlider("Brightness", 0, 5, 1);
    materialBrightnessSlider->onSliderEvent(this, &Application3d::onMaterialFactorChangeEvent);
    materialFresnelIorColorPicker = objectMenu->addColorPicker("Fresnel IOR");
    materialFresnelIorColorPicker->onColorPickerEvent(this, &Application3d::onMaterialFactorIorChangeEvent);

    objectMenu->addLabel("|~Light Menu~|");
    ofxDatGuiFolder* ambiantLightFolder = objectMenu->addFolder("Ambiant Light");
    ambiantLightColor = ambiantLightFolder->addColorPicker("Color", renderer.ambiantLight.color);
    ambiantLightColor->onColorPickerEvent(this, &Application3d::onLightColorChangeEvent);
    ofxDatGuiFolder* pointLightFolder = objectMenu->addFolder("Point Light");
    pointLightColor = pointLightFolder->addColorPicker("Color", renderer.pointLight.color);
    pointLightColor->onColorPickerEvent(this, &Application3d::onLightColorChangeEvent);
    pointLightBrightness = pointLightFolder->addSlider("Brightness", 0, 64, 40);
    pointLightBrightness->onSliderEvent(this, &Application3d::onLightBrightnessChangeEvent);
    ofxDatGuiFolder* directionalLightFolder = objectMenu->addFolder("Directional Light");
    directionalLightColor = directionalLightFolder->addColorPicker("Color", renderer.directionalLight.color);
    directionalLightColor->onColorPickerEvent(this, &Application3d::onLightColorChangeEvent);
    directionalLightBrightness = directionalLightFolder->addSlider("Brightness", 0, 64, 40);
    directionalLightBrightness->onSliderEvent(this, &Application3d::onLightBrightnessChangeEvent);
    ofxDatGuiFolder* spotLightFolder = objectMenu->addFolder("Spot Light");
    spotLightColor = spotLightFolder->addColorPicker("Color", renderer.spotLight.color);
    spotLightColor->onColorPickerEvent(this, &Application3d::onLightColorChangeEvent);
    spotLightBrightness = spotLightFolder->addSlider("Brightness", 0, 64, 40);
    spotLightBrightness->onSliderEvent(this, &Application3d::onLightBrightnessChangeEvent);
    objectMenu->addFooter();

    objectMenu->setTheme(new ofxDatGuiThemeSmoke());

 

    curveMenu = new ofxDatGui(330, 550);
    curveMenu->addHeader("Bezier Curve Menu");
    vector<string> curveControlPoints = { "1", "2", "3", "4", "5" };
    curvePointControlDropdown = curveMenu->addDropdown("Control Point", curveControlPoints);
    curveXSlider = curveMenu->addSlider("Position X", -500, 500, 0);
    curveXSlider->onSliderEvent(this, &Application3d::onCurveControlPointPositionChangeEvent);
    curveYSlider = curveMenu->addSlider("Position Y", -500, 500, 0);
    curveYSlider->onSliderEvent(this, &Application3d::onCurveControlPointPositionChangeEvent);
    curveZSlider = curveMenu->addSlider("Position Z", -500, 500, 0);
    curveZSlider->onSliderEvent(this, &Application3d::onCurveControlPointPositionChangeEvent);


    surfaceMenu = new ofxDatGui(330, 550);
    surfaceMenu->addHeader("Bezier Surface Menu");
    vector<string> surfaceControlPoints = { "0", "1", "2", "3", "4", "5", "6", "7", "8" };
    surfacePointControlDropdown = surfaceMenu->addDropdown("Control Point", surfaceControlPoints);
    surfacePointControlDropdown->onDropdownEvent(this, &Application3d::onSurfacePointControlSelectionEvent);
    surfaceXSlider = surfaceMenu->addSlider("Position X", -500, 500, 0);
    surfaceXSlider->onSliderEvent(this, &Application3d::onSurfaceControlPointPositionChangeEvent);
    surfaceYSlider = surfaceMenu->addSlider("Position Y", -500, 500, 0);
    surfaceYSlider->onSliderEvent(this, &Application3d::onSurfaceControlPointPositionChangeEvent);
    surfaceZSlider = surfaceMenu->addSlider("Position Z", -500, 500, 0);
    surfaceZSlider->onSliderEvent(this, &Application3d::onSurfaceControlPointPositionChangeEvent);



    // ofxDatGuiButton* changeCameraButton = objectMenu->addButton("Switch Camera Mode");
     //changeCameraButton->onButtonEvent(this, &Application3d::onChangeCameraMode);

    //basicCursor = new ofImage();
    //basicCursor->load("/data/basic_cursor.png");



}

void Application3d::draw() {
    renderer.draw(getRenderMode(), selectionObjet);

    objectScrollView->draw();
    selectionScrollView->setHeight(selectionScrollView->getNumItems() * 26);
    selectionScrollView->draw();


    float x = static_cast<float>(ofGetMouseX());
    float y = static_cast<float>(ofGetMouseY());

    if (!selectionObjet.empty()) {
        if (dynamic_cast<Curve*>(selectionObjet.at(0)) != nullptr) {
            curveMenu->setVisible(true);
        }
        else if (dynamic_cast<Surface*>(selectionObjet.at(0)) != nullptr) {
            surfaceMenu->setVisible(true);
        }
        else {
            curveMenu->setVisible(false);
            surfaceMenu->setVisible(false);
        }
    }
    else {
        curveMenu->setVisible(false);
        surfaceMenu->setVisible(false);
    }
    /*
        if (exporting) {
            ofHideCursor();
            downloadCursor->draw(x - (downloadCursor->getWidth() / 2), y - (downloadCursor->getWidth() / 2));
        }
        else {
            ofShowCursor();
        }
        */
  
        //ofHideCursor();
        //basicCursor->draw(x - (basicCursor->getWidth()/2), y - (basicCursor->getWidth()/2));
        //ofShowCursor();
        
    
}

void Application3d::update() {
    renderer.update();
    objectScrollView->update();



    for (int i = 0; i < allObject.size(); i++) {
        selectionScrollView->remove(0);
    }



    for (Object* object : allObject) {
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

    for (Object* object : selectionObjet) {
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

    ofShowCursor();

    Light* lightToMove;
    if (is1Pressed)
        lightToMove = &renderer.pointLight;
    if (is2Pressed)
        lightToMove = &renderer.directionalLight;
    if (is3Pressed)
        lightToMove = &renderer.spotLight;

    if (is1Pressed || is2Pressed || is3Pressed) {
        ofVec3f worldMouse = renderer.camera->screenToWorld(ofVec3f(ofGetMouseX(), ofGetMouseY(), 0));
        ofVec3f direction = worldMouse - renderer.camera->getPosition();
        direction.normalize();
        glm::vec3 newPoint = renderer.camera->getPosition() + direction * 500;
        lightToMove->position = newPoint;
        lightToMove->direction = newPoint;
    }

  

}

void Application3d::exit() {}

void Application3d::showUi()
{
    fileMenu->setVisible(true);
    addMenu->setVisible(true);
    editMenu->setVisible(true);
    renderMenu->setVisible(true);
    header->setVisible(true);
    objectMenu->setVisible(true);
    cameraMenu->setVisible(true);
    
}

void Application3d::hideUi()
{
    fileMenu->setVisible(false);
    addMenu->setVisible(false);
    editMenu->setVisible(false);
    renderMenu->setVisible(false);
    header->setVisible(false);
    objectMenu->setVisible(false);
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
    renderMenu->setPosition(optionWidth * 4, 0);

    headerLabel->setWidth(ofGetWidth() - optionWidth * 5);
    headerLabel->setPosition((optionWidth * 6)+1, 0);


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
       // hideUi();
      //  exportRender();
      //  showUi();
    }
    // Fonction pour importer un objet lorsque bouton Import cliqué.
    else if (buttonLabel == "Import 3D Model")
    {
        import();
    }

    else if (buttonLabel == "Import Albedo Texture")
    {
        importTexture();
    }


    else if (buttonLabel == "Import Normal Map")
    {
        importNormalMap();
    }
    else if (buttonLabel == "Import Roughness Map")
    {
        importRoughnesslMap();
    }
    else if (buttonLabel == "Import Metallic Map")
    {
        importMetallicMap();
    }
    else if (buttonLabel == "Import Occlusion Map")
    {
        importOcclusionMap();
    }
    else if (buttonLabel == "Random Texture")
    {
        randomTexture();
    }
    // Fonction pour ajouter un carré à la scène lorsque bouton Add Sphere cliqué.
    else if (buttonLabel == "Add Cube")
    {
        addCube();
    }
    // Fonction pour ajouter un carré à la scène lorsque bouton Add Sphere cliqué.
    else if (buttonLabel == "Add Sphere")
    {
        addSphere();
    }

    // Fonction pour ajouter un rectangle à la scène lorsque bouton Add Cylinder cliqué.
    else if (buttonLabel == "Add Cylinder")
    {

        addCylinder();

    }

    // Fonction pour ajouter un Circle à la scène lorsque bouton Add Monkey cliqué.
    else if (buttonLabel == "Add Monkey")
    {
        addMonkey();
    }
    else if (buttonLabel == "Add Custom Object")
    {
        AddCustomObject();
    }

    else if (buttonLabel == "Deleted")
    {
        deleteSelected();
    }

    else if (buttonLabel == "Deleted All")
    {
        deleteAll();
    }

    else if (buttonLabel == "Perspective") {

        if (renderer.cameraMode == Renderer3d::Orthographic) {
            renderer.setCameraToPerspective();
            renderer.cameraMode = Renderer3d::Perspective;

        }
    }

    else if (buttonLabel == "Orthogonale") {
        if (renderer.cameraMode == Renderer3d::Perspective) {
            renderer.setCameraToOrthographic();
            renderer.cameraMode = Renderer3d::Orthographic;
        }

    }

    else if (buttonLabel == "WireFrame")
    {
        renderMode = "Wireframe";
    }

    else if (buttonLabel == "Lambert")
    {
         renderMode =  "Lambert";
    }

    else if (buttonLabel == "Phong")
    {
        renderMode = "Phong";
    }
    else if (buttonLabel == "Phill-Phong")
    {
        renderMode = "Phill-Phong";
    }
    else if (buttonLabel == "Tesselation")
    {
        renderMode = "Tesselation";
    }
    else if (buttonLabel == "Texture")
    {
        renderMode = "Texture";
    }
    else if (buttonLabel == "PBR")
    {
        renderMode = "PBR";
    }

    else if (buttonLabel == "Add Bezier Curve") {
        AddBezierCurve();
    }

    else if (buttonLabel == "Add Bezier Surface") {
        AddBezierSurface();
    }

    // Ajouter des cas supplémentaires selon les besoins pour d'autres formes.

}



// Fonction principale pour configurer la barre d'outils 3D.
void Application3d::setup3DTaskbar()
{
    // Configure le menu 'File' avec un bouton 'Export'.
    //setupMenu(fileMenu, "File", optionWidth, { "Export", "Import" });
    setupMenu(fileMenu, "File", optionWidth, { "Export", "Import 3D Model", "Import Albedo Texture", "Import Normal Map", "Import Roughness Map", "Import Metallic Map", "Import Occlusion Map", "Random Texture"});
    // Configure le menu 'Add' avec plusieurs boutons pour ajouter différentes formes.
    setupMenu(addMenu, "Add", optionWidth * 2, {"Add Cube", "Add Sphere", "Add Cylinder", "Add Monkey" , "Add Bezier Curve", "Add Bezier Surface", "Add Custom Object"});

    setupMenu(editMenu, "Edit", optionWidth * 3, { "Undo", "Redo", "Deleted", "Deleted All" });

    setupMenu(renderMenu, "Render Mode", optionWidth * 4, { "WireFrame", "Lambert","Phong","Phill-Phong","Tesselation","Texture","PBR"});

    setupMenu(cameraMenu, "Camera Menu", optionWidth * 5, { "Perspective", "Orthogonale" });


    header = new ofxDatGui(ofGetWidth() - optionWidth * 6, 0);
    headerLabel = header->addLabel("ArtiFrame 3D");
    headerLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    headerLabel->setWidth(ofGetWidth() - optionWidth * 6);
    headerLabel->setPosition((optionWidth * 6)+1, 0);
    headerLabel->setStripeVisible(false);
}

void  Application3d::keyPressed(int key) {

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
    if (key == 49) // 1
        is1Pressed = true;
    if (key == 50) // 2
        is2Pressed = true;
    if (key == 51) // 3
        is3Pressed = true;


}

void  Application3d::keyReleased(int key) {
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
    if (key == 49) // 1
        is1Pressed = false;
    if (key == 50) // 2
        is2Pressed = false;
    if (key == 51) // 3
        is3Pressed = false;
}

void Application3d::mousePressed(int x, int y, int button) {}

void  Application3d::mouseReleased(int x, int y, int button) {
    ofLog() << "Mouse release at : " << x << ", " << y;
    if (button == 2 && !selectionObjet.empty()) { // right click
        if (dynamic_cast<CustomObject*>(selectionObjet.at(0)) != nullptr) {
            CustomObject* customObject = dynamic_cast<CustomObject*>(selectionObjet.at(0));
            ofVec3f worldMouse = renderer.camera->screenToWorld(ofVec3f(x, y, 0));
            ofVec3f direction = worldMouse - renderer.camera->getPosition();
            direction.normalize();
            glm::vec3 newPoint = renderer.camera->getPosition() + direction * 500;
            customObject->triangulator->addPoint(newPoint.x, newPoint.y, newPoint.z);
            customObject->triangulator->triangule();
        }
    }
}

void Application3d::windowResized(int w, int h) {
    rezize3DTaskbar();
    objectScrollView->setPosition(ofGetWidth() - 255, header->getHeight() - 1);
    selectionScrollView->setPosition(optionWidth * 6, header->getHeight() - 1);
}


void Application3d::onObjectSelection(ofxDatGuiScrollViewEvent e) {




    Object* obj = allObject.at(e.target->getIndex());
    if (find(selectionObjet.begin(), selectionObjet.end(), obj) != selectionObjet.end()) {
        selectionObjet.erase(std::find(selectionObjet.begin(), selectionObjet.end(), obj));
    }
    else {
        selectionObjet.push_back(obj);
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
    operation->targets = selectionObjet;
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
    if (selectionObjet.empty()) {
        object->name = filename;
        renderer.scene->objects.push_back(object);
    }
    else {
        Object* parent = selectionObjet.at(0);
        object->name = filename;
        parent->children.push_back(object);
        object->parent = parent;
    }

    objectScrollView->add(object->name);

    allObject.push_back(object);

    selectionObjet.clear();
    selectionObjet.push_back(object);
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
    std::vector<Object*> allObject;

    for (Object* children : renderer.scene->objects) {
        allObject.push_back(children);
        std::vector<Object*> allObjectFromChildren = getAllElementFromObject(children);
        allObject.insert(allObject.end(), allObjectFromChildren.begin(), allObjectFromChildren.end());
    }

    return allObject;
}

std::vector<Object*> Application3d::getAllElementFromObject(Object* object) {
    std::vector<Object*> allObject;

    for (Object* children : object->children) {
        std::vector<Object*> allObjectFromChildren = getAllElementFromObject(children);
        allObject.insert(allObject.end(), allObjectFromChildren.begin(), allObjectFromChildren.end());
    }

    return allObject;
}

Renderer3d::RenderMode Application3d::getRenderMode() {
    if (renderMode == "Wireframe")
        return Renderer3d::RenderMode::Wireframe;
    else if (renderMode == "Lambert")
        return Renderer3d::RenderMode::Lambert;
    else if (renderMode == "Phong")
        return Renderer3d::RenderMode::Phong;
    else if (renderMode == "Phill-Phong")
        return Renderer3d::RenderMode::Blinn_Phong;
    else if (renderMode == "Tesselation")
        return Renderer3d::RenderMode::Tesselation;
    else if (renderMode == "Texture")
        return Renderer3d::RenderMode::Texture;
    else if (renderMode == "PBR")
        return Renderer3d::RenderMode::PBR;
}


void Application3d::import() {
    ofLog() << "<import>";
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an object");
    if (openFileResult.bSuccess) {
        ofLog() << "<file import - success>";
        importPath(openFileResult.getPath());
    }
    else {
        ofLog() << "<import(file) - failed>";
    }
}

void Application3d::importPath(string path) {
    ofxAssimpModelLoader* model = new ofxAssimpModelLoader();
    if (model->loadModel(path)) {
        LoadedFile* obj = new LoadedFile();
        obj->model = model;

        // Use ofFilePath to extract the filename from the path
        std::string filename = ofFilePath::getFileName(path);

        obj->originalName = filename;
        filename = getElementName(filename);

        addObject(obj, filename);
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}

void Application3d::addMonkey(){
    importPath("./Monkey.obj");
}

void Application3d::exportRender() {
    // Open a file dialog to get the desired file path and name
    ofFileDialogResult result = ofSystemLoadDialog("Select a folder", true);
    if (result.bSuccess) {
        string folderPath = result.filePath;
        ofLogNotice("Folder Selected") << folderPath;
        
        // Create an ofImage and grab the screen
        ofImage image;
        image.grabScreen(0, 60, ofGetWidth()-255, ofGetHeight() - 60);
        // Save the image to the specified folder with a default name 
        string defaultFileName = "render.png";
        folderPath = ofFilePath::addTrailingSlash(folderPath);


        string filePath = folderPath + defaultFileName;
        image.save(filePath);


        ofLogNotice("Image Saved") << "Render saved to: " << filePath;
    }
    else {
        ofLogNotice("File selectionObjet canceled");
    }
    
}

void Application3d::onEnableTurntable(ofxDatGuiButtonEvent e) {
    for (Object* object : selectionObjet) {
        object->rotation_animation = !object->rotation_animation;
    }
}

void Application3d::onEnableTranslationAnimation(ofxDatGuiButtonEvent e) {
    for (Object* object : selectionObjet) {
        object->translation_animation = !object->translation_animation;
    }
}

void Application3d::deleteSelected() {
    while (!selectionObjet.empty()) {
        Object* object = selectionObjet.at(0);
        auto itInselectionObjet = find(selectionObjet.begin(), selectionObjet.end(), object);
        selectionScrollView->remove(itInselectionObjet - selectionObjet.begin());
        selectionObjet.erase(find(selectionObjet.begin(), selectionObjet.end(), object));

        if (object->parent != nullptr) {
            if (count(object->parent->children.begin(), object->parent->children.end(), object))
                object->parent->children.erase(find(object->parent->children.begin(), object->parent->children.end(), object));
        }

        for (Object* child : object->children) {
            child->parent = nullptr;
            renderer.scene->objects.push_back(child);
        }

        auto it = find(allObject.begin(), allObject.end(), object);
        objectScrollView->remove(it - allObject.begin());
        if (count(renderer.scene->objects.begin(), renderer.scene->objects.end(), object))
            renderer.scene->objects.erase(find(renderer.scene->objects.begin(), renderer.scene->objects.end(), object));
        allObject.erase(it);
        delete object;
    }
}

void Application3d::deleteAll() {
    deleteSelected();
        while (!allObject.empty()) {
            Object* object = allObject.back();  // Get the last element

            // Remove from parent's children
            if (object->parent != nullptr) {
                auto childIt = find(object->parent->children.begin(), object->parent->children.end(), object);
                if (childIt != object->parent->children.end()) {
                    object->parent->children.erase(childIt);
                }
            }

            // Add children to the scene
            for (Object* child : object->children) {
                child->parent = nullptr;
                renderer.scene->objects.push_back(child);
            }

            // Remove from objectScrollView
            auto scrollViewIt = find(allObject.begin(), allObject.end(), object);
            if (scrollViewIt != allObject.end()) {
                objectScrollView->remove(distance(allObject.begin(), scrollViewIt));
            }

            // Remove from scene
            auto sceneIt = find(renderer.scene->objects.begin(), renderer.scene->objects.end(), object);
            if (sceneIt != renderer.scene->objects.end()) {
                renderer.scene->objects.erase(sceneIt);
            }

            // Delete the object
            allObject.pop_back();  // Remove from the end
            delete object;
        }
    }

void Application3d::addCube() {
    CubePrimitive* cubePrimitive = new CubePrimitive();
    cubePrimitive->cube = new ofBoxPrimitive();
    cubePrimitive->cube->setDepth(100);
    cubePrimitive->cube->setWidth(100);
    cubePrimitive->cube->setHeight(100);
    cubePrimitive->cube->setPosition({ 0, 0, 0 });

    std::string filename = "cube";
    cubePrimitive->originalName = filename;
    filename = getElementName(filename);
    addObject(cubePrimitive, filename);
}

void Application3d::importTexture() {
    ofLog() << "<app::import texture>";
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
    if (openFileResult.bSuccess) {
        for (Object* object : selectionObjet) {
            ofLoadImage(object->texture, openFileResult.filePath);
            object->filteredTexture = object->texture;
        }
        ofLog() << "<app::import - success>";
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}

void Application3d::randomTexture() {
    for (Object* object : selectionObjet) {
        ofTexture texture;
        ofPixels pixels;
        int width = 256;
        int height = 256;
        pixels.allocate(width, height, OF_PIXELS_RGB);
        float noiseScale = ofRandom(1);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float noiseValue = ofNoise(x * noiseScale * ofRandom(1), y * noiseScale * ofRandom(1));
                ofColor color(noiseValue * 255, noiseValue * 255, noiseValue * 255);
                pixels.setColor(x, y, color);
            }
        }
        object->texture.allocate(pixels);
        object->filteredTexture.allocate(pixels);
    }
}

void Application3d::importNormalMap() {
    ofLog() << "<app::import normal map>";
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
    if (openFileResult.bSuccess) {
        for (Object* object : selectionObjet) {
            ofLoadImage(object->normalMap, openFileResult.filePath);
        }
        ofLog() << "<app::import - success>";
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}


void Application3d::importMetallicMap() {
    ofLog() << "<app::import metallic map>";
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
    if (openFileResult.bSuccess) {
        for (Object* object : selectionObjet) {
            ofLoadImage(object->metallicMap, openFileResult.filePath);
        }
        ofLog() << "<app::import - success>";
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}


void Application3d::importRoughnesslMap() {
    ofLog() << "<app::import roughness map>";
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
    if (openFileResult.bSuccess) {
        for (Object* object : selectionObjet) {
            ofLoadImage(object->roughnessMap, openFileResult.filePath);
        }
        ofLog() << "<app::import - success>";
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}

void Application3d::importOcclusionMap() {
    ofLog() << "<app::import occlusion map>";
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
    if (openFileResult.bSuccess) {
        for (Object* object : selectionObjet) {
            ofLoadImage(object->occlusionMap, openFileResult.filePath);
        }
        ofLog() << "<app::import - success>";
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}

void Application3d::onLightColorChangeEvent(ofxDatGuiColorPickerEvent e) {
    renderer.ambiantLight.color = ambiantLightColor->getColor();
    renderer.pointLight.color = pointLightColor->getColor();
    renderer.directionalLight.color = directionalLightColor->getColor();
    renderer.spotLight.color = spotLightColor->getColor();
}

void Application3d::onLightBrightnessChangeEvent(ofxDatGuiSliderEvent e) {
    renderer.pointLight.brightness = pointLightBrightness->getValue();
    renderer.directionalLight.brightness = directionalLightBrightness->getValue();
    renderer.spotLight.brightness = spotLightBrightness->getValue();
}

void Application3d::AddBezierCurve() {
    Curve* curve = new Curve();
    std::string curveName = "courbe Bezier";
    curve->originalName = curveName;
    curveName = getElementName(curveName);
    curve->name = curveName;
    addObject(curve, curveName);
}

void Application3d::AddBezierSurface() {
    Surface* surface = new Surface();
    surface->surfaceBezierInstance->setup(250, 250, 3, 8);
    std::string surfaceName = "surface Bezier";
    surface->originalName = surfaceName;
    surfaceName = getElementName(surfaceName);
    surface->name = surfaceName;
    addObject(surface, surfaceName);
}

void Application3d::AddCustomObject() {
    CustomObject* customObject = new CustomObject();
    std::string filename = "Custom Object";
    customObject->originalName = filename;
    filename = getElementName(filename);
    customObject->name = filename;
    addObject(customObject, filename);
}


void Application3d::onCurveControlPointPositionChangeEvent(ofxDatGuiSliderEvent e) {
    if (!selectionObjet.empty()) {
        if (dynamic_cast<Curve*>(selectionObjet.at(0)) != nullptr) {
            Curve* curve = dynamic_cast<Curve*>(selectionObjet.at(0));
            int controlPointsIndex = curvePointControlDropdown->getSelected()->getIndex();
            curve->controlPoints[controlPointsIndex].x = curveXSlider->getValue();
            curve->controlPoints[controlPointsIndex].y = curveYSlider->getValue();
            curve->controlPoints[controlPointsIndex].z = curveZSlider->getValue();
        }
    }
}

void Application3d::onAddBezierSurfaceEvent(ofxDatGuiButtonEvent e) {
    Surface* surface = new Surface();
    surface->surfaceBezierInstance->setup(250, 250, 3, 36);
    std::string filename = "Bezier Surface";
    surface->originalName = filename;
    filename = getElementName(filename);
    surface->name = filename;
    addObject(surface, filename);


}


void Application3d::onSurfaceControlPointPositionChangeEvent(ofxDatGuiSliderEvent e) {
    if (!selectionObjet.empty()) {
        if (dynamic_cast<Surface*>(selectionObjet.at(0)) != nullptr) {
            Surface* surface = dynamic_cast<Surface*>(selectionObjet.at(0));
            int controlPointsIndex = surfacePointControlDropdown->getSelected()->getIndex();
            surface->surfaceBezierInstance->modifierPointControle(controlPointsIndex,
                ofVec3f(surfaceXSlider->getValue(), surfaceYSlider->getValue(), surfaceZSlider->getValue()));
            surface->surfaceBezierInstance->update();
        }
    }
}

void Application3d::onSurfacePointControlSelectionEvent(ofxDatGuiDropdownEvent e) {
    if (!selectionObjet.empty()) {
        if (dynamic_cast<Surface*>(selectionObjet.at(0)) != nullptr) {
            Surface* surface = dynamic_cast<Surface*>(selectionObjet.at(0));
            int controlPointsIndex = surfacePointControlDropdown->getSelected()->getIndex();
            ofVec3f controlPoint = surface->surfaceBezierInstance->getPointControle(controlPointsIndex);
            surfaceXSlider->setValue(controlPoint.x);
            surfaceYSlider->setValue(controlPoint.y);
            surfaceZSlider->setValue(controlPoint.z);
        }
    }
}

void Application3d::onTextureMagFilterDropdownSelection(ofxDatGuiDropdownEvent e) {
    for (Object* object : selectionObjet) {
        if (e.child == 0)
            object->magFilter = GL_NEAREST;
        if (e.child == 1)
            object->magFilter = GL_LINEAR;
    }
}

void Application3d::onTextureFilterDropdownSelection(ofxDatGuiDropdownEvent e) {
    float kernel[9];
    for (Object* object : selectionObjet) {
        if (e.child == 0) {
            object->filteredTexture = object->texture;
            return;
        }
        if (e.child == 1) {
            float gaussianBlurKernel[9] =
            { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
               1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
               1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };
            copy(begin(gaussianBlurKernel), end(gaussianBlurKernel), begin(kernel));
        }
        if (e.child == 2) {
            float sharpenKernel[9] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
            copy(begin(sharpenKernel), end(sharpenKernel), begin(kernel));
        }
        if (e.child == 3) {
            float edgeDetectKernel[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
            copy(begin(edgeDetectKernel), end(edgeDetectKernel), begin(kernel));
        }

        int width = object->texture.getWidth();
        int height = object->texture.getHeight();

        ofPixels pixels;
        object->texture.readToPixels(pixels);
        ofImage redImage, greenImage, blueImage;
        redImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        greenImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        blueImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = y * width + x;
                redImage.getPixels()[index] = pixels[index * 3];
                greenImage.getPixels()[index] = pixels[index * 3 + 1];
                blueImage.getPixels()[index] = pixels[index * 3 + 2];
            }
        }
        redImage.update();
        greenImage.update();
        blueImage.update();

        ofImage filteredImage;
        filteredImage.allocate(width, height, OF_IMAGE_COLOR);
        for (int c = 0; c < 3; c++) {
            for (int y = 1; y < height - 1; y++) {
                for (int x = 1; x < width - 1; x++) {
                    float sum = 0.0;
                    for (int j = -1; j <= 1; j++) {
                        for (int i = -1; i <= 1; i++) {
                            int index = (y + j) * width + (x + i);
                            sum += pixels[index * 3 + c] * kernel[(j + 1) * 3 + (i + 1)];
                        }
                    }
                    int index = y * width + x;
                    filteredImage.getPixels()[index * 3 + c] = ofClamp(sum, 0, 255);
                }
            }
        }
        filteredImage.update();
        object->filteredTexture.allocate(filteredImage);
    }
}

void Application3d::onToneMappingEvent(ofxDatGuiSliderEvent e) {
    for (Object* object : selectionObjet) {
        object->exposure = exposureSlider->getValue();
        object->gamma = gammaSlider->getValue();
    }
}

void Application3d::onMaterialColorChangeEvent(ofxDatGuiColorPickerEvent e) {
    for (Object* object : selectionObjet) {
        object->materialAmbiant = materialAmbiantCP->getColor();
        object->materialDiffuse = materialDiffuseCP->getColor();
        object->materialSpecular = materialSpecularCP->getColor();
    }
}

void Application3d::onMaterialFactorChangeEvent(ofxDatGuiSliderEvent e) {
    for (Object* object : selectionObjet) {
        object->materialMetallic = materialMetallicSlider->getValue();
        object->materialRoughness = materialRoughnessSlider->getValue();
        object->materialOcclusion = materialOcclusionSlider->getValue();
        object->materialBrightness = materialBrightnessSlider->getValue();
    }
}

void Application3d::onMaterialFactorIorChangeEvent(ofxDatGuiColorPickerEvent e) {
    for (Object* object : selectionObjet) {
        object->materialIor = materialFresnelIorColorPicker->getColor();
    }
}