#pragma once

#include "PerlinNoise.hpp"
#include "glm.hpp"

namespace Engine {

	class HeightmapGenerator {

	private:
	public:

		glm::u16vec2 terrainSize;
		unsigned int* data;
		unsigned int textureId;
		unsigned int shadowMapTextureId;
		std::vector<glm::u8vec2> heightmapSectorBoundaryList;

		HeightmapGenerator() {}
		~HeightmapGenerator() {}
		HeightmapGenerator(glm::u16vec2 terrainSize, double frequency, UINT8 octaves, unsigned int seed, unsigned short maxHeight);
		float getHeight(glm::vec2 cameraPosition);
	};
}