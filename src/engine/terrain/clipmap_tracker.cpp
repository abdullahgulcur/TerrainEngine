#include "pch.h"
#include "clipmap_tracker.h"
#include "texture2d.h"
#include "core.h"
#include "input.h"
#include "gl_context.h"

namespace Engine {

	ClipmapTracker::ClipmapTracker() {

	}

	ClipmapTracker::~ClipmapTracker() {

	}

	void ClipmapTracker::init(std::string path, unsigned short blockSize, UINT8 clipmapLevels) {

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
		
		ClipmapTracker::initEmptyStack();

		clipmaps = std::vector<TerrainClipmap>(clipmapLevels);
		heightmapData = HeightmapData(path, blockSize, clipmapLevels);

		Texture2D physicalTextureData(physicalTextureSize.x * blockSize, physicalTextureSize.y * blockSize, 1);
		this->heightmapTextureId = GLTexture::generateHeightmapTexture2D(physicalTextureData.channels, physicalTextureData.width, physicalTextureData.height, physicalTextureData.data);
		physicalTextureData.clean();


		Texture2D grassTexture("../../../resource/texture/marble.jpg");
		unsigned int grassTextureId = GLTexture::generateTexture2D(grassTexture.channels, grassTexture.width, grassTexture.height, grassTexture.data);
		grassTexture.clean();
		glm::ivec2 pageSize(512, 512);
		unsigned int physicalPageGeneratorShaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_RVT];
		unsigned int pageTableGeneratorShaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_PAGE_TABLE];
		physicalPageGeneratorFrame = FramePhsyicalPages(glm::u16vec2(physicalTextureSize.x * pageSize.x, physicalTextureSize.y * pageSize.y), grassTextureId, physicalPageGeneratorShaderProgramId);
		pageTableGeneratorFrame = FramePageTable(glm::u16vec2(heightmapData.rows, heightmapData.columns), pageTableGeneratorShaderProgramId);

		Camera* camera = Core::getCamera();
		glm::vec2 camPos = glm::vec2(camera->position.x, camera->position.z);

		glm::vec2 min(ClipmapTracker::getBorderDistance());
		glm::vec2 max = glm::vec2(heightmapData.terrainSize) - min;
		camPos = glm::clamp(camPos, min, max);

		ClipmapTracker::calculateBlockPositionIndicesAtFirst(camPos);
		ClipmapTracker::handleBlockJobs();
	}

	void ClipmapTracker::update() {

		Camera* camera = Core::getCamera();
		glm::vec2 camPos = glm::vec2(camera->position.x, camera->position.z);

		glm::vec2 min(ClipmapTracker::getBorderDistance());
		glm::vec2 max = glm::vec2(heightmapData.terrainSize) - min;
		camPos = glm::clamp(camPos, min, max);

		ClipmapTracker::calculateBlockPositionIndices(camPos);
		ClipmapTracker::handleBlockJobs();
	}

	void ClipmapTracker::calculateBlockPositionIndices(const glm::vec2 camPos) {

		ClipmapTracker::calculateBlockPositionIndicesLevel0(camPos);

		for (int i = 0; i < clipmaps.size(); i++)
			ClipmapTracker::calculateBlockPositionIndices(i, camPos);
	}

	void ClipmapTracker::calculateBlockPositionIndicesAtFirst(const glm::vec2 camPos) {

		ClipmapTracker::calculateBlockPositionIndicesAtFirstLevel0(camPos);

		for (int i = 0; i < clipmaps.size(); i++)
			ClipmapTracker::calculateBlockPositionIndicesAtFirst(i, camPos);
	}

	void ClipmapTracker::calculateBlockPositionIndicesAtFirst(const UINT8 level, const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(level, camPos);
		this->clipmaps[level].startGridIndex = ClipmapTracker::getStartGridIndex(gridIndex);

		glm::u16vec2 blockIndices[BLOCK_COUNT_PER_LEVEL];

		ClipmapTracker::getBlockIndices(gridIndex, this->clipmaps[level].startGridIndex, blockIndices);
		for (int i = 0; i < BLOCK_COUNT_PER_LEVEL; i++) {
			this->clipmaps[level].blockIndices[i] = blockIndices[i];

			Block block(level, static_cast<unsigned short>(i));
			this->jobs.push(block);
		}

		this->clipmaps[level].gridIndex = gridIndex;
	}

	void ClipmapTracker::calculateBlockPositionIndicesAtFirstLevel0(const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(0, camPos);
		clipmapLevel0.startGridIndex = ClipmapTracker::getStartGridIndexLevel0(gridIndex);

		glm::u16vec2 blockIndices[BLOCK_COUNT_INNER];

		ClipmapTracker::getBlockIndicesLevel0(clipmapLevel0.startGridIndex, blockIndices);
		for (int i = 0; i < BLOCK_COUNT_INNER; i++) {
			clipmapLevel0.blockIndices[i] = blockIndices[i];
			Block block(0, static_cast<unsigned short>(i));
			this->jobsLevel0.push(block);
		}

		clipmapLevel0.gridIndex = gridIndex;
	}

	void ClipmapTracker::calculateBlockPositionIndices(const UINT8 level, const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(level, camPos);
		TerrainClipmap& clipmap = this->clipmaps[level];
		clipmap.startGridIndex = ClipmapTracker::getStartGridIndex(gridIndex);

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
			ClipmapTracker::getBlockIndices(gridIndex, clipmap.startGridIndex, current);

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

	void ClipmapTracker::calculateBlockPositionIndicesLevel0(const glm::vec2 camPos) {

		glm::u16vec2 gridIndex = getGridIndex(0, camPos);
		TerrainClipmap0& clipmap = this->clipmapLevel0;
		clipmap.startGridIndex = ClipmapTracker::getStartGridIndexLevel0(gridIndex);

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
			ClipmapTracker::getBlockIndicesLevel0(clipmap.startGridIndex, current);

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

	void ClipmapTracker::getBlockIndices(const glm::u16vec2 gridIndex, const glm::u16vec2 startGridIndex, glm::u16vec2* indices) {

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

	void ClipmapTracker::getBlockIndicesLevel0(const glm::u16vec2 startGridIndex, glm::u16vec2* indices) {

		int counter = 0;
		for (int y = startGridIndex.y; y < startGridIndex.y + 3; y++)
			for (int x = startGridIndex.x; x < startGridIndex.x + 3; x++)
				indices[counter++] = glm::u16vec2(x, y);
	}

	glm::u16vec2 ClipmapTracker::getGridIndex(const UINT8 level, const glm::vec2 camPos) {

		return glm::u16vec2(camPos / glm::vec2(heightmapData.blockSize << level));
	}

	glm::u16vec2 ClipmapTracker::getStartGridIndex(glm::u16vec2 gridIndex) {

		return  glm::u16vec2((gridIndex.x / 2) * 2 - 2, (gridIndex.y / 2) * 2 - 2);
	}

	glm::u16vec2 ClipmapTracker::getStartGridIndexLevel0(glm::u16vec2 gridIndex) {

		return glm::u16vec2(gridIndex.x - 1, gridIndex.y - 1);
	}

	void ClipmapTracker::initEmptyStack() {

		int stackMaxSize = physicalTextureSize.x * physicalTextureSize.y;
		for (int i = 0; i < stackMaxSize; i++)
			this->emptyStack.push(static_cast<unsigned short>(i));
	}

	void ClipmapTracker::handleBlockJobs() {

		while (!this->jobsLevel0.empty()) {

			Block block = this->jobsLevel0.top();
			this->jobsLevel0.pop();

			unsigned short topEmpty = this->emptyStack.top();
			this->clipmapLevel0.pageTableIndices[block.blockIndex] = topEmpty;
			this->emptyStack.pop();

			glm::u16vec2 pagePosition(topEmpty % physicalTextureSize.x, topEmpty / physicalTextureSize.x);
			glm::u16vec2 blockPos = this->clipmapLevel0.blockIndices[block.blockIndex];

			ClipmapTracker::updatePageTableTexturePartial(pagePosition, block.level, blockPos);
		}

		while (!this->jobs.empty()) {

			Block block = this->jobs.top();
			this->jobs.pop();

			unsigned short topEmpty = this->emptyStack.top();
			this->clipmaps[block.level].pageTableIndices[block.blockIndex] = topEmpty;
			this->emptyStack.pop();

			glm::u16vec2 pagePosition(topEmpty % physicalTextureSize.x, topEmpty / physicalTextureSize.x);
			glm::u16vec2 blockPos = this->clipmaps[block.level].blockIndices[block.blockIndex];

			ClipmapTracker::updatePageTableTexturePartial(pagePosition, block.level, blockPos);
		}

	}

	void ClipmapTracker::updatePageTableTexturePartial(glm::u8vec2 pagePosition, UINT8 level, glm::u16vec2 blockPos) {

		glm::uvec2 pageSize(512, 512);

		glm::ivec2 blockPosInPage(pagePosition.x * heightmapData.blockSize, pagePosition.y * heightmapData.blockSize);
		glm::u16vec2 blockPosInPhysicalPagesTexture(pagePosition.x * pageSize.x, pagePosition.y * pageSize.y);
		glm::ivec2 blockPosInPageTable(blockPos.x << level, blockPos.y << level);
		glm::ivec2 blockSizeInPageTable(1 << level);

		UINT8 r = blockPos.x % 256;
		UINT8 g = blockPos.y % 256;
		UINT8 b = (pagePosition.y << 4) + pagePosition.x;
		UINT8 a = ((blockPos.y / 256) << 5) + ((blockPos.x / 256) << 3) + level;
		glm::u8vec4 tu(r, g, b, a);

		Texture2D subDataPhysicalTexture(heightmapData.blockSize, heightmapData.blockSize, 1);
		subDataPhysicalTexture.data = &heightmapData.data[heightmapData.getArrayIndex(level, blockPos)];

		GLTexture::updateTexture2D(heightmapTextureId, subDataPhysicalTexture.channels, subDataPhysicalTexture.width, subDataPhysicalTexture.height, subDataPhysicalTexture.data, blockPosInPage);

		pageTableGeneratorFrame.setViewport(blockPosInPageTable, blockSizeInPageTable);
		pageTableGeneratorFrame.setUniforms(tu);
		pageTableGeneratorFrame.draw();

		physicalPageGeneratorFrame.setViewport(blockPosInPhysicalPagesTexture, pageSize);
		physicalPageGeneratorFrame.setUniforms(level, glm::vec2(blockPos));
		physicalPageGeneratorFrame.draw();

	}

	float ClipmapTracker::getBorderDistance() {
		float sum = 0.f;
		for (int i = 0; i < clipmaps.size(); i++)
			sum += heightmapData.blockSize << i;
		return sum + 1;
	}

}