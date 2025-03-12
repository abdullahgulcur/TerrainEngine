#include "pch.h"
#include "filesystem.h"

namespace Engine {

	FileSystem* FileSystem::create() {
		return new FileSystem;
	}

	FileSystem::FileSystem() {}

	FileSystem::~FileSystem() {}

	void FileSystem::init() {
		
	}

}