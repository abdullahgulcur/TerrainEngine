#include "pch.h"
#include "terrain.h"

namespace Engine {

	Terrain* Terrain::create() {

		return new Terrain();
	}

	Terrain::Terrain() {

	}

	Terrain::~Terrain() {

	}

	void Terrain::init(std::string path, unsigned short blockSize, UINT8 clipmapLevels) {

		terrainRender.init(path, blockSize, clipmapLevels);
	}

	void Terrain::update() {

		terrainRender.clipmapTracker.update();
		terrainRender.update();
	}

}