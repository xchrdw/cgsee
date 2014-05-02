
#include <core/datacore/typefunctions.h>

#include <cassert>

#include <QMetaType>


AbstractInPlaceTypeFunctions::~AbstractInPlaceTypeFunctions()
{

}

QtTypeFunctions::QtTypeFunctions(const char * qTypeName)
{
    m_typeId = QMetaType::type(qTypeName);
    assert(m_typeId != 0);
}

void QtTypeFunctions::construct(void * place) const
{
    assert(m_typeId != 0);
    QMetaType::construct(m_typeId, place, nullptr);
}

void QtTypeFunctions::destruct(void * place) const
{
    assert(m_typeId != 0);
    QMetaType::destruct(m_typeId, place);
}

void QtTypeFunctions::copyTo(const void * from, void * to) const
{
    assert(m_typeId != 0);
    QMetaType::construct(m_typeId, to, from);
}
