// ArtiFrame/3D/renderer3d.cpp
// Classe responsable du rendu de l'application 3D.
#include <3D/renderer3d.h>


void Renderer3d::setup()
{
	scene = new Scene();
	camera = new ofEasyCam();

	camera->setupPerspective();
	camera->setDistance(1000);

	TextureMapping = new ofShader();
	TextureMapping->load("shaders/tone_mapping_330_vs.glsl", "shaders/tone_mapping_330_fs.glsl");

	lambert = new ofShader();
	lambert->load("shaders/basic_shader.vert", "shaders/lambert.frag");

	phong = new ofShader();
	phong->load("shaders/basic_shader.vert", "shaders/phong.frag");

	blinnPhong = new ofShader();
	blinnPhong->load("shaders/basic_shader.vert", "shaders/blinn-phong.frag");

	pbr = new ofShader();
	pbr->load("shaders/pbr_330_vs.glsl", "shaders/pbr_330_fs.glsl");

	//Light setup
	ambiantLight.color = ofColor::black;
	pointLight.position = ofVec3f(-200, 200, 200);
	pointLight.brightness = 40;
	pointLight.color = ofColor::white;
	directionalLight.position = ofVec3f(200, 200, 200);
	directionalLight.direction = ofVec3f(1, 1, 1);
	directionalLight.color = ofColor::darkGreen;
	directionalLight.brightness = 40;
	spotLight.position = ofVec3f(-200, -200, -200);
	spotLight.brightness = 40;
	spotLight.direction = ofVec3f(-1, -1, -1);
	spotLight.color = ofColor::darkOrchid;
	spotLight.innerCutoff = 10;
	spotLight.outerCutoff = 15;

}

void Renderer3d::update() {

}

void Renderer3d::draw(Renderer3d::RenderMode renderMode, vector<Object*> selected)
{
	ofPushStyle();

	camera->begin();

	if (renderMode != RenderMode::Wireframe) {
		ofEnableDepthTest();
	}

	if (renderMode == RenderMode::Lambert || renderMode == RenderMode::Phong || renderMode == RenderMode::Blinn_Phong) {

		// Set drawing mode to OF_MESH_FILL for drawing faces
		ofFill();

		ofSetColor(pointLight.color);
		ofDrawSphere(pointLight.position, 20);
		ofSetColor(directionalLight.color);
		ofDrawSphere(directionalLight.position, 20);
		ofSetColor(spotLight.color);
		ofDrawSphere(spotLight.position, 20);
		ofSetColor(ofColor::white);
		
	}

	for (Object* object : scene->objects) {

		switch (renderMode) {
		case RenderMode::Wireframe:
			ofSetColor(ofColor::black);
			object->drawWireframe();
			ofSetColor(ofColor::white);
			break;
		case RenderMode::Texture:
			object->drawTexture(TextureMapping);
			break;
		case RenderMode::Lambert:
			object->drawShader(lambert, ambiantLight, pointLight, directionalLight, spotLight);
			break;
		case RenderMode::Phong:
			object->drawShader(phong, ambiantLight, pointLight, directionalLight, spotLight);
			break;
		case RenderMode::Blinn_Phong:
			object->drawShader(blinnPhong, ambiantLight, pointLight, directionalLight, spotLight);
			break;
		case RenderMode::PBR:
			object->drawPBR(pbr, pointLight, directionalLight, spotLight);
			break;
		}
		ofSetColor(ofColor::orangeRed);
		object->checkIfSelected();
		ofSetColor(ofColor::white);
	}
	camera->end();
	ofDisableDepthTest();
	ofPopStyle();
}

void Renderer3d::setCameraToPerspective() {
	camera->disableOrtho();
}

void Renderer3d::setCameraToOrthographic() {
	camera->enableOrtho();
}
