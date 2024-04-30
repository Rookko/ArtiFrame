#include "object.h"

void Object::drawWireframe()
{
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawWireframeOverride();

	for (Object* child : children) {
		child->drawWireframe();
	}

	ofPopMatrix();
}

void Object::drawTexture(ofShader* toneMapping) {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	if (filteredTexture.isAllocated()) {
		filteredTexture.setTextureMinMagFilter(magFilter, magFilter);
		filteredTexture.bind();

		toneMapping->begin();
		toneMapping->setUniformTexture("image", filteredTexture, 1);
		toneMapping->setUniform1f("tone_mapping_exposure", exposure);
		toneMapping->setUniform1f("tone_mapping_gamma", gamma);
		toneMapping->setUniform1i("tone_mapping_toggle", false);
	}

	drawShaderOverride();


	if (filteredTexture.isAllocated()) {
		filteredTexture.unbind();
		toneMapping->end();
	}

	for (Object* child : children) {
		child->drawTexture(toneMapping);
	}

	ofPopMatrix();
}

void Object::drawShader(ofShader* shader, Light ambiantLight, Light pointLight, Light directionalLight, Light spotLight) {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	shader->begin();
	shader->setUniform3f("pointLight.position", pointLight.position);
	shader->setUniform1f("pointLight.brightness", pointLight.brightness);
	ofFloatColor pointLightcolor = pointLight.color;
	shader->setUniform3f("pointLight.color", pointLightcolor.r, pointLightcolor.g, pointLightcolor.b);

	shader->setUniform3f("directionalLight.direction", directionalLight.direction);
	shader->setUniform1f("directionalLight.brightness", directionalLight.brightness);
	ofFloatColor directionalLightColor = directionalLight.color;
	shader->setUniform3f("directionalLight.color", directionalLightColor.r, directionalLightColor.g, directionalLightColor.b);

	shader->setUniform3f("projectorLight.position", spotLight.position);
	shader->setUniform3f("projectorLight.direction", spotLight.direction);
	shader->setUniform1f("projectorLight.brightness", spotLight.brightness);
	shader->setUniform1f("projectorLight.cutoffAngle", spotLight.innerCutoff);
	shader->setUniform1f("projectorLight.outerCutoffAngle", spotLight.outerCutoff);
	ofFloatColor spotLightColor = spotLight.color;
	shader->setUniform3f("projectorLight.color", spotLightColor.r, spotLightColor.g, spotLightColor.b);

	if (filteredTexture.isAllocated())
		shader->setUniformTexture("diffuseMap", filteredTexture, 0);
	if (normalMap.isAllocated())
		shader->setUniformTexture("normalMap", normalMap, 1);

	ofFloatColor ambiant = ofFloatColor(materialAmbiant);
	shader->setUniform3f("material.ambiant", ambiant.r, ambiant.g, ambiant.b);
	ofFloatColor diffuse = ofFloatColor(materialDiffuse);
	shader->setUniform3f("material.diffuse", diffuse.r, diffuse.g, diffuse.b);
	ofFloatColor specular = ofFloatColor(materialSpecular);
	shader->setUniform3f("material.specular", specular.r, specular.g, specular.b);

	ofFloatColor ambiantColor = ambiantLight.color;
	shader->setUniform3f("ambiant", ambiantColor.r, ambiantColor.g, ambiantColor.b);

	drawShaderOverride();

	shader->end();

	for (Object* child : children) {
		child->drawShader(shader, ambiantLight, pointLight, directionalLight, spotLight);
	}

	ofPopMatrix();
}

void Object::drawPBR(ofShader* shader, Light pointLight, Light directionalLight, Light spotLight) {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	shader->begin();

	ofFloatColor ambiant = ofFloatColor(materialAmbiant);
	shader->setUniform3f("material_color_ambient", ambiant.r, ambiant.g, ambiant.b);
	ofFloatColor diffuse = ofFloatColor(materialDiffuse);
	shader->setUniform3f("material_color_diffuse", diffuse.r, diffuse.g, diffuse.b);
	ofFloatColor specular = ofFloatColor(materialSpecular);
	shader->setUniform3f("material_color_specular", specular.r, specular.g, specular.b);

	shader->setUniform1f("material_brightness", materialBrightness);
	shader->setUniform1f("material_metallic", materialMetallic);
	shader->setUniform1f("material_roughness", materialRoughness);
	shader->setUniform1f("material_occlusion", materialOcclusion);

	ofFloatColor ior = ofFloatColor(materialIor);
	shader->setUniform3f("material_fresnel_ior", ior.r, ior.g, ior.b);

	if (texture.isAllocated())
		shader->setUniformTexture("texture_diffuse", texture, 0);
	if (normalMap.isAllocated())
		shader->setUniformTexture("texture_normale", normalMap, 1);
	if (metallicMap.isAllocated())
		shader->setUniformTexture("texture_metallic", metallicMap, 2);
	if (roughnessMap.isAllocated())
		shader->setUniformTexture("texture_roughness", roughnessMap, 3);
	if (occlusionMap.isAllocated())
		shader->setUniformTexture("texture_occlusion", occlusionMap, 4);

	shader->setUniform1f("light_intensity", pointLight.brightness / 64);
	ofFloatColor lightColor = ofFloatColor(pointLight.color);
	shader->setUniform3f("light_color", lightColor.r, lightColor.g, lightColor.b);
	shader->setUniform3f("light_position", pointLight.position);

	shader->setUniform1f("tone_mapping_exposure", exposure);
	shader->setUniform1f("tone_mapping_gamma", gamma);
	shader->setUniform1i("tone_mapping_toggle", true);

	drawShaderOverride();

	shader->end();

	for (Object* child : children) {
		child->drawPBR(shader, pointLight, directionalLight, spotLight);
	}

	ofPopMatrix();
}

void Object::checkIfSelected() {
	if (selected) {
		drawBoundingBox();
	}
	else {
		ofPushMatrix();

		ofTranslate(translationX, translationY, translationZ);

		ofRotateXDeg(rotationX);
		ofRotateYDeg(rotationY);
		ofRotateZDeg(rotationZ);

		ofScale(scaleX, scaleY, scaleZ);

		for (Object* child : children) {
			child->checkIfSelected();
		}

		ofPopMatrix();
	}
}

void Object::drawBoundingBox() {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawBoundingBoxOverride();

	for (Object* child : children) {
		child->drawBoundingBox();
	}

	ofPopMatrix();
}
