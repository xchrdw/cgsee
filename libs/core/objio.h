#pragma once

#include <cstring>
#include <sstream>
#include <memory>

#include <QString>

#include "declspec.h"

#include "common.h"


class SceneGraph;
class Group;
class PolygonalDrawable;
class GeometryData;

class CGSEE_API ObjIO
{
public:
    static void groupFromObjFile(const QString & filePath, SceneGraph & scene, Group & parent);

protected:

    struct ObjGroup
    {
        std::string name;
        inline const QString qname() const
        {
            return QString::fromStdString(name);
        }

        t_uints vis;
        t_uints vtis;
        t_uints vnis;

        // indices are stored in groups for, e.g., triangles, quads, polygons etc.
        // foffs stores the indices offsets to each face. This allows storing the
        // obj indices in the simplest and most supportive way.
        t_uints foffs;
    };

    typedef QVector<ObjGroup *> t_groups;

    struct ObjObject : public ObjGroup
    {
        t_vec3s vs;
        t_vec2s vts;
        t_vec3s vns;

        t_groups groups;

        ~ObjObject();
    };

    typedef QVector<ObjObject *> t_objects;

    enum e_FaceFormat
    {
        FF_V
    ,   FF_VT
    ,   FF_VN
    ,   FF_VTN
    };


protected:
    static void addToScene( const t_objects & objects, SceneGraph & scene, Group & parent );
    static std::shared_ptr<GeometryData> createGeometry(
        PolygonalDrawable & drawable
    ,   const ObjObject & object
    ,   const ObjGroup & group);

    static void parseV(
        std::istringstream & line
    ,   ObjObject & object);
    static void parseVT(
        std::istringstream & line
    ,   ObjObject & object);
    static void parseVN(
        std::istringstream & line
    ,   ObjObject & object);
    static void parseF(
        std::istringstream & line
    ,   ObjObject & object);
    static void parseO(
        std::istringstream & line
    ,   t_objects & objects);
    static void parseG(
        std::istringstream & line
    ,   ObjObject & object); 

    static const e_FaceFormat parseFaceFormat(const std::istringstream & line);
};