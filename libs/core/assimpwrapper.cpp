
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

Group * AssimpWrapper::parseNode(const aiScene & scene, aiNode & node)
{
//    TODO: Create Group for each aiNode and PolygonalDrawables for Meshes
    return nullptr;
}