
#include <core/camera/monocamera.h>

#include <QDebug>

#include <core/gpuquery.h>

MonoCamera::MonoCamera(const QString & name, Projection * projection)
:   AbstractCamera(name, projection)
{

}

MonoCamera::MonoCamera(const AbstractCamera & camera)
:   AbstractCamera(camera)
{

}

MonoCamera::~MonoCamera()
{
}

bool MonoCamera::isStereo() const
{
    return false;
}




