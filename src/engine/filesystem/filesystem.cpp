#include "pch.h"
#include "filesystem.h"
#include "gl_texture.h"

namespace Engine {

	FileSystem* FileSystem::create() {
		return new FileSystem;
	}

	FileSystem::FileSystem() {}

	FileSystem::~FileSystem() {}

	void FileSystem::init() {
		
	}

	void FileSystem::loadTexture2D(unsigned int key, std::string path) {
		
		Texture2D tex(path);
		textures2D[key] = GLTexture::generateTexture2D(tex.channels, tex.width, tex.height, tex.data);
		tex.clean();
	}

	unsigned int FileSystem::getTexture2D(unsigned int key) {

		return textures2D[key];
	}

}