/// Include header file.
#include <core/navigation/navigationhistoryelement.h>

/// Include QImage and QSize for thumbnails.
#include <QImage>
#include <QSize>

/// Include QDateTime for timestamps.
#include <QDateTime>

/// Include glm for matrices.
#include <glm/glm.hpp>

/// Property for setting the maximum number of stored history elements.
const int NavigationHistoryElement::MAX_LENGTH = 16;

/// Property for setting the thumbnail size.
const int NavigationHistoryElement::THUMBNAIL_SIZE = 128;

/// Member variable holding the current size of the history element list.
int NavigationHistoryElement::m_length = 0;

/**
 * @brief Constructor for NavigationHistoryElement class.
 * @details NavigationHistoryElement is the class for holding all data and
 *          properties for the single navigation history objects. Each element
 *          has pointers to a previous and a next element, creating a linked
 *          list of history items.
 *
 * @param previous The previous NavigationHistoryElement in history.
 * @param viewmatrix The current view matrix.
 * @param fovy The current field of view.
 * @param thumbnail The current snapshot of the canvas.
 */
NavigationHistoryElement::NavigationHistoryElement(NavigationHistoryElement * previous, glm::mat4 viewmatrix, float fovy, QImage thumbnail)
    : m_previous(previous)
    , m_next(nullptr)
    , m_viewmatrix(viewmatrix)
    , m_fovy(fovy)
    , m_thumbnail(thumbnail)
{
    /// Tags each element with a unique timestamp.
    m_timestamp = QDateTime::currentMSecsSinceEpoch();

    /// Increases history list length.
    m_length++;

    /// Scales thumbnail to defined size.
    m_thumbnail = m_thumbnail.scaled(QSize(THUMBNAIL_SIZE, THUMBNAIL_SIZE), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    /// Links elements if previous item in history exists.
    if (previous != nullptr)
    {

        /// Handles orphaned history branching, deletes unused elements.
        if (previous->next() != nullptr && previous->next() != previous)
            deleteOrphaned();

        previous->setNext(this);
    }

    /// Limits history to defined size.
    if (m_length > MAX_LENGTH)
        deleteFirst();
}

/**
 * @brief Destructor for NavigationHistoryElement class.
 */
NavigationHistoryElement::~NavigationHistoryElement() {}

/**
 * @brief Setter for previous element in history list.
 * @details Allows to link a new item before this element in the list.
 *
 * @param previous The previous element in list.
 */
void NavigationHistoryElement::setPrevious(NavigationHistoryElement * previous)
{
    m_previous = previous;
}

/**
 * @brief Setter for next element in history list.
 * @details Allows to link a new item after this element in the list.
 *
 * @param next The next element in list.
 */
void NavigationHistoryElement::setNext(NavigationHistoryElement * next)
{
    m_next = next;
}

/**
 * @brief Getter for previous element in history list.
 * @details Allows to access the item before this element in the list.
 * @return Previous item in history list.
 */
NavigationHistoryElement * NavigationHistoryElement::previous()
{
        return m_previous;
}


/**
 * @brief Getter for next element in history list.
 * @details Allows to access the item after this element in the list.
 * @return Next item in history list.
 */
NavigationHistoryElement * NavigationHistoryElement::next()
{
        return m_next;
}

/**
 * @brief Getter for first element in history list.
 * @details Allows to access the first item in this linked list.
 * @return The first item in the history list.
 */
NavigationHistoryElement * NavigationHistoryElement::first()
{
    NavigationHistoryElement * temp {this};
    while(!temp->isFirst())
    {
        temp = temp->previous();
    }
    return temp;
}

/**
 * @brief Getter for last element in history list.
 * @details Allows to access the last item in this linked list.
 * @return The last item in the history list.
 */
NavigationHistoryElement * NavigationHistoryElement::last()
{
    NavigationHistoryElement * temp {this};
    while(!temp->isLast())
    {
        temp = temp->next();
    }
    return temp;
}

/**
 * @brief Getter for the view matrix of the current history element.
 * @details Allows to access the view matrix of the current history element.
 * @return The view matrix of the current history element.
 */
glm::mat4 NavigationHistoryElement::viewMatrix()
{
    return m_viewmatrix;
}

/**
 * @brief Getter for the field of view of the current history element.
 * @details Allows to access the field of view of the current history element.
 * @return The field of view of the current history element.
 */
float NavigationHistoryElement::fovy()
{
    return m_fovy;
}

/**
 * @brief Getter for the thumbnail of the current history element.
 * @details Allows to access the preview of the current history element.
 * @return The thumbnail of the current history element.
 */
QImage NavigationHistoryElement::thumbnail()
{
    return m_thumbnail;
}

/**
 * @brief Getter for the timestamp of the current history element.
 * @details Allows to access the timestamp of the current history element.
 * @return The timestamp of the current history element.
 */
qint64 NavigationHistoryElement::timestamp()
{
    return m_timestamp;
}

/**
 * @brief Getter for the size of the current history list.
 * @details Allows to access the size of the current history list.
 * @return The size of the current history list.
 */
int NavigationHistoryElement::size()
{
    return m_length;
}

/**
 * @brief Checks if element is first.
 * @details Checks if this element is the first in the history list.
 * @return True if it is first, else false.
 */
bool NavigationHistoryElement::isFirst()
{
    return (m_previous == nullptr);
}

/**
 * @brief Checks if element is last.
 * @details Checks if this element is the last in the history list.
 * @return True if it is last, else false.
 */
bool NavigationHistoryElement::isLast()
{
    return (m_next == nullptr);
}

/**
 * @brief Compares the view matrices.
 * @details Compares the given view matrix with the matrix of this element.
 *
 * @param viewmatrix A view matrix to compare with.
 * @return True if they are equal, else false.
 */
bool NavigationHistoryElement::isEqualViewMatrix(const glm::mat4 & viewmatrix)
{
    bool isEqual = false;
    (m_length != 0) ? isEqual = (viewmatrix == m_viewmatrix) : isEqual = false;
    return isEqual;
}

/**
 * @brief Compares the field of views.
 * @details Compares the given field of view with the fovy of this element.
 *
 * @param fovy A field of view to compare with.
 * @return True if they are equal, else false.
 */
bool NavigationHistoryElement::isEqualFovy(const float & fovy)
{
    bool isEqual = false;
    (m_length != 0) ? isEqual = (fovy == m_fovy) : isEqual = false;
    return isEqual;
}

/**
 * @brief Handles orphaned history branching.
 * @details Handles orphaned history branching and deletes unused elements.
 *          For example if you go back in history for 3 steps and save a new
 *          element, this will take care of the orphaned history elements in the
 *          unused branch.
 */
void NavigationHistoryElement::deleteOrphaned()
{
    NavigationHistoryElement * temp {m_previous->last()};
    while(temp != m_previous)
    {
        temp = temp->previous();
        temp->setNext(nullptr);
        --m_length;
    }
}

/**
 * @brief Deletes first item in list.
 * @details Deletes first item in history elements list. To be used to manage
 *          the maximum list size or to reduce the history size.
 */
void NavigationHistoryElement::deleteFirst()
{
    NavigationHistoryElement * new_first {this->first()->next()};
    new_first->setPrevious(nullptr);
    --m_length;
}
