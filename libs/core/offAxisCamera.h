#pragma once
#include "abstractstereocamera.h"
#include "declspec.h"

class QString;
class OffAxisCamera :
	public AbstractStereoCamera
{
public:
	OffAxisCamera(Camera & abstraction); 
	~OffAxisCamera(void);

    virtual const QString implementationName() const override;

protected:
    float m_focusDistance;

private:
    static const QString m_implementationName;
    static bool isRegistered;
};

