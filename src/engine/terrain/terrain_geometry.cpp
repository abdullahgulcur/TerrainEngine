#include "pch.h"
#include "terrain_geometry.h"
#include "gl_buffer.h"

namespace Engine {

	TerrainGeometry::TerrainGeometry() {}

	TerrainGeometry::~TerrainGeometry() {}

	/*
	* Creates all the vao for the meshes of the terrain
	*/
	void TerrainGeometry::generateTerrainGeometryVertexArrays(const int blockResolution, const int totalLevel, const int totalInstance) {

		std::vector<unsigned short> blockIndices;
		std::vector<unsigned short> outerDegenerateIndices;

		std::vector<glm::u16vec2> blockVerts;
		std::vector<glm::u16vec2> outerDegenerateVerts;

		for (int i = 0; i <= blockResolution; i++)
			for (int j = 0; j <= blockResolution; j++)
				blockVerts.push_back(glm::u16vec2(j, i));

		for (int i = 0; i < blockResolution; i++) {
			for (int j = 0; j < blockResolution; j++) {

				blockIndices.push_back(j + i * (blockResolution + 1));
				blockIndices.push_back(j + (i + 1) * (blockResolution + 1));
				blockIndices.push_back(j + i * (blockResolution + 1) + 1);

				blockIndices.push_back(j + i * (blockResolution + 1) + 1);
				blockIndices.push_back(j + (i + 1) * (blockResolution + 1));
				blockIndices.push_back(j + (i + 1) * (blockResolution + 1) + 1);
			}
		}


		// outer degenerate
		for (int i = 0; i < blockResolution * 6; i++)
			outerDegenerateVerts.push_back(glm::u16vec2(i, 0));
		for (int i = 0; i < blockResolution * 6; i++)
			outerDegenerateVerts.push_back(glm::u16vec2(blockResolution * 6, i));
		for (int i = 0; i < blockResolution * 6; i++)
			outerDegenerateVerts.push_back(glm::u16vec2(blockResolution * 6 - i, blockResolution * 6));
		for (int i = 0; i < blockResolution * 6; i++)
			outerDegenerateVerts.push_back(glm::u16vec2(0, blockResolution * 6 - i));

		int triCount = blockResolution * 12;
		for (int i = 0; i < triCount; i++) {
			int index0 = i * 2;
			int index1 = ((i + 1) % triCount) * 2;
			int index2 = i * 2 + 1;

			outerDegenerateIndices.push_back(index0);
			outerDegenerateIndices.push_back(index1);
			outerDegenerateIndices.push_back(index2);

			outerDegenerateIndices.push_back(index0);
			outerDegenerateIndices.push_back(index2);
			outerDegenerateIndices.push_back(index1);
		}

		block.indiceCount = blockIndices.size();
		outerDegenerate.indiceCount = outerDegenerateIndices.size();

		GLBuffer::createInstancedTerrainStaticMesh2D(block.VAO, block.instanceBuffer, &blockVerts[0], blockVerts.size() * sizeof(glm::u16vec2), &blockIndices[0], blockIndices.size() * sizeof(unsigned short), totalInstance);
		GLBuffer::createInstancedTerrainStaticMesh2D(outerDegenerate.VAO, outerDegenerate.instanceBuffer, &outerDegenerateVerts[0], outerDegenerateVerts.size() * sizeof(glm::u16vec2), &outerDegenerateIndices[0], outerDegenerateIndices.size() * sizeof(unsigned short), totalInstance);
		// 
		//std::vector<unsigned short> blockIndices;
		//std::vector<unsigned short> outerDegenerateIndices;

		//std::vector<glm::u8vec2> blockVerts;
		//std::vector<glm::u8vec2> outerDegenerateVerts;

		//int blockResolution = Resolution + 0;

		//for (int i = 0; i < blockResolution; i++)
		//	for (int j = 0; j < blockResolution; j++)
		//		blockVerts.push_back(glm::u8vec2(j, i));

		//for (int i = 0; i <= blockResolution / 2; i++)
		//	blockVerts.push_back(glm::u8vec2(i * 2, blockResolution));

		//for (int i = 0; i <= blockResolution / 2; i++)
		//	blockVerts.push_back(glm::u8vec2(blockResolution, i * 2));

		//for (int i = 0; i < blockResolution - 1; i++) {
		//	for (int j = 0; j < blockResolution - 1; j++) {

		//		blockIndices.push_back(j + i * blockResolution);
		//		blockIndices.push_back(j + (i + 1) * blockResolution);
		//		blockIndices.push_back(j + i * blockResolution + 1);

		//		blockIndices.push_back(j + i * blockResolution + 1);
		//		blockIndices.push_back(j + (i + 1) * blockResolution);
		//		blockIndices.push_back(j + (i + 1) * blockResolution + 1);
		//	}
		//}

		//// horizontal, up
		//int ptStartIndex0_0 = blockResolution * (blockResolution - 1);
		//int ptStartIndex0_1 = blockResolution * blockResolution;

		//for (int i = 0; i < blockResolution / 2; i++) {
		//	blockIndices.push_back(ptStartIndex0_0 + i * 2);
		//	blockIndices.push_back(ptStartIndex0_1 + i);
		//	blockIndices.push_back(ptStartIndex0_0 + i * 2 + 1);

		//	blockIndices.push_back(ptStartIndex0_0 + i * 2 + 1);
		//	blockIndices.push_back(ptStartIndex0_1 + i);
		//	blockIndices.push_back(ptStartIndex0_1 + i + 1);
		//}

		//for (int i = 1; i < blockResolution / 2; i++) {
		//	blockIndices.push_back(ptStartIndex0_0 + i * 2);
		//	blockIndices.push_back(ptStartIndex0_0 + i * 2 - 1);
		//	blockIndices.push_back(ptStartIndex0_1 + i);
		//}

		//// vertical, left
		//ptStartIndex0_1 = blockResolution * blockResolution + (blockResolution / 2) + 1;

		//// blockres - 1 + 2 i blockres + blockres

		//for (int i = 0; i < blockResolution / 2; i++) {
		//	int index0 = blockResolution * (i * 2 + 1) - 1;
		//	int index1 = blockResolution * (i * 2 + 2) - 1;
		//	blockIndices.push_back(index0);
		//	blockIndices.push_back(index1);
		//	blockIndices.push_back(ptStartIndex0_1 + i);

		//	blockIndices.push_back(ptStartIndex0_1 + i);
		//	blockIndices.push_back(index1);
		//	blockIndices.push_back(ptStartIndex0_1 + i + 1);


		//	/*blockIndices.push_back(ptStartIndex0_0 + i * 2 + 1);
		//	blockIndices.push_back(ptStartIndex0_1 + i);
		//	blockIndices.push_back(ptStartIndex0_1 + i + 1);*/
		//}

		//for (int i = 1; i < blockResolution / 2; i++) {
		//	int index0 = blockResolution * (i * 2 + 1) - 1;
		//	int index1 = blockResolution * (i * 2) - 1;
		//	blockIndices.push_back(index0);
		//	blockIndices.push_back(ptStartIndex0_1 + i);
		//	blockIndices.push_back(index1);

		//}



		//// outer degenerate
		//for (int i = 0; i < blockResolution * 6; i++)
		//	outerDegenerateVerts.push_back(glm::u8vec2(i, 0));
		//for (int i = 0; i < blockResolution * 6; i++)
		//	outerDegenerateVerts.push_back(glm::u8vec2(blockResolution * 6, i));
		//for (int i = 0; i < blockResolution * 6; i++)
		//	outerDegenerateVerts.push_back(glm::u8vec2(blockResolution * 6 - i, blockResolution * 6));
		//for (int i = 0; i < blockResolution * 6; i++)
		//	outerDegenerateVerts.push_back(glm::u8vec2(0, blockResolution * 6 - i));

		//int triCount = blockResolution * 12;
		//for (int i = 0; i < triCount; i++) {
		//	int index0 = i * 2;
		//	int index1 = ((i + 1) % triCount) * 2;
		//	int index2 = i * 2 + 1;

		//	outerDegenerateIndices.push_back(index0);
		//	outerDegenerateIndices.push_back(index1);
		//	outerDegenerateIndices.push_back(index2);

		//	outerDegenerateIndices.push_back(index0);
		//	outerDegenerateIndices.push_back(index2);
		//	outerDegenerateIndices.push_back(index1);
		//}
		//
		//block.indiceCount = blockIndices.size();
		//outerDegenerate.indiceCount = outerDegenerateIndices.size();

		//GLBuffer::createInstancedTerrainStaticMesh2D(block.VAO, block.instanceBuffer, &blockVerts[0], blockVerts.size() * sizeof(glm::u8vec2), &blockIndices[0], blockIndices.size() * sizeof(unsigned short), totalInstance);
		//GLBuffer::createInstancedTerrainStaticMesh2D(outerDegenerate.VAO, outerDegenerate.instanceBuffer, &outerDegenerateVerts[0], outerDegenerateVerts.size() * sizeof(glm::u8vec2), &outerDegenerateIndices[0], outerDegenerateIndices.size() * sizeof(unsigned short), totalInstance);
	}

}