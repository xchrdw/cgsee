#pragma once

#include "abstractstereocamera.h"
#include "declspec.h"

class QString;

class CGSEE_API ConvergentCamera :
	public AbstractStereoCamera
{
public:
	ConvergentCamera(const QString & name = "unnamed"); 
	~ConvergentCamera(void);
    virtual void activateRightCamera(const Program & program,   FrameBufferObject * target);
    virtual void activateLeftCamera(const Program & program,   FrameBufferObject * target);
    virtual void draw(const Program & program, FrameBufferObject * target = nullptr);
    virtual void draw(const Program & program, const glm::mat4 & transform);
    virtual void setCameraSeparation(float cameraSeparation);
    void setFocusDistance(float focusDistance);

protected:
    float m_focusDistance;

    void initialize(const Program &program);
};

