#pragma once

#include <core/core_api.h>

#include <memory>

#include <QString>
#include <QList>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <core/typedefs.h>
#include <core/abstractmodelloader.h>

class DataBlockRegistry;
class Group;
class PolygonalDrawable;
class AbstractImageLoader;
class Image;
struct aiNode;
struct aiScene;
struct aiMesh;


class CORE_API AssimpLoader : public AbstractModelLoader
{
public:
    AssimpLoader(std::shared_ptr<DataBlockRegistry> registry = nullptr);
    virtual ~AssimpLoader();
    
    virtual QStringList namedLoadableTypes() const;
    virtual Group * importFromFile(const QString & filePath) const;
    
protected:
    virtual QStringList loadableExtensions() const;
    
protected:
    Group * parseNode(const aiScene & scene,
        const QList<PolygonalDrawable *> &drawables, const aiNode & node) const;

    void parseTextures(aiTexture **textures, unsigned int numTextures) const;
    void parseTexture(aiTexture *texture) const;

    void parseMaterials(aiMaterial **materials, unsigned int numMaterials, const QString & filePath) const;
    void parseMaterial(aiMaterial *material, const QString & filePath) const;
    void loadTextures(aiMaterial *material, const QString & filePath) const;
    void loadTextures(aiMaterial *material, aiTextureType type, const QString & filePath) const;
    Image* loadTexture(aiMaterial *material, aiTextureType type, int texture, const QString & filePath) const;

    void parseMeshes(aiMesh ** meshes,
        const unsigned int numMeshes, QList<PolygonalDrawable *> & drawables) const;

    PolygonalDrawable * parseMesh(const aiMesh & mesh) const;

protected:
    Assimp::Importer * m_importer;

    QList<AbstractImageLoader*> m_imageLoaders;
};
