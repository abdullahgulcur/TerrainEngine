#pragma once

#include "terrain_config.h"
#include "terrain_texturing.h"
#include "terrain_geometry.h"
#include "terrain_geometry_manager.h"
#include "water.h"

// TODO: data lari freelemeyi unutma

namespace Engine {

	enum class TerrainClipmapSize {
		BLOCK_SIZE_4 = 4, // very low
		BLOCK_SIZE_8 = 8, // low
		BLOCK_SIZE_16 = 16, // medium
		BLOCK_SIZE_32 = 32, // high
		BLOCK_SIZE_64 = 64, // ultra
	};

	class Terrain {

	private:

	public:

		HeightmapGenerator hg;
		TerrainTexturing terrainTexturing;
		TerrainGeometry terrainGeometry;
		TerrainGeometryManager terrainGeometryManager;
		Water water;

		Terrain() {}
		~Terrain() {}
		void init(std::string path, unsigned short blockSize, UINT8 clipmapLevels);
		void init(glm::u16vec2 terrainSize, unsigned short blockSize, UINT8 clipmapLevels, UINT8 innerClipmapLevel);
		void update();
	};
}