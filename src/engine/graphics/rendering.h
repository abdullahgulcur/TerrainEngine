#pragma once

#include "glm.hpp"

namespace Engine {

	//struct Texture2D {
	//	unsigned int textureId;
	//};

	/*struct TextureData2D {
		unsigned short width;
		unsigned short height;
		UINT8 channels;
		unsigned char* data;
	};*/

	struct TextureCubemap {
		unsigned int environmentCubemap;
		unsigned int irradianceMap;
		unsigned int prefilterMap;
		unsigned int brdfLUTTexture;
	};

	struct AABB_Box {
		glm::vec3 start;
		glm::vec3 end;
	};

	struct StaticMesh {
		unsigned int VAO;
		unsigned int indiceCount;
		AABB_Box aabbBox;
	};

	struct StaticMesh2D {

		unsigned int VAO;
		unsigned int indiceCount;

		StaticMesh2D() {}
		StaticMesh2D(unsigned int VAO, unsigned int indiceCount) : VAO(VAO), indiceCount(indiceCount) {}
	};

	struct RenderFrame {
		unsigned int planeVAO;
		unsigned int FBO;
		unsigned int textureId;
		//unsigned int shaderProgramId;
		glm::u16vec2 size;
	};

	struct TerrainVertexAttribs {

		glm::vec2 position;
		float level;
		glm::vec3 color;

		TerrainVertexAttribs() {}
		TerrainVertexAttribs(glm::vec2 position, float level, glm::vec3 color) : 
			position(position), 
			level(level),
			color(color) 
		{
		}
	};

	struct TerrainVertexAttribsNoColor {

		glm::vec2 position;
		float level;

		TerrainVertexAttribsNoColor() {}
		TerrainVertexAttribsNoColor(glm::vec2 position, float level) :
			position(position),
			level(level)
		{
		}
	};

	struct InstancedStaticMesh {

		unsigned int VAO;
		unsigned int indiceCount;
		unsigned int instanceBuffer;

		InstancedStaticMesh() {}
		InstancedStaticMesh(unsigned int VAO, unsigned int indiceCount, unsigned int instanceBuffer) : 
			VAO(VAO), 
			indiceCount(indiceCount), 
			instanceBuffer(instanceBuffer) 
		{
		}
	};

	//struct Material_PBR_Textured {
	//	unsigned int shaderProgramId;
	//	Texture2D textures[5];
	//};
	
	struct Skybox {
		unsigned int VAO;
		unsigned int indiceCount = 36;
		unsigned int shaderProgramId;
		TextureCubemap textureCubemap;
	};
}