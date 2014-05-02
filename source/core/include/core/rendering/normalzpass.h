#pragma once

#include <core/core_api.h>

#include <core/rendering/defaultpass.h>

class CORE_API NormalzPass : public DefaultPass
{
public:
    NormalzPass(Camera * camera, FileAssociatedShader * depth_util);
    virtual ~NormalzPass(void);
};

