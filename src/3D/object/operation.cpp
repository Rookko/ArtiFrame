#include "operation.h"

void Operation::apply() {
	for (Object* obj : targets) {
		obj->translationX += transformation->translationX;
		obj->translationY += transformation->translationY;
		obj->translationZ += transformation->translationZ;

		obj->rotationX += transformation->rotationX * (1000 / 360);
		obj->rotationY += transformation->rotationY * (1000 / 360);
		obj->rotationZ += transformation->rotationZ * (1000 / 360);

		obj->scaleX = obj->scaleX * transformation->scaleX;
		obj->scaleY = obj->scaleY * transformation->scaleY;
		obj->scaleZ = obj->scaleZ * transformation->scaleZ;
	}
}

void Operation::rollback() {
	for (Object* obj : targets) {
		obj->translationX -= transformation->translationX;
		obj->translationY -= transformation->translationY;
		obj->translationZ -= transformation->translationZ;

		obj->rotationX -= transformation->rotationX * (1000 / 360);
		obj->rotationY -= transformation->rotationY * (1000 / 360);
		obj->rotationZ -= transformation->rotationZ * (1000 / 360);

		obj->scaleX = obj->scaleX * (1 / transformation->scaleX);
		obj->scaleY = obj->scaleY * (1 / transformation->scaleY);
		obj->scaleZ = obj->scaleZ * (1 / transformation->scaleZ);
	}
}