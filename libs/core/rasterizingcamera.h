#pragma once

#include <glm/glm.hpp>

#include "declspec.h"
#include "cameraimplementation.h"


class Program;

class CGSEE_API RasterizingCamera : public CameraImplementation
{
public:
    RasterizingCamera(Camera & abstraction);
    virtual ~RasterizingCamera();

    virtual void draw( const Program & program, const glm::mat4 & transform) override;

    virtual const QString implementationName() const override;

private:
    static const QString m_implementationName;
    static bool isRegistered;
};
