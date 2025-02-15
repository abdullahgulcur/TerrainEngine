#include "pch.h"
#include "heightmap_data.h"
#include "heightmap.h"
#include "terrain_config.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Engine {

	HeightmapData::HeightmapData() {}

	HeightmapData::HeightmapData(std::string path, unsigned short blockSize, UINT8 clipmapLevels) {

		//int width;
		//int height;
		//int channels;
		//unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);
		//if (width / blockSize > 1024 || height / blockSize > 1024) {

		//	// handle problem
		//	return;
		//}


		//Texture2D heightmapMipmap(width, height, channels, clipmapLevels);
		//heightmapMipmap.loadData(imageData);
		//stbi_image_free(imageData);

		////heightmapMipmap.writeDataToFile("fucker.png", 1);


		//int totalBlocksPerColumn = width / blockSize;
		//int totalBlocksPerRow = height / blockSize;

		//this->columns = totalBlocksPerColumn;
		//this->rows = totalBlocksPerRow;
		//this->channels = channels;
		//this->blockSize = blockSize;
		//this->terrainSize = glm::u16vec2(totalBlocksPerColumn, totalBlocksPerRow) * blockSize;

		//this->mipStartIndices = std::vector<unsigned int>(clipmapLevels);

		//this->data = new unsigned char[getDataSize()];
		//setMipmapStartIndex();

		//for (int level = 0; level < clipmapLevels; level++) {
		//	for (int y = 0; y < totalBlocksPerRow; y++) {
		//		for (int x = 0; x < totalBlocksPerColumn; x++) {

		//			int arrayIndex = getArrayIndex(level, glm::ivec2(x, y));

		//			Texture2D base(blockSize, blockSize, 1); // TODO: channels is 1 here
		//			base.data = &data[arrayIndex];

		//			glm::ivec2 startIndexInHeightmapMipmap(blockSize * x, blockSize * y);
		//			base.copyData(heightmapMipmap, 0, level, glm::ivec2(0, 0), startIndexInHeightmapMipmap, glm::ivec2(blockSize));

		//		}
		//	}
		//	totalBlocksPerColumn >>= 1;
		//	totalBlocksPerRow >>= 1;
		//}
	}

	HeightmapData::HeightmapData(HeightmapGenerator heightmapGenerator, unsigned short blockSize, UINT8 clipmapLevels) {

		Heightmap heightmapMipmap(heightmapGenerator.terrainSize.x, heightmapGenerator.terrainSize.y, clipmapLevels);
		heightmapMipmap.loadData(heightmapGenerator.data);

		int totalBlocksPerColumn = heightmapGenerator.terrainSize.x / blockSize;
		int totalBlocksPerRow = heightmapGenerator.terrainSize.y / blockSize;

		this->columns = totalBlocksPerColumn;
		this->rows = totalBlocksPerRow;
		this->blockSize = blockSize;
		this->terrainSize = glm::u16vec2(totalBlocksPerColumn, totalBlocksPerRow) * blockSize;

		this->mipStartIndices = std::vector<unsigned int>(clipmapLevels);

		this->data = new unsigned int[getDataSize()];
		setMipmapStartIndex();

		for (int level = 0; level < clipmapLevels; level++) {
			for (int y = 0; y < totalBlocksPerRow; y++) {
				for (int x = 0; x < totalBlocksPerColumn; x++) {

					int arrayIndex = getArrayIndex(level, glm::ivec2(x, y));

					Heightmap base(blockSize, blockSize);
					base.data = &data[arrayIndex];

					glm::ivec2 startIndexInHeightmapMipmap(blockSize * x, blockSize * y);
					base.copyData(heightmapMipmap, 0, level, glm::ivec2(0, 0), startIndexInHeightmapMipmap, glm::ivec2(blockSize));
				}
			}
			totalBlocksPerColumn >>= 1;
			totalBlocksPerRow >>= 1;
		}
	}

	void HeightmapData::setMipmapStartIndex() {
		int r = rows;
		int c = columns;

		int blockIndex = 0;
		for (int i = 0; i < mipStartIndices.size(); i++) {

			mipStartIndices[i] = blockIndex;
			blockIndex += r * c;
			r >>= 1;
			c >>= 1;
		}
	}

	unsigned int HeightmapData::getDataSize() {
		return columns * rows * getBlockCount();
	}

	unsigned int HeightmapData::getBlockCount() {
		int r = rows;
		int c = columns;

		int sum = 0;
		for (int i = 0; i < mipStartIndices.size(); i++) {

			sum += r * c;
			r >>= 1;
			c >>= 1;
		}
		return sum;
	}

	int HeightmapData::getBlockStartIndexAtHeightmapTexture(int level, glm::ivec2 index) {
		int r = rows >> level;
		int remainder = index.y * r + index.x;

		return mipStartIndices[level] + remainder;
	}

	int HeightmapData::getArrayIndex(int level, glm::ivec2 index) {

		int arraySize = blockSize * blockSize;
		int blockIndex = getBlockStartIndexAtHeightmapTexture(level, index);
		return arraySize * blockIndex;
	}

}