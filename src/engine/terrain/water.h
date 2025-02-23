#pragma once

#include "rendering.h"

namespace Engine {

	class Water {


	private:

	public:

		unsigned int quadVAO;
		unsigned int waterShaderProgramId;

		//StaticMesh2D waterMesh;

		Water() {}
		~Water() {}

		void init();
		void update();
		
	};
}