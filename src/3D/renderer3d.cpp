// ArtiFrame/3D/renderer3d.cpp
// Classe responsable du rendu de l'application 3D.
#include <3D/renderer3d.h>


void Renderer3d::setup()
{
	scene = new Scene();
	camera = new ofEasyCam();

	camera->setupPerspective();
	camera->setDistance(1000);

	toneMapping = new ofShader();
	toneMapping->load("shaders/tone_mapping_330_vs.glsl", "shaders/tone_mapping_330_fs.glsl");

	lambert = new ofShader();
	lambert->load("shaders/basic_shader.vert", "shaders/lambert.frag");

	phong = new ofShader();
	phong->load("shaders/basic_shader.vert", "shaders/phong.frag");

	blinnPhong = new ofShader();
	blinnPhong->load("shaders/basic_shader.vert", "shaders/blinn-phong.frag");

	pbr = new ofShader();
	pbr->load("shaders/pbr_330_vs.glsl", "shaders/pbr_330_fs.glsl");


	light.setPointLight();
	light.setDiffuseColor(255);
	light.setGlobalPosition(500, 500, 500);
}

void Renderer3d::update() {

}

void Renderer3d::draw(Renderer3d::RenderMode renderMode, vector<Object*> selected)
{
	ofSetColor(ofColor::black);

	if (renderMode != Renderer3d::RenderMode::Wireframe) {
		ofEnableDepthTest();
		ofEnableLighting();
		light.enable();
	}


	camera->begin();

	if (renderMode == RenderMode::Lambert || renderMode == RenderMode::Phong || renderMode == RenderMode::Blinn_Phong) {
	}

	for (Object* object : scene->objects) {

		switch (renderMode) {
		case RenderMode::Wireframe:
			ofSetColor(ofColor::black);
			object->drawWireframe();
			ofSetColor(ofColor::white);
			break;
		case RenderMode::Texture:
			object->drawTexture(toneMapping);
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

		camera->end();
		light.disable();
		ofDisableDepthTest();
		ofDisableLighting();
		ofSetColor(ofColor::white);
	}
}

void Renderer3d::setCameraToPerspective() {
	camera->disableOrtho();
}

void Renderer3d::setCameraToOrthographic() {
	camera->enableOrtho();
}
