#pragma once

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "declspec.h"
#include "group.h"

class DataBlockRegistry;
class Group;
class Group;
class PolygonalDrawable;

class CGSEE_API SceneGraph
{
public:
    SceneGraph( DataBlockRegistry & registry );
    
    Group * createGroup( Group * parent = nullptr, const QString & name = "Group" );
    PolygonalDrawable * createPolygonalDrawable( Group * parent = nullptr, const QString & name = "Geometry" );
    
    const Group & root() const;
    Group & root();
    
    DataBlockRegistry & registry() { return m_registry; }
    
protected:
    DataBlockRegistry & m_registry;
    Group m_root;
};

#endif // SCENEGRAPH_H
