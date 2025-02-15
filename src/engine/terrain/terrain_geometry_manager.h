#pragma once

#include "glm.hpp"
#include "heightmap_generator.h"

namespace Engine {

	class TerrainGeometryManager {

	private:
		HeightmapGenerator* heightmapGenerator;

	public:

		unsigned short terrainSize;
		UINT8 blockSize;

		UINT8 totalClipmapLevel;
		UINT8 startClipmapLevel;

		std::vector<glm::u16vec2> clipmapGridIndexList;
		std::vector<glm::u16vec2> clipmapStartGridIndexList;

		std::vector<glm::u16vec2> blockIndexList;
		std::vector<glm::u8vec2> blockLocalIndexList;
		std::vector<UINT8> isInnerLists;
		std::vector<UINT8> outOfBorderList;

		TerrainGeometryManager(){}
		~TerrainGeometryManager(){}

		void init(unsigned short terrainSize, unsigned short blockSize, HeightmapGenerator* heightmapGenerator);
		void update();
		void setOutOfBorderList(const UINT8 clipmapLevel);
		void initBlockProperties(const glm::vec2 cameraPosition);
		void calculateBlockPositionIndices(const UINT8 clipmapLevel, const glm::vec2 cameraPosition);
		void getCurrentBlockProperties(UINT8 level, const glm::u16vec2 gridIndex, const glm::u16vec2 startGridIndex,
			glm::u16vec2* currentBlockIndices, glm::u8vec2* currentBlockLocalIndices, UINT8* currentIsInners);
		UINT8 getMinClipmapLevel(const glm::vec2 cameraPosition, float elevation);
		glm::u16vec2 getGridIndex(const UINT8 level, const glm::vec2 camPos);
		glm::u16vec2 getStartGridIndex(glm::u16vec2 const gridIndex);
		void setClipmapStartGridIndex(const UINT8 clipmapLevel, const glm::u16vec2 index);
		void setClipmapGridIndex(const UINT8 clipmapLevel, const glm::u16vec2 index);
		glm::u16vec2& getClipmapStartGridIndex(const UINT8 clipmapLevel);
		glm::u16vec2& getClipmapGridIndex(const UINT8 clipmapLevel);
		void setBlockIndex(const UINT8 clipmapLevel, const UINT8 blockIndex, const glm::u16vec2 index);
		glm::u16vec2& getBlockIndex(const UINT8 clipmapLevel, const UINT8 blockIndex);
		void setIsInner(const UINT8 clipmapLevel, const UINT8 blockIndex, const UINT8 index);
		UINT8& getIsInner(const UINT8 clipmapLevel, const UINT8 blockIndex);
		void setBlockLocalIndex(const UINT8 clipmapLevel, const UINT8 blockIndex, const glm::u8vec2 index);
		glm::u8vec2& getBlockLocalIndex(const UINT8 clipmapLevel, const UINT8 blockIndex);
		void setOutOfBorder(const UINT8 clipmapLevel, const UINT8 blockIndex, const UINT8 outOfBorder);
		UINT8& getOutOfBorder(const UINT8 clipmapLevel, const UINT8 blockIndex);
		glm::vec2 worldSpaceToTerrainSpace(const glm::vec2 cameraPosition);
		glm::u16vec2 terrainSpaceToWorldSpace(const glm::u16vec2 blockPosition, const UINT8 level);
		unsigned short getTerrainMinBlockIndex(const UINT8 level);
		unsigned short getTerrainMaxBlockIndex(const UINT8 level);
		glm::u16vec2 getBlockIndexWorldSpace(const UINT8 clipmapLevel, const UINT8 blockIndex);
		glm::u16vec2 getOuterDegenerateIndexWorldSpace(const UINT8 clipmapLevel);
	};
}