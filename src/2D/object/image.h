// ArtiFrame/2d/object/image.h
// Classe principale des images 2D.

#pragma once
#include "object2D.h";
#include "ofMain.h";

class Image : public object2D {

public:
	ofImage dataImg;
	string path;
	void draw(int offsetX, int offsetY);

	void rotate90();

};