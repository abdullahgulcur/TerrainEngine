#include "pch.h"
#include "terrain_render.h"
#include "core.h"
#include "gl_context.h"
#include "terrain.h"

namespace Engine {

	void TerrainRender::draw(Terrain* terrain) {

		struct TerrainVertexAttribs {
			glm::u16vec2 position;
			UINT8 level;
			TerrainVertexAttribs() {}
			TerrainVertexAttribs(glm::u16vec2 position, UINT8 level) : position(position), level(level) {}
		};

		Camera* camera = Core::getCamera();
		unsigned int shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN_GEOMETRY];
		TerrainGeometry& terrainGeometry = terrain->terrainGeometry;

		GLShader::useProgram(shaderProgramId);
		GLUniform::setMatrix4f(shaderProgramId, "projectionView", camera->projectionMatrix* camera->viewMatrix);
		GLUniform::setFloat3(shaderProgramId, "cameraPosition", camera->position);
		GLUniform::setInt1(shaderProgramId, "blockSize", terrain->terrainGeometryManager.blockSize);
		//GLUniform::setUInt1(shaderProgramId, "currentMipmapIndexForShader", terrain->terrainTexturing.currentMipmapIndexForShader);
		GLUniform::setUInt1(shaderProgramId, "totalMipmapLevel", terrain->terrainTexturing.totalMipmapLevel);
		GLUniform::setUInt2(shaderProgramId, "mipCounts", terrain->terrainTexturing.pageTableGeneratorFrame.mipCounts);
		//GLUniform::setUInt2(shaderProgramId, "pageCounts", terrain->terrainTexturing.physicalPageGeneratorFrame.pageCounts);

		GLUniform::setIntArray(shaderProgramId, "mipmapLevelList", &terrain->terrainTexturing.availableMipmapLevelList[0], 16);

		GLTexture::useTexture(0, terrain->terrainTexturing.heightmapGenerator->textureId);
		GLTexture::useTexture(1, terrain->terrainTexturing.pageTableGeneratorFrame.getTextureId());
		GLTexture::useTextureArray(2, terrain->terrainTexturing.physicalPageGeneratorFrame.textureId);
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		std::vector<TerrainVertexAttribs> instanceArray;

		// BLOCKS
		for (int i = terrain->terrainGeometryManager.startClipmapLevel; i < terrain->terrainGeometryManager.totalClipmapLevel; i++) {
			for (int j = 0; j < BLOCK_COUNT_PER_LEVEL; j++) {
				if((!terrain->terrainGeometryManager.getIsInner(i, j) || i == terrain->terrainGeometryManager.startClipmapLevel) &&
					!terrain->terrainGeometryManager.getOutOfBorder(i, j))
					instanceArray.push_back(TerrainVertexAttribs(terrain->terrainGeometryManager.getBlockIndexWorldSpace(i, j), i));
			}
		}
		
		if (instanceArray.size()) {
			GLCommand::drawInstancedWithSubData1(terrainGeometry.block.VAO, terrainGeometry.block.indiceCount, instanceArray.size(), terrainGeometry.block.instanceBuffer, 0, instanceArray.size() * sizeof(TerrainVertexAttribs), &instanceArray[0]);
			instanceArray.clear();
		}
		instanceArray.clear();

		// OUTER DEGENERATE
		for (int i = terrain->terrainGeometryManager.startClipmapLevel; i < terrain->terrainGeometryManager.totalClipmapLevel - 1; i++)
			instanceArray.push_back(TerrainVertexAttribs(terrain->terrainGeometryManager.getOuterDegenerateIndexWorldSpace(i), i));

		if (instanceArray.size()) {
			GLCommand::drawInstancedWithSubData1(terrainGeometry.outerDegenerate.VAO, terrainGeometry.outerDegenerate.indiceCount, instanceArray.size(), terrainGeometry.outerDegenerate.instanceBuffer, 0, instanceArray.size() * sizeof(TerrainVertexAttribs), &instanceArray[0]);
			instanceArray.clear();
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}