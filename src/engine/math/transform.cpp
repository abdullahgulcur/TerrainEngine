#include "pch.h"
#include "transform.h"
#include "ext/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/euler_angles.hpp"

namespace Engine {

	glm::mat4 Transform::getTranslationMatrix() {

		return glm::translate(glm::mat4(1.0f), this->position);
	}

	glm::mat4 Transform::getRotationMatrix() {

		glm::vec3 rotationInRadians = glm::radians(this->rotation);
		return glm::eulerAngleXYZ(rotationInRadians.x, rotationInRadians.y, rotationInRadians.z);

		/*glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		return rotX * rotY * rotZ;*/
	}

	glm::mat4 Transform::getScaleMatrix() {

		return glm::scale(glm::mat4(1.0f), this->scale);
	}

	glm::mat4 Transform::getModelMatrix() {

		const glm::mat4 rotationMatrix = Transform::getRotationMatrix();
		const glm::mat4 translationMatrix = Transform::getTranslationMatrix();
		const glm::mat4 scaleMatrix = Transform::getScaleMatrix();

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	//glm::mat4 Transform::getModelMatrix(const Transform2C& transform) {

	//	const glm::mat4 rotationMatrix = Transform::getRotationMatrix(transform.rotation);
	//	const glm::mat4 translationMatrix = Transform::getTranslationMatrix(transform.position);

	//	return translationMatrix * rotationMatrix;
	//}

	glm::mat4 Transform::getViewMatrix() {

		return glm::inverse(Transform::getModelMatrix());
	}

	//void Transform::setPosition(glm::vec3 position) {
	//	this->position = position;
	//}

	//void Transform::setRotation(glm::vec3 rotation) {
	//	this->rotation = rotation;
	//}

	//void Transform::setScale(glm::vec3 scale) {
	//	this->scale = scale;
	//}

	void Transform::decomposeModelMatrixNoScale(const glm::mat4& modelMatrix) {
		// Extract position directly from the translation part of the matrix
		this->position = glm::vec3(modelMatrix[3]);

		// Extract the rotation matrix (directly from the 3x3 part of the model matrix)
		glm::mat3 rotationMatrix = glm::mat3(modelMatrix);

		// Convert rotation matrix to quaternion and then to Euler angles
		glm::quat rotationQuat = glm::quat_cast(rotationMatrix);
		this->rotation = glm::degrees(glm::eulerAngles(rotationQuat)); // Euler angles in degrees
	}
}
