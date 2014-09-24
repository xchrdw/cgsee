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


class Material;
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

    void parseMaterials(aiMaterial **materials, unsigned int numMaterials, const QString & filePath, std::vector<Material*> & newMaterials) const;
    Material* parseMaterial(const aiMaterial *material, const QString & filePath) const;
    void loadTextures(const aiMaterial *material, const QString & filePath, Material * const newMaterial) const;
    void loadTextures(const aiMaterial *material, aiTextureType type, const QString & filePath, Material * const newMaterial) const;
    Image* loadTexture(const aiMaterial *material, aiTextureType type, int texture, const QString & filePath) const;

    void parseMeshes(aiMesh ** meshes,
        const unsigned int numMeshes, QList<PolygonalDrawable *> &, const std::vector<Material*> & materials) const;

    PolygonalDrawable * parseMesh(const aiMesh & mesh, const std::vector<Material*> & materials) const;

protected:
    Assimp::Importer * m_importer;

    QList<AbstractImageLoader*> m_imageLoaders;
};
