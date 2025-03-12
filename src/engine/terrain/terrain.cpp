#include "pch.h"
#include "terrain.h"
#include "core.h"
#include "gl_context.h"

namespace Engine {

	void Terrain::init(std::string path, unsigned short blockSize, UINT8 clipmapLevels) {

		//pageTableManager.init(path, blockSize, clipmapLevels);
		//terrainGeometry.generateTerrainGeometryVertexArrays(pageTableManager.heightmapData.blockSize, pageTableManager.heightmapData.mipStartIndices.size(), BLOCK_COUNT_PER_LEVEL * pageTableManager.heightmapData.mipStartIndices.size() + BLOCK_COUNT_INNER);
	}

	void Terrain::init(glm::u16vec2 terrainSize, unsigned short blockSize, UINT8 clipmapLevels, UINT8 innerClipmapLevel) {
		
		hg = HeightmapGenerator(glm::u16vec2(terrainSize), 8, 4, 152746, 500);
		terrainTexturing.init(terrainSize.x, &hg);
		terrainGeometryManager.init(terrainSize.x, blockSize, &hg);
		terrainGeometry.generateTerrainGeometryVertexArrays(terrainGeometryManager.blockSize, terrainGeometryManager.totalClipmapLevel);

		//water.init();
	}

	void Terrain::update() {

		terrainGeometryManager.update();
		terrainTexturing.update();
	}

}