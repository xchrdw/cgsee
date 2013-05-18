
#pragma once

#include <QString>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "declspec.h"
#include "common.h"
#include "abstractmodelloader.h"

class Group;
class PolygonalDrawable;
struct aiNode;
struct aiScene;
struct aiMesh;

class CGSEE_API AssimpLoader : public AbstractModelLoader
{
public:
    AssimpLoader();
    virtual ~AssimpLoader();
    
    virtual QStringList loadableTypes() const;
    virtual Group * importFromFile(const QString & filePath) const;
    
protected:
    virtual QStringList loadableExtensions() const;
    
protected:
    Group * parseNode(const aiScene & scene,
        const QVector<PolygonalDrawable *> &drawables, const aiNode & node) const;

    const QVector<PolygonalDrawable *> * parseMeshes(
        aiMesh ** meshes, const unsigned int numMeshes) const;

    PolygonalDrawable * parseMesh(const aiMesh & mesh) const;
    
protected:
    Assimp::Importer * m_importer;
};