#include "pch.h"
#include "gl_texture.h"

namespace Engine {

    unsigned int GLTexture::generateTexture2D(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data) {

		GLenum internalFormats[4] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
		GLenum formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

		GLenum internalFormat = internalFormats[channels-1];
		GLenum format = formats[channels - 1];
		
		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		float maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		glGenerateMipmap(GL_TEXTURE_2D);

		return textureId;
    }

	unsigned int GLTexture::generateMaskTexture(unsigned short width, unsigned short height) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return textureId;
	}

	unsigned int GLTexture::generateDepthTexture(unsigned short width, unsigned short height) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		return textureId;
	}

	unsigned int GLTexture::generateHeightmapTexture2D(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data) {

		GLenum internalFormats[4] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
		GLenum formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

		GLenum internalFormat = internalFormats[channels - 1];
		GLenum format = formats[channels - 1];

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureId;
	}

	unsigned int GLTexture::generateHeightmapTexture2D(glm::u16vec2 size) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, size.x, size.y, 0, GL_RG, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureId;
	}

	unsigned int GLTexture::generateHeightmapTexture(glm::u16vec2 size) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, size.x, size.y, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureId;
	}

	unsigned int GLTexture::generatePhysicalPagesTexture(UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data) {

		GLenum internalFormats[4] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
		GLenum formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

		GLenum internalFormat = internalFormats[channels - 1];
		GLenum format = formats[channels - 1];

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return textureId;
	}

	unsigned int GLTexture::createPhysicalPagesFrameBufferTexture(glm::u16vec2 size) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		return textureId;
	}

	unsigned int GLTexture::createPageTableFrameBufferTexture(glm::u16vec2 size) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, size.x, size.y, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		return textureId;
	}

	unsigned int GLTexture::createFrameSceneTexture(glm::u16vec2 size) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		return textureId;
	}

	void GLTexture::updateTexture2D(unsigned int textureId, UINT8 channels, unsigned short width, unsigned short height, const unsigned char* data, glm::ivec2 pos) {

		GLenum formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
		GLenum format = formats[channels-1];

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, pos.x, pos.y, width, height, format, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture::updateHeightmapPhysicalTexture(unsigned int textureId, unsigned short width, unsigned short height, const unsigned int* data, glm::ivec2 pos) {

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, pos.x, pos.y, width, height, GL_RED_INTEGER, GL_UNSIGNED_INT, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture::useTexture(unsigned int index, unsigned int textureId) {

		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	void GLTexture::setTextureUniformLocation(std::string location, unsigned int shaderProgramId, unsigned int index) {
		glUseProgram(shaderProgramId);
		glUniform1i(glGetUniformLocation(shaderProgramId, location.c_str()), index);
	}

	void GLTexture::getTextureContent(UINT8 channels, unsigned char* data, unsigned int textureId) {

		GLenum formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
		GLenum format = formats[channels - 1];

		glBindTexture(GL_TEXTURE_2D, textureId);
		glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, data);
	}

	//void GLTexture::getTextureContent(UINT8 channels, unsigned int* data, unsigned int textureId) {

	//	glBindTexture(GL_TEXTURE_2D, textureId);
	//	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, data);
	//}

	void GLTexture::getHeightmapContent(unsigned int* data, unsigned int textureId) {

		glBindTexture(GL_TEXTURE_2D, textureId);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, data);
	}

	void GLTexture::deleteTexture(unsigned int textureId) {

		glDeleteTextures(1, &textureId);
	}

	void GLTexture::generateMipmap(unsigned int textureId) {

		glBindTexture(GL_TEXTURE_2D, textureId);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

}

