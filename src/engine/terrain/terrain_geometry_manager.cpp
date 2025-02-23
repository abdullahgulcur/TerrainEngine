#include "pch.h"
#include "terrain_geometry_manager.h"
#include "core.h"

namespace Engine {

	void TerrainGeometryManager::init(unsigned short terrainSize, unsigned short blockSize, HeightmapGenerator* heightmapGenerator) {

		this->heightmapGenerator = heightmapGenerator;
		this->terrainSize = terrainSize;
		this->blockSize = blockSize;

		totalClipmapLevel = std::log2(terrainSize / blockSize) + 1;

		clipmapGridIndexList = std::vector<glm::i16vec2>(totalClipmapLevel);
		clipmapStartGridIndexList = std::vector<glm::i16vec2>(totalClipmapLevel);

		blockIndexList = std::vector<glm::u16vec2>(totalClipmapLevel * 36);
		blockLocalIndexList = std::vector<glm::u8vec2>(totalClipmapLevel * 36);
		isInnerLists = std::vector<UINT8>(totalClipmapLevel * 36);
		outOfBorderList = std::vector<UINT8>(totalClipmapLevel * 36);

		Camera* camera = Core::getCamera();
		glm::vec2 cameraPosition = glm::vec2(camera->position.x, camera->position.z);
		glm::vec2 cameraPositionTerrainSpace = TerrainGeometryManager::worldSpaceToTerrainSpace(cameraPosition);

		//startClipmapLevel = 0;
		startClipmapLevel = TerrainGeometryManager::getMinClipmapLevel(cameraPosition, camera->position.y);

		TerrainGeometryManager::initBlockProperties(cameraPositionTerrainSpace);
	}

	void TerrainGeometryManager::update() {

		Camera* camera = Core::getCamera();
		glm::vec2 cameraPosition = glm::vec2(camera->position.x, camera->position.z);
		glm::vec2 cameraPositionTerrainSpace = TerrainGeometryManager::worldSpaceToTerrainSpace(cameraPosition);

		//startClipmapLevel = 0;
		startClipmapLevel = TerrainGeometryManager::getMinClipmapLevel(cameraPosition, camera->position.y);

		for (int i = 0; i < totalClipmapLevel; i++) {
			TerrainGeometryManager::calculateBlockPositionIndices(i, cameraPositionTerrainSpace);
			TerrainGeometryManager::setOutOfBorderList(i);
		}
	
	}

	void TerrainGeometryManager::setOutOfBorderList(const UINT8 clipmapLevel) {

		for (int j = 0; j < 36; j++) {
			glm::u16vec2 blockIndex = TerrainGeometryManager::getBlockIndex(clipmapLevel, j);
			unsigned short min = TerrainGeometryManager::getTerrainMinBlockIndex(clipmapLevel);
			unsigned short max = TerrainGeometryManager::getTerrainMaxBlockIndex(clipmapLevel);

			TerrainGeometryManager::setOutOfBorder(clipmapLevel, j, 0);
			if (blockIndex.x < min || blockIndex.y < min || blockIndex.x >= max || blockIndex.y >= max)
				TerrainGeometryManager::setOutOfBorder(clipmapLevel, j, 1);
		}
	}

	void TerrainGeometryManager::initBlockProperties(const glm::vec2 cameraPosition) {

		for (int i = 0; i < totalClipmapLevel; i++) {
			const glm::i16vec2 gridIndex = TerrainGeometryManager::getGridIndex(i, cameraPosition);
			const glm::i16vec2 startGridIndex = TerrainGeometryManager::getStartGridIndex(gridIndex);
			TerrainGeometryManager::setClipmapStartGridIndex(i, startGridIndex);
			TerrainGeometryManager::setClipmapGridIndex(i, gridIndex);

			glm::u16vec2* blockIndexList = &TerrainGeometryManager::getBlockIndex(i, 0);
			glm::u8vec2* blockLocalIndexList = &TerrainGeometryManager::getBlockLocalIndex(i, 0);
			UINT8* currentIsInnerList = &TerrainGeometryManager::getIsInner(i, 0);

			TerrainGeometryManager::getCurrentBlockProperties(i, gridIndex, startGridIndex, blockIndexList, blockLocalIndexList, currentIsInnerList);
			TerrainGeometryManager::setOutOfBorderList(i);
		}
	}

	void TerrainGeometryManager::calculateBlockPositionIndices(const UINT8 clipmapLevel, const glm::vec2 cameraPosition) {

		const glm::i16vec2 gridIndex = TerrainGeometryManager::getGridIndex(clipmapLevel, cameraPosition);
		const glm::i16vec2 startGridIndex = TerrainGeometryManager::getStartGridIndex(gridIndex);
		TerrainGeometryManager::setClipmapStartGridIndex(clipmapLevel, startGridIndex);

		if (TerrainGeometryManager::getClipmapGridIndex(clipmapLevel) != gridIndex) {
			TerrainGeometryManager::setClipmapGridIndex(clipmapLevel, gridIndex);

			glm::u16vec2 currentBlockIndices[36];
			glm::u8vec2 currentBlockLocalIndices[36];
			UINT8 currentIsInners[36];

			bool previousCommon[36];
			bool currentCommon[36];

			for (int i = 0; i < 36; i++) {
				previousCommon[i] = false;
				currentCommon[i] = false;
			}
			TerrainGeometryManager::getCurrentBlockProperties(clipmapLevel, gridIndex, startGridIndex, currentBlockIndices, currentBlockLocalIndices, currentIsInners);

			int counter = 36;
			for (int i = 0; i < 36; i++) {
				for (int j = 0; j < 36; j++) {
					if (TerrainGeometryManager::getBlockIndex(clipmapLevel, i) == currentBlockIndices[j] && 
						TerrainGeometryManager::getIsInner(clipmapLevel, i) == currentIsInners[j]) {
						previousCommon[i] = true;
						currentCommon[j] = true;
						counter--;
					}
				}
			}

			std::vector<UINT8> willBeDiscarded(counter);
			std::vector<UINT8> willBeAdded(counter);

			UINT8 lastPrevious = 0;
			UINT8 lastCurrent = 0;
			for (UINT8 i = 0; i < 36; i++) {
				if (!previousCommon[i])
					willBeDiscarded[lastPrevious++] = i;
				if (!currentCommon[i])
					willBeAdded[lastCurrent++] = i;
			}

			for (int i = 0; i < willBeDiscarded.size(); i++) {
				UINT8 discardIndex = willBeDiscarded[i];
				UINT8 addIndex = willBeAdded[i];

				TerrainGeometryManager::setBlockIndex(clipmapLevel, discardIndex, currentBlockIndices[addIndex]);
				TerrainGeometryManager::setBlockLocalIndex(clipmapLevel, discardIndex, currentBlockLocalIndices[addIndex]);
				TerrainGeometryManager::setIsInner(clipmapLevel, discardIndex, currentIsInners[addIndex]);
			}
		}
	}

	void TerrainGeometryManager::getCurrentBlockProperties(UINT8 level, const glm::u16vec2 gridIndex, const glm::u16vec2 startGridIndex,
		glm::u16vec2* currentBlockIndices, glm::u8vec2* currentBlockLocalIndices, UINT8* currentIsInners) {

		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				int index = y * 6 + x;
				currentBlockIndices[index] = glm::u16vec2(startGridIndex.x + x, startGridIndex.y + y);
				currentBlockLocalIndices[index] = glm::u16vec2(x, y);
				currentIsInners[index] = 0;
			}
		}

		glm::u16vec2 startInnerIndex = (gridIndex - startGridIndex) - glm::u16vec2(1);
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				int index = (y + startInnerIndex.y) * 6 + x + startInnerIndex.x;
				currentIsInners[index] = 1;
			}
		}

	}

	UINT8 TerrainGeometryManager::getMinClipmapLevel(const glm::vec2 cameraPosition, float elevation) {

		float terrainElevation = heightmapGenerator->getHeight(cameraPosition);
		elevation -= terrainElevation;

		if (elevation <= 0)
			return 0;

		const unsigned short clipmapSize = 3 * blockSize;
		unsigned short distancePrev = 0;
				
		for (int i = 0;; i++) {
			unsigned short size = clipmapSize * (1 << i);
			if (elevation > distancePrev && elevation < size)
				return i;
			distancePrev = size;
		}
	}

	glm::u16vec2 TerrainGeometryManager::getGridIndex(const UINT8 level, const glm::vec2 camPos) {

		return glm::u16vec2(camPos / glm::vec2(blockSize << level));
	}

	glm::u16vec2 TerrainGeometryManager::getStartGridIndex(const glm::u16vec2 gridIndex) {
		return  glm::u16vec2((gridIndex.x / 2) * 2 - 2, (gridIndex.y / 2) * 2 - 2);
	}

	void TerrainGeometryManager::setClipmapStartGridIndex(const UINT8 clipmapLevel, const glm::i16vec2 index) {
		clipmapStartGridIndexList[clipmapLevel] = index;
	}

	void TerrainGeometryManager::setClipmapGridIndex(const UINT8 clipmapLevel, const glm::i16vec2 index) {
		clipmapGridIndexList[clipmapLevel] = index;
	}

	glm::i16vec2& TerrainGeometryManager::getClipmapStartGridIndex(const UINT8 clipmapLevel) {
		return clipmapStartGridIndexList[clipmapLevel];
	}

	glm::i16vec2& TerrainGeometryManager::getClipmapGridIndex(const UINT8 clipmapLevel) {
		return clipmapGridIndexList[clipmapLevel];
	}

	void TerrainGeometryManager::setBlockIndex(const UINT8 clipmapLevel, const UINT8 blockIndex, const glm::u16vec2 index) {
		blockIndexList[clipmapLevel * 36 + blockIndex] = index;
	}

	glm::u16vec2& TerrainGeometryManager::getBlockIndex(const UINT8 clipmapLevel, const UINT8 blockIndex) {
		return blockIndexList[clipmapLevel * 36 + blockIndex];
	}

	void TerrainGeometryManager::setIsInner(const UINT8 clipmapLevel, const UINT8 blockIndex, const UINT8 index) {
		isInnerLists[clipmapLevel * 36 + blockIndex] = index;
	}

	UINT8& TerrainGeometryManager::getIsInner(const UINT8 clipmapLevel, const UINT8 blockIndex) {
		return isInnerLists[clipmapLevel * 36 + blockIndex];
	}

	void TerrainGeometryManager::setBlockLocalIndex(const UINT8 clipmapLevel, const UINT8 blockIndex, const glm::u8vec2 index) {
		blockLocalIndexList[clipmapLevel * 36 + blockIndex] = index;
	}

	glm::u8vec2& TerrainGeometryManager::getBlockLocalIndex(const UINT8 clipmapLevel, const UINT8 blockIndex) {
		return blockLocalIndexList[clipmapLevel * 36 + blockIndex];
	}

	void TerrainGeometryManager::setOutOfBorder(const UINT8 clipmapLevel, const UINT8 blockIndex, const UINT8 outOfBorder) {
		outOfBorderList[clipmapLevel * 36 + blockIndex] = outOfBorder;
	}

	UINT8& TerrainGeometryManager::getOutOfBorder(const UINT8 clipmapLevel, const UINT8 blockIndex) {
		return outOfBorderList[clipmapLevel * 36 + blockIndex];
	}

	glm::vec2 TerrainGeometryManager::worldSpaceToTerrainSpace(const glm::vec2 cameraPosition) {
		return cameraPosition + glm::vec2(32768);
	}

	//glm::u16vec2 TerrainGeometryManager::terrainSpaceToWorldSpace(const glm::u16vec2 blockPosition, const UINT8 level) {
	//	return blockPosition - glm::u16vec2(16384) / glm::u16vec2(1 << level);
	//}

	unsigned short TerrainGeometryManager::getTerrainMinBlockIndex(const UINT8 level) {
		return 32768 / (blockSize << level);
	}

	unsigned short TerrainGeometryManager::getTerrainMaxBlockIndex(const UINT8 level) {
		unsigned short max = terrainSize / (blockSize << level);
		return TerrainGeometryManager::getTerrainMinBlockIndex(level) + max;
	}

	glm::u16vec2 TerrainGeometryManager::getBlockIndexWorldSpace(const UINT8 clipmapLevel, const UINT8 blockIndex) {
		return TerrainGeometryManager::getBlockIndex(clipmapLevel, blockIndex) - TerrainGeometryManager::getTerrainMinBlockIndex(clipmapLevel);
	}

	glm::i16vec2 TerrainGeometryManager::getOuterDegenerateIndexWorldSpace(const UINT8 clipmapLevel) {
		glm::i16vec2 a = glm::i16vec2(TerrainGeometryManager::getClipmapStartGridIndex(clipmapLevel));
		glm::i16vec2 b = glm::i16vec2(TerrainGeometryManager::getTerrainMinBlockIndex(clipmapLevel));
		return a - b;
	}
}