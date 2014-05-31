#include <core/navigation/navigationhistory.h>
#include <core/navigation/navigationhistoryelement.h>

#include <signalzeug/Signal.h>
#include <QImage>
#include <QDebug>

NavigationHistory::NavigationHistory()
    : m_navigation(nullptr)
    , m_navigationhistory(nullptr)
{   }

NavigationHistory::~NavigationHistory()
{
    m_navigationhistory->reset();
}

void NavigationHistory::setNavigation(AbstractNavigation * navigation)
{
    m_navigation = navigation;
}

NavigationHistoryElement * NavigationHistory::navigationhistory()
{
    if (!this->isEmpty())
        return m_navigationhistory;
    return false;
}


void NavigationHistory::save(glm::mat4 viewmatrix, float fovy, QImage thumbnail)
{
   if(!m_navigationhistory->isEqualViewMatrix(viewmatrix) || !m_navigationhistory->isEqualFovy(fovy))
   {
        m_navigationhistory = new NavigationHistoryElement(m_navigationhistory, viewmatrix, fovy, thumbnail);
        onHistoryChanged();
        qDebug() << "save #" <<  m_navigationhistory->getTimestamp() << " / " << m_navigationhistory->getSize() << " views in history.";
   }
}

void NavigationHistory::undo()
{
    // if not reached the oldest history element
    if(!m_navigationhistory->isFirst() && !isEmpty())
    {
        if (m_navigationhistory->isLast())
        {
            // save last object before undo
            m_navigation->onViewChanged();
        }
        m_navigationhistory = m_navigationhistory->getPrevious();
        m_navigation->loadView(m_navigationhistory->getViewMatrix(), m_navigationhistory->getFovy(), false);
        qDebug() << "go back to #" << m_navigationhistory->getTimestamp() << " / " << m_navigationhistory->getSize() << " views in history.";
    }
}

void NavigationHistory::redo()
{
    // if not reached the youngest history element
    if(!m_navigationhistory->isLast() && !isEmpty())
    {
        m_navigationhistory = m_navigationhistory->getNext();
        m_navigation->loadView(m_navigationhistory->getViewMatrix(), m_navigationhistory->getFovy(), false);
        qDebug() << "redo #" << m_navigationhistory->getTimestamp() << " / " << m_navigationhistory->getSize() << " views in history.";
    }
}

bool NavigationHistory::isEmpty()
{
    if(m_navigationhistory == nullptr)
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
