#pragma once
#include "cameraimplementation.h"
#include "declspec.h"
#include <QMap>
class QString;

class CGSEE_API AbstractStereoCamera :public CameraImplementation
{
public:
	AbstractStereoCamera(Camera & abstraction);  
	virtual ~AbstractStereoCamera(void);
    virtual void setCameraSeparation(float cameraSeparation);
    virtual void setCenter(glm::vec3 center);
    virtual void virtualCameraPosition(glm::vec3 virtualCameraPosition);
    virtual void activateRightCamera();
    virtual void activateLeftCamera();
    void setFromMatrix();
protected:
    float m_cameraSeparation;
    //for lookat calculation
    glm::vec3 m_cameraSeparationVector;
    glm::vec3 m_center;
    glm::vec3 m_virtualCameraPosition;
    glm::vec3 m_up;

};

