#pragma once

#include <core/core_api.h>

#include <glm/glm.hpp>

#include <QVector>

#include <core/camera/abstractcamera.h>

class Projection;

class CORE_API MonoCamera : public AbstractCamera
{
public:

    MonoCamera(const QString & name, Projection * projection);
    virtual ~MonoCamera();

    virtual bool isStereo() override;
};
