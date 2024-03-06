// ArtiFrame/3D/renderer3d.cpp
// Classe responsable du rendu de l'application 3D.
#include <3D/renderer3d.h>


void Renderer3d::setup()
{
	scene = new Scene();
	camera = new ofEasyCam();

	camera->setupPerspective();
	camera->setDistance(1000);

	shader = new ofShader();
	if (!shader->load("lambert_330_vs.glsl", "lambert_330_fs.glsl")) {
		ofLogError() << "Shader compilation failed!";
		return;
	}

	light.setPointLight();
	light.setDiffuseColor(255);
	light.setGlobalPosition(500, 500, 500);
}

void Renderer3d::update() {
	shader->begin();
	shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
	shader->setUniform3f("color_diffuse", 1, 1, 1);
	shader->setUniform3f("light_position", light.getGlobalPosition());
	shader->end();
}

void Renderer3d::draw(Renderer3d::RenderMode renderMode, vector<Object*> selected)
{
	ofSetColor(ofColor::black);

	if (renderMode != Renderer3d::RenderMode::Wireframe) {
		ofEnableDepthTest();
		ofEnableLighting();
		light.enable();
	}

	shader->begin();

	camera->begin();

	for (Object* object : scene->objects) {

		switch (renderMode) {
		case Renderer3d::RenderMode::Wireframe:
			shader->end();
			object->drawWireframe();
			shader->begin();
			break;
		case Renderer3d::RenderMode::Shader:
			object->drawShader();
			break;
		}

		object->checkIfSelected();
	}

	shader->end();
	camera->end();
	light.disable();
	ofDisableDepthTest();
	ofDisableLighting();
	ofSetColor(ofColor::white);
}

void Renderer3d::setCameraToPerspective() {
	camera->enableOrtho();
}

void Renderer3d::setCameraToOrthographic() {
	camera->disableOrtho();
}
