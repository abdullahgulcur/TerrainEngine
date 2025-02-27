#include "pch.h"
#include "heightmap_generator.h"
#include "gl_context.h"
#include "texture2d.h"
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

		//unsigned short* shadowmapData = new unsigned short[terrainSize.x * terrainSize.y];
		//HeightmapGenerator::calculateShadowMap(shadowmapData);
		//shadowMapTextureId = GLTexture::generateShadowmapTexture(terrainSize, shadowmapData);
		//delete[] shadowmapData;



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

	void HeightmapGenerator::calculateShadowMap(unsigned short* shadowmapData) {

		glm::vec3 lightDir(-1, -1, -1);

		for (int y = 0; y < terrainSize.y; y++) {
			for (int x = 0; x < terrainSize.x; x++) {

				glm::vec2 terrainPos(x, y);

				//bool intersects = HeightmapGenerator::anyIntersection(terrainPos, lightDir);
				bool intersects = HeightmapGenerator::marchRay(terrainPos, HeightmapGenerator::getHeight(glm::ivec2(x, y)), lightDir);
				
				shadowmapData[terrainSize.x * y + x] = intersects;
			}
		}
	}

	bool HeightmapGenerator::marchRay(glm::vec2 terrainPos, float height, glm::vec3 lightDir) {
		const float stepSize = 1.0f; // Control precision
		const float epsilon = 0.1f;
		glm::vec2 currentPos(terrainPos.x, terrainPos.y);
		glm::vec2 lightMarchRay(lightDir.x, lightDir.z);

		currentPos += lightMarchRay * stepSize;

		// Move in the direction of the light ray, check triangles along the path
		while (HeightmapGenerator::isInBounds(currentPos)) {

			int x = (int)currentPos.x;
			int y = (int)currentPos.y;

			float testHheight = HeightmapGenerator::getHeight(glm::ivec2(x, y));

			glm::vec3 pos(x, testHheight, y);

			glm::vec3 rayDir = glm::vec3(terrainPos.x, height, terrainPos.y) - pos;

			if (glm::abs(glm::dot(rayDir, lightDir)) < epsilon)
				return true;

			// Check the two triangles forming the quad in this cell
			//glm::vec3 quad[4];
			//quad[0] = glm::vec3(x, HeightmapGenerator::getHeight(glm::ivec2(x, y)), y);
			//quad[1] = glm::vec3(x + 1, HeightmapGenerator::getHeight(glm::ivec2(x + 1, y)), y);
			//quad[2] = glm::vec3(x, HeightmapGenerator::getHeight(glm::ivec2(x, y + 1)), y + 1);
			//quad[3] = glm::vec3(x + 1, HeightmapGenerator::getHeight(glm::ivec2(x + 1, y + 1)), y + 1);

			//if (HeightmapGenerator::isThereIntersection(terrainPos, lightDir, quad)) {
			//	return true; // Shadowed
			//}

			// Move along the light direction
			currentPos += lightMarchRay * stepSize;
		}

		return false; // No intersection found
	}

	// Check if the current position is within the bounds of the terrain
	bool HeightmapGenerator::isInBounds(const glm::vec2& pos) {
		return (pos.x >= 0 && pos.x < terrainSize.x&& pos.y >= 0 && pos.y < terrainSize.y);
	}

	//bool HeightmapGenerator::anyIntersection(glm::vec3 terrainPos, glm::vec3 lightDir) {

	//	for (int y = 0; y < terrainSize.y - 1; y++) {
	//		for (int x = 0; x < terrainSize.x - 1; x++) {

	//			glm::vec3 quad[4];

	//			float h0 = HeightmapGenerator::getHeight(glm::ivec2(x, y));
	//			float h1 = HeightmapGenerator::getHeight(glm::ivec2(x + 1, y));
	//			float h2 = HeightmapGenerator::getHeight(glm::ivec2(x, y + 1));
	//			float h3 = HeightmapGenerator::getHeight(glm::ivec2(x + 1, y + 1));

	//			quad[0] = glm::vec3(x, h0, y);
	//			quad[1] = glm::vec3(x + 1, h1, y);
	//			quad[2] = glm::vec3(x, h2, y + 1);
	//			quad[3] = glm::vec3(x + 1, h3, y + 1);

	//			bool intersects = HeightmapGenerator::isThereIntersection(terrainPos, lightDir, quad);
	//			
	//			if (intersects)
	//				return true;
	//		}
	//	}

	//	return false;
	//}

	//bool HeightmapGenerator::isThereIntersection(glm::vec3 terrainPos, glm::vec3 lightDir, glm::vec3* quad) {

	//	glm::vec3 triangle0[3] = { quad[0], quad[1], quad[3] };
	//	glm::vec3 triangle1[3] = { quad[0], quad[2], quad[3] };
	//	return HeightmapGenerator::rayIntersectsTriangle(terrainPos, lightDir, triangle0) || HeightmapGenerator::rayIntersectsTriangle(terrainPos, lightDir, triangle1);
	//}

	//bool HeightmapGenerator::rayIntersectsTriangle(glm::vec3 vectorPos, glm::vec3 direction, glm::vec3* triangle) {
	//	const float EPSILON = 1e-8;

	//	glm::vec3 v0 = triangle[0];
	//	glm::vec3 v1 = triangle[1];
	//	glm::vec3 v2 = triangle[2];

	//	glm::vec3 e1 = v1 - v0;
	//	glm::vec3 e2 = v2 - v0;

	//	glm::vec3 h = cross(direction, e2);
	//	float det = glm::dot(e1, h);

	//	if (fabs(det) < EPSILON)
	//		return false;

	//	float f = 1.0 / det;
	//	glm::vec3 s = vectorPos - v0;
	//	float u = f * dot(s, h);

	//	if (u < 0.0 || u > 1.0)
	//		return false; // Intersection is outside the triangle

	//	glm::vec3 q = cross(s, e1);
	//	float v = f * glm::dot(direction, q);

	//	if (v < 0.0 || u + v > 1.0)
	//		return false; // Intersection is outside the triangle

	//	float t = f * glm::dot(e2, q);

	//	return t > EPSILON;
	//}

	float HeightmapGenerator::getHeight(glm::ivec2 terrainPos) {

		return data[terrainPos.y * terrainSize.x + terrainPos.x] * 0.03f;
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