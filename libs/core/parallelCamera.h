#pragma once
#include "abstractstereocamera.h"
#include "declspec.h"

class QString;

class CGSEE_API ParallelCamera :
	public AbstractStereoCamera
{
public:
	ParallelCamera(const QString & name = "unnamed"); 
	~ParallelCamera(void);
};

