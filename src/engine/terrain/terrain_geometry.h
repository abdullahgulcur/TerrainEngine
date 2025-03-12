#pragma once

#include "glm.hpp"
#include "rendering.h"

namespace Engine {

	class TerrainGeometry {

	private:

	public:

		InstancedStaticMesh block;
		InstancedStaticMesh outerDegenerate;

		TerrainGeometry();
		~TerrainGeometry();
		void generateTerrainGeometryVertexArrays(const int blockResolution, const int totalLevel);
	};
}