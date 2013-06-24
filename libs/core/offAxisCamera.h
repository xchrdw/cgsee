#pragma once
#include "abstractstereocamera.h"
#include "declspec.h"

class QString;
class OffAxisCamera :
	public AbstractStereoCamera
{
public:
	OffAxisCamera(const QString & name = "unnamed"); 
	~OffAxisCamera(void);

protected:
    float m_focusDistance;
};

