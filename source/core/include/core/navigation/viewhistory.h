#pragma once
#include <core/navigation/viewhistoryelement.h>
#include <core/navigation/abstractnavigation.h>
#include <QImage>
#include <signalzeug/Signal.h>

class ViewHistory
{
public:
    ViewHistory();
    ~ViewHistory();
    void setNavigation(AbstractNavigation * navigation);
    ViewHistoryElement * viewhistory();
    void save(glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    void undo();
    void redo();
    bool isEmpty();

    void onHistoryChanged();
    signalzeug::Signal<> historyChanged;

private:
    AbstractNavigation * m_navigation;
    ViewHistoryElement * m_viewhistory;
};
