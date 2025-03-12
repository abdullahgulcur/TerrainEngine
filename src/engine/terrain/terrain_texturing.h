#pragma once
#include "glm.hpp"
#include "frame_physical_pages.h"
#include "frame_page_table.h"
#include "heightmap_generator.h"

namespace Engine {

	class TerrainTexturing {

		struct BlockJob {
			glm::u16vec2 index;
			UINT8 level;
			BlockJob(UINT8 level, glm::u16vec2 index) : level(level), index(index) {}
		};

	private:

	public:

		HeightmapGenerator* heightmapGenerator;

		unsigned short terrainSize;

		UINT8 totalMipmapLevel;
		UINT8 startMipmapLevel;

		// bunlardan biri gereksiz ?
		std::vector<glm::u16vec2> mipmapGridIndexList;
		std::vector<glm::u16vec2> mipmapStartGridIndexList;

		std::stack<unsigned short> emptyStack;

		FramePhsyicalPages physicalPageGeneratorFrame;
		FramePageTable pageTableGeneratorFrame;

		std::vector<glm::u16vec2> blockIndexList;
		std::vector<unsigned short> pageTableIndexList;
		std::vector<UINT8> isActiveList;

		std::vector<int> availableMipmapLevelList;

		std::vector<std::queue<BlockJob>> blockIndexToAddQueue;

		TerrainTexturing(){}
		~TerrainTexturing(){}

		void init(unsigned short terrainSize, HeightmapGenerator* heightmapGenerator);
		void update();
		bool checkIfEveryCornerInDistance(const UINT8 clipmapLevel, const glm::vec2 cameraPosition);
		void calculateBlockPositionIndices(const UINT8 clipmapLevel, const glm::vec2 cameraPosition);
		void rasterBlock(BlockJob blockJob);
		void updatePageTableTexturePartial(unsigned short pageIndex, glm::u16vec2 blockIndex, UINT8 level);
		void getCurrentBlockProperties(const glm::u16vec2 startGridIndex, glm::u16vec2* currentBlockIndices);
		//UINT8 getMinMipmapLevel(const glm::vec2 cameraPosition, float elevation);
		bool isInBorders(const UINT8 level, const glm::u16vec2 gridIndex);
		unsigned short getTerrainMaxBlockIndex(const UINT8 level);
		glm::u16vec2 getGridIndex(const UINT8 level, const glm::vec2 camPos);
		glm::u16vec2 getStartGridIndex(glm::u16vec2 const gridIndex);
		void setMipmapStartGridIndex(const UINT8 clipmapLevel, const glm::u16vec2 index);
		void setMipmapGridIndex(const UINT8 clipmapLevel, const glm::u16vec2 index);
		glm::u16vec2& getMipmapStartGridIndex(const UINT8 clipmapLevel);
		glm::u16vec2& getMipmapGridIndex(const UINT8 clipmapLevel);
		void setBlockIndex(const UINT8 clipmapLevel, const UINT8 blockIndex, const glm::u16vec2 index);
		glm::u16vec2& getBlockIndex(const UINT8 clipmapLevel, const UINT8 blockIndex);
		void setPageTableIndex(const UINT8 clipmapLevel, const UINT8 blockIndex, const unsigned short index);
		unsigned short& getPageTableIndex(const UINT8 clipmapLevel, const UINT8 blockIndex);
		void setIsActive(const UINT8 clipmapLevel, const UINT8 blockIndex, const UINT8 isActive);
		UINT8& getIsActive(const UINT8 clipmapLevel, const UINT8 blockIndex);
	};
}