#include <core/navigation/viewhistory.h>

int ViewHistory::m_size = 0;

ViewHistory::ViewHistory(ViewHistory* previous, glm::mat4 viewmatrix, float fovy)
    : m_previous(previous)
    , m_next(nullptr)
    , m_viewmatrix(viewmatrix)
    , m_fovy(fovy)
    , m_thumbnail(64, 64, QImage::Format_RGB32)
{
    m_timestamp = QDateTime::currentMSecsSinceEpoch();
    m_size++;

    // @TODO get canvas snapshot
    m_thumbnail.fill(Qt::white);

    // if previos object in history exists, link it
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

    // limit history size to 8
    if (m_size > 8) {
        deleteFirst();
    }
}

ViewHistory::~ViewHistory()
{
    // remove this object from the list
    ViewHistory* previous {this->getPrevious()};
    ViewHistory* next {this->getNext()};
    this->getNext()->setPrevious(previous);
    this->getPrevious()->setNext(next);
    m_size--;
}

void ViewHistory::reset()
{
    // remove the entire history
    ViewHistory* last {this->getLast()};
    ViewHistory* first {this->getFirst()};
    while(last != first)
    {
        last = last->getPrevious();
        delete last->getNext();
    }
    delete first;
    m_size = 0;
}

void ViewHistory::setNext(ViewHistory* next)
{
    m_next = next;
}

void ViewHistory::setPrevious(ViewHistory* previous)
{
    m_previous = previous;
}

ViewHistory* ViewHistory::getPrevious()
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

ViewHistory* ViewHistory::getNext()
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

ViewHistory* ViewHistory::getLast()
{
    ViewHistory* temp {this};
    while(!temp->isLast())
    {
        temp = temp->getNext();
    }
    return temp;
}

ViewHistory* ViewHistory::getFirst()
{
    ViewHistory* temp {this};
    while(!temp->isFirst())
    {
        temp = temp->getPrevious();
    }
    return temp;
}

qint64 ViewHistory::getTimestamp()
{
    return m_timestamp;
}

int ViewHistory::getSize()
{
    return m_size;
}

glm::mat4 ViewHistory::getViewMatrix()
{
    return m_viewmatrix;
}

float ViewHistory::getFovy()
{
    return m_fovy;
}

QImage ViewHistory::getThumbnail()
{
    return m_thumbnail;
}

bool ViewHistory::isFirst()
{
    return (m_previous == nullptr);
}

bool ViewHistory::isLast()
{
    return (m_next == nullptr);
}

bool ViewHistory::isEqualViewMatrix(const glm::mat4 & viewmatrix)
{
    bool isEqual = false;
    (m_size != 0) ? isEqual = (viewmatrix == m_viewmatrix) : isEqual = false;
    return isEqual;
}

bool ViewHistory::isEqualFovy(const float & fovy)
{
    bool isEqual = false;
    (m_size != 0) ? isEqual = (fovy == m_fovy) : isEqual = false;
    return isEqual;
}

void ViewHistory::deleteOrphaned()
{
    ViewHistory* temp {m_previous->getLast()};
    while(temp != m_previous)
    {
        temp = temp->getPrevious();
        temp->setNext(nullptr);
        --m_size;
    }
}

void ViewHistory::deleteFirst()
{
    ViewHistory* new_first {this->getFirst()->getNext()};
    new_first->setPrevious(nullptr);
    --m_size;
}
