// ArtiFrame/3D/renderer3d.h
// Classe responsable du rendu de l'application 3D.

#pragma once

#include "ofMain.h"
#include "object/scene.h"
#include "./object/light.h"
#include "./object/tess.h"



class Renderer3d
{
public:
	enum RenderMode { Wireframe, Texture, Lambert, Phong, Blinn_Phong, PBR, Tesselation };
	enum CameraMode { Perspective, Orthographic };

	ofEasyCam* camera;
	CameraMode cameraMode = Perspective;
	Scene* scene;


	Light ambiantLight;
	Light pointLight;
	Light directionalLight;
	Light spotLight;


	ofShader* lambert;
	ofShader* phong;
	ofShader* blinnPhong;
	ofShader* TextureMapping;
	ofShader* pbr;
	ofImage cubemap[6];

	TesselatorUtil* tesselator;


	void setup();
	void update();
	void draw(Renderer3d::RenderMode renderMode, vector<Object*> selected);


	void setCameraToPerspective();

	void setCameraToOrthographic();
};