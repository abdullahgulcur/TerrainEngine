#include "pch.h"
#include "heightmap_generator.h"
#include "gl_context.h"
#include "texture2d.h"

namespace Engine {

	HeightmapGenerator::HeightmapGenerator(glm::u16vec2 terrainSize, double frequency, UINT8 octaves, unsigned int seed, unsigned short maxHeight) {

		this->terrainSize = terrainSize;
		data = new unsigned int[terrainSize.x * terrainSize.y];

		unsigned int shaderProgramId = GLShader::loadShader("../../../resource/shader/generative/pnoise.vert", "../../../resource/shader/generative/pnoise.frag");
		textureId = GLTexture::generateHeightmapTexture(terrainSize);
		unsigned int FBO = GLBuffer::createFBO(textureId);
		unsigned int planeVAO = GLBuffer::createQuadVAO();

		GLCommand::setScreen(glm::u16vec2(0), terrainSize, FBO);
		GLShader::useProgram(shaderProgramId);
		GLCommand::drawQuad(planeVAO);
		GLTexture::getHeightmapContent(data, textureId);
		GLShader::deleteProgram(shaderProgramId);

		//Texture2D textureData(terrainSize.x, terrainSize.y, 2);
		//GLTexture::getTextureContent(2, textureData.data, textureId);

		//Texture2D textureData1(terrainSize.x, terrainSize.y, 1);
		//unsigned char* data1 = new unsigned char[terrainSize.x * terrainSize.y];
		/*for (int i = 0; i < terrainSize.x * terrainSize.y; i++) {
			textureData1.data[i] = textureData.data[i * 2];
		}*/

		
		//textureData1.writeDataToFile("fuckeri.png", 0);

		//GLTexture::deleteTexture(textureId);

	}

	float HeightmapGenerator::getHeight(glm::vec2 cameraPosition) {

		unsigned int camPosX = unsigned int(cameraPosition.x);
		unsigned int camPosY = unsigned int(cameraPosition.y);

		if (camPosX < 0 || camPosY < 0 || camPosX >= terrainSize.x || camPosY >= terrainSize.y)
			return 0;

		unsigned int index = camPosY * terrainSize.x + camPosX;
		return data[index] * 0.03f;
	}

}

// perlin noise texture generator shader dan image generation icin bir daha yazilacak
// image generator diye class olusturulacak
// image olusturma tek fonksiyon ile saglanip uniform olarak kullanilacak