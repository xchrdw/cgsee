#pragma once
#include "camera.h"
#include "declspec.h"

class QString;

class CGSEE_API AbstractStereoCamera :public Camera
{
public:
	AbstractStereoCamera(const QString & name = "unnamed");  
	virtual ~AbstractStereoCamera(void);
    virtual void setCameraSeparation(glm::vec3 cameraSeparation);
    virtual void setCenter(glm::vec3 center);
    virtual void setViewDirection(glm::vec3 viewDirection);
    virtual void activateRightCamera();
    virtual void activateLeftCamera();
protected:
    glm::vec3 m_cameraSeparation;
    glm::vec3 m_center;
    glm::vec3 m_viewDirection;
};

