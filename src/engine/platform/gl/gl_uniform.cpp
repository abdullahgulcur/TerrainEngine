#include "pch.h"
#include "gl_uniform.h"

namespace Engine {

	void GLUniform::setMatrix4f(unsigned int shaderProgramId, std::string location, glm::mat4& value) {
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, &location[0]), 1, GL_FALSE, &value[0][0]);
	}

	void GLUniform::setFloat3(unsigned int shaderProgramId, std::string location, glm::vec3 value) {
		glUniform3fv(glGetUniformLocation(shaderProgramId, &location[0]), 1, &value[0]);
	}

	void GLUniform::setFloat2(unsigned int shaderProgramId, std::string location, glm::vec2 value) {
		glUniform2fv(glGetUniformLocation(shaderProgramId, &location[0]), 1, &value[0]);
	}

	void GLUniform::setInt1(unsigned int shaderProgramId, std::string location, int value) {
		glUniform1i(glGetUniformLocation(shaderProgramId, &location[0]), value);
	}

	void GLUniform::setInt2(unsigned int shaderProgramId, std::string location, glm::ivec2 value) {
		glUniform2iv(glGetUniformLocation(shaderProgramId, &location[0]), 1, &value[0]);
	}

	void GLUniform::setUInt1(unsigned int shaderProgramId, std::string location, unsigned int value) {
		glUniform1ui(glGetUniformLocation(shaderProgramId, &location[0]), value);
	}
}
