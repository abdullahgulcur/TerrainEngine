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

		struct TerrainClipmap0 {

			glm::u16vec2 gridIndex;
			glm::u16vec2 startGridIndex;

			glm::u16vec2 blockIndices[BLOCK_COUNT_INNER];
			unsigned short pageTableIndices[BLOCK_COUNT_INNER];
		};

		struct TerrainClipmap {

			glm::u16vec2 gridIndex;
			glm::u16vec2 startGridIndex;

			glm::u16vec2 blockIndices[BLOCK_COUNT_PER_LEVEL];
			unsigned short pageTableIndices[BLOCK_COUNT_PER_LEVEL];
		};

	private:

	public:

		TerrainClipmap0 clipmapLevel0;
		std::vector<TerrainClipmap> clipmaps;
		std::stack<unsigned short> emptyStack;
		std::stack<Block> jobs;
		std::stack<Block> jobsLevel0;

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
		void calculateBlockPositionIndicesAtFirstLevel0(const glm::vec2 camPos);
		void calculateBlockPositionIndices(const UINT8 level, const glm::vec2 camPos);
		void calculateBlockPositionIndicesLevel0(const glm::vec2 camPos);
		void calculateBlockPositionIndicesAtFirst(const UINT8 level, const glm::vec2 camPos);
		void getBlockIndices(const glm::u16vec2 gridIndex, const glm::u16vec2 startGridIndex, glm::u16vec2* current);
		void getBlockIndicesLevel0(const glm::u16vec2 startGridIndex, glm::u16vec2* indices);
		glm::u16vec2 getGridIndex(const UINT8 level, const glm::vec2 camPos);
		glm::u16vec2 getStartGridIndex(glm::u16vec2 gridIndex);
		glm::u16vec2 getStartGridIndexLevel0(glm::u16vec2 gridIndex);
		void initEmptyStack();
		void handleBlockJobs();
		void updatePageTableTexturePartial(glm::u8vec2 pagePosition, UINT8 level, glm::u16vec2 blockPos);
		float getBorderDistance();
	};
}