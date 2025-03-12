#pragma once

namespace Engine {

	class FileSystem {

	private:

		std::map<unsigned int, unsigned int> textures2D;

	public:

		static FileSystem* create();
		FileSystem();
		~FileSystem();
		void init();
	};
}