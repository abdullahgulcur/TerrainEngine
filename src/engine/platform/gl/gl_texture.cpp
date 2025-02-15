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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		float maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		glGenerateMipmap(GL_TEXTURE_2D);

		return textureId;
    }

	unsigned int GLTexture::generateTerrainPaletteTexture2D(unsigned short width, unsigned short height, const unsigned char* data) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		return textureId;
	}

	unsigned int GLTexture::generateCompressedTerrainPaletteTexture2D(std::string imagepath) {


		//unsigned char header[124];

		//FILE* fp;

		///* try to open the file */
		//fp = fopen(imagepath.c_str(), "rb");
		//if (fp == NULL)
		//	return 0;

		///* verify the type of file */
		//char filecode[4];
		//fread(filecode, 1, 4, fp);
		//if (strncmp(filecode, "DDS ", 4) != 0) {
		//	fclose(fp);
		//	return 0;
		//}

		///* get the surface desc */
		//fread(&header, 124, 1, fp);

		//unsigned int height = *(unsigned int*)&(header[8]);
		//unsigned int width = *(unsigned int*)&(header[12]);
		//unsigned int linearSize = *(unsigned int*)&(header[16]);
		//unsigned int mipMapCount = *(unsigned int*)&(header[24]);
		//unsigned int fourCC = *(unsigned int*)&(header[80]);

		//unsigned char* buffer;
		//unsigned int bufsize;
		///* how big is it going to be including all mipmaps? */
		//bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		//buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
		//fread(buffer, 1, bufsize, fp);
		///* close the file pointer */
		//fclose(fp);

		//unsigned int components = 3;

		//// Create one OpenGL texture
		//GLuint textureID;
		//glGenTextures(1, &textureID);

		//// "Bind" the newly created texture : all future texture functions will modify this texture
		//glBindTexture(GL_TEXTURE_2D, textureID);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//unsigned int blockSize = 8;
		//unsigned int offset = 0;

		///* load the mipmaps */
		//for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
		//{
		//	unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		//	glCompressedTexImage2D(GL_TEXTURE_2D, level, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, width, height, 0, size, buffer + offset);

		//	offset += size;
		//	width /= 2;
		//	height /= 2;
		//}
		//free(buffer);

		//return textureID;

		// FourCC codes for DXT compression formats
#define FOURCC_DXT1 0x31545844 // "DXT1"
#define FOURCC_DXT3 0x33545844 // "DXT3"
#define FOURCC_DXT5 0x35545844 // "DXT5"

		struct DDSHeader {
			uint32_t size;
			uint32_t flags;
			uint32_t height;
			uint32_t width;
			uint32_t pitchOrLinearSize;
			uint32_t depth;
			uint32_t mipMapCount;
			uint32_t reserved1[11];
			uint32_t pixelFormatSize;
			uint32_t pixelFormatFlags;
			uint32_t fourCC;
			uint32_t rgbBitCount;
			uint32_t rBitMask;
			uint32_t gBitMask;
			uint32_t bBitMask;
			uint32_t aBitMask;
			uint32_t caps;
			uint32_t caps2;
			uint32_t caps3;
			uint32_t caps4;
			uint32_t reserved2;
		};

		std::ifstream file(imagepath.c_str(), std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << imagepath.c_str() << std::endl;
			return 0;
		}

		// Read and validate the magic number
		char magic[4];
		file.read(magic, 4);
		if (strncmp(magic, "DDS ", 4) != 0) {
			std::cerr << "Not a valid DDS file: " << imagepath.c_str() << std::endl;
			return 0;
		}

		// Read the DDS header
		DDSHeader header;
		file.read(reinterpret_cast<char*>(&header), sizeof(DDSHeader));

		// Determine the format based on the fourCC code
		GLenum format;
		switch (header.fourCC) {
		case FOURCC_DXT1: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
		case FOURCC_DXT3: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
		case FOURCC_DXT5: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
		default:
			std::cerr << "Unsupported DDS format: " << header.fourCC << std::endl;
			return 0;
		}

		// Calculate the size of the texture data
		size_t dataSize = header.mipMapCount > 1
			? header.pitchOrLinearSize * 2
			: header.pitchOrLinearSize;

		// Read texture data
		std::vector<char> textureData(dataSize);
		file.read(textureData.data(), dataSize);
		file.close();

		// Create the OpenGL texture
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Load the texture data for each mipmap level
		uint32_t width = header.width;
		uint32_t height = header.height;
		size_t offset = 0;

		for (uint32_t level = 0; level < header.mipMapCount && (width || height); ++level) {
			size_t blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			size_t size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, textureData.data() + offset);
			offset += size;

			width = std::max(1u, width / 2);
			height = std::max(1u, height / 2);
		}

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureID;
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

		//unsigned char* data = new unsigned char[size.x * size.y * 3];
		//for (int i = 0; i < size.x * size.y * 3; i++) {
		//	data[i] = 255;
		//}

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		float maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); // Minimum mipmap level
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);  // Maximum mipmap level

		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f);

		return textureId;
	}

	unsigned int GLTexture::createPhysicalPagesFrameBufferTextureArray(unsigned short pageSize, UINT8 mipLevels, unsigned short pageCount) {

		unsigned int textureArray;
		glGenTextures(1, &textureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevels, GL_RGBA8, pageSize, pageSize, pageCount);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);

		float maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		return textureArray;
		//unsigned char* data = new unsigned char[size.x * size.y * 3];
		//for (int i = 0; i < size.x * size.y * 3; i++) {
		//	data[i] = 255;
		//}

		//unsigned int textureId;
		//glGenTextures(1, &textureId);
		//glBindTexture(GL_TEXTURE_2D, textureId);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//float maxAniso = 0.0f;
		//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); // Minimum mipmap level
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);  // Maximum mipmap level

		////glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f);

		//return textureId;
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

	void GLTexture::useTextureArray(unsigned int index, unsigned int textureId) {

		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
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

