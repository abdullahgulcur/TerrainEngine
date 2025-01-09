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
		unsigned int shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN];
		PageTableManager& clipmapTracker = terrain->pageTableManager;
		TerrainGeometry& terrainGeometry = terrain->terrainGeometry;

		GLShader::useProgram(shaderProgramId);
		GLUniform::setMatrix4f(shaderProgramId, "projectionView", camera->projectionMatrix* camera->viewMatrix);
		GLUniform::setFloat3(shaderProgramId, "cameraPosition", camera->position);
		GLUniform::setFloat2(shaderProgramId, "physicalTexturePageCount", glm::vec2(clipmapTracker.physicalTextureSize));
		GLUniform::setInt1(shaderProgramId, "blockSize", clipmapTracker.heightmapData.blockSize);
		GLUniform::setInt2(shaderProgramId, "terrainSize", clipmapTracker.heightmapData.terrainSize);

		GLTexture::useTexture(0, clipmapTracker.pageTableGeneratorFrame.getTextureId());
		GLTexture::useTexture(1, clipmapTracker.heightmapTextureId);
		GLTexture::useTexture(2, clipmapTracker.physicalPageGeneratorFrame.texture_0_Id);
		/*GLTexture::useTexture(3, clipmapTracker.physicalPageGeneratorFrame.texture_1_Id);
		GLTexture::useTexture(4, clipmapTracker.physicalPageGeneratorFrame.texture_2_Id);*/

		std::vector<TerrainVertexAttribs> instanceArray;

		// BLOCKS
		for (int j = 0; j < BLOCK_COUNT_INNER; j++)
			instanceArray.push_back(TerrainVertexAttribs(clipmapTracker.clipmapLevel0.blockIndices[j], 0));

		for (int i = 0; i < clipmapTracker.heightmapData.mipStartIndices.size(); i++)
			for (int j = 0; j < BLOCK_COUNT_PER_LEVEL; j++)
				instanceArray.push_back(TerrainVertexAttribs(clipmapTracker.clipmaps[i].blockIndices[j], i));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (instanceArray.size()) {
			GLCommand::drawInstancedWithSubData1(terrainGeometry.block.VAO, terrainGeometry.block.indiceCount, instanceArray.size(), terrainGeometry.block.instanceBuffer, 0, instanceArray.size() * sizeof(TerrainVertexAttribs), &instanceArray[0]);
			instanceArray.clear();
		}
		instanceArray.clear();

		// OUTER DEGENERATE
		for (int i = 0; i < clipmapTracker.heightmapData.mipStartIndices.size() - 1; i++)
			instanceArray.push_back(TerrainVertexAttribs(clipmapTracker.clipmaps[i].startGridIndex, i));

		if (instanceArray.size()) {
			GLCommand::drawInstancedWithSubData1(terrainGeometry.outerDegenerate.VAO, terrainGeometry.outerDegenerate.indiceCount, instanceArray.size(), terrainGeometry.outerDegenerate.instanceBuffer, 0, instanceArray.size() * sizeof(TerrainVertexAttribs), &instanceArray[0]);
			instanceArray.clear();
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}