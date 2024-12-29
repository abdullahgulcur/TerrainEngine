#include "pch.h"
#include "heightmap_generator.h"
#include "gl_context.h"
#include "texture2d.h"

namespace Engine {

	HeightmapGenerator::HeightmapGenerator(glm::u16vec2 terrainSize, double frequency, UINT8 octaves, unsigned int seed, unsigned short maxHeight) {
		/*this->terrainSize = terrainSize;

		data = new unsigned short[terrainSize.x * terrainSize.y];

		auto getIndex = [](unsigned short x, unsigned short y, unsigned short width) {
			return width * y + x;
		};

		const siv::PerlinNoise perlin{ seed };
		const double fx = (frequency / terrainSize.x);
		const double fy = (frequency / terrainSize.y);

		for (unsigned short y = 0; y < terrainSize.y; y++){
			for (unsigned short x = 0; x < terrainSize.x; x++){
				double valueNormalized = perlin.octave2D_01((x * fx), (y * fy), octaves);
				unsigned short height = 65536 * valueNormalized;
				data[getIndex(x, y, terrainSize.x)] = height;
			}
		}*/

		//terrainSize = glm::u16vec2(256);

		this->terrainSize = terrainSize;
		data = new unsigned int[terrainSize.x * terrainSize.y];

		unsigned int shaderProgramId = GLShader::loadShader("../../../resource/shader/generative/pnoise.vert", "../../../resource/shader/generative/pnoise.frag");
		unsigned int textureId = GLTexture::generateHeightmapTexture(terrainSize);
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

		GLTexture::deleteTexture(textureId);

	}

}