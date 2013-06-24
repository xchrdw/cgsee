
#pragma once

#include "declspec.h"



class CGSEE_API CoordinateProvider
{
public:
    CoordinateProvider();
    virtual ~CoordinateProvider();

    int objID(int x, int y);
    float3 pointAt(int x, int y);
};