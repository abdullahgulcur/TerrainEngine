#pragma once

#include "glm.hpp"

namespace Engine {

	class HeightmapData {
	private:
	public:

		unsigned short columns;
		unsigned short rows;
		glm::u16vec2 terrainSize;
		UINT8 channels;
		UINT8 blockSize;
		std::vector<unsigned int> mipStartIndices;
		unsigned char* data;

		HeightmapData();
		HeightmapData(std::string path, unsigned short blockSize, UINT8 clipmapLevels);
		void setMipmapStartIndex();
		unsigned int getDataSize();
		unsigned int getBlockCount();
		int getBlockStartIndexAtHeightmapTexture(int level, glm::ivec2 index);
		int getArrayIndex(int level, glm::ivec2 index);
	};
}