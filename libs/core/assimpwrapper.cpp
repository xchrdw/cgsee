
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assimpwrapper.h"
#include "group.h"


Group * AssimpWrapper::groupFromFile(const QString & filePath)
{
    Assimp::Importer importer;
    
    qDebug("Reading geometry with Assimp from \"%s\".", qPrintable(filePath));
    
    const aiScene * scene = importer.ReadFile(filePath.toStdString(),
                                              aiProcess_Triangulate);
    
    if (!scene) {
        qCritical("Assimp couldn't load %s", qPrintable(filePath));
    }
        
    return parseNode(*scene, *(scene->mRootNode));
}

Group * AssimpWrapper::parseNode(const aiScene & scene, const aiNode & node)
{
    Group * group = new Group(node.mName.C_Str());
    
    const aiMatrix4x4 & mat = node.mTransformation;
    glm::mat4 transform(
        mat.a1, mat.a2, mat.a3, mat.a4,
        mat.b1, mat.b2, mat.b3, mat.b4,
        mat.c1, mat.c2, mat.c3, mat.c4,
        mat.d1, mat.d2, mat.d3, mat.d4
    );
    group->setTransform(transform);
    
    for (int i = 0; i < node.mNumChildren; i++)
        group->append(parseNode(scene, *(node.mChildren[i])));
    
    for (int i = 0; i < node.mNumMeshes; i++)
        group->append(parseMesh(*(scene.mMeshes[node.mMeshes[i]])));
    
    return group;
}

PolygonalDrawable * AssimpWrapper::parseMesh(const aiMesh & mesh)
{
    return nullptr;
}