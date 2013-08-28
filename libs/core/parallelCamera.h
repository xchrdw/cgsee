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
    virtual void activateOculusRiftRightCamera(const Program & program, FrameBufferObject * target);
    virtual void activateOculusRiftLeftCamera(const Program & program, FrameBufferObject * target);
    virtual void activateRedCyanRightCamera(const Program & program, FrameBufferObject * target);
    virtual void activateRedCyanLeftCamera(const Program & program, FrameBufferObject * target);
    virtual void draw(const Program & program, FrameBufferObject * target = nullptr);
    virtual void draw(const Program & program, const glm::mat4 & transform);
    virtual void drawRedCyan(const Program & program, FrameBufferObject * target = nullptr);
    virtual void drawOculusRift(const Program & program, FrameBufferObject * target = nullptr);
    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

protected:
    FrameBufferObject *m_left, *m_right;

    void initialize(const Program &program);
    static void bindSampler(const t_samplerByName & sampler, const Program & program);
    static void releaseSampler(const t_samplerByName & sampler);
};

