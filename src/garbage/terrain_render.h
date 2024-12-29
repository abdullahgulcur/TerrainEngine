#pragma once

#include "glm.hpp"
#include "terrain_config.h"
#include "page_table_manager.h"
#include "terrain_geometry.h"

namespace Engine {

	class TerrainRender {

	private:

	public:

		ClipmapTracker clipmapTracker;
		TerrainGeometry terrainGeometry;
		unsigned int shaderProgramId;

		TerrainRender();
		~TerrainRender();
		void init(std::string path, unsigned short blockSize, UINT8 clipmapLevels);
		void update();

	};
}