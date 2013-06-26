#pragma once
#include "abstractstereocamera.h"
#include "declspec.h"

class QString;

class CGSEE_API ParallelCamera :
	public AbstractStereoCamera
{
public:
	ParallelCamera(const QString & name = "unnamed"); 
	~ParallelCamera(void);
    virtual void activateRightCamera(const Program & program
,   FrameBufferObject * target);
    virtual void activateLeftCamera(const Program & program
,   FrameBufferObject * target);
    virtual void draw(
        const Program & program
    ,   FrameBufferObject * target = nullptr);
    virtual void draw(
        const Program & program
    ,   const glm::mat4 & transform);
};

