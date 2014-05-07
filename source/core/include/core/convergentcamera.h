#pragma once

#include <core/core_api.h>

#include <core/abstractstereocamera.h>


class QString;

class CORE_API ConvergentCamera : public AbstractStereoCamera
{
public:
	ConvergentCamera(Camera & abstraction); 
	~ConvergentCamera(void);

    virtual void activateRightCamera(const Program & program);
    virtual void activateLeftCamera(const Program & program);
    virtual void drawScene(const Program & program) override;
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
