#include "pch.h"
#include "heightmap_generator.h"
#include "gl_context.h"
#include "config.h"

namespace Engine {

	HeightmapGenerator::HeightmapGenerator(glm::u16vec2 terrainSize, double frequency, UINT8 octaves, unsigned int seed, unsigned short maxHeight) {

		this->terrainSize = terrainSize;
		data = new unsigned int[terrainSize.x * terrainSize.y];

		std::string resource_dir(RESOURCES);

		unsigned int shaderProgramId = GLShader::loadShader(resource_dir + "/shader/generative/pnoise.vert", resource_dir + "/shader/generative/pnoise.frag");
		unsigned int elevationMapTextureId = GLTexture::generateHeightmapTexture(resource_dir + "/texture/terrain/elevationmap.png");
		textureId = GLTexture::generateHeightmapTexture(terrainSize);

		unsigned int FBO = GLBuffer::createFBO(textureId);
		unsigned int planeVAO = GLBuffer::createQuadVAO();

		GLCommand::setScreen(glm::u16vec2(0), terrainSize, FBO);
		GLShader::useProgram(shaderProgramId);
		GLTexture::setTextureUniformLocation("heightmap", shaderProgramId, 0);
		GLTexture::useTexture(0, elevationMapTextureId);

		GLCommand::drawQuad(planeVAO);
		GLTexture::getHeightmapContent(data, textureId);

		GLShader::deleteProgram(shaderProgramId);
	}

	float HeightmapGenerator::getHeight(glm::vec2 cameraPosition) {

		unsigned int camPosX = unsigned int(cameraPosition.x);
		unsigned int camPosY = unsigned int(cameraPosition.y);

		if (camPosX < 0 || camPosY < 0 || camPosX >= terrainSize.x || camPosY >= terrainSize.y)
			return 0;

		unsigned int index = camPosY * terrainSize.x + camPosX;
		return data[index] * 0.01f;
	}

}
