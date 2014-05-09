#include <core/navigation/viewhistory.h>

int ViewHistory::m_size = 0;

ViewHistory::ViewHistory(ViewHistory* previous, glm::mat4 viewmatrix, float fovy)
    : m_previous(previous)
    , m_viewmatrix(viewmatrix)
    , m_fovy(fovy)
    , m_next(nullptr)
{

    // @TODO debug remove id or replace with timestamp
    m_id = m_size;
    m_size++;

    // limit history size to 128
    if (m_size > 127) {
        deleteFirst();
    }

    // if previos object in history exists, link it
    if (previous != nullptr)
    {

        // if new history shall be saved before youngest state is the
        // the current view: delete orphaned history path (create new path)
        if (previous->getNext() != nullptr)
        {
            deleteOrphaned();
        }

        // connect previous node with current
        previous->setNext(this);
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
        return this; // @TODO this should be NULL if isFirst(); ?
        //return nullptr;
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
        return this; // @TODO this should be NULL if isLast(); ?
        //return nullptr;
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

int ViewHistory::getId()
{
    return m_id; // @TODO debg remove or replace with timestamp
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

bool ViewHistory::isFirst()
{
    if (m_previous == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ViewHistory::isLast()
{
    if (m_next == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ViewHistory::isEqualTo(const glm::mat4 & viewmatrix)
{
    // @TODO compare to ViewHistory objects
    if (m_size != 0)
    {
        if (viewmatrix == m_viewmatrix)
        {
            return true;
        }
    }
    return false;
}

void ViewHistory::deleteOrphaned()
{
    ViewHistory* temp {m_previous->getLast()};
    while(temp != m_previous)
    {
        temp = temp->getPrevious();
        delete temp->getNext();
    }
}

void ViewHistory::deleteFirst()
{
    ViewHistory* first {this->getFirst()};
    ViewHistory* new_first {this->getFirst()->getNext()};
    new_first->setPrevious(nullptr);
    delete first;
    m_size--;
}
