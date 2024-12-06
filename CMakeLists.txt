cmake_minimum_required(VERSION 3.16)
project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/thirdparty/glfw)
add_subdirectory(src/thirdparty/glew)

file(GLOB sourceFiles "src/app/*.cpp" 
	"src/engine/core/*.cpp"
	"src/engine/filesystem/*.cpp"
	"src/engine/graphics/*.cpp"
	"src/engine/io/*.cpp"
	"src/engine/math/*.cpp"
	"src/engine/memory/*.cpp"
	"src/engine/platform/gl/*.cpp"
	"src/engine/platform/glfw/*.cpp"
	"src/engine/scene/*.cpp"
	"src/engine/terrain/*.cpp")

add_executable(${PROJECT_NAME} ${sourceFiles})

target_precompile_headers(${PROJECT_NAME} PRIVATE src/engine/core/pch.h)

target_include_directories(${PROJECT_NAME} PRIVATE src/thirdparty/glew/include)
target_include_directories(${PROJECT_NAME} PRIVATE src/thirdparty/glfw/include)
target_include_directories(${PROJECT_NAME} PRIVATE src/thirdparty/glm)
target_include_directories(${PROJECT_NAME} PRIVATE src/thirdparty/rapidXML)
target_include_directories(${PROJECT_NAME} PRIVATE src/thirdparty/stb_image)
target_include_directories(${PROJECT_NAME} PRIVATE src/engine/core)
target_include_directories(${PROJECT_NAME} PRIVATE src/engine/filesystem)
target_include_directories(${PROJECT_NAME} PRIVATE src/engine/graphics)
target_include_directories(${PROJECT_NAME} PRIVATE src/engine/io)
target_include_directories(${PROJECT_NAME} PRIVATE src/engine/math)
target_include_directories(${PROJECT_NAME} PRIVATE src/engine/terrain)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glew glfw OpenGL::GL)