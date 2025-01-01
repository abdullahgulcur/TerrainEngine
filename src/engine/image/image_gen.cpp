#include "pch.h"
#include "image_gen.h"
#include "gl_context.h"

namespace Engine {

	unsigned int ImageGenerator::generatePerlinNoiseTexture(glm::u16vec2 size, float scale) {

		unsigned int shaderProgramId = GLShader::loadShader("../../../resource/shader/generative/common/pnoise.vert", "../../../resource/shader/generative/common/pnoise.frag");
		unsigned int textureId = GLTexture::generateMaskTexture(size.x, size.y);
		unsigned int FBO = GLBuffer::createFBO(textureId);
		unsigned int planeVAO = GLBuffer::createQuadVAO();

		GLCommand::setScreen(glm::u16vec2(0), size, FBO);
		GLShader::useProgram(shaderProgramId);
		GLUniform::setFloat1(shaderProgramId, "scale", scale);
		GLCommand::drawQuad(planeVAO);
		GLShader::deleteProgram(shaderProgramId);

		GLTexture::generateMipmap(textureId);

		return textureId;
	}

}

// perlin noise texture generator shader dan image generation icin bir daha yazilacak
// image generator diye class olusturulacak
// image olusturma tek fonksiyon ile saglanip uniform olarak kullanilacak