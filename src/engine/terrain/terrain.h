#pragma once

#include "terrain_config.h"
#include "page_table_manager.h"
#include "terrain_geometry.h"

// TODO: data lari freelemeyi unutma

namespace Engine {

	enum class TerrainClipmapSize {
		BLOCK_SIZE_8 = 8, // low quality terrain, if there is no elevation at all, it can be used.
		BLOCK_SIZE_16 = 16, // medium quality terrain
		BLOCK_SIZE_32 = 32, // high quality terrain (most of the time, it is good enough)
		BLOCK_SIZE_64 = 64, // ultra quality terrain
	};

	class Terrain {

	private:

	public:

		PageTableManager pageTableManager;
		TerrainGeometry terrainGeometry;

		Terrain() {}
		~Terrain() {}
		void init(std::string path, unsigned short blockSize, UINT8 clipmapLevels);
		void init(glm::u16vec2 terrainSize, unsigned short blockSize, UINT8 clipmapLevels, UINT8 innerClipmapLevel);
		void update();
	};
}