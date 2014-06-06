/// Preprocessor directive
#pragma once

/// Include navigation history wrapper and history elements list.
#include <core/navigation/abstractnavigation.h>
#include <core/navigation/navigationhistoryelement.h>

/// Include signalzeug from libzeug.
#include <signalzeug/Signal.h>

/// Include QImage for thumbnails.
#include <QImage>

/**
 * @brief The navigation history wrapper class.
 * @details NavigationHistory is a wrapper class which handles all commands for
 *          the NavigationHistoryElements linked list which stores the complete
 *          history.
 */
class NavigationHistory
{

public:

    /// Constructor & destructor
    NavigationHistory();
    ~NavigationHistory();

    /// Setter
    void setNavigation(AbstractNavigation * navigation);

    /// Getter
    NavigationHistoryElement * navigationHistoryElement();

    /// Public methods
    void save(glm::mat4 viewmatrix, float fovy, QImage thumbnail);
    void undo();
    void redo();
    void reset();
    bool isEmpty();

    /// Signals
    void onHistoryChanged();
    signalzeug::Signal<> historyChanged;

private:

    /// Member variables
    AbstractNavigation * m_navigation;
    NavigationHistoryElement * m_navigationHistory;

};
