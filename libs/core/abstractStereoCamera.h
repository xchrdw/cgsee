#pragma once
#include "camera.h"
#include "declspec.h"
#include <QMap>
class QString;

class CGSEE_API AbstractStereoCamera :public Camera
{
public:
	AbstractStereoCamera(const QString & name = "unnamed");  
	virtual ~AbstractStereoCamera(void);
    virtual void setCameraSeparation(float cameraSeparation);
    virtual void setCenter(glm::vec3 center);
    virtual void virtualCameraPosition(glm::vec3 virtualCameraPosition);
    virtual void activateRightCamera();
    virtual void activateLeftCamera();
    void setFromMatrix();
protected:
    float m_cameraSeparation;
    typedef QMap<QString, FrameBufferObject *> t_samplerByName;
    //for lookat calculation
    glm::vec3 m_cameraSeparationVector;
    glm::vec3 m_center;
    glm::vec3 m_virtualCameraPosition;
    glm::vec3 m_up;
    void bindSampler(const t_samplerByName & sampler, const Program & program);
    void releaseSampler(const t_samplerByName & sampler);
};

