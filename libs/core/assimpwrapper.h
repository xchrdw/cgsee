
#pragma once

#include <QString>

#include "declspec.h"
#include "common.h"

class Group;
struct aiNode;
struct aiScene;

class CGSEE_API AssimpWrapper
{
public:
    static Group * groupFromFile(const QString & filePath);
    
protected:
    static Group * parseNode(const aiScene & scene, aiNode & node);
    AssimpWrapper();
};