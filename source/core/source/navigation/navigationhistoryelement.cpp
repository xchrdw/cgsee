#include <core/navigation/navigationhistoryelement.h>
#include <QImage>
#include <QSize>
#include <QDir>

// @TODO these config values should be added to cgsee properties
int NavigationHistoryElement::m_maxLength = 16;
int NavigationHistoryElement::m_thumbnailSize = 128;
// ---------->>

int NavigationHistoryElement::m_length = 0;

NavigationHistoryElement::NavigationHistoryElement(NavigationHistoryElement* previous, glm::mat4 viewmatrix, float fovy, QImage thumbnail)
    : m_previous(previous)
    , m_next(nullptr)
    , m_viewmatrix(viewmatrix)
    , m_fovy(fovy)
    , m_thumbnail(thumbnail)
{
    m_timestamp = QDateTime::currentMSecsSinceEpoch();
    m_length++;

    m_thumbnail = m_thumbnail.scaled(QSize(m_thumbnailSize,m_thumbnailSize), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // if previous object in history exists, link it
    if (previous != nullptr)
    {
        // if new history shall be saved before youngest state is the
        // the current view: delete orphaned history path (create new path)
        if (previous->getNext() != nullptr && previous->getNext() != previous)
        {
            deleteOrphaned();
        }
        // connect previous node with current
        previous->setNext(this);
    }

    // limit history size to m_maxLength
    if (m_length > m_maxLength)
    {
        deleteFirst();
    }
}

NavigationHistoryElement::~NavigationHistoryElement()
{
    // remove this object from the list
    NavigationHistoryElement* previous {this->getPrevious()};
    NavigationHistoryElement* next {this->getNext()};
    this->getNext()->setPrevious(previous);
    this->getPrevious()->setNext(next);
    m_length--;
}

void NavigationHistoryElement::reset()
{
    // remove the entire history
    NavigationHistoryElement* last {this->getLast()};
    NavigationHistoryElement* first {this->getFirst()};
    while(last != first)
    {
        last = last->getPrevious();
        delete last->getNext();
    }
    delete first;
    m_length = 0;
}

void NavigationHistoryElement::setNext(NavigationHistoryElement* next)
{
    m_next = next;
}

void NavigationHistoryElement::setPrevious(NavigationHistoryElement* previous)
{
    m_previous = previous;
}

NavigationHistoryElement* NavigationHistoryElement::getPrevious()
{
    if (isFirst())
    {
        return this;
    }
    else
    {
        return m_previous;
    }
}

NavigationHistoryElement* NavigationHistoryElement::getNext()
{
    if (isLast())
    {
        return this;
    }
    else
    {
        return m_next;
    }
}

NavigationHistoryElement* NavigationHistoryElement::getLast()
{
    NavigationHistoryElement* temp {this};
    while(!temp->isLast())
    {
        temp = temp->getNext();
    }
    return temp;
}

NavigationHistoryElement* NavigationHistoryElement::getFirst()
{
    NavigationHistoryElement* temp {this};
    while(!temp->isFirst())
    {
        temp = temp->getPrevious();
    }
    return temp;
}

qint64 NavigationHistoryElement::getTimestamp()
{
    return m_timestamp;
}

int NavigationHistoryElement::getSize()
{
    return m_length;
}

glm::mat4 NavigationHistoryElement::getViewMatrix()
{
    return m_viewmatrix;
}

float NavigationHistoryElement::getFovy()
{
    return m_fovy;
}

QImage NavigationHistoryElement::getThumbnail()
{
    return m_thumbnail;
}

bool NavigationHistoryElement::isFirst()
{
    return (m_previous == nullptr);
}

bool NavigationHistoryElement::isLast()
{
    return (m_next == nullptr);
}

bool NavigationHistoryElement::isEqualViewMatrix(const glm::mat4 & viewmatrix)
{
    bool isEqual = false;
    (m_length != 0) ? isEqual = (viewmatrix == m_viewmatrix) : isEqual = false;
    return isEqual;
}

bool NavigationHistoryElement::isEqualFovy(const float & fovy)
{
    bool isEqual = false;
    (m_length != 0) ? isEqual = (fovy == m_fovy) : isEqual = false;
    return isEqual;
}

void NavigationHistoryElement::deleteOrphaned()
{
    NavigationHistoryElement* temp {m_previous->getLast()};
    while(temp != m_previous)
    {
        temp = temp->getPrevious();
        temp->setNext(nullptr);
        --m_length;
    }
}

void NavigationHistoryElement::deleteFirst()
{
    NavigationHistoryElement* new_first {this->getFirst()->getNext()};
    new_first->setPrevious(nullptr);
    --m_length;
}
