#pragma once
#include "camera.h"
#include "declspec.h"

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
    void setFromMatrix(const glm::mat4 & view);
protected:
    float m_cameraSeparation;
    //for lookat calculation
    glm::vec3 m_cameraSeparationVector;
    glm::vec3 m_center;
    glm::vec3 m_virtualCameraPosition;
    glm::vec3 m_up;
};

