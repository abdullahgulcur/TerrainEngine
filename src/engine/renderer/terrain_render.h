#pragma once

namespace Engine {

	class Terrain;

	class TerrainRender {

	private:

	public:

		TerrainRender() {};
		~TerrainRender() {};
		void draw(Terrain* terrain);
	};
}