// ArtiFrame/3D/renderer3d.h
// Classe responsable du rendu de l'application 3D.

#pragma once

#include "ofMain.h"
#include "object/scene.h"

class Renderer3d
{
public:
	enum RenderMode { Wireframe, Shader };
	enum CameraMode { Perspective, Orthographic };

	ofEasyCam* camera;
	CameraMode cameraMode = Perspective;
	Scene* scene;

	ofLight light;
	ofShader* shader;

	void setup();
	void update();
	void draw(Renderer3d::RenderMode renderMode, vector<Object*> selected);

	void setCameraToPerspective();

	void setCameraToOrthographic();
};