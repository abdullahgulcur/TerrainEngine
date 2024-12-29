#include "pch.h"
#include "frame.h"
#include "gl_context.h"

namespace Engine {

    Frame::Frame() {}

    Frame::~Frame(){}

    void Frame::setViewport(glm::u16vec2 pos, glm::u16vec2 size) {
        this->viewportPos = pos;
        this->viewportSize = size;
    }

    unsigned int Frame::getTextureId() {
        return textureId;
    }

    glm::u16vec2 Frame::getSize() {
        return size;
    }

}
