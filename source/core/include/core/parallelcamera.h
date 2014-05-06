#pragma once

#include <core/core_api.h>

#include <core/abstractstereocamera.h>

class QString;
class FrameBufferObject;

class CORE_API ParallelCamera : public AbstractStereoCamera
{
public:
	ParallelCamera(Camera & abstraction); 
	~ParallelCamera(void);
    virtual void setCameraSeparation(float cameraSeparation);
    virtual void activateOculusRiftRightCamera(const Program & program, FrameBufferObject * target);
    virtual void activateOculusRiftLeftCamera(const Program & program, FrameBufferObject * targe);
    virtual void activateRedCyanRightCamera(const Program & program);
    virtual void activateRedCyanLeftCamera(const Program & program);
    virtual void drawScene(const Program & program) override;
    virtual void drawRedCyan(const Program & program);
    virtual void drawOculusRift(const Program & program);
    typedef QMap<QString, FrameBufferObject *> t_samplerByName;
    void activateOculusRift();
    void deactivateOculusRift();

    virtual const QString implementationName() const override;

protected:
    FrameBufferObject *m_left, *m_right;
    bool m_oculus;

    void initialize(const Program &program);
    static void bindSampler(const t_samplerByName & sampler, const Program & program);
    static void releaseSampler(const t_samplerByName & sampler);

private:
    static const QString m_implementationName;
    static bool isRegistered;
};
