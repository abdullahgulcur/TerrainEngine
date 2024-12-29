#include "pch.h"
#include "terrain.h"
#include "core.h"

namespace Engine {

	void Terrain::init(std::string path, unsigned short blockSize, UINT8 clipmapLevels) {

		pageTableManager.init(path, blockSize, clipmapLevels);
		terrainGeometry.generateTerrainGeometryVertexArrays(pageTableManager.heightmapData.blockSize, pageTableManager.heightmapData.mipStartIndices.size(), BLOCK_COUNT_PER_LEVEL * pageTableManager.heightmapData.mipStartIndices.size() + BLOCK_COUNT_INNER);
	}

	void Terrain::update() {

		pageTableManager.update();
	}

}