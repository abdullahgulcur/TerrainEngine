#include "pch.h"
#include "renderer.h"
#include "terrain_render.h"
#include "gl_context.h"
#include "core.h"

namespace Engine {

	Renderer* Renderer::create() {

		return new Renderer;
	}

	void Renderer::init() {

		GLContext::init();

	}

	void Renderer::loadFrames() {

		frameScene = FrameScene(Core::getGlfwContext()->getScreenSize());
		frameDefault = FrameDefault(Core::getGlfwContext()->getScreenSize(), Core::getShader()->shaders[ShaderType::FRAME_DEFAULT], frameScene.textureId, frameScene.depthTextureId);
	}

	void Renderer::draw(Scene* scene) {

		frameScene.setScreen();

		if (scene->terrain) {
			TerrainRender renderer;
			renderer.draw(scene->terrain);
		}

		// post process frames...

		frameDefault.draw();
	}
}