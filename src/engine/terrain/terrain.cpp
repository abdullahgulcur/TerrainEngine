#include "pch.h"
#include "terrain.h"
#include "core.h"

namespace Engine {

	void Terrain::init(std::string path, unsigned short blockSize, UINT8 clipmapLevels) {

		//pageTableManager.init(path, blockSize, clipmapLevels);
		//terrainGeometry.generateTerrainGeometryVertexArrays(pageTableManager.heightmapData.blockSize, pageTableManager.heightmapData.mipStartIndices.size(), BLOCK_COUNT_PER_LEVEL * pageTableManager.heightmapData.mipStartIndices.size() + BLOCK_COUNT_INNER);
	}

	void Terrain::init(glm::u16vec2 terrainSize, unsigned short blockSize, UINT8 clipmapLevels, UINT8 innerClipmapLevel) {

		pageTableManager.init(terrainSize, blockSize, clipmapLevels, innerClipmapLevel);
		terrainGeometry.generateTerrainGeometryVertexArrays(pageTableManager.heightmapData.blockSize, pageTableManager.heightmapData.mipStartIndices.size(), BLOCK_COUNT_PER_LEVEL * pageTableManager.heightmapData.mipStartIndices.size() + BLOCK_COUNT_INNER);
	}

	void Terrain::update() {

		pageTableManager.update();
	}

}