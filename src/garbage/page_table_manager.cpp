#include "pch.h"
#include "page_table_manager.h"
#include "texture2d.h"
#include "core.h"
#include "input.h"
#include "gl_context.h"
#include "heightmap.h"
#include <chrono>

//#define SIZE_T 1024

namespace Engine {

	PageTableManager::PageTableManager() {

	}

	PageTableManager::~PageTableManager() {

	}

	void PageTableManager::init(glm::u16vec2 terrainSize, unsigned short blockSize, UINT8 clipmapLevels, UINT8 innerClipmapLevel) {

		const unsigned short totalBlocks = clipmapLevels * BLOCK_COUNT_PER_LEVEL;
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

		for (int i = 0; i < clipmaps.size(); i++)
			PageTableManager::calculateBlockPositionIndices(i, camPos);
	}

	void PageTableManager::calculateBlockPositionIndicesAtFirst(const glm::vec2 camPos) {

		for (int i = 0; i < clipmaps.size(); i++)
			PageTableManager::calculateBlockPositionIndicesAtFirst(i, camPos);
	}

	void PageTableManager::calculateBlockPositionIndicesAtFirst(const UINT8 level, const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(level, camPos);
		this->clipmaps[level].startGridIndex = PageTableManager::getStartGridIndex(gridIndex);

		glm::u16vec2 blockIndices[BLOCK_COUNT_PER_LEVEL];
		unsigned short pageTableIndices[BLOCK_COUNT_PER_LEVEL];
		glm::u8vec2 blockLocalIndices[BLOCK_COUNT_PER_LEVEL];
		UINT8 isInners[BLOCK_COUNT_PER_LEVEL];

		PageTableManager::getBlockIndices(level, gridIndex, this->clipmaps[level].startGridIndex, blockIndices, pageTableIndices, blockLocalIndices, isInners);
		for (int i = 0; i < BLOCK_COUNT_PER_LEVEL; i++) {
			this->clipmaps[level].blockIndices[i] = blockIndices[i];
			this->clipmaps[level].pageTableIndices[i] = pageTableIndices[i];
			this->clipmaps[level].blockLocalIndices[i] = blockLocalIndices[i];
			this->clipmaps[level].isInners[i] = isInners[i];

			Block block(level, static_cast<unsigned short>(i));
			this->jobs.push(block);
		}

		this->clipmaps[level].gridIndex = gridIndex;
	}

	void PageTableManager::calculateBlockPositionIndices(const UINT8 level, const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(level, camPos);
		TerrainClipmap& clipmap = this->clipmaps[level];
		clipmap.startGridIndex = PageTableManager::getStartGridIndex(gridIndex);

		if (clipmap.gridIndex != gridIndex) {
			clipmap.gridIndex = gridIndex;

			glm::u16vec2 previousBlockIndices[BLOCK_COUNT_PER_LEVEL];
			unsigned short previousPageTableIndices[BLOCK_COUNT_PER_LEVEL];
			glm::u8vec2 previousBlockLocalIndices[BLOCK_COUNT_PER_LEVEL];
			bool previousIsInners[BLOCK_COUNT_PER_LEVEL];

			glm::u16vec2 currentBlockIndices[BLOCK_COUNT_PER_LEVEL];
			unsigned short currentPageTableIndices[BLOCK_COUNT_PER_LEVEL];
			glm::u8vec2 currentBlockLocalIndices[BLOCK_COUNT_PER_LEVEL];
			UINT8 currentIsInners[BLOCK_COUNT_PER_LEVEL];

			bool previousCommon[BLOCK_COUNT_PER_LEVEL];
			bool currentCommon[BLOCK_COUNT_PER_LEVEL];

			for (int i = 0; i < BLOCK_COUNT_PER_LEVEL; i++) {
				previousBlockIndices[i] = clipmap.blockIndices[i];
				previousBlockLocalIndices[i] = clipmap.blockLocalIndices[i];
				previousIsInners[i] = clipmap.isInners[i];
				previousCommon[i] = false;
				currentCommon[i] = false;
			}
			PageTableManager::getBlockIndices(level, gridIndex, clipmap.startGridIndex, currentBlockIndices, currentPageTableIndices, currentBlockLocalIndices, currentIsInners);

			int counter = BLOCK_COUNT_PER_LEVEL;
			for (int i = 0; i < BLOCK_COUNT_PER_LEVEL; i++) {
				for (int j = 0; j < BLOCK_COUNT_PER_LEVEL; j++) {
					if (previousBlockIndices[i] == currentBlockIndices[j] && previousIsInners[i] == currentIsInners[j]) {
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

				clipmap.blockIndices[discardIndex] = currentBlockIndices[addIndex];
				clipmap.blockLocalIndices[discardIndex] = currentBlockLocalIndices[addIndex];
				clipmap.isInners[discardIndex] = currentIsInners[addIndex];

				this->jobs.push(Block(level, discardIndex));
				this->emptyStack.push(clipmap.pageTableIndices[discardIndex]);
			}
		}
	}

	void PageTableManager::getBlockIndices(UINT8 level, const glm::u16vec2 gridIndex, const glm::u16vec2 startGridIndex, glm::u16vec2* currentBlockIndices, unsigned short* currentPageTableIndices, glm::u8vec2* currentBlockLocalIndices, UINT8* currentIsInners) {

		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				int index = y * 6 + x;
				currentBlockIndices[index] = glm::u16vec2(startGridIndex.x + x, startGridIndex.y + y);
				currentBlockLocalIndices[index] = glm::u16vec2(x, y);
				currentIsInners[index] = 0;
			}
		}

		if (level != 0) {
			glm::u16vec2 startInnerIndex = (gridIndex - startGridIndex) - glm::u16vec2(1);
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					int index = (y + startInnerIndex.y) * 6 + x + startInnerIndex.x;
					currentIsInners[index] = 1;
				}
			}
		}
			
	}

	//void PageTableManager::getBlockIndices(const glm::u16vec2 gridIndex, const glm::u16vec2 startGridIndex, glm::u16vec2* indices) {

	//	int counter = 0;

	//	// ---------------- DYNAMIC BLOCKS ----------------

	//	auto findIndicesHorizontal = [](glm::u16vec2 startGridIndex, glm::u16vec2* indices, int xOffset, int yOffset, int& index)
	//	{
	//		int start = startGridIndex.x + xOffset;
	//		int end = start + 4;
	//		for (int i = start; i < end; i++)
	//			indices[index++] = glm::u16vec2(i, startGridIndex.y + yOffset);
	//	};
	//	auto findIndicesVertical = [](glm::u16vec2 startGridIndex, glm::u16vec2* indices, int xOffset, int yOffset, int& index)
	//	{
	//		int start = startGridIndex.y + yOffset;
	//		int end = start + 3;
	//		for (int i = start; i < end; i++)
	//			indices[index++] = glm::u16vec2(startGridIndex.x + xOffset, i);
	//	};

	//	if (gridIndex.x % 2 == 0 && gridIndex.y % 2 == 0) {

	//		findIndicesHorizontal(startGridIndex, indices, 1, 4, counter);
	//		findIndicesVertical(startGridIndex, indices, 4, 1, counter);
	//	}
	//	else if (gridIndex.x % 2 == 1 && gridIndex.y % 2 == 0) {

	//		findIndicesHorizontal(startGridIndex, indices, 1, 4, counter);
	//		findIndicesVertical(startGridIndex, indices, 1, 1, counter);
	//	}
	//	else if (gridIndex.x % 2 == 0 && gridIndex.y % 2 == 1) {

	//		findIndicesHorizontal(startGridIndex, indices, 1, 1, counter);
	//		findIndicesVertical(startGridIndex, indices, 4, 2, counter);
	//	}
	//	else {

	//		findIndicesHorizontal(startGridIndex, indices, 1, 1, counter);
	//		findIndicesVertical(startGridIndex, indices, 1, 2, counter);
	//	}

	//	// ---------------- STATIC BLOCKS ----------------

	//	for (int i = startGridIndex.x; i < startGridIndex.x + 6; i++) {

	//		// above horizontal
	//		indices[counter++] = glm::u16vec2(i, startGridIndex.y);

	//		// below horizontal
	//		indices[counter++] = glm::u16vec2(i, startGridIndex.y + 5);
	//	}

	//	for (int i = startGridIndex.y + 1; i < startGridIndex.y + 5; i++) {

	//		// left vertical
	//		indices[counter++] = glm::u16vec2(startGridIndex.x, i);

	//		// right vertical
	//		indices[counter++] = glm::u16vec2(startGridIndex.x + 5, i);
	//	}
	//}

	glm::u16vec2 PageTableManager::getGridIndex(const UINT8 level, const glm::vec2 camPos) {

		return glm::u16vec2(camPos / glm::vec2(heightmapData.blockSize << level));
	}

	glm::u16vec2 PageTableManager::getStartGridIndex(glm::u16vec2 gridIndex) {

		return  glm::u16vec2((gridIndex.x / 2) * 2 - 2, (gridIndex.y / 2) * 2 - 2);
	}

	void PageTableManager::initEmptyStack() {

		int stackMaxSize = physicalTextureSize.x * physicalTextureSize.y;
		for (int i = 0; i < stackMaxSize; i++)
			this->emptyStack.push(static_cast<unsigned short>(i));
	}

	void PageTableManager::handleBlockJobs() {

		bool jobDone = false;


		while (!this->jobs.empty()) {

			Block block = this->jobs.top();
			this->jobs.pop();

			unsigned short topEmpty = this->emptyStack.top();
			this->clipmaps[block.level].pageTableIndices[block.blockIndex] = topEmpty;
			this->emptyStack.pop();

			glm::u16vec2 pagePosition(topEmpty % physicalTextureSize.x, topEmpty / physicalTextureSize.x);
			glm::u16vec2 blockPos = this->clipmaps[block.level].blockIndices[block.blockIndex];
			UINT8 inner = this->clipmaps[block.level].isInners[block.blockIndex];

			PageTableManager::updatePageTableTexturePartial(pagePosition, block.level, blockPos, inner);

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
		//exit(3);
		//// Record the end time
		//auto end = std::chrono::high_resolution_clock::now();

		//// Calculate the elapsed time in milliseconds
		//unsigned long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		//if(elapsed != 0)
		//	std::cout << "Elapsed time: " << elapsed << " micro seconds" << std::endl;
		
	}

	void PageTableManager::updatePageTableTexturePartial(glm::u8vec2 pagePosition, UINT8 level, glm::u16vec2 blockPos, UINT8 inner) {

		glm::uvec2 pageSize(SIZE_T, SIZE_T);

		glm::ivec2 blockPosInPage(pagePosition.x * heightmapData.blockSize, pagePosition.y * heightmapData.blockSize);
		glm::u16vec2 blockPosInPhysicalPagesTexture(pagePosition.x * pageSize.x, pagePosition.y * pageSize.y);

		unsigned short pageTableSizeX = heightmapData.columns;

		if (level != 0)
			int aaa = 5;

		unsigned short xOffset = 0;
		UINT8 maxLevel = heightmapData.mipStartIndices.size() - 1;
		for (int i = 0; i < level; i++) {
			xOffset += pageTableSizeX;
			pageTableSizeX >>= 1;
		}

		glm::ivec2 blockPosInPageTable = blockPos;
		blockPosInPageTable.x += xOffset;

		unsigned int pageTableColor = (pagePosition.x << 8) + (pagePosition.y << 4) + level;

		Heightmap subDataPhysicalTexture(heightmapData.blockSize, heightmapData.blockSize);
		subDataPhysicalTexture.data = &heightmapData.data[heightmapData.getArrayIndex(level, blockPos)];

		GLTexture::updateHeightmapPhysicalTexture(heightmapTextureId, subDataPhysicalTexture.width, subDataPhysicalTexture.height, subDataPhysicalTexture.data, blockPosInPage);

		pageTableGeneratorFrame.setViewport(blockPosInPageTable, glm::ivec2(1));
		pageTableGeneratorFrame.setUniforms(pageTableColor, level, inner);
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