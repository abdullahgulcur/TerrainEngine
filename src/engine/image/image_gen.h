#pragma once

#include "glm.hpp"

namespace Engine {

	class ImageGenerator {

	private:

	public:

		ImageGenerator() {}
		~ImageGenerator() {}
		unsigned int generatePerlinNoiseTexture(glm::u16vec2 size, float scale);

	};
}