#pragma once

#include "abstractstereocamera.h"
#include "declspec.h"

class QString;

class CGSEE_API ConvergentCamera :
	public AbstractStereoCamera
{
public:
	ConvergentCamera(Camera & abstraction); 
	~ConvergentCamera(void);
    virtual void activateRightCamera(const Program & program);
    virtual void activateLeftCamera(const Program & program);
    virtual void drawScene(const Program & program, const glm::mat4 & transform) override;
    virtual void setCameraSeparation(float cameraSeparation);
    void setFocusDistance(float focusDistance);

    virtual const QString implementationName() const override;

protected:
    float m_focusDistance;

    void initialize(const Program &program);

private:
    static const QString m_implementationName;
    static bool isRegistered;
};

