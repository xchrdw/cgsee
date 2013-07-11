
#pragma once

#include <core/declspec.h>


// this interface is used to allow an resize call from within 
// the painter to an, e.g., QGLWidget (i.e. Canvas) without 
// the need of signals and slots.

class CGSEE_CORE_API AbstractGLParent
{
public:
    virtual void resize(int width, int height) = 0;
};