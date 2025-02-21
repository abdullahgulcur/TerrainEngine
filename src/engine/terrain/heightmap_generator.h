#pragma once

#include "PerlinNoise.hpp"
#include "glm.hpp"

namespace Engine {

	class HeightmapGenerator {

	private:
	public:

		glm::u16vec2 terrainSize;
		unsigned int* data;
		//unsigned short maxHeight;
		unsigned int textureId;
		unsigned int shadowMapTextureId;
		std::vector<glm::u8vec2> heightmapSectorBoundaryList;

		HeightmapGenerator() {}
		~HeightmapGenerator() {}
		HeightmapGenerator(glm::u16vec2 terrainSize, double frequency, UINT8 octaves, unsigned int seed, unsigned short maxHeight);
		void calculateShadowMap(unsigned short* shadowmapData);
		bool marchRay(glm::vec2 terrainPos, float height, glm::vec3 lightDir);
		bool isInBounds(const glm::vec2& pos);
		/*bool anyIntersection(glm::vec3 terrainPos, glm::vec3 lightDir);
		bool isThereIntersection(glm::vec3 terrainPos, glm::vec3 lightDir, glm::vec3* quad);
		bool rayIntersectsTriangle(glm::vec3 vectorPos, glm::vec3 direction, glm::vec3* triangle);*/
		float getHeight(glm::ivec2 terrainPos);
		float getHeight(glm::vec2 cameraPosition);

	};
}