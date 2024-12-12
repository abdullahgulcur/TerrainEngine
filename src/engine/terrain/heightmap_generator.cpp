#include "pch.h"
#include "heightmap_generator.h"

namespace Engine {

	HeightmapGenerator::HeightmapGenerator(glm::u16vec2 terrainSize, double frequency, UINT8 octaves, unsigned int seed, unsigned short maxHeight) {
		this->terrainSize = terrainSize;

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
		}
	}
	HeightmapGenerator::~HeightmapGenerator() {

	}


}