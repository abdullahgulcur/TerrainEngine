#include "pch.h"
#include "terrain_texturing.h"
#include "core.h"
//#include "gl_context.h"
//#include "texture2d.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//#define STBI_MSC_SECURE_CRT
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

namespace Engine {

	void TerrainTexturing::init(unsigned short terrainSize, HeightmapGenerator* heightmapGenerator) {

		this->heightmapGenerator = heightmapGenerator;
		this->terrainSize = terrainSize;

		//---------

		// terrainin bir kosesinden baktigini hayal et. en uc noktaya bak. terrainsize * kok2  = max distance.
		// margin filan da onemli.
		// border terrain oldugu zaman...

		float margin = 0.2;
		float baseDistance = 2 * (1 - margin);
		float maxDist = terrainSize * glm::sqrt(2);
		int maxLodLevel = int(glm::floor(glm::log2(maxDist / (baseDistance * 0.5))));

		totalMipmapLevel = maxLodLevel + 1; /////

		//---------

		const unsigned short pageCounts = totalMipmapLevel * 25;

		mipmapGridIndexList = std::vector<glm::u16vec2>(totalMipmapLevel);
		mipmapStartGridIndexList = std::vector<glm::u16vec2>(totalMipmapLevel);
		blockIndexToAddQueue = std::vector<std::queue<BlockJob>>(totalMipmapLevel);
		//availableMipmapLevelList = std::vector<int>(totalMipmapLevel, -1);
		//availableMipmapLevelList.back() = totalMipmapLevel - 1;

		//for (int i = 0; i < totalMipmapLevel; i++)
		//	availableMipmapLevelList[i] = i;

		blockIndexList = std::vector<glm::u16vec2>(pageCounts);
		pageTableIndexList = std::vector<unsigned short>(pageCounts);
		isActiveList = std::vector<UINT8>(pageCounts, 0);

		//---------

		for (int i = 0; i < pageCounts; i++)
			this->emptyStack.push(i);

		//---------

		physicalPageGeneratorFrame = FramePhsyicalPages(pageCounts, 1024, heightmapGenerator->textureId);
		pageTableGeneratorFrame = FramePageTable(glm::u8vec2(4));

		//---------

		Camera* camera = Core::getCamera();
		glm::vec2 camPos = glm::vec2(camera->position.x, camera->position.z);
		camPos = glm::clamp(glm::vec2(1), glm::vec2(terrainSize - 1), camPos);

		for (int i = 0; i < totalMipmapLevel; i++) {
			const glm::u16vec2 gridIndex = TerrainTexturing::getGridIndex(i, camPos);
			const glm::u16vec2 startGridIndex = TerrainTexturing::getStartGridIndex(gridIndex);
			TerrainTexturing::setMipmapStartGridIndex(i, startGridIndex);
			TerrainTexturing::setMipmapGridIndex(i, gridIndex);

			glm::u16vec2* blockIndexList = &TerrainTexturing::getBlockIndex(i, 0);
			TerrainTexturing::getCurrentBlockProperties(startGridIndex, blockIndexList);

			for (int j = 0; j < 25; j++) {
				if (TerrainTexturing::isInBorders(i, blockIndexList[j]))
					blockIndexToAddQueue[i].push(BlockJob(i, blockIndexList[j]));
			}
		}

		//---------

		//currentMipmapIndex = TerrainTexturing::getMinMipmapLevel(camPos, camera->position.y);
		//currentMipmapIndexForShader = currentMipmapIndex;

		for (int i = 0; i < totalMipmapLevel; i++) {
			while (!blockIndexToAddQueue[i].empty()) {
				TerrainTexturing::rasterBlock(blockIndexToAddQueue[i].front());
				blockIndexToAddQueue[i].pop();
			}
		}
	}

	void TerrainTexturing::update() {

		Camera* camera = Core::getCamera();
		glm::vec2 camPos = glm::vec2(camera->position.x, camera->position.z);
		camPos = glm::clamp(glm::vec2(1), glm::vec2(terrainSize - 1), camPos);

		//currentMipmapIndex = 0; //TerrainTexturing::getMinMipmapLevel(camPos, camera->position.y);
		//currentMipmapIndexForShader = currentMipmapIndex;

		for (int i = 0; i < totalMipmapLevel; i++)
			TerrainTexturing::calculateBlockPositionIndices(i, camPos);

		// priority queue depending on camera elevetion
		//for (int i = 0; i < totalMipmapLevel; i++) {
		//	if (!blockIndexToAddQueue[i].empty()) {
		//		TerrainTexturing::rasterBlock(blockIndexToAddQueue[i].front());
		//		blockIndexToAddQueue[i].pop();
		//		break;
		//	}
		//}
		for (int i = totalMipmapLevel - 1; i >= 0; i--) {
			if (!blockIndexToAddQueue[i].empty()) {
				TerrainTexturing::rasterBlock(blockIndexToAddQueue[i].front());
				blockIndexToAddQueue[i].pop();
				break;
			}
		}



		//for (int i = 0; i < totalMipmapLevel; i++)
		//	availableMipmapLevelList[i] = TerrainTexturing::checkIfEveryCornerInDistance(i, camPos) ? i : -1;


		//for (int i = 0; i < totalMipmapLevel; i++) {
		//	if (availableMipmapLevelList[i] == -1) {
		//		for (int j = i + 1; j < totalMipmapLevel; j++) {
		//			if (availableMipmapLevelList[j] != -1) {
		//				availableMipmapLevelList[i] = availableMipmapLevelList[j];
		//				break;
		//			}
		//		}
		//	}
		//}

		/*if (true) {
			Texture2D texTemp(physicalPageGeneratorFrame.size.x, physicalPageGeneratorFrame.size.y, 3);
			GLTexture::getTextureContent(3, texTemp.data, physicalPageGeneratorFrame.texture_0_Id);
			texTemp.writeDataToFile("testphysicalpages.png", 0);
			std::exit(0);
		}*/
	}

	bool TerrainTexturing::checkIfEveryCornerInDistance(const UINT8 clipmapLevel, const glm::vec2 cameraPosition) {

		return true;

		float margin = 0.2; ///// ?????

		for (int i = 0; i < 25; i++) {

			UINT8 isActive = TerrainTexturing::getIsActive(clipmapLevel, i);
			if (isActive)
				continue;

			unsigned short sideLength = (1 << clipmapLevel);
			glm::vec2 blockCorner0 = glm::vec2(TerrainTexturing::getBlockIndex(clipmapLevel, i)) + glm::vec2(1 << clipmapLevel) * 0.5f;;// +glm::vec2(1 << clipmapLevel) * 0.5f;
			glm::u16vec2 blockCorner1(blockCorner0.x + sideLength, blockCorner0.y);
			glm::u16vec2 blockCorner2(blockCorner0.x, blockCorner0.y + sideLength);
			glm::u16vec2 blockCorner3(blockCorner0.x + sideLength, blockCorner0.y + sideLength);

			if (!glm::distance(cameraPosition, glm::vec2(blockCorner0)) < (2.f - margin) * (1 << clipmapLevel))
				return false;
			if (!glm::distance(cameraPosition, glm::vec2(blockCorner1)) < (2.f - margin) * (1 << clipmapLevel))
				return false;
			if (!glm::distance(cameraPosition, glm::vec2(blockCorner2)) < (2.f - margin) * (1 << clipmapLevel))
				return false;
			if (!glm::distance(cameraPosition, glm::vec2(blockCorner3)) < (2.f - margin) * (1 << clipmapLevel))
				return false;
		}
	}

	void TerrainTexturing::calculateBlockPositionIndices(const UINT8 clipmapLevel, const glm::vec2 cameraPosition) {

		const glm::u16vec2 gridIndex = TerrainTexturing::getGridIndex(clipmapLevel, cameraPosition);
		
		if (TerrainTexturing::getMipmapGridIndex(clipmapLevel) != gridIndex) {

			TerrainTexturing::setMipmapGridIndex(clipmapLevel, gridIndex);
			const glm::u16vec2 startGridIndex = TerrainTexturing::getStartGridIndex(gridIndex);
			TerrainTexturing::setMipmapStartGridIndex(clipmapLevel, startGridIndex);

			bool previousCommon[25];
			bool currentCommon[25];

			for (int i = 0; i < 25; i++) {
				previousCommon[i] = false;
				currentCommon[i] = false;
			}

			glm::u16vec2 currentBlockIndices[25];
			TerrainTexturing::getCurrentBlockProperties(startGridIndex, currentBlockIndices);

			int counter = 25;
			for (int i = 0; i < 25; i++) {

				if (!TerrainTexturing::getIsActive(clipmapLevel, i))
					continue;

				for (int j = 0; j < 25; j++) {
					if (TerrainTexturing::getBlockIndex(clipmapLevel, i) == currentBlockIndices[j]) {
						previousCommon[i] = true;
						currentCommon[j] = true;
						counter--;
					}
				}
			}

			blockIndexToAddQueue[clipmapLevel] = std::queue<BlockJob>();

			for (UINT8 i = 0; i < 25; i++) {
				if (!previousCommon[i] && TerrainTexturing::getIsActive(clipmapLevel, i)) {
					TerrainTexturing::setIsActive(clipmapLevel, i, 0);
					this->emptyStack.push(TerrainTexturing::getPageTableIndex(clipmapLevel, i));
				}
				if (!currentCommon[i] && TerrainTexturing::isInBorders(clipmapLevel, currentBlockIndices[i]))
					blockIndexToAddQueue[clipmapLevel].push(BlockJob(clipmapLevel, currentBlockIndices[i]));
			}
		}
	}

	void TerrainTexturing::rasterBlock(BlockJob blockJob) {

		UINT8 inactiveBlockIndexInArray = -1;
		for (int i = 0; i < 25; i++) {
			if (!TerrainTexturing::getIsActive(blockJob.level, i)) {
				TerrainTexturing::setIsActive(blockJob.level, i, 1);
				inactiveBlockIndexInArray = i;
				break;
			}
		}

		unsigned short topEmpty = this->emptyStack.top();
		this->emptyStack.pop();
		TerrainTexturing::setPageTableIndex(blockJob.level, inactiveBlockIndexInArray, topEmpty);
		TerrainTexturing::setBlockIndex(blockJob.level, inactiveBlockIndexInArray, blockJob.index);

		//auto start = std::chrono::high_resolution_clock::now();

		TerrainTexturing::updatePageTableTexturePartial(topEmpty, blockJob.index, blockJob.level);


		glBindTexture(GL_TEXTURE_2D_ARRAY, physicalPageGeneratorFrame.textureId);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		
		//auto end = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		//int x = 5;
		

		//if (true) {
		//	Texture2D texTemp(physicalPageGeneratorFrame.size.x, physicalPageGeneratorFrame.size.y, 3);
		//	GLTexture::getTextureContent(3, texTemp.data, physicalPageGeneratorFrame.texture_0_Id);
			//texTemp.writeDataToFile("testphysicalpages.png", 0);
		//	std::exit(0);
		//}

		//if (false) {

		//	if (topEmpty == 274) {

		//		//glBindTexture(GL_TEXTURE_2D_ARRAY, physicalPageGeneratorFrame.textureId);

		//		//// Get the size of the texture (width, height, layers, format, type, etc.)
		//		//GLint width, height, layers;
		//		//glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &width);
		//		//glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &height);
		//		//glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_DEPTH, &layers);

		//		//// Allocate memory to store the texture data
		//		//// Assume it's RGBA format and unsigned byte for simplicity. Adjust format/type based on your actual texture.
		//		//GLubyte* data = new GLubyte[width * height * layers * 3];  // 4 for RGBA

		//		//// Retrieve the texture data
		//		//glGetTexImage(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//		//stbi_write_png("testphysicalpages_.png", width, height, 3, &data[topEmpty * width * height * 3], width * 3);

		//		//std::exit(0);

		//		// Create and bind a framebuffer
		//		GLuint fbo;
		//		glGenFramebuffers(1, &fbo);
		//		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//		// Attach a specific layer of the texture array to the framebuffer
		//		int layerIndex = topEmpty;  // Specify which layer you want to read
		//		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, physicalPageGeneratorFrame.textureId, 0, layerIndex);

		//		// Check if framebuffer is complete
		//		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		//			std::cerr << "Framebuffer is not complete!" << std::endl;
		//		}

		//		// Get the size of the texture (width, height)
		//		GLint width, height;
		//		glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &width);
		//		glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &height);

		//		// Allocate memory to read back the layer data
		//		GLubyte* layerData = new GLubyte[width * height * 3];  // 3 for RGB

		//		// Read the data from the framebuffer
		//		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, layerData);

		//		stbi_write_png("testphysicalpages_.png", width, height, 3, layerData, width * 3);
		//		// Now, 'layerData' contains the data of the specified layer.
		//		// You can process or save it.

		//		// Clean up
		//		delete[] layerData;
		//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//		glDeleteFramebuffers(1, &fbo);

		//		std::exit(0);

		//	}

		//	
		//}

	}


	void TerrainTexturing::updatePageTableTexturePartial(unsigned short pageIndex, glm::u16vec2 blockIndex, UINT8 level) {

		UINT8 pageX = (level % pageTableGeneratorFrame.mipCounts.x) * 5;
		UINT8 pageY = (level / pageTableGeneratorFrame.mipCounts.y) * 5;
		UINT8 pageLocalX = blockIndex.x % 5;
		UINT8 pageLocalY = blockIndex.y % 5;

		pageTableGeneratorFrame.setViewport(glm::u8vec2(pageX + pageLocalX, pageY + pageLocalY), glm::ivec2(1));
		pageTableGeneratorFrame.setUniforms(pageIndex, level);
		pageTableGeneratorFrame.draw();

		physicalPageGeneratorFrame.setUniforms(level, blockIndex);
		physicalPageGeneratorFrame.draw(pageIndex, level);
	}

	void TerrainTexturing::getCurrentBlockProperties(const glm::u16vec2 startGridIndex, glm::u16vec2* currentBlockIndices) {

		for (int y = 0; y < 5; y++)
			for (int x = 0; x < 5; x++)
				currentBlockIndices[y * 5 + x] = glm::u16vec2(startGridIndex.x + x, startGridIndex.y + y);
	}

	//UINT8 TerrainTexturing::getMinMipmapLevel(const glm::vec2 cameraPosition, float elevation) {

	//	float terrainElevation = heightmapGenerator->getHeight(cameraPosition);
	//	elevation -= terrainElevation;

	//	if (elevation <= 0)
	//		return 0;

	//	float margin = 0.1;
	//	float baseDistance = 4 * (1 - margin);
	//	unsigned short distancePrev = 0;

	//	for (int i = 0;; i++) {
	//		unsigned short size = baseDistance * (1 << i);
	//		if (elevation > distancePrev && elevation < size)
	//			return i;
	//		distancePrev = size;
	//	}
	//}

	bool TerrainTexturing::isInBorders(const UINT8 level, const glm::u16vec2 gridIndex) { // Causes PROBLEM

		return true;

		unsigned short max = TerrainTexturing::getTerrainMaxBlockIndex(level);
		if (gridIndex.x >= 0 && gridIndex.y >= 0 && gridIndex.x < max && gridIndex.y < max)
			return true;
		return false;
	}

	unsigned short TerrainTexturing::getTerrainMaxBlockIndex(const UINT8 level) {
		return terrainSize / (1 << level);
	}

	glm::u16vec2 TerrainTexturing::getGridIndex(const UINT8 level, const glm::vec2 camPos) {
		return glm::u16vec2((camPos + glm::vec2(0.5 * (1 << level))) / glm::vec2(1 << level)); // AAA     + glm::vec2(0.5 * (1 << level))
	}

	glm::u16vec2 TerrainTexturing::getStartGridIndex(const glm::u16vec2 gridIndex) {
		return gridIndex - glm::u16vec2(2);
	}

	void TerrainTexturing::setMipmapStartGridIndex(const UINT8 clipmapLevel, const glm::u16vec2 index) {
		mipmapStartGridIndexList[clipmapLevel] = index;
	}

	void TerrainTexturing::setMipmapGridIndex(const UINT8 clipmapLevel, const glm::u16vec2 index) {
		mipmapGridIndexList[clipmapLevel] = index;
	}

	glm::u16vec2& TerrainTexturing::getMipmapStartGridIndex(const UINT8 clipmapLevel) {
		return mipmapStartGridIndexList[clipmapLevel];
	}

	glm::u16vec2& TerrainTexturing::getMipmapGridIndex(const UINT8 clipmapLevel) {
		return mipmapGridIndexList[clipmapLevel];
	}

	void TerrainTexturing::setBlockIndex(const UINT8 clipmapLevel, const UINT8 blockIndex, const glm::u16vec2 index) {
		unsigned short i = clipmapLevel * 25 + blockIndex;
		blockIndexList[i] = index;
	}

	glm::u16vec2& TerrainTexturing::getBlockIndex(const UINT8 clipmapLevel, const UINT8 blockIndex) {
		unsigned short i = clipmapLevel * 25 + blockIndex;
		return blockIndexList[i];
	}

	void TerrainTexturing::setPageTableIndex(const UINT8 clipmapLevel, const UINT8 blockIndex, const unsigned short index) {
		unsigned short i = clipmapLevel * 25 + blockIndex;
		pageTableIndexList[i] = index;
	}

	unsigned short& TerrainTexturing::getPageTableIndex(const UINT8 clipmapLevel, const UINT8 blockIndex) {
		unsigned short i = clipmapLevel * 25 + blockIndex;
		return pageTableIndexList[i];
	}

	void TerrainTexturing::setIsActive(const UINT8 clipmapLevel, const UINT8 blockIndex, const UINT8 isActive) {
		unsigned short i = clipmapLevel * 25 + blockIndex;
		isActiveList[i] = isActive;
	}

	UINT8& TerrainTexturing::getIsActive(const UINT8 clipmapLevel, const UINT8 blockIndex) {
		unsigned short i = clipmapLevel * 25 + blockIndex;
		return isActiveList[i];
	}
}