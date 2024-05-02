#pragma once

#include "ofMain.h"
#include <scene.h>

class RayTracer {
public:
	void run(ofCamera* camera, Scene* scene);
};