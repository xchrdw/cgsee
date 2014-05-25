#pragma once
#include <core/navigation/viewhistoryelement.h>
#include <core/navigation/abstractnavigation.h>
#include <QImage>

class ViewHistory
{
public:
    ViewHistory(AbstractNavigation * navigation);
    ~ViewHistory();

    void save(glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    void undo();
    void redo();

private:
    AbstractNavigation * m_navigation;
    ViewHistoryElement* m_viewhistory;
};
