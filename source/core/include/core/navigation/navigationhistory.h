#pragma once
#include <core/navigation/navigationhistoryelement.h>
#include <core/navigation/abstractnavigation.h>
#include <QImage>
#include <signalzeug/Signal.h>

class NavigationHistory
{
public:
    NavigationHistory();
    ~NavigationHistory();
    void setNavigation(AbstractNavigation * navigation);
    NavigationHistoryElement * navigationHistory();
    void save(glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    void undo();
    void redo();
    bool isEmpty();

    void onHistoryChanged();
    signalzeug::Signal<> historyChanged;

private:
    AbstractNavigation * m_navigation;
    NavigationHistoryElement * m_navigationHistory;
};
