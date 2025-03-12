#include "pch.h"
#include "gl_shader.h"

namespace Engine {

	// TODO: make it in one single function
    unsigned int GLShader::loadShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath) {

		auto shaderCode = [](std::string& path, std::string& shaderCode) {
			std::ifstream shaderStream(path.c_str(), std::ios::in);
			if (shaderStream.is_open()) {
				std::stringstream sstr;
				sstr << shaderStream.rdbuf();
				shaderCode = sstr.str();
				shaderStream.close();
			}
			else
				printf("Cannot open %s !\n", path.c_str());
		};

		auto shaderCompile = [](std::string& path, std::string& shaderCode, unsigned int& shaderId) {
			printf("Compiling shader : %s\n", path.c_str());
			char const* sourcePointer = shaderCode.c_str();
			glShaderSource(shaderId, 1, &sourcePointer, NULL);
			glCompileShader(shaderId);
		};

		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		unsigned int geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

		std::string vertexShaderCode;
		std::string fragmentShaderCode;
		std::string geometryShaderCode;

		shaderCode(vertexPath, vertexShaderCode);
		shaderCode(fragmentPath, fragmentShaderCode);
		shaderCode(geometryPath, geometryShaderCode);

		shaderCompile(vertexPath, vertexShaderCode, vertexShaderID);
		shaderCompile(fragmentPath, fragmentShaderCode, fragmentShaderID);
		shaderCompile(geometryPath, geometryShaderCode, geometryShaderID);

		unsigned int ProgramID = glCreateProgram();
		glAttachShader(ProgramID, vertexShaderID);
		glAttachShader(ProgramID, fragmentShaderID);
		glAttachShader(ProgramID, geometryShaderID);
		glLinkProgram(ProgramID);

		glDetachShader(ProgramID, vertexShaderID);
		glDetachShader(ProgramID, fragmentShaderID);
		glDetachShader(ProgramID, geometryShaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		glDeleteShader(geometryShaderID);

		return ProgramID;
    }

	unsigned int GLShader::loadShader(std::string vertexPath, std::string fragmentPath) {

		auto shaderCode = [](std::string& path, std::string& shaderCode) {
			std::ifstream shaderStream(path.c_str(), std::ios::in);
			if (shaderStream.is_open()) {
				std::stringstream sstr;
				sstr << shaderStream.rdbuf();
				shaderCode = sstr.str();
				shaderStream.close();
			}
			else
				printf("Cannot open %s !\n", path.c_str());
		};

		auto shaderCompile = [](std::string& path, std::string& shaderCode, unsigned int& shaderId) {
			printf("Compiling shader : %s\n", path.c_str());
			char const* sourcePointer = shaderCode.c_str();
			glShaderSource(shaderId, 1, &sourcePointer, NULL);
			glCompileShader(shaderId);
		};

		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		std::string vertexShaderCode;
		std::string fragmentShaderCode;

		shaderCode(vertexPath, vertexShaderCode);
		shaderCode(fragmentPath, fragmentShaderCode);

		shaderCompile(vertexPath, vertexShaderCode, vertexShaderID);
		shaderCompile(fragmentPath, fragmentShaderCode, fragmentShaderID);

		unsigned int ProgramID = glCreateProgram();
		glAttachShader(ProgramID, vertexShaderID);
		glAttachShader(ProgramID, fragmentShaderID);
		glLinkProgram(ProgramID);

		glDetachShader(ProgramID, vertexShaderID);
		glDetachShader(ProgramID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return ProgramID;
	}

	void GLShader::useProgram(unsigned int programId) {
		glUseProgram(programId);
	}

	void GLShader::deleteProgram(unsigned int programId) {
		glDeleteProgram(programId);
	}

}