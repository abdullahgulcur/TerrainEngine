#include "pch.h"
#include "water.h"
#include "gl_buffer.h"
#include "core.h"

namespace Engine {

	void Water::init() {

		GLBuffer::createWaterQuadVAO(this->quadVAO);
		waterShaderProgramId = Core::getShader()->shaders[ShaderType::WATER];
	}

	void Water::update() {

	}

}