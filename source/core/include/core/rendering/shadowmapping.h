#pragma once

#include <core/core_api.h>

#include <vector>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <core/rendering/defaultpass.h>

class FileAssociatedShader;
class LightSourcePass;

class CORE_API  ShadowMappingPass : public DefaultPass
{
public:
    ShadowMappingPass(Camera * camera, FileAssociatedShader * depth_util, LightSourcePass * lightsource);
    virtual ~ShadowMappingPass(void);
    virtual void render() override;
    virtual void setUniforms() override;

private:
    std::vector<glm::vec2> m_shadow_samples;
    LightSourcePass * m_lightsource;
    static const glm::mat4 biasMatrix;
};

