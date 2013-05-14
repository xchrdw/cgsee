
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assimpwrapper.h"
#include "group.h"
#include "polygonalgeometry.h"
#include "polygonaldrawable.h"


Group * AssimpWrapper::groupFromFile(const QString & filePath)
{
    Assimp::Importer importer;
    
    qDebug("Reading geometry with Assimp from \"%s\".", qPrintable(filePath));
    
    const aiScene * scene = importer.ReadFile(filePath.toStdString(),
                                              aiProcess_Triangulate);
    
    if (!scene) {
        qCritical("Assimp couldn't load %s", qPrintable(filePath));
        return nullptr;
    }
    
    const QVector<PolygonalDrawable *> * drawables = parseMeshes(scene->mMeshes, scene->mNumMeshes);
    Group * group = parseNode(*scene, *drawables, *(scene->mRootNode));
    delete drawables;
    return group;
}

Group * AssimpWrapper::parseNode(const aiScene & scene, const QVector<PolygonalDrawable *> &drawables, const aiNode & node)
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
        group->append(parseNode(scene, drawables, *(node.mChildren[i])));
    
    for (int i = 0; i < node.mNumMeshes; i++)
        group->append(drawables[node.mMeshes[i]]);
    
    return group;
}

const QVector<PolygonalDrawable *> * AssimpWrapper::parseMeshes(aiMesh ** meshes, const unsigned int numMeshes)
{
    QVector<PolygonalDrawable *> * drawables = new QVector<PolygonalDrawable *>(numMeshes);
    for (int i = 0; i < numMeshes; i++) 
        drawables->insert(i, parseMesh(*meshes[i]));

    return drawables;
}

PolygonalDrawable * AssimpWrapper::parseMesh(const aiMesh & mesh)
{
    PolygonalGeometry * geometry = new PolygonalGeometry(QString(mesh.mName.C_Str()) + " geometry");
    
    for (int i = 0; i < mesh.mNumVertices; i++) {
        glm::vec3 vector(
            mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z
        );
        geometry->vertices().push_back(vector);
    }
    
    for (int i = 0; i < mesh.mNumFaces; i++) {
        if (mesh.mFaces[i].mNumIndices != 3)
            qCritical("Ignore polygon with num vertices != 3 (only triangles are supported).");
        else
            for (int j = 0; j < mesh.mFaces[i].mNumIndices; j++)
                geometry->indices().push_back(mesh.mFaces[i].mIndices[j]);
    }
    
    geometry->setMode(GL_TRIANGLES);
    geometry->retrieveNormals();
    
    PolygonalDrawable * drawable = new PolygonalDrawable(mesh.mName.C_Str());
    drawable->setGeometry(geometry);
    return drawable;
}