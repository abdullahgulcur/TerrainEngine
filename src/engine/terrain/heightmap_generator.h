#pragma once

#include "PerlinNoise.hpp"
#include "glm.hpp"

namespace Engine {

	class HeightmapGenerator {
	private:
	public:

		glm::u16vec2 terrainSize;
		unsigned short* data;
		unsigned short maxHeight;

		HeightmapGenerator(glm::u16vec2 terrainSize, double frequency, UINT8 octaves, unsigned int seed, unsigned short maxHeight);
		~HeightmapGenerator();

	};
}