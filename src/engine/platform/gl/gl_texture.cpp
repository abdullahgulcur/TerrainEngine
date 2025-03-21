#include "pch.h"
#include "gl_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define FOURCC_DXT1 0x31545844 // "DXT1"
#define FOURCC_DXT3 0x33545844 // "DXT3"
#define FOURCC_DXT5 0x35545844 // "DXT5"

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

	unsigned int GLTexture::generateMacroVariationTexture(std::string path) {

		int width;
		int height;
		int channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		return textureId;
	}

	unsigned int GLTexture::generateCompressedTerrainPaletteTextureArray(std::vector<std::string>& texturePathList, unsigned short textureSize) {

		unsigned int textureArray;
		glGenTextures(1, &textureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

		UINT8 totalMipLevels = glm::log2(float(textureSize));
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, totalMipLevels, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, textureSize, textureSize, texturePathList.size());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		for (int i = 0; i < texturePathList.size(); i++)
			GLTexture::compressedTextureSubImage(texturePathList[i], i);

		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

		return textureArray;
	}

	void GLTexture::compressedTextureSubImage(std::string& texturePath, UINT8 index) {

		std::ifstream file(texturePath.c_str(), std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << texturePath.c_str() << std::endl;
			return;
		}

		// Read and validate the magic number
		char magic[4];
		file.read(magic, 4);
		if (strncmp(magic, "DDS ", 4) != 0) {
			std::cerr << "Not a valid DDS file: " << texturePath.c_str() << std::endl;
			return;
		}

		// Read the DDS header
		DDSHeader header;
		file.read(reinterpret_cast<char*>(&header), sizeof(DDSHeader));

		// Determine the format based on the fourCC code
		GLenum format;
		switch (header.fourCC) {
		case FOURCC_DXT1: 
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
			break;
		case FOURCC_DXT3: 
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
			break;
		case FOURCC_DXT5: 
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
			break;
		default:
			std::cerr << "Unsupported DDS format: " << header.fourCC << std::endl;
			return;
		}

		// Calculate the size of the texture data
		size_t dataSize = header.mipMapCount > 1
			? header.pitchOrLinearSize * 2
			: header.pitchOrLinearSize;

		// Read texture data
		std::vector<char> textureData(dataSize);
		file.read(textureData.data(), dataSize);
		file.close();

		// Load the texture data for each mipmap level
		uint32_t width = header.width;
		uint32_t height = header.height;

		size_t blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		size_t size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

		glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, width, height, 1, format, size, textureData.data()); // GL_RGBA8 ??????????????????????????
	}

	// birlesebilir 
	unsigned int GLTexture::generateCompressedTerrainPaletteTexture2D(std::string imagepath) {

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

	unsigned int GLTexture::generateHeightmapTexture(glm::u16vec2 size) {

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, size.x, size.y, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureId;
	}

	unsigned int GLTexture::generateHeightmapTexture(std::string path) {

		int width;
		int height;
		int channels;
		unsigned short* data = (unsigned short*)stbi_load_16(path.c_str(), &width, &height, &channels, 1);

		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureId;
	}

	unsigned int GLTexture::createPhysicalPagesFrameBufferTextureArray(unsigned short pageSize, UINT8 mipLevels, unsigned short pageCount) {

		unsigned int textureArray;
		glGenTextures(1, &textureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevels, GL_RGBA8, pageSize, pageSize, pageCount);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		float maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		return textureArray;
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

	void GLTexture::generateMipmapArrayTexture2D(unsigned int textureId) {

		glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}
}