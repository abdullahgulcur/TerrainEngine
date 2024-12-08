#pragma once

#include "glm.hpp"

namespace Engine {

    class Texture2D {
    private:

        unsigned int size;
        void generateMipmaps();

    public:

        unsigned short width;
        unsigned short height;
        UINT8 channels;
        UINT8 mipLevel;
        unsigned char* data = NULL;

        Texture2D();
        Texture2D(std::string path);
        Texture2D(std::string path, UINT8 mipLevel);
        ~Texture2D();
        unsigned int getIndex(unsigned short x, unsigned short y);
        unsigned int getIndex(unsigned short x, unsigned short y, UINT8 mipLevel);
        void loadData(unsigned char* data);
        void writeDataToFile(std::string path, UINT8 mipLevel);
        void setSize();
        unsigned int getWidth(int level);
        unsigned int getHeight(int level);
        unsigned int getMipmapSize(int level);
        unsigned int getSize();
        unsigned int getMipmapStartIndex(int level);
        void clean();
        void copyData(Texture2D& copy, glm::ivec2 posBase, glm::ivec2 posCopy, glm::ivec2 size);
        void copyData(Texture2D& copy, UINT8 mipLevelBase, UINT8 mipLevelCopy, glm::ivec2 posBase, glm::ivec2 posCopy, glm::ivec2 size);
    };
}