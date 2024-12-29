#include "pch.h"
#include "frame_scene.h"
#include "gl_context.h"
#include "core.h"

namespace Engine {

    FrameScene::FrameScene() {}

    FrameScene::FrameScene(glm::u16vec2 size) {
        this->size = size;
        textureId = GLTexture::createFrameSceneTexture(size);
        FBO = GLBuffer::createFBO(textureId);
        RBO = GLBuffer::createRBO(size);
    }

    FrameScene::~FrameScene() {}

    void FrameScene::setScreen() {

        GLCommand::refreshScene(size, FBO);
    }

}