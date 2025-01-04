#pragma once

#include "GL/glew.h"
#include "glm.hpp"

namespace Engine {

    class GLUniform {
    private:
    public:

		static void setMatrix4f(unsigned int shaderProgramId, std::string location, glm::mat4& value);
		static void setFloat3(unsigned int shaderProgramId, std::string location, glm::vec3 value);
		static void setFloat2(unsigned int shaderProgramId, std::string location, glm::vec2 value);
		static void setFloat1(unsigned int shaderProgramId, std::string location, float value);
		static void setInt1(unsigned int shaderProgramId, std::string location, int value);
		static void setInt2(unsigned int shaderProgramId, std::string location, glm::ivec2 value);
		static void setUInt1(unsigned int shaderProgramId, std::string location, unsigned int value);
		static void setUInt2(unsigned int shaderProgramId, std::string location, glm::uvec2 value);
    };
}