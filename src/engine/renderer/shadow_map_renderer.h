#pragma once
#include "glm.hpp"

namespace Engine {

	class Terrain;

	class ShadowmapRenderer {

	private:

	public:

		std::vector<float> shadowCascadeLevels;
		unsigned int lightFBO;
		unsigned int lightDepthMaps;
		unsigned int depthMapResolution = 4096;
		unsigned int matricesUBO;

		ShadowmapRenderer() {}
		~ShadowmapRenderer() {}

		void init();
		void update(Terrain* terrain);
		std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview);
		std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
		glm::mat4 getLightSpaceMatrix(const float nearPlane, const float farPlane);
		std::vector<glm::mat4> getLightSpaceMatrices();
	};
}