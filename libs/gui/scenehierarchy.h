
#pragma once


#include <QTreeView>


#include <core/declspec.h>



class CGSEE_API SceneHierarchy : public QTreeView
{
public:
    SceneHierarchy(QWidget * parent = nullptr);
    virtual ~SceneHierarchy();
};