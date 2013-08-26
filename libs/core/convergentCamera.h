#pragma once



#include "abstractstereocamera.h"
#include "declspec.h"

//#include <GL/glew.h>

class QString;

class CGSEE_API ConvergentCamera :
	public AbstractStereoCamera
{
public:
	ConvergentCamera(const QString & name = "unnamed"); 
	~ConvergentCamera(void);
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
    typedef QMap<QString, FrameBufferObject *> t_samplerByName;
protected:
    float m_focusDistance;
    FrameBufferObject *left, *right;
    void initialize(const Program &program);
   // unsigned int m_aFramebuffer;
  //  unsigned int m_bFramebuffer;
   // unsigned int m_aTexture[2];
  //  unsigned int m_bTexture;

    static void bindSampler(
        const t_samplerByName & sampler
    ,   const Program & program);

    static void releaseSampler(
        const t_samplerByName & sampler);
};

