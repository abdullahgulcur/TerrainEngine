#include "pch.h"
#include "shadow_map_renderer.h"
#include "terrain.h"
#include "gl_context.h"
#include "core.h"
#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"
#include "terrain_render.h"

namespace Engine {

	void ShadowmapRenderer::init() {

        shadowCascadeLevels = { 16,32,64,128,256,512,1024,2048,4096,8192 };

        // configure light FBO
        // -----------------------
        glGenFramebuffers(1, &lightFBO);

        glGenTextures(1, &lightDepthMaps);
        glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, depthMapResolution, depthMapResolution, int(shadowCascadeLevels.size()) + 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

        glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, lightDepthMaps, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
            throw 0;
        }

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // configure UBO
        // --------------------
        glGenBuffers(1, &matricesUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void ShadowmapRenderer::update(Terrain* terrain) {

        // 0. UBO setup
        const auto lightMatrices = getLightSpaceMatrices();
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
        for (size_t i = 0; i < lightMatrices.size(); ++i)
            glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // 1. render depth of scene to texture (from light's perspective)        
        glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
        glViewport(0, 0, depthMapResolution, depthMapResolution);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);  // peter panning

        TerrainRender renderer;
        //renderer.drawShadow(terrain);
	}

    std::vector<glm::vec4> ShadowmapRenderer::getFrustumCornersWorldSpace(const glm::mat4& projview) {

        const auto inv = glm::inverse(projview);
        std::vector<glm::vec4> frustumCorners;
        for (unsigned int x = 0; x < 2; ++x) {
            for (unsigned int y = 0; y < 2; ++y) {
                for (unsigned int z = 0; z < 2; ++z) {
                    const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }
        return frustumCorners;
    }


    std::vector<glm::vec4> ShadowmapRenderer::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) {
        return getFrustumCornersWorldSpace(proj * view);
    }

    glm::mat4 ShadowmapRenderer::getLightSpaceMatrix(const float nearPlane, const float farPlane) {

        glm::ivec2 cameraBounds = Core::getCamera()->bounds;
        glm::vec3 lightDir(-1, 1, -1);

        const auto proj = glm::perspective(glm::radians(45.f), (float)cameraBounds.x / (float)cameraBounds.y, nearPlane, farPlane);
        const auto corners = getFrustumCornersWorldSpace(proj, Core::getCamera()->viewMatrix);

        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& v : corners)
            center += glm::vec3(v);
        center /= corners.size();

        const auto lightView = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();
        for (const auto& v : corners) {
            const auto trf = lightView * v;
            minX = std::min(minX, trf.x);
            maxX = std::max(maxX, trf.x);
            minY = std::min(minY, trf.y);
            maxY = std::max(maxY, trf.y);
            minZ = std::min(minZ, trf.z);
            maxZ = std::max(maxZ, trf.z);
        }

        // Tune this parameter according to the scene
        constexpr float zMult = 10.0f;
        if (minZ < 0)
            minZ *= zMult;
        else
            minZ /= zMult;
        if (maxZ < 0)
            maxZ /= zMult;
        else
            maxZ *= zMult;

        const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        return lightProjection * lightView;
    }

    std::vector<glm::mat4> ShadowmapRenderer::getLightSpaceMatrices() {
        std::vector<glm::mat4> ret;
        for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i) {
            if (i == 0)
                ret.push_back(getLightSpaceMatrix(Core::getCamera()->nearClip, shadowCascadeLevels[i]));
            else if (i < shadowCascadeLevels.size())
                ret.push_back(getLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
            else
                ret.push_back(getLightSpaceMatrix(shadowCascadeLevels[i - 1], Core::getCamera()->farClip));
        }
        return ret;
    }
}