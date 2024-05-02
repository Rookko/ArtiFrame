#include "customObject.h"

void CustomObject::drawWireframeOverride() {
	triangulator->mesh.draw(OF_MESH_WIREFRAME);

	for (glm::vec3 vertex : triangulator->mesh.getVertices()) {
		ofDrawSphere(vertex, 3);
	}
}

void CustomObject::drawShaderOverride() {
}

void CustomObject::drawBoundingBoxOverride() {

}

ofVec3f CustomObject::getMeshBoundingBoxDimension(ofMesh* mesh) {
	ofPoint minVertex, maxVertex;
	minVertex.x = minVertex.y = minVertex.z = std::numeric_limits<float>::max();
	maxVertex.x = maxVertex.y = maxVertex.z = -std::numeric_limits<float>::max();

	std::for_each(mesh->getVertices().begin(), mesh->getVertices().end(),
		[&minVertex, &maxVertex](const ofPoint& vertex) {
			minVertex.x = std::min(minVertex.x, vertex.x);
			minVertex.y = std::min(minVertex.y, vertex.y);
			minVertex.z = std::min(minVertex.z, vertex.z);
			maxVertex.x = std::max(maxVertex.x, vertex.x);
			maxVertex.y = std::max(maxVertex.y, vertex.y);
			maxVertex.z = std::max(maxVertex.z, vertex.z);
		});

	return maxVertex - minVertex;
}
