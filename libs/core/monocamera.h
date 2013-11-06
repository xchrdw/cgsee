#pragma once
#include "abstractstereocamera.h"
#include "declspec.h"

class QString;
class MonoCamera :
	public AbstractStereoCamera
{
public:
	MonoCamera(Camera & abstraction);  
	~MonoCamera(void);

    virtual void drawScene( const Program & program ) override;

    virtual const QString implementationName() const override;

private:
    static const QString m_implementationName;
    static bool isRegistered;
};

