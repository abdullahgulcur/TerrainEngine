#pragma once

#include "glm.hpp"

namespace Engine {

    class Heightmap {
    private:

        unsigned int size;
        void generateMipmaps();

    public:

        unsigned short width;
        unsigned short height;
        UINT8 mipLevel;
        unsigned int* data = NULL;

        Heightmap();
        Heightmap(std::string path);
        Heightmap(std::string path, UINT8 mipLevel);

        Heightmap(unsigned short width, unsigned short height)
            : Heightmap(width, height, 1) {}

        Heightmap(unsigned short width, unsigned short height, UINT8 mipLevel)
            : width(width), height(height), mipLevel(mipLevel) {

            setSize();
            data = new unsigned int[size];
        }

        ~Heightmap();
        void generateNormals();
        unsigned int getIndex(unsigned short x, unsigned short y);
        unsigned int getIndex(unsigned short x, unsigned short y, UINT8 mipLevel);
        void loadData(unsigned int* data);
        void writeDataToFile(std::string path, UINT8 mipLevel);
        void setSize();
        unsigned int getWidth(int level);
        unsigned int getHeight(int level);
        unsigned int getMipmapSize(int level);
        unsigned int getSize();
        unsigned int getMipmapStartIndex(int level);
        void clean();
        void copyData(Heightmap& copy, glm::ivec2 posBase, glm::ivec2 posCopy, glm::ivec2 size);
        void copyData(Heightmap& copy, UINT8 mipLevelBase, UINT8 mipLevelCopy, glm::ivec2 posBase, glm::ivec2 posCopy, glm::ivec2 size);

    };
}