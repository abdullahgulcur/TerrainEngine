#include "pch.h"
#include "heightmap.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#define STBI_MSC_SECURE_CRT
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Engine {

    Heightmap::Heightmap() {}

    Heightmap::Heightmap(std::string path) {

        //int width;
        //int height;
        //int channels;
        //unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        //// TODO: free data

        //this->width = width;
        //this->height = height;
        //this->channels = channels;
        //this->data = data;
        //this->mipLevel = 1;
    }

    Heightmap::Heightmap(std::string path, UINT8 mipLevel) {

        //int width;
        //int height;
        //int channels;
        //unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        //// TODO: free data
        //this->width = width;
        //this->height = height;
        //this->channels = channels;
        //this->mipLevel = mipLevel;

        //loadData(data);
    }

    Heightmap::~Heightmap() {}

    void Heightmap::generateMipmaps() {
        unsigned int w = width;
        unsigned int h = height;

        int prevStartIndex = 0;
        for (int level = 1; level < mipLevel; level++) {

            int prevSize = w * h;
            int currentStartIndex = prevStartIndex + prevSize;

            w >>= 1;
            h >>= 1;
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {

                    unsigned int lowDataIndex = (prevStartIndex + 2 * (y * w * 2 + x));
                    unsigned int highDataIndex = (currentStartIndex + (w * y + x));

                    data[highDataIndex] = data[lowDataIndex];
                }
            }
            prevStartIndex = currentStartIndex;
        }
    }

    void Heightmap::generateNormals() {
        
    }

    unsigned int Heightmap::getIndex(unsigned short x, unsigned short y) {

        return (y * width + x);
    }

    unsigned int Heightmap::getIndex(unsigned short x, unsigned short y, UINT8 mipLevel) {

        unsigned int indexInLastMipLevel = (y * getWidth(mipLevel) + x);
        return getMipmapStartIndex(mipLevel) + indexInLastMipLevel;
    }

    void Heightmap::loadData(unsigned int* data) {

        unsigned int firstMipmapSize = width * height;
        for (int i = 0; i < firstMipmapSize; i++)
            this->data[i] = data[i];

        Heightmap::generateMipmaps();
        Heightmap::generateNormals();
    }

    void Heightmap::writeDataToFile(std::string path, UINT8 mipLevel) {

        //stbi_write_png(path.c_str(), getWidth(mipLevel), getHeight(mipLevel), channels, &data[getMipmapStartIndex(mipLevel)], getWidth(mipLevel) * channels);
    }

    void Heightmap::setSize() {

        size = Heightmap::getMipmapStartIndex(mipLevel);
    }

    unsigned int Heightmap::getWidth(int level) {

        return width >> level;
    }

    unsigned int Heightmap::getHeight(int level) {

        return height >> level;
    }

    unsigned int Heightmap::getMipmapSize(int level) {

        unsigned int w = width;
        unsigned int h = height;
        return (w >> level) * (h >> level);
    }

    unsigned int Heightmap::getSize() {

        return size;
    }

    unsigned int Heightmap::getMipmapStartIndex(int level) {
        unsigned int w = width;
        unsigned int h = height;

        unsigned int size = 0;
        for (int i = 0; i < level; i++) {

            size += w * h;
            w >>= 1;
            h >>= 1;
        }
        return size;
    }

    void Heightmap::clean() {

        delete[] data;
    }

    void Heightmap::copyData(Heightmap& copy, glm::ivec2 posBase, glm::ivec2 posCopy, glm::ivec2 size) {

        int entireRowSize = size.x;

        for (int y = 0; y < size.y; y++) {

            int indexCopy = copy.getIndex(posCopy.x, posCopy.y + y);
            int indexBase = Heightmap::getIndex(posBase.x, posBase.y + y);

            for (int i = 0; i < entireRowSize; i++)
                data[indexBase + i] = copy.data[indexCopy + i];
        }
    }

    void Heightmap::copyData(Heightmap& copy, UINT8 mipLevelBase, UINT8 mipLevelCopy, glm::ivec2 posBase, glm::ivec2 posCopy, glm::ivec2 size) {

        int entireRowSize = size.x;

        for (int y = 0; y < size.y; y++) {

            int indexCopy = copy.getIndex(posCopy.x, posCopy.y + y, mipLevelCopy);
            int indexBase = Heightmap::getIndex(posBase.x, posBase.y + y, mipLevelBase);

            for (int i = 0; i < entireRowSize; i++)
                data[indexBase + i] = copy.data[indexCopy + i];
        }
    }
}
