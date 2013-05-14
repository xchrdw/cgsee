
#pragma once

#include <QString>

#include "declspec.h"
#include "common.h"

class Group;
class PolygonalDrawable;
struct aiNode;
struct aiScene;
struct aiMesh;

class CGSEE_API AssimpWrapper
{
public:
    static Group * groupFromFile(const QString & filePath);
    
protected:
    static Group * parseNode(const aiScene & scene,
                             const QVector<PolygonalDrawable *> &drawables,
                             const aiNode & node);
    static const QVector<PolygonalDrawable *> * parseMeshes(aiMesh ** meshes,
                                                            const unsigned int numMeshes);
    static PolygonalDrawable * parseMesh(const aiMesh & mesh);
    AssimpWrapper();
};