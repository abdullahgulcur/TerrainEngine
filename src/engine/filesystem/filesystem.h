#pragma once

#include "texture2d.h"

namespace Engine {

	class FileSystem {

	private:

		std::map<unsigned int, unsigned int> textures2D;

	public:

		static FileSystem* create();
		FileSystem();
		~FileSystem();
		void init();

		void loadTexture2D(unsigned int key, std::string path);
		unsigned int getTexture2D(unsigned int key);
	};
}