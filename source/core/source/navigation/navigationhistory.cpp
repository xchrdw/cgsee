/// Include navigation history wrapper and history elements list.
#include <core/navigation/navigationhistory.h>

/// Include QImage for thumbnails.
#include <QImage>

/// Include signalzeug from libzeug.
#include <signalzeug/Signal.h>

/**
 * @brief Constructor for NavigationHistory class.
 * @details NavigationHistory is a wrapper class which handles all commands for
 *          the NavigationHistoryElements linked list which stores the complete
 *          history.
 */
NavigationHistory::NavigationHistory()
    : m_navigation(nullptr)
    , m_navigationHistory(nullptr)
{ }

/**
 * @brief Destructor for NavigationHistory class.
 * @details Resets the complete history, unlinks and deletes all list elements.
 */
NavigationHistory::~NavigationHistory()
{
    reset();
}

/**
 * @brief Setter for navigation type.
 * @details Allows to link a navigation manipulator to the history.
 *
 * @param navigation Navigation manipulator object.
 */
void NavigationHistory::setNavigation(AbstractNavigation * navigation)
{
    m_navigation = navigation;
}

/**
 * @brief Getter for navigation history list elements.
 * @details Allows to access the current history element from the linked list.
 *
 * @return NavigationHistoryElement or false if not available.
 */
NavigationHistoryElement * NavigationHistory::navigationHistoryElement()
{
    return m_navigationHistory;
}

/**
 * @brief Saves a new history element to the list.
 * @details Verifies if this is not the same view matrix and field of view, adds
 *          a new element to the linked history list and triggers an
 *          onHistoryChanged() signal to update the GUI.
 *
 * @param viewmatrix The view matrix to save.
 * @param fovy The field of view to save.
 * @param thumbnail A snapshot of the canvas to save.
 */
void NavigationHistory::save(const glm::mat4 & viewmatrix, const float & fovy, const QImage & thumbnail)
{
    if (m_navigationHistory != nullptr && (!m_navigationHistory->isEqualViewMatrix(viewmatrix) || !m_navigationHistory->isEqualFovy(fovy)))
    {
        m_navigationHistory = new NavigationHistoryElement(m_navigationHistory, viewmatrix, fovy, thumbnail);
        onHistoryChanged();
    }
}

/**
 * @brief Goes one step back in navigation history.
 * @details Goes to the previous history element in list, loads the previous
 *          view and triggers an onHistoryChanged() signal to update the GUI
 *          until the first (oldest) item in history is reached.
 */
void NavigationHistory::undo()
{
    if(isEmpty() || m_navigationHistory->isFirst())
    {
        return;
    }
    else
    {
        m_navigationHistory = m_navigationHistory->previous();
        m_navigation->loadView(m_navigationHistory->viewMatrix(), m_navigationHistory->fovy(), false);
        onHistoryChanged();
    }
}

/**
 * @brief Goes one step forward in navigation history.
 * @details Goes to the next history element in list, loads the next view and
 *          triggers an onHistoryChanged() signal to update the GUI until the
 *          last (latest) item in history is reached.
 */
void NavigationHistory::redo()
{
    if(isEmpty() || m_navigationHistory->isLast())
    {
        return;
    }
    else
    {
        m_navigationHistory = m_navigationHistory->next();
        m_navigation->loadView(m_navigationHistory->viewMatrix(), m_navigationHistory->fovy(), false);
        onHistoryChanged();
    }
}

/**
 * @brief Resets the navigation history elements.
 * @details Unlinks and deletes all history elements in the list, creates a new
 *          item and resets size to 1.
 */
void NavigationHistory::reset()
{
    if(isEmpty())
        return;

    NavigationHistoryElement * historyHandle {m_navigationHistory->last()};
    NavigationHistoryElement * prev;

    /// Saves one state with new object to history tail
    m_navigation->onViewChanged();
    NavigationHistoryElement * newHandle {m_navigationHistory->last()};

    while(historyHandle->previous() != nullptr)
    {
        prev = historyHandle->previous();
        delete historyHandle;
        historyHandle = prev;
    }

    m_navigationHistory = newHandle;
    m_navigationHistory->setPrevious(nullptr);
    m_navigationHistory->m_length = 1;
    onHistoryChanged();
}

/**
 * @brief Checks if history is empty.
 * @details Checks if items are already stored to the element list.
 * @return True if at least one history element exists, or false if not.
 */
bool NavigationHistory::isEmpty()
{
    return m_navigationHistory == nullptr;
}

/**
 * @brief Triggers history changed signal.
 * @details Triggers the history changed signal which can be used to update
 *          the GUI for example.
 */
void NavigationHistory::onHistoryChanged()
{
    historyChanged();
}
