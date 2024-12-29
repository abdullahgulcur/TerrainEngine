#pragma once

#include "terrain_config.h"
#include "page_table_manager.h"
#include "terrain_geometry.h"

// TODO: data lari freelemeyi unutma

namespace Engine {

	enum class TerrainClipmapSize {
		BLOCK_SIZE_8 = 8,
		BLOCK_SIZE_16 = 16,
		BLOCK_SIZE_32 = 32,
	};

	class Terrain {

	private:

	public:

		PageTableManager pageTableManager;
		TerrainGeometry terrainGeometry;

		Terrain() {}
		~Terrain() {}
		void init(std::string path, unsigned short blockSize, UINT8 clipmapLevels);
		void update();
	};
}