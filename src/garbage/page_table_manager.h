#pragma once
#include "glm.hpp"
#include "terrain_config.h"
#include "heightmap_data.h"
#include "frame_physical_pages.h"
#include "frame_page_table.h"

namespace Engine {

	class PageTableManager {

		struct Block {
			UINT8 level;
			unsigned short blockIndex;
			Block(UINT8 level, unsigned short blockIndex) : level(level), blockIndex(blockIndex) {}
		};

		struct TerrainClipmap {

			glm::u16vec2 gridIndex;
			glm::u16vec2 startGridIndex;

			glm::u16vec2 blockIndices[BLOCK_COUNT_PER_LEVEL];
			unsigned short pageTableIndices[BLOCK_COUNT_PER_LEVEL];
			glm::u8vec2 blockLocalIndices[BLOCK_COUNT_PER_LEVEL];
			UINT8 isInners[BLOCK_COUNT_PER_LEVEL];
		};

	private:

	public:

		std::vector<TerrainClipmap> clipmaps;
		std::stack<unsigned short> emptyStack;
		std::stack<Block> jobs;

		HeightmapData heightmapData;
		unsigned int heightmapTextureId;
		
		FramePhsyicalPages physicalPageGeneratorFrame;
		FramePageTable pageTableGeneratorFrame;

		glm::u8vec2 physicalTextureSize;

		PageTableManager();
		~PageTableManager();

		void init(glm::u16vec2 terrainSize, unsigned short blockSize, UINT8 clipmapLevels, UINT8 innerClipmapLevel);
		void update();
		void calculateBlockPositionIndices(const glm::vec2 camPos);
		void calculateBlockPositionIndicesAtFirst(const glm::vec2 camPos);
		void calculateBlockPositionIndices(const UINT8 level, const glm::vec2 camPos);
		void calculateBlockPositionIndicesAtFirst(const UINT8 level, const glm::vec2 camPos);
		void getBlockIndices(UINT8 level, const glm::u16vec2 gridIndex, const glm::u16vec2 startGridIndex, glm::u16vec2* currentBlockIndices, unsigned short* currentPageTableIndices, glm::u8vec2* currentBlockLocalIndices, UINT8* currentIsInners);
		glm::u16vec2 getGridIndex(const UINT8 level, const glm::vec2 camPos);
		glm::u16vec2 getStartGridIndex(glm::u16vec2 gridIndex);
		void initEmptyStack();
		void handleBlockJobs();
		void updatePageTableTexturePartial(glm::u8vec2 pagePosition, UINT8 level, glm::u16vec2 blockPos, UINT8 inner);
		float getBorderDistance();
	};
}