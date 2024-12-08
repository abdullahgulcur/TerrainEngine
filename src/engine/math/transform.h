#pragma once

#include "glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/compatibility.hpp"

namespace Engine {

	class Transform {
	private:

		glm::mat4 getTranslationMatrix();
		glm::mat4 getRotationMatrix();
		glm::mat4 getScaleMatrix();

	public:

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		Transform() {
			this->position = glm::vec3(0, 0, 0);
			this->rotation = glm::vec3(0, 0, 0);
			this->scale = glm::vec3(1, 1, 1);
		}

		Transform(glm::vec3 position) {
			this->position = position;
			this->rotation = glm::vec3(1, 1, 1);
			this->scale = glm::vec3(1, 1, 1);
		}

		Transform(glm::vec3 position, glm::vec3 rotation) {
			this->position = position;
			this->rotation = rotation;
			this->scale = glm::vec3(1, 1, 1);
		}

		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
			this->position = position;
			this->rotation = rotation;
			this->scale = scale;
		}

		//void setPosition(glm::vec3 position);
		//void setRotation(glm::vec3 rotation);
		//void setScale(glm::vec3 scale);

		glm::mat4 getModelMatrix();
		glm::mat4 getViewMatrix();

		void decomposeModelMatrixNoScale(const glm::mat4& modelMatrix);
	};

	//struct Transform3C {
	//	glm::vec3 position;
	//	glm::vec3 rotation;
	//	glm::vec3 scale;
	//};

	//struct Transform2C {
	//	glm::vec3 position;
	//	glm::vec3 rotation;
	//};

	//class Transform {
	//private:
	//public:

	//	Transform();
	//	~Transform();
	//	static glm::mat4 getTranslationMatrix(const glm::vec3& position);
	//	static glm::mat4 getRotationMatrix(const glm::vec3& rotation);
	//	static glm::mat4 getScaleMatrix(const glm::vec3& scale);
	//	static glm::mat4 getModelMatrix(const Transform3C& transform);
	//	static glm::mat4 getModelMatrix(const Transform2C& transform);
	//	static glm::mat4 getViewMatrix(const glm::mat4& model);
	//};
}