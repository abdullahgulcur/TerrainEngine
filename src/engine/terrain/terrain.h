#pragma once

#include "terrain_render.h"

#include "terrain_config.h"

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

		TerrainRender terrainRender;

		static Terrain* create();
		Terrain();
		~Terrain();
		void init(std::string path, unsigned short blockSize, UINT8 clipmapLevels);
		void update();
	};
}