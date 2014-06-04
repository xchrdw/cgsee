#include <core/navigation/navigationhistory.h>
#include <core/navigation/navigationhistoryelement.h>

#include <signalzeug/Signal.h>
#include <QImage>
#include <QDebug>

NavigationHistory::NavigationHistory()
    : m_navigation(nullptr)
    , m_navigationHistory(nullptr)
{   }

NavigationHistory::~NavigationHistory()
{
    m_navigationHistory->reset();
}

void NavigationHistory::setNavigation(AbstractNavigation * navigation)
{
    m_navigation = navigation;
}

NavigationHistoryElement * NavigationHistory::navigationHistory()
{
    if (!this->isEmpty())
        return m_navigationHistory;
    return false;
}


void NavigationHistory::save(glm::mat4 viewmatrix, float fovy, QImage thumbnail)
{
   if(!m_navigationHistory->isEqualViewMatrix(viewmatrix) || !m_navigationHistory->isEqualFovy(fovy))
   {
        m_navigationHistory = new NavigationHistoryElement(m_navigationHistory, viewmatrix, fovy, thumbnail);
        onHistoryChanged();
        qDebug() << "save #" <<  m_navigationHistory->getTimestamp() << " / " << m_navigationHistory->getSize() << " views in history.";
   }
}

void NavigationHistory::undo()
{
    // if not reached the oldest history element
    if(!m_navigationHistory->isFirst() && !isEmpty())
    {
        if (m_navigationHistory->isLast())
        {
            // save last object before undo
            m_navigation->onViewChanged();
        }
        m_navigationHistory = m_navigationHistory->getPrevious();
        m_navigation->loadView(m_navigationHistory->getViewMatrix(), m_navigationHistory->getFovy(), false);
        onHistoryChanged();
        qDebug() << "go back to #" << m_navigationHistory->getTimestamp() << " / " << m_navigationHistory->getSize() << " views in history.";
    }
}

void NavigationHistory::redo()
{
    // if not reached the youngest history element
    if(!m_navigationHistory->isLast() && !isEmpty())
    {
        m_navigationHistory = m_navigationHistory->getNext();
        m_navigation->loadView(m_navigationHistory->getViewMatrix(), m_navigationHistory->getFovy(), false);
        onHistoryChanged();
        qDebug() << "redo #" << m_navigationHistory->getTimestamp() << " / " << m_navigationHistory->getSize() << " views in history.";
    }
}

bool NavigationHistory::isEmpty()
{
    if(m_navigationHistory == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void NavigationHistory::onHistoryChanged()
{
       historyChanged();
}
