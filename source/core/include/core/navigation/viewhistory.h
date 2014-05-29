#pragma once
#include <core/navigation/viewhistoryelement.h>
#include <core/navigation/abstractnavigation.h>
#include <QImage>

class ViewHistory
{
public:
    ViewHistory();
    ~ViewHistory();
    void setNavigation(AbstractNavigation * navigation);
    void save(glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    void undo();
    void redo();
    bool isEmpty();

private:
    AbstractNavigation * m_navigation;
    ViewHistoryElement* m_viewhistory;
};
