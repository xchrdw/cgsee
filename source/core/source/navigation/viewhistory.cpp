#include <core/navigation/viewhistory.h>
#include <core/navigation/viewhistoryelement.h>

#include <signalzeug/Signal.h>
#include <QImage>
#include <QDebug>

ViewHistory::ViewHistory(AbstractNavigation * navigation)
    : m_navigation(navigation)
    , m_viewhistory(nullptr)
{   }

ViewHistory::~ViewHistory(){
    m_viewhistory->reset();
}

void ViewHistory::save(glm::mat4 viewmatrix, float fovy, QImage thumbnail)
{
   if(!m_viewhistory->isEqualViewMatrix(viewmatrix) || !m_viewhistory->isEqualFovy(fovy)){
        m_viewhistory = new ViewHistoryElement(m_viewhistory, viewmatrix, fovy);
        qDebug() << "save #" <<  m_viewhistory->getTimestamp() << " / " << m_viewhistory->getSize() << " views in history.";
   }
}

void ViewHistory::undo()
{
    // if not reached the oldest history element
    if(!m_viewhistory->isFirst()){
        if (m_viewhistory->isLast()) {
            // save last object before undo
            m_navigation->saveView();
        }
        m_viewhistory = m_viewhistory->getPrevious();
        m_navigation->loadView(m_viewhistory->getViewMatrix(),m_viewhistory->getFovy(),false);
        qDebug() << "go back to #" << m_viewhistory->getTimestamp() << " / " << m_viewhistory->getSize() << " views in history.";
    }
}

void ViewHistory::redo()
{
    // if not reached the youngest history element
    if(!m_viewhistory->isLast()){
        m_viewhistory = m_viewhistory->getNext();
        m_navigation->loadView(m_viewhistory->getViewMatrix(),m_viewhistory->getFovy(),false);
        qDebug() << "redo #" << m_viewhistory->getTimestamp() << " / " << m_viewhistory->getSize() << " views in history.";
    }
}


