// ArtiFrame/2d/object/image.cpp
// Classe principale des images 2D.

#include "image.h";

void Image::draw(int offsetX, int offsetY) {
	dataImg.draw(object2D::coordinates.x + offsetX,
		object2D::coordinates.y + offsetY);
}

void Image::rotate90() {
	dataImg.rotate90(1);
}