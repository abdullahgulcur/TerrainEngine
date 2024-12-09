#pragma once

#include "GL/glew.h"
#include "glm.hpp"

#include "gl_uniform.h"
#include "gl_buffer.h"
#include "gl_command.h"
#include "gl_shader.h"
#include "gl_texture.h"

namespace Engine {

    class GLContext {
    private:
    public:
        
        static void init();
    };
}