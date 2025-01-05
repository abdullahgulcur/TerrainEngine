#include "pch.h"
#include "texture2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "gl_context.h"

namespace Engine {

    Texture2D::Texture2D(){}

    Texture2D::Texture2D(std::string path) {

        int width;
        int height;
        int channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        // TODO: free data

        this->width = width;
        this->height = height;
        this->channels = channels;
        this->data = data;
        this->mipLevel = 1;
    }

    Texture2D::Texture2D(std::string path, UINT8 mipLevel) {

        int width;
        int height;
        int channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        // TODO: free data
        this->width = width;
        this->height = height;
        this->channels = channels;
        this->mipLevel = mipLevel;

        loadData(data);
    }

    Texture2D::~Texture2D() {}

    void Texture2D::generateMipmaps() {
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

                    unsigned int lowDataIndex = (prevStartIndex + 2 * (y * w * 2 + x)) * channels;
                    unsigned int highDataIndex = (currentStartIndex + (w * y + x)) * channels;

                    for (int c = 0; c < channels; c++)
                        data[highDataIndex + c] = data[lowDataIndex + c];
                }
            }
            prevStartIndex = currentStartIndex;
        }
    }

    unsigned int Texture2D::getIndex(unsigned short x, unsigned short y) {

        return (y * width + x) * channels;
    }

    unsigned int Texture2D::getIndex(unsigned short x, unsigned short y, UINT8 mipLevel) {

        unsigned int indexInLastMipLevel = (y * getWidth(mipLevel) + x) * channels;
        return getMipmapStartIndex(mipLevel) + indexInLastMipLevel;
    }

    void Texture2D::loadData(unsigned char* data) {

        unsigned int firstMipmapSize = width * height * channels;
        for (int i = 0; i < firstMipmapSize; i++)
            this->data[i] = data[i];

        Texture2D::generateMipmaps();
    }

    void Texture2D::writeDataToFile(std::string path, UINT8 mipLevel) {

        stbi_write_png(path.c_str(), getWidth(mipLevel), getHeight(mipLevel), channels, &data[getMipmapStartIndex(mipLevel)], getWidth(mipLevel) * channels);
    }

    void Texture2D::setSize() {

        size = Texture2D::getMipmapStartIndex(mipLevel);
    }

    unsigned int Texture2D::getWidth(int level) {

        return width >> level;
    }

    unsigned int Texture2D::getHeight(int level) {

        return height >> level;
    }

    unsigned int Texture2D::getMipmapSize(int level) {

        unsigned int w = width;
        unsigned int h = height;
        return (w >> level) * (h >> level) * channels;
    }

    unsigned int Texture2D::getSize() {

        return size;
    }

    unsigned int Texture2D::getMipmapStartIndex(int level) {
        unsigned int w = width;
        unsigned int h = height;

        unsigned int size = 0;
        for (int i = 0; i < level; i++) {

            size += w * h;
            w >>= 1;
            h >>= 1;
        }
        return size * channels;
    }

    void Texture2D::clean() {

        delete[] data;
    }

    void Texture2D::copyData(Texture2D& copy, glm::ivec2 posBase, glm::ivec2 posCopy, glm::ivec2 size) {

        int entireRowSize = size.x * channels;

        for (int y = 0; y < size.y; y++) {

            int indexCopy = copy.getIndex(posCopy.x, posCopy.y + y);
            int indexBase = Texture2D::getIndex(posBase.x, posBase.y + y);

            for (int i = 0; i < entireRowSize; i++)
                data[indexBase + i] = copy.data[indexCopy + i];
        }
    }

    void Texture2D::copyData(Texture2D& copy, UINT8 mipLevelBase, UINT8 mipLevelCopy, glm::ivec2 posBase, glm::ivec2 posCopy, glm::ivec2 size) {

        int entireRowSize = size.x * channels;

        for (int y = 0; y < size.y; y++) {

            int indexCopy = copy.getIndex(posCopy.x, posCopy.y + y, mipLevelCopy);
            int indexBase = Texture2D::getIndex(posBase.x, posBase.y + y, mipLevelBase);

            for (int i = 0; i < entireRowSize; i++)
                data[indexBase + i] = copy.data[indexCopy + i];
        }
    }

    unsigned int Texture2D::generateGLTexture() {

        return GLTexture::generateTexture2D(channels, width, height, data);
    }

}
