#include <core/navigation/viewhistoryelement.h>

int ViewHistoryElement::m_size = 0;

ViewHistoryElement::ViewHistoryElement(ViewHistoryElement* previous, glm::mat4 viewmatrix, float fovy)
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
    if (m_size > 16) {
        deleteFirst();
    }
}

ViewHistoryElement::~ViewHistoryElement()
{
    // remove this object from the list
    ViewHistoryElement* previous {this->getPrevious()};
    ViewHistoryElement* next {this->getNext()};
    this->getNext()->setPrevious(previous);
    this->getPrevious()->setNext(next);
    m_size--;
}

void ViewHistoryElement::reset()
{
    // remove the entire history
    ViewHistoryElement* last {this->getLast()};
    ViewHistoryElement* first {this->getFirst()};
    while(last != first)
    {
        last = last->getPrevious();
        delete last->getNext();
    }
    delete first;
    m_size = 0;
}

void ViewHistoryElement::setNext(ViewHistoryElement* next)
{
    m_next = next;
}

void ViewHistoryElement::setPrevious(ViewHistoryElement* previous)
{
    m_previous = previous;
}

ViewHistoryElement* ViewHistoryElement::getPrevious()
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

ViewHistoryElement* ViewHistoryElement::getNext()
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

ViewHistoryElement* ViewHistoryElement::getLast()
{
    ViewHistoryElement* temp {this};
    while(!temp->isLast())
    {
        temp = temp->getNext();
    }
    return temp;
}

ViewHistoryElement* ViewHistoryElement::getFirst()
{
    ViewHistoryElement* temp {this};
    while(!temp->isFirst())
    {
        temp = temp->getPrevious();
    }
    return temp;
}

qint64 ViewHistoryElement::getTimestamp()
{
    return m_timestamp;
}

int ViewHistoryElement::getSize()
{
    return m_size;
}

glm::mat4 ViewHistoryElement::getViewMatrix()
{
    return m_viewmatrix;
}

float ViewHistoryElement::getFovy()
{
    return m_fovy;
}

QImage ViewHistoryElement::getThumbnail()
{
    return m_thumbnail;
}

bool ViewHistoryElement::isFirst()
{
    return (m_previous == nullptr);
}

bool ViewHistoryElement::isLast()
{
    return (m_next == nullptr);
}

bool ViewHistoryElement::isEqualViewMatrix(const glm::mat4 & viewmatrix)
{
    bool isEqual = false;
    (m_size != 0) ? isEqual = (viewmatrix == m_viewmatrix) : isEqual = false;
    return isEqual;
}

bool ViewHistoryElement::isEqualFovy(const float & fovy)
{
    bool isEqual = false;
    (m_size != 0) ? isEqual = (fovy == m_fovy) : isEqual = false;
    return isEqual;
}

void ViewHistoryElement::deleteOrphaned()
{
    ViewHistoryElement* temp {m_previous->getLast()};
    while(temp != m_previous)
    {
        temp = temp->getPrevious();
        temp->setNext(nullptr);
        --m_size;
    }
}

void ViewHistoryElement::deleteFirst()
{
    ViewHistoryElement* new_first {this->getFirst()->getNext()};
    new_first->setPrevious(nullptr);
    --m_size;
}
