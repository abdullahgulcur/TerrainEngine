#include "pch.h"
#include "terrain_render.h"
#include "core.h"
#include "gl_context.h"

namespace Engine {

	TerrainRender::TerrainRender() {}

	TerrainRender::~TerrainRender() {}

	void TerrainRender::init(std::string path, unsigned short blockSize, UINT8 clipmapLevels) {

		shaderProgramId = Core::getShader()->shaders[ShaderType::TERRAIN];
		clipmapTracker.init(path, blockSize, clipmapLevels);
		terrainGeometry.generateTerrainGeometryVertexArrays(clipmapTracker.heightmapData.blockSize, clipmapTracker.heightmapData.mipStartIndices.size(), BLOCK_COUNT_PER_LEVEL * clipmapTracker.heightmapData.mipStartIndices.size() + BLOCK_COUNT_INNER);
	}

	void TerrainRender::update() {

		struct TerrainVertexAttribs {
			glm::vec2 position;
			float level;
			TerrainVertexAttribs() {}
			TerrainVertexAttribs(glm::vec2 position, float level) : position(position), level(level) {}
		};

		Camera* camera = Core::getCamera();
		glm::mat4 PV = camera->projectionMatrix * camera->viewMatrix;
		glm::vec3 camPos = camera->position;

		GLCommand::refreshScene(Core::getGlfwContext()->getScreenSize(), 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		GLShader::useProgram(shaderProgramId);
		GLUniform::setMatrix4f(shaderProgramId, "projectionView", PV);
		GLUniform::setFloat3(shaderProgramId, "cameraPosition", camPos);
		GLUniform::setFloat2(shaderProgramId, "physicalTextureSize", glm::vec2(clipmapTracker.physicalTextureSize));
		GLUniform::setInt1(shaderProgramId, "blockSize", clipmapTracker.heightmapData.blockSize);
		GLUniform::setInt2(shaderProgramId, "terrainSize", clipmapTracker.heightmapData.terrainSize);

		GLTexture::useTexture(0, clipmapTracker.pageTableGeneratorFrame.getTextureId());
		GLTexture::useTexture(1, clipmapTracker.heightmapTextureId);
		GLTexture::useTexture(2, clipmapTracker.physicalPageGeneratorFrame.getTextureId());

		std::vector<TerrainVertexAttribs> instanceArray;

		// BLOCKS
		for (int j = 0; j < BLOCK_COUNT_INNER; j++) {
			glm::vec2 gridPos(clipmapTracker.clipmapLevel0.blockIndices[j].x, clipmapTracker.clipmapLevel0.blockIndices[j].y);
			instanceArray.push_back(TerrainVertexAttribs(gridPos * float(clipmapTracker.heightmapData.blockSize), 0));
		}

		for (int i = 0; i < clipmapTracker.heightmapData.mipStartIndices.size() - 1; i++){
			for (int j = 0; j < BLOCK_COUNT_PER_LEVEL; j++) {
				glm::vec2 gridPos(clipmapTracker.clipmaps[i].blockIndices[j].x, clipmapTracker.clipmaps[i].blockIndices[j].y);
				instanceArray.push_back(TerrainVertexAttribs(gridPos * float(clipmapTracker.heightmapData.blockSize), i));
			}
		}

		if (instanceArray.size()) {
			GLCommand::drawInstancedWithSubData(terrainGeometry.block.VAO, terrainGeometry.block.indiceCount, instanceArray.size(), terrainGeometry.block.instanceBuffer, 0, instanceArray.size() * sizeof(TerrainVertexAttribs), &instanceArray[0]);
			instanceArray.clear();
		}
		instanceArray.clear();

		// OUTER DEGENERATE
		for (int i = 0; i < clipmapTracker.heightmapData.mipStartIndices.size() - 2; i++) {
			glm::vec2 gridPos(clipmapTracker.clipmaps[i].startGridIndex.x, clipmapTracker.clipmaps[i].startGridIndex.y);
			instanceArray.push_back(TerrainVertexAttribs(gridPos * float(clipmapTracker.heightmapData.blockSize), i));
		}

		if (instanceArray.size()) {
			GLCommand::drawInstancedWithSubData(terrainGeometry.outerDegenerate.VAO, terrainGeometry.outerDegenerate.indiceCount, instanceArray.size(), terrainGeometry.outerDegenerate.instanceBuffer, 0, instanceArray.size() * sizeof(TerrainVertexAttribs), &instanceArray[0]);
			instanceArray.clear();
		}
	}
}