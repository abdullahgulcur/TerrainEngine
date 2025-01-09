#include "pch.h"
#include "page_table_manager.h"
#include "texture2d.h"
#include "core.h"
#include "input.h"
#include "gl_context.h"
#include "heightmap.h"
#include <chrono>

#define SIZE_T 512

namespace Engine {

	PageTableManager::PageTableManager() {

	}

	PageTableManager::~PageTableManager() {

	}

	void PageTableManager::init(glm::u16vec2 terrainSize, unsigned short blockSize, UINT8 clipmapLevels, UINT8 innerClipmapLevel) {

		const unsigned short totalBlocks = clipmapLevels * BLOCK_COUNT_PER_LEVEL + BLOCK_COUNT_INNER;
		for (int i = 0;; i++) {
			unsigned short width = 1 << (i / 2);
			unsigned short height = 1 << ((i+1) / 2);
			if (width * height > totalBlocks) {
				physicalTextureSize.x = width;
				physicalTextureSize.y = height;
				break;
			}
		}
		
		PageTableManager::initEmptyStack();

		clipmaps = std::vector<TerrainClipmap>(clipmapLevels);

		HeightmapGenerator hg(terrainSize, 8, 4, 152746, 500); //  * glm::u16vec2(1 << innerClipmapLevel)
		heightmapData = HeightmapData(hg, blockSize, clipmapLevels);
		//heightmapData = HeightmapData(path, blockSize, clipmapLevels);

		Heightmap physicalTextureData(physicalTextureSize.x * blockSize, physicalTextureSize.y * blockSize);
		this->heightmapTextureId = GLTexture::generateHeightmapTexture(glm::u16vec2(physicalTextureData.width, physicalTextureData.height));
		physicalTextureData.clean();

		glm::ivec2 pageSize(SIZE_T, SIZE_T);
		unsigned int pageTableGeneratorShaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_PAGE_TABLE];
		physicalPageGeneratorFrame = FramePhsyicalPages(glm::u16vec2(physicalTextureSize.x * pageSize.x, physicalTextureSize.y * pageSize.y), heightmapTextureId, blockSize);
		pageTableGeneratorFrame = FramePageTable(glm::u16vec2(heightmapData.rows, heightmapData.columns), pageTableGeneratorShaderProgramId);

		Camera* camera = Core::getCamera();
		glm::vec2 camPos = glm::vec2(camera->position.x, camera->position.z);

		glm::vec2 min(PageTableManager::getBorderDistance());
		glm::vec2 max = glm::vec2(heightmapData.terrainSize) - min;
		camPos = glm::clamp(camPos, min, max);

		PageTableManager::calculateBlockPositionIndicesAtFirst(camPos);
		PageTableManager::handleBlockJobs();
	}

	void PageTableManager::update() {

		Camera* camera = Core::getCamera();
		glm::vec2 camPos = glm::vec2(camera->position.x, camera->position.z);

		glm::vec2 min(PageTableManager::getBorderDistance());
		glm::vec2 max = glm::vec2(heightmapData.terrainSize) - min;
		camPos = glm::clamp(camPos, min, max);

		PageTableManager::calculateBlockPositionIndices(camPos);
		PageTableManager::handleBlockJobs();
	}

	void PageTableManager::calculateBlockPositionIndices(const glm::vec2 camPos) {

		PageTableManager::calculateBlockPositionIndicesLevel0(camPos);

		for (int i = 0; i < clipmaps.size(); i++)
			PageTableManager::calculateBlockPositionIndices(i, camPos);
	}

	void PageTableManager::calculateBlockPositionIndicesAtFirst(const glm::vec2 camPos) {

		PageTableManager::calculateBlockPositionIndicesAtFirstLevel0(camPos);

		for (int i = 0; i < clipmaps.size(); i++)
			PageTableManager::calculateBlockPositionIndicesAtFirst(i, camPos);
	}

	void PageTableManager::calculateBlockPositionIndicesAtFirst(const UINT8 level, const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(level, camPos);
		this->clipmaps[level].startGridIndex = PageTableManager::getStartGridIndex(gridIndex);

		glm::u16vec2 blockIndices[BLOCK_COUNT_PER_LEVEL];

		PageTableManager::getBlockIndices(gridIndex, this->clipmaps[level].startGridIndex, blockIndices);
		for (int i = 0; i < BLOCK_COUNT_PER_LEVEL; i++) {
			this->clipmaps[level].blockIndices[i] = blockIndices[i];

			Block block(level, static_cast<unsigned short>(i));
			this->jobs.push(block);
		}

		this->clipmaps[level].gridIndex = gridIndex;
	}

	void PageTableManager::calculateBlockPositionIndicesAtFirstLevel0(const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(0, camPos);
		clipmapLevel0.startGridIndex = PageTableManager::getStartGridIndexLevel0(gridIndex);

		glm::u16vec2 blockIndices[BLOCK_COUNT_INNER];

		PageTableManager::getBlockIndicesLevel0(clipmapLevel0.startGridIndex, blockIndices);
		for (int i = 0; i < BLOCK_COUNT_INNER; i++) {
			clipmapLevel0.blockIndices[i] = blockIndices[i];
			Block block(0, static_cast<unsigned short>(i));
			this->jobsLevel0.push(block);
		}

		clipmapLevel0.gridIndex = gridIndex;
	}

	void PageTableManager::calculateBlockPositionIndices(const UINT8 level, const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(level, camPos);
		TerrainClipmap& clipmap = this->clipmaps[level];
		clipmap.startGridIndex = PageTableManager::getStartGridIndex(gridIndex);

		if (clipmap.gridIndex != gridIndex) {
			clipmap.gridIndex = gridIndex;

			glm::u16vec2 previous[BLOCK_COUNT_PER_LEVEL];
			glm::u16vec2 current[BLOCK_COUNT_PER_LEVEL];
			bool previousCommon[BLOCK_COUNT_PER_LEVEL];
			bool currentCommon[BLOCK_COUNT_PER_LEVEL];

			for (int i = 0; i < BLOCK_COUNT_PER_LEVEL; i++) {
				previous[i] = clipmap.blockIndices[i];
				previousCommon[i] = false;
				currentCommon[i] = false;
			}
			PageTableManager::getBlockIndices(gridIndex, clipmap.startGridIndex, current);

			int counter = BLOCK_COUNT_PER_LEVEL;
			for (int i = 0; i < BLOCK_COUNT_PER_LEVEL; i++) {
				for (int j = 0; j < BLOCK_COUNT_PER_LEVEL; j++) {
					if (previous[i] == current[j]) {
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
			for (UINT8 i = 0; i < BLOCK_COUNT_PER_LEVEL; i++) {
				if (!previousCommon[i])
					willBeDiscarded[lastPrevious++] = i;
				if (!currentCommon[i])
					willBeAdded[lastCurrent++] = i;
			}

			for (int i = 0; i < willBeDiscarded.size(); i++) {
				UINT8 discardIndex = willBeDiscarded[i];
				UINT8 addIndex = willBeAdded[i];
				clipmap.blockIndices[discardIndex] = current[addIndex];
				this->jobs.push(Block(level, discardIndex));
				this->emptyStack.push(clipmap.pageTableIndices[discardIndex]);
			}
		}
	}

	void PageTableManager::calculateBlockPositionIndicesLevel0(const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(0, camPos);
		TerrainClipmap0& clipmap = this->clipmapLevel0;
		clipmap.startGridIndex = PageTableManager::getStartGridIndexLevel0(gridIndex);

		if (clipmap.gridIndex != gridIndex) {
			clipmap.gridIndex = gridIndex;

			glm::u16vec2 previous[BLOCK_COUNT_INNER];
			glm::u16vec2 current[BLOCK_COUNT_INNER];
			bool previousCommon[BLOCK_COUNT_INNER];
			bool currentCommon[BLOCK_COUNT_INNER];

			for (int i = 0; i < BLOCK_COUNT_INNER; i++) {
				previous[i] = clipmap.blockIndices[i];
				previousCommon[i] = false;
				currentCommon[i] = false;
			}
			PageTableManager::getBlockIndicesLevel0(clipmap.startGridIndex, current);

			int counter = BLOCK_COUNT_INNER;
			for (int i = 0; i < BLOCK_COUNT_INNER; i++) {
				for (int j = 0; j < BLOCK_COUNT_INNER; j++) {
					if (previous[i] == current[j]) {
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
			for (UINT8 i = 0; i < BLOCK_COUNT_INNER; i++) {
				if (!previousCommon[i])
					willBeDiscarded[lastPrevious++] = i;
				if (!currentCommon[i])
					willBeAdded[lastCurrent++] = i;
			}

			for (int i = 0; i < willBeDiscarded.size(); i++) {
				UINT8 discardIndex = willBeDiscarded[i];
				UINT8 addIndex = willBeAdded[i];
				clipmap.blockIndices[discardIndex] = current[addIndex];
				this->jobsLevel0.push(Block(0, discardIndex));
				this->emptyStack.push(clipmap.pageTableIndices[discardIndex]);
			}
		}
	}

	void PageTableManager::getBlockIndices(const glm::u16vec2 gridIndex, const glm::u16vec2 startGridIndex, glm::u16vec2* indices) {

		int counter = 0;

		// ---------------- DYNAMIC BLOCKS ----------------

		auto findIndicesHorizontal = [](glm::u16vec2 startGridIndex, glm::u16vec2* indices, int xOffset, int yOffset, int& index)
		{
			int start = startGridIndex.x + xOffset;
			int end = start + 4;
			for (int i = start; i < end; i++)
				indices[index++] = glm::u16vec2(i, startGridIndex.y + yOffset);
		};
		auto findIndicesVertical = [](glm::u16vec2 startGridIndex, glm::u16vec2* indices, int xOffset, int yOffset, int& index)
		{
			int start = startGridIndex.y + yOffset;
			int end = start + 3;
			for (int i = start; i < end; i++)
				indices[index++] = glm::u16vec2(startGridIndex.x + xOffset, i);
		};

		if (gridIndex.x % 2 == 0 && gridIndex.y % 2 == 0) {

			findIndicesHorizontal(startGridIndex, indices, 1, 4, counter);
			findIndicesVertical(startGridIndex, indices, 4, 1, counter);
		}
		else if (gridIndex.x % 2 == 1 && gridIndex.y % 2 == 0) {

			findIndicesHorizontal(startGridIndex, indices, 1, 4, counter);
			findIndicesVertical(startGridIndex, indices, 1, 1, counter);
		}
		else if (gridIndex.x % 2 == 0 && gridIndex.y % 2 == 1) {

			findIndicesHorizontal(startGridIndex, indices, 1, 1, counter);
			findIndicesVertical(startGridIndex, indices, 4, 2, counter);
		}
		else {

			findIndicesHorizontal(startGridIndex, indices, 1, 1, counter);
			findIndicesVertical(startGridIndex, indices, 1, 2, counter);
		}

		// ---------------- STATIC BLOCKS ----------------

		for (int i = startGridIndex.x; i < startGridIndex.x + 6; i++) {

			// above horizontal
			indices[counter++] = glm::u16vec2(i, startGridIndex.y);

			// below horizontal
			indices[counter++] = glm::u16vec2(i, startGridIndex.y + 5);
		}

		for (int i = startGridIndex.y + 1; i < startGridIndex.y + 5; i++) {

			// left vertical
			indices[counter++] = glm::u16vec2(startGridIndex.x, i);

			// right vertical
			indices[counter++] = glm::u16vec2(startGridIndex.x + 5, i);
		}
	}

	void PageTableManager::getBlockIndicesLevel0(const glm::u16vec2 startGridIndex, glm::u16vec2* indices) {

		int counter = 0;
		for (int y = startGridIndex.y; y < startGridIndex.y + 3; y++)
			for (int x = startGridIndex.x; x < startGridIndex.x + 3; x++)
				indices[counter++] = glm::u16vec2(x, y);
	}

	glm::u16vec2 PageTableManager::getGridIndex(const UINT8 level, const glm::vec2 camPos) {

		return glm::u16vec2(camPos / glm::vec2(heightmapData.blockSize << level));
	}

	glm::u16vec2 PageTableManager::getStartGridIndex(glm::u16vec2 gridIndex) {

		return  glm::u16vec2((gridIndex.x / 2) * 2 - 2, (gridIndex.y / 2) * 2 - 2);
	}

	glm::u16vec2 PageTableManager::getStartGridIndexLevel0(glm::u16vec2 gridIndex) {

		return glm::u16vec2(gridIndex.x - 1, gridIndex.y - 1);
	}

	void PageTableManager::initEmptyStack() {

		int stackMaxSize = physicalTextureSize.x * physicalTextureSize.y;
		for (int i = 0; i < stackMaxSize; i++)
			this->emptyStack.push(static_cast<unsigned short>(i));
	}

	void PageTableManager::handleBlockJobs() {

		bool jobDone = false;

		while (!this->jobsLevel0.empty()) {

			Block block = this->jobsLevel0.top();
			this->jobsLevel0.pop();

			unsigned short topEmpty = this->emptyStack.top();
			this->clipmapLevel0.pageTableIndices[block.blockIndex] = topEmpty;
			this->emptyStack.pop();

			glm::u16vec2 pagePosition(topEmpty % physicalTextureSize.x, topEmpty / physicalTextureSize.x);
			glm::u16vec2 blockPos = this->clipmapLevel0.blockIndices[block.blockIndex];

			PageTableManager::updatePageTableTexturePartial(pagePosition, block.level, blockPos);

			jobDone = true;
		}

		while (!this->jobs.empty()) {

			Block block = this->jobs.top();
			this->jobs.pop();

			unsigned short topEmpty = this->emptyStack.top();
			this->clipmaps[block.level].pageTableIndices[block.blockIndex] = topEmpty;
			this->emptyStack.pop();

			glm::u16vec2 pagePosition(topEmpty % physicalTextureSize.x, topEmpty / physicalTextureSize.x);
			glm::u16vec2 blockPos = this->clipmaps[block.level].blockIndices[block.blockIndex];

			PageTableManager::updatePageTableTexturePartial(pagePosition, block.level, blockPos);

			jobDone = true;
		}

		auto start = std::chrono::high_resolution_clock::now();

		if (jobDone) {
			glBindTexture(GL_TEXTURE_2D, physicalPageGeneratorFrame.texture_0_Id);
			glGenerateMipmap(GL_TEXTURE_2D);
			//glBindTexture(GL_TEXTURE_2D, physicalPageGeneratorFrame.texture_1_Id);
			//glGenerateMipmap(GL_TEXTURE_2D);
		}
		
		//Texture2D texTemp(physicalPageGeneratorFrame.size.x, physicalPageGeneratorFrame.size.y, 3);
		//GLTexture::getTextureContent(3, texTemp.data, physicalPageGeneratorFrame.texture_0_Id);
		//texTemp.writeDataToFile("testphysicalpages.png", 0);
		//
		//int x = 5;

		//// Record the end time
		//auto end = std::chrono::high_resolution_clock::now();

		//// Calculate the elapsed time in milliseconds
		//unsigned long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		//if(elapsed != 0)
		//	std::cout << "Elapsed time: " << elapsed << " micro seconds" << std::endl;
		
	}

	void PageTableManager::updatePageTableTexturePartial(glm::u8vec2 pagePosition, UINT8 level, glm::u16vec2 blockPos) {

		glm::uvec2 pageSize(SIZE_T, SIZE_T);

		glm::ivec2 blockPosInPage(pagePosition.x * heightmapData.blockSize, pagePosition.y * heightmapData.blockSize);
		glm::u16vec2 blockPosInPhysicalPagesTexture(pagePosition.x * pageSize.x, pagePosition.y * pageSize.y);
		glm::ivec2 blockPosInPageTable(blockPos.x << level, blockPos.y << level);
		glm::ivec2 blockSizeInPageTable(1 << level);

		unsigned int pageTableColor = (pagePosition.x << 8) + (pagePosition.y << 4) + level;

		Heightmap subDataPhysicalTexture(heightmapData.blockSize, heightmapData.blockSize);
		subDataPhysicalTexture.data = &heightmapData.data[heightmapData.getArrayIndex(level, blockPos)];

		GLTexture::updateHeightmapPhysicalTexture(heightmapTextureId, subDataPhysicalTexture.width, subDataPhysicalTexture.height, subDataPhysicalTexture.data, blockPosInPage);

		pageTableGeneratorFrame.setViewport(blockPosInPageTable, blockSizeInPageTable);
		pageTableGeneratorFrame.setUniforms(pageTableColor);
		pageTableGeneratorFrame.draw();

		physicalPageGeneratorFrame.setViewport(blockPosInPhysicalPagesTexture, pageSize);
		physicalPageGeneratorFrame.setUniforms(level, blockPos, pagePosition, physicalTextureSize);
		physicalPageGeneratorFrame.draw();
	}

	float PageTableManager::getBorderDistance() {
		float sum = 0.f;
		for (int i = 0; i < clipmaps.size() + 1; i++)
			sum += heightmapData.blockSize << i;
		return sum;
	}

}